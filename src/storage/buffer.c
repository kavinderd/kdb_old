/*
 * Implementation of the databases buffer pool
 *
 * All functions related to accessing buffer pool pages can be found here
 */
#include "buffer_manager.h"
#include "buffer.h"

#define BUFFER_POOL_SIZE 100

BufferPage *buffer_pool[100];

/*
 * Find a free page slot in the buffer pool. If none exists evict a resident page or return NULL
 */
BufferPage *get_free_buffer_page() {
    //A very naive algorithm to search for the first free buffer pool page that exists. If none is present, evict the first page in pool's array
    //TODO: Make this algorithm better
    BufferPage *bp;
    for (int i = 0; i < BUFFER_POOL_SIZE; i++) {
        if (buffer_pool[i] == NULL) {
            bp = (BufferPage *) malloc(sizeof(*bp));
            buffer_pool[i] = bp;
            break;
        }
        if (buffer_pool[i]->ref_count == 0) {
            bp = buffer_pool[i];
            break;
        }
    }
    bp->rel_name = NULL;
    bp->buffer = NULL;
    bp->ref_count = 0;
    return bp;
}
