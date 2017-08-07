/*
 * Main entry point for KDB server
 */

#include "server/server.h"

/*
 * main function for KDB server.
 *
 * current functionality:
 *
 * kdb "create table foo";
 * kdb "select * from foo;"
 *
 * TODO: Implement proper standalone server functionality
 */
int main(int argc, char **argv){
    if (argc < 2)
        return -1;
    char *query = argv[1];
    execute_query(query);
}
