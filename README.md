# 🍝 SO2_Projekt – Problem jedzących filozofów

---

## 🧠 Opis problemu

**Problem jedzących filozofów** to klasyczny problem synchronizacji procesów. Przy okrągłym stole siedzi `N` filozofów,
a między nimi znajduje się `N` widelców – po jednym między każdą parą. Filozofowie naprzemiennie **myślą** i **jedzą
spaghetti**, przy czym do jedzenia potrzebują **obu sąsiednich widelców**.

### Potencjalne problemy:

- **Deadlock (zakleszczenie)**: gdy każdy filozof trzyma jeden widelec i czeka na drugi – nikt nie może jeść.
- **Starvation (głodzenie)**: niektórzy filozofowie nigdy nie dostają widelców, bo są stale wyprzedzani przez innych.

---

## 🛠️ Kompilacja i uruchomienie

Projekt korzysta z **CMake** i działa na systemie **Windows**.

### 🔧 Kompilacja

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### ▶️ Uruchomienie

```bash
./build/SO2.exe <liczba_filozofów> <liczba_iteracji>
```

### Przykład:

```bash
./SO2.exe 5 10
```

---

## ⚙️ Opis działania

- Każdy filozof to osobny wątek.
- Wątki wykonują pętlę: **think → get hungry → try to eat → eat → release forks**
- Program kończy się po wykonaniu przez każdego filozofa zadanej liczby iteracji.

---

## 🧵 Wątki

| Element         | Reprezentacja                                              |
|-----------------|------------------------------------------------------------|
| Filozof         | Wątek `std::thread`                                        |
| Widelce         | Indeksy w tablicy `bool forkAvailable[]` w klasie `Waiter` |
| Kelner (Waiter) | Wspólny obiekt kontrolujący dostęp do widelców             |

---

## 🔐 Sekcje krytyczne

| Sekcja                          | Rozwiązanie                                                |
|---------------------------------|------------------------------------------------------------|
| Dostęp do konsoli (wypisywanie) | `std::mutex` – globalny `printMutex`                       |
| Dostęp do widelców              | `std::mutex` + `std::condition_variable` w klasie `Waiter` |

### Zasada działania `Waiter`:

- Filozof **prosi** kelnera o dwa widelce.
- Jeśli oba są dostępne – dostaje je.
- Jeśli nie – **czeka**, aż zostaną zwolnione.
- Po zjedzeniu **oddaje** widelce i sygnalizuje czekającym.

To rozwiązanie zapewnia:

- brak zakleszczeń,
- brak głodzenia,
- sprawiedliwe podejście do współdzielonych zasobów.

---

## 💻 Przykładowy wynik działania

```
Iteration 1
Philosopher 0 -> THINK
Philosopher 1 -> THINK
Philosopher 2 -> THINK
Philosopher 1 -> IS HUNGRY
Philosopher 1 -> EAT
Philosopher 1 -> THINK
...
All philosophers have finished eating.
```

---

## 📁 Struktura projektu

```
.
├── main.cpp               # Główna pętla programu
├── philosopher.hpp/cpp    # Klasa filozofa (wątki)
├── waiter.hpp/cpp         # Klasa kelnera (synchronizacja)
├── CMakeLists.txt         # Konfiguracja budowania
└── README.md              # Dokumentacja projektu
```

---

## 📝 Uwagi końcowe

- Program nie używa gotowych klas do zarządzania filozofami czy forkami – wszystko zaimplementowano ręcznie przy użyciu
  `std::mutex` i `std::condition_variable`.

- Całość przetestowana z różną liczbą filozofów (3–10).

---

© 2025 | Systemy Operacyjne 2 

