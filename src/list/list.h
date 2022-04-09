#ifndef _UMLCC_LIST_H_
#define _UMLCC_LIST_H_

typedef struct _ListItem ListItem;

typedef struct _List {
    ListItem* _head;
} List;

List* new_list();
void  delete_list(List* list);
void  list_push(List* list, void* item);
void* list_pop(List* list);
void* list_at(List* list, int index);
int   list_length(List* list);

#endif
