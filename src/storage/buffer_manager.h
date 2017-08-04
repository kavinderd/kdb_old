#include <stdbool.h>
#ifndef BUFFER_MANAGER
#define BUFFER_MANAGER

#define PAGE_SIZE 8192

typedef struct BufferPage {
    char *rel_name;
    char *buffer;
    int ref_count;
} BufferPage;

extern BufferPage* buffer_manager_get_page(char *rel_name, int page);
#endif
