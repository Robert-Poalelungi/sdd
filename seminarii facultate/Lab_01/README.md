# Lab 01 — Introducere în Pointeri

Lab-ul fundamental — fără asta, nimic din Lab 2-6 nu are sens.

---

## 1. Pointer de bază

```c
int x = 42;
int* p = &x;        // p reține ADRESA lui x

printf("%d", x);    // 42       — valoarea
printf("%p", &x);   // 0x...    — adresa lui x
printf("%p", p);    // 0x...    — aceeași adresă
printf("%d", *p);   // 42       — valoarea de la adresa p (dereference)

*p = 100;           // modifică x prin p
printf("%d", x);    // 100
```

**3 operatori esențiali:**
- `&x` = adresa lui x
- `*p` = valoarea de la adresa p (dereference)
- `int*` = tipul „pointer la int"

---

## 2. Pointer arithmetic

```c
int arr[5] = {10, 20, 30, 40, 50};
int* ptr = arr;               // arr "decade" automat la &arr[0]

*(ptr + 0)  // 10
*(ptr + 2)  // 30
*(ptr + 4)  // 50
```

**Important:** `ptr + 1` nu adaugă 1 byte — adaugă `sizeof(int)` bytes (4). Compilatorul știe tipul.

### Array = pointer

```c
arr[2]      // același lucru cu
*(arr + 2)  // și
ptr[2]      // și
*(ptr + 2)
```

Numele unui array „decade" automat la un pointer la primul element.

---

## 3. Pass by reference

Funcțiile primesc **copii** ale parametrilor. Dacă vrei să modifici variabila originală, trimiți **adresa** ei:

```c
void increment(int* p) {
    (*p)++;               // modifică ce se află la adresa p
}

int val = 5;
increment(&val);          // trimiți adresa
// val este acum 6
```

**Exemplu swap:**
```c
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int x = 3, y = 7;
swap(&x, &y);             // x=7, y=3
```

**Regulă:** fără `*` și `&`, swap-ul n-ar funcționa (ar schimba doar copiile locale).

---

## 4. Double pointers

Un pointer la un pointer:

```c
int n = 99;
int* pn = &n;
int** ppn = &pn;

printf("%d", n);      // 99
printf("%d", *pn);    // 99
printf("%d", **ppn);  // 99

**ppn = 55;           // modifică n prin două nivele
```

**Când îl folosești?** Când vrei ca o funcție să schimbe **ce spre arată un pointer** (vezi Lab 03 — `Node** list`).

---

## 5. Alocare dinamică (malloc / realloc / free)

```c
// malloc - alocă N bytes, returnează void*
int* arr = malloc(3 * sizeof(int));
if (arr == NULL) { /* eroare */ }

arr[0] = 1; arr[1] = 2; arr[2] = 3;

// realloc - schimbă dimensiunea
int* temp = realloc(arr, 5 * sizeof(int));
if (temp == NULL) { free(arr); /* eroare */ }
arr = temp;

arr[3] = 4; arr[4] = 5;

// free - eliberează memoria
free(arr);
arr = NULL;           // bună practică: evită dangling pointer
```

**Reguli de aur:**
1. Mereu verifică `== NULL` după `malloc/realloc`
2. Pentru `realloc`, folosește **variabilă temporară** (dacă eșuează, pierzi pointerul original)
3. După `free`, setează pointerul la `NULL`
4. **Orice `malloc` are un `free` corespunzător** — altfel memory leak

---

## 6. NULL și void*

```c
int* p = NULL;                    // nu arată spre nimic
if (p == NULL) { /* nu dereferenția! */ }

void* vp = &someInt;              // pointer generic
int val = *(int*)vp;              // cast înainte de dereference
```

**`void*`:** pointer „fără tip" — poate stoca orice adresă, dar trebuie castat înainte de a fi folosit. `malloc` returnează `void*`.

---

## 7. const pointers

Trei combinații:

```c
int m = 10;

// a) Pointer la const — nu modifici valoarea prin pointer, dar pointerul poate re-pointa
const int* pc = &m;
// *pc = 99;     // EROARE
pc = &k;         // OK

// b) Pointer const — pointerul nu re-pointează, dar valoarea se modifică
int* const cp = &m;
*cp = 99;        // OK
// cp = &k;      // EROARE

// c) Const pointer la const — totul e read-only
const int* const cpc = &m;
// *cpc = 99;    // EROARE
// cpc = &k;     // EROARE
```

**Memento:** citești de la dreapta la stânga:
- `const int*` = pointer la (int const) — valoarea e const
- `int* const` = const pointer la int — pointerul e const

---

## 8. Function pointers

```c
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

// Declarație function pointer
int (*op)(int, int);

op = add;
printf("%d", op(3, 4));       // 7

op = multiply;
printf("%d", op(3, 4));       // 12
```

**Pattern:** `tip_retur (*nume)(parametri)`.

**Function pointer ca parametru:**
```c
void applyAndPrint(int a, int b, int (*op)(int, int), const char* name) {
    printf("%s(%d,%d) = %d\n", name, a, b, op(a, b));
}

applyAndPrint(3, 4, add, "add");
applyAndPrint(3, 4, multiply, "multiply");
```

---

## Cheat-sheet Lab 01

```c
// Declarare pointer
int* p = &x;

// Dereference (valoarea)
int val = *p;

// Modificare prin pointer
*p = 100;

// Double pointer
int** pp = &p;

// Alocare dinamică
int* arr = malloc(n * sizeof(int));
if (!arr) return;
// ... folosire ...
free(arr); arr = NULL;

// Realloc sigur
int* tmp = realloc(arr, newSize);
if (!tmp) { free(arr); return; }
arr = tmp;

// Function pointer
int (*op)(int, int) = add;
int r = op(3, 4);
```

## Reguli de aur

1. **`&` = ia adresa, `*` = dereferențiază**
2. **Array decade la pointer la primul element**
3. **Pass by reference = trimite `&var`, primește `tip*`**
4. **Double pointer = modifici unde arată un pointer extern**
5. **Mereu verifică `malloc`/`realloc` pentru NULL**
6. **`free` + `pointer = NULL` după**
7. **`const T*` = valoare read-only | `T* const` = pointer read-only**
8. **`sizeof(type)` pentru `malloc`, nu numere hardcoded**
