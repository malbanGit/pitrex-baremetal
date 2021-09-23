
#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "GSBCommon.h"
#ifdef _MSC_VER
#include <algorithm>
#endif
#include <stdlib.h>

extern "C" void OutOfMemoryHandler();

#define _allocationIncrement 10

/**
 * A dynamic array. A lighter weight replacement for STL's vector class
 */
template <class T>
class Array
{
public:
	Array() :
		_capacity(0), _size(0), _array(null)
	{
	}
	virtual ~Array()
	{
		resize(0);
	}

	void push_back(const T &val)
	{
		if (_capacity == _size)
			Enlarge(_size + 1);
		// This is a kludge to allow us to use MSVC's memory leak detection. It normally
		// doesn't work with placement new, so we move all the calls to placement new into
		// a separate file
		PlacementNew(_array + _size);
		_array[_size++] = val;
	}
	void pop_back() 
	{
		if (_size) 
		{
			--_size;
			((T *) (_array + _size))->~T();
		}
	}

	T &back() {return _array[_size - 1];}
	const T &back() const {return _array[_size - 1];}
	T &back(size_t depth) { return _array[_size - 1 - depth]; }
	const T &back(size_t depth) const { return _array[_size - 1 - depth]; }

	T &operator[](size_t n) {return _array[n];}
	const T &operator[](size_t n) const {return _array[n];}

	virtual void clear() {resize(0);}

	size_t size() const {return _size;}
	bool empty() const {return _size == 0;}

	virtual void resize(size_t sz)
	{
		if (sz <= size())
		{
			for (size_t i = sz; i < size(); ++i)
            {
                ((T *) (_array + i))->~T();
            }
            if (sz == 0)
            {
                free(_array);
                _array = NULL;
                _capacity = 0;
            }
		}
		else
		{
			Enlarge(sz);
			for (size_t i = size(); i < sz; ++i)
				PlacementNew(_array + i);
			_capacity = sz;
		}
		_size = sz;
	}

	void sort(int (*compare)(const T &a, const T &b))
	{
		// Implement a bubble sort. We don't expect to have large arrays anyway
		bool swapped;
		do
		{
			swapped = false;
			for (int i = 0; i < int(_size - 1); ++i)
			{
				if (compare(_array[i], _array[i + 1]) > 0)
				{
					T temp = _array[i];
					_array[i] = _array[i + 1];
					_array[i + 1] = temp;
					swapped = true;
				}
			}
		} while (swapped);
	}
    
    const T *Raw() const {return _array;}

	void shrink_to_fit()
	{
		if (_capacity > _size)
		{
			int newCapacity = _size;
			_array = (T *) realloc(_array, newCapacity * sizeof(T));
			if (_array == NULL)
				OutOfMemoryHandler();
			_capacity = newCapacity;
		}
	}

    void append(const Array<T> &data)
    {
        size_t currentSize = size();
        resize(currentSize + data.size());
        for (size_t i = 0; i < data.size(); ++i)
            (*this)[currentSize + i] = data[i];
    }
    
    // std::vector compatibility
    void reserve(size_t sz) {Enlarge(sz);}
    T *data() {return _array;}
    const T *data() const {return _array;}
    T *begin() {return _array;}
    T *end() {return _array + size();}
    T *insert(T *pos, const T &elem)
    {
        size_t insertIndex = pos - data();
        resize(size() + 1);
        for (size_t i = size() - 1; i > insertIndex; --i)
            (*this)[i] = (*this)[i - 1];
        (*this)[insertIndex] = elem;
        return data() + insertIndex;
    }
    T *erase(T *item)
    {
        size_t eraseIndex = item - data();
        item->~T();
        for (size_t i = eraseIndex; i < size(); ++i)
            (*this)[i] = (*this)[i + 1];
        if (size()>=1)
          resize(size() - 1);
        return item;
    }
    
protected:
	void Enlarge(size_t newMinimumSize)
	{
		if (_capacity + _allocationIncrement > newMinimumSize)
			newMinimumSize = _capacity + _allocationIncrement;
		int newCapacity = newMinimumSize * sizeof(T);
        T *newArray = (T *) malloc(newCapacity);
        if (newArray == NULL)
            OutOfMemoryHandler();
        for (size_t i = 0; i < size(); ++i)
        {
            PlacementNew(newArray + i);
            newArray[i] = _array[i];
            _array[i].~T();
        }
        free(_array);
		_array = newArray;
		_capacity = newMinimumSize;
	}

