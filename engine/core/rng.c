#include 'rng.h'
#include <stdio.h>
#include <stdint.h>

#include <time.h>

static uint64_t rng_state = 0x4d595df4d0f33175ULL; 
static uint64_t rng_inc   = 0x0595e9900ec0b323ULL;

void rng_seed(rng_state_t *rng, uint64_t seed, uint64_t stream) {
    rng->state = 0U;
    rng->inc   = (stream << 1u) | 1u;
    rng_next_u32(rng);
    rng->state += seed;
    rng_next_u32(rng);
}

uint32_t rng_next_u32(rng_state_t *rng) {
    uint64_t oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;

    uint32_t xorshifted = (uint32_t)(((oldstate >> 18u) ^ oldstate) >> 27u);
    uint32_t rot        = (uint32_t)(oldstate >> 59u);
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31u));
}

float rng_uniform(rng_state_t *rng) {
    uint32_t r = rng_next_u32(rng);
    return (float)(r >> 8) / (float)(1u << 24);
}