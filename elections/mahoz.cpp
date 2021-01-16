#include "mahoz.h"
#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

int mahoz::serialNumCount = 0;

ostream& operator<<(ostream& os,const mahoz& mahozToPrint)
{
	os << "serial number:" << mahozToPrint.serialNum << " Name:" << mahozToPrint.name << " Number of representatives:" << mahozToPrint.numOfRep;
	if (typeid(mahozToPrint) == typeid(mahoz))
		cout << " The mahoz is Standard";
	else
		cout << " The mahoz is Divided";
	return os;
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
	const char* tmpName = new char[nameLen];
	tmpName = name.c_str();
	int electionResultsSize = electionResults.size();
	out.write(rcastcc(&nameLen), sizeof(nameLen));
	out.write(rcastcc(tmpName), nameLen * sizeof(char));
	out.write(rcastcc(&numOfRep), sizeof(numOfRep));
	out.write(rcastcc(&numOfCitizen), sizeof(numOfCitizen));
	out.write(rcastcc(&numOfVoters), sizeof(numOfVoters));
	out.write(rcastcc(&electionResultsSize), sizeof(int));
	out.write(rcastcc(&electionResults[0]), electionResultsSize * sizeof(int));

	delete[] tmpName;
}
void mahoz::Load(istream& in)
{
	int nameLen, electionResultsSize;
	char* tmpName;
	in.read(rcastc(&nameLen), sizeof(nameLen));
	if (!in.good())
	{
		cout << "Failed to load mahoz name len" << endl;
		exit(-1);
	}
	tmpName = new char[nameLen + 1];
	in.read(rcastc(tmpName), nameLen * sizeof(char));
	if (!in.good())
	{
		cout << "Failed to load mahoz name" << endl;
		exit(-1);
	}
	tmpName[nameLen] = '\0';
	name = tmpName;
	delete[] tmpName;
	name[nameLen] = '\0';
	in.read(rcastc(&numOfRep), sizeof(numOfRep));
	if (!in.good())
	{
		cout << "Failed to load number of representive in mahoz" << endl;
		exit(-1);
	}
	in.read(rcastc(&numOfCitizen), sizeof(numOfRep));
	if (!in.good())
	{
		cout << "Failed to load number of citizens in mahoz" << endl;
		exit(-1);
	}
	in.read(rcastc(&numOfVoters), sizeof(numOfVoters));
	if (!in.good())
	{
		cout << "Failed to load number of voters in mahoz" << endl;
		exit(-1);
	}
	in.read(rcastc(&electionResultsSize), sizeof(electionResultsSize));
	if (!in.good())
	{
		cout << "Failed to load elections results SIZE" << endl;
		exit(-1);
	}

	if (electionResultsSize > 0)
	{
		electionResults.resize(electionResultsSize);
		in.read(rcastc(&electionResults[0]), electionResultsSize * sizeof(int));
		if (!in.good())
		{
			cout << "Failed to load elections results Array" << endl;
			exit(-1);
		}
	}
	serialNumCount++;
	//will load num of rep at load function
	electedCit.setlenght(numOfRep);
}

mahoz::~mahoz()
{
}