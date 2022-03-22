#include "Timer.h"
#include <iostream>
using namespace std::chrono;

void Timer::startTimer()
{
	start = chrono::high_resolution_clock::now();
}

void Timer::stopTimer()
{
	stop = chrono::high_resolution_clock::now();
}

float Timer::timeCounter()
{
	duration<float> odds = duration_cast<duration<float>>(stop - start);
	time = odds.count();
	return time;
}

Timer::Timer()
{
}

Timer::~Timer()
{
}
