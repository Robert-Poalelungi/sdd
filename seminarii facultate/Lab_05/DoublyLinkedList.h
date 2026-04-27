#pragma once

#include "Movie.h"

typedef struct Node {
    Movie* data;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
} List;

typedef void (*InsertListFn)(List*, Movie*);

void insertBeginning(List* list, Movie* movie);
void insertEnd(List* list, Movie* movie);

int loadMovies(const char* filename, List* list, InsertListFn insertFn);
