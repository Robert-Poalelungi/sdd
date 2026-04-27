# Lab 09 — AVL Tree — walkthrough complet

**Sursa codului:** `S11_12052025/main.c` din github.com/tcervinski-csie/DS2025
**Pre-requisites:** **Lab 08 (BST) FLUENT.** Daca nu stii BST, intoarce-te.

---

## Concept central in 30 secunde

**AVL** = BST + auto-balansare prin **rotatii**.

**Problema BST:** daca inserezi 1,2,3,4,5 in ordine, arborele degenereaza in lista. Cautarea devine O(n).

**Solutie AVL:** dupa fiecare insert/delete, verifici **balance factor** la fiecare nod pe drum inapoi spre radacina. Daca depaseste +/-1, aplici **rotatii** ca sa rebalansezi. Toate operatiile raman **O(log n)**.

**3 concepte noi peste BST:**
1. `treeHeight` — inaltimea unui nod
2. `balanceFactor` — diferenta intre inaltimile copiilor
3. `rotateRight` / `rotateLeft` — operatiile de rebalansare

---

# Cod + walkthrough

## 1. Headere si structuri

```c
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 128
#define COUNT 10

typedef struct FileProperties FP;
typedef struct TreeNode TreeNode;

struct FileProperties {
    int id;
    char* filename;
};

struct TreeNode {
    FP fp;
    TreeNode* left;
    TreeNode* right;
};
```

**Pas cu pas:**
- Identic cu BST. **Nu se adauga `height` in nod** in implementarea asta — recalculezi inaltimea de fiecare data (mai lent dar mai simplu).
- `typedef ... FP;` = alias scurt pentru `FileProperties`.

**Atentie pedagogica:** alte implementari de AVL stocheaza `height` direct in nod (mai eficient). Cea de la seminar e simplificata.

---

## 2. `treeHeight` — inaltimea unui arbore

```c
int treeHeight(TreeNode* tree) {
    if (tree) {
        int hLeft = treeHeight(tree->left);
        int hRight = treeHeight(tree->right);

        return 1 + (hLeft > hRight ? hLeft : hRight);
    }
    else {
        return 0;
    }
}
```

**Pas cu pas:**

**Bloc 1 — caz de baza:**
```c
if (tree) {
    ...
} else {
    return 0;
}
```
NULL → inaltime 0. Frunza singura → inaltime 1 (1 + max(0,0) = 1).

**Bloc 2 — recursivitate pe ambele subarbori:**
```c
int hLeft = treeHeight(tree->left);
int hRight = treeHeight(tree->right);
```
Inaltimea sub-arborilor.

**Bloc 3 — returnezi 1 + maximul:**
```c
return 1 + (hLeft > hRight ? hLeft : hRight);
```
- Operator ternar: daca `hLeft > hRight` returnezi `hLeft`, altfel `hRight`.
- `+1` pentru nodul curent.

**Mnemonic:** "inaltimea = 1 + cea mai inalta ramura".

**Complexitate:** O(n) — viziteaza fiecare nod o data.

---

## 3. `balanceFactor` — cat de "stramb" e arborele

```c
int balanceFactor(TreeNode* tree) {
    if (tree) {
        int hLeft = treeHeight(tree->left);
        int hRight = treeHeight(tree->right);

        return hRight - hLeft;
    }
    else {
        return 0;
    }
}
```

**Pas cu pas:**
1. Caz de baza: NULL → 0.
2. Calculezi inaltimile copiilor.
3. Returnezi `hRight - hLeft`.

**Interpretare:**
- **0** → balansat perfect
- **-1** → stanga e cu 1 mai inalta — OK (tolerat)
- **+1** → dreapta e cu 1 mai inalta — OK (tolerat)
- **-2** → stanga PREA inalta → trebuie rotatie dreapta
- **+2** → dreapta PREA inalta → trebuie rotatie stanga

---

## 4. `rotateRight` — rotatie spre dreapta

```c
void rotateRight(TreeNode** root) {
    TreeNode* aux = (*root)->left;
    (*root)->left = aux->right;     // (*root)->left->right
    aux->right = *root;
    *root = aux;
}
```

