# Lab 05 — Doubly Linked List

Lab_05 aduce două diferențe majore față de lista simplă:

1. **Fiecare nod are `prev`** (pointer spre nodul anterior) pe lângă `next`
2. **Lista e o structură** cu `head` și `tail` (nu doar un `Node*`)

**Consecințe:**
- `insertEnd` devine **O(1)** (ai direct `tail`)
- Ștergerea e mai simplă (ai direct `prev`)
- Poți parcurge și înapoi (de la `tail` spre `head`)

---

## Structura fișierelor

```
Movie.h              → struct Movie + prototip printMovie
Movie.c              → implementare loadMovies + printMovie
DoublyLinkedList.h   → Node (cu prev+next) + List (head+tail) + prototipuri
DoublyLinkedList.c   → implementare insert/delete/parse/free
main.c               → folosește lista
```

---

## PAS 1 — `Movie.h`

```c
#pragma once

typedef struct {
    int year;
    char* title;
    int hasOscar;
} Movie;

void printMovie(const Movie* movie);
```

---

## PAS 2 — `DoublyLinkedList.h` (diferență majoră)

```c
#pragma once

#include "Movie.h"

typedef struct Node {
    Movie* data;
    struct Node* prev;       // ← NOU: pointer la nodul anterior
    struct Node* next;
} Node;

typedef struct {
    Node* head;              // primul nod
    Node* tail;              // ultimul nod
} List;

typedef void (*InsertListFn)(List*, Movie*);

void insertBeginning(List* list, Movie* movie);
void insertEnd(List* list, Movie* movie);
void parseList(List* list);
void deleteBeginning(List* list);
void deleteEnd(List* list);
void freeList(List* list);

int loadMovies(const char* filename, List* list, InsertListFn insertFn);
```

**Diferență crucială vs Lab_03/04:**
- Trimiți **`List*`** (NU `Node**`), pentru că `List` e o structură cu `head` și `tail` pe care le modifici prin pointer
- Nu mai ai nevoie de double pointer

---

## PAS 3 — `insertBeginning`

```c
void insertBeginning(List* list, Movie* movie) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = movie;
    newNode->prev = NULL;                // e primul → prev e NULL
    newNode->next = list->head;          // leagă-l de fostul cap

    if (list->head) {
        list->head->prev = newNode;      // fostul cap pointează înapoi spre newNode
    }
    else {
        list->tail = newNode;            // lista era goală → e și tail
    }

    list->head = newNode;                // noul cap
}
```

**4 legături** de gestionat:
1. `newNode->prev = NULL`
2. `newNode->next = head`
3. `head->prev = newNode`
4. `head = newNode`

Dacă lista era goală → și `tail = newNode`.

---

## PAS 4 — `insertEnd` (acum O(1)!)

```c
void insertEnd(List* list, Movie* movie) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = movie;
    newNode->next = NULL;                // e ultimul → next e NULL
    newNode->prev = list->tail;          // leagă-l de fostul tail

    if (list->tail) {
        list->tail->next = newNode;      // fostul tail pointează înainte spre newNode
    }
    else {
        list->head = newNode;            // lista era goală → e și head
    }

    list->tail = newNode;                // noul tail
}
```

**Simetric** cu `insertBeginning`: `head` ↔ `tail` și `prev` ↔ `next`.

**Nu mai parcurgi lista!** Ai direct `tail`, deci **O(1)**.

---

## PAS 5 — `parseList`

```c
void parseList(List* list) {
    if (list->head) {
        Node* aux = list->head;
        while (aux) {
            printMovie(aux->data);
            aux = aux->next;
        }
    }
    else {
        printf("List is empty\n");
    }
}
```

**Bonus** — parcurgere inversă:
```c
Node* aux = list->tail;
while (aux) {
    printMovie(aux->data);
    aux = aux->prev;
}
```

---

## PAS 6 — `deleteBeginning`

