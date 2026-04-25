# Lab 04 — Singly Linked List + Delete + Function Pointers

**Ce inveti:** cum pasezi **functii ca argument** si cum stergi noduri dintr-o lista simplu inlantuita.
**Pre-requisites:** **Lab 3 trebuie stiut fluent.** Nu incepe aici pana nu poti scrie Lab 3 de la zero in 20 min.
**Timp estimat:** 60-90 min prima data.

---

## Ce e NOU vs Lab 3

Doar 2 concepte, dar importante:

1. **Function pointers** — treci o functie ca parametru
2. **3 functii de stergere** — `deleteFromBeginning`, `deleteFromEnd`, `deleteByCondition`

Restul (Node, addToBeginning, addToEnd, parseList, freeList, loadCars) = **identic cu Lab 3**.

---

## Pas 1 — Intelege function pointers (15 min)

Sintaxa „urata":
```c
void (*insertFn)(Node**, F1Car*);
```
= „o variabila numita `insertFn` care pointeaza la o functie ce primeste `Node**` si `F1Car*` si returneaza `void`".

Cu `typedef` devine curata:
```c
typedef void (*InsertListFn)(Node**, F1Car*);
InsertListFn insertFn;   // exact ca `int x`, doar ca e functie
```

**La ce foloseste?** `loadCars` nu mai decide daca inserezi la cap sau la coada:
```c
loadCars("cars.csv", &list, addToEnd);        // la coada
loadCars("cars.csv", &list, addToBeginning);  // la cap
```

**Apelul:** `insertFn(list, car)` — ca orice functie normala.

**Exercitiu:** declara un typedef pentru o functie `int (F1Car*)` care intoarce 1/0 pentru o conditie. (Raspuns: `typedef int (*ConditionFn)(F1Car*);`)

---

## Pas 2 — `deleteFromBeginning` (10 min)

Cea mai simpla stergere. 3 pasi:

```c
// 1. retine capul vechi
Node* toDelete = *list;
// 2. muta capul pe urmatorul
*list = (*list)->next;
// 3. elibereaza vechiul cap
free(toDelete->data->driver); free(toDelete->data->team);
free(toDelete->data); free(toDelete);
```

Scrie-o pe hartie. Apoi compar-o cu `Lab_04.c`.

---

## Pas 3 — `deleteFromEnd` (15 min)

**2 sub-cazuri** (atent!):
- **≥ 2 elemente**: mergi pana la **penultim**, elibereaza ultimul, seteaza `penultim->next = NULL`
- **exact 1 element**: elibereaza-l si seteaza `*list = NULL`

**Pattern nou:** parcurgere pana la penultim
```c
while (aux->next->next) aux = aux->next;   // opreste PE penultim
```

**De ce `->next->next`?** La lab 3 foloseai `while(aux->next)` = „opreste pe ultim". Aici vrei penultimul, deci cu un `->next` mai mult.

**Capcana:** `while (aux->next->next)` crasheaza daca lista are 1 element (`aux->next` e NULL, apoi `NULL->next` = crash). De aia verifici intai `if ((*list)->next)` inainte sa parcurgi.

---

## Pas 4 — `deleteByCondition` (25 min) — CEA MAI GREA

Primeste o functie care decide daca un nod trebuie sters.

**Desenează pe hartie:**

```
[A] -> [B] -> [C] -> [D] -> NULL
prev   aux
```

Ai 2 pointeri:
- `aux` = nodul curent (cel pe care-l verificam)
- `prev` = nodul anterior (sau `NULL` daca suntem pe cap)

**3 ramuri:**
1. `cond(aux->data)` e **false** → nu sterge, `prev = aux; aux = aux->next`
2. `cond` e **true** si `prev != NULL` → mijloc/coada: `prev->next = aux->next`, free(aux), `aux = prev->next`
3. `cond` e **true** si `prev == NULL` → cap: `*list = aux->next`, free(aux), `aux = *list`

**ATENTIE subtila:** dupa ce **stergi** un nod, `prev` **ramane la fel** (nu-l avansezi). De ce? Pentru ca `aux` a disparut si acum `prev->next` pointeaza la urmatorul, care e noul `aux`.

