#include <time.h>

#ifdef MULTITHREADING
	#include "pso_mt.h"
#else
	#include "pso.h"
#endif

int main(int argc, char const *argv[])
{
	printf("-> Initializing...");

	clock_t start_t, end_t;
	pso_params params;
	ftsp_instance inst;

	params.multithreaded = false;
	int inst_type = 0;

	for(int i=1; i < argc; i++) {
		if(strcmp(argv[i], "-uMax") == 0) {params.u_max = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-uMin") == 0) {params.u_min = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-vMin") == 0) {params.v_min = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-vMax") == 0) {params.v_max = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-wMin") == 0) {params.w_min = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-wMax") == 0) {params.w_max = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-cp") == 0) {params.cp = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-cg") == 0) {params.cg = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-cn") == 0) {params.cn = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-cl") == 0) {params.cl = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-uq") == 0) {params.uq = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-seed") == 0) {params.seed = strtof(argv[i+1], NULL); i++; continue;};
		if(strcmp(argv[i], "-nParticles") == 0) {params.I = atoi(argv[i+1]); i++; continue;};
		if(strcmp(argv[i], "-maxIter") == 0) {params.max_iter = atoi(argv[i+1]); i++; continue;};
		if(strcmp(argv[i], "-inst") == 0) {initialize_instance(argv[i+1], &inst); i++; continue;};
		if(strcmp(argv[i], "-jobs") == 0) {params.multithreaded = true; params.thread_count = atoi(argv[i+1]); i++; continue;};
		if(strcmp(argv[i], "-type") == 0) {
			if(strcmp(argv[i+1], "RAD") == 0) {inst_type = 1;}; 
			i++; continue;
		}
	}

	if(inst_type == 1) {
		params.u_max = params.u_max/2.0;
		params.u_min = -params.u_max;
	}

	pso_t pso;
	create_pso(&inst, &params, &pso);
	initialize_pso(pso);

	printf("done.\n-> Optimizing...");
	fflush(stdout);

	start_t = clock();
	double score = start(pso);
	end_t = clock();

	double runtime = (double)(end_t-start_t)/CLOCKS_PER_SEC;

	printf("done.\n-> Best Score  = %lf\n-> Runtime = %lf\n", score, runtime);

	destroy_pso(pso);
	return EXIT_SUCCESS;
}