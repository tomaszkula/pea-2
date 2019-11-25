#pragma once
#include "Graph.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <regex>

class Menu
{
private:
	int opt = 0;
	Graph *g;

	void ReadFromFile();
	void GenerateRandomly();
	void GenerateRandomly(int, int, int);
	void DisplayData();
	void TimeMeasure();
	void SetStopCondition();
	void DisplayTabuSearch();

public:
	Menu(int optToQuit = 0);
	~Menu();

	void Choose();
	int GetOption();
	void Display();
	void DoTask();
};

