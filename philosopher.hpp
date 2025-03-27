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
    Waiter* waiter; // pointer to the waiter object

public:
    // Constructor
    Philosopher(int philosopherID, int numPhilosophers, int iters, Waiter* w);

    // Main method of the philosopher's actions
    void run();

    // Method to print the state of the philosopher
    void printState(PhilosopherState state);
};

// Wrapper function for the thread
void philosopherThreadFunction(Philosopher* philosopher);

#endif
