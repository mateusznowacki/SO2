#include "philosopher.hpp"
#include <iostream>

Philosopher::Philosopher(int philosopherID, int numPhilosophers, int iters, Waiter* w)
    : id(philosopherID),
      numberOfPhilosophers(numPhilosophers),
      iterations(iters),
      waiter(w)
{
}

void Philosopher::run() {
    // Obliczamy numery widelców: lewy = id, prawy = (id + 1) % numberOfPhilosophers
    int leftFork = id;
    int rightFork = (id + 1) % numberOfPhilosophers;

    for(int i = 0; i < iterations; i++) {
        // Filozof myśli
        printState(THINKING);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // Robi się głodny
        printState(HUNGRY);

        // Pyta kelnera o widelce
        waiter->requestForks(id, leftFork, rightFork);

        // Je
        printState(EATING);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // Zwalnia widelce
        waiter->releaseForks(id, leftFork, rightFork);
    }
}

void Philosopher::printState(PhilosopherState state) {
    switch (state) {
    case THINKING:
        std::cout << "Filozof " << id << " -> MYŚLI" << std::endl;
        break;
    case HUNGRY:
        std::cout << "Filozof " << id << " -> JEST GŁODNY" << std::endl;
        break;
    case EATING:
        std::cout << "Filozof " << id << " -> JE" << std::endl;
        break;
    default:
        break;
    }
}

// Funkcja wywoływana przez std::thread
void philosopherThreadFunction(Philosopher* philosopher) {
    philosopher->run();
}
