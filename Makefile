CC=clang
TOPDIR=src
PARSER_DIR=src/parser
PARSER_TEST_DIR=src/parser/test
PLANNER_TEST_DIR=src/planner/test
EXECUTOR_TEST_DIR=src/executor/test
TEST_INCLUDE=/usr/local/include/
CFLAGS=-g -O0

VPATH= src:src/parser:src/planner:src/executor:src/utils:src/parser/test:src/storage/:src/access:src/server
OBJDIR:=build
OBJ:=$(addprefix $(OBJDIR)/, list.o parser.o lexer.o planner.o executor.o scan_node.o buffer_manager.o buffer.o storage_manager.o server.o heap_manager.o)
SRC=main.c list.c lexer.c

$(OBJDIR)/%.o : %.c %.h
	$(COMPILE.c) $(OUTPUT_OPTION) $<

all: $(OBJ)
	$(CC) $(CFLAGS) -o kdb src/main.c $^
$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)
clean:
	rm -f $(OBJ)
	rm -f $(TESTEXE)
	rm -f *.t
	rm -rf *.dSYM

## TEST MAKE TARGETS
check: $(OBJ) utils_test query_processing_test heap_manager_test storage_test server_test
query_processing_test: lexer_test parser_test planner_test executor_test scan_node_test
lexer_test: $(OBJ)
	$(CC) $(CFLAGS) -o lexer_test.t src/parser/test/lexer_test.c -I $(TEST_INCLUDE) -lcmocka
	./lexer_test.t

parser_test: $(OBJ)
	$(CC) $(CFLAGS)  -o parser_test.t src/parser/test/parser_test.c build/list.o -I ${TEST_INCLUDE} -lcmocka
	./parser_test.t

planner_test: $(OBJ)
	$(CC) $(CFLAGS) -o planner_test.t src/planner/test/planner_test.c build/list.o build/parser.o build/lexer.o -I ${TEST_INCLUDE} -lcmocka
	./planner_test.t

executor_test: $(OBJ)
	$(CC) $(CFLAGS) -o executor_test.t src/executor/test/executor_test.c build/list.o -I ${TEST_INCLUDE} -lcmocka
	./executor_test.t

scan_node_test: $(OBJ)
	$(CC) $(CFLAGS) -o scan_node_test.t src/executor/test/scan_node_test.c build/list.o -I ${TEST_INCLUDE} -lcmocka
	./scan_node_test.t


## Utility Tests

UTILS_TEST_DIR=src/utils/test
utils_test: list_test
list_test: $(OBJ)
	$(CC) $(CFLAGS)  -o list_test.t $(UTILS_TEST_DIR)/list_test.c -I ${TEST_INCLUDE} -lcmocka 
	./list_test.t

## Heap Manager Tests
HEAP_MANAGER_TEST_DIR=src/access/test
heap_manager_test: $(OBJ)
	$(CC) $(CFLAGS) -o heap_manager_test.t $(HEAP_MANAGER_TEST_DIR)/heap_manager_test.c -I ${TEST_INCLUDE} -lcmocka
	./heap_manager_test.t

## Storage Tests
storage_test: buffer_manager_test buffer_test storage_manager_test

BUFFER_MANAGER_TEST_DIR=src/storage/test

buffer_manager_test: $(OBJ)
	$(CC) $(CFLAGS)  -o buffer_manager_test.t $(BUFFER_MANAGER_TEST_DIR)/buffer_manager_test.c -I ${TEST_INCLUDE} -lcmocka 
	./buffer_manager_test.t

buffer_test: $(OBJ)
	$(CC) $(CFLAGS)  -o buffer_test.t $(BUFFER_MANAGER_TEST_DIR)/buffer_test.c -I ${TEST_INCLUDE} -lcmocka 
	./buffer_test.t

storage_manager_test: $(OBJ)
	$(CC) $(CFLAGS)  -o storage_manager_test.t $(BUFFER_MANAGER_TEST_DIR)/storage_manager_test.c -I ${TEST_INCLUDE} -lcmocka 
	./storage_manager_test.t

## Server Test

SERVER_TEST_DIR=src/server/test
server_test: $(OBJ)
	$(CC) $(CFLAGS)  -o server_test.t $(SERVER_TEST_DIR)/server_test.c -I ${TEST_INCLUDE} -lcmocka 
	./server_test.t



.PHONY:  clean compile all utils_test list_test lexer_test parser_test planner_test executor_test scan_node_test query_processing_test test heap_manager_test storage_test buffer_manager_test buffer_test storage_manager_test server_test
