#pragma once
#include "globalFunctions.h"
#include "miflagaRepList.h"

class Round;

class miflaga
{
public:
	miflaga() = default;
	miflaga(string& _name, citizen* head);
	miflaga(istream& in,Round&_round);
	~miflaga();

	const int getSerialNumCount() const { return serialNumCount; }
	//this function is required to communicate with dynamicArray
	//returns serialNum
	const int getID() const { return serialNum; }
	const string getName() const { return name; }
	const citizen* getHeadOfMiflaga() const { return headOfMiflaga; }
	miflagaRepList& getList()  { return repList; }

	bool setName(string newName) { name = newName; return true; }
	bool setHeadOfMiflaga(const citizen* newHead) { headOfMiflaga = const_cast<citizen*>(newHead); return true; }
	friend ostream& operator<<(ostream& os, miflaga& mif);

	//working with files functions
	void Save(ostream& out) const;
	void Load(istream& in,Round& _round);

	
private:
	static int serialNumCount;
	const int serialNum;
	string name;
	citizen* headOfMiflaga;
	miflagaRepList repList;
};