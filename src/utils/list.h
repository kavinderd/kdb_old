#ifndef LIST_H
#define LIST_H
typedef struct ListItem{
    void *ptr;
   struct ListItem* next;
} ListItem;

typedef struct {
    int length;
    ListItem* head;
    ListItem* tail;
} List;
#endif

List* new_list();

void list_append(List* list, void* datum);
void list_remove(List* list, void* datum);
