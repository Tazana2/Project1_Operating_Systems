#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "planner.h"
#include "utils/logger.h"
#include "utils/colors.h"

/*
    * Function: get_register_ptr
    * -----------------------------------------------------------------------------------
    * Purpose:
    *   Returns a pointer to the requested CPU register of a given process.
    * -----------------------------------------------------------------------------------
    * Parameters:
    *   p        - Pointer to the process structure.
    *   reg_name - String representing the register name ("AX", "BX", "CX").
    * -----------------------------------------------------------------------------------
    * Returns:
    *   Pointer to the corresponding integer register inside the process.
    *   Returns NULL if the register name is unknown.
    * -----------------------------------------------------------------------------------
    * Usage:
    *   Used by the instruction execution function to directly modify registers.
*/
int* get_register_ptr(process_t* p, const char* reg_name) {
    if (strcmp(reg_name, "AX") == 0) return &(p->ax);
    if (strcmp(reg_name, "BX") == 0) return &(p->bx);
    if (strcmp(reg_name, "CX") == 0) return &(p->cx);
    return NULL;
}

/*
    * Function: exec_instruction
    * -----------------------------------------------------------------------------------
    * Purpose:
    *   Decodes and executes a single instruction for a given process.
    * -----------------------------------------------------------------------------------
    * Parameters:
    *   p     - Pointer to the process structure to execute the instruction on.
    *   instr - String containing the instruction to execute (e.g., "ADD AX, 5").
    * -----------------------------------------------------------------------------------
    * Behavior:
    *   - Supports arithmetic operations (ADD, SUB, MUL).
    *   - Supports increment operation (INC).
    *   - Supports jump operation (JMP) with repeated jump detection.
    *   - Ignores "NOP" instructions.
    *   - Handles both register-to-register and register-to-immediate operations.
    *   - Updates the process registers and program counter (pc) as needed.
    * -----------------------------------------------------------------------------------
    * Usage:
    *   Called by the Round-Robin scheduler to execute each instruction in a process.
*/
void exec_instruction(process_t* p, const char* instr) {
    char command[5];      // "ADD", "SUB", "INC", etc.
    char arg1_str[10];    // First argument (destination register or jump target)
    char arg2_str[10];    // Second argument (value or source register)
    
     if (strstr(instr, "NOP") != NULL) { 
        return;
    }

    // Format: COMMAND ARG1, ARG2 (e.g., "ADD AX, 5")
    if (sscanf(instr, "%4s %9[^,],%9s", command, arg1_str, arg2_str) == 3 ||
        sscanf(instr, "%4s %9[^,], %9s", command, arg1_str, arg2_str) == 3) {
        int* dest_reg = get_register_ptr(p, arg1_str); // first operand (destination)
        if (!dest_reg) {
            printf(COLOR_ERROR "Error: Unknown register in '%s'" COLOR_RESET "\n", instr);
            return;
        }

        // Determine if ARG2 is an immediate value or another register
        int value;
        if (isdigit(arg2_str[0]) || (arg2_str[0] == '-' && isdigit(arg2_str[1]))) { // immediate value
            value = atoi(arg2_str);
        } else {
            int* src_reg = get_register_ptr(p, arg2_str); // second operand is a register
            if (!src_reg) {
                printf(COLOR_ERROR "Error: Register unknown in '%s'" COLOR_RESET "\n", instr);
                return;
            }
            value = *src_reg;
        }
        
        if (strcmp(command, "ADD") == 0) {      // ADD dest, src|imm
                *dest_reg += value;
            } else if (strcmp(command, "SUB") == 0) {
                *dest_reg -= value;
            } else if (strcmp(command, "MUL") == 0) {
                *dest_reg *= value;
            } else {
                printf(COLOR_ERROR "Error: Unknown arithmetic command in '%s'" COLOR_RESET "\n", instr);
            }

    // Format: COMMAND ARG1 (e.g., "INC AX" or "JMP 5")
    } else if (sscanf(instr, "%4s %9s", command, arg1_str) == 2) {
        if (strcmp(command, "INC") == 0) {      // INC reg
            int* reg = get_register_ptr(p, arg1_str);
            if (reg) {
                (*reg)++;
            } else{
                printf(COLOR_ERROR "Error: Unknown register in '%s'" COLOR_RESET "\n", instr);
            }
        } else if (strcmp(command, "JMP") == 0) {
            int dest = atoi(arg1_str);
            if (dest < 0 || dest >= p->num_instructions) {
                printf(COLOR_ERROR "Error: JMP destination out of range in '%s' (pc stays)" COLOR_RESET "\n", instr);
            } else {
                // Track repeated jumps to detect infinite loops
                if (p->last_jump == dest) {
                    p->repeated_jumps++;
                } else {
                    p->repeated_jumps = 0;
                }
                p->last_jump = dest;

                // Adjust program counter to jump target (minus 1 due to increment after execution)
                p->pc = dest - 1;
            }
        } else {
            printf(COLOR_ERROR "Error: Unknown command in '%s'" COLOR_RESET "\n", instr);
        }
    } else {
        printf(COLOR_ERROR "Error: Unknown sintax '%s'" COLOR_RESET "\n", instr);
    }
}

