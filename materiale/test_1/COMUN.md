# Partea comuna + ce adauga fiecare lab

**Ideea:** ~70% din cod e identic in toate 4 laburile. Inveti UN scheletul de mai jos, apoi doar diff-ul pe fiecare lab.

---

## PARTEA COMUNA (identica in Lab 3, 4, 5, 6)

### 1. Headere

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
```

### 2. Struct de date — un `char*` + campuri simple

```c
typedef struct {
    int   campIntreg;     // atoi la parsing
    char* campString;     // malloc + strcpy la parsing
    float campFloat;      // atof la parsing
} Data;
```

Exemple:
- Lab 3/4: `F1Car` = `int number, char* driver, char* team, float engine_hp`
- Lab 5: `Movie` = `int year, char* title, int hasOscar`
- Lab 6: `FootballTeam` = `char* country, char group, int matchesPlayed`

### 3. Functie `print`

```c
void printX(const Data* x) {
    printf("...", x->camp1, x->camp2, ...);
}
```

### 4. Nodul listei (Lab 3, 4, 6 — identic)

```c
typedef struct Node {
    Data* data;
    struct Node* next;
} Node;
```

**Lab 5 adauga `prev`** (vezi diff-ul mai jos).

### 5. Parcurgere lista — IDENTIC in toate

```c
while (aux) {
    printX(aux->data);
    aux = aux->next;
}
```

### 6. Free lista — IDENTIC ca schelet

```c
while (current) {
    Node* next = current->next;        // SALVEAZA next inainte de free
    free(current->data->string1);      // 1. toate string-urile din struct
    free(current->data->string2);      // (cate are)
    free(current->data);               // 2. struct-ul
    free(current);                     // 3. nodul
    current = next;
}
```

### 7. Parsing CSV — IDENTIC ca schelet

```c
FILE* f = fopen(filename, "r");
if (f == NULL) { printf("Error: ..."); return -1; }

char line[256];
int count = 0;

while (fgets(line, sizeof(line), f) != NULL) {

    Data* x = malloc(sizeof(Data));

    char* token = strtok(line, ",");           // PRIMUL strtok cu line
    x->campIntreg = atoi(token);

    token = strtok(NULL, ",");                 // RESTUL cu NULL
    x->campString = malloc(strlen(token) + 1);
    strcpy(x->campString, token);

    token = strtok(NULL, ",");
    x->campFloat = (float)atof(token);

    insertFn(container, x);                    // insereaza in container
    count++;
}

