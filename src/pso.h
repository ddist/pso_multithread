#ifndef _PSO_H_
#define _PSO_H_

#include "shared.h"
#include "instance.h"

typedef struct {
	pso_params_t params;
	ftsp_instance_t inst;
	pso_particle_t *swarm;
	double *best_u, best_score;
} _pso;

typedef _pso* pso_t;

void create_pso(ftsp_instance_t, pso_params_t, pso_t*);

void initialize_pso(pso_t);

void destroy_pso(pso_t);

#endif