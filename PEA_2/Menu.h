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
	char opt = '0';
	Graph *g;

	void ReadFromFile();
	bool ReadFromFile(std::string);
	void GenerateRandomly();
	void GenerateRandomly(int, int, int);
	void DisplayData();
	void TimeMeasure();
	void TimeMeasure1();
	void SetStopCondition();
	void SetTemperatureRatio();
	void DisplayTabuSearch();
	void DisplaySimulatedAnnealing();
	void SetPopulationSize();
	void SetCrossOverRatio();
	void SetMutationRatio();
	void SetCrossOverMethod();
	void DisplayGeneticAlgorithm();

public:
	Menu();
	~Menu();

	void Choose();
	char GetOption();
	void Display();
	void DoTask();
};

