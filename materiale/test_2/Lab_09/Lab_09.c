// Lab 09 — AVL Tree (BST auto-balansat prin rotatii)
// Sursa: S11_12052025/main.c din github.com/tcervinski-csie/DS2025
// Compilare: cl /D_CRT_SECURE_NO_WARNINGS Lab_09.c
// Rulare:   pune files.txt langa exe

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 128
#define COUNT 10

// ============================================================
// 1. STRUCTS — FileProperties + TreeNode (la fel ca BST)
// ============================================================

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

// ============================================================
// 2. AVL HELPERS — treeHeight, balanceFactor
// ============================================================

// Inaltimea: cea mai lunga cale de la nod la o frunza
int treeHeight(TreeNode* tree) {
    if (tree) {
        int hLeft = treeHeight(tree->left);
        int hRight = treeHeight(tree->right);
        return 1 + (hLeft > hRight ? hLeft : hRight);
    }
    return 0;
}

// Balance factor = inaltime(dreapta) - inaltime(stanga)
//   - rezultat in {-1, 0, 1} -> arbore balansat
//   - rezultat 2 sau -2 -> dezechilibrat -> rotatie
int balanceFactor(TreeNode* tree) {
    if (tree) {
        int hLeft = treeHeight(tree->left);
        int hRight = treeHeight(tree->right);
        return hRight - hLeft;
    }
    return 0;
}

// ============================================================
// 3. ROTATII — singura idee noua fata de BST
// ============================================================

// Rotatie dreapta: cand subarborele stang e prea inalt
//
//        A                B
//       / \              / \
//      B   c     ->     d   A
//     / \                  / \
//    d   e                e   c
//
void rotateRight(TreeNode** root) {
    TreeNode* aux = (*root)->left;
    (*root)->left = aux->right;     // e devine copil stang al lui A
    aux->right = *root;              // A devine copil drept al lui B
    *root = aux;                     // B e noua radacina a subarborelui
}

// Rotatie stanga: cand subarborele drept e prea inalt
//
//      A                    B
//     / \                  / \
//    a   B        ->      A   e
//       / \              / \
//      d   e            a   d
//
void rotateLeft(TreeNode** root) {
    TreeNode* aux = (*root)->right;
    (*root)->right = aux->left;     // d devine copil drept al lui A
    aux->left = *root;               // A devine copil stang al lui B
    *root = aux;                     // B e noua radacina a subarborelui
}

// ============================================================
// 4. INSERT cu rebalansare automata
// ============================================================

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
        int bf = balanceFactor(*root);

        // Cazul "right-heavy" (bf == 2)
        if (bf == 2) {
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
        if (bf == -2) {
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

// ============================================================
// 5. DELETE cu rebalansare
// ============================================================

FP deleteTreeNode(TreeNode** root, int id) {
    FP deletedFP = { -1, NULL };

    if (!root || !*root) {
        return deletedFP;
    }

    if (id < (*root)->fp.id) {
        deletedFP = deleteTreeNode(&(*root)->left, id);
    }
    else if (id > (*root)->fp.id) {
        deletedFP = deleteTreeNode(&(*root)->right, id);
    }
    else {
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
            }
            else {
                rotateRight(&(*root)->right);
                rotateLeft(root);
            }
        }

        if (bf == -2) {
            if (balanceFactor((*root)->left) <= 0) {
                rotateRight(root);
            }
            else {
                rotateLeft(&(*root)->left);
                rotateRight(root);
            }
        }
    }

    return deletedFP;
}

// ============================================================
// 6. PRINT (la fel ca BST)
// ============================================================

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

// ============================================================
// 7. LOAD
// ============================================================

// NOTA: codul original (S11) folosea `while (!feof(f))` (anti-pattern -> crash).
// Aici am inlocuit cu pattern-ul corect: `while (fgets(...) != NULL)`.

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

// ============================================================
// 8. MAIN
// ============================================================

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
