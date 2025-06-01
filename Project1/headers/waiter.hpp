#ifndef PROJECT1_HEADERS_WAITER_HPP_
#define PROJECT1_HEADERS_WAITER_HPP_

#include <mutex>
#include <condition_variable>

/**
 * The Waiter class is responsible for managing access to forks in the
 * Dining Philosophers problem. It ensures that philosophers can safely
 * acquire and release forks without causing a deadlock.
 */
class Waiter {
 public:
    /**
     * Constructor.
     * @param n Number of philosophers (and forks).
     */
    explicit Waiter(int n);

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

 private:
    bool* forkAvailable;                 // Availability of each fork
    int numberOfForks;                   // Total number of forks
    std::mutex mtx;                      // Mutex to protect shared resources
    std::condition_variable condition;   // Thread synchronization
};

#endif  // PROJECT1_HEADERS_WAITER_HPP_
