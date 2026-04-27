// Lab 07 — Heap (max-heap pe vector)
// Sursa: S9_28042025/main.c din github.com/tcervinski-csie/DS2025
// Compilare: cl /D_CRT_SECURE_NO_WARNINGS Lab_07.c
// Rulare:   pune scheduler.txt langa exe

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 128

// ============================================================
// 1. STRUCTS — Task + Heap
// ============================================================

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

// ============================================================
// 2. UTIL — swap si init Task
// ============================================================

void swapTask(Task* t1, Task* t2) {
    Task aux = *t1;
    *t1 = *t2;
    *t2 = aux;
}

Task initTask(char* description, int priority) {
    Task task = { .priority = priority };
    task.description = malloc((strlen(description) + 1) * sizeof(char));
    strcpy(task.description, description);
    return task;
}

void printTaskToConsole(Task task) {
    printf("%s - priority %d\n", task.description, task.priority);
}

// ============================================================
// 3. HEAP CORE — init, insert (sift-up), heapify (sift-down)
// ============================================================

Heap initHeap() {
    Heap heap;
    heap.size = 0;
    heap.tasks = malloc(heap.size * sizeof(Task));
    return heap;
}

// Insert: pune elementul la final, apoi "urca" (sift-up) cat timp e mai mare ca parintele
void insertHeap(Heap* heap, Task task) {

    heap->size += 1;
    heap->tasks = realloc(heap->tasks, heap->size * sizeof(Task));

    int element_index = heap->size - 1;
    heap->tasks[element_index] = task;

    int parent_index = (element_index - 1) / 2;

    while (heap->tasks[element_index].priority > heap->tasks[parent_index].priority) {
        // max-heap condition not met -> swap cu parintele
        swapTask(&heap->tasks[element_index], &heap->tasks[parent_index]);
        element_index = parent_index;
        parent_index = (element_index - 1) / 2;
    }
}

void printHeap(Heap heap) {
    for (int i = 0; i < heap.size; i++) {
        printf("%d ", heap.tasks[i].priority);
    }
    printf("\n");
}

// Heapify: "coboara" elementul de la indexul dat pana cand respecta proprietatea de heap
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

// ============================================================
// 4. HEAP OPERATIONS — buildHeap, deleteFromHeap
// ============================================================

// buildHeap: porneste de la un array si aplica heapify de la mijloc spre inceput
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

// deleteFromHeap: extrage radacina (max), pune ultimul in locul ei, scade size, heapify
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

// ============================================================
// 5. LOAD — citire din fisier
// ============================================================
//
// NOTA: codul original (S9) folosea `while (!feof(f))` care e anti-pattern in C
// (citeste o data in plus la EOF cu buffer neinitializat -> crash).
// Aici am inlocuit cu pattern-ul corect: `while (fgets(...) != NULL)`.

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
        token[strcspn(token, "\n")] = '\0';     // scoate '\n' de la final
        task.description = malloc((strlen(token) + 1) * sizeof(char));
        strcpy(task.description, token);

        insertHeap(heap, task);
    }

    fclose(f);
}

// ============================================================
// 6. MAIN
// ============================================================

int main(void) {

    printf("\n--- Heap incarcat din scheduler.txt ---\n");

    Heap heap = initHeap();
    readTasksFromFile("scheduler.txt", &heap);
    printHeap(heap);

    printf("\n--- Dupa insertHeap(Task Z, priority 27) ---\n");
    insertHeap(&heap, initTask("Task Z", 27));
    printHeap(heap);

    printf("\n--- buildHeap dintr-un array de 7 taskuri (priority 0..6) ---\n");
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
