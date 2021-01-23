#include "round.h"
#include "mahoz.h"
#include "dividedMahoz.h"
#include "miflaga.h"
#include "citizen.h"

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

Round::Round(int _day, int _month, int _year, bool _isSimpleRound) :day(_day), month(_month), year(_year), isSimpleRound(_isSimpleRound)
{
}

Round::~Round()
{
	int numOfMahoz;
	numOfMahoz = allMahoz.getlenght();
}

bool Round::AddMahozToArray(string& name, int numOfRep, bool isDivided)
{
	if (isDivided)
		allMahoz.addptr(new dividedMahoz(name, numOfRep));
	else
		allMahoz.addptr(new mahoz(name, numOfRep));
	return true;
}

bool Round::AddCitizenToArray(string& name, int id, int year, int mahozNum)
{
	mahoz* m = allMahoz.getObjectPtr(mahozNum);
	if (m == nullptr)
		return false;

	allCitizen.addptr(new citizen(name, id, year, m));
	return true;
}

bool Round::AddMiflagaToArray(string& name, int headID)
{
	citizen* head = allCitizen.getObjectPtr(headID);
	if (head == nullptr)
		return false;
	allMiflaga.addptr(new miflaga(name, head));
	return true;
}

void Round::StartNewVotesSchedule()
{
	int numOfMahoz, numOfMiflaga;
	numOfMahoz = allMahoz.getlenght();
	numOfMiflaga = allMiflaga.getlenght();

	for (int i = 0; i < numOfMahoz; i++)
	{
		allMahoz.getptr(i)->initElectionResults(numOfMiflaga);
	}
}

bool Round::vote(int mahozSerial, int miflagaSerial)
{
	return allMahoz.getObjectPtr(mahozSerial)->setVote(miflagaSerial - 1);
}

void Round::InitResults()
{
	int numOfMiflaga = allMiflaga.getlenght();
	results.resize(numOfMiflaga);

	for (auto itr = results.begin(); itr != results.end(); itr++)
	{
		*itr = 0;
	}
}

///a function that divide the representive of each party  into mahoz
void Round::setRepList()
{
	float tempNumOfRep;
	for (int i = 0; i < allMahoz.getlenght(); i++)
	{
		for (int j = 0; j < allMiflaga.getlenght(); j++)
		{
			mahoz* mahozptr = allMahoz.getObjectPtr(i + 1);
			tempNumOfRep = ((static_cast<float>(mahozptr->getVotesOfMiflaga(j)) / (static_cast<float>(mahozptr->getNumOfVoters()))) * static_cast<float>(mahozptr->getNumOfRep()));
			tempNumOfRep = round(tempNumOfRep);
			int tempInt = static_cast<int>(tempNumOfRep);
			for (int t = 0; t < tempInt; t++)
			{
				mahozptr->getElectedCit().addptrNoResize(allMiflaga.getObjectPtr(j + 1)->getList().getRepOfMahoz(i + 1).getptr(t));
			}
		}
	}
}

///calculates the num of rep for each head of party
void Round::setResults()
{
	mahoz* mahozptr;
	int i, winnerIndx, numOfRep, * tempArr;
	int numOfMiflaga = allMiflaga.getlenght();
	
	for (i = 0; i < allMahoz.getlenght(); i++)
	{
		mahozptr = allMahoz.getptr(i);
		
		if (typeid(*mahozptr) == typeid(mahoz))
		{
			numOfRep = mahozptr->getWinnerRep(winnerIndx);
			results[winnerIndx] += numOfRep;
		}
		else
		{
			tempArr = static_cast<dividedMahoz*>(mahozptr)->getDividedRep();
			for (int j = 0; j < numOfMiflaga; j++)
			{
				results[j] += tempArr[j];
			}
		}
	}
}

void Round::PrintVotesSchedule(int mahozIndex)
{
	float percent;
	int numOfMiflaga = allMiflaga.getlenght();
	mahoz* mahozptr = allMahoz.getptr(mahozIndex);

	for (int i = 0; i < allMiflaga.getlenght(); i++)
	{
		percent = static_cast<float>(mahozptr->getVotesOfMiflaga(i)) / static_cast<float>(mahozptr->getNumOfVoters()) * 100;
		cout << allMiflaga.getObjectPtr(i + 1)->getName() << ": " << mahozptr->getVotesOfMiflaga(i) << " votes, " << percent << "%" << endl;
	}
}

