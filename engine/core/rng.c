#include 'rng.h'
#include <stdio.h>
#include <stdint.h>

#include <time.h>

static uint64_t rng_state = 0x4d595df4d0f33175ULL; 
static uint64_t rng_inc   = 0x0595e9900ec0b323ULL;
