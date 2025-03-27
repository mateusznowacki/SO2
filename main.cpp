#include <iostream>
#include <thread>

#include "headers/waiter.hpp"
#include "headers/philosopher.hpp"

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0]
            << " <number_of_philosophers> <iterations_per_philosopher>"
            << std::endl;
        return 1;
    }

    int numberOfPhilosophers = std::stoi(argv[1]);
    int iterations = std::stoi(argv[2]);

    // Create the waiter
    Waiter waiter(numberOfPhilosophers);

    // Array of Philosopher objects
    Philosopher** philosophers = new Philosopher*[numberOfPhilosophers];

    // Array of threads
    std::thread* threads = new std::thread[numberOfPhilosophers];

    // Initialize and start all philosopher threads
    for (int i = 0; i < numberOfPhilosophers; i++)
    {
        philosophers[i] = new Philosopher(i, numberOfPhilosophers,
                                          iterations, &waiter);
        threads[i] = std::thread(philosopherThreadFunction, philosophers[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < numberOfPhilosophers; i++)
    {
        if (threads[i].joinable())
        {
            threads[i].join();
        }
    }

    // Clean up dynamically allocated memory
    for (int i = 0; i < numberOfPhilosophers; i++)
    {
        delete philosophers[i];
    }
    delete[] philosophers;
    delete[] threads;

    std::cout << "All philosophers have finished eating." << std::endl;
    return 0;
}
