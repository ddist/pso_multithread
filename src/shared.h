#ifndef _SHARED_H_
#define _SHARED_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NO_FILE "The instance file can't be opened"
#define MISFORMATTED "The instance file doesn't have the right format"
#define MEMALLOC_ERROR "Memory allocation did not succeded"

void throw_error(int, const char*);

double norm1(double, double, double, double);

typedef struct {
	float cp, cg, cl, cn, uq, u_max, u_min, v_max, v_min, w_max, w_min;
	unsigned int I, seed, max_iter;
} pso_params;


#define ROUTE_MIN_CAPACITY 10

typedef struct {
	unsigned int *buffer;
	unsigned int size, capacity;
} pso_route;

unsigned int create_route(pso_route**);

unsigned int add_to_route(unsigned int, pso_route*);

void destroy_route(pso_route*);

#endif