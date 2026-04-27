# Lab 08 — Binary Search Tree (BST)

**Sursa codului:** `S10_05052025/main.c` din github.com/tcervinski-csie/DS2025
**Pre-requisites:** Lab 3 (recursivitate pe pointeri), `Node**` pattern.
**Timp estimat prima data:** 60-90 min.

---

## De ce inveti BST inainte de AVL

**AVL = BST + rotatii.** Daca nu intelegi cum functioneaza un BST normal, rotatiile sunt magie neagra. Aici fixezi:
- Recursivitate pe arbore
- `TreeNode**` pentru a modifica radacina
- Cele 3 parcurgeri (in/pre/post-order)

---

## Concept central

**BST** = arbore binar in care:
- **Stanga** are doar chei **mai mici** ca radacina
- **Dreapta** are doar chei **mai mari** (sau egale) ca radacina
- **Recursiv** — fiecare subarbore e si el un BST

Exemplu cu insertia in ordinea: 8, 9, 10, 7, 6, 12, 11, 4, 5, 3:

```
                  8
                /   \
               7     9
              /       \
             6         10
            /            \
           4              12
          / \            /
         3   5          11
```

---

## Functii in Lab_08.c

| Functie | Ce face |
|---------|---------|
| `insertTreeNode` | adauga recursiv (stanga/dreapta dupa cheie) |
| `printTree` | afiseaza arborele rotit cu 90 grade |
| `readOneFromFile` | citeste o linie CSV -> FileProperties |
| `readDataFromFile` | parcurge fisierul si insereaza |

---

## Pasi de invatare

### Pas 1 — Recursivitate pe arbore (15 min)

`insertTreeNode` e schema recursiva fundamentala:

```c
void insertTreeNode(TreeNode** root, FileProperties fp) {
    if (*root) {
        // exista nod -> recurg pe stanga sau dreapta
        if (fp.id < (*root)->fp.id)
            insertTreeNode(&(*root)->left, fp);
        else
            insertTreeNode(&(*root)->right, fp);
    } else {
        // *root e NULL -> aici ii e locul, creeaza-l
        TreeNode* newNode = malloc(sizeof(TreeNode));
        newNode->fp = fp;
        newNode->left = NULL;
        newNode->right = NULL;
        *root = newNode;
    }
}
```

**De ce `TreeNode**`?** Pentru ca atunci cand ajungi la pozitia goala (NULL), trebuie sa modifici **pointerul** care pointeaza la acel NULL — adica `parent->left` sau `parent->right`. Asta e exact ce face `*root = newNode` la baza recursivitatii.

### Pas 2 — Cele 3 parcurgeri (15 min)

Toate sunt recursive. Difera doar **ordinea** in care apelezi:

**In-order (LRR):** stanga -> radacina -> dreapta
- Da elementele in **ordine sortata** (de la cel mai mic la cel mai mare)
```c
void inOrder(TreeNode* tree) {
    if (tree) {
        inOrder(tree->left);
        printf("%d ", tree->fp.id);    // proceseaza radacina
        inOrder(tree->right);
    }
}
```

**Pre-order (RLR):** radacina -> stanga -> dreapta
- Util pentru clonare (proceseaza parintele inainte de copii)
```c
void preOrder(TreeNode* tree) {
    if (tree) {
        printf("%d ", tree->fp.id);    // proceseaza radacina prima
        preOrder(tree->left);
        preOrder(tree->right);
    }
}
```

**Post-order (LRR):** stanga -> dreapta -> radacina
- Util pentru `freeTree` (elibereaza copiii inainte de parinte)
```c
void postOrder(TreeNode* tree) {
    if (tree) {
        postOrder(tree->left);
        postOrder(tree->right);
        printf("%d ", tree->fp.id);    // proceseaza radacina ultima
    }
}
```

### Pas 3 — `printTree` cu rotire vizuala (10 min)

Trick-ul: parcurgere **right-radacina-left** (invers de in-order) cu indentare crescatoare. Cand intorci capul cu 90 grade -> arata ca arbore normal.

Nu e in cerintele testului — il poti sari.

### Pas 4 — `readDataFromFile` (10 min)

Schelet CSV identic cu lab 3-6, doar ca apeleaza `insertTreeNode` cu pointer la radacina.

### Pas 5 — `freeTree` (de adaugat manual, ~10 min)

Lipseste din S10. **Trebuie scris** pentru igiena memoriei. Pattern post-order:

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

### Pas 6 — Drill (10 min)

Foaie alba. Scrie `insertTreeNode` + cele 3 parcurgeri + `freeTree` din memorie.

---

## Functii utile (pentru AVL si testul 2)

### Numarare in arbore (parcurgere + contor)
```c
int countByCondition(TreeNode* tree, int (*cond)(FileProperties)) {
    if (tree) {
        int count = cond(tree->fp) ? 1 : 0;
        count += countByCondition(tree->left, cond);
        count += countByCondition(tree->right, cond);
        return count;
    }
    return 0;
}
```

### Inaltimea arborelui (folosit la AVL pentru balance factor)
```c
int treeHeight(TreeNode* tree) {
    if (tree) {
        int hLeft = treeHeight(tree->left);
        int hRight = treeHeight(tree->right);
        return 1 + (hLeft > hRight ? hLeft : hRight);
    }
    return 0;
}
```

---

## Capcane

| Capcana | Antidot |
|---------|---------|
| Confuzia `TreeNode*` vs `TreeNode**` | Modifici radacina/copilul -> `**`. Doar citesti -> `*`. |
| Egalitate la insert: stanga sau dreapta? | Conventie. Aici codul pune `>=` la dreapta. La examen, fii consecvent. |
| Recursivitate pe NULL | Mereu testeaza `if (tree)` la inceput, ramura altfel returneaza 0/NULL. |
| `feof` anti-pattern | La examen, prefera `while (fgets(...) != NULL)`. |

---

## Self-test

- [ ] Pot desena BST-ul din inserarea: 8,9,10,7,6,12,11,4,5,3.
- [ ] Pot scrie `insertTreeNode` recursiv in 4 min.
- [ ] Pot scrie cele 3 parcurgeri (in/pre/post) in 5 min.
- [ ] Pot scrie `freeTree` in 3 min, post-order.
- [ ] Pot scrie `treeHeight` in 2 min.
- [ ] Pot explica de ce in-order da elementele sortate.

Daca bifezi 6/6 -> esti gata pentru AVL.
