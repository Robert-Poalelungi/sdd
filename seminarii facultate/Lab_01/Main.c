#include <stdio.h>
#include <stdlib.h>

// ============================================================
// Lab 01 - Introduction to Pointers in C
// ============================================================


// --- Section 4: Pass by reference ---
void increment(int* p) {
    (*p)++;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// --- Section 7: Function pointers ---
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

void applyAndPrint(int a, int b, int (*op)(int, int), const char* name) {
    printf("%s(%d, %d) = %d\n", name, a, b, op(a, b));
}


int main() {

    // --------------------------------------------------------
    // 1. Basic pointers: declaration, address-of (&), dereference (*)
    // --------------------------------------------------------
    printf("=== 1. Basic Pointers ===\n");

    int x = 42;
    int* p = &x;

    printf("Value of x:       %d\n", x);
    printf("Address of x:     %p\n", (void*)&x);
    printf("Pointer p holds:  %p\n", (void*)p);
    printf("Value via *p:     %d\n", *p);

    *p = 100;
    printf("After *p = 100, x = %d\n\n", x);


    // --------------------------------------------------------
    // 2. Pointer arithmetic
    // --------------------------------------------------------
    printf("=== 2. Pointer Arithmetic ===\n");

    int arr[5] = {10, 20, 30, 40, 50};
    int* ptr = arr;

    for (int i = 0; i < 5; i++) {
        printf("ptr + %d -> address: %p, value: %d\n", i, (void*)(ptr + i), *(ptr + i));
    }

    printf("Pointer difference: %td\n\n", (ptr + 4) - ptr);


    // --------------------------------------------------------
    // 3. Arrays and pointers
    // --------------------------------------------------------
    printf("=== 3. Arrays and Pointers ===\n");

    // Array name decays to a pointer to its first element
    printf("arr[2]      = %d\n", arr[2]);
    printf("*(arr + 2)  = %d\n", *(arr + 2));
    printf("ptr[2]      = %d\n", ptr[2]);
    printf("*(ptr + 2)  = %d\n\n", *(ptr + 2));


    // --------------------------------------------------------
    // 4. Pass by reference (pointers as function arguments)
    // --------------------------------------------------------
    printf("=== 4. Pass by Reference ===\n");

    int val = 5;
    printf("Before increment: %d\n", val);
    increment(&val);
    printf("After increment:  %d\n", val);

    int a = 3, b = 7;
    printf("Before swap: a = %d, b = %d\n", a, b);
    swap(&a, &b);
    printf("After swap:  a = %d, b = %d\n\n", a, b);


    // --------------------------------------------------------
    // 5. Pointer to pointer (double pointer)
    // --------------------------------------------------------
    printf("=== 5. Double Pointers ===\n");

    int n = 99;
    int* pn = &n;
    int** ppn = &pn;

    printf("n    = %d\n", n);
    printf("*pn  = %d\n", *pn);
    printf("**ppn = %d\n", **ppn);

    **ppn = 55;
    printf("After **ppn = 55, n = %d\n\n", n);


    // --------------------------------------------------------
    // 6. Dynamic memory allocation (malloc, realloc, free)
    // --------------------------------------------------------
    printf("=== 6. Dynamic Memory Allocation ===\n");

    // malloc
    int* dynArr = malloc(3 * sizeof(int));
    if (dynArr == NULL) {
        printf("Allocation failed\n");
        return 1;
    }
    dynArr[0] = 1; dynArr[1] = 2; dynArr[2] = 3;
    printf("dynArr: %d %d %d\n", dynArr[0], dynArr[1], dynArr[2]);

    // realloc - grow to 5 elements
    int* temp = realloc(dynArr, 5 * sizeof(int));
    if (temp == NULL) {
        printf("Reallocation failed\n");
        free(dynArr);
        return 1;
    }
    dynArr = temp;
    dynArr[3] = 4; dynArr[4] = 5;
    printf("After realloc: %d %d %d %d %d\n", dynArr[0], dynArr[1], dynArr[2], dynArr[3], dynArr[4]);

    free(dynArr);
    dynArr = NULL;
    printf("Memory freed, pointer set to NULL\n\n");


    // --------------------------------------------------------
    // 7. NULL pointers and void pointers
    // --------------------------------------------------------
    printf("=== 7. NULL and void* ===\n");

    int* nullPtr = NULL;
    printf("nullPtr = %p\n", (void*)nullPtr);

    if (nullPtr == NULL) {
        printf("Always check for NULL before dereferencing!\n");
    }

    // void* is a generic pointer — must be cast before dereferencing
    int y = 77;
    void* vp = &y;
    printf("void* points to: %d\n\n", *(int*)vp);


    // --------------------------------------------------------
    // 8. const pointers
    // --------------------------------------------------------
    printf("=== 8. const Pointers ===\n");

    int m = 10, k = 20;

    // Pointer to const: cannot modify the value through the pointer
    const int* pc = &m;
    printf("*pc = %d (read-only through pointer)\n", *pc);
    pc = &k;  // OK: the pointer itself can change
    printf("*pc after pc = &k: %d\n", *pc);

    // Const pointer: the pointer itself cannot change
    int* const cp = &m;
    *cp = 99;  // OK: value can be modified
    printf("m after *cp = 99: %d\n", m);

    // Const pointer to const: neither can change
    const int* const cpc = &k;
    printf("*cpc = %d (fully read-only)\n\n", *cpc);


    // --------------------------------------------------------
    // 9. Function pointers
    // --------------------------------------------------------
    printf("=== 9. Function Pointers ===\n");

    int (*op)(int, int);

    op = add;
    applyAndPrint(3, 4, op, "add");

    op = multiply;
    applyAndPrint(3, 4, op, "multiply");

    return 0;
}
