# Lab 07 — Heap (max-heap pe vector) — walkthrough complet

**Sursa codului:** `S9_28042025/main.c` din github.com/tcervinski-csie/DS2025
**Pre-requisites:** vectori, malloc/realloc, recursivitate.

---

## Concept central in 30 secunde

**Heap = arbore binar complet, stocat ca vector.** Aici e **max-heap**: parintele are mereu prioritate >= ambele copii.

**Mapping vector <-> arbore:**
```
index 0       -> radacina
index i:        parintele e la (i-1)/2
                copil stang la 2*i+1
                copil drept la 2*i+2
```

```
            [10]                  vector: [10, 7, 9, 4, 5, 8, 6]
           /     \                index:   0  1  2  3  4  5  6
        [7]       [9]
       /   \     /   \
     [4]   [5] [8]   [6]
```

**2 operatii fundamentale:**
- **Sift-up** (urcare) — folosit la `insertHeap`
- **Sift-down** (coborare) — folosit la `heapify`

---

# Cod + walkthrough

## 1. Headere si structuri

```c
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 128

typedef struct Task Task;
typedef struct Heap Heap;

struct Task {
    char* description;
    int priority;
};

struct Heap {
    Task* tasks;     // vector dinamic de Task-uri
    int size;        // numarul curent de elemente
};
```

**Pas cu pas:**
- `_CRT_SECURE_NO_WARNINGS` dezactiveaza avertismentele MSVC pentru `strcpy`, `strtok`, `fopen` etc.
- `LINESIZE 128` = lungimea maxima a unei linii din fisier.
- `typedef struct Task Task;` declara forward — permite folosirea lui `Task` fara `struct Task`.
- **Heap-ul e un struct** cu 2 campuri: vectorul `tasks` (alocat dinamic) si `size` (cate elemente ai pus).

**Atentie:** `size` e numarul EFECTIV de elemente, nu capacitatea. Folosim `realloc` la fiecare insert (vezi mai jos).

---

## 2. `swapTask` — schimba doua taskuri

```c
void swapTask(Task* t1, Task* t2) {
    Task aux = *t1;
    *t1 = *t2;
    *t2 = aux;
}
```

**Pas cu pas:**
1. `Task aux = *t1;` — copiaza CONTINUTUL primului task in `aux`
2. `*t1 = *t2;` — pune in primul ce era in al doilea
3. `*t2 = aux;` — pune in al doilea ce era salvat (continutul original al lui t1)

**De ce asa:** swap clasic intre 2 valori. Lucram cu pointeri pentru ca structurile sunt "mari" si vrem sa modificam datele in loc, nu copii.

---

## 3. `initTask` — creeaza un Task nou

```c
Task initTask(char* description, int priority) {
    Task task = { .priority = priority };
    task.description = malloc((strlen(description) + 1) * sizeof(char));
    strcpy(task.description, description);
    return task;
}
```

**Pas cu pas:**
1. `Task task = { .priority = priority };` — initializare partiala (designated initializer). Setezi `priority` direct, `description` ramane nesetat.
2. `task.description = malloc(...)` — aloci memorie pentru string. `strlen + 1` = lungimea + locul pentru `'\0'`.
3. `strcpy(task.description, description)` — copiezi string-ul primit.
4. `return task;` — returnezi structura (e copiata pe stiva apelantului).

**De ce asa:** Cand creezi un Task, vrei o copie proprie a string-ului — nu vrei sa depinzi de pointerul primit. Deep copy = siguranta.

---

## 4. `printTaskToConsole` — afisare

```c
void printTaskToConsole(Task task) {
    printf("%s - priority %d\n", task.description, task.priority);
}
```

Simplu — primeste task by value (copie) si afiseaza.

---

## 5. `initHeap` — heap gol

```c
Heap initHeap() {
    Heap heap;
    heap.size = 0;
    heap.tasks = malloc(heap.size * sizeof(Task));
    return heap;
}
```

**Pas cu pas:**
1. `heap.size = 0;` — pornim cu 0 elemente.
2. `malloc(0 * sizeof(Task))` — aloca 0 bytes (in C standard, returneaza un pointer valid sau NULL — implementare-specific).

**Atentie:** `malloc(0)` poate intoarce NULL pe unele compilatoare. In practica, primul `realloc` (in `insertHeap`) va functiona corect oricum. Daca vrei sa fii sigur, poti scrie `heap.tasks = NULL;` direct.

---

## 6. `insertHeap` — inserezi cu **sift-up**

```c
void insertHeap(Heap* heap, Task task) {

    heap->size += 1;
    heap->tasks = realloc(heap->tasks, heap->size * sizeof(Task));

    int element_index = heap->size - 1;
    heap->tasks[element_index] = task;

    int parent_index = (element_index - 1) / 2;

    while (heap->tasks[element_index].priority > heap->tasks[parent_index].priority) {
        swapTask(&heap->tasks[element_index], &heap->tasks[parent_index]);
        element_index = parent_index;
        parent_index = (element_index - 1) / 2;
    }
}
```