//calculates the sum number of votes the miflaga has got from all mahoz.
int Round::CalculateVotesForHead(int miflagaIndex)
{
	int sum = 0;
	for (int i = 0; i < allMahoz.getlenght(); i++)
	{
		sum += allMahoz.getptr(i)->getVotesOfMiflaga(miflagaIndex);
	}
	return sum;
}

void Round::PrintResults()
{
	int miflagaNum = allMiflaga.getlenght();
	vector<int> temp = results;
	int max, index = 0, count = 0, i;
	miflaga* tempMiflaga;
	DynamicArray<citizen> tempCitArr;
	while (count < miflagaNum)
	{
		for (i = 0, max = temp[0], index = 0; i < miflagaNum; i++)
		{
			if (temp[i] > max)
			{
				max = temp[i];
				index = i;
			}
		}
		tempMiflaga = allMiflaga.getObjectPtr(index + 1);
		cout << "miflaga serial: " << tempMiflaga->getID() << " miflaga name: " << tempMiflaga->getName() << endl;
		cout << "head of miflaga: " << tempMiflaga->getHeadOfMiflaga()->getName();
		cout << " all votes got: " << CalculateVotesForHead(index) << " all representives got: " << temp[index] << endl;

		if (isSimpleRound)
		{
			tempCitArr = tempMiflaga->getList().getRepOfMahoz(1);
			for (int i = 0; i < temp[index]; i++)
			{
				cout << *(tempCitArr.getptr(i)) << endl;
			}
		}
		temp[index] = -1;
		count++;
	}
}

void Round::PrintDevidedResults(dividedMahoz* mahozptr)
{
	int numOfMiflaga = allMiflaga.getlenght();
	int* tempArr = mahozptr->getDividedRep();
	miflaga* miflagaptr;
	int max, index = 0, count = 0, i;
	while (count < numOfMiflaga)
	{
		for (i = 0, max = tempArr[0], index = 0; i < numOfMiflaga; i++)
		{
			if (tempArr[i] > max)
			{
				max = tempArr[i];
				index = i;
			}
		}
		if (max > 0)
		{
			miflagaptr = allMiflaga.getObjectPtr(index + 1);
			cout << miflagaptr->getHeadOfMiflaga()->getName() << " got " << max << " representives." << endl;
		}

		tempArr[index] = -1;
		count++;
	}
	delete[] tempArr;
}

void Round::Save(ostream& out) const
{
	out.write(rcastcc(&isSimpleRound), sizeof(isSimpleRound));
	out.write(rcastcc(&day), sizeof(day));
	out.write(rcastcc(&month), sizeof(month));
	out.write(rcastcc(&year), sizeof(year));

	allMahoz.SaveMahoz(out);
	allCitizen.Save(out);
	allMiflaga.Save(out);
	for (int i = 0; i < allMahoz.getlenght(); i++)
	{
		allMahoz.getObjectPtr(i + 1)->getCanVoteCit().Save(out);
	}
}

void Round::Load(istream& in)
{
	in.read(rcastc(&isSimpleRound), sizeof(isSimpleRound));
	if (!in.good())
		throw Load_error("Failed to load election type");

	in.read(rcastc(&day), sizeof(day));
	if (!in.good())
		throw Load_error("Failed to load election date (day)");

	in.read(rcastc(&month), sizeof(month));
	if (!in.good())
		throw Load_error("Failed to load election date (month)");
	
	in.read(rcastc(&year), sizeof(year));
	if (!in.good())
		throw Load_error("Failed to load election date (year)");

	allMahoz.LoadMahoz(in);
	allCitizen.Load(in, *this);
	allMiflaga.Load(in, *this);
	for (int i = 0; i < allMahoz.getlenght(); i++)
	{
		allMahoz.getObjectPtr(i + 1)->getCanVoteCit().Load(in, *this);
	}
}