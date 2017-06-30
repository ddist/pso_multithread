#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

typedef struct {
	unsigned int n,m,N, MZ, MW, M;
	unsigned int **s;
	unsigned int *w,*p,*e,*l,*a,*b;
	double **c;
} Instance;

void initialize_instance(const char*, Instance*);

#endif