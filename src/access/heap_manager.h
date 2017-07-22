#define CATALOG_PAGE 0

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
