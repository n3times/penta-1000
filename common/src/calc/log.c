#include "log.h"

#include <string.h>

int log_get_entry_count(log_t *log) {
    return log->count;
}

char *log_get_entry(log_t *log, int i) {
    return &log->mem[log->indices[i]];
}

void log_add_entry(log_t *log, char *entry) {
    int index = 0;
    if (log->count) {
        int last_index = log->indices[log->count - 1];
        index = last_index + strlen(&log->mem[last_index]) + 1;
    }
    log->indices[log->count] = index;
    strcpy(&log->mem[index], entry);
    log->count++;
}

void log_clear(log_t *log) {
    memset(log, 0, sizeof(log_t));
}
