#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments.\n");
        fprintf(stderr, "Usage: procplanner -f file_path.\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-f") != 0) {
        fprintf(stderr, "Invalid flag. Expected -f\n");
        fprintf(stderr, "Usage: procplanner -f file_path\n");
        return EXIT_FAILURE;
    }

    FILE* ptr;
    ptr = fopen(argv[2], "r");

    if (ptr == NULL) {
        fprintf(stderr, "The provided file is not readable");
        return EXIT_FAILURE;
    }
    
    // tbc
    char str[80];
    if(fgets(str, 80, ptr) != NULL){
        puts(str);
    }

    fclose(ptr);
    return 0;
}