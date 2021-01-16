#pragma once
#define CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <typeinfo>
#include <math.h>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;
///our own strdup
char* mystrdup(const char* name);
//a function that resize the the string;
char* exactLenName(char* name);