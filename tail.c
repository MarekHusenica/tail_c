#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LINE_COUNT 10

inline void print_help();

int main(int argc, char** argv) {

    unsigned long n_lines = DEFAULT_LINE_COUNT;
    char* file_name;

    if (argc > 1 && argc < 5) {
        int curr_arg = 1;

        if (strcmp(argv[curr_arg], "-n") == 0) {
            curr_arg++;

            n_lines = strtoul(argv[curr_arg], NULL, 10);

            if (errno == ERANGE) {
                fprintf(stderr, "ERROR: Number of lines too large.");
                return 1;
            }
            else if (n_lines = 0) {
                if (argv[curr_arg] == "0") fprintf(stderr, "ERROR: Number of lines can't be 0");
                else fprintf(stderr, "ERROR: Incorrect value after -n argument");
                return 1;
            }
            curr_arg++;
        }
        else {
            file_name = argv[argc];
        }
    }
    else {
        print_help();
        return 0;
    }

    printf("Number of lines: %d", n_lines);

    return 0;
}

inline void print_help() {
    printf("Help");
}