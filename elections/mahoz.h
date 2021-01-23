#pragma once
#include "globalFunctions.h"
#include "dynamicArray.h"

class citizen;

class mahoz
{
public:
	mahoz() = default;
	mahoz(string& _name, int _numOfRep);
	mahoz(istream& in);
	virtual ~mahoz();

	const int getSerialNumCount() const { return serialNumCount; }
	//this function is required to communicate with dynamicArray
	//returns serialNum
	const int getID() const { return serialNum; }
	const string getName() const { return name; }
	const int getNumOfCitizen() const { return numOfCitizen; }
	const int getNumOfRep() const { return numOfRep; }
	const int getNumOfVoters() const { return numOfVoters; }
	const int getVotesOfMiflaga(int miflagaIndx) { return electionResults[miflagaIndx]; }

	DynamicArray<citizen>& getCanVoteCit() { return canVoteCit; }
	DynamicArray<citizen>& getElectedCit() { return electedCit; }

	bool setNumOfCitizen(int citizenNum) { numOfCitizen = citizenNum; }
	bool setName(const string newName) { name = newName; return true; }
	bool setNumOfRep(int newNumOfRep) { numOfRep = newNumOfRep; return true; }
	bool setVote(int miflagaIndx) { electionResults[miflagaIndx]++; return true; }
	bool addCitizenNum() { numOfCitizen++; return true; }
	bool addVotersNum() { numOfVoters++; return true; }
	friend ostream& operator<<(ostream& os, const mahoz& mahozToPrint);
	void initElectionResults(int numOfMiflaga);
	int getWinnerRep(int& miflagaWinnerIndx);
	int getWinnerIndx();

	//working with files functions
	void Save(ostream& out) const;
	void Load(istream& in);

protected:
	static int serialNumCount;
	const int serialNum;
	string name;
	int numOfRep;
	int numOfCitizen;///to calculate % of voters
	int numOfVoters;
	DynamicArray<citizen> canVoteCit;
	DynamicArray<citizen> electedCit;
	vector<int> electionResults;
};