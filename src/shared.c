#include "shared.h"

void throw_error(int err_code, const char* err_msg) {
	fputs("\n", stderr);
	fputs(err_msg, stderr);
	fputs("\n", stderr);
	exit(err_code);
}

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

double norm(double x1, double x2, double y1, double y2) {
	return sqrt( pow(x1 - y1, 2) + pow(x2 - y2, 2));
}

double rand_uniform(double min, double max) { 
   return min + (rand()/(double)RAND_MAX) * (max-min); 
}

double max(double a, double b) {
	if(a < b) return b;
	return a;
}

double min(double a, double b) {
	if(a < b) return a;
	return b;
}

size_t create_route(pso_route_t* route) {
	*route = malloc(sizeof(pso_route));
	if(!(*route)) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
	(*route)->buffer = calloc(ROUTE_MIN_CAPACITY, sizeof (unsigned int));
	if(!(*route)->buffer) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

	(*route)->capacity = ROUTE_MIN_CAPACITY;
	(*route)->size = 2;
	return (*route)->size;
}

size_t add_to_route(unsigned int task, pso_route_t route) {
	if(route->size == route->capacity) {
		route->buffer = realloc(route->buffer, (route->capacity + ROUTE_MIN_CAPACITY) * sizeof (unsigned int));
		if(!route->buffer) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
		route->capacity += ROUTE_MIN_CAPACITY;
	}
	route->buffer[route->size-1] = task;
	route->buffer[route->size] = 0;
	route->size++;
	return route->size-2;
}

unsigned int at(int i, pso_route_t route) {
	return route->buffer[i%route->size];
}

void destroy_route(pso_route_t route) {
	free(route->buffer);
	free(route);
}

void create_particle(unsigned int s, pso_particle_t* p) {
	*p = malloc(sizeof(pso_particle));
	if(!(*p)) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
	(*p)->size = s;
	(*p)->u = malloc(s * sizeof (double));
	if(!(*p)->u) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
	(*p)->v = malloc(s * sizeof (double));
	if(!(*p)->v) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
	(*p)->best_u = malloc(s * sizeof (double));
	if(!(*p)->best_u) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
}

void initialize_particle(int i, double u_max, double u_min, double v_max, double v_min, pso_particle_t p) {
	p->best_score = 0.0;
	p->id = i;
	for(size_t i=0; i < p->size;i++) {
		p->u[i] = rand_uniform(u_min, u_max);
		p->v[i] = rand_uniform(v_min, v_max);
		p->best_u[i] = p->u[i];
	}
	pthread_mutex_init(&(p->particle_mutex), NULL);
}

void destroy_particle(pso_particle_t p) {
	free(p->u);
	free(p->v);
	free(p->best_u);
	free(p);
}

void compute_distances(unsigned int N, double **c, double **cij) {
	for (int i=0; i < N; i++) {
		for(int j=0; j < N; j++) {
			if(i == j) {
				cij[i][j] = 0.0;
			} else if(i > j) {
				cij[i][j] = cij[j][i];
			} else {
				cij[i][j] = norm(c[i][0], c[i][1], c[j][0], c[j][1]);
			}
		}
	}
}

void compute_coverage(unsigned int m, unsigned int n, double* u, unsigned int** s, unsigned int* w, double** c, unsigned int** cov) {
	for(int i=0; i < m; i++) {
		for(int j=0; j < n;j++) {
			if(s[j+1][i]) {
				if(norm(u[3*i], u[3*i+1], c[j+1][0], c[j+1][1]) <= fabs(u[3*i+2])) {
					cov[i][j] = w[j+1];
				}
			}
		}
	}
}