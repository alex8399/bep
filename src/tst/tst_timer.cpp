/*
 * This file is taken from:
 * https://github.com/alex8399/algo_eng_project
 *
 * Original author: Aleksandr Vardanian
 */

#include <gtest/gtest.h>
#include "timer.hpp"
#include <thread>
#include <chrono>

TEST(TimerTests, StartFromInitialDoesNotThrow)
{
    Timer timer;
    EXPECT_NO_THROW(timer.start());
}

TEST(TimerTests, StartTwiceThrows)
{
    Timer timer;
    timer.start();
    EXPECT_THROW(timer.start(), std::runtime_error);
}

TEST(TimerTests, StopWithoutStartThrows)
{
    Timer timer;
    EXPECT_THROW(timer.stop(), std::runtime_error);
}

TEST(TimerTests, GetResultWithoutStopThrows)
{
    Timer timer;
    timer.start();
    EXPECT_THROW(timer.getElapsedTime(), std::runtime_error);
}

TEST(TimerTests, StopAfterStartDoesNotThrow)
{
    Timer timer;
    timer.start();
    EXPECT_NO_THROW(timer.stop());
}

TEST(TimerTests, GetResultAfterStopDoesNotThrow)
{
    Timer timer;
    timer.start();
    std::this_thread::sleep_for(ChronoTime(20));
    timer.stop();

    EXPECT_NO_THROW(timer.getElapsedTime());
}

TEST(TimerTests, ResultIsNonNegative)
{
    Timer timer;
    timer.start();
    std::this_thread::sleep_for(ChronoTime(100));
    timer.stop();

    EXPECT_GE(timer.getElapsedTime(), 0);
}

TEST(TimerTests, CanRestart)
{
    Timer timer;

    timer.start();
    std::this_thread::sleep_for(ChronoTime(100));
    timer.stop();
    TimerTime first = timer.getElapsedTime();

    EXPECT_NO_THROW(timer.start());
    std::this_thread::sleep_for(ChronoTime(100));
    EXPECT_NO_THROW(timer.stop());
    TimerTime second = timer.getElapsedTime();

    EXPECT_GE(first, 0);
    EXPECT_GE(second, 0);

    EXPECT_NO_THROW(timer.reset());
}

TEST(TimerTests, StopTwiceThrows)
{
    Timer timer;
    timer.start();
    timer.stop();
    EXPECT_THROW(timer.stop(), std::runtime_error);
}

TEST(TimerTests, GetResultBeforeFinished)
{
    Timer timer;
    EXPECT_EQ(timer.getElapsedTime(), 0);
}