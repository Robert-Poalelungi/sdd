# Lab 09 — AVL Tree (BST auto-balansat)

**Sursa codului:** `S11_12052025/main.c` din github.com/tcervinski-csie/DS2025
**Pre-requisites:** **Lab 08 (BST) FLUENT.** Daca nu stii BST, intoarce-te.
**Timp estimat prima data:** 3-4h (e cea mai grea structura din semestru).

---

## De ce AVL

BST normal poate degenera in **lista** daca inserezi sortat (1, 2, 3, 4...). Cautare devine O(n).
AVL se **rebalanseaza automat** la fiecare inserare/stergere. Toate operatiile raman O(log n).

**Cum?** Dupa fiecare modificare, verifici **balance factor** la fiecare nod pe drum inapoi spre radacina. Daca depaseste +/-1, aplici **rotatii**.

---

## Conceptele NOI fata de BST (3 lucruri)

### 1. Inaltimea unui arbore
Cea mai lunga cale de la nod la o frunza.
```c
int treeHeight(TreeNode* tree) {
    if (!tree) return 0;
    int hL = treeHeight(tree->left);
    int hR = treeHeight(tree->right);
    return 1 + (hL > hR ? hL : hR);
}
```
- Frunza: inaltime 1
- NULL: inaltime 0

### 2. Balance factor
```c
int balanceFactor(TreeNode* tree) {
    return treeHeight(tree->right) - treeHeight(tree->left);
}
```
- **0** = balansat perfect
- **-1 sau +1** = OK, e tolerat
- **+2** = dreapta prea inalta -> rotatie stanga
- **-2** = stanga prea inalta -> rotatie dreapta

### 3. Rotatii — singura magie
Sunt 2 rotatii primitive:

**`rotateRight(root)`** — folosita cand stanga e prea inalta:
```
        A                B
       / \              / \
      B   c     ->     d   A
     / \                  / \
    d   e                e   c
```
Codul (memorizati pe de rost!):
```c
void rotateRight(TreeNode** root) {
    TreeNode* aux = (*root)->left;
    (*root)->left = aux->right;
    aux->right = *root;
    *root = aux;
}
```

**`rotateLeft(root)`** — simetrica, cand dreapta e prea inalta:
```c
void rotateLeft(TreeNode** root) {
    TreeNode* aux = (*root)->right;
    (*root)->right = aux->left;
    aux->left = *root;
    *root = aux;
}
```

---

## Cele 4 cazuri de dezechilibru

### LL — left-left (insertia s-a facut in stanga lui copil-stang)
```
       3
      /
     2          ->   rotateRight(3)
    /
   1
```
**Fix:** o singura `rotateRight` pe radacina.

### RR — right-right (insertia in dreapta lui copil-drept)
```
   1
    \
     2          ->   rotateLeft(1)
      \
       3
```
**Fix:** o singura `rotateLeft` pe radacina.

### LR — left-right (insertia in dreapta lui copil-stang)
```
       3
      /
     1          ->   rotateLeft(1) apoi rotateRight(3)
      \
       2
```
**Fix:** dubla rotatie. Intai `rotateLeft` pe copilul stang, apoi `rotateRight` pe radacina.

### RL — right-left (insertia in stanga lui copil-drept)
```
   1
    \
     3          ->   rotateRight(3) apoi rotateLeft(1)
    /
   2
```
**Fix:** dubla rotatie. Intai `rotateRight` pe copilul drept, apoi `rotateLeft` pe radacina.

---

## Cum decizi care caz

In `insertTree`, dupa ce inserezi recursiv, verifici `bf = balanceFactor(*root)`:

| `bf` | `bf` al copilului | Caz | Rotatii |
|------|------------------|-----|---------|
| +2 | dreapta = +1 | RR | `rotateLeft(root)` |
| +2 | dreapta = -1 | RL | `rotateRight(child); rotateLeft(root);` |
| -2 | stanga = -1 | LL | `rotateRight(root)` |
| -2 | stanga = +1 | LR | `rotateLeft(child); rotateRight(root);` |

**Mnemonic:** semnul `bf` iti spune **partea opusa** (unde "atarna" arborele). Semnul copilului iti spune daca e simplu sau dublu.

---

## Pasi de invatare

### Pas 1 — Repeta BST-ul (15 min)
Daca nu poti scrie `insertTreeNode` BST din memorie in 4 min, intoarce-te la Lab 08.

### Pas 2 — Inteleg `treeHeight` si `balanceFactor` (15 min)
- Scrie ambele functii pe hartie. Calculeaza pe un arbore mic.
- Test: pentru arborele `3 -> 2 -> 1` (degenerat in lista), care e bf-ul lui 3? (Raspuns: -2)

### Pas 3 — Memoreaza cele 2 rotatii (30 min)
**Cea mai importanta sectiune.** Pe hartie, deseneaza arborele inainte si dupa fiecare rotatie. Repeta de 3-4 ori pana intelegi ce se intampla cu fiecare pointer.

Cele 4 linii din rotateRight, scrise pe ordine:
```
1. aux = root->left
2. root->left = aux->right
3. aux->right = root
4. root = aux
```

