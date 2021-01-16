#include "globalFunctions.h"

char* mystrdup(const char* name)
{
	int strSize = strlen(name);
	char* NewName = new char[strSize + 1];
	for (int i = 0; i < strSize; i++)
	{
		NewName[i] = name[i];
	}
	NewName[strSize] = '\0';
	return NewName;
}

char* exactLenName(char* name)
{
	int len = strlen(name);
	char* updatedName = new char[len + 1];
	for (int i = 0; i < len; i++)
		updatedName[i] = name[i];
	updatedName[len] = '\0';
	delete[]name;
	return updatedName;
}