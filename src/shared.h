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

double rand_uniform(double, double);

typedef struct {
	float cp, cg, cl, cn, uq, u_max, u_min, v_max, v_min, w_max, w_min;
	unsigned int I, seed, max_iter;
} pso_params;

#define ROUTE_MIN_CAPACITY 10

typedef struct {
	unsigned int *buffer;
	size_t size, capacity;
} pso_route;
typedef pso_route* pso_route_t;

size_t create_route(pso_route_t*);

size_t add_to_route(unsigned int, pso_route_t);

void destroy_route(pso_route_t);

typedef struct {
	double *u, *v;
	double *best_u;
	double best_score;
	size_t size;
} pso_particle;
typedef pso_particle* pso_particle_t;

void create_particle(unsigned int, pso_particle_t*);

void initialize_particle(double, double, double, double, pso_particle_t);

void destroy_particle(pso_particle_t);

#endif