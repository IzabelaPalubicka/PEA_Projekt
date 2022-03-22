#include "Result.h"

Result::Result()
{
}

Result::Result(vector<int> path, int pathCost)
{
	this->path = path;
	this->pathCost = pathCost;
}

Result::~Result()
{
} 