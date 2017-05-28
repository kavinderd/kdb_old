#Introduction

kdb is a database written from scratch. The intention is not to provide *you* with a high performance state of the art database, but to provide *me* with a opportunity to learn about the internals of a database by writing one of my own. This is not the database you're looking for.

## Architecture

KDB is comprised of the following components

- Query Processor
- Access Manager (Buffer): This subsystem is the middleman between the underlying storage and all the other components
- Buffer Manager: This subsystem is responsible for retrieving data from the storage manager and holding data in its buffer
- Storage Manager: The subsytem is responsible for accessing the underlying filesystem and make the necessary system calls

### Query Processing

KDB has the following flow when dealing with Query Processing:

1. Lex & Parse: This phase generates a parse tree
2. Transform: This phase transforms a parse tree into a query tree
3. Execute: This phase executes a query tree

The Query Processor is not very sophisticated and the plan is to add a rewriter and optimizer at some point. Additionally the goal is to support the entire SQL syntax.

### Storage Layer

As of now KDB takes a simple approach to the underlying storage layer. Data is written to files as line delimited records and data is read back by opening a file and scanning the lines in memory. This is a preliminary implementation that will be improved as query performance suffers
