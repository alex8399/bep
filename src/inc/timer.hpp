/*
 * This file is taken from:
 * https://github.com/alex8399/algo_eng_project
 *
 * Original author: Aleksandr Vardanian
 */

#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <chrono>

using TimerTime = long long;
using ChronoTime = std::chrono::microseconds;

class Timer
{
private:
    enum class State
    {
        RUNNING,
        PAUSED
    };

    Timer::State m_state = Timer::State::PAUSED;
    
    TimerTime m_elapsedTime = 0;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_end;
public:
    Timer() {};
    void start();
    void stop();
    void reset();
    TimerTime getElapsedTime() const;
};

#endif
