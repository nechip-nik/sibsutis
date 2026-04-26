#include "timer.hpp"
#include <x86intrin.h>

Timer::Timer() : running_(false) {}

void Timer::start() {
    start_time_ = std::chrono::high_resolution_clock::now();
    start_cycles_ = rdtsc();
    running_ = true;
}

void Timer::stop() {
    end_time_ = std::chrono::high_resolution_clock::now();
    end_cycles_ = rdtsc();
    running_ = false;
}

uint64_t Timer::getCycles() const {
    if (running_) return 0;
    return end_cycles_ - start_cycles_;
}

double Timer::getSeconds() const {
    if (running_) return 0.0;
    return std::chrono::duration<double>(end_time_ - start_time_).count();
}

uint64_t Timer::rdtsc() {
    return __rdtsc();
}