#ifndef __LINKED_LIST_C__
#define __LINKED_LIST_C__

#include "LinkedList.h"


void LinkedList_Init(LinkedList* list) {
    if (list == NULL)
    {
        list = malloc(sizeof(LinkedList));
    } 
    
    list->head = NULL;
    list->tail = NULL;
    list->current = NULL;
    list->size = 0;
};

void LinkedList_Dispose(LinkedList* list) {
    for (int i = 0; i < list->size; i++)
    {
        LinkedList_PopBack(list);
    }
    free(list);
};


bool LinkedList_MoveNext(LinkedList* list) {
    if (list->current->nextNode)
    {
        list->current = list->current->nextNode;
        return true;
    }
    return false;
};

bool LinkedList_MovePrev(LinkedList* list) {
    if (list->current->prevNode)
    {
        list->current = list->current->prevNode;
    }
    return false;
};

void LinkedList_AddFront(LinkedList* list, void* data) {

    LinkedList_Node* newNode = malloc(sizeof(LinkedList_Node)); 

    newNode->data = data;

    //Список не пустой
    if(list->head) {
        newNode->nextNode = list->head;
        newNode->prevNode = NULL;
        list->head->prevNode = newNode;
        list->head = newNode;
    } else { //Список пустой
        newNode->nextNode = NULL;
        newNode->prevNode = NULL;
        list->head = newNode;
        list->tail = newNode;
    }

    list->size++;
};

void LinkedList_AddBack(LinkedList* list, void* data) {
    LinkedList_Node* newNode = malloc(sizeof(LinkedList_Node)); 

    newNode->data = data;

     //Список не пустой
     if(list->tail) {
        newNode->prevNode = list->tail;
        newNode->nextNode = NULL;
        list->tail->nextNode = newNode;
        list->tail = newNode;
    } else { //Список пустой
        newNode->nextNode = NULL;
        newNode->prevNode = NULL;
        list->head = newNode;
        list->tail = newNode;
    }

    list->size++;
};

void* LinkedList_GetFirst(LinkedList* list) {
    return list->head->data;
};

void* LinkedList_GetLast(LinkedList* list) {
    return list->tail->data;

};

void* LinkedList_PopFront(LinkedList* list) {
    if (list->head)
    {
        void* result = list->head->data;
        LinkedList_Node* firstNode = list->head;
        list->head = list->head->nextNode;
        list->head->prevNode = NULL;
        list->size--;
        free(firstNode);
        return result;
    } else {
        return NULL;
    }

};

void* LinkedList_PopBack(LinkedList* list) {
    if (list->head)
    {
        void* result = list->tail->data;
        LinkedList_Node* lastNode = list->tail;
        list->tail = list->tail->prevNode;
        list->tail->nextNode = NULL;
        list->size--;
        free(lastNode);
        return result;
    } else {
        return NULL;
    }
};

void* LinkedList_Get(LinkedList* list, int index) {
    
    if (index >= list->size)
    {
        return NULL;
    }

    LinkedList_Node* currentNode;

    //Обход с начала списка
    if (index <= list->size / 2) {
        currentNode = list->head;
    
        for (int i = 0; i < index; i++)
        {
            currentNode = currentNode->nextNode;
        }
    } else { // Обход с конца списка
        currentNode = list->tail;

        int steps = list->size - (index + 1); 
        for (int i = 0; i < steps;  i++)
        {
            currentNode = currentNode->prevNode;
        }
    }

    return currentNode->data;
};

#endif //! __LINKED_LIST_C__