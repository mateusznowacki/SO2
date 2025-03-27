#include "waiter.hpp"
#include <iostream>

// The Waiter class is responsible for managing forks in the Dining Philosophers problem.
// It prevents deadlock by controlling when forks can be acquired and released.

Waiter::Waiter(int n)
{
    // Number of forks is equal to the number of philosophers.
    numberOfForks = n;

    // Dynamically allocate a boolean array to track fork availability.
    forkAvailable = new bool[n];
    for (int i = 0; i < n; i++)
    {
        // Initially, all forks are available.
        forkAvailable[i] = true;
    }
}

Waiter::~Waiter()
{
    // Clean up the dynamically allocated array.
    delete[] forkAvailable;
}

void Waiter::requestForks(int philosopherID, int leftFork, int rightFork)
{
    // Use a unique_lock to lock the mutex. This allows us to use condition variables.
    std::unique_lock<std::mutex> lock(mtx);

    // If either fork is not available, wait until the condition variable is signaled.
    while (!forkAvailable[leftFork] || !forkAvailable[rightFork])
    {
        condition.wait(lock);
    }

    // Once both forks are available, mark them as unavailable for this philosopher.
    forkAvailable[leftFork] = false;
    forkAvailable[rightFork] = false;
}

void Waiter::releaseForks(int philosopherID, int leftFork, int rightFork)
{
    std::unique_lock<std::mutex> lock(mtx);

    // Release both forks by marking them as available.
    forkAvailable[leftFork] = true;
    forkAvailable[rightFork] = true;

    // Notify all waiting threads that forks may now be available.
    condition.notify_all();
}
