#include <stdio.h>
#include <string.h>
#include "process.h"

void exec_instruction(process_t* p, const char* instr) {
    if (strcmp(instr, "NOP") == 0) {return;}
    else if (strcmp(instr, "INC AX") == 0) {
        p->ax = p->ax + 1;
    }

    else if (strcmp(instr, "ADD AX, BX") == 0) {// Add
        p->ax = p->ax + p->bx;
    }

    else if (strcmp(instr, "SUB CX, 1") == 0) {// Sub -1
        p->cx = p->cx - 1;
    }

    else if (strcmp(instr, "MUL AX, CX") == 0) {// AX x CX
        p->ax = p->ax * p->cx;
    }

    // (JMP <index>)
    else if (strncmp(instr, "JMP", 3) == 0) {
        int destination;

        // Read jump index
        sscanf(instr, "JMP %d", &destination);

        // -1?
        p->pc = destination - 0;
    }

    else {
        printf("Unknown: %s\n", instr);
    }
}
