#include "../storage/buffer_manager.h"
#define CATALOG_PAGE 0
#ifndef HEAP_MANAGER
#define HEAP_MANAGER
typedef enum {
    INT,
    TEXT
} DataType;

typedef struct {
    char *name;
    DataType type; 
    unsigned int length;
} AttributeData;

typedef struct {
    int attr_count; //Number of attributes in the Tuple
    AttributeData **attrs;
} TupleDescriptor;

typedef struct {
    char *rel_name;
    TupleDescriptor *tuple_descriptor;
    int cur_buffer_position;
    BufferPage *cur_buf_page;
} Relation;

typedef struct {
    int tuple_header_offset; //The number of bits before actual user data
    //User data comes at the end
} HeapTupleData;

typedef struct {
    int tuple_count;
    int tup_length;
    int offset_to_free_space;
    int offset_to_last_tuple;
} TuplePageHeaderData;
#endif
