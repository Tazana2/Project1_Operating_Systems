#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "planner.h"

int* get_register_ptr(process_t* p, const char* reg_name) {
    if (strcmp(reg_name, "AX") == 0) return &(p->ax);
    if (strcmp(reg_name, "BX") == 0) return &(p->bx);
    if (strcmp(reg_name, "CX") == 0) return &(p->cx);
    return NULL;
}

void exec_instruction(process_t* p, const char* instr) {
    char command[5];      // "ADD", "SUB", "INC", etc.
    char arg1_str[10];    
    char arg2_str[10];    
    
    if (strcmp(instr, "NOP") == 0) {
        return;
    }

    if (sscanf(instr, "%4s %9[^,], %9s", command, arg1_str, arg2_str) == 3) {
        int* dest_reg = get_register_ptr(p, arg1_str);
        if (!dest_reg) {
            printf("Error: Unknown register in '%s'\n", instr);
            return;
        }

        int value;
        if (isdigit(arg2_str[0]) || (arg2_str[0] == '-' && isdigit(arg2_str[1]))) {
            value = atoi(arg2_str);
        } else {
            int* src_reg = get_register_ptr(p, arg2_str); // It's a register
            if (!src_reg) {
                printf("Error: Register unknown in '%s'\n", instr);
                return;
            }
            value = *src_reg;
        }
        
        if (strcmp(command, "ADD") == 0) {
            *dest_reg += value;
        } else if (strcmp(command, "SUB") == 0) {
            *dest_reg -= value;
        } else if (strcmp(command, "MUL") == 0) {
            *dest_reg *= value;
        } else {
            printf("Error: Multiple command in '%s'\n", instr);
        }
    } else if (sscanf(instr, "%4s %9s", command, arg1_str) == 2) {
        if (strcmp(command, "INC") == 0) {
            int* reg = get_register_ptr(p, arg1_str);
            if(reg) {
                (*reg)++;
            } else{
                printf("Error: Unknown register in '%s'\n", instr);
            }
        } else if (strcmp(command, "JMP") == 0){
            int dest = atoi(arg1_str);

            if (p->last_jump == dest) {
                p->repeated_jumps++;
            } else {
                p->repeated_jumps = 0;
            }
            p->last_jump = dest;

            p->pc = dest - 1;
        } else {
            printf("Error: Unknown command in '%s'\n", instr);
        }

    } else {
        printf("Error: Unknown sintax '%s'\n", instr);
    }
}


void run_round_robin(process_t processes[], int num_processes) {
    bool all_finished = false;

    while (!all_finished) {
        all_finished = true;

        for (int i = 0; i < num_processes; i++) {
            process_t* p = &processes[i];

            if (strcmp(p->status, "Finished") == 0) continue;

            all_finished = false;

            printf("\n[Switching context]\n");
            printf("Saving process (%d) status: PC=%d, AX=%d, BX=%d, CX=%d\n",
                    p->pid, p->pc, p->ax, p->bx, p->cx);

            strcpy(p->status, "Executing");

            int executed = 0;
            while (executed < p->quantum && p->pc < p->num_instructions) {
                printf("- %d. Executing %s\n", p->pc, p->instructions[p->pc]);
                exec_instruction(p, p->instructions[p->pc]);
                p->pc++;
                executed++;
                if (p->repeated_jumps > MAX_REPEATED_JUMPS) {
                    printf("Process %d has exceeded the maximum number of repeated jumps (%d). Terminating process...\n", p->pid, MAX_REPEATED_JUMPS);
                    break;
                }
            }

            printf("Updated process (%d) status: PC=%d, AX=%d, BX=%d, CX=%d\n",
                    p->pid, p->pc, p->ax, p->bx, p->cx);

            if (p->pc >= p->num_instructions || p->repeated_jumps > MAX_REPEATED_JUMPS) {
                strcpy(p->status, "Finished");
                printf("- Process %d finished.\n", p->pid);
            } else {
                strcpy(p->status, "Ready");
            }
        }
    }

    printf("\n[End of simulation: All process have finished.]\n");
}
