#include "waiter.hpp"
#include <iostream>

Waiter::Waiter(int n) {
    numberOfForks = n;
    // Dynamicznie alokujemy tablicę booli: forkAvailable
    forkAvailable = new bool[n];
    for(int i = 0; i < n; i++) {
        forkAvailable[i] = true;
    }
}

Waiter::~Waiter() {
    delete[] forkAvailable;
}

void Waiter::requestForks(int philosopherID, int leftFork, int rightFork) {
    std::unique_lock<std::mutex> lock(mtx);

    // Czekamy tak długo, aż oba widelce będą wolne
    condition.wait(lock, [&](){
        return forkAvailable[leftFork] && forkAvailable[rightFork];
    });

    // Rezerwujemy widelce
    forkAvailable[leftFork] = false;
    forkAvailable[rightFork] = false;
}

void Waiter::releaseForks(int philosopherID, int leftFork, int rightFork) {
    std::unique_lock<std::mutex> lock(mtx);

    // Zwalniamy widelce
    forkAvailable[leftFork] = true;
    forkAvailable[rightFork] = true;

    // Powiadamiamy oczekujące wątki
    condition.notify_all();
}
