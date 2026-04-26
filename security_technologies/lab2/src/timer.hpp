#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <cstdint>

class Timer {
public:
    Timer();
    void start();
    void stop();
    uint64_t getCycles() const;
    double getSeconds() const;
    
    static uint64_t rdtsc();
    
private:
    std::chrono::high_resolution_clock::time_point start_time_;
    std::chrono::high_resolution_clock::time_point end_time_;
    uint64_t start_cycles_;
    uint64_t end_cycles_;
    bool running_;
};

#endif // TIMER_H