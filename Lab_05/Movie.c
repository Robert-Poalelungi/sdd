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
    printf("[%d] %s - Oscar: %s\n", movie->year, movie->title, movie->hasOscar ? "Yes" : "No");
}
