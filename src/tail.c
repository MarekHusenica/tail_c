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
        if (argc > 1) fprintf(stderr, "ERROR: Too many arguments\n");
        print_help();
        return 0;
    }

    FILE* file;
    if (file_name != NULL) file = fopen(file_name, "r");
    else file = stdin;

    if (!file) {
        fprintf(stderr, "ERROR: Couldn't find/open %s\n", file_name);
        return 1;
    }

    circ_buffer_t* buff = circ_buff_init(n_lines + 1);
    if (!buff) {
        fprintf(stderr, "ERROR: Memory allocation error\n");
        return 1;
    }

    char* curr_line = NULL;
    size_t len = 0;
    int status = 0;

    while (status != 1) {
        status = get_line(&curr_line, &len, file);

        if (status == -1) {
            fprintf(stderr, "ERROR: Memory allocation error\n");
            break;
        }

        if (curr_line) {
            circ_buff_put(buff, curr_line);
            curr_line = NULL;
        }
    }

    for (long unsigned int i = 0; i < n_lines; ++i) {
        char* line = NULL;
        circ_buff_get(buff, &line);
        if (line) printf("%s", line);
    }

    circ_buff_free(buff);
    fclose(file);

    return 0;
}

void print_help() {
    printf("Usage: tail [-n lines] [filename]\n");
}

int get_line(char** line, size_t* len, FILE* file) {
    if (!file || !len) return -1;

    size_t buff_size = 80;
    char* buff = malloc(buff_size * sizeof(char));
    if (!buff) return -1;

    size_t i = 0;
    int eof_flag = 0;

    while (1) {

        if (i >= (buff_size - 1)) {
            char *tmp = realloc(buff, 2*buff_size*sizeof(char));
            if (!tmp) {
                free(buff);
                return -1;
            }
            buff = tmp;
            buff_size *= 2;
        }

        int c = fgetc(file);
        if (c == EOF) {
            eof_flag = 1;
            break;
        }

        buff[i++] = c;

        if (c == '\n') break;
    }

    if (eof_flag && i == 0) {
        free(buff);
        *line = NULL;
        return 1;
    }

    *line = malloc((i + 1) * sizeof(char));
    if (!*line) {
        free(buff);
        return -1;
    }

    memcpy(*line, buff, i);
    (*line)[i] = '\0';
    *len = i;

    free(buff);
    return eof_flag;
}