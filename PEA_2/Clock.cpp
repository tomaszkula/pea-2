#include "Clock.h"

Clock::Clock()
{
	this->PCFreq = 0.0;
	this->CounterStart = 0;
}

Clock::~Clock()
{
}

double Clock::getCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

void Clock::startCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		std::cout << "Licznik nie dziala.";

	PCFreq = double(li.QuadPart) / 1000000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