/*
    * Function: run_round_robin
    * -----------------------------------------------------------------------------------
    * Purpose:
    *   Simulates a Round-Robin scheduling algorithm for multiple processes.
    * -----------------------------------------------------------------------------------
    * Parameters:
    *   processes     - Array of process structures.
    *   num_processes - Number of processes in the array.
    * -----------------------------------------------------------------------------------
    * Behavior:
    *   - Cycles through processes in order, giving each a fixed quantum of CPU time.
    *   - Skips finished processes.
    *   - Executes instructions one by one within the quantum using exec_instruction().
    *   - Updates process status ("Executing", "Ready", "Finished").
    *   - Detects processes stuck in repeated jumps and terminates them.
    *   - Logs context switches and process state changes.
    *   - Continues until all processes are finished.
    * -----------------------------------------------------------------------------------
    * Usage:
    *   Called to start the simulation of process execution.
    * -----------------------------------------------------------------------------------
*/
void run_round_robin(process_t processes[], int num_processes) {
    bool all_finished = false;

    while (!all_finished) {
        all_finished = true;

        for (int i = 0; i < num_processes; i++) { // iterate over each process in order
            process_t* p = &processes[i];

            if (strcmp(p->status, "Finished") == 0) continue; // skip completed

            all_finished = false;

            printf(COLOR_CONTEXT "\n[Context switch] -> Next PID %d" COLOR_RESET "\n", p->pid);
            log_info("Context switch to PID %d", p->pid);
            printf(COLOR_INFO "Saving state: " COLOR_RESET COLOR_PROCESS "PID=%d " COLOR_RESET 
                COLOR_REGISTER "PC=%d AX=%d BX=%d CX=%d" COLOR_RESET "\n",
            p->pid, p->pc, p->ax, p->bx, p->cx);

            strcpy(p->status, "Executing");
            log_info("Executing process %d", p->pid);

            int executed = 0; // how many instructions consumed from its quantum
            while (executed < p->quantum && p->pc < p->num_instructions) { // run time slice
                printf(COLOR_INSTRUCTION "  [%d] %s" COLOR_RESET "\n", p->pc, p->instructions[p->pc]);
                exec_instruction(p, p->instructions[p->pc]);
                p->pc++;
                executed++;
                if (p->repeated_jumps > MAX_REPEATED_JUMPS) {
                    printf(COLOR_ERROR "Process %d has exceeded the maximum number of repeated jumps (%d). Terminating process..." COLOR_RESET "\n", p->pid, MAX_REPEATED_JUMPS);
                    log_error("Process %d exceeded max repeated jumps. Terminating process", p->pid);
                    break;
                }
            }
            printf(COLOR_INFO "Updated state: " COLOR_RESET COLOR_PROCESS "PID=%d " COLOR_RESET 
                    COLOR_REGISTER "PC=%d AX=%d BX=%d CX=%d" COLOR_RESET "\n",
                    p->pid, p->pc, p->ax, p->bx, p->cx);
            log_info("Process %d updated status: PC=%d, AX=%d, BX=%d, CX=%d",
                    p->pid, p->pc, p->ax, p->bx, p->cx);

            if (p->pc >= p->num_instructions || p->repeated_jumps > MAX_REPEATED_JUMPS) { // done or killed
                strcpy(p->status, "Finished");
                printf(COLOR_SUCCESS "  -> Process %d finished" COLOR_RESET "\n", p->pid);
                log_info("Process %d finished execution", p->pid);
            } else {
                strcpy(p->status, "Ready"); // put back in ready state
                log_info("Process %d set to Ready", p->pid);
            }
        }
    }

    printf(COLOR_SUCCESS COLOR_BOLD "\n[End of simulation] All processes have finished." COLOR_RESET "\n");
    log_info("End of simulation: All processes have finished execution"); // log summary
}