**Pas cu pas:**

**Bloc 1 — alocare loc:**
```c
heap->size += 1;
heap->tasks = realloc(heap->tasks, heap->size * sizeof(Task));
```
Marim heap-ul cu 1 element. `realloc` extinde vectorul (sau il muta intr-o zona mai mare daca trebuie).

**Bloc 2 — pune task-ul la final:**
```c
int element_index = heap->size - 1;
heap->tasks[element_index] = task;
```
`element_index` e ultimul index (size-1). Pui task-ul acolo.

**Bloc 3 — sift-up:**
```c
int parent_index = (element_index - 1) / 2;
while (heap->tasks[element_index].priority > heap->tasks[parent_index].priority) {
    swapTask(...);
    element_index = parent_index;
    parent_index = (element_index - 1) / 2;
}
```
- Calculezi indexul parintelui: `(i-1)/2`.
- **Cat timp** `tasks[i].priority > tasks[parent].priority` (regula de max-heap incalcata) → swap.
- Dupa swap, urca: noul `element_index` e fostul parinte, recalculezi noul parinte.
- Loop-ul se opreste cand `task[i] <= task[parent]` (max-heap respectat) sau cand ajungi la radacina (parent_index = 0, element_index = 0, ele sunt egale → conditia falsa).

**Mnemonic:** "urca pana esti sef sau atingi varful".

**Complexitate:** O(log n) — inaltimea heap-ului.

---

## 7. `printHeap` — afisare prioritati

```c
void printHeap(Heap heap) {
    for (int i = 0; i < heap.size; i++) {
        printf("%d ", heap.tasks[i].priority);
    }
    printf("\n");
}
```

Doar parcurge vectorul si afiseaza prioritatile in ordinea din vector (NU in ordine sortata — heap-ul nu e sortat, doar respecta proprietatea parinte > copii).

---

## 8. `heapify` — sift-down

```c
void heapify(Heap heap, int index) {

    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap.size && heap.tasks[left].priority > heap.tasks[largest].priority) {
        largest = left;
    }

    if (right < heap.size && heap.tasks[right].priority > heap.tasks[largest].priority) {
        largest = right;
    }

    if (largest != index) {
        swapTask(&heap.tasks[largest], &heap.tasks[index]);
        heapify(heap, largest);
    }
}
```

**Pas cu pas:**

**Bloc 1 — gaseste cel mai mare dintre `index`, `stang`, `drept`:**
```c
int largest = index;
int left = 2 * index + 1;
int right = 2 * index + 2;
```
Calculezi indexii copiilor.

**Bloc 2 — verifica daca stangul e mai mare:**
```c
if (left < heap.size && heap.tasks[left].priority > heap.tasks[largest].priority) {
    largest = left;
}
```
- `left < heap.size` — copilul stang exista?
- `tasks[left].priority > tasks[largest].priority` — e mai mare ca actualul `largest`?
- Daca da, `largest = left`.

**Bloc 3 — verifica daca dreptul e mai mare:**
```c
if (right < heap.size && heap.tasks[right].priority > heap.tasks[largest].priority) {
    largest = right;
}
```
La fel, dar pentru drept. Compari cu `largest` (care poate fi acum stangul).

**Bloc 4 — daca am gasit unul mai mare ca radacina:**
```c
if (largest != index) {
    swapTask(&heap.tasks[largest], &heap.tasks[index]);
    heapify(heap, largest);
}
```
- Daca `largest != index`, regula de max-heap e incalcata. Swap.
- Apoi **recursiv**: heapify pe pozitia in care am coborat — pentru ca s-ar putea ca acum acolo regula sa fie iar incalcata.

**Mnemonic:** "gaseste cel mai mare dintre eu+copii, daca nu sunt eu → swap si continua de acolo".

**Complexitate:** O(log n).

---

## 9. `buildHeap` — din vector la heap valid

```c
Heap buildHeap(Task* tasks, int size) {
    Heap heap;
    heap.size = size;
    heap.tasks = malloc(size * sizeof(Task));

    for (int i = 0; i < size; i++) {
        heap.tasks[i] = initTask(tasks[i].description, tasks[i].priority);
    }

    int startIndex = heap.size / 2 - 1;

    for (int i = startIndex; i >= 0; i--) {
        heapify(heap, i);
    }

    return heap;
}
```

**Pas cu pas:**

**Bloc 1 — alocare:**
```c
heap.size = size;
heap.tasks = malloc(size * sizeof(Task));
```

