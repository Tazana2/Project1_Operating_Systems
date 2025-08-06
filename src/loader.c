#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "process.h"
#include "loader.h"

int load_processes(const char* filename, process_t processes[], int max_processes) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error openning the processes file");
        return 0;
    }

    char line[256];
    int process_count = 0;

    while (fgets(line, sizeof(line), file) != NULL && process_count < max_processes) {
        
        process_t* p = &processes[process_count];
        p->pid = -1;
        p->pc = 0;
        p->ax = 0;
        p->bx = 0;
        p->cx = 0;
        p->quantum = 0;
        strcpy(p->status, "Ready");
        p->num_instructions = 0;
        p->last_jump = -1;
        p->repeated_jumps = 0;

        line[strcspn(line, "\n")] = 0;

        char* token = strtok(line, ",");
        while (token != NULL) {
            char reg_name[3];
            int value;

            if (sscanf(token, "PID:%d", &p->pid) == 1) {
            } else if (sscanf(token, "Quantum=%d", &p->quantum) == 1) {
            } else if (sscanf(token, "%2s=%d", reg_name, &value) == 2) {
                if (strcmp(reg_name, "AX") == 0) p->ax = value;
                else if (strcmp(reg_name, "BX") == 0) p->bx = value;
                else if (strcmp(reg_name, "CX") == 0) p->cx = value;
            }
            token = strtok(NULL, ",");
        }

        if (p->pid != -1) {
            char instr_filename[20];
            sprintf(instr_filename, "%d.txt", p->pid); 

            FILE* instr_file = fopen(instr_filename, "r");
            if (instr_file) {
                char instr_line[20];
                while (fgets(instr_line, sizeof(instr_line), instr_file)) {
                    instr_line[strcspn(instr_line, "\n")] = 0; 
                    if (p->num_instructions < MAX_INSTR) {
                        strcpy(p->instructions[p->num_instructions++], instr_line);
                    }
                }
                fclose(instr_file);
            } else {
                printf("Warning: Can't find the instructions file '%s' for PID %d.\n", instr_filename, p->pid);
            }
            process_count++;
        }
    }

    fclose(file);
    return process_count;
}