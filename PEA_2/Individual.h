#pragma once
#include <vector>

class Individual
{
private:
	std::vector<int> path;
	int distance;

public:
	Individual();
	Individual(std::vector<int>, int);
	~Individual();
	void CrossOver(Individual&);
	std::vector<int>& GetPath();
	int GetDistance();
	void SetDistance(int distance);

	bool operator<(const Individual&);
	bool operator==(const Individual&);
};

