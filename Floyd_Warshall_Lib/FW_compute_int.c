#include "FW_compute.h"
#include "../include/FW_Lib_CommonTypes.h"

#include <omp.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

//------------------------------------------------------------------------- Paralell Floyd-Warshall Algorithm Implementation ----------------------------------------------------------------------------


static inline void FW_BLOCK_PARALLEL(void *const graph, int BS, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int *const tmp1, int *const tmp2) __attribute__((always_inline));
static inline void FW_BLOCK(void *const graph, int BS, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int *const tmp1, int *const tmp2) __attribute__((always_inline));
#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)
#define NO_PATH


void compute_FW_int_paralell(FW_Matrix FW, int threads_num)
{
    uint64_t r, row_of_blocks_disp, num_of_bock_elems;
    r = FW.norm_size / FW.BS;
    row_of_blocks_disp = FW.norm_size * FW.BS;
    num_of_bock_elems = FW.BS * FW.BS;
    int  * D = (int *)FW.dist;
    int * P = FW.path;
    int BS = FW.BS;

    // --------------------------- BLOQUE AGREGADO -----------------------

    uint64_t x, y;
    sem_t **S;

    // Asignación de memoria
    S = (sem_t **)malloc(r * sizeof(sem_t *));
    for (x = 0; x < r; x++)
        S[x] = (sem_t *)malloc(r * sizeof(sem_t));

    for (x = 0; x < r; x++)
    {
        for (y = 0; y < r; y++)
        {
            sem_init(&S[x][y], 0, 0);
        }
    }

    // ------------------------- FIN BLOQUE AGREGADO -----------------------

    // Modificación: shared(pendientes, cv, mutex)

#pragma omp parallel shared(S) default(none) firstprivate(r, BS, row_of_blocks_disp, num_of_bock_elems, D, P) num_threads(threads_num)
    {
        uint64_t i, j, k, b, kj, ik, kk, ij, k_row_disp, k_col_disp, i_row_disp, j_col_disp, w;

        // Variable agregada
        uint64_t aux;

#ifndef NO_PATH
        int *tmp1 = NULL;
        int *tmp2 = NULL;
#else
        int tmp1[BS], tmp2[BS];
        // tmp1 and tmp2 are used as a patch to avoid a compiler bug which makes it lose performance instead of winning while omitting the compute of the P patrix
#endif

        // inicialización de semaforos
        // #pragma omp for collapse(2)

        for (k = 0; k < r; k++)
        {
            b = k * BS;
            k_row_disp = k * row_of_blocks_disp;
            k_col_disp = k * num_of_bock_elems;

            // Phase 1
            kk = k_row_disp + k_col_disp;
            FW_BLOCK_PARALLEL(D, BS, kk, kk, kk, P, b, tmp1, tmp2);

// Phase 2 y 3
#pragma omp for schedule(dynamic) nowait
            for (w = 0; w < r * 2; w++)
            {
                if (w < r)
                {
                    // Phase 2
                    j = w;
                    if (j == k)
                        continue;
                    kj = k_row_disp + j * num_of_bock_elems;
                    FW_BLOCK(D, BS, kj, kk, kj, P, b, tmp1, tmp2);

                    // -------------- BLOQUE AGREGADO -------------------

                    // Finalizó el computo del bloque (k,j) = (k,w)
                    // Modif: se debe decrementar pendientes de la columna actual "j"
                    for (aux = 0; aux < r; aux++)
                    {
                        if (aux == k)
                            continue; // se ignora actual (k,j)
                        sem_post(&S[aux][j]);
                    }

                    // -------------- FIN BLOQUE AGREGADO -----------------
                }
                else
                {
                    // Phase 3
                    i = w - r;
                    if (i == k)
                        continue;
                    ik = i * row_of_blocks_disp + k_col_disp;
                    FW_BLOCK(D, BS, ik, ik, kk, P, b, tmp1, tmp2);

                    // -------------- BLOQUE AGREGADO -------------------

                    // Finalizo el computo del bloque (i,k) = (w-r, k)
                    // Modif: se debe decrementar pendientes de la fila actual "i"
                    for (aux = 0; aux < r; aux++)
                    {
                        if (aux == k)
                            continue; // se ignora actual (i,k)
                        sem_post(&S[i][aux]);
                    }

                    // -------------- FIN BLOQUE AGREGADO -----------------
                }
            }

// Phase 4
#pragma omp for collapse(2) schedule(dynamic)
            for (i = 0; i < r; i++)
            {
                for (j = 0; j < r; j++)
                {
                    if ((j == k) || (i == k))
                        continue;

                    // ----------- BLOQUE AGREGADO -----------------

                    // Esperar que se computen los bloques (k,j) e (i,k)
                    sem_wait(&S[i][j]);
                    sem_wait(&S[i][j]);

                    // ---------- FIN BLOQUE AGREGADO --------------

                    i_row_disp = i * row_of_blocks_disp;
                    ik = i_row_disp + k_col_disp;
                    j_col_disp = j * num_of_bock_elems;
                    kj = k_row_disp + j_col_disp;
                    ij = i_row_disp + j_col_disp;
                    FW_BLOCK(D, BS, ij, ik, kj, P, b, tmp1, tmp2);
                }
            }
        }
    }

    // liberación de memoria reservada
    for (x = 0; x < r; x++)
        free(S[x]);
    free(S);
}


