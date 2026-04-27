#include <stdio.h>
#include "Movie.h"
#include "DoublyLinkedList.h"

int main() {

    List list = { NULL, NULL };
    int count = loadMovies("movies.csv", &list, insertEnd);

    if (count < 0) {
        printf("Failed to load movies.\n");
        return 1;
    }

    printf("Loaded %d movies.\n", count);

    return 0;
}
