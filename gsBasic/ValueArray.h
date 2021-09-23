
#ifndef _VALUEARRAY_H
#define _VALUEARRAY_H

#include "Value.h"
#include "Array.h"
#ifdef DBG_NEW
#undef new
#endif
#include <new>

class ValueArray : public Array<Value>
{
public:
	void push_back(int val)
	{
		if (_capacity == _size)
			Enlarge(_size + 1);
		// This is a kludge to allow us to use MSVC's memory leak detection. It normally
		// doesn't work with placement new, so we move all the calls to placement new into
		// a separate file
		new (_array + _size) Value;
		_array[_size++] = val;
	}

	void push_back(float val)
	{
		if (_capacity == _size)
			Enlarge(_size + 1);
		// This is a kludge to allow us to use MSVC's memory leak detection. It normally
		// doesn't work with placement new, so we move all the calls to placement new into
		// a separate file
		new (_array + _size) Value;
		_array[_size++] = val;
	}

	void push_back(const String &val)
	{
		if (_capacity == _size)
			Enlarge(_size + 1);
		// This is a kludge to allow us to use MSVC's memory leak detection. It normally
		// doesn't work with placement new, so we move all the calls to placement new into
		// a separate file
		new (_array + _size) Value;
		_array[_size++] = val;
	}

	void push_back(const Value &val)
	{
		Array<Value>::push_back(val);
	};
};

#endif