**Diagrama:**
```
        A                    B
       / \                  / \
      B   c     ------>    d   A
     / \                      / \
    d   e                    e   c
```

**Pas cu pas:**

**Bloc 1 — salveaza copilul stang:**
```c
TreeNode* aux = (*root)->left;
```
`aux` = B (devine noua radacina).

**Bloc 2 — copilul drept al lui B devine copilul stang al lui A:**
```c
(*root)->left = aux->right;
```
e (era `B->right`) trece ca `A->left`.

**Bloc 3 — A devine copilul drept al lui B:**
```c
aux->right = *root;
```
B preia A ca subarbore drept.

**Bloc 4 — B inlocuieste A ca radacina:**
```c
*root = aux;
```

**Mnemonic vizual:** "B urca, A coboara la dreapta lui B. e (subarborele drept al lui B) trece pe stanga lui A."

**Cod scris liniar:** memoreaza ordinea celor 4 linii. Nu schimba ordinea — pierzi pointeri.

---

## 5. `rotateLeft` — rotatie spre stanga (oglinda lui Right)

```c
void rotateLeft(TreeNode** root) {
    TreeNode* aux = (*root)->right;
    (*root)->right = aux->left;     // (*root)->right->left
    aux->left = *root;
    *root = aux;
}
```

**Diagrama:**
```
      A                    B
     / \                  / \
    a   B      ------>   A   e
       / \              / \
      d   e            a   d
```

**Pas cu pas:** identic cu `rotateRight`, dar cu `left ↔ right` peste tot. E **oglinda exacta**.

**Memorare:** scrie `rotateRight` o data, apoi inlocuieste mental `left` cu `right` si invers — asa obtii `rotateLeft`.

---

## 6. `insertTree` — insert cu rebalansare automata

```c
void insertTree(TreeNode** root, FP fp) {
    if (*root) {

        // 1. INSERTIE BST normala
        if (fp.id < (*root)->fp.id) {
            insertTree(&(*root)->left, fp);
        }
        else {
            insertTree(&(*root)->right, fp);
        }

        // 2. VERIFICARE BALANCE FACTOR si REBALANSARE
        int balanceFactorValue = balanceFactor(*root);

        // Cazul "right-heavy" (bf == 2)
        if (balanceFactorValue == 2) {
            if (balanceFactor((*root)->right) == 1) {
                // RR — rotatie stanga simpla
                rotateLeft(root);
            }
            else {
                // RL — rotatie dreapta pe copilul drept, apoi rotatie stanga
                rotateRight(&(*root)->right);
                rotateLeft(root);
            }
        }

        // Cazul "left-heavy" (bf == -2)
        if (balanceFactorValue == -2) {
            if (balanceFactor((*root)->left) == -1) {
                // LL — rotatie dreapta simpla
                rotateRight(root);
            }
            else {
                // LR — rotatie stanga pe copilul stang, apoi rotatie dreapta
                rotateLeft(&(*root)->left);
                rotateRight(root);
            }
        }
    }
    else {
        // Loc liber — creeaza nodul
        TreeNode* newNode = malloc(sizeof(TreeNode));
        newNode->fp = fp;
        newNode->left = NULL;
        newNode->right = NULL;
        *root = newNode;
    }
}
```

**Pas cu pas:**

**Bloc 1 — insert BST normal (ca la Lab 08):**
```c
if (*root) {
    if (fp.id < (*root)->fp.id) {
        insertTree(&(*root)->left, fp);
    } else {
        insertTree(&(*root)->right, fp);
    }
    ...
}
```
La fel ca BST: recurgi pe stanga sau dreapta dupa cheie.

**Bloc 2 — verifica balance factor:**
```c
int balanceFactorValue = balanceFactor(*root);
```
**Important:** asta se executa **DUPA** revenirea din recursivitate. Deci verifici balance-ul la fiecare nod pe drumul inapoi spre radacina.

