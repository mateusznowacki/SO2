#ifndef PHILOSOPHER_HPP
#define PHILOSOPHER_HPP

#include "states.hpp"
#include "waiter.hpp"
#include <thread>
#include <chrono>

class Philosopher {
private:
    int id;
    int numberOfPhilosophers;
    int iterations;
    Waiter* waiter; // wskaźnik do kelnera

public:
    // Konstruktor
    Philosopher(int philosopherID, int numPhilosophers, int iters, Waiter* w);

    // Metoda główna, którą wywoła wątek
    void run();

    // Pomocnicza metoda do czytelnego wypisywania stanu
    void printState(PhilosopherState state);
};

// Funkcja startująca wątek (tzw. wrapper), żeby można było użyć w std::thread
void philosopherThreadFunction(Philosopher* philosopher);

#endif
