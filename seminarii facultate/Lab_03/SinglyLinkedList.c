#include <stdio.h>
#include <stdlib.h>
#include "SinglyLinkedList.h"
#include "F1Car.h"

void addToBeginning(Node** list, F1Car* car) {

	Node* newNode = malloc(sizeof(Node));
	newNode->data = car;
	newNode->next = *list;

	*list = newNode;
}

void addToEnd(Node** list, F1Car* car) {

	Node* newNode = malloc(sizeof(Node));
	newNode->data = car;
	newNode->next = NULL;

	if (*list) {
		Node* aux = *list;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = newNode;
	}
	else {
		*list = newNode;
	}
}

void parseList(Node* list) {
	if (list) {
		while (list) {
			printCar(list->data);
			list = list->next;
		}
	}
	else {
		printf("List is empty\n");
	}
}

void freeList(Node** list) {
	Node* current = *list;
	while (current) {
		Node* next = current->next;
		free(current->data->driver);
		free(current->data->team);
		free(current->data);
		free(current);
		current = next;
	}
	*list = NULL;
}