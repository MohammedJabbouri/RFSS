#ifndef ENGINE_CORE_RNG_H
#define ENGINE_CORE_RNG_H


#include <stdint.h>

typedef struct {
    uint64_t state;
    uint64_t inc;
} rng_state_t;

// seeds a stream and the stream picks PCG32 so it is independent and stuff, 
void rng_seed(rng_state_t *rng, uint64_t seed, uint64_t stream);

uint32_t rng_next_u32(rng_state_t *rng);

float rng_uniform(rng_state_t *rng);

#endif