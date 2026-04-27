// Lab 05 — Doubly Linked List (versiune MINIMALA + free/delete)
//
// Contine ce era in seminar (insertBeginning, insertEnd, loadMovies, printMovie)
// + adaugat: deleteBeginning, deleteEnd, freeList (pattern-uri identice cu Lab 3,
//   merita ~2 minute de invatare in caz de examen).
//
// Ce NU e aici: parseList. (Daca pica, se scrie in 30 sec — vezi README.)
//
// Compilare: gcc Lab_05_minimal.c -o Lab_05_minimal
// Rulare:   pune movies.csv langa executabil, apoi ./Lab_05_minimal

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================
// 1. DATA — Movie
// ============================================================

typedef struct {
    int year;
    char* title;
    int hasOscar;
} Movie;

void printMovie(const Movie* movie) {
    printf("[%d] %s - Oscar: %s\n",
        movie->year, movie->title, movie->hasOscar ? "Yes" : "No");
}

// ============================================================
// 2. LIST — Doubly Linked List (head + tail)
// ============================================================

typedef struct Node {
    Movie* data;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
} List;

typedef void (*InsertListFn)(List*, Movie*);

void insertBeginning(List* list, Movie* movie) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = movie;
    newNode->prev = NULL;
    newNode->next = list->head;

    if (list->head) {
        list->head->prev = newNode;
    }
    else {
        list->tail = newNode;
    }

    list->head = newNode;
}

void insertEnd(List* list, Movie* movie) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = movie;
    newNode->next = NULL;
    newNode->prev = list->tail;

    if (list->tail) {
        list->tail->next = newNode;
    }
    else {
        list->head = newNode;
    }

    list->tail = newNode;
}

void deleteBeginning(List* list) {
    if (list->head) {
        Node* toDelete = list->head;
        list->head = list->head->next;

        if (list->head) {
            list->head->prev = NULL;
        }
        else {
            list->tail = NULL;
        }

        free(toDelete->data->title);
        free(toDelete->data);
        free(toDelete);
    }
}

void deleteEnd(List* list) {
    if (list->tail) {
        Node* toDelete = list->tail;
        list->tail = list->tail->prev;

        if (list->tail) {
            list->tail->next = NULL;
        }
        else {
            list->head = NULL;
        }

        free(toDelete->data->title);
        free(toDelete->data);
        free(toDelete);
    }
}

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

// ============================================================
// 3. LOADER — CSV -> Movie -> lista
// ============================================================

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

// ============================================================
// 4. MAIN (la fel ca in seminar — fara parseList, fara freeList)
// ============================================================

int main(void) {

    List list = { NULL, NULL };
    int count = loadMovies("movies.csv", &list, insertEnd);

    if (count < 0) {
        printf("Failed to load movies.\n");
        return 1;
    }

    printf("Loaded %d movies.\n", count);

    freeList(&list);
    return 0;
}
