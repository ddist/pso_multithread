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

	(*pso)->cij = malloc(inst->N * sizeof(double*));
	if(!(*pso)->cij) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
	for(int i=0; i < inst->N; i++) {
		(*pso)->cij[i] = calloc(inst->N, sizeof(double));
		if(!(*pso)->cij[i]) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
	}
}

void initialize_pso(pso_t pso) {
	srand(pso->params->seed);
	pso->best_score = 0.0;
	for(int i=0; i < pso->params->I; i++) {
		initialize_particle(i, pso->params->u_max, pso->params->u_min, pso->params->v_max, pso->params->v_min, pso->swarm[i]);
	}
	compute_distances(pso->inst->N, pso->inst->c, pso->cij);
}

double compute_urgency(int i, int j, int p, ftsp_instance_t inst, double **cij, double *tk) {
	double u = min((double)inst->l[j], (double)inst->b[i] - cij[j][0]) - (double)inst->p[j] - cij[p][j] - tk[i];
	return u/((double)inst->p[j]);
}

double compute_distance(double* x, double* y, unsigned int size) {
	double sum = 0.0;
	for(int i=0; i < size; i++) {
		sum += (x[i] - y[i]);
	}
	return fabs(sum);
}

double evaluate_routes(ftsp_instance_t inst, pso_route_t* routes, double *zk) {
	double w = 0.0, z = 0.0;
	for(int i=0; i < inst->m;i++) {
		if(routes[i]->size < 3) {
			w = 0.0; z = 0.0; break;
		}

		for(int j=0; j < routes[i]->size; j++) {
			w += (double)inst->w[at(j, routes[i])];
		}
		z += zk[i];
	}
	return w/(double)inst->MW + z/(double)inst->MZ;
}

void update_particle_velocity(pso_particle_t p, unsigned int i, pso_t pso) {
	double *ring_best = p->u, *fdr_best = p->u, score = p->best_score;

	if(pso->swarm[(p->id-1)%pso->params->I]->best_score > score) {
		score = pso->swarm[(p->id-1)%pso->params->I]->best_score;
		ring_best = pso->swarm[(p->id-1)%pso->params->I]->u;
	}
	if(pso->swarm[(p->id+1)%pso->params->I]->best_score > score) {
		score = pso->swarm[(p->id+1)%pso->params->I]->best_score;
		ring_best = pso->swarm[(p->id+1)%pso->params->I]->u;
	}

	double fdr, best_fdr = MIN_FDR;

	for(int i=0; i < pso->params->I; i++){
		if(p->id == i) continue;
		double distance = compute_distance(pso->swarm[i]->u, p->u, p->size);
		fdr = (pso->swarm[i]->best_score - p->best_score)/distance;
		if(fdr > best_fdr) {
			best_fdr = fdr;
			fdr_best = pso->swarm[i]->u;
		}
	}

	double w = pso->params->w_max - i*(pso->params->w_max - pso->params->w_min)/pso->params->max_iter;

	for(int i=0; i < p->size; i++) {
		p->v[i] = p->v[i]*w + pso->params->cp*rand_uniform(0.0, 1.0)*(p->best_u[i] - p->u[i]);
		p->v[i] +=  pso->params->cg*rand_uniform(0.0, 1.0)*(pso->best_u[i] - p->u[i]);
		p->v[i] += pso->params->cl*rand_uniform(0.0, 1.0)*(ring_best[i] - p->u[i]);
		p->v[i] += pso->params->cn*rand_uniform(0.0, 1.0)*(fdr_best[i] - p->u[i]);
		if(p->v[i] > pso->params->v_max) p->v[i] = pso->params->v_max;
		if(p->v[i] < pso->params->v_min) p->v[i] = pso->params->v_min;
	}
}

void update_particle_position(pso_particle_t p, pso_t pso) {
	for(int i=0; i < p->size;i++) {
		p->u[i] += p->v[i];
		if(p->u[i] > pso->params->u_max) p->u[i] = pso->params->u_max;
		if(p->u[i] < pso->params->u_min) p->u[i] = pso->params->u_min;
	}
}

