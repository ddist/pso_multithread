#include "pso.h"

void create_pso(ftsp_instance_t inst, pso_params_t params, pso_t* pso) {
	*pso = malloc(sizeof(_pso));
	if(!(*pso)) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
	(*pso)->inst = inst;
	(*pso)->params = params;

	(*pso)->swarm = malloc(params->I * sizeof(pso_particle_t));
	if(!(*pso)->swarm) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
	for(int i=0; i < params->I;i++) {
		create_particle(3*inst->m, &((*pso)->swarm[i]));
	}
	(*pso)->best_u = calloc(3*inst->m, sizeof(double));
	if(!(*pso)->best_u) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
}

void initialize_pso(pso_t pso) {
	pso->best_score = 0.0;
	for(int i=0; i < pso->params->I; i++) {
		initialize_particle(pso->params->u_max, pso->params->u_min, pso->params->v_max, pso->params->v_min, pso->swarm[i]);
	}
}

void destroy_pso(pso_t pso) {
	if(pso) {
		free(pso->best_u);
		for(int i=0; i < pso->params->I;i++) {
			destroy_particle(pso->swarm[i]);
		}
		free(pso->swarm);
		free(pso);
	}
}