**Bloc 2 — copiaza datele (deep copy via `initTask`):**
```c
for (int i = 0; i < size; i++) {
    heap.tasks[i] = initTask(tasks[i].description, tasks[i].priority);
}
```
Foloseste `initTask` ca sa duplice si string-urile (deep copy).

**Bloc 3 — heapify de la mijloc spre inceput:**
```c
int startIndex = heap.size / 2 - 1;
for (int i = startIndex; i >= 0; i--) {
    heapify(heap, i);
}
```

**De ce de la `size/2 - 1`?** Indexii > size/2 sunt **frunze** — n-au copii, deci heapify pe ele nu schimba nimic. Pornim de pe ultimul nod care **are copii**.

**De ce mergem invers (de la mai mare la 0)?** Heapify presupune ca sub-arborii copiilor sunt deja valizi. Mergand invers, cand ajungi la index `i`, sub-arborele de sub el e deja heapified.

**Complexitate:** O(n) (nu O(n log n)) — un argument matematic subtil.

---

## 10. `deleteFromHeap` — extrage maximul

```c
Task deleteFromHeap(Heap* heap) {

    Task task = heap->tasks[0];

    swapTask(&heap->tasks[0], &heap->tasks[heap->size - 1]);

    heap->size -= 1;
    heap->tasks = realloc(heap->tasks, heap->size * sizeof(Task));

    int startIndex = heap->size / 2 - 1;
    for (int i = startIndex; i >= 0; i--) {
        heapify(*heap, i);
    }

    return task;
}
```

**Pas cu pas:**

**Bloc 1 — salveaza maxul (radacina):**
```c
Task task = heap->tasks[0];
```
Maximul e mereu la index 0 (proprietate de max-heap).

**Bloc 2 — swap cu ultimul element:**
```c
swapTask(&heap->tasks[0], &heap->tasks[heap->size - 1]);
```
Maxul devine ultim, ultimul devine radacina (probabil incorect din punct de vedere heap).

**Bloc 3 — scade size-ul (ignora ultimul, care era maxul):**
```c
heap->size -= 1;
heap->tasks = realloc(heap->tasks, heap->size * sizeof(Task));
```

**Bloc 4 — heapify de la mijloc:**
```c
int startIndex = heap->size / 2 - 1;
for (int i = startIndex; i >= 0; i--) {
    heapify(*heap, i);
}
```

**Atentie:** in cazul standard, e suficient `heapify(*heap, 0)` (doar de la radacina). Codul de la seminar face heapify pe toata jumatatea — e mai mult decat necesar dar functional.

---

## 11. `readTasksFromFile` — citire CSV

```c
void readTasksFromFile(const char* filename, Heap* heap) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Error: cannot open '%s'\n", filename);
        return;
    }

    char buffer[LINESIZE];

    while (fgets(buffer, LINESIZE, f) != NULL) {
        Task task;

        char* token = strtok(buffer, ",");
        task.priority = atoi(token);

        token = strtok(NULL, ",");
        token[strcspn(token, "\n")] = '\0';
        task.description = malloc((strlen(token) + 1) * sizeof(char));
        strcpy(task.description, token);

        insertHeap(heap, task);
    }

    fclose(f);
}
```

**Pas cu pas:**

**Bloc 1 — deschide fisierul:**
```c
FILE* f = fopen(filename, "r");
if (!f) {
    printf("Error: cannot open '%s'\n", filename);
    return;
}
```

**Bloc 2 — citeste linie cu linie:**
```c
while (fgets(buffer, LINESIZE, f) != NULL) {
```
Pattern-ul corect (NU `while (!feof(f))` care e anti-pattern).

**Bloc 3 — parseaza prioritatea (primul camp):**
```c
char* token = strtok(buffer, ",");
task.priority = atoi(token);
```
`strtok` cu `buffer` (prima oara), `atoi` converteste la int.

**Bloc 4 — parseaza descrierea (string, al doilea camp):**
```c
token = strtok(NULL, ",");
token[strcspn(token, "\n")] = '\0';     // scoate '\n'
task.description = malloc((strlen(token) + 1) * sizeof(char));
strcpy(task.description, token);
```
- `strtok(NULL, ",")` — continua tokenizarea liniei curente.
- `strcspn(token, "\n")` — gaseste pozitia primului `\n` (sau lungimea daca nu exista). Setezi `\0` acolo → tai string-ul.
- `malloc + strcpy` = deep copy.

**Bloc 5 — inserezi in heap:**
```c
insertHeap(heap, task);
```

---

## 12. `main`

