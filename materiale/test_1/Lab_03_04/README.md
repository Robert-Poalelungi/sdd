# Lab 03 + 04 — Singly Linked List complet

**Ideea:** Lab 4 e **un superset** al Lab 3. Un singur fisier acopera ambele = o singura lectie de invatat.

**Pre-requisites:** pointeri, malloc/free, struct-uri.
**Timp estimat:** 2-2.5h pentru tot.

---

## De ce un singur lab?

```
Lab 4 = Lab 3 + { typedef InsertListFn, deleteFromBeginning,
                  deleteFromEnd, deleteByCondition }
```

Daca stii Lab 4, automat stii Lab 3. La examen, dupa ce citesti subiectul, scrii **doar functiile cerute** — nu trebuie sa decizi „e Lab 3 sau Lab 4?".

---

## Map mental: 7 functii in 2 grupe

**Grupa A — Lab 3 (cele de baza):**
1. `addToBeginning(Node** list, F1Car* car)`
2. `addToEnd(Node** list, F1Car* car)`
3. `parseList(Node* list)`
4. `freeList(Node** list)`

**Grupa B — Lab 4 (extra peste Lab 3):**
5. `deleteFromBeginning(Node** list)`
6. `deleteFromEnd(Node** list)`
7. `deleteByCondition(Node** list, int (*cond)(F1Car*))`

Plus **typedef `InsertListFn`** si `loadCars` care primeste functia de insert ca argument.

---

## Pași de invatare (in ordinea dificultatii)

### Bloc 1 — Fundatia (45 min)

1. **`addToBeginning`** (5 min) — cea mai simpla. 4 linii.
2. **`addToEnd`** (10 min) — 2 cazuri (lista goala / nu). **Capcana:** `while (aux->next)`, NU `while (aux)`.
3. **`parseList`** (5 min) — `while (list) { print; list = list->next; }`
4. **`freeList`** (10 min) — interior -> exterior, salveaza `next` inainte de `free`.
5. **`loadCars`** (15 min) — pattern strtok: prima oara cu `line`, apoi cu `NULL`.

**Checkpoint:** poti scrie un `main` care incarca CSV, parcurge si elibereaza? Compileaza si ruleaza.

### Bloc 2 — Stergere (45 min)

6. **`deleteFromBeginning`** (10 min) — 3 pasi: salveaza vechiul cap, muta capul, free vechi.
7. **`deleteFromEnd`** (15 min) — **2 sub-cazuri**: ≥2 elem (parcurgi pana la **penultim** cu `while (aux->next->next)`) / 1 elem (free direct si `*list = NULL`).
8. **`deleteByCondition`** (20 min) — **CEA MAI GREA**. Pattern `prev + aux`, 3 ramuri:
   - cond false → `prev = aux; aux = aux->next;`
   - cond true + prev != NULL → `prev->next = aux->next; free(aux); aux = prev->next;`
   - cond true + prev == NULL → `*list = aux->next; free(aux); aux = *list;`

**Atentie subtila:** dupa stergere, `prev` **NU avanseaza** (nodul curent a disparut, urmatorul devine `aux`).

### Bloc 3 — Function pointers (15 min)

9. **`typedef void (*InsertListFn)(Node**, F1Car*);`**
10. **`loadCars`** primeste `InsertListFn insertList` si apeleaza `insertList(list, car)` in loc de `addToEnd(list, car)`.
11. **Functii de conditie in `main`:** `int isRedBull(F1Car* car) { return strcmp(...) == 0; }`

### Bloc 4 — Drill final (30 min)

Fisier gol. Scrie tot `Lab_03_04.c` de la zero. Max 3 peek-uri. Compileaza, ruleaza, verifica.

---

## Patternuri esentiale (memoreaza-le pe hartie)

```c
// 1. Nod nou
Node* n = malloc(sizeof(Node));
n->data = x;
n->next = NULL;            // sau *list pentru addToBeginning

// 2. Parcurgere pana PE ultim
while (aux->next) aux = aux->next;

// 3. Parcurgere pana PE penultim (pt. deleteFromEnd)
while (aux->next->next) aux = aux->next;

// 4. Parcurgere toata lista
while (aux) { ...; aux = aux->next; }

// 5. Free nod (interior -> exterior)
free(n->data->str1); free(n->data->str2); free(n->data); free(n);

// 6. Salvare next inainte de free
Node* next = current->next; free(current); current = next;

// 7. Pattern prev+aux pentru deleteByCondition
Node* aux = *list;
Node* prev = NULL;
while (aux) {
    if (cond(aux->data)) {
        if (prev) prev->next = aux->next;
        else      *list = aux->next;
        // free(aux), apoi aux = (prev ? prev->next : *list)
    } else {
        prev = aux;
        aux = aux->next;
    }
}

// 8. Typedef function pointer
typedef void (*InsertListFn)(Node**, F1Car*);
typedef int  (*ConditionFn)(F1Car*);

// 9. Apel function pointer (ca orice functie)
insertList(list, car);
conditionFn(aux->data);
```

---

## Capcane (din experienta + bug-ul tau de azi seara)

| Capcana | Antidot |
|---------|---------|
| `while (aux)` la `addToEnd` -> CRASH (NULL->next) | Mereu `while (aux->next)` ca sa stai PE ultim. |
| `while (aux->next->next)` pe lista cu 1 elem | Verifica intai `if ((*list)->next)`. |
| Dupa stergere in mijloc, avansezi `prev` | Nu. `prev` ramane, doar `aux` trece mai departe. |
| Free in ordine gresita | Mereu interior -> exterior. Strings, struct, nod. |
| `&list` vs `list` in `main` | Modific cap -> `&list`. Doar citesc -> `list`. |
| `+1` la `malloc(strlen(token)+1)` | Pentru `'\0'`. |
| `#include <string.h>` lipsa pentru `strcmp` | Pune-l mereu daca folosesti strcmp/strcpy/strlen. |

---

## Self-test final (fara sa te uiti la cod, sub 3 minute fiecare)

- [ ] `addToBeginning` (3 min)
- [ ] `addToEnd` cu cele 2 cazuri (4 min)
- [ ] `parseList` (2 min)
- [ ] `freeList` (3 min)
- [ ] `deleteFromBeginning` (3 min)
- [ ] `deleteFromEnd` cu cele 2 sub-cazuri (5 min)
- [ ] `deleteByCondition` cu cele 3 ramuri (8 min)
- [ ] `typedef InsertListFn` + `loadCars` modificat (5 min)
- [ ] `main` cu functii de conditie si apeluri `deleteByCondition` (3 min)

**Total drill: ~35 min.** Daca le faci pe toate sub timp si compileaza din prima -> Lab 3 si Lab 4 sunt rezolvate. Treci la Lab 5.

---

## Daca subiectul de examen e clar Lab 3 sau clar Lab 4

- **„Implementeaza singly linked list cu add la inceput/sfarsit"** -> Lab 3 (grupa A)
- **„Adauga functie de stergere dupa criteriu"** -> Lab 4 (grupa B)
- **„Foloseste function pointer pentru a parametriza insertul"** -> Lab 4 (typedef + loadCars cu InsertListFn)
- **Mix** -> scrii ce-ti cere subiectul, in ordine, din acelasi bagaj de 9 functii / patternuri.
