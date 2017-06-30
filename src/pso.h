#include "util.h"
#include "instance.h"

typedef struct {
	float cp, cg, cl, cn, uq, u_max, u_min, v_max, v_min, w_max, w_min;
	unsigned int I, seed, max_iter;
} PSOParams;

