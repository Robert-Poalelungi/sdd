#pragma once

typedef struct {
    char* description;
    int priority;
} Task;

void printTask(Task task);
