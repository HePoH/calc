#ifndef CMPLX_NUM_H
#define CMPLX_NUM_H

#define _DEFAULT_SOURCE
#define BUF_SIZE 255

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <dlfcn.h>

typedef struct cmplx {
	double a;
	double b;
} cmplx_num;

typedef cmplx_num* (*plgn_f)(cmplx_num*, cmplx_num*);

typedef struct pi {
	char* plgn_name;
	char* main_func;
	char* menu_title;

	plgn_f pf;
} PLUGIN_INFO;

#endif
