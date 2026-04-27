# Lab 05 — versiune MINIMALA (cat era in seminar)

**Ce e:** EXACT ce era implementat in fisierele `.c` din `Lab_05/` original. Nimic in plus, nimic ce nu a fost predat.

**Pre-requisites:** Lab 3 fluent.
**Timp estimat:** 45-60 min.

---

## Ce contine

Doar 4 functii (in afara de `main`):

1. **`printMovie`** (era in `Movie.c`)
2. **`insertBeginning`** (era in `DoublyLinkedList.c`)
3. **`insertEnd`** (era in `DoublyLinkedList.c`)
4. **`loadMovies`** (era in `Movie.c`)

Plus:
- Struct `Movie` (era in `Movie.h`)
- Struct `Node` cu `prev` + `next` (era in `DoublyLinkedList.h`)
- Struct `List` cu `head` + `tail` (era in `DoublyLinkedList.h`)
- `typedef InsertListFn` (era in `DoublyLinkedList.h`)
- `main` minimal — doar incarca si afiseaza count

## Ce NU contine (intentionat)

Astea erau in `Lab_05/README.md` cu cod complet, dar **NU** in `.c`-urile predate la seminar:

- ❌ `parseList`
- ❌ `deleteBeginning`
- ❌ `deleteEnd`
- ❌ `freeList`

---

## Pasi de invatare (cele 3 lucruri esentiale)

### Pas 1 — Schimbarea structurala vs Lab 3 (15 min)

Lab 5 NU adauga functii peste Lab 3. **Schimba structura.**

| | Lab 3 | Lab 5 |
|---|-------|-------|
| Nod | `data + next` | `data + prev + next` |
| Container | `Node*` | `List { head, tail }` |
| Pasare | `Node** list` | `List* list` |

**Cheia:** la Lab 5 ai `head` SI `tail` direct, deci `insertEnd` e **O(1)** (nu mai parcurgi).

---

### Pas 2 — `insertBeginning` cu cele 4 legaturi (15 min)

```c
void insertBeginning(List* list, Movie* movie) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = movie;
    newNode->prev = NULL;             // 1. e noul cap, nu are prev
    newNode->next = list->head;       // 2. leaga de fostul cap

    if (list->head)
        list->head->prev = newNode;   // 3. fostul cap pointeaza inapoi
    else
        list->tail = newNode;         // 4. lista era goala -> e si tail

    list->head = newNode;             // 5. noul cap
}
```

Cele **4 legaturi** de gestionat:
1. `newNode->prev`
2. `newNode->next`
3. `head->prev` (sau `tail` daca era goala)
4. `head` actualizat

---

### Pas 3 — `insertEnd` ca **oglinda** (10 min)

`insertEnd` e oglinda lui `insertBeginning`. Schimbi:
- `head` ↔ `tail`
- `prev` ↔ `next`

```c
void insertEnd(List* list, Movie* movie) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = movie;
    newNode->next = NULL;             // 1. oglinda: prev->next, NULL
    newNode->prev = list->tail;       // 2. oglinda: head->tail

    if (list->tail)
        list->tail->next = newNode;   // 3. oglinda: head->prev -> tail->next
    else
        list->head = newNode;         // 4. oglinda: tail -> head

    list->tail = newNode;             // 5. oglinda: head -> tail
}
```

**Diferenta enorma vs Lab 3:** nu mai parcurgi! `insertEnd` aici e **O(1)**, la Lab 3 era **O(n)**.

---

### Pas 4 — `loadMovies` (5 min)

Schelet CSV identic cu lab 3/4. Singura diferenta: container e `List*`, nu `Node**`. Insertia se face cu `insertFn(list, movie)` (function pointer ca la Lab 4).

---

### Pas 5 — Drill (10 min)

Foaie alba. Scrie tot. Compileaza. Verifica „Loaded 10 movies."

---

## Patternuri de memorat

```c
// 1. Init lista
List list = { NULL, NULL };

// 2. Insert begin (4 legaturi)
newNode->prev = NULL;
newNode->next = list->head;
if (list->head) list->head->prev = newNode;
else            list->tail = newNode;
list->head = newNode;

// 3. Insert end (oglinda begin)
newNode->next = NULL;
newNode->prev = list->tail;
if (list->tail) list->tail->next = newNode;
else            list->head = newNode;
list->tail = newNode;
```

---

## Capcane

| Capcana | Antidot |
|---------|---------|
| Uiti sa setezi `tail` cand lista era goala | La `insertBeginning`: `else list->tail = newNode;` |
| Uiti `head->prev = newNode` la insertBeginning | Lista nu e bidirectionala daca lipseste asta |
| Scrii `Node**` in semnaturi | Nu. E `List*`. Un singur nivel. |
| Copiezi mot-a-mot insertBeginning pentru insertEnd | Nu. E **simetric**, nu identic. |

---

## Self-test

- [ ] Pot scrie `insertBeginning` cu cele 4 legaturi, in 5 min.
- [ ] Pot scrie `insertEnd` ca oglinda a lui `insertBeginning`, in 3 min.
- [ ] Pot explica de ce `insertEnd` e O(1) aici, dar O(n) la Lab 3.
- [ ] Pot scrie `loadMovies` cu `InsertListFn`, in 5 min.

Daca bifezi 4/4 -> stapanesti Lab 5 minimal.

---

## Daca la examen pica si stergere/parcurgere

Cele 4 functii lipsa (`parseList`, `delete*`, `freeList`) sunt **simple** daca stii Lab 3 + structura noua a Lab 5:

- `parseList` = identic cu Lab 3, dar incepi cu `list->head` in loc de `*list`
- `deleteBeginning` = `list->head = list->head->next; list->head->prev = NULL;`
- `deleteEnd` = simetric: `list->tail = list->tail->prev; list->tail->next = NULL;`
- `freeList` = identic cu Lab 3 pe `list->head`, plus `list->tail = NULL` la final

**Risc real:** profesorul testeaza doar ce era in seminar = cele 4 functii din acest fisier.
**Risc minim:** profesorul cere si ce era in README. In acest caz, `Lab_05/Lab_05.c` (versiunea completa) le contine.
