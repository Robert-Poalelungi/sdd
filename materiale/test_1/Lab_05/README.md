# Lab 05 — Doubly Linked List

**Ce inveti:** lista **dublu inlantuita** (fiecare nod pointeaza si inapoi, nu doar inainte) + container ca struct (`List{head, tail}`).
**Pre-requisites:** Lab 3 fluent. Lab 4 util dar nu obligatoriu.
**Timp estimat:** 75-100 min prima data.

---

## Schimbarea majora vs Lab 3/4

Lab 5 **nu e aditiv**, e **structural diferit**. Doua schimbari:

1. **Fiecare nod are `prev`** (pointeaza la nodul anterior), nu doar `next`
2. **Lista e un struct** cu `head` si `tail`, nu doar un `Node*`

**Consecinte uriase:**
- `insertEnd` devine **O(1)** (ai direct `tail`, nu mai parcurgi)
- `deleteEnd` devine **O(1)** (ai direct `tail->prev`)
- Functiile iau **`List*`** (un singur pointer), nu `Node**`
- Poti parcurge si **inapoi** (`tail → head`)

---

## Pas 1 — Deseneaza dubla inlantuire (10 min)

```
        head                                          tail
         |                                             |
         v                                             v
NULL <- [A] <-> [B] <-> [C] <-> [D] -> NULL
```

Fiecare nod are **2 sageti**: `prev` (inapoi) si `next` (inainte). Primul nod are `prev = NULL`. Ultimul are `next = NULL`.

**Container:** nu mai e doar `Node* list`, e:
```c
typedef struct { Node* head; Node* tail; } List;
```

---

## Pas 2 — De ce `List*` si nu `Node**` (10 min)

In Lab 3/4 foloseai `Node**` pentru ca functia trebuia sa modifice capul listei (o variabila `Node*` din apelant).

Acum, capul si coada sunt **campuri ale unui struct**. Cand pasezi `List*`, ai deja acces la `list->head` si `list->tail` si le poti modifica direct prin pointer-ul la struct. **Nu mai ai nevoie de pointer-la-pointer.**

```c
void insertBeginning(List* list, Movie* movie) {
    // ...
    list->head = newNode;   // modifici campul direct
}
```

In `main`:
```c
List list = { NULL, NULL };            // init
loadMovies("movies.csv", &list, ...);  // &list, adica List*
```

---

## Pas 3 — `insertBeginning` cu 4 legaturi (15 min)

La lista simpla aveai 2 legaturi (`newNode->next` si `head`). Acum ai **4**:

```c
void insertBeginning(List* list, Movie* movie) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = movie;

    newNode->prev = NULL;                           // 1. e noul cap, nu are prev
    newNode->next = list->head;                     // 2. leaga de fostul cap

    if (list->head)
        list->head->prev = newNode;                 // 3. fostul cap pointeaza inapoi spre newNode
    else
        list->tail = newNode;                       // lista era goala → e si tail

    list->head = newNode;                           // 4. noul cap
}
```

**Cele 4 legaturi de gestionat:**
1. `newNode->prev`
2. `newNode->next`
3. `head->prev` (daca exista head)
4. `head` (si `tail` daca era goala)

**Desenează** starea inainte si dupa insert. Asigura-te ca toate sagetile pointeaza corect.

---

## Pas 4 — `insertEnd` — simetric cu `insertBeginning` (10 min)

**Trucul lab 5:** `insertEnd` e **oglinda** lui `insertBeginning`. Inlocuiesti:
- `head` ↔ `tail`
- `prev` ↔ `next`

```c
void insertEnd(List* list, Movie* movie) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = movie;

    newNode->next = NULL;                           // 1. oglinda: prev→next, NULL
    newNode->prev = list->tail;                     // 2. oglinda: head→tail

    if (list->tail)
        list->tail->next = newNode;                 // 3. oglinda: head->prev→tail->next
    else
        list->head = newNode;                       // oglinda: tail→head

    list->tail = newNode;                           // 4. oglinda: head→tail
}
```

**Diferenta enorma vs Lab 3:** nu mai parcurgi lista! Ai direct `tail`. → **O(1) in loc de O(n)**.

---

## Pas 5 — `deleteBeginning` si `deleteEnd` — si ele simetrice (15 min)

```c
void deleteBeginning(List* list) {
    if (!list->head) return;

    Node* toDelete = list->head;
    list->head = list->head->next;

    if (list->head)
        list->head->prev = NULL;         // noul cap nu mai are prev
    else
        list->tail = NULL;                // lista a ramas goala

    free(toDelete->data->title);
    free(toDelete->data);
    free(toDelete);
}
```

