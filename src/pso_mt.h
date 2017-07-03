#ifndef _PSO_MT_H_
#define _PSO_MT_H_

#include "shared.h"
#include "instance.h"
#include "thread_pool.h"

#define MAX_URGENCY 1000.0
#define MIN_FDR -9999999.9

typedef struct {
	pso_params_t params;
	ftsp_instance_t inst;
	pso_particle_t *swarm;
	double *best_u, best_score;
	double **cij;
	pthread_mutex_t global_mutex;
} _pso;
typedef _pso* pso_t;

typedef struct _pso_frame {
	unsigned int i, j;
	pso_t pso;
} pso_frame_t;
typedef pso_frame_t* pso_frame_p;

void create_pso(ftsp_instance_t, pso_params_t, pso_t*);

void initialize_pso(pso_t);

double compute_urgency(int, int, int, ftsp_instance_t, double**, double*);

double compute_distance(double*, double*, unsigned int);

double evaluate_routes(ftsp_instance_t, pso_route_t*, double*);

void update_particle_velocity(pso_particle_t, unsigned int, pso_t);

void update_particle_position(pso_particle_t, pso_t);

void decode(pso_particle_t, pso_route_t*, double*, pso_t);

void compute_frame(pso_frame_p);

double start(pso_t);

void destroy_pso(pso_t);

#endif