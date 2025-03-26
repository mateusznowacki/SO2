#ifndef WAITER_HPP
#define WAITER_HPP

#include <mutex>
#include <condition_variable>

/*
 * Klasa Waiter (Kelner) zarządza dostępnością widelców.
 */
class Waiter {
private:
    bool* forkAvailable;                // tablica dostępności widelców
    int numberOfForks;
    std::mutex mtx;                    // mutex do ochrony zasobów
    std::condition_variable condition; // warunek do synchronizacji wątków

public:
    Waiter(int n);
    ~Waiter();

    void requestForks(int philosopherID, int leftFork, int rightFork);
    void releaseForks(int philosopherID, int leftFork, int rightFork);
};

#endif
