#pragma once

#include "FootballTeam.h"

typedef struct Node {
    FootballTeam* data;
    struct Node* next;
} Node;

typedef struct {
    Node** buckets;
    int size;
} HashTable;

HashTable initHashTable(int size);
unsigned int hash(const char* str, int size);
void insertHashTable(HashTable* ht, FootballTeam* team);
int loadTeams(const char* filename, HashTable* ht);
void printHashTable(const HashTable* ht);
void freeHashTable(HashTable* ht);
