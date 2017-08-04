/*
 * The Heap Manager has the following responsibilities:
 *
 * - Prepare state for reading from disk
 * - Communicate with BufferManager to get a tuple from the buffer
 *
 * The Heap Manager understands both the physical and logical view of data.
 * It reads from a page, finds a tuple and is able to return that tuple.
 *
 * - heap_open_relation(char * rel_name): Given a rel_name open the relation's file and return an instance of a RelationAccessor struct.
 * - heap_get_tuple(RelationAccessor *rel_acc); Given a relation accessor object, scan the block for the next tuple and return it
 */
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "../storage/buffer_manager.h"
#include "heap_manager.h"
#define MAX_PATH_LENGTH 100

/*
 * build_tuple_descriptor copies the TupleDescriptor information from a buffer page into memory.
 *
 * The first page of a relation has schema information
 */
void build_tuple_descriptor(Relation *rel, BufferPage *buf) {
    if (buf->buffer == NULL)  {
        return;
    }
    int attr_count = 0;
    TupleDescriptor *td = (TupleDescriptor *) malloc(sizeof(*td));
    //First read the TupleDescriptor information from the page
    memcpy((void *) td, (void *)buf->buffer, sizeof(*td));
    //Allocate as many elements in the array of attribute data as there are attrs 
    td->attrs = (AttributeData **) malloc(sizeof(AttributeData *) * td->attr_count);

    //For each attribute read its information off of the buffer and link it to the TupleDescriptor
    for (int i = 0; i < td->attr_count; i++) {
       AttributeData *ad = (AttributeData *) malloc(sizeof(*ad));
       memcpy((void *) ad, (void *) buf->buffer + sizeof(td) + (sizeof(ad) * i), sizeof(*ad));  
       td->attrs[i] = ad;
    }
    //Link the Relation to the TupleDescriptor
    rel->tuple_descriptor = td;
}

/*
 * heap_open_relation()
 *
 * Given a file name lookup the corresponding file name and invoke the buffer
 * manager to open the file.
 *
 * Since currently the catalog for each relation is stored separately in the first page of a relation, this function
 * causes I/O
 */
Relation * heap_open_relation(char *rel_name) {
    Relation *relation = (Relation *) malloc(sizeof(*relation));
    relation->rel_name = rel_name;

    /*
     * Access the catalog page to get the following information:
     *
     * - relation's oid (Do I really need this)
     * - tuple description (Information about each column in the table)
     *
     * Catalog access goes through the buffer manager.
     */
    //Have the buffer manager read the first page of the relation into the buffer pool 
    BufferPage *buf_page = buffer_manager_get_page(rel_name, CATALOG_PAGE);
    build_tuple_descriptor(relation, buf_page);
    relation->cur_buffer_position = -1;
    return relation;
}

HeapTupleData *heap_get_next_tuple(Relation *rel) {
    if (rel->cur_buf_page) {
        //We have a buffer page
        if (rel->cur_buffer_position == 0) {
            //We have not read a single tuple from this page yet
            TuplePageHeaderData* header_data = (TuplePageHeaderData *) malloc(sizeof(*header_data));
            memcpy((void *) header_data, (void *)rel->cur_buf_page->buffer, sizeof(*header_data));
            HeapTupleData *data = (HeapTupleData *) malloc(sizeof(*data));
            memcpy((void *) data, (void*) (rel->cur_buf_page->buffer + header_data->offset_to_last_tuple), header_data->tup_length);
            return data;
        }
    }
    return NULL;
}
