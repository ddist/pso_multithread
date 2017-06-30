#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>

#define NO_FILE "The instance file can't be opened"
#define MISFORMATTED "The instance file doesn't have the right format"
#define MEMALLOC_ERROR "Memory allocation did not succeded"

void throw_error(int, const char*);

double norm1(double, double, double, double);

#endif