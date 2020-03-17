#include "calc.h"

void log_get_available_interval(log_t *log,
                                long *first_index_out,
                                long *last_index_out);

char *log_get_entry(log_t *log, int index);

void log_add_entry(log_t *log, char *entry);

void log_clear(log_t *log);
