# Lab 06 — Hash Table

**Concept:** hash table = **array de buckets**, fiecare bucket e o **listă simplă** (chaining pentru coliziuni).

**Insert:**
1. Calculezi `hash(cheie)`
2. `index = hash % size`
3. Inserezi la începutul listei din `buckets[index]`

**Search:** același hash → mergi direct în bucket → parcurgi lista scurtă → **medie O(1)**.

---

## Structura fișierelor

```
FootballTeam.h   → struct FootballTeam + prototip printTeam
FootballTeam.c   → implementare printTeam
HashTable.h      → Node + HashTable + prototipuri
HashTable.c      → implementare hash, init, insert, load, print, free
main.c           → folosește hash table
```

---

## PAS 1 — `FootballTeam.h`

```c
#pragma once

typedef struct {
    char* country;
    char group;
    int matchesPlayed;
} FootballTeam;

void printTeam(const FootballTeam* team);
```

---

## PAS 2 — `HashTable.h`

```c
#pragma once

#include "FootballTeam.h"

typedef struct Node {
    FootballTeam* data;
    struct Node* next;
} Node;

typedef struct {
    Node** buckets;       // array de pointeri la Node (capul fiecărei liste)
    int size;             // numărul de bucket-uri
} HashTable;

HashTable initHashTable(int size);
unsigned int hash(const char* str, int size);
void insertHashTable(HashTable* ht, FootballTeam* team);
int loadTeams(const char* filename, HashTable* ht);
void printHashTable(const HashTable* ht);
void freeHashTable(HashTable* ht);
```

Schema mentală:
```
buckets[0] → NULL
buckets[1] → [Team A] → [Team B] → NULL
buckets[2] → NULL
buckets[3] → [Team C] → NULL
...
buckets[46] → [Team D] → [Team E] → [Team F] → NULL
```

---

## PAS 3 — Hash function

```c
unsigned int hash(const char* str, int size) {
    unsigned int h = 0;
    while (*str) {
        h = h * 31 + *str;
        str++;
    }
    return h % size;
}
```

- **`31`** — număr prim mic, distribuție bună
- **`% size`** — îl încadrează în `[0, size-1]`
- **size prim (47)** — reduce coliziunile

---

## PAS 4 — `initHashTable`

```c
HashTable initHashTable(int size) {
    HashTable ht;
    ht.size = size;
    ht.buckets = malloc(size * sizeof(Node*));
    for (int i = 0; i < size; i++) {
        ht.buckets[i] = NULL;         // toate bucket-urile goale
    }
    return ht;
}
```

**Important:** inițializezi fiecare bucket cu `NULL` (altfel conține gunoaie → crash).

---

## PAS 5 — `insertHashTable`

```c
void insertHashTable(HashTable* ht, FootballTeam* team) {
    unsigned int index = hash(team->country, ht->size);

    Node* newNode = malloc(sizeof(Node));
    newNode->data = team;
    newNode->next = ht->buckets[index];   // leagă de fostul cap al bucket-ului
    ht->buckets[index] = newNode;          // noul cap al bucket-ului
}
```

E pur și simplu `addToBeginning` pe lista din bucket-ul respectiv. **O(1)**.

---

## PAS 6 — `loadTeams`

```c
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
        if (nl) *nl = '\0';                    // scoate '\n' de la final

        FootballTeam* team = malloc(sizeof(FootballTeam));

        char* token = strtok(line, ",");
        team->country = malloc(strlen(token) + 1);
        strcpy(team->country, token);

        token = strtok(NULL, ",");
        team->group = token[0];                // grupa = un singur char

        token = strtok(NULL, ",");
        team->matchesPlayed = atoi(token);

        insertHashTable(ht, team);
        count++;
    }

    fclose(f);
    return count;
}
```

**Observații noi:**
- `team->group = token[0]` — grupa e `char`, nu string
- **Scoți `\n`** din linie, altfel strings cu „`\n`" la final produc hash-uri diferite

---

## PAS 7 — `printHashTable`

```c
void printHashTable(const HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        if (ht->buckets[i]) {                   // doar bucket-urile nenule
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
```

**Pattern:** `for` peste bucket-uri + `while` peste lista din fiecare.

---

## PAS 8 — `freeHashTable`

```c
void freeHashTable(HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        Node* current = ht->buckets[i];
        while (current) {
            Node* next = current->next;
            free(current->data->country);      // string din FootballTeam
            free(current->data);                // FootballTeam
            free(current);                      // Nodul
            current = next;
        }
    }
    free(ht->buckets);                          // array-ul de bucket-uri
    ht->buckets = NULL;
    ht->size = 0;
}
```

**Ordinea eliberării:**
1. Pentru fiecare bucket: eliberezi lista (strings → team → nod)
2. La final: eliberezi **array-ul de buckets**

---

## PAS 9 — `main.c`

```c
#include <stdio.h>
#include "FootballTeam.h"
#include "HashTable.h"

int main() {
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
```

---

## Cheat-sheet Lab 06

```c
// 1. Struct hash table
typedef struct {
    Node** buckets;
    int size;
} HashTable;

// 2. Hash function (string)
unsigned int h = 0;
while (*str) { h = h * 31 + *str; str++; }
return h % size;

// 3. Init - alocă buckets[] și pune NULL peste tot
ht.buckets = malloc(size * sizeof(Node*));
for (i) ht.buckets[i] = NULL;

// 4. Insert = addToBeginning pe bucket[index]
int index = hash(key, size);
newNode->next = ht->buckets[index];
ht->buckets[index] = newNode;

// 5. Free = free la fiecare listă + free(buckets)
for (i) { while (current) free all + current = next; }
free(ht->buckets);
```

## Complexități hash table

| Operație | Medie | Worst-case |
|----------|-------|------------|
| Insert   | O(1)  | O(n) — toate în același bucket |
| Search   | O(1)  | O(n) |
| Delete   | O(1)  | O(n) |

**De ce worst-case O(n)?** Dacă toate cheile nimeresc în același bucket (hash function prost sau size mic), hash table devine o listă simplă.
