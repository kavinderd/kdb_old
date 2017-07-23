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
} Relation;

typedef struct {
    int tuple_header_offset; //The number of bits before actual user data
    //User data comes at the end
} HeapTupleData;
#endif