// Paralell algorithm implementation
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK(void *const graph, int BS, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int *const tmp1, int *const tmp2)
{
    // Casteo de graph a int
    int *graph_int = (int *)graph; // Segun Datatype

    uint64_t i, j, k, i_disp, i_disp_d1, k_disp, k_disp_d3;
    uint64_t dij, dik, dkj, sum; // Segun Datatype

    for (k = 0; k < BS; k++)
    {
        k_disp = k * BS;
        k_disp_d3 = k_disp + d3;
        for (i = 0; i < BS; i += 2)
        {
            i_disp = i * BS;
            i_disp_d1 = i_disp + d1;
            dik = graph_int[i_disp + d2 + k];
#ifdef INTEL_ARC
#pragma unroll(UNROLL_FACTOR)
#endif
#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BS; j++)
            {
                dij = graph_int[i_disp_d1 + j];
                dkj = graph_int[k_disp_d3 + j];
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph_int[i_disp_d1 + j] = sum;
#ifndef NO_PATH
                    path[i_disp_d1 + j] = base + k;
#else
                    tmp1[j] = tmp2[j];
#endif
                }
            }
            i_disp = (i + 1) * BS;
            i_disp_d1 = i_disp + d1;
            dik = graph_int[i_disp + d2 + k];
#ifdef INTEL_ARC
#pragma unroll(UNROLL_FACTOR)
#endif
#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BS; j++)
            {
                dij = graph_int[i_disp_d1 + j];
                dkj = graph_int[k_disp_d3 + j];
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph_int[i_disp_d1 + j] = sum;
#ifndef NO_PATH
                    path[i_disp_d1 + j] = base + k;
#else
                    tmp1[j] = tmp2[j];
#endif
                }
            }
        }
    }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK_PARALLEL(void *const graph, int BS, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int *const tmp1, int *const tmp2)
{
    // Casteo de graph a int
    int *graph_int = (int *)graph; // Segun Datatype

    uint64_t i, j, k, i_disp, i_disp_d1, k_disp, k_disp_d3;
    uint64_t dij, dik, dkj, sum; // Segun Datatype

    for (k = 0; k < BS; k++)
    {
        k_disp = k * BS;
        k_disp_d3 = k_disp + d3;
#pragma omp for
        for (i = 0; i < BS; i += 2)
        {
            i_disp = i * BS;
            i_disp_d1 = i_disp + d1;
            dik = graph_int[i_disp + d2 + k];
#ifdef INTEL_ARC
#pragma unroll(UNROLL_FACTOR)
#endif
#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BS; j++)
            {
                int test1 = graph_int[i_disp_d1 + j], test2 = graph_int[k_disp_d3 + j];
                dij = (uint64_t)test1;
                dkj = (uint64_t)test2;
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph_int[i_disp_d1 + j] = sum;
#ifndef NO_PATH
                    path[i_disp_d1 + j] = base + k;
#else
                    tmp1[j] = tmp2[j];
#endif
                }
            }
            i_disp = (i + 1) * BS;
            i_disp_d1 = i_disp + d1;
            dik = graph_int[i_disp + d2 + k];
#ifdef INTEL_ARC
#pragma unroll(UNROLL_FACTOR)
#endif
#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BS; j++)
            {
                dij = graph_int[i_disp_d1 + j];
                dkj = graph_int[k_disp_d3 + j];
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph_int[i_disp_d1 + j] = sum;
#ifndef NO_PATH
                    path[i_disp_d1 + j] = base + k;
#else
                    tmp1[j] = tmp2[j];
#endif
                }
            }
        }
    }
}

