// Lab 03 — Singly Linked List (one-file)
// Compilare: gcc Lab_03.c -o Lab_03
// Rulare:   pune cars.csv langa executabil, apoi ./Lab_03

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================
// 1. DATA — F1Car
// ============================================================

typedef struct {
    int number;
    char* driver;
    char* team;
    float engine_hp;
} F1Car;

void printCar(const F1Car* car) {
    printf("Car #%d - %s from %s - HP: %.1f\n",
        car->number, car->driver, car->team, car->engine_hp);
}

// ============================================================
// 2. LIST — Singly Linked List
// ============================================================

typedef struct Node {
    F1Car* data;
    struct Node* next;
} Node;

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

// ============================================================
// 3. LOADER — CSV -> F1Car -> lista
// ============================================================

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
        car->engine_hp = (float)atof(token);

        addToEnd(list, car);
        count++;
    }

    fclose(f);
    return count;
}

// ============================================================
// 4. MAIN
// ============================================================

int main(void) {

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
