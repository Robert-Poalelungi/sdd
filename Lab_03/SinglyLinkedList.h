#pragma once

#include "F1Car.h"

typedef struct Node {
	F1Car* data;
	struct Node* next;
} Node;

void addToBeginning(Node** list, F1Car* car);
void addToEnd(Node** list, F1Car* car);
void parseList(Node* list);
void freeList(Node** list);
