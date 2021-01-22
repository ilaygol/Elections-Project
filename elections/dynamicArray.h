#pragma once
#include "globalFunctions.h"
#include "MahozLoader.h"

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

class Round;

template <class T>
class DynamicArray {
public:
	explicit DynamicArray(int lenght = 0) :arr(new T* [lenght]), phsize(lenght), lsize(0)
	{}

	~DynamicArray()
	{
		for (int i = 0; i < lsize; ++i)
			delete arr[i];
		delete[] arr;
	}

	DynamicArray(const DynamicArray&) = delete;

	const int getlenght() const { return lsize; };
	T* getptr(int indx) { return arr[indx]; }
	bool setlenght(int newLen)
	{
		if (newLen > phsize)
			resize(newLen);
		else
			return false;

		return true;
	}

	bool setptr(int indx, T* ptr) { arr[indx] = ptr; return true; }

	T* getObjectPtr(int id) const
	{
		for (int i = 0; i < lsize; i++)
		{
			if (arr[i]->getID() == id)
				return arr[i];
		}
		return nullptr;
	}

	bool addptr(T* objPtr)
	{
		if (lsize == phsize)
			resize(phsize * 2 + 1);
		arr[lsize] = objPtr;
		++lsize;
		return true;
	}

	//use only when the size is known
	bool addptrNoResize(T* objPtr)
	{
		if (lsize < phsize)
		{
			arr[lsize] = objPtr;
			++lsize;
			return true;
		}
		return false;
	}

	bool removeptr(const T* objPtr)
	{
		if (lsize == 1) ///the arr we recieved has one object inside
		{

			delete arr[lsize - 1];
			arr[lsize - 1] = nullptr;
			lsize = 0;
			return true;

		}
		else ////there are alot of objects in the array
		{
			for (int i = 0; i < lsize; i++)
			{
				if (arr[i] == objPtr)
				{
					T* temp;
					temp = arr[i];
					arr[i] = arr[lsize - 1];
					arr[lsize - 1] = temp;
					temp = nullptr;
					delete arr[lsize - 1];
					lsize--;
					return true;
				}
			}

		}
		return false; ///if we reached here, for unknown reason we couldnt delete ptr from arr
	}

	void printAll()const
	{
		for (int i = 0; i < lsize; i++)
			cout << *arr[i] << endl;
	}

	//saving/loading from/to file functions
	void Save(ostream& out) const
	{
		out.write(rcastcc(&lsize), sizeof(lsize));
		for (int i = 0; i < lsize; i++)
			arr[i]->Save(out);
	}

	void SaveMahoz(ostream& out) const
	{
		out.write(rcastcc(&lsize), sizeof(lsize));
		for (int i = 0; i < lsize; i++)
			mahozLoader::save(arr[i], out);
	}

	void Load(istream& in, Round& _round)
	{
		in.read(rcastc(&lsize), sizeof(lsize));
		if (!in.good())
		{
			cout << "Failed to load array lenght" << endl;
			exit(-1);
		}
		arr = new T * [lsize];
		phsize = lsize;
		for (int i = 0; i < lsize; i++)
			arr[i] = new T(in, _round);
	}

	void LoadMahoz(istream& in)
	{
		in.read(rcastc(&lsize), sizeof(lsize));
		if (!in.good())
		{
			cout << "Failed to load array lenght" << endl;
			exit(-1);
		}
		arr = new T* [lsize];
		phsize = lsize;

		for (int i = 0; i < lsize; i++)
			arr[i] = mahozLoader::load(in);
	}

private:
	void resize(int size)
	{
		T** temp = new T* [size];
		for (int i = 0; i < lsize; i++)
			temp[i] = arr[i];
		delete[] arr;
		arr = temp;
		phsize = size;
	}

private:
	T** arr;
	int phsize;
	int lsize;
};