void decode(pso_particle_t p, pso_route_t *routes, double *zk, pso_t pso) {
	unsigned int **cov = malloc(pso->inst->m * sizeof(unsigned int*));
	if(!cov) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
	for(int i=0; i < pso->inst->m;i++) {
		cov[i] = calloc(pso->inst->n, sizeof(unsigned int));
		if(!cov[i]) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
	}
	compute_coverage(pso->inst->m, pso->inst->n, p->u, pso->inst->s, pso->inst->w, pso->inst->c, cov);

	double *tk = calloc(pso->inst->m, sizeof(double));
	unsigned int *tech_status = calloc(pso->inst->m, sizeof(unsigned int));
	unsigned int *job_status = malloc(pso->inst->N * sizeof(unsigned int));
	memcpy(job_status, pso->inst->w, pso->inst->N * sizeof(unsigned int));
	if(!tk || !tech_status || !job_status) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

	int techs_finished = 0, jobs_finished = 0;
	bool impossible = false, fixing = false;

	do {
		for(int i=0; i < pso->inst->m; i++) {
			if(tech_status[i]) continue;
			if(tk[i] == 0.0) tk[i] = (double)pso->inst->a[i];
			unsigned int prev_job = at(routes[i]->size - 2, routes[i]), best_i = 0, best_l = pso->inst->l[0];
			double best_u = MAX_URGENCY;

			for(int j=0; j < pso->inst->n;j++) {
				if(cov[i][j] == 0 || job_status[j+1] == 0) continue;
				if((double)pso->inst->l[j+1] < tk[i] + pso->cij[prev_job][j+1] + (double)pso->inst->p[j+1]) {
					cov[i][j] = 0; 
					continue;
				}
				if(max(tk[i] + pso->cij[prev_job][j+1], (double)pso->inst->e[j+1]) > (double)pso->inst->b[i] - pso->cij[j+1][0] - (double)pso->inst->p[j+1]) {
					cov[i][j] = 0; 
					continue;
				}
				double u = compute_urgency(i, j+1, prev_job, pso->inst, pso->cij, tk);
				//if(u<1.0) continue;
				if(pso->params->uq*u < best_u || pso->inst->e[j+1] < best_l) {
					best_u = u;
					best_l = pso->inst->e[j+1];
					best_i = (unsigned int)(j+1);
				}
			}
			if(best_u == MAX_URGENCY) {
				if(routes[i]->size > 2) {
					tech_status[i] = 1;
					techs_finished++;
				} else {
					if(fixing) {
						impossible = true; 
						break;
					}

					for(int k=0; k < pso->inst->n; k++) {
						cov[i][k] = job_status[k+1]*pso->inst->s[k+1][i];
					}
					fixing = true; i--;
				}
				continue;
			}
			add_to_route(best_i, routes[i]);
			tk[i] = max(tk[i] + pso->cij[prev_job][best_i], (double)pso->inst->e[best_i]) + (double)pso->inst->p[best_i];
			zk[i] = (double)pso->inst->b[i] - tk[i] - pso->cij[best_i][0];
			job_status[best_i] = 0; jobs_finished++; fixing = false;
		}
		if(impossible) break;
	} while (techs_finished < pso->inst->m);

	if(techs_finished == pso->inst->m && jobs_finished < pso->inst->n) {
		for(int j=1; j < pso->inst->N; j++) {
			if(job_status[j] == 0) continue;
			for(int i=0; i < pso->inst->m; i++) {
				if(pso->inst->s[j][i] == 0) continue;
				unsigned int prev_job = at(routes[i]->size-2, routes[i]);
				if((double)pso->inst->l[j] < tk[i] + pso->cij[prev_job][j] + (double)pso->inst->p[j]) continue;
				if(max(tk[i] + pso->cij[prev_job][j], (double)pso->inst->e[j]) > (double)pso->inst->b[i] - pso->cij[j][0] - (double)pso->inst->p[j]) continue;
				add_to_route(j, routes[i]);
				tk[i] = max(tk[i] + pso->cij[prev_job][j], (double)pso->inst->e[j]) + (double)pso->inst->p[j];
				zk[i] = (double)pso->inst->b[i] - tk[i] - pso->cij[j][0];
				job_status[j] = 0;
				break;
			}
			job_status[j] = 0;
		}
	}

	for(int i=0; i < pso->inst->m; i++) 
		free(cov[i]);
	free(tk);
	free(tech_status);
	free(job_status);
	free(cov);
}

double start(pso_t pso) {
	for(int i=0;i < pso->params->max_iter;i++) {
		for(int j=0; j < pso->params->I; j++) {
			pso_route_t routes[pso->inst->m];
			for(int k=0; k < pso->inst->m;k++) 
				create_route(&routes[k]);
			double *zk = calloc(pso->inst->m, sizeof(double));
			if(!zk) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

			decode(pso->swarm[j], routes, zk, pso);
			double score = evaluate_routes(pso->inst, routes, zk);
			
			if(pso->swarm[j]->best_score < score) {
				pso->swarm[j]->best_score = score;
				memcpy(pso->swarm[j]->best_u, pso->swarm[j]->u, pso->swarm[j]->size * sizeof(double));

				if(pso->best_score < score) {
					pso->best_score = score;
					memcpy(pso->best_u, pso->swarm[j]->u, pso->swarm[j]->size * sizeof(double));
				}
			}

			update_particle_velocity(pso->swarm[j], i, pso);
			update_particle_position(pso->swarm[j], pso);

			for(int k=0; k < pso->inst->m;k++)
				destroy_route(routes[k]);
			free(zk);
		}
	}
	return pso->best_score;
}

void destroy_pso(pso_t pso) {
	if(pso) {
		free(pso->best_u);
		for(int i=0; i < pso->params->I;i++) {
			destroy_particle(pso->swarm[i]);
		}
		for(int i=0; i < pso->inst->N;i++) {
			free(pso->cij[i]);
		}
		free(pso->cij);
		free(pso->swarm);
		free(pso);
	}
}