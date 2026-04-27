# Patternuri compozite pentru testul 2 (AVL)

**Scopul:** patternuri-șablon pe care le aplici la cerințele testului. **NU sunt soluții complete** — completezi tu detaliile cu structul și condițiile tale.

---

## Pattern A — Inventarea unui struct pe loc

Cerința tipică: „Definiți o structură X care conține Y câmpuri."

### Decision tree pentru fiecare câmp

| Tip de date | Cum declari | Cum citești din CSV | Ce free-uiești |
|-------------|-------------|---------------------|----------------|
| Număr întreg | `int` / `unsigned int` | `atoi(token)` | nimic |
| Număr real | `float` | `(float)atof(token)` | nimic |
| String | `char*` | `malloc(strlen(token)+1)` + `strcpy` | `free(camp)` |
| Caracter unic | `char` | `token[0]` | nimic |
| Sub-struct | `struct X subCamp` | parsare recursivă | după caz |

### Reguli pentru AVL

- **Cheia de balansare** = câmpul după care comparăm noduri (ex. `playerID`, `id`, etc.)
- Trebuie să fie un câmp **comparabil cu `<` `>`** — int, float, sau o conversie
- Restul câmpurilor sunt „date" (folosite doar la afișare/filtrare)

### Schelet de struct + cheie de comparație

```c
typedef struct {
    /* CHEIA AVL — ALEGI ACUM care e */
    <tip> cheia;

    /* RESTUL CAMPURILOR */
    char* string1;        // necesita malloc/free
    int   numar;
    float real;
    /* ... */
} TipulMeu;
```

### Întrebări să-ți pui în 30 secunde
- Care câmp e cheia AVL? (Test cere asta explicit)
- Câte string-uri am? (atatea malloc-uri și free-uri)
- Trebuie să modific `insertTree` să compare după altceva? (Nu, doar redenumesti `fp.id` cu `cheia ta`)

---

## Pattern B — Count by criterion pe AVL (varianta SIMPLĂ, fără function pointer)

Cerința de tip „numărul de X care îndeplinesc condiția Y".

### Schelet

```c
int countXX(TreeNode* tree, /* parametri pentru conditie, ex. const char* target */) {
    if (!tree) return 0;

    int count = 0;
    if (/* CONDITIA TA — ex. strcmp(tree->data.position, target) == 0 */) {
        count = 1;
    }

    count += countXX(tree->left,  /* parametri */);
    count += countXX(tree->right, /* parametri */);

    return count;
}
```

### Note
- **NU function pointer** dacă cerința e specifică ("jucători pe poziția X"). Pasezi direct parametrul ca argument simplu.
- Recursivitate: parcurgi **toate** nodurile (poți folosi pre/in/post-order — nu contează pentru numărare).
- **Condiția** o scrii literal cu `if`, nu cu `cond(tree->data)`.

### Exemple de condiții
```c
if (strcmp(tree->data.string_field, target_string) == 0)   // string egal
if (tree->data.numeric_field < threshold)                   // sub un prag
if (tree->data.numeric_field > min && tree->data.numeric_field < max)  // în interval
```

---

## Pattern C — Delete by criterion cu rebalansare după fiecare (varianta corectă)

Cerința tipică: „Ștergeți toate elementele care îndeplinesc condiția Z. **Validați echilibrul după fiecare ștergere.**"

### De ce NU varianta din README-ul Lab_09 (collect IDs then delete)

Acea variantă funcționează dar:
- Are 2 traversări (colectare + delete)
- Nu reflectă cerința „validați echilibrul după **fiecare** ștergere"

### Varianta cerută — un singur loop

**Idee:** mergi de la rădăcină, găsești PRIMUL element care match-uiește, îl ștergi (cu rebalansarea automată din `deleteTreeNode`), apoi RE-ÎNCEPI de la rădăcină până nu mai găsești.

```c
void deleteAllMatching(TreeNode** root, /* parametri pentru conditie */) {
    int found = 1;

    while (found) {
        found = 0;

        /* Cauta primul element care match-uieste */
        <tipCheie> idToDelete = findFirstMatch(*root, /* parametri */);

        if (/* idToDelete e valid */) {
            deleteTreeNode(root, idToDelete);   // face rebalansarea automat
            found = 1;
        }
    }
}
```

### `findFirstMatch` — helper

```c
<tipCheie> findFirstMatch(TreeNode* tree, /* parametri */) {
    if (!tree) return /* sentinel value, ex. 0 sau -1 */;

    if (/* CONDITIA TA */) return tree->data.cheie;

    <tipCheie> leftResult = findFirstMatch(tree->left, /* parametri */);
    if (/* leftResult valid */) return leftResult;

    return findFirstMatch(tree->right, /* parametri */);
}
```

### Variantă alternativă — recursivă cu colectare la apel curent

Dacă vrei o singură funcție recursivă (mai elegant):

```c
void deleteAllMatching(TreeNode** root, /* parametri */) {
    if (!*root) return;

    /* RECURSIVITATE PE COPII PRIMA (post-order) */
    deleteAllMatching(&(*root)->left, /* parametri */);
    deleteAllMatching(&(*root)->right, /* parametri */);

    /* DUPA, daca radacina match-uieste, sterg */
    if (*root && /* CONDITIA TA */) {
        deleteTreeNode(root, (*root)->data.cheie);
    }
}
```

**Atenție:** ambele variante presupun că `deleteTreeNode` deja face rebalansarea (în Lab_09 face).