```c
int main(void) {

    printf("\n--- Heap incarcat din scheduler.txt ---\n");
    Heap heap = initHeap();
    readTasksFromFile("scheduler.txt", &heap);
    printHeap(heap);

    printf("\n--- Dupa insertHeap(Task Z, priority 27) ---\n");
    insertHeap(&heap, initTask("Task Z", 27));
    printHeap(heap);

    printf("\n--- buildHeap dintr-un array de 7 taskuri ---\n");
    Task tasks[7];
    for (int i = 0; i < 7; i++) {
        Task t = { .priority = i, .description = "Task X" };
        tasks[i] = t;
    }
    Heap heap2 = buildHeap(tasks, 7);
    printHeap(heap2);

    printf("\n--- Dupa deleteFromHeap (extrage maximul) ---\n");
    Task deletedTask = deleteFromHeap(&heap2);
    printTaskToConsole(deletedTask);
    free(deletedTask.description);
    printHeap(heap2);

    return 0;
}
```

3 demo-uri:
1. **Citire din fisier + insert** — verifici ca insertHeap face sift-up corect.
2. **buildHeap** dintr-un array secvential — verifici ca heapify-ul de la mijloc construieste max-heap valid.
3. **deleteFromHeap** — verifici ca extragi corect maxul si heap-ul ramane valid.

---

# Functii pentru testul 2

Acestea NU sunt in cod — le scrii TU. Patternuri:

## A. Numarare cu conditie

```c
int countByCondition(Heap heap, /* parametri pentru conditie */) {
    int count = 0;
    for (int i = 0; i < heap.size; i++) {
        if (/* CONDITIA TA — ex. heap.tasks[i].priority > 20 */) {
            count++;
        }
    }
    return count;
}
```

**Idee:** parcurgere LINIARA a vectorului (nu recursivitate). La fiecare task, verifici conditia.

## B. Stergere cu criteriu (cu heapify dupa fiecare)

```c
void deleteByCondition(Heap* heap, /* parametri pentru conditie */) {
    int i = 0;
    while (i < heap->size) {
        if (/* CONDITIA TA */) {
            // swap cu ultimul, scade size, heapify
            swapTask(&heap->tasks[i], &heap->tasks[heap->size - 1]);
            free(heap->tasks[heap->size - 1].description);
            heap->size -= 1;
            heap->tasks = realloc(heap->tasks, heap->size * sizeof(Task));
            heapify(*heap, i);
            // NU incrementa i — noul element de pe pozitia i poate indeplini si el conditia
        } else {
            i++;
        }
    }
}
```

**Atentie:** dupa swap+heapify, pozitia i contine alt element — verifici si pe el. De aia `if-else` cu `i++` doar pe ramura de "nu sterge".

## C. Salvare in alta structura cu filtru

```c
// varianta cu vector
Task* saveMatching(Heap heap, /* parametri */, int* outSize) {
    Task* result = NULL;
    *outSize = 0;
    for (int i = 0; i < heap.size; i++) {
        if (/* CONDITIA TA */) {
            (*outSize)++;
            result = realloc(result, (*outSize) * sizeof(Task));
            // deep copy daca ai string-uri:
            result[*outSize - 1].priority = heap.tasks[i].priority;
            result[*outSize - 1].description = malloc(strlen(heap.tasks[i].description) + 1);
            strcpy(result[*outSize - 1].description, heap.tasks[i].description);
        }
    }
    return result;
}
```

Sau salvare in lista (folosind `addToEnd` din Lab 3 — patternul e in test_patterns.md din Lab_09).

---

# Capcane

| Capcana | Antidot |
|---------|---------|
| `(i-1)/2` cand i=0 da 0 (parinte = el insusi) | OK in cod, while-ul iese pentru ca nu se mai face swap. |
| `realloc(ptr, 0)` cand size devine 0 | Comportament platforma-specific. In lab nu e o problema. |
| Confuzii intre sift-up si sift-down | `insertHeap` urca un element. `heapify` coboara un element. |
| Crezi ca heap-ul e sortat | NU. Doar respecta `parinte >= copii`. La afisarea cu printHeap, NU vezi ordine sortata. |
| Uiti `heapify` recursiv | Daca dupa swap nu mai apelezi heapify, sub-arborele poate ramane invalid. |
| `feof` in loc de `fgets != NULL` | Anti-pattern. Foloseste mereu pattern-ul corect. |

---

# Self-test (fara cod deschis)

- [ ] Pot desena vectorul `[10, 5, 8, 3, 4, 7, 2]` ca arbore in 1 min.
- [ ] Pot scrie `swapTask` si `initTask` in 2 min.
- [ ] Pot scrie `insertHeap` cu sift-up in 5 min.
- [ ] Pot scrie `heapify` recursiv in 5 min.
- [ ] Pot scrie `deleteFromHeap` in 5 min.
- [ ] Pot scrie `buildHeap` cu `size/2 - 1` corect.
- [ ] Pot scrie cele 3 functii pentru testul 2 (count, delete, save) in 15 min.
- [ ] Pot explica de ce `buildHeap` e O(n), nu O(n log n).

Daca bifezi 7/8 -> stapanesti Heap.
