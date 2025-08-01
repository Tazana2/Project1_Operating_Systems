#include <stdio.h>
#define MAX_INSTR 20

typedef struct {
    int pid;
    int pc; // Simulated program counter
    int ax, bx, cx; // Simulated registers
    int quantum;
    char status[10]; // "Ready", "Executing", "Finished"
    
    int num_instructions;
    char intructions[MAX_INSTR][20]
} process_t;