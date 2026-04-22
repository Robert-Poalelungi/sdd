# Lab 03 — Singly Linked List

## Structura fișierelor (4 + main)

```
F1Car.h              → structura F1Car + prototipuri loadCars/printCar
F1Car.c              → implementarea loadCars și printCar
SinglyLinkedList.h   → definiția nodului + prototipurile listei
SinglyLinkedList.c   → implementarea funcțiilor listei
main.c               → folosește lista
```

**Împărțirea funcțiilor:**
- `F1Car.c` → tot ce ține de **date** (citire din CSV, afișare unei mașini)
- `SinglyLinkedList.c` → tot ce ține de **listă** (add, parse, free)

---

## PAS 1 — `F1Car.h` (structura datelor tale)

```c
#pragma once

typedef struct {
    int number;
    char* driver;
    char* team;
    float engine_hp;
} F1Car;

int loadCars(const char* filename, struct Node** list);
void printCar(const F1Car* car);
```

**Regulă:** dacă un câmp e string cu lungime variabilă → `char*` (alocat cu `malloc`). Dacă e număr fix → `int`/`float`.

---

## PAS 2 — `SinglyLinkedList.h` (nodul + prototipuri)

```c
#pragma once

#include "F1Car.h"

typedef struct Node {
    F1Car* data;
    struct Node* next;
} Node;

void addToBeginning(Node** list, F1Car* car);
void addToEnd(Node** list, F1Car* car);
void parseList(Node* list);
void freeList(Node** list);
```

**Regulă:**
- `Node**` dacă funcția poate schimba capul listei (`addTo*`, `freeList`, `deleteTo*`)
- `Node*` dacă doar citește (`parseList`)

---

## PAS 3 — `addToBeginning` (cel mai simplu)

```c
void addToBeginning(Node** list, F1Car* car) {
    Node* newNode = malloc(sizeof(Node));  // 1. alocă nodul
    newNode->data = car;                   // 2. pune datele
    newNode->next = *list;                 // 3. leagă de fostul cap
    *list = newNode;                       //    noul cap = newNode
}
```

**Mnemonic:** „Alocă, pune data, leagă, mută capul."

---

## PAS 4 — `addToEnd` (același pattern + parcurgere)

```c
void addToEnd(Node** list, F1Car* car) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = car;
    newNode->next = NULL;                  // ultimul → next e NULL

    if (*list) {                           // dacă lista are elemente
        Node* aux = *list;
        while (aux->next) {                // du-te până la ultimul
            aux = aux->next;
        }
        aux->next = newNode;               // leagă ultimul de newNode
    }
    else {                                 // lista e goală
        *list = newNode;                   // newNode devine cap
    }
}
```

**Două cazuri mereu:** lista goală / lista are elemente.

**Mnemonic pentru parcurgere:** `while (aux->next)` înseamnă „oprește-te **pe** ultimul nod, nu după el".

---

## PAS 5 — `parseList` (doar citești)

```c
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
```

**Pattern de parcurgere:** `while (x) { ...; x = x->next; }`.

---

## PAS 6 — `freeList` (eliberare în ordine)

```c
void freeList(Node** list) {
    Node* current = *list;
    while (current) {
        Node* next = current->next;    // SALVEAZĂ next ÎNAINTE
        free(current->data->driver);   // 1. strings din F1Car
        free(current->data->team);     // 2. strings din F1Car
        free(current->data);           // 3. F1Car
        free(current);                 // 4. Nodul
        current = next;
    }
    *list = NULL;
}
```

**Regulă de aur:** eliberezi **din interior spre exterior**. Tot ce ai alocat cu `malloc`, trebuie eliberat — în ordine inversă alocării.

---

## PAS 7 — `F1Car.c` (citire CSV + afișare)

**Conține 2 funcții:** `loadCars` și `printCar`.

```c
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

        char* token = strtok(line, ",");       // PRIMUL strtok cu line
        car->number = atoi(token);

        token = strtok(NULL, ",");             // RESTUL cu NULL
        car->driver = malloc(strlen(token) + 1);
        strcpy(car->driver, token);

        token = strtok(NULL, ",");
        car->team = malloc(strlen(token) + 1);
        strcpy(car->team, token);

        token = strtok(NULL, ",");
        car->engine_hp = atof(token);

        addToEnd(list, car);
        count++;
    }

    fclose(f);
    return count;
}
```

**Pattern pentru fiecare câmp:**
- **număr** → `token = strtok(...); camp = atoi(token);` (sau `atof`)
- **string** → `token = strtok(...); camp = malloc(strlen(token)+1); strcpy(camp, token);`

**`strtok` regulă:** prima dată cu `line`, apoi cu `NULL` până termini.

**Și funcția `printCar` tot aici:**

```c
void printCar(const F1Car* car) {
    printf("Car #%d - %s from %s - HP: %.1f\n",
        car->number, car->driver, car->team, car->engine_hp);
}
```

**Regulă:** `printCar` primește `const F1Car*` — doar citește, nu modifică.

---

## PAS 8 — `main.c`

```c
#include <stdio.h>
#include "F1Car.h"
#include "SinglyLinkedList.h"

int main() {
    Node* list = NULL;                          // 1. init
    int count = loadCars("cars.csv", &list);    // 2. load (&list!)

    if (count >= 0) {
        parseList(list);                        // 3. folosește (list fără &)
    } else {
        printf("Failed to load cars.\n");
    }

    freeList(&list);                            // 4. free (&list!)

    return 0;
}
```

**Regulă `&`:**
- `&list` când funcția **modifică** capul (load, add, free, delete)
- `list` când doar **citește** (parse, print)

---

## Cheat-sheet final

Pe o foaie, pe de rost:

```c
// 1. Alocare nod
Node* newNode = malloc(sizeof(Node));
newNode->data = car;
newNode->next = NULL;  // sau *list pentru begin

// 2. Parcurgere până la ultimul
Node* aux = *list;
while (aux->next) aux = aux->next;

// 3. Parcurgere toată lista
while (node) { ...; node = node->next; }

// 4. Free un nod complet
free(node->data->driver);
free(node->data->team);
free(node->data);
free(node);
```

---

## Reguli de aur

1. **`Node**` când modifici capul, `Node*` când doar citești**
2. **În `main`: `&list` la modificare, `list` la citire**
3. **Eliberează din interior spre exterior** (strings → struct → nod)
4. **Salvează `next` înainte de `free(current)`**
5. **`while (aux->next)` pentru a ajunge PE ultimul nod**
6. **`strtok`: prima dată cu `line`, apoi cu `NULL`**
7. **`malloc(strlen(token) + 1)` — `+1` pentru terminatorul `'\0'`**
8. **Mereu `if (!f)` după `fopen`, mereu `fclose` la final**
