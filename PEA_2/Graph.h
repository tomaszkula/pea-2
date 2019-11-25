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
	std::vector<int> GetNearestNeighbor();
	std::vector<int> GetRandomPermutation();
	std::vector<int> GetBestNeighbor(std::vector<int>, std::vector<std::vector<int>>&, int&);

public:
	static double stopCondition;

	Graph(int nodesCount);
	~Graph();
	void CompleteDistancesRow(int, int*);
	void Display();
	void SetStopCondition(double);
	void TabuSearch();
};