	void PlacementNew(void *addr);

protected:
	size_t _capacity, _size;
	T *_array;
};

/**
 * A dynamic array that holds pointers
 */
template <class T, int minSize = 1>
class PtrArray
{
public:
	typedef T *TP;

	PtrArray() : _size(0) 
	{
		memset(_array, 0, sizeof(_array));
	}
	virtual ~PtrArray()
	{
		resize(0);
	}

	void push_back(const TP &val)
	{
		resize(_size + 1);
		(*this)[_size - 1] = val;
	}
	void pop_back()
	{
		if (_size)
		{
			resize(_size - 1);
		}
	}

	TP &back() { return (*this)[_size - 1]; }
	const TP &back() const { return (*this)[_size - 1]; }
	TP &back(size_t depth) { return (*this)[_size - 1 - depth]; }
	const TP &back(size_t depth) const { return (*this)[_size - 1 - depth]; }

	TP &operator[](size_t n) 
	{
		// If the user is requesting a low enough index that it's in our local array
		// OR the index is to the top of the local array and the top element is a T *
		// (as indicated by the fact that we have not set its low bit)
		if (n < minSize - 1)
			return _array[n];
		// Otherwise, we need to go to our allocated array
		TP *allocated = AllocatedArray();
		if (allocated)
			return allocated[n - minSize + 1];
		// If there's no allocated array, the user must be requesting the top item in the local array
		return _array[n];
	}
	const TP &operator[](size_t n) const
	{
		return ((PtrArray <T, minSize> *) this)->operator[](n);
	}

	virtual void clear() { resize(0); }

	size_t size() const { return _size; }
	bool empty() const { return _size == 0; }

	virtual void resize(size_t newSize)
	{
		TP *allocated = AllocatedArray();

		// If the new size will fit in our local array
		if (newSize <= minSize)
		{
			// We don't need any allocated array anymore
			if (allocated)
			{
				_array[minSize - 1] = allocated[0];
				free(allocated);
			}
		}
		// Else (we need an allocated array)
		else
		{
			// If there's already an allocated array
			if (allocated)
			{
				// Resize it
				allocated = (TP *) realloc(allocated, sizeof(TP) * (newSize - minSize + 1));
				// Set the last element of the local array to point to the allocated array
				_array[minSize - 1] = (TP) allocated;
				for (size_t i = size(); i < newSize; ++i)
					operator[](i) = null;
			}
			// Else (no allocated array yet)
			else
			{
				// Create one
				allocated = (TP *) calloc(sizeof(TP), newSize - minSize + 1);
				// We'll be using the last element of the local array to point to the allocated
				// array, so we need to save the current last element of the local array
				allocated[0] = _array[minSize - 1];
				// Set the last element of the local array to point to the allocated array
				_array[minSize - 1] = (TP) allocated;
			}
		}
		_size = newSize;
	}

	void shrink_to_fit()
	{}

	void sort(int(*compare)(const TP &a, const TP &b))
	{
		// Implement a bubble sort. We don't expect to have large arrays anyway
		bool swapped;
		do
		{
			swapped = false;
			for (int i = 0; i < int(_size - 1); ++i)
			{
				if (compare(operator[](i), operator[](i + 1)) > 0)
				{
					TP temp = operator[](i);
					operator[](i) = operator[](i + 1);
					operator[](i + 1) = temp;
					swapped = true;
				}
			}
		} while (swapped);
	}

private:
	const TP *AllocatedArray() const
	{
		if (size() > minSize)
			return (const TP *) _array[minSize - 1];
		else
			return null;
	}
	TP *AllocatedArray() 
	{
		if (size() > minSize)
			return (TP *) _array[minSize - 1];
		else
			return null;
	}
	
	size_t _size;
	TP _array[minSize];
};

/**
 * A dynamic array that holds pointers and deletes deletes them in its destructor
 */
template <class T, int minSize = 1>
class PtrOwnerArray : public PtrArray<T, minSize>
{
public:
	~PtrOwnerArray() { resize(0); }
	virtual void resize(size_t size)
	{
		size_t oldSize = PtrArray<T, minSize>::size();
		for (size_t i = size; i < oldSize; ++i)
			delete PtrArray<T, minSize>::operator[](i);
		PtrArray<T, minSize>::resize(size);
	}
};

#endif
