#include "utils/logger.h"
#include <stdarg.h>
#include <stdlib.h>

static FILE *log_file = NULL;

void init_logger(const char *filename) {
    log_file = fopen(filename, "w");
    if (!log_file) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }
}

void close_logger() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

void log_with_prefix(const char *level, const char *format, va_list args) {
    if (!log_file) return;

    fprintf(log_file, "[%s] ", level);

    vfprintf(log_file, format, args);
    fprintf(log_file, "\n");
    fflush(log_file); // Ensure the log is written immediately
}

void log_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_with_prefix("INFO", format, args);
    va_end(args);
}

void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_with_prefix("ERROR", format, args);
    va_end(args);
}
