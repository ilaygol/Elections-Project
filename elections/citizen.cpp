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
	:name(_name)
{
	if (mahozSerial == nullptr)
		throw logic_error("could not add the citizen, MAHOZ DOESNT EXIST");
	if (!checkID(_id))
		throw invalid_argument("could not add the citizen, ID is incorrect!");
	if (_yearOfBirth < 1900 || _yearOfBirth>2050)
		throw invalid_argument("could not add the citizen, YEAR is incorrect!");

	id = _id;
	yearOfBirth = _yearOfBirth;
	mahozptr = const_cast<mahoz*>(mahozSerial);
}

citizen::citizen(istream& in, Round& _round) : mahozptr(nullptr)
{
	Load(in,_round);
}

bool citizen::checkID(int id)
{
	int count = 0;
	while (id > 0)
	{
		count++;
		id /= 10;
	}
	if (count == 9)
		return true;
	return false;
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
		throw Load_error("Failed to load citizen name len");
	
	name.clear();
	name.resize(nameLen + 1);
	in.read(rcastc(&name[0]), nameLen * sizeof(char));
	if (!in.good())
		throw Load_error("Failed to load citizen name");
	name[nameLen] = '\0';

	in.read(rcastc(&id), sizeof(id));
	if (!in.good())
		throw Load_error("Failed to load citizen ID");
	
	in.read(rcastc(&yearOfBirth), sizeof(yearOfBirth));
	if (!in.good())
		throw Load_error("Failed to citizen year of birth");

	in.read(rcastc(&mahozNum), sizeof(mahozNum));
	if (!in.good())
		throw Load_error("Failed to load citizen's mahoz number");
	
	mahozptr = _round.getAllMahoz().getObjectPtr(mahozNum);
}

citizen::~citizen()
{
}
