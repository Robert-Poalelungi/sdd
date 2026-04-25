# Lab 06 — Hash Table (Chaining)

**Ce inveti:** hash table = **array de liste simplu inlantuite**. Cautare / insert medie O(1).
**Pre-requisites:** Lab 3 fluent (bucket-ul e exact o lista din Lab 3).
**Timp estimat:** 60-90 min prima data.

---

## Ideea in 30 de secunde

- Ai un array de „buckets" (e.g. 47 de bucket-uri)
- Calculezi `hash(cheie) % size` → un numar intre 0 si 46
- Inserezi in bucket-ul cu indexul ala (in lista simpla de acolo)
- Cand cauti: acelasi hash → mergi direct la bucket → lista scurta → **medie O(1)**

**Schema mentala:**
```
buckets[0]  -> NULL
buckets[1]  -> [TeamA] -> [TeamB] -> NULL     <- „coliziune" (2 chei au hash-uit la 1)
buckets[2]  -> NULL
buckets[3]  -> [TeamC] -> NULL
...
buckets[46] -> [TeamD] -> [TeamE] -> [TeamF] -> NULL
```

**„Coliziune" = 2 chei diferite ajung in acelasi bucket.** Rezolvarea prin **chaining** = lista la bucket-ul respectiv. Alta varianta (open addressing) nu o facem aici.

---

## Pas 1 — Hash function (15 min)

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

**De ce 31?** Numar prim mic → distributie buna a bit-ilor. Alegere clasica (Java `String.hashCode()` la fel).

**De ce `% size`?** Incadrezi rezultatul in `[0, size-1]`, adica indexi validi pentru array.

**De ce size prim (47)?** Reduce coliziunile. Numerele non-prime tind sa creeze patternuri in distributie.

**Scrie functia singur pe hartie** si calculeaza `hash("ab", 10)` mental:
- `h=0`; `*str='a'=97`; `h = 0*31 + 97 = 97`; `str++`
- `*str='b'=98`; `h = 97*31 + 98 = 3105`; `str++`
- `*str='\0'` → iesi
- `return 3105 % 10 = 5`

---

## Pas 2 — Struct HashTable (5 min)

```c
typedef struct {
    Node** buckets;   // array de pointeri la Node (capul fiecarei liste)
    int size;         // cate bucket-uri
} HashTable;
```

**De ce `Node**`?** Fiecare bucket e `Node*` (capul unei liste). Un array de `Node*` → `Node**`.

**Nodul e identic cu Lab 3**:
```c
typedef struct Node { FootballTeam* data; struct Node* next; } Node;
```

---

## Pas 3 — `initHashTable` (10 min)

```c
HashTable initHashTable(int size) {
    HashTable ht;
    ht.size = size;
    ht.buckets = malloc(size * sizeof(Node*));       // aloca array-ul
    for (int i = 0; i < size; i++)
        ht.buckets[i] = NULL;                         // CRUCIAL: init la NULL
    return ht;
}
```

**CAPCANA MORTALA:** daca NU initializezi `buckets[i] = NULL`, atunci la primul `insert` intr-un bucket, `newNode->next = ht->buckets[i]` va pointa la gunoaie → crash la primul `print`.

---

## Pas 4 — `insertHashTable` = `addToBeginning` pe bucket (10 min)

Identic cu `addToBeginning` din Lab 3, doar ca alegi bucket-ul dupa hash:

```c
void insertHashTable(HashTable* ht, FootballTeam* team) {
    unsigned int index = hash(team->country, ht->size);

    Node* newNode = malloc(sizeof(Node));
    newNode->data = team;
    newNode->next = ht->buckets[index];      // fostul cap al bucket-ului
    ht->buckets[index] = newNode;             // noul cap al bucket-ului
}
```

**De ce la inceput si nu la sfarsit?** Pentru ca e **O(1)**. Ordinea in bucket nu conteaza (oricum cauti prin toata lista). Daca ai insera la sfarsit, ai parcurge lista → O(lungime bucket).

---

## Pas 5 — `loadTeams` + scoaterea `\n` (15 min)

Scheletul CSV e identic cu lab 3/4/5. **O singura diferenta importanta:**

```c
char* nl = strchr(line, '\n');
if (nl) *nl = '\0';                 // scoate '\n' de la sfarsit
```

**De ce?** `fgets` pastreaza `\n` in `line`. Ultimul camp ar contine `"...\n"`. Daca acel camp e cheia de hash, `hash("Mexico")` si `hash("Mexico\n")` dau valori diferite → nu gasesti cheia cand cauti.

