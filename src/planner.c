#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "planner.h"
#include "utils/logger.h"

// Returns the address of the register inside the process struct
int* get_register_ptr(process_t* p, const char* reg_name) {
    if (strcmp(reg_name, "AX") == 0) return &(p->ax);
    if (strcmp(reg_name, "BX") == 0) return &(p->bx);
    if (strcmp(reg_name, "CX") == 0) return &(p->cx);
    return NULL;
}

// Decode and execute a single instruction string on the process state
void exec_instruction(process_t* p, const char* instr) {
    char command[5];      // "ADD", "SUB", "INC", etc.
    char arg1_str[10];    
    char arg2_str[10];    
    
    if (strcmp(instr, "NOP") == 0) { // do nothing
        return;
    }

    if (sscanf(instr, "%4s %9[^,],%9s", command, arg1_str, arg2_str) == 3 ||
        sscanf(instr, "%4s %9[^,], %9s", command, arg1_str, arg2_str) == 3) {
    int* dest_reg = get_register_ptr(p, arg1_str); // first operand (destination)
        if (!dest_reg) {
            printf("Error: Unknown register in '%s'\n", instr);
            return;
        }

        int value;
        if (isdigit(arg2_str[0]) || (arg2_str[0] == '-' && isdigit(arg2_str[1]))) { // immediate value
            value = atoi(arg2_str);
        } else {
            int* src_reg = get_register_ptr(p, arg2_str); // second operand is a register
            if (!src_reg) {
                printf("Error: Register unknown in '%s'\n", instr);
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
            printf("Error: Unknown arithmetic command in '%s'\n", instr);
        }
    } else if (sscanf(instr, "%4s %9s", command, arg1_str) == 2) {
    if (strcmp(command, "INC") == 0) {      // INC reg
            int* reg = get_register_ptr(p, arg1_str);
            if(reg) {
                (*reg)++;
            } else{
                printf("Error: Unknown register in '%s'\n", instr);
            }
        } else if (strcmp(command, "JMP") == 0){
            int dest = atoi(arg1_str);
            if(dest < 0 || dest >= p->num_instructions) {
                printf("Error: JMP destination out of range in '%s' (pc stays)\n", instr);
            } else {
        if (p->last_jump == dest) { // same jump target again -> count repetition
                    p->repeated_jumps++;
                } else {
                    p->repeated_jumps = 0;
                }
                p->last_jump = dest;
        p->pc = dest - 1; // -1 because caller loop will ++ after return
            }
        } else {
            printf("Error: Unknown command in '%s'\n", instr);
        }

    } else {
        printf("Error: Unknown sintax '%s'\n", instr);
    }
}


// Simple round-robin scheduler simulation
void run_round_robin(process_t processes[], int num_processes) {
    bool all_finished = false;

    while (!all_finished) {
        all_finished = true;

    for (int i = 0; i < num_processes; i++) { // iterate over each process in order
            process_t* p = &processes[i];

            if (strcmp(p->status, "Finished") == 0) continue; // skip completed

            all_finished = false;

        printf("\n[Context switch] -> Next PID %d\n", p->pid);
        log_info("Context switch to PID %d", p->pid);
        printf("Saving state: PID=%d PC=%d AX=%d BX=%d CX=%d\n",
            p->pid, p->pc, p->ax, p->bx, p->cx);

            strcpy(p->status, "Executing");
            log_info("Executing process %d", p->pid);

            int executed = 0; // how many instructions consumed from its quantum
            while (executed < p->quantum && p->pc < p->num_instructions) { // run time slice
                printf("  [%d] %s\n", p->pc, p->instructions[p->pc]);
                exec_instruction(p, p->instructions[p->pc]);
                p->pc++;
                executed++;
                if (p->repeated_jumps > MAX_REPEATED_JUMPS) {
                    printf("Process %d has exceeded the maximum number of repeated jumps (%d). Terminating process...\n", p->pid, MAX_REPEATED_JUMPS);
                    log_error("Process %d exceeded max repeated jumps. Terminating process", p->pid);
                    break;
                }
            }
            printf("Updated state: PID=%d PC=%d AX=%d BX=%d CX=%d\n",
                    p->pid, p->pc, p->ax, p->bx, p->cx);
            log_info("Process %d updated status: PC=%d, AX=%d, BX=%d, CX=%d",
                    p->pid, p->pc, p->ax, p->bx, p->cx);

            if (p->pc >= p->num_instructions || p->repeated_jumps > MAX_REPEATED_JUMPS) { // done or killed
                strcpy(p->status, "Finished");
                printf("  -> Process %d finished\n", p->pid);
                log_info("Process %d finished execution", p->pid);
            } else {
                strcpy(p->status, "Ready"); // put back in ready state
                log_info("Process %d set to Ready", p->pid);
            }
        }
    }

    printf("\n[End of simulation] All processes have finished.\n");
    log_info("End of simulation: All processes have finished execution"); // log summary
}
