#include <stdio.h>
#include <stdlib.h>
#include "DoublyLinkedList.h"

void insertBeginning(List* list, Movie* movie) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = movie;
    newNode->prev = NULL;
    newNode->next = list->head;

    if (list->head) {
        list->head->prev = newNode;
    }
    else {
        list->tail = newNode;
    }

    list->head = newNode;
}

void insertEnd(List* list, Movie* movie) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = movie;
    newNode->next = NULL;
    newNode->prev = list->tail;

    if (list->tail) {
        list->tail->next = newNode;
    }
    else {
        list->head = newNode;
    }

    list->tail = newNode;
}
