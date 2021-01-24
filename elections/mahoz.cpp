#include "mahoz.h"

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

int mahoz::serialNumCount = 0;

ostream& operator<<(ostream& os,const mahoz& mahozToPrint)
{
	mahozToPrint.print(os);
	return os;
}

void mahoz::print(ostream& os) const
{
	os << "serial number:" << this->serialNum << " Name:" << this->name << " Number of representatives:" << this->numOfRep;
	cout << " The mahoz is Standard";
}

mahoz::mahoz(string& _name, int _numOfRep)
	: name(_name), numOfRep(_numOfRep), serialNum(serialNumCount + 1), numOfCitizen(0), numOfVoters(0)
{
	serialNumCount++;
	electedCit.setlenght(_numOfRep);
}

mahoz::mahoz(istream& in):serialNum(serialNumCount + 1)
{
	Load(in);
}

void mahoz::initElectionResults(int numOfMiflaga)
{
	electionResults.resize(numOfMiflaga);
	for (auto itr = electionResults.begin(); itr != electionResults.end(); itr++)
	{
		*itr = 0;
	}
}

int mahoz::getWinnerRep(int& miflagaWinnerIndx)
{
	int maxNum, winnerIndx, i;
	for (i = 0, winnerIndx = 0, maxNum = electionResults[0]; i < electionResults.size(); i++)
	{
		if (maxNum < electionResults[i])
		{
			maxNum = electionResults[i];
			winnerIndx = i;
		}
	}
	miflagaWinnerIndx = winnerIndx;
	return numOfRep;
}

int mahoz::getWinnerIndx()
{
	int winnerIndx;
	getWinnerRep(winnerIndx);
	return winnerIndx;
}

void mahoz::Save(ostream& out) const
{
	int nameLen = name.size();
	int electionResultsSize = electionResults.size();
	out.write(rcastcc(&nameLen), sizeof(nameLen));
	out.write(rcastcc(&name[0]), nameLen * sizeof(char));
	out.write(rcastcc(&numOfRep), sizeof(numOfRep));
	out.write(rcastcc(&numOfCitizen), sizeof(numOfCitizen));
	out.write(rcastcc(&numOfVoters), sizeof(numOfVoters));
	out.write(rcastcc(&electionResultsSize), sizeof(int));
	if (electionResultsSize != 0)
		out.write(rcastcc(&electionResults[0]), electionResultsSize * sizeof(int));
}

void mahoz::Load(istream& in)
{
	int nameLen, electionResultsSize;
	in.read(rcastc(&nameLen), sizeof(nameLen));
	if (!in.good())
		throw Load_error("Failed to load mahoz name len");
	
	name.clear();
	name.resize(nameLen + 1);
	in.read(rcastc(&name[0]), nameLen * sizeof(char));
	if (!in.good())
		throw Load_error("Failed to load mahoz name");
	name[nameLen] = '\0';

	in.read(rcastc(&numOfRep), sizeof(numOfRep));
	if (!in.good())
		throw Load_error("Failed to load number of representive in mahoz");
	
	in.read(rcastc(&numOfCitizen), sizeof(numOfRep));
	if (!in.good())
		throw Load_error("Failed to load number of citizens in mahoz");

	in.read(rcastc(&numOfVoters), sizeof(numOfVoters));
	if (!in.good())
		throw Load_error("Failed to load number of voters in mahoz");

	in.read(rcastc(&electionResultsSize), sizeof(electionResultsSize));
	if (!in.good())
		throw Load_error("Failed to load elections results SIZE");

	if (electionResultsSize > 0)
	{
		electionResults.resize(electionResultsSize);
		in.read(rcastc(&electionResults[0]), electionResultsSize * sizeof(int));
		if (!in.good())
			throw Load_error("Failed to load elections results Array");
	}
	serialNumCount++;
	//will load num of rep at load function
	electedCit.setlenght(numOfRep);
}

mahoz::~mahoz()
{
}