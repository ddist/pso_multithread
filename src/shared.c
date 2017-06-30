#include "shared.h"

void throw_error(int err_code, const char* err_msg) {
	fputs(err_msg, stderr);
	fputs("\n", stderr);
	exit(err_code);
}

double norm1(double x1, double x2, double y1, double y2) {
	return sqrt( pow(x1 - y1, 2) + pow(x2 - y2, 2));
}

unsigned int create_route(pso_route** route) {
	*route = malloc(sizeof(pso_route));
	if(!(*route)) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);
	(*route)->buffer = malloc(ROUTE_MIN_CAPACITY * sizeof (unsigned int));
	if(!(*route)->buffer) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

	(*route)->capacity = ROUTE_MIN_CAPACITY;
	(*route)->buffer[0] = 0; (*route)->buffer[1] = 0;
	(*route)->size = 2;
	return (*route)->size;
}

unsigned int add_to_route(unsigned int task, pso_route* route) {
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

void destroy_route(pso_route* route) {
	free(route->buffer);
	free(route);
}