Cifrele pointerilor:
- root pointeaza la A (vechea radacina)
- aux pointeaza la B (noua radacina)
- B->right (era e) trece ca root->left
- B->right devine A
- root devine aux (B)

### Pas 4 — Cele 4 cazuri (45 min)
Pe hartie, simuleaza inserarea in: `1,2,3` (RR), `3,2,1` (LL), `1,3,2` (RL), `3,1,2` (LR). Vezi cum dupa al 3-lea element apare dezechilibrul si cum rotatia il fixeaza.

### Pas 5 — `insertTree` cu rebalansare (30 min)
Codul are 3 etape:
1. Insert BST normal (recursiv)
2. Calculeaza `bf` la radacina
3. Daca `bf` e ±2, alege caz si aplica rotatii

### Pas 6 — `deleteTreeNode` (60 min) — CEA MAI GREA FUNCTIE
3 cazuri pentru stergere:
- 0 copii (frunza) -> sterge si returneaza
- 1 copil -> inlocuieste cu copilul
- 2 copii -> gaseste **in-order successor** (cel mai mic din subarborele drept), copiaza datele, apoi sterge succesorul

Apoi rebalanseaza la fel ca la insert.

### Pas 7 — Drill (30 min)
Foaie alba. Scrie `treeHeight`, `balanceFactor`, `rotateRight`, `rotateLeft` din memorie. Apoi `insertTree` cu cele 4 cazuri.

---

## Functii pentru testul 2 (Week 13 Requirements)

### 1. Numarare cu conditie (parcurgere in/pre/post-order)
```c
int countByCondition(TreeNode* tree, int (*cond)(FP)) {
    if (!tree) return 0;
    int count = cond(tree->fp) ? 1 : 0;
    count += countByCondition(tree->left, cond);
    count += countByCondition(tree->right, cond);
    return count;
}
```

### 2. Stergere cu criteriu (cu rotatii)
**Atentie:** trebuie sa colectezi mai intai toate id-urile care indeplinesc conditia (nu poti modifica arborele in timp ce-l parcurgi). Apoi le stergi una cate una cu `deleteTreeNode`.

```c
// Faza 1: colecteaza id-urile in vector
void collectIds(TreeNode* tree, int (*cond)(FP), int* ids, int* count) {
    if (!tree) return;
    if (cond(tree->fp)) {
        ids[*count] = tree->fp.id;
        (*count)++;
    }
    collectIds(tree->left, cond, ids, count);
    collectIds(tree->right, cond, ids, count);
}

// Faza 2: sterge cu deleteTreeNode (face rebalansarea automat)
void deleteByCondition(TreeNode** tree, int (*cond)(FP)) {
    int ids[1000];
    int count = 0;
    collectIds(*tree, cond, ids, &count);

    for (int i = 0; i < count; i++) {
        deleteTreeNode(tree, ids[i]);
    }
}
```

### 3. Copiere in alta structura (in-order -> vector sortat)
```c
void copyToVector(TreeNode* tree, int (*cond)(FP), FP* vec, int* size) {
    if (!tree) return;
    copyToVector(tree->left, cond, vec, size);    // STANGA — pentru ordine sortata
    if (cond(tree->fp)) {
        vec[*size] = tree->fp;
        (*size)++;
    }
    copyToVector(tree->right, cond, vec, size);
}
```

In-order parcurge in **ordinea sortata** -> vectorul rezultat e sortat.

---

## Capcane

| Capcana | Antidot |
|---------|---------|
| Inversezi rotateLeft cu rotateRight | Trick: rotateLeft "muta" radacina in stanga -> partea **dreapta** trebuie sa urce. |
| Confuzii la cazurile dublu (LR, RL) | Mereu: prima rotatie pe **copil** (in directia opusa), a doua pe **radacina**. |
| Uiti `if (*root)` la inceputul rebalansarii dupa delete | La stergere, *root poate deveni NULL — testeaza. |
| Ignori sa rebalansezi pe drumul inapoi | In codul de la seminar, balansarea se face la **fiecare nod** pe drumul de revenire din recursivitate. Asta inseamna ca s-ar putea sa rotesti de mai multe ori. |
| Crezi ca trebuie sa balansezi manual de mai multe ori | NU. O singura rotatie (sau dubla) per nivel, conform celor 4 cazuri. |
| `feof` anti-pattern | Idem ca BST/Heap. |

---

## Self-test

- [ ] Pot desena un arbore din inserarea sortata `1,2,3,4,5` cu rotatii AVL.
- [ ] Pot scrie `treeHeight` si `balanceFactor` in 3 min.
- [ ] Pot scrie `rotateRight` din memorie (4 linii) in 1 min.
- [ ] Pot scrie `rotateLeft` ca oglinda a `rotateRight` in 1 min.
- [ ] Pot enumera cele 4 cazuri (LL, LR, RR, RL) si rotatiile lor.
- [ ] Pot scrie `insertTree` cu rebalansare in 10 min.
- [ ] Pot scrie cele 3 functii pentru testul 2 (count, delete, copy) in 20 min.
- [ ] Pot explica de ce in-order pe BST/AVL da elemente sortate.

Daca bifezi 8/8 -> stapanesti AVL la nivel de examen.
