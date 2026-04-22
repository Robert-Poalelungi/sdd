#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "F1Car.h"
#include "SinglyLinkedList.h"

int loadCars(const char* filename, Node** list) {

	FILE* f = fopen(filename, "r");
	if (f == NULL) {
		printf("Error: could not open file '%s'\n", filename);
		return -1;
	}


	char line[256];
	int count = 0;


	while (fgets(line, sizeof(line), f) != NULL) {

		F1Car* car = malloc(sizeof(F1Car));

		char* token = strtok(line, ",");
		car->number = atoi(token);

		token = strtok(NULL, ",");
		car->driver = malloc(strlen(token) + 1);
		strcpy(car->driver, token);

		token = strtok(NULL, ",");
		car->team = malloc(strlen(token) + 1);
		strcpy(car->team, token);

		token = strtok(NULL, ",");
		car->engine_hp = atof(token);

		//addToBeginning(list, car);
		addToEnd(list, car);

		count++;
	}

	fclose(f);
	return count;
}

void printCar(const F1Car* car) {
	printf("Car #%d - %s from %s - HP: %.1f\n", car->number, car->driver, car->team, car->engine_hp);
}