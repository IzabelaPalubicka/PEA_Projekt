#pragma once

#include <vector>
using namespace std;
class Result
{
public:
	vector<int> path;
	int pathCost;

	Result();
	Result(vector<int> path, int pathCost);
	~Result();
};

