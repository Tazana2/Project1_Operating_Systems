#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include "process.h"

int load_process(FILE* file, process_t processes[], int max_processes);

#endif