**Mini-test:** pe lista `[A, B, C]`, daca toate 3 indeplinesc conditia:
- Start: `prev=NULL, aux=A` → șterge A, `aux=B` (noul cap), `prev` ramane NULL
- Iteratia 2: `prev=NULL, aux=B` → șterge B, `aux=C`, `prev` ramane NULL
- Iteratia 3: `prev=NULL, aux=C` → șterge C, `aux=NULL`
- Exit loop.

---

## Pas 5 — Scrie `loadCars` cu `InsertListFn` (5 min)

E `loadCars` din Lab 3 cu **o singura** diferenta:
```c
int loadCars(const char* filename, Node** list, InsertListFn insertList) {
    // ...
    insertList(list, car);   // in loc de addToEnd(list, car)
    // ...
}
```

---

## Pas 6 — Functii de conditie + `main` (10 min)

In `main.c`, definesti functii care primesc `F1Car*` si returneaza `int`:

```c
int isRedBull(F1Car* car) {
    return strcmp(car->team, "Red Bull") == 0;
}

int isUnder1015HP(F1Car* car) {
    return car->engine_hp < 1015.0f;
}
```

Le pasezi la `deleteByCondition`:
```c
deleteByCondition(&list, isRedBull);
deleteByCondition(&list, isUnder1015HP);
```

**IMPORTANT:** trebuie `#include <string.h>` pentru `strcmp` (lipsea in codul original).

---

## Pas 7 — Lab 4 integral de la zero (20 min)

Fisier gol. Tot labul, fara sa te uiti. Max 3 peek-uri (cel mai probabil la `deleteByCondition`). Compileaza, ruleaza, verifica output.

**Rezultatul asteptat:** dupa `deleteByCondition(isUnder1015HP)` raman doar 6 masini (cele cu exact 1015 HP).

---

## Patternuri NOI vs Lab 3

```c
// 1. Typedef function pointer
typedef void (*InsertListFn)(Node**, F1Car*);
typedef int (*ConditionFn)(F1Car*);

// 2. Apel function pointer (ca orice functie)
insertFn(list, car);
conditionFn(aux->data);

// 3. Parcurgere cu prev + aux (necesar la stergere)
Node* aux = *list;
Node* prev = NULL;
while (aux) {
    if (sterge) {
        if (prev) prev->next = aux->next; else *list = aux->next;
        // free(aux), dar salveaza aux->next intai!
    } else {
        prev = aux;
        aux = aux->next;
    }
}

// 4. Parcurgere pana la penultim
while (aux->next->next) aux = aux->next;
```

---

## Capcane

| Capcana | Antidot |
|---------|---------|
| Dupa stergere in mijloc, avansezi `prev` | Nu. `prev` ramane, doar `aux` trece mai departe. |
| `while (aux->next->next)` pe lista cu 1 elem | Verifica mereu `if ((*list)->next)` inainte. |
| Uiti `#include <string.h>` la `strcmp` | Compilatorul te lasa (warning, nu error) dar poate da comportament ciudat. |
| Conditia returneaza alt decat 0/1 | OK, orice non-zero e „true" in C. Dar scrie explicit `== 0`, `!= 0`, `<`, `>`. |
| Crezi ca `deleteByCondition(isFalse)` face ceva | Daca `cond` nu e true pentru niciun nod, nu se intampla nimic. Nu e bug. |

---

## Self-test (fara sa te uiti la cod)

- [ ] Pot declara si folosi un `typedef` de function pointer.
- [ ] Pot scrie `deleteFromBeginning` in 3 min.
- [ ] Pot scrie `deleteFromEnd` in 5 min, cu cele 2 sub-cazuri.
- [ ] Pot scrie `deleteByCondition` in 10 min, cu cele 3 ramuri (cond false, cond true+prev, cond true+cap).
- [ ] Pot explica de ce `prev` NU avanseaza dupa stergere.
- [ ] Pot pasa `addToEnd` ca argument la `loadCars`.

Daca bifezi toate 6 → treci la Lab 5.
