#pragma once
#include "globalFunctions.h"
#include "dynamicArray.h"

class citizen;
class Round;

class miflagaRepList
{
private:
	struct repNode {
		int mahozID;
		int numOfRep;
		DynamicArray<citizen> rep;
		repNode* next;
		repNode* prev;
	};
	repNode* head;
	repNode* tail;

public:
	miflagaRepList();
	~miflagaRepList();

	bool checkIfNodeExist(int numOfMahoz);
	bool addRepToMahoz(citizen* rep,int numMahoz);
	void addNode(int numMahoz, int numOfRep);
	void addToHead(repNode* node);
	void addToTail(repNode* node);
	int countListLengh()const;

	//saving to File/loading from file functions
	void Save(ostream& out) const;
	void Load(istream& in, Round& _round);
	
	//getter
	DynamicArray<citizen>& getRepOfMahoz(int mahozSerial);
	//printing function
	void printList() const;
};