#include "Individual.h"

Individual::Individual()
{
}

Individual::Individual(std::vector<int> path, int distance)
{
	this->path = path;
	this->distance = distance;
}

Individual::~Individual()
{
}

void Individual::CrossOver(Individual& ind)
{
	int point;
	do {
		point = rand() % path.size();
	} while (point < 2);

	for (int i = 0; i < point; i++)
	{
		int temp = path[i];
		path[i] = ind.path[i];
		ind.path[i] = temp;
	}
}

std::vector<int>& Individual::GetPath()
{
	return path;
}

int Individual::GetDistance()
{
	return distance;
}

void Individual::SetDistance(int distance)
{
	this->distance = distance;
}

bool Individual::operator<(const Individual &ind)
{
	return this->distance < ind.distance;
}

bool Individual::operator==(const Individual &ind)
{
	return this->path == ind.path;
}