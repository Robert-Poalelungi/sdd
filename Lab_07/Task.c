#include <stdio.h>
#include "Task.h"

void printTask(Task task) {
    printf("[Priority %d] %s\n", task.priority, task.description);
}
