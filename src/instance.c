#include "instance.h"

void initialize_instance(const char* file, Instance* inst) {
	FILE* fp;
	char* line = NULL;
	size_t len = 0;
	ssize_t read;
	char* tok = NULL;

	fp = fopen(file, "r");
	if(fp == NULL)
		throw_error(EXIT_FAILURE, NO_FILE);

	getline(&line, &len, fp);
	read = getline(&line, &len, fp);
	if(read == -1) throw_error(EXIT_FAILURE, MISFORMATTED);
	inst->n = atoi(line);
	inst->N = inst->n+1;

	read = getline(&line, &len, fp);
	if(read == -1) throw_error(EXIT_FAILURE, MISFORMATTED);
	inst->m = atoi(line);

	getline(&line, &len, fp);
	getline(&line, &len, fp);

	inst->w = malloc(inst->N * sizeof (unsigned int));
	if(!inst->w) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

	for(int i=0; i < inst->N; i++) {
		read = getline(&line, &len, fp);
		if(read == -1) throw_error(EXIT_FAILURE, MISFORMATTED);
		inst->w[i] = atoi(line);
	}

	getline(&line, &len, fp);

	inst->s = malloc(inst->N * sizeof (unsigned int*));
	if(!inst->s) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

	for(int i=0; i < inst->N; i++) {
		read = getline(&line, &len, fp);
		if(read == -1) throw_error(EXIT_FAILURE, MISFORMATTED);

		inst->s[i] = malloc(inst->m * sizeof (unsigned int));
		if(!inst->s[i]) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

		tok = strtok(line, "\t");
		inst->s[i][0] = atoi(tok);
		for(int j=1; j < inst->m; j++) {
			tok = strtok(NULL, "\t");
			inst->s[i][j] = atoi(tok);
		}
	}

	getline(&line, &len, fp);
	getline(&line, &len, fp);

	inst->p = malloc(inst->N * sizeof (unsigned int));
	if(!inst->p) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

	read = getline(&line, &len, fp);
	if(read == -1) throw_error(EXIT_FAILURE, MISFORMATTED);

	tok = strtok(line, "\t");
	inst->p[0] = atoi(tok);
	for(int i=1; i < inst->N ; i++) {
		tok = strtok(NULL, "\t");
		inst->p[i] = atoi(tok);
	}

	inst->e = malloc(inst->N * sizeof (unsigned int));
	if(!inst->e) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

	read = getline(&line, &len, fp);
	if(read == -1) throw_error(EXIT_FAILURE, MISFORMATTED);

	tok = strtok(line, "\t");
	inst->e[0] = atoi(tok);
	for(int i=1; i < inst->N ; i++) {
		tok = strtok(NULL, "\t");
		inst->e[i] = atoi(tok);
	}

	inst->l = malloc(inst->N * sizeof (unsigned int));
	if(!inst->l) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

	read = getline(&line, &len, fp);
	if(read == -1) throw_error(EXIT_FAILURE, MISFORMATTED);

	tok = strtok(line, "\t");
	inst->l[0] = atoi(tok);
	for(int i=1; i < inst->N ; i++) {
		tok = strtok(NULL, "\t");
		inst->l[i] = atoi(tok);
	}

	getline(&line, &len, fp);
	getline(&line, &len, fp);

	inst->a = malloc(inst->m * sizeof (unsigned int));
	if(!inst->a) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

	read = getline(&line, &len, fp);
	if(read == -1) throw_error(EXIT_FAILURE, MISFORMATTED);

	tok = strtok(line, "\t");
	inst->a[0] = atoi(tok);
	for(int i=1; i < inst->m ; i++) {
		tok = strtok(NULL, "\t");
		inst->a[i] = atoi(tok);
	}

	inst->b = malloc(inst->m * sizeof (unsigned int));
	if(!inst->b) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

	read = getline(&line, &len, fp);
	if(read == -1) throw_error(EXIT_FAILURE, MISFORMATTED);

	tok = strtok(line, "\t");
	inst->b[0] = atoi(tok);
	for(int i=1; i < inst->m ; i++) {
		tok = strtok(NULL, "\t");
		inst->b[i] = atoi(tok);
	}

	getline(&line, &len, fp);
	getline(&line, &len, fp);

	read = getline(&line, &len, fp);
	if(read == -1) throw_error(EXIT_FAILURE, MISFORMATTED);
	inst->M = atoi(line);

	read = getline(&line, &len, fp);
	if(read == -1) throw_error(EXIT_FAILURE, MISFORMATTED);
	inst->MW = atoi(line);

	read = getline(&line, &len, fp);
	if(read == -1) throw_error(EXIT_FAILURE, MISFORMATTED);
	inst->MZ = atoi(line);

	getline(&line, &len, fp);
	getline(&line, &len, fp);

	inst->c = malloc(inst->N * sizeof (double*));
	if(!inst->c) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

	for(int i=0; i < inst->N; i++) {
		read = getline(&line, &len, fp);
		if(read == -1) throw_error(EXIT_FAILURE, MISFORMATTED);

		inst->c[i] = malloc(2 * sizeof (double));
		if(!inst->c[i]) throw_error(EXIT_FAILURE, MEMALLOC_ERROR);

		tok = strtok(line, "\t");
		inst->c[i][0] = atof(tok);
		tok = strtok(NULL, "\t");
		inst->c[i][1] = atof(tok);
	}

	fclose(fp);

	if(line)
		free(line);
}