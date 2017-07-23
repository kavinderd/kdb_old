/*
 * Initialize scan state
 *
 * Get file descriptor for relation and do any other preperation
 */
#include "executor.h"
#include "../access/heap_manager.h"

extern Relation *heap_open_relation(char *rel_name);
extern Tuple* heap_get_next_tuple();

/*
 * Initialize scan does the following:
 *
 * 1. Set status to READY
 * 2. Open relation (talk to buffer manager subsystem)
 */
void initialize_scan(ScanState* scan_state) {
    scan_state->status = READY;
    Relation *rel = heap_open_relation(scan_state->rel_name);
    scan_state->relation = rel;
}

Tuple* execute_scan(ScanState *scan_state) {
    return heap_get_next_tuple(scan_state->relation); 
}
