#include "shared.h"

void throw_error(int err_code, const char* err_msg) {
	fputs(err_msg, stderr);
	fputs("\n", stderr);
	exit(err_code);
}

double norm1(double x1, double x2, double y1, double y2) {
	return sqrt( pow(x1 - y1, 2) + pow(x2 - y2, 2));
}

double rand_uniform(double min, double max) { 
   return min + (rand()/(double)RAND_MAX) * (max-min); 
}

size_t create_route(pso_route_t* route) {
	*route = malloc(sizeof(pso_route));
	if(!(*route)) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
	(*route)->buffer = malloc(ROUTE_MIN_CAPACITY * sizeof (unsigned int));
	if(!(*route)->buffer) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

	(*route)->capacity = ROUTE_MIN_CAPACITY;
	(*route)->buffer[0] = 0; (*route)->buffer[1] = 0;
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

void initialize_particle(double u_max, double u_min, double v_max, double v_min, pso_particle_t p) {
	p->best_score = 0.0;
	
	for(size_t i=0; i < p->size;i++) {
		p->u[i] = rand_uniform(u_min, u_max);
		p->v[i] = rand_uniform(v_min, v_max);
		p->best_u[i] = p->u[i];
	}
}

void destroy_particle(pso_particle_t p) {
	free(p->u);
	free(p->v);
	free(p->best_u);
	free(p);
}