#ifndef SPINLOCK_H
#define SPINLOCK_H

/**
 * SpinLock – a simple busy-wait mutual exclusion lock.
 * Uses atomic built-in GCC functions to implement a binary lock.
 * This is NOT reentrant and should only be used for short critical sections.
 */
class SpinLock {
private:
    volatile int locked = 0;

public:
    void lock() {
        // Busy wait until the lock is acquired
        while (__sync_lock_test_and_set(&locked, 1)) {}
    }

    void unlock() {
        // Release the lock
        __sync_lock_release(&locked);
    }
};

#endif
