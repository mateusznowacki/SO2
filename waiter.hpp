#ifndef WAITER_HPP
#define WAITER_HPP

#include <mutex>
#include <condition_variable>

/**
 * The Waiter class is responsible for managing access to forks in the
 * Dining Philosophers problem. It ensures that philosophers can safely
 * acquire and release forks without causing a deadlock.
 */
class Waiter {
private:
    bool* forkAvailable;                // Array indicating availability of each fork
    int numberOfForks;                  // Total number of forks (equal to number of philosophers)
    std::mutex mtx;                     // Mutex to protect shared resources
    std::condition_variable condition;  // Condition variable for thread synchronization

public:
    /**
     * Constructor.
     * @param n Number of philosophers (and forks).
     */
    Waiter(int n);

    /**
     * Destructor.
     * Releases the dynamically allocated fork availability array.
     */
    ~Waiter();

    /**
     * Called by a philosopher to request access to two forks.
     * Blocks if either fork is currently unavailable.
     * @param philosopherID ID of the requesting philosopher.
     * @param leftFork Index of the left fork.
     * @param rightFork Index of the right fork.
     */
    void requestForks(int philosopherID, int leftFork, int rightFork);

    /**
     * Called by a philosopher to release the two forks after eating.
     * @param philosopherID ID of the philosopher.
     * @param leftFork Index of the left fork.
     * @param rightFork Index of the right fork.
     */
    void releaseForks(int philosopherID, int leftFork, int rightFork);
};

#endif // WAITER_HPP
