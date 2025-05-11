#include <iostream>

#include "headers/waiter.hpp"

// The Waiter class is responsible for managing forks in the
// Dining Philosophers problem. It prevents deadlock by controlling
// when forks can be acquired and released.

Waiter::Waiter(int n) {
    numberOfForks = n;

    // Dynamically allocate a boolean array to track fork availability.
    forkAvailable = new bool[n];
    for (int i = 0; i < n; i++) {
        // Initially, all forks are available.
        forkAvailable[i] = true;
    }
}

Waiter::~Waiter() {
    // Clean up the dynamically allocated array.
    delete[] forkAvailable;
}

void Waiter::requestForks(int philosopherID,
                          int leftFork, int rightFork) {
    // Use a unique_lock to lock the mutex. This allows us to use CVs.
    std::unique_lock<std::mutex> lock(mtx);

    // Wait until both forks are available.
    while (!forkAvailable[leftFork] || !forkAvailable[rightFork]) {
        condition.wait(lock);
    }

    // Mark forks as unavailable.
    forkAvailable[leftFork] = false;
    forkAvailable[rightFork] = false;
}

void Waiter::releaseForks(int philosopherID,
                          int leftFork, int rightFork) {
    std::unique_lock<std::mutex> lock(mtx);

    // Mark forks as available.
    forkAvailable[leftFork] = true;
    forkAvailable[rightFork] = true;

    // Notify waiting threads.
    condition.notify_all();
}
