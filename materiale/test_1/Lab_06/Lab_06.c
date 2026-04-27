// Lab 06 — Hash Table (one-file)
// Compilare: gcc Lab_06.c -o Lab_06
// Rulare:   pune teams.csv langa executabil, apoi ./Lab_06

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================
// 1. DATA — FootballTeam
// ============================================================

typedef struct {
    char* country;
    char group;
    int matchesPlayed;
} FootballTeam;

void printTeam(const FootballTeam* team) {
    printf("Group %c | %s - Matches played: %d\n",
        team->group, team->country, team->matchesPlayed);
}

// ============================================================
// 2. HASH TABLE — array de buckets (liste simple inlantuite)
// ============================================================

typedef struct Node {
    FootballTeam* data;
    struct Node* next;
} Node;

typedef struct {
    Node** buckets;
    int size;
} HashTable;

unsigned int hash(const char* str, int size) {
    unsigned int h = 0;
    while (*str) {
        h = h * 31 + *str;
        str++;
    }
    return h % size;
}

HashTable initHashTable(int size) {
    HashTable ht;
    ht.size = size;
    ht.buckets = malloc(size * sizeof(Node*));
    for (int i = 0; i < size; i++) {
        ht.buckets[i] = NULL;
    }
    return ht;
}

void insertHashTable(HashTable* ht, FootballTeam* team) {
    unsigned int index = hash(team->country, ht->size);

    Node* newNode = malloc(sizeof(Node));
    newNode->data = team;
    newNode->next = ht->buckets[index];
    ht->buckets[index] = newNode;
}

void printHashTable(const HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        if (ht->buckets[i]) {
            printf("Bucket %d:\n", i);
            Node* aux = ht->buckets[i];
            while (aux) {
                printf("  ");
                printTeam(aux->data);
                aux = aux->next;
            }
        }
    }
}

void freeHashTable(HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        Node* current = ht->buckets[i];
        while (current) {
            Node* next = current->next;
            free(current->data->country);
            free(current->data);
            free(current);
            current = next;
        }
    }
    free(ht->buckets);
    ht->buckets = NULL;
    ht->size = 0;
}

// ============================================================
// 3. LOADER — CSV -> FootballTeam -> hash table
// ============================================================

int loadTeams(const char* filename, HashTable* ht) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: could not open file '%s'\n", filename);
        return -1;
    }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), f) != NULL) {

        char* nl = strchr(line, '\n');
        if (nl) *nl = '\0';

        FootballTeam* team = malloc(sizeof(FootballTeam));

        char* token = strtok(line, ",");
        team->country = malloc(strlen(token) + 1);
        strcpy(team->country, token);

        token = strtok(NULL, ",");
        team->group = token[0];

        token = strtok(NULL, ",");
        team->matchesPlayed = atoi(token);

        insertHashTable(ht, team);
        count++;
    }

    fclose(f);
    return count;
}

// ============================================================
// 4. MAIN
// ============================================================

int main(void) {

    HashTable ht = initHashTable(47);
    int count = loadTeams("teams.csv", &ht);

    if (count < 0) {
        printf("Failed to load teams.\n");
        return 1;
    }

    printf("Loaded %d teams.\n\n", count);
    printHashTable(&ht);

    freeHashTable(&ht);
    return 0;
}
