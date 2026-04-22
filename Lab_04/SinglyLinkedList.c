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

void deleteFromBeginning(Node** list) {
	
	if (*list) {
		Node* toDelete = *list;
		*list = (*list)->next;

		free(toDelete->data->driver);
		free(toDelete->data->team);
		free(toDelete->data);
		free(toDelete);
	}
	else {
		printf("List is empty\n");
	}
}

void deleteFromEnd(Node** list) {
	if (*list) {
		if ((*list)->next) {
			Node* aux = *list;
			while (aux->next->next) {
				aux = aux->next;
			}

			Node* toDelete = aux->next;
			free(toDelete->data->driver);
			free(toDelete->data->team);
			free(toDelete->data);
			free(toDelete);

			aux->next = NULL;
		}
		else {
			free((*list)->data->driver);
			free((*list)->data->team);
			free((*list)->data);
			free(*list);
			*list = NULL;
		}
	}
	else {
		printf("List is empty\n");
	}
}

void deleteByCondition(Node** list, int (*conditionFn)(F1Car*)) {
	if (*list) {

		Node* aux = *list;
		Node* prev = NULL;

		while (aux) {

			if (conditionFn(aux->data)) {

				if (prev) {

					Node* toDelete = aux;

					prev->next = aux->next;
					aux = aux->next;

					free(toDelete->data->driver);
					free(toDelete->data->team);
					free(toDelete->data);
					free(toDelete);

				}
				else {

					*list = (*list)->next;
					free(aux->data->driver);
					free(aux->data->team);
					free(aux->data);
					free(aux);
					aux = *list;
				}

			}
			else {
				prev = aux;
				aux = aux->next;
			}
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