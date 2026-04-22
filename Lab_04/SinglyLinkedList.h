#pragma once

#include "F1Car.h"

typedef struct Node {
	F1Car* data;
	struct Node* next;
} Node;

void addToBeginning(Node** list, F1Car* car);
void addToEnd(Node** list, F1Car* car);
void parseList(Node* list);
void deleteFromBeginning(Node** list);
void deleteFromEnd(Node** list);
void deleteByCondition(Node** list, int (*conditionFn)(F1Car*));
void freeList(Node** list);
