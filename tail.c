#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LINE_COUNT 10

inline void print_help();

int main(int argc, char** argv) {

    unsigned long n_lines = DEFAULT_LINE_COUNT;
    char* file_name = NULL;

    if (argc > 1 && argc < 5) {
        int curr_arg = 1;

        int set_file_name = 0;
        int set_n_lines = 0;

        while (curr_arg < argc) {

            if (strcmp(argv[curr_arg], "-n") == 0) {
                if (set_n_lines) {
                    fprintf(stderr, "ERROR: passed -n argument twice\n");
                    return 1;
                }
                set_n_lines = 1;

                curr_arg++;
                if (curr_arg >= argc) {
                    fprintf(stderr, "ERROR: No value after -n argument\n");
                    return 1;
                }

                n_lines = strtoul(argv[curr_arg], NULL, 10);

                if (errno == ERANGE) {
                    fprintf(stderr, "ERROR: Number of lines too large.\n");
                    return 1;
                }
                else if (n_lines == 0) {
                    if (argv[curr_arg][0] == '0') fprintf(stderr, "ERROR: Number of lines can't be 0\n");
                    else fprintf(stderr, "ERROR: Incorrect value after -n argument\n");
                    return 1;
                }
                curr_arg++;
            }
            else {
                if (set_file_name) {
                    fprintf(stderr, "ERROR: Tried to set file name twice or incorrect argument was passed\n");
                    return 1;
                }
                set_file_name = 1;

                file_name = argv[curr_arg];
                curr_arg++;
            }
        }
        if (file_name == NULL) {
            fprintf(stderr, "ERROR: No file provided\n");
            return 1;
        }
    }
    else {
        if (argc > 1) fprintf(stderr, "ERROR: Too many arguments");
        print_help();
        return 0;
    }

    printf("Number of lines: %ld\n", n_lines);
    printf("File name: %s\n", file_name);

    return 0;
}

void print_help() {
    printf("Help\n");
}