#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "DataMatrix.h"
#include <vector>
#include "Path.h"
using namespace std;

class Method
{
public:
	Method();
	virtual ~Method();

	string name;
	//virtual Path runMethod(DataMatrix data, vector<int> nodes) = 0;

};