Aici cheia e `country` (primul camp), dar defensiv tot scoti `\n` la inceput.

**Alt detaliu:** grupa e un `char`, nu string:
```c
team->group = token[0];     // nu malloc+strcpy, ci primul caracter
```

---

## Pas 6 — `printHashTable` (10 min)

Pattern nou: **`for` peste bucket-uri + `while` peste lista din fiecare**:

```c
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
```

---

## Pas 7 — `freeHashTable` (10 min)

Free in **2 nivele**:
1. Pentru fiecare bucket → free lista din bucket (ca la Lab 3)
2. La sfarsit → free array-ul `buckets`

```c
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
    free(ht->buckets);        // IMPORTANT: elibereaza si array-ul
    ht->buckets = NULL;
    ht->size = 0;
}
```

**Capcana:** daca uiti `free(ht->buckets)` la final, pierzi memorie. Array-ul a fost alocat cu `malloc` in `init` → trebuie `free` la final.

---

## Pas 8 — `main` + Lab 6 integral (15 min)

```c
int main(void) {
    HashTable ht = initHashTable(47);
    loadTeams("teams.csv", &ht);
    printHashTable(&ht);
    freeHashTable(&ht);
    return 0;
}
```

Scrie Lab 6 integral de la zero. Verifica output: 48 de echipe incarcate, vei vedea cateva bucket-uri cu 2-3 echipe (coliziuni — normal).

---

## Patternuri NOI vs Lab 3-5

```c
// 1. Struct HashTable
typedef struct { Node** buckets; int size; } HashTable;

// 2. Hash function pe string
unsigned int h = 0;
while (*str) { h = h * 31 + *str; str++; }
return h % size;

// 3. Init (aloca si NULL everywhere)
ht.buckets = malloc(size * sizeof(Node*));
for (i) ht.buckets[i] = NULL;

// 4. Insert = addToBeginning pe bucket[index]
int i = hash(key, size);
newNode->next = ht->buckets[i];
ht->buckets[i] = newNode;

// 5. Scoate `\n` din linia CSV
char* nl = strchr(line, '\n'); if (nl) *nl = '\0';

// 6. Double iteration (buckets + lista)
for (i) { for (aux = buckets[i]; aux; aux = aux->next) ... }

// 7. Free cu 2 nivele
for (i) { free lista; } free(buckets);
```

---

## Complexitati hash table

| Operatie | Medie | Worst-case |
|----------|-------|------------|
| Insert   | O(1)  | O(n) — toate in acelasi bucket |
| Search   | O(1)  | O(n) |
| Delete   | O(1)  | O(n) |

**De ce worst-case O(n)?** Daca toate cheile nimeresc in acelasi bucket (hash prost sau `size` mic), hash table devine practic o lista simpla.

**Regula pragmatica:** `size` ≈ de 2x numarul asteptat de elemente, si **prim**.

---

## Capcane

| Capcana | Antidot |
|---------|---------|
| Nu initializezi `buckets[i] = NULL` | Primul `insert` crapa. Mereu for-NULL dupa `malloc`. |
| Uiti `free(ht->buckets)` | Memory leak. Array-ul a fost alocat, trebuie eliberat. |
| Nu scoti `\n` din CSV si hash-uiesti cheia care contine `\n` | Cauti „Mexico", dar ai inserat „Mexico\n" → miss. |
| Insert la sfarsitul bucket-ului | OK functional, dar O(n). Pierzi avantajul hash table. |
| `size` non-prim sau prea mic | Coliziuni masive. Lab = 47, bine. 10 sau 100 = prost. |
| `team->group = token` (char* in loc de char) | Grupa e UN caracter. Scrie `token[0]`. |

---

## Self-test (fara sa te uiti la cod)

- [ ] Pot scrie `hash` in 3 min (h*31 + *str, apoi `% size`).
- [ ] Pot scrie `initHashTable` corect, cu initializare NULL la buckets.
- [ ] Pot scrie `insertHashTable` in 3 min (= addToBeginning pe bucket[hash%size]).
- [ ] Pot scrie `freeHashTable` cu cele 2 nivele (free liste + free array).
- [ ] Pot explica de ce hash table e „array de liste" si nu o singura lista mare.
- [ ] Pot explica de ce scot `\n` din CSV si ce s-ar intampla daca nu l-as scoate.

Daca bifezi toate 6 → stapanesti Lab 6. Treci la Lab 7 (Heap + scheduler).
