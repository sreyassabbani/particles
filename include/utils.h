#ifndef HELPERS_H
#define HELPERS_H

#include <stdint.h>

void rng_seed(uint32_t seed);

/* returns float in [0,1) */
float randf_unit(void);

/* returns float in [a,b) */
float randf_range(float a, float b);

#endif /* HELPERS_H */
