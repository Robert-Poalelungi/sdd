// Lab 03 + Lab 04 — Singly Linked List complet (one-file)
// Lab 4 e un superset al Lab 3 -> un singur fisier acopera ambele.
//
// Lab 3 (baseline) = addToBeginning, addToEnd, parseList, freeList, loadCars (cu addToEnd hardcodat)
// Lab 4 (+ extras) = InsertListFn typedef, deleteFromBeginning, deleteFromEnd,
//                    deleteByCondition, conditionFn-uri, loadCars cu InsertListFn
//
// Compilare: gcc Lab_03_04.c -o Lab_03_04
// Rulare:   pune cars.csv langa executabil, apoi ./Lab_03_04

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

typedef void (*InsertListFn)(Node**, F1Car*);

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

// ============================================================
// 3. LOADER — CSV -> F1Car -> lista (cu function pointer)
// ============================================================

int loadCars(const char* filename, Node** list, InsertListFn insertList) {

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

        insertList(list, car);
        count++;
    }

    fclose(f);
    return count;
}

// ============================================================
// 4. CONDITION HELPERS (pentru deleteByCondition)
// ============================================================

int isRedBull(F1Car* car) {
    return strcmp(car->team, "Red Bull") == 0;
}

int isUnder1015HP(F1Car* car) {
    return car->engine_hp < 1015.0f;
}

// ============================================================
// 5. MAIN
// ============================================================

int main(void) {

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
