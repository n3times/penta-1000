#include "calc.h"

// Returns the indices of the first and last available log entries.
void log_get_available_interval(log_t *log,
                                long *first_index_out,
                                long *last_index_out);

// Gets the entry of given 'index'.
char *log_get_entry(log_t *log, long index);

// Adds a new entry to the log.
void log_add_entry(log_t *log, char *entry);

// Removes all the entries from the log.
void log_clear(log_t *log);
