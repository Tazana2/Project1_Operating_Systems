#include "utils/logger.h"
#include <stdarg.h>
#include <stdlib.h>

// Global static variable that stores the pointer to the log file
// Being static, it is only accessible within this translation unit (logger.c)
static FILE *log_file = NULL;

/*
    * Function: init_logger
    * -----------------------------------------------------------------------------------
    * Purpose:
    *   Initializes the logging system by opening the specified file for writing logs.
    * -----------------------------------------------------------------------------------
    * Parameters:
    *   filename - The name (path) of the file where logs will be stored.
    * -----------------------------------------------------------------------------------
    * Behavior:
    *   - Opens the file in write mode ("w"), overwriting any existing content.
    *   - If the file cannot be opened, prints an error message and terminates the program.
    * -----------------------------------------------------------------------------------
    * Usage:
    *   Called at the start of the program to set up logging.
    *   Should be paired with close_logger() to properly close the file when done.
*/
void init_logger(const char *filename) {
    log_file = fopen(filename, "w");
    if (!log_file) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }
}

/*
    * Function: close_logger
    * -----------------------------------------------------------------------------------
    * Purpose:
    *   Closes the log file and cleans up resources.
    * -----------------------------------------------------------------------------------
    * Behavior:
    *   - Checks if the log file is open.
    *   - Closes it and sets the pointer to NULL to prevent accidental reuse.
    * -----------------------------------------------------------------------------------
    * Usage:
    *   Called at the end of the program to ensure all logs are flushed and resources are released.
*/
void close_logger() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

/*
    * Function: log_with_prefix
    * -----------------------------------------------------------------------------------
    * Purpose:
    *   Writes a formatted log entry with a severity level prefix (e.g., "INFO", "ERROR").
    * -----------------------------------------------------------------------------------
    * Parameters:
    *   level   - A string representing the severity level of the log message.
    *   format  - A printf-style format string.
    *   args    - A va_list containing the arguments for the format string.
    * -----------------------------------------------------------------------------------
    * Behavior:
    *   - Writes the severity prefix to the log file.
    *   - Writes the formatted message.
    *   - Appends a newline and flushes the output so logs are written immediately.
    * -----------------------------------------------------------------------------------
    * Usage:
    *   Used internally by log_info() and log_error() to standardize log formatting.
*/
void log_with_prefix(const char *level, const char *format, va_list args) {
    if (!log_file) return;

    fprintf(log_file, "[%s] ", level);

    vfprintf(log_file, format, args);
    fprintf(log_file, "\n");
    fflush(log_file); // Ensure the log is written immediately
}

/*
    * Function: log_info
    * -----------------------------------------------------------------------------------
    * Purpose:
    *   Logs an informational message.
    * -----------------------------------------------------------------------------------
    * Parameters:
    *   format - A printf-style format string.
    *   ...    - Variable arguments matching the format string.
    * -----------------------------------------------------------------------------------
    * Behavior:
    *   - Creates a variable argument list.
    *   - Passes the arguments to log_with_prefix() with "INFO" as the level.
    * -----------------------------------------------------------------------------------
    * Usage:
    *   Used throughout the program to log general information and state changes.
*/
void log_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_with_prefix("INFO", format, args);
    va_end(args);
}

/*
    * Function: log_error
    * -----------------------------------------------------------------------------------
    * Purpose:
    *   Logs an error message.
    * -----------------------------------------------------------------------------------
    * Parameters:
    *   format - A printf-style format string.
    *   ...    - Variable arguments matching the format string.
    * -----------------------------------------------------------------------------------
    * Behavior:
    *   - Creates a variable argument list.
    *   - Passes the arguments to log_with_prefix() with "ERROR" as the level.
    * -----------------------------------------------------------------------------------
    * Usage:
    *   Used to log error conditions, such as file opening failures or invalid operations.
*/
void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_with_prefix("ERROR", format, args);
    va_end(args);
}