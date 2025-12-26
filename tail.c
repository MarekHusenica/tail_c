#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "circ_buffer.h"

#define DEFAULT_LINE_COUNT 10

inline void print_help();
int get_line(char** line, size_t* len, FILE* file);

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
    }
    else {
        if (argc > 1) fprintf(stderr, "ERROR: Too many arguments");
        print_help();
        return 0;
    }

    FILE* file;
    if (file_name != NULL) file = fopen(file_name, "r");
    else file = stdin;

    circ_buffer_t buff = circ_buff_init(n_lines + 1);
    char* curr_line = NULL;
    size_t len = 0;
    int reached_eof = 0;

    while (reached_eof != 1) {
        reached_eof = get_line(&curr_line, &len, file);
        circ_buff_put(&buff, curr_line);
    }

    for (long unsigned int i = 0; i < n_lines; ++i) {
        char* line = NULL;
        circ_buff_get(&buff, &line);
        if (line) printf("%s", line);
    }

    circ_buff_free(&buff);
    fclose(file);

    return 0;
}

void print_help() {
    printf("Help\n");
}

int get_line(char** line, size_t* len, FILE* file) {
    if (!file || !len) return -1;

    size_t buff_size = 80;
    char* buff = malloc(buff_size * sizeof(char));
    if (!buff) return -1;
    size_t i = 0;
    int eof_flag = 0;

    while (1) {

        if (i == (buff_size - 1)) {
            char *tmp = realloc(buff, 2*buff_size*sizeof(char));
            if (tmp == NULL) {
                free(buff);
                return -1;
            }
            buff = tmp;
            buff_size *= 2;
        }

        int c = fgetc(file);
        if (c == EOF) {
            if (i == 0) {
                free(buff);
                return 2;
            }
            eof_flag = 1;
            break;
        }
        else if (c == '\n') {
            buff[i] = '\n';
            break;
        }
        else buff[i] = c;

        i++;
    }

    *line = malloc((i + 2) * sizeof(char));
    if (*line == NULL) {
        free(buff);
        return -1;
    }
    memcpy(*line, buff, i + 1);
    free(buff);

    (*line)[i+1] = '\0';
    *len = i;

    return eof_flag;
}