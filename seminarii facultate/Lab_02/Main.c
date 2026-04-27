#include <stdio.h>
#include "F1Car.h"

int main() {
	F1Car* cars = NULL;
	int count = loadCars("cars.csv", &cars);

	if (count >= 0) {
		for (int i = 0; i < count; i++) {
			printCar(&cars[i]);
		}
	}
	else {
		printf("Failed to load cars.\n");
	}

	freeCars(cars, count);

	return 0;

}