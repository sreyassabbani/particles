#include "utils.h"

#include <stdint.h>

static uint32_t rng_state = 2463534242u; // default

void rng_seed(uint32_t seed) {
    if (seed == 0) seed = 1; // avoid zero state
    rng_state = seed;
}

static uint32_t rng_u32() {
    // xorshift32
    uint32_t x = rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rng_state = x;
    return x;
}

/* float in [0,1) */
float randf_unit() {
    return (float)(rng_u32() / 4294967296.0); // 2^32
}

/* float in [a,b) */
float randf_range(float a, float b) {
    return a + (b - a) * randf_unit();
}
