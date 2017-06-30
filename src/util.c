#include "util.h"

void throw_error(int err_code, const char* err_msg) {
	fputs(err_msg, stderr);
	fputs("\n", stderr);
	exit(err_code);
}

double norm1(double x1, double x2, double y1, double y2) {
	return sqrt( pow(x1 - y1, 2) + pow(x2 - y2, 2));
}