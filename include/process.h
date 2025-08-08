#include <stdio.h>
#ifndef PROCESS_H
#define PROCESS_H
#define MAX_INSTR 20
#define MAX_REPEATED_JUMPS 2

// Basic representation of a process in the simulation
// We only keep a few integer registers and a tiny instruction list
typedef struct {
    int pid;                // Process ID
    int pc;                 // Program Counter (index into instructions array)
    int ax, bx, cx;         // Simulated CPU registers
    int quantum;            // Time slice assigned to this process
    char status[10];        // Text status: "Ready", "Executing", "Finished"
    
    int last_jump;          // Last jump destination (for loop detection)
    int repeated_jumps;     // How many times we jumped to the same line in a row
    int num_instructions;   // Loaded instruction count
    char instructions[MAX_INSTR][20]; // Fixed-size array of instruction strings
} process_t;

#endif