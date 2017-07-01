#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shared.h"

typedef struct {
	unsigned int n,m,N, MZ, MW, M;
	unsigned int **s;
	unsigned int *w,*p,*e,*l,*a,*b;
	double **c;
} ftsp_instance;
typedef ftsp_instance* ftsp_instance_t;

void initialize_instance(const char*, ftsp_instance_t);

#endif