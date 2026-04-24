# Laburi 3-6 — versiune one-file

Fiecare lab intr-un folder separat, cu un singur `.c` si CSV-ul aferent.

```
onefile/
├── Lab_03/  Lab_03.c  + cars.csv    (singly linked list)
├── Lab_04/  Lab_04.c  + cars.csv    (singly + delete + function pointers)
├── Lab_05/  Lab_05.c  + movies.csv  (doubly linked list)
└── Lab_06/  Lab_06.c  + teams.csv   (hash table cu chaining)
```

## Compilare

Intra in folderul labului, apoi:

**Developer Command Prompt (MSVC):**
```
cd Lab_03
cl /D_CRT_SECURE_NO_WARNINGS Lab_03.c
Lab_03.exe
```

**gcc / MinGW:**
```
cd Lab_03
gcc Lab_03.c -o Lab_03
./Lab_03
```

**Visual Studio:** File -> New -> Project -> Empty C++ Project, drag `Lab_0X.c` inauntru, build. Pune `.csv`-ul langa executabil (sau langa `.vcxproj` cand rulezi din VS).

## Structura interna a fiecarui fisier

```
1. includes
2. Data struct (F1Car / Movie / FootballTeam) + printX
3. Node + (List / HashTable daca e cazul) + operatii
4. loadX (CSV -> struct -> container)
5. main
```

## Diferenta fata de laburile originale

- **Lab_05** e **completat**: `parseList`, `deleteBeginning`, `deleteEnd`, `freeList` nu erau scrise in codul original, aici sunt (conform README-ului din `Lab_05/`).
- **Lab_04 main**: `#include <string.h>` adaugat (lipsea, desi se foloseste `strcmp`).
- Restul: identic cu originalul, doar fuzionat intr-un singur fisier.
