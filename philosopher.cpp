#include "headers/philosopher.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

#include "headers/waiter.hpp"

// Global (or static) mutex used to synchronize console output
static std::mutex printMutex;

/**
 * Constructor for the Philosopher class.
 * @param philosopherID Unique ID of the philosopher (0..n-1).
 * @param numPhilosophers Total number of philosophers at the table.
 * @param iters Number of times each philosopher will repeat the eat/think cycle.
 * @param w Pointer to the Waiter object, which manages forks to prevent deadlock.
 */
Philosopher::Philosopher(int philosopherID, int numPhilosophers, int iters, Waiter* w)
    : id(philosopherID),
      numberOfPhilosophers(numPhilosophers),
      iterations(iters),
      waiter(w)
{
}

/**
 * Main loop of the philosopher's actions:
 * 1. Think
 * 2. Get hungry and request forks
 * 3. Eat
 * 4. Release forks
 * Repeats for the specified number of iterations.
 */
void Philosopher::run()
{
    // The left fork is associated with this philosopher's ID.
    // The right fork is (ID + 1) % numberOfPhilosophers.
    int leftFork = id;
    int rightFork = (id + 1) % numberOfPhilosophers;

    for (int i = 0; i < iterations; i++)
    {
        // Only philosopher with ID = 0 prints the iteration number
        // in order to avoid multiple "Iteracja X" prints from each thread.
        if (id == 0)
        {
            std::lock_guard<std::mutex> lk(printMutex);
            std::cout << "\nIteracja " << (i + 1) << std::endl;
        }
        // Short pause so philosopher 0 can print the iteration number first
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

/**
 * Prints the current state of the philosopher.
 * @param state The state to be printed (THINKING, HUNGRY, or EATING).
 */
void Philosopher::printState(PhilosopherState state)
{
    std::lock_guard<std::mutex> lk(printMutex);

    switch (state)
    {
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

/**
 * Thread function that simply calls philosopher->run().
 * This function is passed to the std::thread constructor.
 */
void philosopherThreadFunction(Philosopher* philosopher)
{
    philosopher->run();
}
