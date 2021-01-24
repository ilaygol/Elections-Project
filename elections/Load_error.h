#pragma once
#include <exception>
#include <iostream>
using namespace std;

class Load_error :public exception
{
public:
	Load_error(string _msg) :temp(_msg), msg("Load error: ")
	{
		msg += temp;
	}

	~Load_error()
	{}
	
	const char* what() const throw()
	{
		return msg.c_str();
	}

private:
	string msg;
	string temp;
};