#pragma once
#include "globalFunctions.h"
class mahoz;
class Round;

class citizen
{
public:
	citizen() = default;
	citizen(string& _name, int _id, int _yearOfBirth,const mahoz* mahozSerial);
	citizen(istream& in, Round& _round);
	citizen(const citizen& other) = delete;
	~citizen();

	const string getName() const { return name; }
	const int getID() const { return id; }
	const int getYear() const { return yearOfBirth; }
	const mahoz* getMahoz() const { return mahozptr; }

	bool setName(const string newName) { name = newName; return true; }
	bool setYear(int newYear) { yearOfBirth = newYear; return true; }
	bool setMahoz(const mahoz* newMahoz) { mahozptr = const_cast<mahoz*>(newMahoz); return true; }
	friend ostream& operator<<(ostream& os, const citizen& cit);


	void Save(ostream& out) const;
	void Load(istream& in, Round& _round);


private:
	string name;
	int id;
	int yearOfBirth;
	mahoz* mahozptr;
};