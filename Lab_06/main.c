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
