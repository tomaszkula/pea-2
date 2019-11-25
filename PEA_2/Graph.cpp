#include "Graph.h"

double Graph::stopCondition = 10.0;

Graph::Graph(int nodesCount)
{
	this->nodesCount = nodesCount;

	distances = new int *[nodesCount];
	for (int i = 0; i < nodesCount; i++)
	{
		distances[i] = new int[nodesCount];
	}
}

Graph::~Graph()
{
	for (int i = 0; i < nodesCount; i++)
	{
		delete[] distances[i];
	}
	delete[] distances;
}

void Graph::CompleteDistancesRow(int rowId, int *row)
{
	delete[] distances[rowId];
	distances[rowId] = row;
}

void Graph::Display()
{
	for (int i = 0; i < nodesCount; i++)
	{
		for (int j = 0; j < nodesCount; j++)
		{
			std::cout << distances[i][j] << "\t";
		}
		std::cout << "\n";
	}
}

void Graph::SetStopCondition(double stopTime)
{
	stopCondition = stopTime;
}

void Graph::TabuSearch()
{
	double stopConditionMs = stopCondition * 1000000.0;
	int tabuCount = nodesCount * nodesCount;

	//std::vector<int> currentPath = GetNearestNeighbor();
	std::vector<int> currentPath = GetRandomPermutation();
	int currentDistance = GetDistance(currentPath);

	std::vector<int> minPath = currentPath;
	int minDistance = currentDistance;

	std::vector<std::vector<int>> tabuList(nodesCount + 1);
	for (auto& t : tabuList) t = std::vector<int>(nodesCount + 1, 0);

	Clock c;
	c.startCounter();
	while(c.getCounter() < stopConditionMs)
	{
		int minDistanceNeighbor = INT_MAX, iTabu = 1, jTabu = 2;
		for (int i = 1; i < currentPath.size() - 2; i++)
		{
			for (int j = i + 1; j < currentPath.size() - 1; j++)
			{
				int temp = currentPath[i];
				currentPath[i] = currentPath[j];
				currentPath[j] = temp;

				int distance = GetDistance(currentPath);

				temp = currentPath[i];
				currentPath[i] = currentPath[j];
				currentPath[j] = temp;

				if ((currentPath[i] > currentPath[j] ? tabuList[currentPath[j]][currentPath[i]] <= 0 :
					tabuList[currentPath[i]][currentPath[j]] <= 0) || distance < minDistance)
				{
					if (distance < minDistanceNeighbor)
					{
						minDistanceNeighbor = distance;
						iTabu = i;
						jTabu = j;
					}
				}
			}
		}

		for (int i = 0; i < tabuList.size() - 1; i++)
		{
			for (int j = i + 1; j < tabuList.size(); j++)
			{
				if (tabuList[i][j] > 0) tabuList[i][j]--;
			}
		}

		//std::cout << minDistanceNeighbor << "\n";

		currentPath[iTabu] > currentPath[jTabu] ? tabuList[currentPath[jTabu]][currentPath[iTabu]] += tabuCount : 
			tabuList[currentPath[iTabu]][currentPath[jTabu]] += tabuCount;

		int temp = currentPath[iTabu];
		currentPath[iTabu] = currentPath[jTabu];
		currentPath[jTabu] = temp;

		//currentPath = GetBestNeighbor(currentPath, tabuList, tabuCount);
		currentDistance = GetDistance(currentPath);
		if (currentDistance < minDistance)
		{
			minPath = currentPath;
			minDistance = currentDistance;
		}
		//std::cout << minDistanceNeighbor << "\n";
	}

	std::cout << "Najkrotszy dystans(koszt): " << minDistance << "\n";
	std::cout << "Sciezka: ";
	for (int i = 0; i < minPath.size() - 1; i++) std::cout << minPath[i] << " --> ";
	std::cout << minPath[minPath.size() - 1] << "\n";
}

int Graph::GetDistance(std::vector<int> path)
{
	int distance = 0;
	for (int i = 0; i < path.size() - 1; i++)
	{
		distance += distances[path[i]][path[i + 1]];
	}
	return distance;
}

std::vector<int> Graph::GetNearestNeighbor() //greedy algorithm for first permutation
{
	std::vector<int> path(nodesCount + 1);
	bool *visited = new bool[nodesCount];
	for (int i = 0; i < nodesCount; i++) visited[i] = false;
	visited[0] = true;

	for (int i = 0; i < nodesCount - 1; i++)
	{
		int minDinstance = INT_MAX;
		for (int j = 0; j < nodesCount; j++)
		{
			if (i == j || visited[j]) continue;

			if (distances[i][j] < minDinstance)
			{
				minDinstance = distances[i][j];
				path[i + 1] = j;
			}
		}
		visited[path[i + 1]] = true;
	}
	path[nodesCount] = 0;

	return path;
}

std::vector<int> Graph::GetRandomPermutation()
{
	srand(time(NULL));
	std::vector<int> nodes(nodesCount + 1, 0);
	for (int i = 1; i < nodesCount; i++) nodes[i] = i;

	std::vector<int> path(nodesCount + 1);
	for (int i = nodesCount - 1; i > 0; i--)
	{
		int r = rand() % i + 1;
		path[i] = nodes[r];
		nodes[r] = nodes[i];
	}

	for (auto& p : path) std::cout << p << " ";
	return path;
}

std::vector<int> Graph::GetBestNeighbor(std::vector<int> currentPath, std::vector<std::vector<int>>& tabuList, int& tabuCount)
{
	int licznik1 = 0, licznik2 = 0;
	int minDistance = INT_MAX, iTabu = 1, jTabu = 2;

	for (int i = 1; i < currentPath.size() - 2; i++)
	{
		for (int j = i + 1; j < currentPath.size() - 1; j++)
		{
			int temp = currentPath[i];
			currentPath[i] = currentPath[j];
			currentPath[j] = temp;

			licznik1++;
			if (currentPath[i] > currentPath[j] ? tabuList[currentPath[j]][currentPath[i]] > 0 : tabuList[currentPath[i]][currentPath[j]] > 0) continue;
			licznik2++;
			

			int distance = GetDistance(currentPath);
			if (distance < minDistance)
			{
				minDistance = distance;
				iTabu = i;
				jTabu = j;
			}

			temp = currentPath[i];
			currentPath[i] = currentPath[j];
			currentPath[j] = temp;
		}
	}

	std::cout << licznik1 << " x " << licznik2 << "\n";

	for (int i = 0; i < tabuList.size(); i++)
	{
		for (int j = 0; j < tabuList.size(); j++)
		{
			if (tabuList[i][j] > 0) tabuList[i][j]--;
		}
	}
	currentPath[iTabu] > currentPath[jTabu] ? tabuList[jTabu][iTabu] += tabuCount : tabuList[iTabu][jTabu] += tabuCount;

	int temp = currentPath[iTabu];
	currentPath[iTabu] = currentPath[jTabu];
	currentPath[jTabu] = temp;
	return currentPath;
}