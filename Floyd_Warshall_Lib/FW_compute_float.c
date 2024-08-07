#include "FW_compute.h"

//------------------------------------------------------------------------- parallel Floyd-Warshall Algorithm Implementation ----------------------------------------------------------------------------

static inline void FW_BLOCK_PARALLEL(float *const graph, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int no_path) __attribute__((always_inline));
static inline void FW_BLOCK(float *const graph, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int no_path) __attribute__((always_inline));
#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

void compute_FW_float_parallel(FW_Matrix FW, int threads_num, int no_path)
{
    uint64_t r, row_of_blocks_disp, num_of_bock_elems;
    r = FW.norm_size / BLOCK_SIZE;
    row_of_blocks_disp = FW.norm_size * BLOCK_SIZE;
    num_of_bock_elems = BLOCK_SIZE * BLOCK_SIZE;
    float *D = (float *)FW.dist;
    int *P = FW.path;

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

#pragma omp parallel shared(S, no_path) default(none) firstprivate(r, row_of_blocks_disp, num_of_bock_elems, D, P) num_threads(threads_num)
    {
        uint64_t i, j, k, b, kj, ik, kk, ij, k_row_disp, k_col_disp, i_row_disp, j_col_disp, w;

        // Variable agregada
        uint64_t aux;


        for (k = 0; k < r; k++)
        {
            b = k * BLOCK_SIZE;
            k_row_disp = k * row_of_blocks_disp;
            k_col_disp = k * num_of_bock_elems;

            // Phase 1
            kk = k_row_disp + k_col_disp;
            FW_BLOCK_PARALLEL(D, kk, kk, kk, P, b, no_path);

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
                    FW_BLOCK(D, kj, kk, kj, P, b, no_path);

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
                    FW_BLOCK(D, ik, ik, kk, P, b, no_path);

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
                    FW_BLOCK(D, ij, ik, kj, P, b, no_path);
                }
            }
        }
    }

    // Liberación de memoria reservada
    for (x = 0; x < r; x++)
        free(S[x]);
    free(S);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK(float *const graph, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int no_path)
{

    uint64_t i, j, k, i_disp, i_disp_d1, k_disp, k_disp_d3;
    float dij, dik, dkj, sum; // Segun Datatype

    for (k = 0; k < BLOCK_SIZE; k++)
    {
        k_disp = k * BLOCK_SIZE;
        k_disp_d3 = k_disp + d3;
        for (i = 0; i < BLOCK_SIZE; i += 2)
        {
            i_disp = i * BLOCK_SIZE;
            i_disp_d1 = i_disp + d1;
            dik = graph[i_disp + d2 + k];
			#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BLOCK_SIZE; j++)
            {
                dij = graph[i_disp_d1 + j];
                dkj = graph[k_disp_d3 + j];
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph[i_disp_d1 + j] = sum;
                   if (no_path == 0)
                    {
                       path[i_disp_d1 + j] = base + k;
                   }
                }
            }
            i_disp = (i + 1) * BLOCK_SIZE;
            i_disp_d1 = i_disp + d1;
            dik = graph[i_disp + d2 + k];
			#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BLOCK_SIZE; j++)
            {
                dij = graph[i_disp_d1 + j];
                dkj = graph[k_disp_d3 + j];
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph[i_disp_d1 + j] = sum;
                   if (no_path == 0)
                    {
                       path[i_disp_d1 + j] = base + k;
                   }
                }
            }
        }
    }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK_PARALLEL(float *const graph, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int no_path)
{
    uint64_t i, j, k, i_disp, i_disp_d1, k_disp, k_disp_d3;
    float dij, dik, dkj, sum; // Segun Datatype

    for (k = 0; k < BLOCK_SIZE; k++)
    {
        k_disp = k * BLOCK_SIZE;
        k_disp_d3 = k_disp + d3;
		#pragma omp for
        for (i = 0; i < BLOCK_SIZE; i += 2)
        {
            i_disp = i * BLOCK_SIZE;
            i_disp_d1 = i_disp + d1;
            dik = graph[i_disp + d2 + k];
			#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BLOCK_SIZE; j++)
            {
                dij = graph[i_disp_d1 + j];
                dkj = graph[k_disp_d3 + j];
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph[i_disp_d1 + j] = sum;
                   if (no_path == 0)
                    {
                       path[i_disp_d1 + j] = base + k;
                   }
                }
            }
            i_disp = (i + 1) * BLOCK_SIZE;
            i_disp_d1 = i_disp + d1;
            dik = graph[i_disp + d2 + k];
			#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BLOCK_SIZE; j++)
            {
                dij = graph[i_disp_d1 + j];
                dkj = graph[k_disp_d3 + j];
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph[i_disp_d1 + j] = sum;
                    if (no_path == 0)
                    {
                       path[i_disp_d1 + j] = base + k;
                   }
                }
            }
        }
    }
}

