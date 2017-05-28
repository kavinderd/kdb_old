/*
 * The buffer manager brings in pages from disk and holds them in its buffer.
 * The access manager communicates with the buffer manager to get a handle to a
 * page. The buffer manager does not understand the logical view of data only
 * the physical one.
 *
 * At KDB server's start the Buffer Manager allocates a fixed length array of
 * Buffer Pages. As pages are brought into and out of memory they are first
 * staged in the buffer
 */

#include "buffer_manager.h"
#include "../storage/storage_manager.h"
#include "buffer.h"

#define BUFFER_SIZE 10000

BufferPage buffer[BUFFER_SIZE];

BufferPage* buffer_manager_get_page(char *rel_name, int page) {
    BufferPage *pg = get_free_buffer_page();
    pg->ref_count++;
    pg->rel_name = rel_name;
    get_page_from_disk(pg->buffer, pg->rel_name, page);
    return pg;
}

