#include <stdio.h>
#include "F1Car.h"
#include "SinglyLinkedList.h"

int isRedBull(F1Car* car) {
	return strcmp(car->team, "Red Bull") == 0;
}

int isUnder1015HP(F1Car* car) {
	return car->engine_hp < 1015.0;
}

int main() {

	Node* list = NULL;
	int count = loadCars("cars.csv", &list, addToEnd);

	if (count >= 0) {
		parseList(list);
	}
	else {
		printf("Failed to load cars.\n");
	}

	printf("\n==============================================\n");

	deleteByCondition(&list, isRedBull);
	parseList(list);

	printf("\n==============================================\n");

	deleteByCondition(&list, isUnder1015HP);
	parseList(list);

	printf("\n==============================================\n");

	freeList(&list);

	return 0;

}