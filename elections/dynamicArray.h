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
			throw Load_error("Failed to load array lenght");
		
		arr = new T * [lsize];
		phsize = lsize;
		for (int i = 0; i < lsize; i++)
			arr[i] = new T(in, _round);
	}

	void LoadMahoz(istream& in)
	{
		in.read(rcastc(&lsize), sizeof(lsize));
		if (!in.good())
			throw Load_error("Failed to load array lenght");
		
		arr = new T* [lsize];
		phsize = lsize;

		for (int i = 0; i < lsize; i++)
			arr[i] = mahozLoader::load(in);
	}

	template <bool is_const>
	class base_iterator
	{
	public:
		using ds_type = std::conditional_t<is_const, const DynamicArray, DynamicArray>;

		using iterator_category = std::bidirectional_iterator_tag;
		// other options exist, e.g., std::forward_iterator_tag
		using different_type = std::ptrdiff_t;
		using value_type = std::conditional_t<is_const, const T, T>;
		using pointer = value_type*;
		using reference = value_type&;

		base_iterator(ds_type& arr, int i) : _da(&arr), _i(i) {}

		// we want to use the default copy constructor
		base_iterator(const base_iterator&) = default;

		// and the default assignment operator
		constexpr base_iterator& operator=(const base_iterator&) = default;

		// we want to allow construction of const_iterator from iterator
		friend class base_iterator<true>;
		template <bool _is_const = is_const, class = std::enable_if_t<_is_const>>
		base_iterator(const base_iterator<false>& other) : _da(other._da), _i(other._i) {}

		// comparison with another iterator
		bool operator==(const base_iterator& other) const {
			return (_da == other._da) && (_i == other._i);
		}
		bool operator!=(const base_iterator& other) const {
			return !(*this == other);
		}

		// smart-pointer iterator methods
		reference operator*() {
			return _da->_arr[_i];
		}
		pointer operator->() {
			return &_da->_arr[_i];
		}

		// increment-decrement iterator methods
		base_iterator& operator++() {
			++_i;
			return *this;
		}
		base_iterator operator++(int) {
			base_iterator temp(*this);
			++_i;
			return temp;
		}
		base_iterator& operator--() {
			--_i;
			return *this;
		}
		base_iterator operator--(int) {
			base_iterator temp(*this);
			--_i;
			return temp;
		}
	private:
		ds_type* _da;
		int			_i;
	};

	using iterator = base_iterator<false>;
	using const_iterator = base_iterator<true>;

	// should return iterator to new element
	void insert(const iterator& pos, const T& val) {
		if (lsize == phsize)
			resize();

		iterator itrEnd = end();
		iterator itrCurrent = itrEnd, itrPrev = --itrEnd;
		while (itrCurrent != pos)
		{
			*itrCurrent = *itrPrev;
			itrCurrent = itrPrev--;
		}

		iterator p = pos;
		*p = val;
		++lsize;
	}

	// returns iterator to first element after deleted element/s
	iterator erase(const iterator& iter);
	iterator erase(const iterator& first, const iterator& last);

	iterator begin() {
		return iterator(*this, 0);
	}
	iterator end() {
		return iterator(*this, lsize);
	}
	const_iterator begin() const {
		return const_iterator(*this, 0);
	}
	const_iterator end() const {
		return const_iterator(*this, lsize);
	}

private:
	T** arr;
	int phsize;
	int lsize;
	void resize(int size)
	{
		T** temp = new T* [size];
		for (int i = 0; i < lsize; i++)
			temp[i] = arr[i];
		delete[] arr;
		arr = temp;
		phsize = size;
	}
};