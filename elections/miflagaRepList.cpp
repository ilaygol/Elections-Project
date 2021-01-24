#include "miflagaRepList.h"
#include "round.h"
#include "citizen.h"

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

miflagaRepList::miflagaRepList() :head(nullptr),tail(nullptr)
{} ///start an empty list;

miflagaRepList::~miflagaRepList()
{
	//free all the items in the list
	repNode* temp1 = head, * temp2;
	while (temp1 != nullptr) {
		temp2 = temp1->next;
		delete temp1;
		temp1 = temp2;
	}
}

bool miflagaRepList::addRepToMahoz(citizen* rep, int numMahoz) 
{
	addNode(numMahoz, 0);

	repNode* searchNode = head;
	while (searchNode != nullptr)
	{
		if (searchNode->mahozID == numMahoz)
		{
			searchNode->rep.addptr(rep);
			searchNode->numOfRep = (searchNode->numOfRep) + 1;
			return true;
		}
		searchNode = searchNode->next;
	}
	return false;
}

void miflagaRepList::addNode(int numMahoz, int numOfRep)
{
	if (!checkIfNodeExist(numMahoz))
	   {
		repNode* rnode = new repNode;
		rnode->mahozID = numMahoz;
		rnode->numOfRep = numOfRep;
		rnode->rep.setlenght(numOfRep);
		rnode->next = nullptr;
		rnode->prev = nullptr;

		if (head == nullptr)
			head=tail=rnode;
		else if (head->mahozID > rnode->mahozID)
			addToHead(rnode);
		else if (tail->mahozID < rnode->mahozID)
			addToTail(rnode);


		else ////the new node need to be at the middle;
		{
			repNode* temp = head;
			while (temp->next->mahozID < rnode->mahozID)///finding the right index
				temp = temp->next;
			rnode->prev = temp;
			rnode->next = temp->next;
			temp->next->prev = rnode;
			temp->next = rnode;
		}
	}
}

bool miflagaRepList::checkIfNodeExist(int numOfMahoz)
{
	bool res = false;
	repNode* scanNode = head;
	while (scanNode != nullptr && !res)
	{
		if (scanNode->mahozID == numOfMahoz)
			res = true;
		scanNode = scanNode->next;
	}
	return res;
}


void miflagaRepList::addToHead(repNode* node)
{
	head->prev = node;
	head = node;
	node->prev = nullptr;
}
void miflagaRepList::addToTail(repNode* node)
{
	tail->next = node;
	tail = node;
	node->next = nullptr;
}

DynamicArray<citizen>& miflagaRepList::getRepOfMahoz(int mahozSerial)
{
	repNode* search = head;
	bool found = false;
	while (search != nullptr &&!found)
	{
		if (search->mahozID == mahozSerial)
			found = true;
		else
			search = search->next;
	}
	return search->rep;
}

int miflagaRepList::countListLengh() const
{
	repNode* countNode = head;
	int count = 0;
	while (countNode != nullptr)
	{
		countNode = countNode->next;
		count++;
	}
	return count;
}

bool miflagaRepList::IsExactNumOfRep(int mahozSerial, int numOfRep)
{
	repNode* search = head;
	bool found = false;
	while (search != nullptr && !found)
	{
		if (search->mahozID == mahozSerial)
		{
			found = true;
			if (search->numOfRep == numOfRep)
				return true;
		}
		else
			search = search->next;
	}
	return false;
}

void miflagaRepList::Save(ostream& out) const
{
	repNode* savingNode = head;
	int listLenght = countListLengh();
	out.write(rcastcc(&listLenght), sizeof(listLenght));
	while (savingNode != nullptr)
	{
		int numOfRepForMahoz = savingNode->rep.getlenght();
		int mahozNum = savingNode->mahozID;
		out.write(rcastcc(&mahozNum), sizeof(mahozNum));
		out.write(rcastcc(&numOfRepForMahoz), sizeof(numOfRepForMahoz));
		for (int i = 0; i < numOfRepForMahoz; i++)
		{
			citizen* citizenPtr = savingNode->rep.getptr(i);
			int citizenID = citizenPtr->getID();
			out.write(rcastcc(&citizenID), sizeof(citizenID));
		}
		savingNode = savingNode->next;
	}	
}

void miflagaRepList::Load(istream& in, Round& _round)
{
	int listLenght;
	in.read(rcastc(&listLenght), sizeof(listLenght));
	if (!in.good())
		throw Load_error("Failed to load mifalga Representives List");

	for (int i = 0; i < listLenght; i++)
	{
		int mahozNum, numOfRep;
		in.read(rcastc(&mahozNum), sizeof(mahozNum));
		if (!in.good())
			throw Load_error("Failed to load mahoz number for miflaga representives list");
			
		in.read(rcastc(&numOfRep), sizeof(numOfRep));
		if (!in.good())
			throw Load_error("Failed to load numbers of representives for miflaga in mahoz");

		for (int j = 0; j < numOfRep; j++)
		{
			int citizenID;
			citizen* citizenPtr;
			in.read(rcastc(&citizenID), sizeof(citizenID));
			if (!in.good())
				throw Load_error("Failed to load representive ID");
				
			citizenPtr= _round.getAllCitizen().getObjectPtr(citizenID);
			addRepToMahoz(citizenPtr, mahozNum);
		}
	}
}

void miflagaRepList::printList() const
{
	repNode* printNode = head;
	while (printNode != nullptr)
	{
		cout << "Representatives for mahoz with serial number " << printNode->mahozID << " are:" << endl;
		printNode->rep.printAll();
		printNode = printNode->next;
	}
}