#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "process.h"

// Format example
// pid pc ax bx cx quantum estado
// 1 0 2 3 1 3 Ready
// ADD AX, BX
// INC AX
// NOP

int load_process(FILE* file, process_t processes[], int max_processes){
    int i = -1;

    char line[100];
    while(fgets(line, sizeof(line), file) != NULL){
        if(isdigit(line[0])){
            i++;
            if (i >= max_processes){
                break;
            }

            sscanf(line, 
                    "%d %d %d %d %d %d %s",
                    &processes[i].pid, 
                    &processes[i].pc,
                    &processes[i].ax, 
                    &processes[i].bx, 
                    &processes[i].cx,
                    &processes[i].quantum, 
                    processes[i].status);

            processes[i].num_instructions = 0;
        } else if(i >= 0 && strlen(line) > 1) {
            line[strcspn(line, "\n")] = 0; // delete jumpline
            strcpy(processes[i].instructions[processes[i].num_instructions++], line);
        }
    }

    return i + 1;
}