fclose(f);
return count;
```

### 8. `main` — IDENTIC ca schelet

```c
int main(void) {
    Container c = init();                        // sau NULL / {NULL,NULL}
    int count = loadX("fisier.csv", &c, insertFn);

    if (count < 0) return 1;

    parseList(c);      // sau parseList(&c)
    freeList(&c);
    return 0;
}
```

---

## CE ADAUGA FIECARE LAB (doar diff-ul)

### Lab 3 — baseline

**Container:** `Node* list = NULL;`

**Functii specifice:**
```c
void addToBeginning(Node** list, F1Car* car);
void addToEnd(Node** list, F1Car* car);     // O(n) — parcurge pana la ultimul
void parseList(Node* list);
void freeList(Node** list);
```

**Regula cheie:** `Node**` cand modifici capul, `Node*` cand doar citesti.

**Extra minim** peste scheletul comun: doar cele 4 functii de sus.

---

### Lab 4 — Lab 3 + 3 functii noi + function pointers

**Diff vs Lab 3:**

1. **Typedef pentru function pointer:**
```c
typedef void (*InsertListFn)(Node**, F1Car*);
```

2. **`loadCars` primeste `InsertListFn`:**
```c
int loadCars(const char* filename, Node** list, InsertListFn insertList);
// In loop: insertList(list, car);  // in loc de addToEnd(list, car)
```

3. **3 functii noi de stergere:**
```c
void deleteFromBeginning(Node** list);
void deleteFromEnd(Node** list);                                  // while (aux->next->next)
void deleteByCondition(Node** list, int (*cond)(F1Car*));         // cu prev+aux
```

4. **Functii de conditie in `main`:**
```c
int isRedBull(F1Car* car) { return strcmp(car->team, "Red Bull") == 0; }
```

**Cheie noua:** pattern `prev + aux` pentru stergere din mijlocul listei.

---

### Lab 5 — STRUCTURAL DIFERIT de Lab 3/4

**Nu mai folosesti `Node**` nicaieri.** Lista devine un struct.

**Diff vs Lab 3/4:**

1. **Nodul capata `prev`:**
```c
typedef struct Node {
    Movie* data;
    struct Node* prev;      // NOU
    struct Node* next;
} Node;
```

2. **Container nou — `List` cu head + tail:**
```c
typedef struct {
    Node* head;
    Node* tail;
} List;
```

3. **Toate functiile iau `List*` (nu `Node**`):**
```c
void insertBeginning(List* list, Movie* movie);
void insertEnd(List* list, Movie* movie);       // acum O(1)! ai direct tail
void parseList(List* list);
void deleteBeginning(List* list);
void deleteEnd(List* list);                      // acum O(1)! ai direct tail->prev
void freeList(List* list);
```

4. **Init in main:**
```c
List list = { NULL, NULL };
```

**Cheia:** simetrie. `insertBeginning` si `insertEnd` sunt oglinda (`head`↔`tail`, `prev`↔`next`). La fel `deleteBeginning` si `deleteEnd`.

**4 legaturi** de gestionat la insert: `newNode->prev`, `newNode->next`, `head->prev` (sau `tail->next`), `head` (sau `tail`).

---

### Lab 6 — STRUCTURAL DIFERIT: container e un array de liste

**Ideea:** `HashTable` = array de `Node*`, fiecare bucket e o lista simpla (ca Lab 3).

**Diff vs Lab 3:**

1. **Nodul — identic cu Lab 3.**

2. **Container nou:**
```c
typedef struct {
    Node** buckets;         // array de pointeri la Node
    int size;
} HashTable;
```

3. **Functie noua — `hash`:**
```c
unsigned int hash(const char* str, int size) {
    unsigned int h = 0;
    while (*str) { h = h * 31 + *str; str++; }
    return h % size;
}
```

4. **Init — aloca array-ul si pune NULL peste tot:**
```c
HashTable initHashTable(int size) {
    HashTable ht;
    ht.size = size;
    ht.buckets = malloc(size * sizeof(Node*));
    for (int i = 0; i < size; i++) ht.buckets[i] = NULL;
    return ht;
}
```

5. **Insert = `addToBeginning` pe bucket-ul potrivit:**
```c
void insertHashTable(HashTable* ht, FootballTeam* team) {
    unsigned int index = hash(team->country, ht->size);
    Node* newNode = malloc(sizeof(Node));
    newNode->data = team;
    newNode->next = ht->buckets[index];
    ht->buckets[index] = newNode;
}
```

6. **Print/Free = `for` peste buckets + `while` peste lista din fiecare:**
```c
for (int i = 0; i < ht->size; i++) {
    Node* current = ht->buckets[i];
    while (current) { ... }
}
free(ht->buckets);    // la final, elibereaza si array-ul
```

7. **Parsing in plus:** `team->group = token[0]` (char unic, nu string). Si scoate `\n`-ul cu `strchr(line, '\n')`.

**Cheie noua:** bucket-ul e lista din Lab 3; `insert` = `addToBeginning(&buckets[i], data)`.

---

## Tabel rezumativ

| Element                | Lab 3       | Lab 4          | Lab 5              | Lab 6               |
|------------------------|-------------|----------------|--------------------|---------------------|
| Node                   | `data+next` | `data+next`    | `data+prev+next`   | `data+next`         |
| Container              | `Node*`     | `Node*`        | `List{head,tail}`  | `HashTable{**,size}`|
| Pasare la functii      | `Node**`    | `Node**`       | `List*`            | `HashTable*`        |
| Insert end             | O(n)        | O(n)           | **O(1)**           | —                   |
| Function pointers      | nu          | **da**         | da (insertFn)      | nu                  |
| Operatii de stergere   | doar free   | **+3 functii** | deleteBegin/End    | doar free           |
| Hash function          | nu          | nu             | nu                 | **da**              |

---

## Cum sa inveti eficient

1. **Scrie scheletul comun** (sectiunile 1-8 de sus) pe hartie. De 2-3 ori. Pana curge.
2. **Lab 3 = scheletul + 4 functii.** Scrie-l de la 0 fara sa te uiti.
3. **Lab 4 = Lab 3 + typedef InsertListFn + 3 functii noi.** Scrie doar delta.
4. **Lab 5 = rescrie totul cu `List{head,tail}` + `prev`.** Observa simetria begin↔end.
5. **Lab 6 = array de Lab 3 + hash.** Insert = `addToBeginning` pe bucket.

Daca stii scheletul comun pe de rost, fiecare lab se reduce la 20-30 de linii de memorat.
