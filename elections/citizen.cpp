#include "citizen.h"
#include "mahoz.h"
#include "round.h"
#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

ostream& operator<<(ostream& os, const citizen& cit)
{
	os << "Name: " << cit.name << " ID: " << cit.id << " YearOfBirth: " << cit.yearOfBirth << " Mahoz: " << cit.mahozptr->getID();
	return os;
}

citizen::citizen(string& _name, int _id, int _yearOfBirth, const mahoz* mahozSerial)
	:id(_id), name(_name), yearOfBirth(_yearOfBirth), mahozptr(const_cast<mahoz*>(mahozSerial))
{
}

citizen::citizen(istream& in, Round& _round) : mahozptr(nullptr)
{
	Load(in,_round);
}


void citizen::Save(ostream& out) const
{
	int nameLen = name.size();
	int mahozNum = mahozptr->getID();
	out.write(rcastcc(&nameLen),sizeof(nameLen));
	out.write(rcastcc(&name[0]), nameLen*sizeof(char));
	out.write(rcastcc(&id), sizeof(id));
	out.write(rcastcc(&yearOfBirth), sizeof(yearOfBirth));
	out.write(rcastcc(&mahozNum), sizeof(mahozNum));
}

void citizen::Load(istream& in, Round& _round)
{
	int nameLen, mahozNum;
	in.read(rcastc(&nameLen), sizeof(nameLen));
	if (!in.good())
	{
		cout << "Failed to load citizen name len" << endl;
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
	in.read(rcastc(&id), sizeof(id));
	if (!in.good())
	{
		cout << "Failed to load citizen ID" << endl;
		exit(-1);
	}
	in.read(rcastc(&yearOfBirth), sizeof(yearOfBirth));
	if (!in.good())
	{
		cout << "Failed to citizen year of birth" << endl;
		exit(-1);
	}
	in.read(rcastc(&mahozNum), sizeof(mahozNum));
	if (!in.good())
	{
		cout << "Failed to load citizen's mahoz number" << endl;
		exit(-1);
	}
	mahozptr = _round.getAllMahoz().getObjectPtr(mahozNum);
}

citizen::~citizen()
{
}
