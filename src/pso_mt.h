#ifndef _PSO_MT_H_
#define _PSO_MT_H_

#include "shared.h"
#include "instance.h"

#define MAX_URGENCY 1000.0
#define MIN_FDR -9999999.9

typedef struct {
	pso_params_t params;
	ftsp_instance_t inst;
	pso_particle_t *swarm;
	double *best_u, best_score;
	double **cij;
} _pso;

typedef _pso* pso_t;

void create_pso(ftsp_instance_t, pso_params_t, pso_t*);

void initialize_pso(pso_t);

double compute_urgency(int, int, int, ftsp_instance_t, double**, double*);

double compute_distance(double*, double*, unsigned int);

double evaluate_routes(ftsp_instance_t, pso_route_t*, double*);

void update_particle_velocity(pso_particle_t, unsigned int, pso_t);

void update_particle_position(pso_particle_t, pso_t);

void decode(pso_particle_t, pso_route_t*, double*, pso_t);

double start(pso_t);

void destroy_pso(pso_t);

#endif