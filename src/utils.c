#include <stdint.h>
#include <stdlib.h>

static const double SCALE_2POW32 = 4294967296.0; /* 2^32 */

/* float in [0,1) */
float randf_unit() {
    uint32_t v = arc4random();
    return (float)((double)v / SCALE_2POW32);
}

/* float in [a,b) */
float randf_range(float a, float b) {
    return a + (b - a) * randf_unit();
}
