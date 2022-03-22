#pragma once
#include <vector>
using namespace std;

class Path
{
public:
	vector<int> path;
	int pathCost;
	float time;

	Path(vector<int> path, int pathCost, float time);
	~Path();
};