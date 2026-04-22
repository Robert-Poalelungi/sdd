#pragma once

typedef struct {
	int number;
	char* driver;
	char* team;
	float engine_hp;
} F1Car;

int loadCars(const char* filename, F1Car** cars);

void printCar(const F1Car* car);

void freeCars(F1Car* cars, int count);