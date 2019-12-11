#include "Graph.h"

double Graph::stopCondition = 120.0;
double Graph::temperatureRatio = 0.999999;

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

int Graph::GetNodesCount()
{
	return nodesCount;
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

void Graph::TabuSearch()
{
	double stopConditionMs = stopCondition * 1000000.0;
	int tabuCount = nodesCount;

	std::vector<int> currentPath = InitSolution();
	int currentDistance = GetDistance(currentPath);

	std::vector<int> minPath = currentPath;
	int minDistance = currentDistance;

	std::vector<std::vector<int>> tabuList(nodesCount - 1);
	for (auto& t : tabuList) t = std::vector<int>(nodesCount - 1, 0);

	Clock c; c.startCounter();
	double bestTime = 0.0;
	while(c.getCounter() < stopConditionMs)
	{
		int minDistanceNeighbor = INT_MAX, iTabu, jTabu;
		bool diversification = true;

		for (int i = 1; i < currentPath.size() - 2; i++)
		{
			for (int j = i + 1; j < currentPath.size() - 1; j++)
			{
				int distance = currentDistance;
				distance -= distances[currentPath[i - 1]][currentPath[i]] + distances[currentPath[j]][currentPath[j + 1]];
				if (j == i + 1) distance -= distances[currentPath[i]][currentPath[j]];
				else distance -= distances[currentPath[i]][currentPath[i + 1]] + distances[currentPath[j - 1]][currentPath[j]];
					
				int temp = currentPath[i];
				currentPath[i] = currentPath[j];
				currentPath[j] = temp;

				distance += distances[currentPath[i - 1]][currentPath[i]] + distances[currentPath[j]][currentPath[j + 1]];
				if (j == i + 1) distance += distances[currentPath[i]][currentPath[j]];
				else distance += distances[currentPath[i]][currentPath[i + 1]] + distances[currentPath[j - 1]][currentPath[j]];

				temp = currentPath[i];
				currentPath[i] = currentPath[j];
				currentPath[j] = temp;

				if ((currentPath[i] > currentPath[j] ? tabuList[currentPath[j] - 1][currentPath[i] - 1] <= 0 :
					tabuList[currentPath[i] - 1][currentPath[j] - 1] <= 0) || distance < minDistance)
				{
					if (distance < minDistanceNeighbor)
					{
						diversification = false;

						minDistanceNeighbor = distance;
						iTabu = i;
						jTabu = j;

						if (minDistanceNeighbor < minDistance) bestTime = c.getCounter();
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

		if(diversification)
		{
			currentPath = InitSolution();
			currentDistance = GetDistance(currentPath);

			if (currentDistance < minDistance)
			{
				minPath = currentPath;
				minDistance = currentDistance;

				bestTime = c.getCounter();
			}
		}
		else
		{
			currentPath[iTabu] > currentPath[jTabu] ? tabuList[currentPath[jTabu] - 1][currentPath[iTabu] - 1] += tabuCount :
				tabuList[currentPath[iTabu] - 1][currentPath[jTabu] - 1] += tabuCount;

			int temp = currentPath[iTabu];
			currentPath[iTabu] = currentPath[jTabu];
			currentPath[jTabu] = temp;

			currentDistance = minDistanceNeighbor;
			if (currentDistance < minDistance)
			{
				minPath = currentPath;
				minDistance = currentDistance;
			}
		}
	}

	std::cout << "Najkrotszy dystans(koszt): " << minDistance << "\n";
	std::cout << "Sciezka: ";
	for (int i = 0; i < minPath.size() - 1; i++) std::cout << minPath[i] << " --> ";
	std::cout << minPath[minPath.size() - 1] << "\n";
	std::cout << "Czas znalezienia rozwiazania: " << bestTime / 1000000.0 << "s\n";
}

void Graph::SimulatedAnnealing()
{
	double stopConditionMs = stopCondition * 1000000.0;

	std::vector<int> currentPath = GetNearestNeighbor();
	int currentDistance = GetDistance(currentPath);

	std::vector<int> minPath = currentPath;
	int minDistance = currentDistance;

	double temperature = 1000.0 * nodesCount;

	Clock c;
	c.startCounter();
	double bestTime = 0.0;

	srand(time(NULL));
	while (c.getCounter() < stopConditionMs)
	{
		int i = rand() % (currentPath.size() - 2) + 1;
		int j = rand() % (currentPath.size() - 2) + 1;
		while(i == j) j = rand() % (currentPath.size() - 2) + 1;

		if (i > j)
		{
			int tempI = i;
			i = j;
			j = tempI;
		}

		int distance = currentDistance;
		distance -= distances[currentPath[i - 1]][currentPath[i]] + distances[currentPath[j]][currentPath[j + 1]];
		if (j == i + 1) distance -= distances[currentPath[i]][currentPath[j]];
		else distance -= distances[currentPath[i]][currentPath[i + 1]] + distances[currentPath[j - 1]][currentPath[j]];

		int temp = currentPath[i];
		currentPath[i] = currentPath[j];
		currentPath[j] = temp;

		distance += distances[currentPath[i - 1]][currentPath[i]] + distances[currentPath[j]][currentPath[j + 1]];
		if (j == i + 1) distance += distances[currentPath[i]][currentPath[j]];
		else distance += distances[currentPath[i]][currentPath[i + 1]] + distances[currentPath[j - 1]][currentPath[j]];

		//std::cout << (exp((currentDistance - distance) / temperature)) << "\n";
		if (distance < currentDistance || (exp((currentDistance - distance) / temperature) > (rand() / (double)RAND_MAX)))
		{
			currentDistance = distance;

			if (currentDistance < minDistance)
			{
				minPath = currentPath;
				minDistance = currentDistance;

				bestTime = c.getCounter();
			}
		}
		else
		{
			temp = currentPath[i];
			currentPath[i] = currentPath[j];
			currentPath[j] = temp;
		}
		
		temperature *= temperatureRatio;
	}

	std::cout << "Najkrotszy dystans(koszt): " << minDistance << "\n";
	std::cout << "Sciezka: ";
	for (int i = 0; i < minPath.size() - 1; i++) std::cout << minPath[i] << " --> ";
	std::cout << minPath[minPath.size() - 1] << "\n";
	std::cout << "Czas znalezienia rozwiazania: " << bestTime / 1000000.0 << "s\n";
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

std::vector<int> Graph::InitSolution()
{
	srand(time(NULL));
	std::vector<int> path(nodesCount + 1, 0);

	bool *visited = new bool[nodesCount];
	for (int i = 0; i < nodesCount; i++) visited[i] = false;
	visited[0] = true;

	//LOSOWANIE countToRandom WIERZCHOLKOW i ustawienie ich na pierwszych miejscach rozwiazania
	int countToRandom = (rand() % nodesCount) / 2;
	std::vector<int> nodes(nodesCount - 1);
	for (int i = 0; i < nodes.size(); i++) nodes[i] = i + 1;

	for (int i = 0; i < countToRandom; i++)
	{
		int r = rand() % (nodesCount - 1 - i);
		path[i + 1] = nodes[r];
		nodes[r] = nodes[nodesCount - 2 - i];

		visited[path[i + 1]] = true;
	}

	//GREEDY dla reszty
	for (int i = countToRandom; i < nodesCount - 1; i++)
	{
		int minDinstance = INT_MAX;
		for (int j = 0; j < nodesCount; j++)
		{
			if (path[i] == j || visited[j]) continue;

			if (distances[path[i]][j] < minDinstance)
			{
				minDinstance = distances[path[i]][j];
				path[i + 1] = j;
			}
		}
		visited[path[i + 1]] = true;
	}

	/*std::cout << "CTR = " << countToRandom << "\n";
	for (auto p : path) std::cout << p << " ";
	std::cout << "\n";*/
	return path;
}

std::vector<int> Graph::GetNearestNeighbor()
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

			if (distances[path[i]][j] < minDinstance)
			{
				minDinstance = distances[path[i]][j];
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

	//for (auto& p : path) std::cout << p << " ";
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

std::vector<int> Graph::GetCloseNeighbor(std::vector<int> path)
{
	int v1 = rand() % (path.size() - 2) + 1;
	int v2 = v1;
	while (v2 == v1) v2 = rand() % (path.size() - 2) + 1;

	int temp = path[v1];
	path[v1] = path[v2];
	path[v2] = temp;
	return path;
}