#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Heap.h"

int loadTasks(const char* filename, Heap* heap) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: could not open file '%s'\n", filename);
        return -1;
    }

    char line[128];
    int count = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        Task task;

        char* token = strtok(line, ",");
        task.priority = atoi(token);

        token = strtok(NULL, ",");
        token[strcspn(token, "\n")] = '\0';
        task.description = malloc(strlen(token) + 1);
        strcpy(task.description, token);

        insertHeap(heap, task);
        count++;
    }

    fclose(f);
    return count;
}
