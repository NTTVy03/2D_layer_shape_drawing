#pragma once

#include <chrono>

using namespace std::chrono;

class Timer {
private:
    static time_point<high_resolution_clock> start;
    static time_point<high_resolution_clock> stop;

public:
    static void startTime() {
        start = high_resolution_clock::now();
    }

    static void stopTime() {
        stop = high_resolution_clock::now();
    }

    static double getDuration() {
        return duration_cast<nanoseconds>(stop - start).count() / 1000000.0;
    }
};

time_point<high_resolution_clock> Timer::start;
time_point<high_resolution_clock> Timer::stop;