`deleteEnd` = oglinda: `head↔tail`, `prev↔next`. Scrie-o pe hartie singur.

**Diferenta vs Lab 4:** nu mai parcurgi pana la penultim. Ai direct `tail->prev`. → **O(1)**.

---

## Pas 6 — `parseList` + `freeList` (10 min)

Parcurgerea e aproape identica cu Lab 3, dar pe `List*`:
```c
void parseList(List* list) {
    Node* aux = list->head;
    while (aux) { printMovie(aux->data); aux = aux->next; }
}
```

**Bonus:** parcurgere inversa (imposibila in Lab 3/4):
```c
Node* aux = list->tail;
while (aux) { printMovie(aux->data); aux = aux->prev; }
```

`freeList` e la fel ca Lab 3 pe `head`, dar la final setezi **ambele** campuri NULL:
```c
list->head = NULL;
list->tail = NULL;
```

---

## Pas 7 — `loadMovies` + `main` (10 min)

`loadMovies` = scheletul CSV comun. Nimic nou. Foloseste `insertFn` ca Lab 4.

`main`:
```c
List list = { NULL, NULL };
loadMovies("movies.csv", &list, insertEnd);
parseList(&list);
freeList(&list);
```

Observa: **mereu `&list`** (nu `&&list`). Un singur nivel de indirectare. Asta e cheia cand folosesti `List*`.

---

## Pas 8 — Lab 5 integral de la zero (25 min)

Fisier gol. Tot labul. Compileaza, ruleaza. Verifica ca dupa `deleteBeginning + deleteEnd` raman cele 8 filme din mijloc.

---

## Patternuri NOI vs Lab 3/4

```c
// 1. Init lista
List list = { NULL, NULL };

// 2. Insert begin — 4 legaturi
newNode->prev = NULL;
newNode->next = list->head;
if (list->head) list->head->prev = newNode;
else            list->tail = newNode;
list->head = newNode;

// 3. Insert end — simetric (head↔tail, prev↔next)
newNode->next = NULL;
newNode->prev = list->tail;
if (list->tail) list->tail->next = newNode;
else            list->head = newNode;
list->tail = newNode;

// 4. Delete begin
list->head = list->head->next;
if (list->head) list->head->prev = NULL;
else            list->tail = NULL;

// 5. Parcurgere inversa (doar doubly!)
for (Node* a = list->tail; a; a = a->prev) ...
```

---

## Singly vs Doubly — tabel de memorat

| Operatie      | Singly (Lab 3/4) | Doubly (Lab 5) |
|---------------|------------------|----------------|
| Insert begin  | O(1)             | O(1)           |
| Insert end    | O(n) (fara tail) | **O(1)**       |
| Delete begin  | O(1)             | O(1)           |
| Delete end    | O(n)             | **O(1)**       |
| Parcurgere ←  | imposibila       | `aux->prev`    |
| Mem / nod     | 2 pointeri       | 3 pointeri     |
| Pasare        | `Node**`         | `List*`        |

---

## Capcane

| Capcana | Antidot |
|---------|---------|
| Uiti sa setezi `head->prev = NULL` dupa `deleteBeginning` | Noul cap trebuie sa aiba `prev = NULL`, altfel pointeaza la gunoaie. |
| Uiti sa setezi `tail = NULL` cand lista ramane goala | Altfel `tail` pointeaza la memorie eliberata. Crash. |
| Scrii `Node**` in signatura functiilor | Nu. E `List*`. Un singur nivel. |
| Uiti sa updatezi `tail` la `insertBeginning` cand lista era goala | La lista cu 1 element, `head == tail == newNode`. |
| Copiezi „mot-a-mot" insertBeginning pentru insertEnd | Nu. E **simetric**, nu identic. Schimba `head↔tail` si `prev↔next`. |

---

## Self-test (fara sa te uiti la cod)

- [ ] Pot explica de ce `List*` in loc de `Node**`.
- [ ] Pot scrie `insertBeginning` cu toate cele 4 legaturi, in 5 min.
- [ ] Pot scrie `insertEnd` ca oglinda a lui `insertBeginning`, in 3 min.
- [ ] Pot scrie `deleteBeginning` in 4 min, gestionand cazul „a ramas goala".
- [ ] Pot scrie parcurgere inversa (`tail → head`).
- [ ] Pot explica de ce `insertEnd` e O(1) aici si O(n) la Lab 3.

Daca bifezi toate 6 → treci la Lab 6.
