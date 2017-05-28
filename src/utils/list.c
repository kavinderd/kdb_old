/*
 * Implementation of a linked list
 */

#include <stdbool.h>
#include <stdlib.h>
#include "list.h"

List* new_list() {
    List* l = (List*) malloc(sizeof(*l));
    l->head = NULL;
    l->tail = NULL;
    l->length = 0;
    return l;
}

void list_append(List* list, void* datum) {
    ListItem* item = (ListItem*) malloc(sizeof(*item));
    item->next = NULL;
    item->ptr = datum;
    if (list->head != NULL) {
        ListItem* tail = list->tail;
        tail->next = item;
        list->tail = item;
        list->length++;
    } else {
        list->head = item;
        list->tail = item;
        list->length++;
    }
}

void list_remove(List* list, void* datum) {
  ListItem *current, *previous;
  if (list->head->ptr == datum) {
      list->head = list->head->next;
      list->length--;
  } else {
      current = list->head;
      while ( current != NULL && current->ptr != datum ) {
          previous = current;
          current = current->next;
      }
      if (current->ptr == datum) {
          previous->next = current->next;
          free(current);
          list->length--;
      }
  }
}