**Bloc 3 — caz "right-heavy" (bf == 2):**
```c
if (balanceFactorValue == 2) {
    if (balanceFactor((*root)->right) == 1) {
        rotateLeft(root);                      // RR
    } else {
        rotateRight(&(*root)->right);          // RL — primul pas
        rotateLeft(root);                      // RL — al doilea pas
    }
}
```
- bf=2 → dreapta prea inalta.
- Verifici balance-ul **copilului drept** ca sa decizi tipul de dezechilibru:
  - bf-copil-drept = +1 → RR (right-right) → rotatie stanga simpla
  - bf-copil-drept = -1 → RL (right-left) → rotatie dubla

**Bloc 4 — caz "left-heavy" (bf == -2):** simetric:
- bf-copil-stang = -1 → LL → rotatie dreapta simpla
- bf-copil-stang = +1 → LR → rotatie stanga pe copilul stang, apoi rotatie dreapta

**Bloc 5 — caz de baza:**
```c
TreeNode* newNode = malloc(sizeof(TreeNode));
newNode->fp = fp;
newNode->left = NULL;
newNode->right = NULL;
*root = newNode;
```
Identic cu BST.

---

## 7. Cele 4 cazuri AVL — fundamentale

### LL (left-left) — insertia in stanga lui copil-stang
```
       3
      /
     2          ->   rotateRight(3)
    /
   1
```
**Fix:** `rotateRight` pe radacina.

### RR (right-right) — insertia in dreapta lui copil-drept
```
   1
    \
     2          ->   rotateLeft(1)
      \
       3
```
**Fix:** `rotateLeft` pe radacina.

### LR (left-right) — insertia in dreapta lui copil-stang
```
       3
      /
     1          ->   rotateLeft(1) apoi rotateRight(3)
      \
       2
```
**Fix:** `rotateLeft` pe copilul stang, apoi `rotateRight` pe radacina.

### RL (right-left) — insertia in stanga lui copil-drept
```
   1
    \
     3          ->   rotateRight(3) apoi rotateLeft(1)
    /
   2
```
**Fix:** `rotateRight` pe copilul drept, apoi `rotateLeft` pe radacina.

### Tabelul de decizie

| `bf(root)` | `bf(copil)` | Caz | Rotatii |
|------------|-------------|-----|---------|
| +2 | dreapta = +1 | RR | `rotateLeft(root)` |
| +2 | dreapta = -1 | RL | `rotateRight(child); rotateLeft(root);` |
| -2 | stanga = -1 | LL | `rotateRight(root)` |
| -2 | stanga = +1 | LR | `rotateLeft(child); rotateRight(root);` |

**Mnemonic:** Semnul `bf` iti spune **partea opusa** (unde "atarna" arborele). Semnul copilului iti spune daca e simplu sau dublu.

---

## 8. `deleteTreeNode` — stergere cu rebalansare

```c
FP deleteTreeNode(TreeNode** root, int id) {
    FP deletedFP = { -1, NULL };

    if (!root || !*root) {
        return deletedFP;
    }

    if (id < (*root)->fp.id) {
        deletedFP = deleteTreeNode(&(*root)->left, id);
    } else if (id > (*root)->fp.id) {
        deletedFP = deleteTreeNode(&(*root)->right, id);
    } else {
        // Nod gasit
        deletedFP = (*root)->fp;

        // Caz 1 si 2: 0 sau 1 copil
        if (!(*root)->left || !(*root)->right) {
            TreeNode* temp = (*root)->left ? (*root)->left : (*root)->right;
            free(*root);
            *root = temp;
        }
        else {
            // Caz 3: 2 copii — gaseste in-order successor (cel mai mic din dreapta)
            TreeNode* successor = (*root)->right;
            while (successor->left) {
                successor = successor->left;
            }

            // Inlocuieste datele cu cele ale succesorului
            (*root)->fp.id = successor->fp.id;
            (*root)->fp.filename = successor->fp.filename;

            // Sterge succesorul (nu eliberam filename — l-am preluat)
            deleteTreeNode(&(*root)->right, successor->fp.id);
        }
    }

    // Rebalansare dupa stergere
    if (*root) {
        int bf = balanceFactor(*root);

        if (bf == 2) {
            if (balanceFactor((*root)->right) >= 0) {
                rotateLeft(root);
            } else {
                rotateRight(&(*root)->right);
                rotateLeft(root);
            }
        }

        if (bf == -2) {
            if (balanceFactor((*root)->left) <= 0) {
                rotateRight(root);
            } else {
                rotateLeft(&(*root)->left);
                rotateRight(root);
            }
        }
    }

    return deletedFP;
}
```