// ------------------------------------------------------------------ Sequentiall Floyd-Warshall Algorithm Implementation ------------------------------------------------------------------------------------------------

static inline void FW_BLOCK_SEQ(void* const graph, int BS,const uint64_t d1, const uint64_t d2, const uint64_t d3, int* const path, const uint64_t base) __attribute__((always_inline));

//Private
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK_SEQ(void* const graph, int BS, const uint64_t d1, const uint64_t d2, const uint64_t d3, int* const path, const uint64_t base){
	
    int *graph_int = (int *)graph; // Segun Datatype
    
    uint64_t i, j, k, i_disp, i_disp_d1, k_disp, k_disp_d3;
	uint64_t dij, dik, dkj, sum;

	for(k=0; k<BS; k++){
		k_disp = k*BS;
		k_disp_d3 = k_disp + d3;
		for(i=0; i<BS; i++){
			i_disp = i*BS;
			i_disp_d1 = i_disp + d1;
			dik = graph_int[i_disp + d2 + k];
			for(j=0; j<BS; j++){
				dij = graph_int[i_disp_d1 + j];
				dkj = graph_int[k_disp_d3 + j];
				sum = dik + dkj;
				if(sum < dij){
					graph_int[i_disp_d1 + j] = sum;
					#ifndef NO_PATH
						path[i_disp_d1 + j] = base + k;
					#endif
				}
			}
		}
	}
}

//Public
#pragma GCC diagnostic ignored "-Wunused-parameter"
void compute_FW_int_sequential(FW_Matrix FW){ 
	uint64_t i, j, k, r, b, kj, ik, kk, ij, row_of_blocks_disp, block_size, k_row_disp, k_col_disp, i_row_disp, j_col_disp;
	int BS = FW.BS;
	int n = FW.norm_size;

	r = n/BS;
	row_of_blocks_disp = n*BS;
	block_size = BS*BS;
	
	for(k=0; k<r; k++){
		b = k*BS;
		k_row_disp = k*row_of_blocks_disp;
		k_col_disp = k*block_size;
		
		//Phase 1
		kk = k_row_disp + k_col_disp;
		FW_BLOCK_SEQ(FW.dist, BS, kk, kk, kk, FW.path, b);
		
		//Phase 2
		for(j=0; j<r; j++){
			if(j == k)
		        continue;
			kj = k_row_disp + j*block_size;
			FW_BLOCK_SEQ(FW.dist, BS, kj, kk, kj, FW.path, b);
		}
		
		//Phase 3
		for(i=0; i<r; i++){
			if(i == k)
		        continue;
			ik = i*row_of_blocks_disp + k_col_disp;
			FW_BLOCK_SEQ(FW.dist, BS, ik, ik, kk, FW.path, b);
		}
		
		//Phase 4
		for(i=0; i<r; i++){
		    if(i == k)
				continue;
			i_row_disp = i*row_of_blocks_disp;
			ik = i_row_disp + k_col_disp;
			for(j=0; j<r; j++){
				if(j == k)
					continue;
				j_col_disp = j*block_size;
				kj = k_row_disp + j_col_disp;
				ij = i_row_disp + j_col_disp;
				FW_BLOCK_SEQ(FW.dist, BS, ij, ik, kj, FW.path, b);
			}
		}
	}
}