### Choice point
- Loop simplu (varianta 1) — mai ușor de scris, mai puține bug-uri
- Recursiv post-order (varianta 2) — mai elegant, mai puține traversări dar mai subtil

**Recomandare:** loop simplu la examen. E mai sigur.

---

## Pattern D — AVL → LSI cu filtru (PATTERNUL NOU, nu e în alte fișiere)

Cerința tipică: „Construiți o listă simplu înlănțuită cu toate elementele care îndeplinesc condiția W. Afișați lista."

### Idea
1. Pornești cu o listă goală (`Node* list = NULL`)
2. Parcurgi AVL-ul recursiv
3. La fiecare nod care match-uiește → `addToEnd(&list, copy)` (din Lab 3)
4. Afișezi lista cu `parseList(list)`
5. La final, eliberezi lista cu `freeList(&list)`

### Decizie importantă: shallow copy vs deep copy

Când inserezi în LSI, NU paseza pointer la nodul AVL — riști double-free când eliberezi ambele structuri. **Fă o copie:**

```c
TipulMeu* copy = malloc(sizeof(TipulMeu));
copy->cheia    = source.cheia;
copy->numar    = source.numar;
copy->string1  = malloc(strlen(source.string1) + 1);
strcpy(copy->string1, source.string1);
/* ... pentru fiecare camp ... */
```

### Schelet pattern

```c
/* Functia care construieste lista filtrata */
void buildFilteredList(TreeNode* tree, ListNode** list, /* parametri pentru conditie */) {
    if (!tree) return;

    /* Recursivitate stanga */
    buildFilteredList(tree->left, list, /* parametri */);

    /* Verifica nodul curent */
    if (/* CONDITIA TA */) {
        TipulMeu* copy = malloc(sizeof(TipulMeu));
        /* COPIAZA fiecare camp din tree->data in *copy */
        addToEnd(list, copy);    // functia din Lab 3
    }

    /* Recursivitate dreapta */
    buildFilteredList(tree->right, list, /* parametri */);
}
```

### Apelul în main

```c
ListNode* filteredList = NULL;
buildFilteredList(avlTree, &filteredList, /* argumente */);

parseList(filteredList);          // Lab 3
freeList(&filteredList);          // Lab 3
```

### Ce-ți trebuie din alte laburi pentru asta

- **Din Lab 3:** struct `Node` (cu `data + next`), `addToEnd`, `parseList`, `freeList`
- **Din Lab 9:** struct `TreeNode`, parcurgerea recursivă

**Esti obligat să le pui pe AMBELE în același fișier la examen.** Asta înseamnă două structuri `Node` cu același nume — **redenumește una** (ex. `TreeNode` și `ListNode`).

---

## Workflow recomandat la examen

1. **Citește subiectul de 2 ori.** Identifică:
   - Care e structul cerut
   - Care e cheia AVL
   - Care sunt cele 3-4 funcții cerute
   - Care e save target (vector / LSI / lista dubla)

2. **Definește structul.** ~2 min.

3. **Copy-paste mental Lab_09:** scrii `treeHeight`, `balanceFactor`, `rotateLeft`, `rotateRight`, `insertTree`, `deleteTreeNode`. ~15 min.

4. **Citire din fișier.** ~5 min. Pattern strtok cu `\n` removal.

5. **Funcțiile specifice testului.** ~30 min:
   - Count cu condiție
   - Delete cu criteriu (varianta cu loop)
   - Build LSI cu filtru (dacă e save target)

6. **Adaugă structurile auxiliare:** dacă save target e LSI, ai nevoie de `Node` (LSI) + `addToEnd` + `parseList` + `freeList`. **Redenumește pentru a evita conflict cu `TreeNode`.**

7. **`main`** care apelează totul. ~5 min.

8. **Compilează des.** După fiecare 2-3 funcții.

---

## Capcane specifice testului 2

| Capcană | Antidot |
|---------|---------|
| 2 struct-uri cu nume `Node` în același fișier | Redenumește unul: `TreeNode` și `ListNode`. |
| Inserezi pointerul direct din AVL în LSI | Double-free la freeList + freeTree. **Fă deep copy.** |
| Uiți să rebalansezi după ștergere | `deleteTreeNode` din Lab_09 deja face. **Nu rescrie de la 0** — copiezi codul. |
| Uiți `\n` removal în CSV | Defensive habit, mereu pune-l. |
| Folosești `feof` la citire | Anti-pattern — folosește `while (fgets(...) != NULL)`. |
| Compari string-uri cu `==` în loc de `strcmp` | `strcmp(a, b) == 0` pentru egal. |
| Pasezi `TreeNode*` în loc de `TreeNode**` la modificare | Modific copilul/rădăcina → `**`. Doar citesc → `*`. |

---

## Self-test înainte de examen

- [ ] Pot defini un struct cu 4 câmpuri (mix int + char* + float) în 2 min
- [ ] Pot scrie `insertTree` cu cele 4 cazuri AVL în 10 min
- [ ] Pot scrie `deleteTreeNode` din memorie (cu rebalansare) — sau pot copia din Lab_09 fără confuzii
- [ ] Pot scrie `countByCondition` cu criteriu specific (nu function pointer) în 5 min
- [ ] Pot scrie `deleteAllMatching` cu loop în 8 min
- [ ] Pot scrie `buildFilteredList` (AVL → LSI) în 10 min
- [ ] Pot să gestionez 2 struct-uri `Node` diferite în același fișier
- [ ] Pot face deep copy între structuri (cu malloc + strcpy)

Dacă bifezi 7/8 → ești gata. Restul e atenție la detalii și compilare des.
