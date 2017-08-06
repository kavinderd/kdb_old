/*
 * KDB Server implementation
 *
 * The KDB server is a master process that accepts client communications and answers queries.
 *
 * execute_query() -> once a client sends a query, parse, and run the query.
 */

/*
 * execute_query
 *
 * Input: Query string
 * Output: NULL
 *
 * execute_query takes a query string, parses, plans and executes the query
 */
#include "../parser/parser.h"
#include "../planner/planner.h"
#include "../executor/executor.h"

void execute_query(char *query) {
    ParseNode *pn = parse(query);
    QueryPlan *qp = generate_plan(pn);
    ExecutionState *estate = (ExecutionState *) malloc(sizeof(*estate));
    initialize_execution_state(qp, estate);
    execute_plan(estate);
}





