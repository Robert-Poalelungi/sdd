# Materiale de studiu — laburi 3-9

Fiecare lab intr-un folder separat, cu un singur `.c` si CSV-ul aferent. Organizat pe testele semestrului.

```
materiale/
├── test_1/                                           Materiale pentru testul 1
│   ├── COMUN.md                                      Schelet comun + diff per lab
│   ├── Lab_03/         Lab_03.c     + cars.csv      (singly linked list)
│   ├── Lab_04/         Lab_04.c     + cars.csv      (singly + delete + function pointers)
│   ├── Lab_03_04/      Lab_03_04.c  + cars.csv      (UNIUNEA — Lab 4 e superset al Lab 3)
│   ├── Lab_05/         Lab_05.c     + movies.csv    (doubly linked list)
│   ├── Lab_05_minimal/ Lab_05_minimal.c + movies.csv (varianta minimala — cat era in seminar)
│   └── Lab_06/         Lab_06.c     + teams.csv     (hash table cu chaining)
│
└── test_2/                                           Materiale pentru testul 2
    ├── Lab_07/         Lab_07.c     + scheduler.txt (Heap)
    ├── Lab_08/         Lab_08.c     + files.txt     (BST)
    └── Lab_09/         Lab_09.c     + files.txt     (AVL)
                        test_patterns.md             (patternuri compozite pentru test)
```

**Pentru testul 1:** foloseste `test_1/Lab_03_04/` (Lab 4 contine tot Lab 3 + 3 functii noi). Pentru recapitulare rapida, citeste `test_1/COMUN.md`.

**Pentru testul 2:** intra in `test_2/Lab_09/` si citeste `test_patterns.md` — patternuri compozite (AVL → LSI cu filtru, delete cu rebalansare etc.).

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
