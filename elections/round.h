#pragma once
#include "globalFunctions.h"
#include "dynamicArray.h"

class dividedMahoz;
class citizen;
class mahoz;
class miflaga;

class Round
{
public:
	Round() = default;
	Round(int _day, int _month, int _year, bool _isSimpleRound);
	~Round();

	const int getDay() const { return day; }
	const int getMonth() const { return month; }
	const int getYear() const { return year; }
	const DynamicArray<citizen>& getAllCitizen() const { return allCitizen; }
	const DynamicArray<miflaga>& getAllMiflaga() const { return allMiflaga; }
	const DynamicArray<mahoz>& getAllMahoz() const { return allMahoz; }
	vector<int> getResults() { return results; }
	const bool getIsSimpleRound() { return isSimpleRound; }


	bool AddMahozToArray(string& name, int numOfRep,bool isDivided);
	bool AddCitizenToArray(string& name, int id, int year, int mahozNum);
	bool AddMiflagaToArray(string& name, int headID);

	void StartNewVotesSchedule();
	bool vote(int mahozSerial, int miflagaSerial);
	void InitResults();
	void setRepList();
	void setResults();
	void PrintVotesSchedule(int mahozIndex);
	void PrintResults();
	void PrintDevidedResults(dividedMahoz* mahozptr);
	int CalculateVotesForHead(int miflagaIndex);


	void Save(ostream& out) const;
	void Load(istream& in);

private:
	bool isSimpleRound;
	int day, month, year;
	DynamicArray<citizen> allCitizen;
	DynamicArray<miflaga> allMiflaga;
	DynamicArray<mahoz> allMahoz;
	vector<int> results;
};
