#include "miflaga.h"
#include "citizen.h"
#include "round.h"
#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

int miflaga::serialNumCount = 0;

ostream& operator<<(ostream& os,miflaga& mif)
{
	os << "divition serial number:" << mif.serialNum << " name:" << mif.name << endl;
	cout << "the head of the divition is" << endl;
	cout << *(mif.getHeadOfMiflaga()) << endl;
	cout << "the representives are:" << endl;
	mif.repList.printList();
	return os;
}

}
miflaga::miflaga(string& _name, citizen* head)
	:name(_name), serialNum(serialNumCount + 1), headOfMiflaga(head)
{
	++serialNumCount;
}

miflaga::miflaga(istream& in, Round& _round) : serialNum(serialNumCount + 1),headOfMiflaga(nullptr)
{
	Load(in, _round);
}

void miflaga::Save(ostream& out) const
{
	int nameLen, HeadID;
	nameLen = name.size();
	HeadID = headOfMiflaga->getID();
	out.write(rcastcc(&nameLen), sizeof(nameLen));
	out.write(rcastcc(&name[0]), nameLen * sizeof(char));
	out.write(rcastcc(&HeadID), sizeof(HeadID));
	//saving representives list
	repList.Save(out);
}

}
void miflaga::Load(istream& in, Round& _round)
{
	int nameLen, HeadID;
	in.read(rcastc(&nameLen), sizeof(nameLen));
	if (!in.good())
	{
		cout << "Failed to load mifalga name len" << endl;
		exit(-1);
	}
	name.clear();
	name.resize(nameLen + 1);
	in.read(rcastc(&name[0]), nameLen * sizeof(char));
	if (!in.good())
	{
		cout << "Failed to load citizen name" << endl;
		exit(-1);
	}
	name[nameLen] = '\0';
	in.read(rcastc(&HeadID), sizeof(HeadID));
	if (!in.good())
	{
		cout << "Failed to load miflaga head ID" << endl;
		exit(-1);
	}
	headOfMiflaga = _round.getAllCitizen().getObjectPtr(HeadID);
	serialNumCount++;
	//loading represetives list
	repList.Load(in, _round);
}

miflaga::~miflaga()
{
}
