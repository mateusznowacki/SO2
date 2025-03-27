#ifndef HEADERS_PHILOSOPHER_HPP_
#define HEADERS_PHILOSOPHER_HPP_

#include <thread>
#include <chrono>

#include "headers/waiter.hpp"
#include "headers/states.hpp"

class Philosopher {
public:
    // Constructor
    Philosopher(int philosopherID, int numPhilosophers,
                int iters, Waiter* w);

    // Main method of the philosopher's actions
    void run();

    // Method to print the state of the philosopher
    void printState(PhilosopherState state);

private:
    int id;
    int numberOfPhilosophers;
    int iterations;
    Waiter* waiter;  // Pointer to the waiter object
};

// Wrapper function for the thread
void philosopherThreadFunction(Philosopher* philosopher);

#endif  // HEADERS_PHILOSOPHER_HPP_
