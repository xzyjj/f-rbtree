/* xstdlib.h - standard library definitions */

#ifndef XSTDLIB_H
#define XSTDLIB_H

#include <stdint.h>


/* @def: random */
#undef random_ctx
struct random_ctx {
	int32_t type;
	int32_t *state;
	int32_t *fptr;
	int32_t *bptr;
	int32_t *eptr;
};

/* LCG (Linear Congruential Generator) */
#undef RANDOM_TYPE0_NEW
#define RANDOM_TYPE0_NEW(name, seed) \
	int32_t name##_random_tab0[1] = { \
		seed \
		}; \
	struct random_ctx name = { \
		.type = 0, \
		.state = name##_random_tab0 \
		}
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* xstdlib_random.c */
extern int32_t xrandom_r(struct random_ctx *p, int32_t *v);

#ifdef __cplusplus
}
#endif


#endif
