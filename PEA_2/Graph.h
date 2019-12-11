#pragma once
#include "Clock.h"

#include <iostream>
#include <string>
#include <vector>
#include <time.h>

class Graph
{
private:
	int nodesCount, **distances;

	int GetDistance(std::vector<int>);

	//for TabuSearch
	std::vector<int> InitSolution();
	std::vector<int> GetNearestNeighbor();
	std::vector<int> GetRandomPermutation();
	std::vector<int> GetBestNeighbor(std::vector<int>, std::vector<std::vector<int>>&, int&);
	std::vector<int> GetCloseNeighbor(std::vector<int>);

public:
	static double stopCondition;
	static double temperatureRatio;

	Graph(int nodesCount);
	~Graph();
	int GetNodesCount();
	void CompleteDistancesRow(int, int*);
	void Display();
	void TabuSearch();
	void SimulatedAnnealing();
};

