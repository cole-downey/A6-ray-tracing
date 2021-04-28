#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>
#include <iostream>
#include <iomanip>

class Timer {
private:
    std::chrono::steady_clock::time_point tStart;
    std::chrono::steady_clock::time_point tEnd;
    bool running;
public:
    Timer() : running(false) {};
    ~Timer() {};
    void start() { tStart = std::chrono::high_resolution_clock::now(); running = true; };
    void stop() { tEnd = std::chrono::high_resolution_clock::now(); running = false; };
    void printTime();

};

void Timer::printTime() {
    if (running) stop();
    double timediff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
    std::cout << "Time elapsed: " << std::setfill('0');
    std::cout << std::setw(2) << ((int)timediff / (1000 * 60 * 60)) % 60 << ":";
    std::cout << std::setw(2) << ((int)timediff / (1000 * 60)) % 60 << ":";
    std::cout << std::setw(2) << ((int)timediff / (1000)) % 60 << ".";
    std::cout << std::setw(2) << (int)(timediff / 10) % 100 << std::endl;
}
#endif