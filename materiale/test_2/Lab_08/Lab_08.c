// Lab 08 — Binary Search Tree (BST) — fundatia pentru AVL
// Sursa: S10_05052025/main.c din github.com/tcervinski-csie/DS2025
// Compilare: cl /D_CRT_SECURE_NO_WARNINGS Lab_08.c
// Rulare:   pune files.txt langa exe

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 128
#define COUNT 10

// ============================================================
// 1. STRUCTS — FileProperties + TreeNode
// ============================================================

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

// ============================================================
// 2. INSERT — recursiv
// ============================================================

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

// ============================================================
// 3. PRINT — afisare arbore "intors" (radacina la stanga, copii la dreapta)
// ============================================================

// Trick vizual: printeaza right-radacina-left in loc de left-radacina-right
// si rotesti capul cu 90 grade -> arata ca un arbore normal
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
// 4. LOAD — citire din fisier
// ============================================================

// NOTA: codul original (S10) folosea `while (!feof(f))` (anti-pattern -> crash).
// Aici am inlocuit cu pattern-ul corect: `while (fgets(...) != NULL)`.

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

// ============================================================
// 5. MAIN
// ============================================================

int main(void) {

    TreeNode* tree = NULL;

    readDataFromFile(&tree, "files.txt");
    printTree(tree, 20);
    printf("\n");

    return 0;
}
