#include "log.h"

#include <string.h>

int log_get_entry_count(log_t *log) {
    return log->count;
}

char *log_get_entry(log_t *log, int i) {
    int entry_offset = log->entry_offsets[i];
    return &log->mem[entry_offset];
}

void log_add_entry(log_t *log, char *entry) {
    int offset = 0;
    if (log->count) {
        int last_offset = log->entry_offsets[log->count - 1];
        offset = last_offset + strlen(&log->mem[last_offset]) + 1;
    }
    log->entry_offsets[log->count] = offset;
    strcpy(&log->mem[offset], entry);
    log->count++;
}

void log_clear(log_t *log) {
    memset(log, 0, sizeof(log_t));
}
