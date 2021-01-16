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
	const char* tmpName = new char[nameLen];
	tmpName = name.c_str();
	int mahozNum = mahozptr->getID();
	out.write(rcastcc(&nameLen),sizeof(nameLen));
	out.write(rcastcc(tmpName), nameLen*sizeof(char));
	out.write(rcastcc(&id), sizeof(id));
	out.write(rcastcc(&yearOfBirth), sizeof(yearOfBirth));
	out.write(rcastcc(&mahozNum), sizeof(mahozNum));

	delete[] tmpName;
}

void citizen::Load(istream& in, Round& _round)
{
	int nameLen, mahozNum;
	char* tmpName;
	in.read(rcastc(&nameLen), sizeof(nameLen));
	if (!in.good())
	{
		cout << "Failed to load citizen name len" << endl;
		exit(-1);
	}
	tmpName = new char[nameLen + 1];
	in.read(rcastc(tmpName), nameLen * sizeof(char));
	if (!in.good())
	{
		cout << "Failed to load citizen name" << endl;
		exit(-1);
	}
	tmpName[nameLen] = '\0';
	name = tmpName;
	delete[] tmpName;
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
