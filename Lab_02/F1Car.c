#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "F1Car.h"

int loadCars(const char* filename, F1Car** cars) {

	FILE* f = fopen(filename, "r");
	if (f == NULL) {
		printf("Error: could not open file '%s'\n", filename);
		return -1;
	}

	*cars = malloc(0);

	char line[256];
	int count = 0;


	while (fgets(line, sizeof(line), f) != NULL) {

		char* pos = strchr(line, '\n');
		if (pos) {
			*pos = '\0';
		}

		F1Car* temp = realloc(*cars, (count + 1) * sizeof(F1Car));

		if (temp == NULL) {
			printf("Error: memory allocation failed\n");
			free(*cars);
			*cars = NULL;
			fclose(f);
			return -1;
		}

		*cars = temp;

		F1Car* car = &(*cars)[count];

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

		count++;
	}

	fclose(f);
	return count;
}

void printCar(const F1Car* car) {
	printf("Car #%d - %s from %s - HP: %.1f\n", car->number, car->driver, car->team, car->engine_hp);
}

void freeCars(F1Car *cars, int count) {
	for (int i = 0; i < count; i++) {
		free(cars[i].driver);
		free(cars[i].team);
		cars[i].driver = NULL;
		cars[i].team = NULL;
	}

	free(cars);
}