#pragma once

#include <type_traits>
#include <chrono>

struct Stopwatch final {
    Stopwatch() { reset(); }
    Stopwatch& reset() { startTime_ = clock_.now(); return *this; }
    double elapsed() { return std::chrono::duration<double>(clock_.now() - startTime_).count(); }
private:
    std::chrono::high_resolution_clock clock_;
    std::chrono::high_resolution_clock::time_point startTime_;
};
