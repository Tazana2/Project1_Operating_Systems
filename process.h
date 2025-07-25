#include <stdio.h>

typedef struct {
    int pid;
    int pc; // Simulated program counter
    int ax, bx, cx; // Simulated registers
    int quantum;
    char status[10]; // "Ready", "Executing", "Finished"
} process_t;