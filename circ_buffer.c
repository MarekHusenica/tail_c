#include <stdlib.h>
#include "circ_buffer.h"


circ_buffer_t circ_buff_init(int size) {
    circ_buffer_t buffer = {0,0,0,NULL};
    buffer.size = size;

    buffer.data = calloc(size, sizeof(char*));
}

int circ_buff_put(circ_buffer_t* buffer, char* item) {
    if ((buffer->write_idx + 1) % buffer->size == buffer->read_idx) {
        return 0;
    }

    buffer->data[buffer->write_idx] = item;
    buffer->write_idx = (buffer->write_idx + 1) % buffer->size;
    return 1;
}

int circ_buff_get(circ_buffer_t* buffer, char* str) {
    if (buffer->read_idx == buffer->write_idx) {
        return 0;
    }

    *str = buffer->data[buffer->read_idx];
    buffer->read_idx = (buffer->read_idx + 1) % buffer->size;
    return 1;
}

void circ_buff_free(circ_buffer_t* buffer) {
    if (buffer == NULL) return;

    for (int i = 0; i < buffer->size; ++i) {
        if (buffer->data[i]) free(buffer->data[i]);
    }
    free(buffer->data);
}