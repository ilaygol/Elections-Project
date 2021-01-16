#pragma once
#include "globalFunctions.h"

class mahoz;
enum class MahozType {dividedMahoz,mahoz};

class mahozLoader {
public:
	static void save(mahoz* m, ostream& out);
	static mahoz* load(istream& in);
};
