#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include "process.h"

/*
    * Function: load_processes
    * See src/loader.c for detailed documentation.
*/
int load_processes(const char* filename, process_t processes[], int max_processes);

#endif