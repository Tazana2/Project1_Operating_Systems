#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "loader.h"
#include "planner.h"
#include "utils/logger.h"

#define MAX_PROCESSES 10

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments.\n");
        fprintf(stderr, "Usage: procplanner -f processes_file\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-f") != 0) {
        fprintf(stderr, "Invalid flag. Expected -f\n");
        fprintf(stderr, "Usage: procplanner -f processes_file");
        return EXIT_FAILURE;
    }

    init_logger("logs.txt");
    log_info("Starting simulation with input file: %s", argv[2]);

    process_t processes[MAX_PROCESSES];
    int num_processes = load_processes(argv[2], processes, MAX_PROCESSES);


    printf("These %d processes have been loaded:\n", num_processes);
    for (int i = 0; i < num_processes; i++) {
        printf("PID: %d | AX: %d | BX: %d | CX: %d | Quantum: %d | Instructions: %d\n",
                processes[i].pid, processes[i].ax, processes[i].bx, processes[i].cx,
                processes[i].quantum, processes[i].num_instructions);
        log_info("PID: %d | AX: %d | BX: %d | CX: %d | Quantum: %d | Instructions: %d",
                processes[i].pid, processes[i].ax, processes[i].bx, processes[i].cx,
                processes[i].quantum, processes[i].num_instructions);
    }

    run_round_robin(processes, num_processes);

    close_logger();
    return 0;
}