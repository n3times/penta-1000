#include "log.h"

#include <string.h>

#define MAX_ENTRY_COUNT 100
#define MEM_LEN 2000

static int get_offset(log_t *log, long index) {
    return log->entry_offsets[(index - 1) % MAX_ENTRY_COUNT];
}

// Returns true if (a, b) intersects (c, d).
static bool intersects(int a, int b, int c, int d) {
    return !(d < a || b < c);
}

void log_get_available_interval(log_t *log,
                                long *first_index_out,
                                long *last_index_out) {
    if (first_index_out) *first_index_out = log->first_index;
    if (last_index_out) *last_index_out = log->last_index;
}

char *log_get_entry(log_t *log, int index) {
    return &log->mem[get_offset(log, index)];
}

void log_add_entry(log_t *log, char *entry) {
    // If log full, remove first element.
    int count = 0;
    if (log->first_index) {
        count = log->last_index - log->first_index + 1;
        if (count == MAX_ENTRY_COUNT) {
            if (count == 1) {
                log->first_index = 0;
                log->last_index = 0;
            }
            else log->first_index++;
        }
    }

    if (!log->first_index) log->first_index = 1;

    // Compute offset.
    int new_offset = 0;
    if (log->last_index) {
        int last_offset = get_offset(log, log->last_index);
        new_offset = last_offset + strlen(&log->mem[last_offset]) + 1;
        if (new_offset + strlen(entry) >= MEM_LEN) {
            new_offset = 0;
        }
    }
    int new_left = new_offset;
    int new_right = new_offset + strlen(entry);
    for (int index = log->first_index; index <= log->last_index; index++) {
        int left = get_offset(log, index);
        int right = left + strlen(&log->mem[left]);
        if (intersects(new_left, new_right, left, right)) {
            log->first_index++;
        } else {
            break;
        }
    }
    log->last_index++;
    log->entry_offsets[(log->last_index - 1) % MAX_ENTRY_COUNT] = new_offset;
    strcpy(&log->mem[new_offset], entry);
}

void log_clear(log_t *log) {
    memset(log, 0, sizeof(log_t));
}
