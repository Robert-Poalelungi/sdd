#pragma once

typedef struct {
	int number;
	char* driver;
	char* team;
	float engine_hp;
} F1Car;

int loadCars(const char* filename, struct Node** list);

void printCar(const F1Car* car);
