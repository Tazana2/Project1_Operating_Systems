#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "loader.h"
#include "planner.h"
#include "utils/logger.h"
#include "utils/colors.h"

#define MAX_PROCESSES 100

/*
    * Function: main
    * -----------------------------------------------------------------------------------
    * Purpose:
    *   Entry point for the process planner simulation program.
    *   Reads a process description file, loads the processes into memory,
    *   logs their details, and runs the Round-Robin scheduling simulation.
    * -----------------------------------------------------------------------------------
    * Parameters:
    *   argc - Number of command-line arguments.
    *   argv - Array of command-line arguments:
    *          argv[1] should be "-f"
    *          argv[2] should be the path to the processes file.
    * -----------------------------------------------------------------------------------
    * Returns:
    *   EXIT_SUCCESS on successful execution.
    *   EXIT_FAILURE if there are invalid arguments or errors during initialization.
    * -----------------------------------------------------------------------------------
    * Behavior:
    *   - Validates the number of command-line arguments.
    *   - Checks that the flag "-f" is provided.
    *   - Initializes the logger to record simulation output.
    *   - Loads processes from the specified file using load_processes().
    *   - Prints and logs process details to both console and log file.
    *   - Executes the Round-Robin scheduler with run_round_robin().
    *   - Closes the logger before exiting.
*/
int main(int argc, char *argv[]) {
    // Validate the number of arguments
    if (argc != 3) {
        fprintf(stderr, COLOR_ERROR "Invalid number of arguments." COLOR_RESET "\n");
        fprintf(stderr, COLOR_INFO "Usage: procplanner -f processes_file" COLOR_RESET "\n");
        return EXIT_FAILURE;
    }

    // Validate that the correct flag "-f" is provided
    if (strcmp(argv[1], "-f") != 0) {
        fprintf(stderr, COLOR_ERROR "Invalid flag. Expected -f" COLOR_RESET "\n");
        fprintf(stderr, COLOR_INFO "Usage: procplanner -f processes_file" COLOR_RESET);
        return EXIT_FAILURE;
    }

    // Initialize the logger
    init_logger("logs.txt");
    log_info("Starting simulation with input file: %s", argv[2]);

    // Load processes from the specified file
    process_t processes[MAX_PROCESSES];
    int num_processes = load_processes(argv[2], processes, MAX_PROCESSES);


    // Display loaded processes on the console and log them
    printf(COLOR_SUCCESS "These %d processes have been loaded:" COLOR_RESET "\n", num_processes);
    for (int i = 0; i < num_processes; i++) {
        printf(COLOR_PROCESS "PID: %d" COLOR_RESET " | " 
               COLOR_REGISTER "AX: %d | BX: %d | CX: %d" COLOR_RESET " | " 
               COLOR_INFO "Quantum: %d" COLOR_RESET " | " 
               COLOR_HIGHLIGHT "Instructions: %d" COLOR_RESET "\n",
                processes[i].pid, processes[i].ax, processes[i].bx, processes[i].cx,
                processes[i].quantum, processes[i].num_instructions);
        log_info("PID: %d | AX: %d | BX: %d | CX: %d | Quantum: %d | Instructions: %d",
                processes[i].pid, processes[i].ax, processes[i].bx, processes[i].cx,
                processes[i].quantum, processes[i].num_instructions);
    }

    // Execute the Round-Robin scheduling algorithm
    run_round_robin(processes, num_processes);

    // Close the logger before program termination
    close_logger();
    return 0;
}