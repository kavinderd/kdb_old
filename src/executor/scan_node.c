/*
 * Initialize scan state
 *
 * Get file descriptor for relation and do any other preperation
 */
#include "executor.h"

extern void open_relation(char *rel_name);
extern void scan_next_tuple();

/*
 * Initialize scan does the following:
 *
 * 1. Set status to READY
 * 2. Open relation (talk to buffer manager subsystem)
 */
void initialize_scan(ScanState* scan_state) {
    scan_state->status = READY;

    if (scan_state->status == READY) {
        open_relation(scan_state->rel_name);
    }
}

void execute_scan(ScanState *scan_state) {
    scan_next_tuple(); 
}
