#include <stdlib.h>
#include "circ_buffer.h"

struct circ_buffer {
    int write_idx;
    int read_idx;
    int size;
    char** data;
};

circ_buffer_t* circ_buff_init(int size) {
    circ_buffer_t* buffer = malloc(sizeof(circ_buffer_t));
    if (!buffer) return NULL;

    buffer->size = size;
    buffer->read_idx = 0;
    buffer->write_idx = 0;

    buffer->data = calloc(size, sizeof(char*));
    if (!buffer->data) {
        free(buffer);
        buffer = NULL;
        return NULL;
    }

    return buffer;
}

int circ_buff_put(circ_buffer_t* buffer, char* item) {
    if (!buffer) return 0;

    if ((buffer->write_idx + 1) % buffer->size == buffer->read_idx) {
        free(buffer->data[buffer->read_idx]);
        buffer->data[buffer->read_idx] = NULL;
        buffer->read_idx = (buffer->read_idx + 1) % buffer->size;
    }

    buffer->data[buffer->write_idx] = item;
    buffer->write_idx = (buffer->write_idx + 1) % buffer->size;
    return 1;
}

int circ_buff_get(circ_buffer_t* buffer, char** str) {
    if (!buffer || !str) return 0;

    if (buffer->read_idx == buffer->write_idx) {
        return 0;
    }

    *str = buffer->data[buffer->read_idx];

    buffer->read_idx = (buffer->read_idx + 1) % buffer->size;
    return 1;
}

void circ_buff_free(circ_buffer_t* buffer) {
    if (!buffer) return;

    if (buffer->data) {
        for (int i = 0; i < buffer->size; ++i) {
            if (buffer->data[i] != NULL) {
                free(buffer->data[i]);
                buffer->data[i] = NULL;
            }
        }

        free(buffer->data);
        buffer->data = NULL;
    }

    free(buffer);
    buffer = NULL;
}