**Pas cu pas:**

**Bloc 1 — caz de baza:**
```c
FP deletedFP = { -1, NULL };
if (!root || !*root) return deletedFP;
```
Sentinel `{-1, NULL}` indica "nu am gasit". Daca arborele e gol, intorci sentinel-ul.

**Bloc 2 — cauta recursiv (BST):**
```c
if (id < (*root)->fp.id) {
    deletedFP = deleteTreeNode(&(*root)->left, id);
} else if (id > (*root)->fp.id) {
    deletedFP = deleteTreeNode(&(*root)->right, id);
} else {
    // gasit!
}
```

**Bloc 3 — daca am gasit nodul (else):**

3 sub-cazuri:

**3a) Nod cu 0 sau 1 copil:**
```c
if (!(*root)->left || !(*root)->right) {
    TreeNode* temp = (*root)->left ? (*root)->left : (*root)->right;
    free(*root);
    *root = temp;
}
```
- `temp` = singurul copil (sau NULL daca e frunza).
- Eliberezi nodul curent.
- Pui `temp` in locul lui (poate fi NULL).

**3b) Nod cu 2 copii — caz greu:**
```c
TreeNode* successor = (*root)->right;
while (successor->left) {
    successor = successor->left;
}
```
**In-order successor** = cel mai mic nod din subarborele drept (mergand mereu la stanga).

```c
(*root)->fp.id = successor->fp.id;
(*root)->fp.filename = successor->fp.filename;
```
**Inlocuiesti DATELE** in nodul curent (NU pointer-ii). E un trick — nu muti nodul, ci copiezi datele din succesor in nodul de sters.

```c
deleteTreeNode(&(*root)->right, successor->fp.id);
```
**Recursiv stergi succesorul** din subarborele drept (acum e duplicat). Stergerea succesorului e usoara — el are cel mult 1 copil (nu poate avea stang, altfel n-ar fi fost cel mai mic).

**Bloc 4 — rebalansare:**
```c
if (*root) {
    int bf = balanceFactor(*root);
    if (bf == 2) { ... }
    if (bf == -2) { ... }
}
```
La fel ca la insert. Verifica `if (*root)` — la stergere, `*root` poate deveni NULL.

**Diferenta vs insert:** la delete, conditia pentru rotatie dubla foloseste `>= 0` si `<= 0` in loc de `==`. De ce? Pentru ca dupa stergere, `bf-copil` poate fi 0 (nu doar +1/-1), si tot trebuie sa rotatii.

---

## 9. `printTree` — la fel ca BST

```c
void printTree(TreeNode* tree, int space) {
    if (tree) {
        space += COUNT;
        printTree(tree->right, space);
        printf("\n");
        for (int i = COUNT; i < space; i++) {
            printf(" ");
        }
        printf("%d", tree->fp.id);
        printTree(tree->left, space);
    }
}
```

Identic cu Lab 08. Vezi explicatia acolo.

---

## 10. `readAllFromFile` — citire CSV

```c
void readAllFromFile(TreeNode** tree, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Error: cannot open '%s'\n", filename);
        return;
    }

    char buffer[LINESIZE];

    while (fgets(buffer, LINESIZE, f) != NULL) {
        FP fp;

        char* token = strtok(buffer, ",");
        fp.id = atoi(token);

        token = strtok(NULL, ",");
        token[strcspn(token, "\n")] = '\0';
        fp.filename = malloc((strlen(token) + 1) * sizeof(char));
        strcpy(fp.filename, token);

        insertTree(tree, fp);
    }

    fclose(f);
}
```

