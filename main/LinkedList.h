#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include "stddef.h"
#include "stdbool.h"
#include "esp_heap_caps.h"

typedef struct LinkedList LinkedList;
typedef struct LinkedList_Node LinkedList_Node;


typedef struct LinkedList_Node {
    LinkedList_Node* prevNode;
    LinkedList_Node* nextNode;
    void* data;
} LinkedList_Node;

typedef struct LinkedList
{
    LinkedList_Node* head;
    LinkedList_Node* tail;
    LinkedList_Node* current;
    int size;
} LinkedList;

void LinkedList_Init(LinkedList* list);

void LinkedList_Dispose(LinkedList* list);

bool LinkedList_MoveNext(LinkedList* list);

bool LinkedList_MoveFirst(LinkedList* list);

bool LinkedList_MoveLast(LinkedList* list);

bool LinkedList_MovePrev(LinkedList* list);

void LinkedList_AddFront(LinkedList* list, void* data);

void LinkedList_AddBack(LinkedList* list, void* data);

void* LinkedList_GetFirst(LinkedList* list);

void* LinkedList_GetLast(LinkedList* list);

void* LinkedList_PopFront(LinkedList* list);

void* LinkedList_PopBack(LinkedList* list);

void* LinkedList_Get(LinkedList* list, int index);

#endif //!__LINKED_LIST_H__