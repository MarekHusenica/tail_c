#ifndef CIRC_BUFFER_H
#define CIRC_BUFFER_H

typedef struct {
    int write_idx;
    int read_idx;
    int size;
    char** data;
} circ_buffer_t;

circ_buffer_t circ_buff_init(int size);
int circ_buff_put(circ_buffer_t* buffer, char* item);
int circ_buff_get(circ_buffer_t* buffer, char* str);
void circ_buff_free(circ_buffer_t* buffer);

#endif