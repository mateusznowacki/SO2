#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

#include "headers/philosopher.hpp"
#include "headers/waiter.hpp"

// Global (or static) mutex used to synchronize console output
static std::mutex printMutex;

Philosopher::Philosopher(int philosopherID, int numPhilosophers,
                         int iters, Waiter* w)
    : id(philosopherID),
      numberOfPhilosophers(numPhilosophers),
      iterations(iters),
      waiter(w) {}

void Philosopher::run() {
    int leftFork = id;
    int rightFork = (id + 1) % numberOfPhilosophers;

    for (int i = 0; i < iterations; i++) {
        // Only philosopher with ID = 0 prints the iteration number
        if (id == 0) {
            std::lock_guard<std::mutex> lk(printMutex);
            std::cout << "\nIteracja " << (i + 1) << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        // 1. Think
        printState(THINKING);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // 2. Announce hunger and request forks from the Waiter
        printState(HUNGRY);
        waiter->requestForks(id, leftFork, rightFork);

        // 3. Eat
        printState(EATING);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // 4. Release forks
        waiter->releaseForks(id, leftFork, rightFork);
    }
}

void Philosopher::printState(PhilosopherState state) {
    std::lock_guard<std::mutex> lk(printMutex);

    switch (state) {
    case THINKING:
        std::cout << "Philosopher " << id << " -> THINK" << std::endl;
        break;
    case HUNGRY:
        std::cout << "Philosopher " << id << " -> IS HUNGRY" << std::endl;
        break;
    case EATING:
        std::cout << "Philosopher " << id << " -> EAT" << std::endl;
        break;
    }
}

void philosopherThreadFunction(Philosopher* philosopher) {
    philosopher->run();
}
