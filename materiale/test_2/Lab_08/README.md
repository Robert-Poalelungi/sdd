# Lab 08 — Binary Search Tree (BST) — walkthrough complet

**Sursa codului:** `S10_05052025/main.c` din github.com/tcervinski-csie/DS2025
**Pre-requisites:** Lab 3 (recursivitate pe pointeri), `Node**` pattern.

---

## Concept central in 30 secunde

**BST** = arbore binar in care:
- **Stanga** are doar chei **mai mici** ca radacina
- **Dreapta** are doar chei **mai mari** (sau egale)
- **Recursiv** — fiecare subarbore e si el un BST

**De ce inveti BST inainte de AVL:** AVL = BST + rotatii. Daca nu intelegi recursivitatea pe arbore aici, AVL e magie neagra.

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

typedef struct FileProperties FileProperties;
typedef struct TreeNode TreeNode;

struct FileProperties {
    int id;
    char* filename;
};

struct TreeNode {
    FileProperties fp;
    TreeNode* left;       // copilul stang (chei MAI MICI)
    TreeNode* right;      // copilul drept (chei MAI MARI)
};
```

**Pas cu pas:**
- `LINESIZE 128` — buffer de citire pe linie.
- `COUNT 10` — pas de indentare la afisarea arborelui (rotit cu 90 grade).
- `FileProperties` = datele utile (un id si un filename).
- `TreeNode` = nodul din arbore: are date + 2 pointeri (`left`, `right`).

**Diferenta vs lista:** in loc de un singur pointer `next`, ai 2 pointeri (`left` si `right`). Asta-i tot.

---

## 2. `insertTreeNode` — inserare recursiva

```c
void insertTreeNode(TreeNode** root, FileProperties fp) {
    if (*root) {
        if (fp.id < (*root)->fp.id) {
            insertTreeNode(&(*root)->left, fp);     // mai mic -> stanga
        }
        else {
            insertTreeNode(&(*root)->right, fp);    // mai mare/egal -> dreapta
        }
    }
    else {
        // gasit loc liber, creeaza nodul
        TreeNode* newNode = malloc(sizeof(TreeNode));
        newNode->fp = fp;
        newNode->left = NULL;
        newNode->right = NULL;
        *root = newNode;
    }
}
```

**Pas cu pas:**

**Bloc 1 — exista nod la pozitia curenta?**
```c
if (*root) {
```
- `*root` = nodul de pe pozitia curenta.
- Daca nu e NULL → exista, recurg.
- Daca e NULL → e loc liber, creez nodul nou.

**Bloc 2 — recurgi pe stanga sau dreapta:**
```c
if (fp.id < (*root)->fp.id) {
    insertTreeNode(&(*root)->left, fp);
}
else {
    insertTreeNode(&(*root)->right, fp);
}
```
- Compari cheia noua cu cheia nodului curent.
- **Mai mic** → recurgi pe `left`. Pasezi `&(*root)->left` (adresa pointerului stang) ca sa il poti modifica.
- **Mai mare sau egal** → recurgi pe `right`.

**Bloc 3 — caz de baza: loc liber, creezi nodul:**
```c
TreeNode* newNode = malloc(sizeof(TreeNode));
newNode->fp = fp;
newNode->left = NULL;
newNode->right = NULL;
*root = newNode;
```
- Aloci memorie pentru nod.
- Setezi datele si pointerii copiilor la NULL (e frunza).
- `*root = newNode;` — atribui pointer-ul la pozitia parintelui (sau radacina daca e primul nod).

**De ce `TreeNode**`?** Pentru ca atunci cand ajungi la pozitia goala (NULL), trebuie sa modifici **pointerul** care pointeaza la acel NULL — adica `parent->left` sau `parent->right`. Asta e exact ce face `*root = newNode` la baza recursivitatii. Daca foloseai `TreeNode*`, copia locala s-ar pierde la return.

**Mnemonic:** "Daca exista, recurg pe partea corecta. Daca nu, creez aici."

---

## 3. `printTree` — afisare cu rotire vizuala

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

**Pas cu pas:**

**Bloc 1 — daca arborele e gol, ies (caz de baza):**
```c
if (tree) {
```

**Bloc 2 — creste indentarea:**
```c
space += COUNT;
```
La fiecare nivel, indentezi cu inca 10 spatii.

**Bloc 3 — recursivitate pe DREAPTA prima:**
```c
printTree(tree->right, space);
```
**De ce dreapta prima?** Pentru ca afisam arborele rotit cu 90 grade — radacina la stanga, copii la dreapta. Daca afisai stanga prima, ar fi afisat invers.

**Bloc 4 — afiseaza cheia curenta cu indentare corecta:**
```c
printf("\n");
for (int i = COUNT; i < space; i++) {
    printf(" ");
}
printf("%d", tree->fp.id);
```
Salt de linie + spatii proportional cu adancimea + cheia.

**Bloc 5 — recursivitate pe STANGA:**
```c
printTree(tree->left, space);
```

**Trick vizual:** parcurgere right-radacina-left = "in-order invers" = arborele apare rotit corect cand intorci capul cu 90 grade.

---

## 4. Cele 3 parcurgeri (NU sunt in cod, dar trebuie sa le stii)

Toate sunt recursive. Difera doar **ordinea** in care apelezi:

### In-order (LRR): stanga -> radacina -> dreapta
Da elementele in **ordine sortata** (de la cel mai mic la cel mai mare).
```c
void inOrder(TreeNode* tree) {
    if (tree) {
        inOrder(tree->left);
        printf("%d ", tree->fp.id);    // proceseaza radacina
        inOrder(tree->right);
    }
}
```

### Pre-order (RLR): radacina -> stanga -> dreapta
Util pentru clonare (proceseaza parintele inainte de copii).
```c
void preOrder(TreeNode* tree) {
    if (tree) {
        printf("%d ", tree->fp.id);    // radacina prima
        preOrder(tree->left);
        preOrder(tree->right);
    }
}
```

### Post-order (LRR): stanga -> dreapta -> radacina
Util pentru `freeTree` (elibereaza copiii inainte de parinte).
```c
void postOrder(TreeNode* tree) {
    if (tree) {
        postOrder(tree->left);
        postOrder(tree->right);
        printf("%d ", tree->fp.id);    // radacina ultima
    }
}
```

**Mnemonic:** "Pozitia lui R (radacina) in numele parcurgerii iti spune unde proceseaza."

---

## 5. `readDataFromFile` — citire CSV

```c
void readDataFromFile(TreeNode** tree, const char* fileName) {
    FILE* f = fopen(fileName, "r");
    if (!f) {
        printf("Error: cannot open '%s'\n", fileName);
        return;
    }

    char buffer[LINESIZE];

    while (fgets(buffer, LINESIZE, f) != NULL) {
        FileProperties fp;

        char* token = strtok(buffer, ",");
        fp.id = atoi(token);

        token = strtok(NULL, ",");
        token[strcspn(token, "\n")] = '\0';
        fp.filename = malloc((strlen(token) + 1) * sizeof(char));
        strcpy(fp.filename, token);

        insertTreeNode(tree, fp);
    }

    fclose(f);
}
```

**Pas cu pas:**

Identic cu Lab 3-7 — schelet CSV standard:
1. `fopen` + check NULL
2. `while (fgets(...) != NULL)` — pattern corect (NU `feof`)
3. `strtok` cu `buffer` (prima oara), apoi cu `NULL`
4. `atoi` pentru int, `malloc + strcpy` pentru string
5. `strcspn(token, "\n")` — scoate `\n` din ultimul camp
6. `insertTreeNode(tree, fp)` — insereaza in arbore

**Singura diferenta vs lista:** apelezi `insertTreeNode` in loc de `addToEnd`.

---

## 6. `main`

```c
int main(void) {

    TreeNode* tree = NULL;

    readDataFromFile(&tree, "files.txt");
    printTree(tree, 20);
    printf("\n");

    return 0;
}
```

**Pas cu pas:**
1. `TreeNode* tree = NULL;` — pornesti cu arbore gol.
2. `readDataFromFile(&tree, ...)` — pasezi `&tree` (TreeNode**) ca sa poata fi modificata radacina.
3. `printTree(tree, 20)` — afiseaza arborele. `20` = padding initial.

**Atentie:** in cod NU se apeleaza `freeTree` — codul de la seminar lasa memory leak. Pentru igiena, adaugi tu (vezi sectiunea de mai jos).

---

# Functii utile (NU sunt in cod, le adaugi tu)

## A. `freeTree` — eliberare recursiva (post-order)

```c
void freeTree(TreeNode** tree) {
    if (*tree) {
        freeTree(&(*tree)->left);
        freeTree(&(*tree)->right);
        free((*tree)->fp.filename);
        free(*tree);
        *tree = NULL;
    }
}
```

**De ce post-order?** Trebuie sa eliberezi copiii **inainte** de parinte, altfel pierzi accesul la ei.

**Pas cu pas:**
1. Recurs stanga (elibereaza intreg sub-arborele stang)
2. Recurs dreapta (elibereaza intreg sub-arborele drept)
3. `free` la string-ul intern (filename)
4. `free` la nodul curent
5. Setezi pointer-ul la NULL (in caz ca apelantul mai foloseste *tree)

## B. `treeHeight` — inaltimea arborelui

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
1. Caz de baza: NULL → inaltime 0.
2. Calculezi recursiv inaltimea stanga si dreapta.
3. Returnezi `1 + max(stanga, dreapta)` — adica inaltimea maxima din copii + 1 pentru nodul curent.

**Folosit la AVL** pentru `balanceFactor`. Important.

## C. Numarare cu conditie

```c
int countByCondition(TreeNode* tree, /* parametri */) {
    if (!tree) return 0;
    int count = (/* CONDITIA TA */) ? 1 : 0;
    count += countByCondition(tree->left, /* parametri */);
    count += countByCondition(tree->right, /* parametri */);
    return count;
}
```

**Idee:** parcurgi recursiv tot arborele si numeri nodurile care indeplinesc conditia.

---

# Capcane

| Capcana | Antidot |
|---------|---------|
| Confuzia `TreeNode*` vs `TreeNode**` | Modifici radacina/copilul -> `**`. Doar citesti -> `*`. |
| Egalitate la insert: stanga sau dreapta? | Conventie. Codul de la seminar pune `>=` la dreapta. La examen, fii consecvent. |
| Recursivitate pe NULL | Mereu testeaza `if (tree)` la inceput. Ramura false returneaza 0/NULL. |
| Crezi ca BST e mereu balansat | NU. Daca inserezi 1,2,3,4,5 in ordine -> arborele degenereaza in lista. AVL fixeaza asta. |
| `feof` anti-pattern | La examen, prefera `while (fgets(...) != NULL)`. |
| Free in ordine gresita (parinte inainte de copii) | Mereu post-order: copii intai, parinte la final. |

---

# Self-test (fara cod deschis)

- [ ] Pot desena BST-ul din inserarea: 8,9,10,7,6,12,11,4,5,3.
- [ ] Pot scrie `insertTreeNode` recursiv in 4 min.
- [ ] Pot scrie cele 3 parcurgeri (in/pre/post) in 5 min.
- [ ] Pot scrie `freeTree` in 3 min, post-order.
- [ ] Pot scrie `treeHeight` in 2 min.
- [ ] Pot explica de ce in-order da elementele sortate.

Daca bifezi 6/6 -> esti gata pentru AVL.
