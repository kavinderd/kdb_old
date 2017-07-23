#include "../planner/planner.h"
#include "../access/heap_manager.h"
#ifndef EXECUTOR_H
#define EXECUTOR_H
typedef enum {
    UNINITIALIZED,
    READY,
    RUNNING,
    DONE,
    FAILED,
    ABORTED
} ExecutionStatus;

typedef enum {
    SCAN_STATE,
} PlanStateType;

typedef struct {
    PlanStateType type; 
    Plan* node;
} PlanState;

typedef struct {
    PlanStateType type;
    Plan* node;

    ExecutionStatus status;

    //The Relation this node will scan
    char* rel_name;

    //The list of columns this node needs to project
    List* projection_list;

    //The Heap Relation struct
    Relation *relation;

    //Tuple offset
    

} ScanState;

typedef struct {
    ExecutionStatus status;
    QueryPlan* plan;
    PlanState* planstate;
} ExecutionState;

typedef struct {
    HeapTupleData *heap_tuple;    
} Tuple;

void initialize_execution_state(QueryPlan* query_planning, ExecutionState* execution_state);

#endif
