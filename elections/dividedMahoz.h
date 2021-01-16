#pragma once
#include "globalFunctions.h"
#include "mahoz.h"

class dividedMahoz :public mahoz
{
public:

	dividedMahoz() = default;
	dividedMahoz(string& _name, int _numOfRep);
	dividedMahoz(istream& in);
	virtual ~dividedMahoz();


	//this function calculate number of rep for each party and return them
	int* getDividedRep();

	

};