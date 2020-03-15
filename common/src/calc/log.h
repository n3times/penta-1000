#include "calc.h"

int log_get_entry_count(log_t *log);

char *log_get_entry(log_t *log, int i);

void log_add_entry(log_t *log, char *entry);

void log_clear(log_t *log);
