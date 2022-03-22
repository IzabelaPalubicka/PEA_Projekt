#pragma once

#include <chrono>
using namespace std;

class Timer
{
private:
	chrono::high_resolution_clock::time_point start;
	chrono::high_resolution_clock::time_point stop;
	float time;
public:
	void startTimer();
	void stopTimer();
	float timeCounter();
	Timer();
	~Timer();
};

