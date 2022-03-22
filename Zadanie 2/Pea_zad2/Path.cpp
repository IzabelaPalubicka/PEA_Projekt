#include "Path.h"

Path::Path(vector<int> path, int pathCost, float time)
{
	this->path = path;
	this->pathCost = pathCost;
	this->time = time;
}

Path::~Path()
{
}
