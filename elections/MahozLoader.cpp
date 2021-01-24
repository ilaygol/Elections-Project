#include "MahozLoader.h"
#include "mahoz.h"
#include "dividedMahoz.h"
#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

void mahozLoader::save(mahoz* m, ostream& out)
{
	if (typeid(*m) == typeid(mahoz))
	{
		MahozType type = MahozType::mahoz;
		out.write(rcastcc(&type), sizeof(type));
	}
	else
	{
		MahozType type = MahozType::dividedMahoz;
		out.write(rcastcc(&type), sizeof(type));
	}
	m->Save(out);

}
mahoz* mahozLoader::load(istream& in) 
{
	MahozType type;
	in.read(rcastc(&type), sizeof(type));
	if (!in.good())
		throw Load_error("Failed to load mahoz type");
	switch (type)
	{
	case MahozType::mahoz:
		return new mahoz(in);
		break;
	case MahozType::dividedMahoz:
		return new dividedMahoz(in);
		break;
	}
}
