# Lab 07 — Heap (max-heap pe vector)

**Sursa codului:** `S9_28042025/main.c` din github.com/tcervinski-csie/DS2025
**Pre-requisites:** vectori, malloc/realloc, recursivitate.
**Timp estimat prima data:** 90-120 min.

---

## Concept central

**Heap = arbore binar complet, stocat ca vector.** Aici e **max-heap**: parintele are mereu prioritate >= ambele copii.

**Mapping vector <-> arbore:**
```
index 0     -> radacina
index i     -> parintele: (i-1)/2
              copil stang:  2*i+1
              copil drept:  2*i+2
```

```
            [10]                  vector: [10, 7, 9, 4, 5, 8, 6]
           /     \                index:   0  1  2  3  4  5  6
        [7]       [9]
       /   \     /   \
     [4]   [5] [8]   [6]
```

---

## Cele 2 operatii fundamentale

### Sift-up (urcare) — folosit la `insertHeap`
Pui elementul la finalul vectorului, apoi cat timp e **mai mare ca parintele**, swap. Te oprestii cand respecti regula sau ajungi la radacina.

### Sift-down (coborare) — folosit la `heapify`
Pornesti de la un index, gasesti cel mai mare dintre el + copii, swap cu cel mai mare. Recursiv pe copilul cu care ai schimbat. Te oprestii cand parintele >= ambii copii.

**Insertia urca, stergerea coboara.** Tine minte asta.

---

## Functii in Lab_07.c

| Functie | Ce face | Complexitate |
|---------|---------|--------------|
| `initHeap` | aloca un heap gol | O(1) |
| `insertHeap` | adauga + sift-up | O(log n) |
| `heapify` | sift-down de la index | O(log n) |
| `buildHeap` | din array -> heap valid | O(n) |
| `deleteFromHeap` | extrage maxul + heapify | O(log n) |
| `readTasksFromFile` | citire din CSV | O(n log n) |
| `printHeap` | afiseaza prioritatile | O(n) |

---

## Pasi de invatare

### Pas 1 — Mapping vector-arbore (15 min)
Pe hartie, deseneaza vectorul `[10, 7, 9, 4, 5]` ca arbore. Calculeaza pentru fiecare index ce e parintele si ce sunt copiii.

### Pas 2 — `insertHeap` cu sift-up (20 min)
- Pune la final, apoi while compara cu parintele
- Mnemonic: "urca pana esti sef"
- Test pe hartie: insereaza 15 in `[10, 7, 9, 4, 5]`. Vezi cum urca.

### Pas 3 — `heapify` (sift-down) (20 min)
- Cea mai grea functie aici, recursiva
- Mnemonic: "gaseste cel mai mare dintre eu+copii, daca nu sunt eu -> swap si continua de acolo"
- Test pe hartie: aplica heapify pe `[3, 10, 9, 7, 5]` la index 0. Vezi cum 3 coboara.

### Pas 4 — `deleteFromHeap` (15 min)
3 pasi:
1. Salveaza `tasks[0]` (maxul)
2. Pune ultimul element pe pozitia 0
3. `size--`, apoi heapify de la 0

(In cod e o varianta cu loop pentru toti indexii, dar logic e suficient `heapify(heap, 0)`.)

### Pas 5 — `buildHeap` din array (15 min)
- Pornesti de la mijlocul vectorului si mergi spre 0
- Aplici heapify pe fiecare
- De ce de la `size/2 - 1`? Toti indexii > size/2 sunt frunze - n-au copii, deci nu trebuie heapify.

### Pas 6 — Testul 2 — cele 3 functii cerute (45 min)
Vezi sectiunea "Functii pentru testul 2" mai jos.

---

## Functii pentru testul 2 (Week 13 Requirements)

**Atentie:** testul nu cere `insertHeap` sau `heapify`. Cere combinatii peste structura.

### 1. Numarare cu conditie
```c
int countByCondition(Heap heap, int (*cond)(Task)) {
    int count = 0;
    for (int i = 0; i < heap.size; i++) {
        if (cond(heap.tasks[i])) count++;
    }
    return count;
}
```
Exemplu functie de conditie:
```c
int isHighPriority(Task t) { return t.priority > 20; }
```

### 2. Stergere cu criteriu (cu heapify dupa fiecare stergere)
```c
void deleteByCondition(Heap* heap, int (*cond)(Task)) {
    int i = 0;
    while (i < heap->size) {
        if (cond(heap->tasks[i])) {
            // swap cu ultimul, scade size, heapify
            swapTask(&heap->tasks[i], &heap->tasks[heap->size - 1]);
            free(heap->tasks[heap->size - 1].description);
            heap->size -= 1;
            heap->tasks = realloc(heap->tasks, heap->size * sizeof(Task));
            heapify(*heap, i);
            // NU incrementa i — noul element de pe pozitia i poate indeplini si el conditia
        } else {
            i++;
        }
    }
}
```

**Atentie:** dupa swap+heapify, pozitia i contine alt element — verifici si pe el. De aia `if-else` cu `i++` doar pe ramura de "nu sterge".

### 3. Salvare in alta structura cu filtru
```c
// varianta cu vector dinamic
Task* saveMatching(Heap heap, int (*cond)(Task), int* outSize) {
    Task* result = NULL;
    *outSize = 0;
    for (int i = 0; i < heap.size; i++) {
        if (cond(heap.tasks[i])) {
            (*outSize)++;
            result = realloc(result, (*outSize) * sizeof(Task));
            result[*outSize - 1] = heap.tasks[i];   // sau initTask pentru copie deep
        }
    }
    return result;
}
```
Sau salvare in lista (simpla / dubla) folosind functii din Lab 3/5.

---

## Capcane

| Capcana | Antidot |
|---------|---------|
| `(i-1)/2` cand i=0 da -1/2=0 in C - parinte = el insusi | OK in cod, ciclul iese pentru ca nu se mai face swap |
| `realloc(ptr, 0)` cand size devine 0 | Comportament platforma-specific. In practica, OK in lab. |
| Confuzii intre sift-up si sift-down | `insertHeap` urca un element. `heapify` coboara un element. |
| `heapify` recursiv vs iterativ | Codul de la seminar e recursiv. La examen poti scrie oricare. |
| `buildHeap` cu `size/2 - 1` | Frunzele nu au copii, deci heapify pe ele nu schimba nimic. Pornim de pe ultimul **non-frunza**. |
| `feof` in `readTasksFromFile` | Anti-pattern clasic in C. Poate procesa o data in plus la EOF. La seminar a mers, dar la examen e mai sigur `while (fgets(...) != NULL)`. |

---

## Self-test (fara cod deschis)

- [ ] Pot desena vectorul `[10, 5, 8, 3, 4, 7, 2]` ca arbore in 1 min.
- [ ] Pot scrie `insertHeap` (cu sift-up) in 5 min.
- [ ] Pot scrie `heapify` recursiv in 5 min.
- [ ] Pot scrie `deleteFromHeap` in 5 min.
- [ ] Pot scrie cele 3 functii pentru testul 2 (count, delete, save) in 15 min.
- [ ] Pot explica de ce `buildHeap` e O(n), nu O(n log n).

Daca bifezi 6/6 -> stapanesti Heap. Treci la BST.
