#pragma once

class Thread {
public:
    void setInterval(unsigned long interval) { this->interval = interval; }
    void onRun(void (*callback)()) { this->callback = callback; }
    
    // For testing
    void run() { if (callback) callback(); }
    
private:
    unsigned long interval;
    void (*callback)();
};

class ThreadController {
public:
    void add(Thread* thread) {
        if (threadCount < MAX_THREADS) {
            threads[threadCount++] = thread;
        }
    }
    
    void run() {
        for (int i = 0; i < threadCount; i++) {
            if (threads[i]) {
                threads[i]->run();
            }
        }
    }
    
private:
    static const int MAX_THREADS = 10;
    Thread* threads[MAX_THREADS] = {nullptr};
    int threadCount = 0;
};
