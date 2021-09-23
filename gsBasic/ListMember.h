
#ifndef _LISTMEMBER_H
#define _LISTMEMBER_H

#include "GSBCommon.h"

template <class T>
class ListMember
{
public:
	ListMember() :
		_next(null)
	{
	}

	T *GetNext() { return _next; }
	void SetNext(T *next) { _next = next; }

private:
	T *_next;
};

#endif
