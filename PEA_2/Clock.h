#pragma once
#include <windows.h>
#include <iostream>

class Clock
{
private:
	double PCFreq;
	__int64 CounterStart;

public:
	Clock();
	~Clock();
	double getCounter();
	void startCounter();
};

