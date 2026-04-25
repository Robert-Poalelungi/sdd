# Lab 03 — Singly Linked List

**Ce inveti:** lista simplu inlantuita (fiecare nod pointeaza DOAR la urmatorul).
**Pre-requisites:** pointeri, `malloc`/`free`, struct-uri in C.
**Timp estimat:** 90-120 min prima data. A 2-a oara: 20-30 min.

---

## De ce incepi aici

Lab 3 e fundatia pentru Lab 4, 5, 6. **Daca il stii fluent, urmatoarele 3 se invata in ~1h fiecare.** Nu trece mai departe pana nu il scrii fara sa te uiti.

---

## Pas 1 — Deseneaza lista pe hartie (10 min)

Inainte de cod, intelege vizual. Deseneaza:

```
head -> [data|next] -> [data|next] -> [data|next] -> NULL
```

Intreaba-te:
- Unde e „capul" listei? (`head`, adica `*list`)
- Ce e in ultimul nod? (`next = NULL`)
- Cum adaugi la inceput? (pune nodul nou inaintea `head`)
- Cum adaugi la sfarsit? (parcurgi pana gasesti nodul cu `next == NULL`)

**Blocaj clasic:** nu intelegi de ce `Node**`. Raspuns: daca functia schimba **capul listei**, ai nevoie sa modifici **variabila apelantului** — adica un pointer la pointer-ul de cap.

---

## Pas 2 — Citeste `Lab_03.c` o singura data (15 min)

Deschide fisierul. Citeste-l de sus in jos. NU memora. Doar intelege:
- Sectiunea 1: `F1Car` + `printCar`
- Sectiunea 2: `Node` + 4 functii de lista
- Sectiunea 3: `loadCars` (CSV → F1Car → lista)
- Sectiunea 4: `main`

**Checkpoint:** poti explica oral ce face fiecare functie?

---

## Pas 3 — Scrie `addToBeginning` pe hartie (10 min)

Cea mai simpla functie. Fara cod deschis:

```c
void addToBeginning(Node** list, F1Car* car) {
    // 1. alocă nod nou
    // 2. pune data
    // 3. nodul nou pointeaza spre FOSTUL cap
    // 4. noul cap = nodul nou
}
```

Completeaz-o. Apoi verifica cu codul. Daca ai gresit, **sterge si refa** — nu te multumi cu „am inteles".

---

## Pas 4 — Scrie `addToEnd` (15 min)

Aici apar 2 cazuri: lista goala / lista are elemente.

**Pattern nou:** parcurgere pana la ultim
```c
Node* aux = *list;
while (aux->next) aux = aux->next;   // opreste PE ultim
aux->next = newNode;
```

**De ce `aux->next` si nu `aux`?** Pentru ca vrei sa te oprești **pe** ultimul, nu **dupa** el. Daca mergi pana cand `aux == NULL`, pierzi legatura.

---

## Pas 5 — Scrie `parseList` + `freeList` (15 min)

`parseList` e pattern de baza de parcurgere:
```c
while (list) { printCar(list->data); list = list->next; }
```

`freeList` e **cea mai importanta functie** de stapanit. Regula: **interior → exterior**.

```c
free(current->data->driver);    // 1. string din F1Car
free(current->data->team);      // 2. celalalt string
free(current->data);            // 3. F1Car
free(current);                  // 4. nodul
```

**Capcana mortala:** daca faci `free(current)` inainte sa salvezi `current->next`, pierzi tot restul listei. De aia:
```c
Node* next = current->next;   // SALVEAZA intai
free(current);
current = next;
```

---

## Pas 6 — `loadCars` de la zero (20 min)

Deschide un fisier gol. Scrie de la 0 `loadCars`. Max 2 peek-uri.

**Patternul `strtok`:**
- prima data cu `line`: `strtok(line, ",")`
- apoi mereu cu `NULL`: `strtok(NULL, ",")`

**Pattern per camp:**
- **numar** → `atoi(token)` sau `atof(token)`
- **string** → `malloc(strlen(token)+1)` + `strcpy(dest, token)`

---

## Pas 7 — Scrie TOT Lab 3 de la zero (30 min)

Fisier gol. Tot labul. Max 3 peek-uri. Compileaza. Ruleaza. Verifica output-ul.

Daca ai terminat sub 30 min si ruleaza corect → treci la Lab 4.
Daca dureaza mai mult sau ai gresit mult → refa a doua zi.

---

## Patternuri de memorat DIN LAB 3

```c
// 1. Nod nou
Node* n = malloc(sizeof(Node));
n->data = x;
n->next = NULL;           // sau *list pentru addToBeginning

// 2. Parcurgere pana la ultim
while (aux->next) aux = aux->next;

// 3. Parcurgere toata lista
while (aux) { ...; aux = aux->next; }

// 4. Free nod complet (interior → exterior)
free(n->data->str1); free(n->data); free(n);

// 5. Salvare next inainte de free
Node* next = current->next; free(current); current = next;

// 6. Regula `&`
&list   // cand modifici capul (add, free)
 list   // cand doar citesti (parse)
```

---

## Capcane

| Capcana | Antidot |
|---------|---------|
| Uiti `&` in `main` la `loadCars(&list)` | Regula: modific capul → `&`. Doar citesc → fara `&`. |
| `while (aux)` in loc de `while (aux->next)` la addToEnd | Vrei sa te oprești PE ultim, nu dupa el. |
| Free in ordine gresita | Mereu interior → exterior. Mai intai string-urile, la urma nodul. |
| Uiti `+1` la `malloc(strlen(token)+1)` | `strcpy` copiaza si terminatorul `\0` — trebuie spatiu pentru el. |
| Nu verifici `f == NULL` dupa `fopen` | Crash la primul `fgets`. |
| `atof` pe un token cu `\n` | Nu e problema — atoi/atof ignora trailing whitespace. |

---

## Self-test (fara sa te uiti la cod)

- [ ] Pot scrie `addToBeginning` in 2 min.
- [ ] Pot scrie `addToEnd` in 3 min, cu cele 2 cazuri (lista goala / nu).
- [ ] Pot scrie `freeList` in 3 min, cu ordinea corecta.
- [ ] Pot scrie `loadCars` cu patternul strtok, in 8 min.
- [ ] Pot explica de ce `Node**` in unele functii si `Node*` in altele.
- [ ] Pot explica ce se intampla daca uit `Node* next = current->next;` in `freeList`.

Daca bifezi toate 6 → treci la Lab 4.
