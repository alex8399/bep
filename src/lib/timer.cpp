
/*
 * This file is taken from:
 * https://github.com/alex8399/algo_eng_project
 *
 * Original author: Aleksandr Vardanian
 */

#include "timer.hpp"

#include <chrono>
#include <stdexcept>

void Timer::start()
{
    if (!(m_state == Timer::State::PAUSED))
    {
        throw std::runtime_error("Timer can be started only in paused state");
    }

    m_state = Timer::State::RUNNING;
    m_start = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
    m_end = std::chrono::high_resolution_clock::now();

    if (!(m_state == Timer::State::RUNNING))
    {
        throw std::runtime_error("Timer can be stopped only in running state");
    }

    m_state = Timer::State::PAUSED;
    m_elapsedTime += std::chrono::duration_cast<ChronoTime>(m_end - m_start).count();
}

TimerTime Timer::getElapsedTime() const
{
    if (!(m_state == Timer::State::PAUSED))
    {
        throw std::runtime_error("Result can be collected only in paused state");
    }

    return m_elapsedTime;
}

void Timer::reset()
{
    if (!(m_state == Timer::State::PAUSED))
    {
        throw std::runtime_error("Timer can be reset only in paused state");
    }

    m_elapsedTime = 0;
}