// ------------------------------------------------------------------ Sequentiall Floyd-Warshall Algorithm Implementation ------------------------------------------------------------------------------------------------

static inline void FW_BLOCK_SEQ(float *const graph, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int no_path) __attribute__((always_inline));

#pragma GCC diagnostic ignored "-Wunused-parameter"
void compute_FW_float_sequential(FW_Matrix FW, int no_path)
{
    uint64_t i, j, k, r, b, kj, ik, kk, ij, row_of_blocks_disp, block_size, k_row_disp, k_col_disp, i_row_disp, j_col_disp;
    int n = FW.norm_size;

    r = n / BLOCK_SIZE;
    row_of_blocks_disp = n * BLOCK_SIZE;
    block_size = BLOCK_SIZE * BLOCK_SIZE;

    for (k = 0; k < r; k++)
    {
        b = k * BLOCK_SIZE;
        k_row_disp = k * row_of_blocks_disp;
        k_col_disp = k * block_size;

        // Phase 1
        kk = k_row_disp + k_col_disp;
        FW_BLOCK_SEQ((float *)FW.dist, kk, kk, kk, FW.path, b, no_path);

        // Phase 2
        for (j = 0; j < r; j++)
        {
            if (j == k)
                continue;
            kj = k_row_disp + j * block_size;
            FW_BLOCK_SEQ((float *)FW.dist, kj, kk, kj, FW.path, b, no_path);
        }

        // Phase 3
        for (i = 0; i < r; i++)
        {
            if (i == k)
                continue;
            ik = i * row_of_blocks_disp + k_col_disp;
            FW_BLOCK_SEQ((float *)FW.dist, ik, ik, kk, FW.path, b, no_path);
        }

        // Phase 4
        for (i = 0; i < r; i++)
        {
            if (i == k)
                continue;
            i_row_disp = i * row_of_blocks_disp;
            ik = i_row_disp + k_col_disp;
            for (j = 0; j < r; j++)
            {
                if (j == k)
                    continue;
                j_col_disp = j * block_size;
                kj = k_row_disp + j_col_disp;
                ij = i_row_disp + j_col_disp;
                FW_BLOCK_SEQ((float *)FW.dist, ij, ik, kj, FW.path, b, no_path);
            }
        }
    }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK_SEQ(float *const graph, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int no_path)
{
    uint64_t i, j, k, i_disp, i_disp_d1, k_disp, k_disp_d3;
    float dij, dik, dkj, sum;

    for (k = 0; k < BLOCK_SIZE; k++)
    {
        k_disp = k * BLOCK_SIZE;
        k_disp_d3 = k_disp + d3;
        for (i = 0; i < BLOCK_SIZE; i += 2)
        {
            i_disp = i * BLOCK_SIZE;
            i_disp_d1 = i_disp + d1;
            dik = graph[i_disp + d2 + k];
			#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BLOCK_SIZE; j++)
            {
                dij = graph[i_disp_d1 + j];
                dkj = graph[k_disp_d3 + j];
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph[i_disp_d1 + j] = sum;
                    if (no_path == 0)
                    {
                       path[i_disp_d1 + j] = base + k;
                    }
                }
            }

            i_disp = (i + 1) * BLOCK_SIZE;
            i_disp_d1 = i_disp + d1;
            dik = graph[i_disp + d2 + k];
			#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BLOCK_SIZE; j++)
            {
                dij = graph[i_disp_d1 + j];
                dkj = graph[k_disp_d3 + j];
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph[i_disp_d1 + j] = sum;
                   if (no_path == 0)
                    {
                       path[i_disp_d1 + j] = base + k;
                   }
                }
            }
        }
    }
}
