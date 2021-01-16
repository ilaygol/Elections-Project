#include "dividedMahoz.h"

dividedMahoz::dividedMahoz(string& _name, int _numOfRep) : mahoz(_name, _numOfRep)
{
}

dividedMahoz::dividedMahoz(istream& in) :mahoz(in)
{
}

dividedMahoz::~dividedMahoz()
{
}

int* dividedMahoz::getDividedRep()
{
	float temp;
	int* resArr = new int[electionResults.size()]; ///we save the num of rep for each party
	for (int i = 0; i < electionResults.size(); i++)
	{
		temp = ((static_cast<float>(electionResults[i]) / (static_cast<float>(numOfVoters))) * static_cast<float>(numOfRep));
		resArr[i] = static_cast<int>(round(temp));
	}
	return resArr;
}
