# Lab 04 — Ștergere + Function Pointers

Lab_04 adaugă peste Lab_03:
1. **Function pointers** — treci o funcție ca argument
2. **Ștergere din listă** — la început, la sfârșit, după condiție

---

## Concept nou: Function Pointer

Declarație „urâtă":
```c
void (*insertFn)(Node**, F1Car*);
```

Cu `typedef` devine curat:
```c
typedef void (*InsertListFn)(struct Node**, F1Car*);
InsertListFn insertList;
```

**La ce folosește?** `loadCars` nu mai decide dacă inserezi la cap sau la coadă:
```c
loadCars("cars.csv", &list, addToEnd);        // insertează la coadă
loadCars("cars.csv", &list, addToBeginning);  // insertează la cap
```

---

## PAS 1 — `F1Car.h` actualizat

```c
#pragma once

typedef struct {
    int number;
    char* driver;
    char* team;
    float engine_hp;
} F1Car;

typedef void (*InsertListFn)(struct Node**, F1Car*);    // ← NOU

int loadCars(const char* filename, struct Node** list, InsertListFn insertList);
void printCar(const F1Car* car);
```

---

## PAS 2 — `SinglyLinkedList.h` actualizat

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
void deleteFromBeginning(Node** list);                      // ← NOU
void deleteFromEnd(Node** list);                            // ← NOU
void deleteByCondition(Node** list, int (*conditionFn)(F1Car*));  // ← NOU
void freeList(Node** list);
```

---

## PAS 3 — `loadCars` actualizat

Singura diferență: apelezi funcția primită ca parametru.

```c
int loadCars(const char* filename, Node** list, InsertListFn insertList) {
    // ... la fel ca Lab_03: fopen, fgets, strtok ...

    insertList(list, car);   // ← în loc de addToEnd(list, car)

    // ...
}
```

---

## PAS 4 — `deleteFromBeginning`

**3 pași:**

```c
void deleteFromBeginning(Node** list) {
    if (*list) {
        Node* toDelete = *list;      // 1. reține capul vechi
        *list = (*list)->next;       // 2. mută capul pe următorul

        free(toDelete->data->driver);  // 3. eliberează vechiul cap
        free(toDelete->data->team);
        free(toDelete->data);
        free(toDelete);
    }
    else {
        printf("List is empty\n");
    }
}
```

---

## PAS 5 — `deleteFromEnd`

**Două sub-cazuri:** ≥ 2 elemente / exact 1 element.

```c
void deleteFromEnd(Node** list) {
    if (*list) {
        if ((*list)->next) {                    // ≥ 2 elemente
            Node* aux = *list;
            while (aux->next->next) {           // oprește-te PE penultimul
                aux = aux->next;
            }

            Node* toDelete = aux->next;         // ultimul
            free(toDelete->data->driver);
            free(toDelete->data->team);
            free(toDelete->data);
            free(toDelete);

            aux->next = NULL;                   // penultimul devine ultim
        }
        else {                                   // doar 1 element
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
```

**Diferență-cheie:**
- `while (aux->next)` = oprește PE ultimul
- `while (aux->next->next)` = oprește PE penultimul (ai nevoie de penultim ca să rupi legătura)

---

## PAS 6 — `deleteByCondition` (cel mai important)

Primește un function pointer care decide dacă un nod trebuie șters.

```c
void deleteByCondition(Node** list, int (*conditionFn)(F1Car*)) {
    if (*list) {
        Node* aux = *list;
        Node* prev = NULL;

        while (aux) {
            if (conditionFn(aux->data)) {       // TREBUIE șters

                if (prev) {                      // nu e primul nod
                    Node* toDelete = aux;
                    prev->next = aux->next;      // sari peste aux
                    aux = aux->next;

                    free(toDelete->data->driver);
                    free(toDelete->data->team);
                    free(toDelete->data);
                    free(toDelete);
                }
                else {                           // este primul nod
                    *list = (*list)->next;       // mută capul
                    free(aux->data->driver);
                    free(aux->data->team);
                    free(aux->data);
                    free(aux);
                    aux = *list;
                }
            }
            else {                               // NU se șterge → avansează
                prev = aux;
                aux = aux->next;
            }
        }
    }
    else {
        printf("List is empty\n");
    }
}
```

**Idee:**
- `prev` = nodul anterior (sau `NULL` la cap)
- `aux` = nodul curent
- **Se șterge** → leagă `prev->next` de `aux->next`, apoi `free(aux)`
- **E cap** → mută `*list` pe `aux->next`
- **Nu se șterge** → `prev = aux; aux = aux->next`

**Atenție:** după ștergere NU faci `prev = aux` (aux a dispărut). `prev` rămâne același.

---

## PAS 7 — `main.c` actualizat

Funcțiile de condiție trăiesc în `main.c`:

```c
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
    int count = loadCars("cars.csv", &list, addToEnd);   // ← pasezi funcția

    if (count >= 0) parseList(list);

    deleteByCondition(&list, isRedBull);                  // ← pasezi condiția
    parseList(list);

    deleteByCondition(&list, isUnder1015HP);
    parseList(list);

    freeList(&list);
    return 0;
}
```

**Regulă:** orice funcție cu signatura `int (*)(F1Car*)` poate fi pasată la `deleteByCondition` — returnează 1 (șterge) sau 0 (păstrează).

---

## Cheat-sheet Lab 04

```c
// 1. Typedef function pointer
typedef void (*InsertListFn)(Node**, F1Car*);

// 2. Apelarea unui function pointer
insertList(list, car);          // apelezi funcția primită
conditionFn(aux->data);         // la fel

// 3. Parcurgere cu prev + aux (pentru ștergere)
Node* aux = *list;
Node* prev = NULL;
while (aux) {
    if (sterge) {
        if (prev) prev->next = aux->next;
        else *list = aux->next;
        // free(aux) — dar ATENȚIE la ordinea salvării lui next
    } else {
        prev = aux;
        aux = aux->next;
    }
}

// 4. Ștergere ultim → while (aux->next->next) = oprește pe penultim
```