```c
void deleteBeginning(List* list) {
    if (list->head) {
        Node* toDelete = list->head;
        list->head = list->head->next;

        if (list->head) {
            list->head->prev = NULL;     // noul cap nu mai are prev
        }
        else {
            list->tail = NULL;           // lista a rămas goală
        }

        free(toDelete->data->title);
        free(toDelete->data);
        free(toDelete);
    }
    else {
        printf("List is empty\n");
    }
}
```

**Atenție:** dacă rămâne goală, setezi și `tail = NULL`.

---

## PAS 7 — `deleteEnd` (ușor cu prev!)

```c
void deleteEnd(List* list) {
    if (list->tail) {
        Node* toDelete = list->tail;
        list->tail = list->tail->prev;

        if (list->tail) {
            list->tail->next = NULL;     // noul tail nu mai are next
        }
        else {
            list->head = NULL;           // lista a rămas goală
        }

        free(toDelete->data->title);
        free(toDelete->data);
        free(toDelete);
    }
    else {
        printf("List is empty\n");
    }
}
```

**Diferență majoră vs Lab_04:** nu mai parcurgi până la penultim! Cu `prev` ajungi direct → **O(1)**.

---

## PAS 8 — `freeList`

```c
void freeList(List* list) {
    Node* current = list->head;
    while (current) {
        Node* next = current->next;
        free(current->data->title);
        free(current->data);
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
}
```

La `Movie` ai un singur `char*` (`title`).

---

## PAS 9 — `Movie.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Movie.h"
#include "DoublyLinkedList.h"

int loadMovies(const char* filename, List* list, InsertListFn insertFn) {

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: could not open file '%s'\n", filename);
        return -1;
    }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), f) != NULL) {

        Movie* movie = malloc(sizeof(Movie));

        char* token = strtok(line, ",");
        movie->year = atoi(token);

        token = strtok(NULL, ",");
        movie->title = malloc(strlen(token) + 1);
        strcpy(movie->title, token);

        token = strtok(NULL, ",");
        movie->hasOscar = atoi(token);

        insertFn(list, movie);
        count++;
    }

    fclose(f);
    return count;
}

void printMovie(const Movie* movie) {
    printf("[%d] %s - Oscar: %s\n",
        movie->year, movie->title, movie->hasOscar ? "Yes" : "No");
}
```

---

## PAS 10 — `main.c`

```c
#include <stdio.h>
#include "Movie.h"
#include "DoublyLinkedList.h"

int main() {
    List list = { NULL, NULL };                             // init: head=NULL, tail=NULL
    int count = loadMovies("movies.csv", &list, insertEnd);

    if (count < 0) {
        printf("Failed to load movies.\n");
        return 1;
    }

    printf("Loaded %d movies.\n", count);
    parseList(&list);

    freeList(&list);
    return 0;
}
```

---

## Cheat-sheet Lab 05

```c
// 1. Initializare listă
List list = { NULL, NULL };

// 2. Insert begin - 4 legături
newNode->prev = NULL;
newNode->next = list->head;
if (list->head) list->head->prev = newNode;
else list->tail = newNode;
list->head = newNode;

// 3. Insert end - simetric cu begin (head↔tail, prev↔next)
newNode->next = NULL;
newNode->prev = list->tail;
if (list->tail) list->tail->next = newNode;
else list->head = newNode;
list->tail = newNode;

// 4. Delete begin/end - muta head/tail, dezleagă prev/next al noului cap/tail
// Dacă rămâne goală: head = tail = NULL

// 5. Parcurgere inversă (bonus)
for (Node* a = list->tail; a; a = a->prev) ...
```

## Singly vs Doubly — tabel

| Operație      | Singly (Lab 3/4) | Doubly (Lab 5) |
|---------------|------------------|----------------|
| Insert begin  | O(1)             | O(1)           |
| Insert end    | O(n) [fără tail] | **O(1)**       |
| Delete begin  | O(1)             | O(1)           |
| Delete end    | O(n)             | **O(1)**       |
| Parcurgere ←  | ❌ imposibilă    | ✅ cu `prev`   |
| Memorie/nod   | 2 pointeri       | 3 pointeri     |
