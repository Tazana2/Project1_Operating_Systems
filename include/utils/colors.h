#ifndef COLORS_H
#define COLORS_H

// ANSI Color codes for terminal output
#define COLOR_RESET     "\033[0m"
#define COLOR_BOLD      "\033[1m"
#define COLOR_DIM       "\033[2m"
#define COLOR_UNDERLINE "\033[4m"

// Text colors
#define COLOR_BLACK     "\033[30m"
#define COLOR_RED       "\033[31m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_YELLOW    "\033[33m"
#define COLOR_BLUE      "\033[34m"
#define COLOR_MAGENTA   "\033[35m"
#define COLOR_CYAN      "\033[36m"
#define COLOR_WHITE     "\033[37m"

// Bright text colors
#define COLOR_BRIGHT_BLACK   "\033[90m"
#define COLOR_BRIGHT_RED     "\033[91m"
#define COLOR_BRIGHT_GREEN   "\033[92m"
#define COLOR_BRIGHT_YELLOW  "\033[93m"
#define COLOR_BRIGHT_BLUE    "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN    "\033[96m"
#define COLOR_BRIGHT_WHITE   "\033[97m"

// Background colors
#define COLOR_BG_BLACK   "\033[40m"
#define COLOR_BG_RED     "\033[41m"
#define COLOR_BG_GREEN   "\033[42m"
#define COLOR_BG_YELLOW  "\033[43m"
#define COLOR_BG_BLUE    "\033[44m"
#define COLOR_BG_MAGENTA "\033[45m"
#define COLOR_BG_CYAN    "\033[46m"
#define COLOR_BG_WHITE   "\033[47m"

// Convenience macros for common formatting
#define COLOR_ERROR     COLOR_BOLD COLOR_RED
#define COLOR_WARNING   COLOR_BOLD COLOR_YELLOW
#define COLOR_SUCCESS   COLOR_BOLD COLOR_GREEN
#define COLOR_INFO      COLOR_BOLD COLOR_CYAN
#define COLOR_HIGHLIGHT COLOR_BOLD COLOR_MAGENTA
#define COLOR_PROCESS   COLOR_BOLD COLOR_BLUE
#define COLOR_INSTRUCTION COLOR_BRIGHT_GREEN
#define COLOR_REGISTER  COLOR_BRIGHT_YELLOW
#define COLOR_CONTEXT   COLOR_BRIGHT_CYAN

#endif
