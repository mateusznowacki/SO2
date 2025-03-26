#include <iostream>
#include <thread>


#include "waiter.hpp"
#include "philosopher.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Użycie: " << argv[0] << " <liczba_filozofow> [liczba_iteracji_na_filozofa]" << std::endl;
        return 1;
    }

    int numberOfPhilosophers = std::stoi(argv[1]);
    int iterations = 5; // domyślna liczba cykli na filozofa

    if (argc >= 3) {
        iterations = std::stoi(argv[2]);
    }

    // Tworzymy kelnera
    Waiter waiter(numberOfPhilosophers);

    // Tablica klas Philosopher
    Philosopher** philosophers = new Philosopher*[numberOfPhilosophers];

    // Tablica wątków
    std::thread* threads = new std::thread[numberOfPhilosophers];

    // Inicjalizujemy i uruchamiamy wątki
    for (int i = 0; i < numberOfPhilosophers; i++) {
        philosophers[i] = new Philosopher(i, numberOfPhilosophers, iterations, &waiter);
        threads[i] = std::thread(philosopherThreadFunction, philosophers[i]);
    }

    // Czekamy, aż wszystkie wątki zakończą pracę
    for (int i = 0; i < numberOfPhilosophers; i++) {
        if (threads[i].joinable()) {
            threads[i].join();
        }
    }

    // Sprzątanie
    for (int i = 0; i < numberOfPhilosophers; i++) {
        delete philosophers[i];
    }
    delete[] philosophers;
    delete[] threads;

    std::cout << "Wszyscy filozofowie zakończyli jedzenie." << std::endl;
    return 0;
}
