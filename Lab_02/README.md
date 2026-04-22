# Lab 02 — Array Dinamic de Structuri

Primul lab „real": ai o structură (`F1Car`) cu câmpuri dinamice (`char*`), le stochezi într-un **array dinamic**, și le încarci dintr-un CSV.

---

## Structura fișierelor

```
F1Car.h    → structura F1Car + prototipuri
F1Car.c    → loadCars + printCar + freeCars
main.c     → folosește array-ul
cars.csv   → datele de intrare
```

**Diferența vs Lab 03:** aici stochezi într-un `F1Car*` (array contigu), nu într-o listă. La fiecare linie nouă din CSV, faci `realloc` ca să mai încapă încă un element.

---

## PAS 1 — `F1Car.h`

```c
#pragma once

typedef struct {
    int number;
    char* driver;        // alocat dinamic
    char* team;          // alocat dinamic
    float engine_hp;
} F1Car;

int loadCars(const char* filename, F1Car** cars);
void printCar(const F1Car* car);
void freeCars(F1Car* cars, int count);
```

**Atenție la `F1Car** cars` în `loadCars`:** vrem să modificăm pointerul `cars` din `main`, deci primim adresa lui (double pointer).

---

## PAS 2 — `loadCars` (array dinamic cu `realloc`)

```c
int loadCars(const char* filename, F1Car** cars) {

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: could not open file '%s'\n", filename);
        return -1;
    }

    *cars = malloc(0);               // array gol la început (sau NULL)

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), f) != NULL) {

        char* pos = strchr(line, '\n');      // scoate '\n'
        if (pos) *pos = '\0';

        // RELOAC: crește array-ul cu 1 element
        F1Car* temp = realloc(*cars, (count + 1) * sizeof(F1Car));
        if (temp == NULL) {
            printf("Error: memory allocation failed\n");
            free(*cars);
            *cars = NULL;
            fclose(f);
            return -1;
        }
        *cars = temp;

        F1Car* car = &(*cars)[count];        // pointer la noul element

        // Parsing CSV
        char* token = strtok(line, ",");
        car->number = atoi(token);

        token = strtok(NULL, ",");
        car->driver = malloc(strlen(token) + 1);
        strcpy(car->driver, token);

        token = strtok(NULL, ",");
        car->team = malloc(strlen(token) + 1);
        strcpy(car->team, token);

        token = strtok(NULL, ",");
        car->engine_hp = atof(token);

        count++;
    }

    fclose(f);
    return count;
}
```

**Pattern critic:**
1. **Realloc sigur** — folosește `temp`, verifică NULL, abia apoi `*cars = temp`
2. **`&(*cars)[count]`** = adresa poziției `count` din array (unde completezi noua mașină)
3. **`strlen(token) + 1`** pentru `\0`
4. **`malloc(0)` la început** — truc pentru ca `realloc` să meargă mereu (`realloc(NULL, ...)` e valid și el, deci și `*cars = NULL` merge)

---

## PAS 3 — `printCar`

```c
void printCar(const F1Car* car) {
    printf("Car #%d - %s from %s - HP: %.1f\n",
        car->number, car->driver, car->team, car->engine_hp);
}
```

**`const F1Car*`** = doar citim, nu modificăm.

---

## PAS 4 — `freeCars` (ORDINEA e critică)

```c
void freeCars(F1Car* cars, int count) {
    for (int i = 0; i < count; i++) {
        free(cars[i].driver);          // 1. eliberează strings din fiecare F1Car
        free(cars[i].team);
        cars[i].driver = NULL;         // bună practică
        cars[i].team = NULL;
    }
    free(cars);                        // 2. eliberează array-ul în sine
}
```

**Ordine:**
1. Pentru fiecare element: `free` la `driver` și `team` (strings alocate separat)
2. La final: `free(cars)` pentru array-ul de structuri

**Dacă inversezi ordinea:** pierzi accesul la `driver`/`team` → memory leak.

---

## PAS 5 — `main.c`

```c
#include <stdio.h>
#include "F1Car.h"

int main() {
    F1Car* cars = NULL;
    int count = loadCars("cars.csv", &cars);      // &cars - funcția modifică pointerul

    if (count >= 0) {
        for (int i = 0; i < count; i++) {
            printCar(&cars[i]);                   // &cars[i] - adresa elementului
        }
    } else {
        printf("Failed to load cars.\n");
    }

    freeCars(cars, count);
    return 0;
}
```

**Observă:**
- `&cars` la `loadCars` (double pointer, pentru că modifică `cars`)
- `cars` singur la `freeCars` (primește `F1Car*`, nu modifică pointerul extern)
- `&cars[i]` = „adresa elementului i" = `cars + i`

---

## Array dinamic vs Listă (Lab 02 vs Lab 03)

| Aspect              | Array dinamic (Lab 02) | Listă simplă (Lab 03) |
|---------------------|------------------------|------------------------|
| Memorie             | **contigu** în RAM     | noduri risipite        |
| Acces aleator `[i]` | **O(1)**               | O(n) (parcurgi)        |
| Inserare capăt      | O(n) amortizat (realloc) | O(n) sau O(1) cu tail |
| Inserare la mijloc  | O(n) (shift)           | O(n) (parcurgi)        |
| Overhead/element    | 0                      | 1 pointer (next)       |
| Cache-friendly      | ✅                     | ❌                     |

**Când alegi ce:**
- Array dinamic → știi aproximativ câte elemente ai, acces aleator frecvent
- Listă → inserții/ștergeri frecvente oriunde, dimensiune imprevizibilă

---

## Cheat-sheet Lab 02

```c
// 1. Pointer la array dinamic - îl modifici în funcție
F1Car** cars   →   în apel: &cars, în interior: *cars

// 2. Realloc sigur
F1Car* temp = realloc(*cars, (count + 1) * sizeof(F1Car));
if (!temp) { free(*cars); *cars = NULL; return -1; }
*cars = temp;

// 3. Acces la elementul de pe poziția count
F1Car* car = &(*cars)[count];
// e același lucru cu:
F1Car* car = *cars + count;

// 4. Parsing CSV pentru un string
token = strtok(..., ",");
car->driver = malloc(strlen(token) + 1);
strcpy(car->driver, token);

// 5. Free complet
for (i) { free(cars[i].driver); free(cars[i].team); }
free(cars);
```

## Reguli de aur

1. **Realloc folosește variabilă temporară** (dacă eșuează, păstrezi pointerul original)
2. **`realloc(NULL, ...)` e OK** — se comportă ca `malloc`
3. **`&(*cars)[count]` = adresa poziției count în array**
4. **`strcspn(line, "\n")` sau `strchr(line, '\n')` ca să scoți newline-ul**
5. **Eliberezi stringurile din fiecare element ÎNAINTE să eliberezi array-ul**
6. **`strtok` modifică string-ul original** (înlocuiește `,` cu `\0`) — de aia folosești un buffer local
