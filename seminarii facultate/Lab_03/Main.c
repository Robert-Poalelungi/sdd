#include <stdio.h>
#include "F1Car.h"
#include "SinglyLinkedList.h"

int main() {

	Node* list = NULL;
	int count = loadCars("cars.csv", &list);

	if (count >= 0) {
		parseList(list);
	}
	else {
		printf("Failed to load cars.\n");
	}

	freeList(&list);

	return 0;

}