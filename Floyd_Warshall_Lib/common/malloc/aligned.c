//TODO Delete common folder
/*
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

// MODIFICACIÓN
#include <malloc.h>

//Public
void* abs_malloc(int64_t vectorLength){
#ifdef INTEL_ARC
    return _mm_malloc(vectorLength, MEM_ALIGN);
#else
    return aligned_alloc(MEM_ALIGN, vectorLength);
#endif
}

//Public
void abs_free(void* vec){
#ifdef INTEL_ARC
    _mm_free(vec);
#else
    free(vec);
#endif
}
 */