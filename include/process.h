#include <stdio.h>
#ifndef PROCESS_H
#define PROCESS_H
#define MAX_INSTR 20
#define MAX_REPEATED_JUMPS 2

/*
    * Struct: process_t
    * -----------------------------------------------------------------------------------
    * Represents a simulated process in the scheduler.
    * -----------------------------------------------------------------------------------
    * Members:
    *   pid               - Unique identifier for the process.
    *   pc                - Program counter, indicating the index of the next instruction to execute.
    *   ax, bx, cx        - Simulated CPU registers used for arithmetic and data storage.
    *   quantum           - Maximum number of instructions the process can execute before a context switch.
    *   status            - String describing the process state ("Ready", "Executing", or "Finished").
    *   last_jump         - Last jump instruction target executed (used for detecting infinite loops).
    *   repeated_jumps    - Counter for how many times the same jump has been repeated consecutively.
    *   num_instructions  - Total number of instructions loaded into the process.
    *   instructions      - Array holding the process's instructions as strings.
*/
typedef struct {
    int pid;
    int pc;
    int ax, bx, cx;
    int quantum;
    char status[10];
    
    int last_jump;          // Last jump destination (for loop detection)
    int repeated_jumps;     // How many times we jumped to the same line in a row
    int num_instructions;   // Loaded instruction count
    char instructions[MAX_INSTR][20]; // Fixed-size array of instruction strings
} process_t;

#endif