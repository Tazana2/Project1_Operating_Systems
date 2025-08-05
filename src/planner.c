#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "planner.h"

void exec_instruction(process_t* p, const char* instr) {
    if (strcmp(instr, "NOP") == 0) return;

    else if (strcmp(instr, "INC AX") == 0) {
        p->ax++;
    } 
    else if (strcmp(instr, "ADD AX, BX") == 0) {
        p->ax += p->bx;
    } 
    else if (strcmp(instr, "SUB CX, 1") == 0) {
        p->cx -= 1;
    } 
    else if (strcmp(instr, "MUL AX, CX") == 0) {
        p->ax *= p->cx;
    } 
    else if (strncmp(instr, "JMP", 3) == 0) {
        int dest;
        sscanf(instr, "JMP %d", &dest);
        if (p->last_jump == dest) p->repeated_jumps++;
        p->last_jump = dest;
        p->pc = dest - 1; // -1 because pc will be incremented after this instruction
    } 
    else {
        printf("Unkown instruction: %s\n", instr);
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