Identic cu BST din Lab 08. Singura diferenta: apelezi `insertTree` (cu rebalansare) in loc de `insertTreeNode` (BST simplu).

---

## 11. `main`

```c
int main(void) {
    TreeNode* tree = NULL;
    readAllFromFile(&tree, "files.txt");
    printTree(tree, 20);

    printf("\n\n----------------------------------------------------\n\n");
    deleteTreeNode(&tree, 7);
    printTree(tree, 20);

    printf("\n\n----------------------------------------------------\n\n");
    deleteTreeNode(&tree, 11);
    printTree(tree, 20);

    printf("\n");
    return 0;
}
```

**Pas cu pas:**
1. Incarci 10 elemente din `files.txt` (id-uri 8,9,10,7,6,12,11,4,5,3).
2. Afisezi arborele balansat.
3. Stergi nodul cu id=7. Afisezi din nou (verifici ca arborele ramane balansat).
4. Stergi nodul cu id=11. Afisezi din nou.

---

# Functii pentru testul 2 (NU sunt in cod, le scrii TU)

Vezi `test_patterns.md` pentru patternuri detaliate. Pe scurt:

## A. Numarare cu conditie (varianta simpla, fara function pointer)

```c
int countByCondition(TreeNode* tree, /* parametri */) {
    if (!tree) return 0;
    int count = (/* CONDITIA TA */) ? 1 : 0;
    count += countByCondition(tree->left, /* parametri */);
    count += countByCondition(tree->right, /* parametri */);
    return count;
}
```

## B. Stergere cu criteriu cu rebalansare per stergere

Vezi `test_patterns.md` Pattern C — varianta cu loop care apeleaza `deleteTreeNode` pentru fiecare match.

## C. Copiere in alta structura (ex. lista simplu inlantuita)

Vezi `test_patterns.md` Pattern D — combina parcurgere AVL + `addToEnd` din Lab 3 (cu deep copy si redenumire `Node` -> `ListNode`).

---

# Capcane

| Capcana | Antidot |
|---------|---------|
| Inversezi `rotateLeft` cu `rotateRight` | `rotateLeft` muta radacina la stanga -> dreapta urca. Si invers. |
| Confuzii la cazurile dublu (LR, RL) | Mereu: prima rotatie pe **copil** (in directia opusa cazului), a doua pe **radacina**. |
| Uiti `if (*root)` la rebalansare dupa delete | La stergere, `*root` poate deveni NULL — testeaza inainte. |
| Folosesti `==` in loc de `>= 0` la delete | Dupa stergere, `bf` copil poate fi 0. Conditia trebuie sa fie `>= 0` / `<= 0`. |
| Crezi ca trebuie sa rotatii manual de mai multe ori | NU. Fiecare apel recursiv face rotatie locala. Recursivitatea propaga inapoi spre radacina. |
| `feof` anti-pattern | Idem ca BST/Heap. |
| 2 struct-uri `Node` in acelasi fisier (la testul 2 cu LSI) | Redenumeste: `TreeNode` si `ListNode`. |
| Shallow copy AVL -> LSI | Double-free. Foloseste `malloc + strcpy` la copiere. |

---

# Self-test (fara cod deschis)

- [ ] Pot desena un arbore din inserarea sortata `1,2,3,4,5` cu rotatii AVL.
- [ ] Pot scrie `treeHeight` si `balanceFactor` in 3 min.
- [ ] Pot scrie `rotateRight` din memorie (4 linii) in 1 min.
- [ ] Pot scrie `rotateLeft` ca oglinda a `rotateRight` in 1 min.
- [ ] Pot enumera cele 4 cazuri (LL, LR, RR, RL) si rotatiile lor.
- [ ] Pot scrie `insertTree` cu rebalansare in 10 min.
- [ ] Pot scrie `deleteTreeNode` cu in-order successor + rebalansare in 15 min.
- [ ] Pot scrie cele 3 functii pentru testul 2 (count, delete, copy) in 20 min.
- [ ] Pot explica de ce in-order pe BST/AVL da elemente sortate.

Daca bifezi 8/9 -> stapanesti AVL la nivel de examen.
