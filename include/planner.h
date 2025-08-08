#ifndef PLANNER_H
#define PLANNER_H
#include "process.h"

/*
    * Function: exec_instruction
    * See src/planner.c for detailed documentation.
*/
void exec_instruction(process_t* p, const char* instr);

/*
    * Function: run_round_robin
    * See src/planner.c for detailed documentation.
*/
void run_round_robin(process_t processes[], int num_processes);

#endif