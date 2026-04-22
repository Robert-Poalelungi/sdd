#pragma once

typedef struct {
	int number;
	char* driver;
	char* team;
	float engine_hp;
} F1Car;

typedef void (*InsertListFn)(struct Node**, F1Car*);

int loadCars(const char* filename, struct Node** list, InsertListFn insertList);

void printCar(const F1Car* car);
