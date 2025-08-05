#include <stdio.h>
#ifndef PROCESS_H
#define PROCESS_H
#define MAX_INSTR 20
#define MAX_REPEATED_JUMPS 2

typedef struct {
    int pid;
    int pc; // Simulated program counter
    int ax, bx, cx; // Simulated registers
    int quantum;
    char status[10]; // "Ready", "Executing", "Finished"
    
    int last_jump, repeated_jumps;
    int num_instructions;
    char instructions[MAX_INSTR][20];
} process_t;

#endif