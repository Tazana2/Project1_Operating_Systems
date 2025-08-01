#ifndef PLANNER_H
#define PLANNER_H
#include "process.h"

void exec_instruction(process_t* p, const char* instr);
void run_round_robin(process_t processes[], int num_processes);

#endif

