#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "GSBCommon.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

// ARM are per default little endian
// this should work!

#if !defined(__XC32) && !defined(_WIN32) && !defined(RPI0)
#error Short strings only work on little-endian platforms
#endif

class String
{
public:
	String() : _p(null) {}
	String(const char *s) :
		_p(null)
	{
		operator=(s);
	}
	String(const String &s) :
		_p(null)
	{
		operator=(s);
	}
	String(const char *arg1, const char *arg2);

	String(const String &arg1, const String &arg2)
	{
		Build(arg1.c_str(), arg1.length(), arg2.c_str(), arg2.length());
	}
	String(const String &arg1, const char *arg2)
	{
		Build(arg1.c_str(), arg1.length(), arg2, (int) strlen(arg2));
	}
	String(const char *arg1, const String &arg2)
	{
		Build(arg1, (int) strlen(arg1), arg2.c_str(), arg2.length());
	}
	String(const char *arg, int length)
	{
		Build(arg, length, null, 0);
	}
	String(char init, int count);

	void Build(const char *arg1, int len1, const char *arg2, int len2);

	~String() { Release(); }

	size_t length() const { return _p ? (IsShortString() ? strlen(_shortString + 1) : _p->length) : 0; }
	bool empty() const { return length() == 0; }

	void clear() { Release(); }

	String &operator=(const char *s);

	String &operator=(const String &s)
	{
		if (s._p != _p)
		{
			Release();
			if (s._p)
			{
				_p = s._p;
				if (!IsShortString())
					_p->refCnt++;
			}
		}
		return *this;
	}

	bool operator==(const String &s) const
	{
		if (s._p == _p)
			return true;
		if (_p)
		{
			return (s._p && strcmp(c_str(), s.c_str()) == 0);
		}
		return false;
	}
	bool operator==(const char *s) const
	{
		if (_p && s)
		{
			return (strcmp(c_str(), s) == 0);
		}
		return false;
	}
	bool operator!=(const String &s) const { return !operator==(s); }
	bool operator<=(const String &s) const
	{
		if (_p)
			return (s._p && strcmp(c_str(), s.c_str()) <= 0);
		return false;
	}
	bool operator<(const String &s) const
	{
		if (_p)
			return (s._p && strcmp(c_str(), s.c_str()) < 0);
		return false;
	}
	bool operator>=(const String &s) const
	{
		if (_p)
			return (s._p && strcmp(c_str(), s.c_str()) >= 0);
		return false;
	}
	bool operator>(const String &s) const
	{
		if (_p)
			return (s._p && strcmp(c_str(), s.c_str()) > 0);
		return false;
	}

	String &operator+=(const String &s)
	{
        if (_p)
            *this = String(*this, s);
        else
            *this = s;
		return *this;
	}
	String &operator+=(const char *s)
	{
		*this = String(*this, s);
		return *this;
	}

	int strcasecmp(const String &s) const
	{
		if (s._p == _p)
			return 0;
		if (_p)
		{
			if (s._p) return ::strcasecmp(c_str(), s.c_str());
		}
		return -1;
	}

	String Substr(int start, int count) const;

	/**
	 * Return true if string is not empty
	 */
	operator bool() const { return (c_str() && *c_str()) ? true : false; }

	const char *c_str() const { return _p ? (IsShortString() ? (_shortString + 1) : _p->text) : null; }
	char *buffer() { return _p ? (IsShortString() ? (_shortString + 1) : _p->text) : null; }

	char operator[](size_t i) const { return IsShortString() ? _shortString[i + 1] : _p->text[i]; }
	char &operator[](size_t i) { return IsShortString() ? _shortString[i + 1] : _p->text[i]; }

	static String itoa(int i);

	static String ftoa(float f);

	static int Compare(const String &a, const String &b) { return strcmp(a.c_str(), b.c_str()); }
	static int CompareCaseInsensitive(const String &a, const String &b) { return a.strcasecmp(b); }

private:
	void Release()
	{
		if (_p && !IsShortString())
		{
			if (--(_p->refCnt) == 0)
			{
				delete [] (uint8_t *) _p;
			}
		}
		_p = null;
	}

	// If the low bit of the pointer is 1 (i.e. it's an odd address), we're storing a short string
	bool IsShortString() const { return uint32_t(_p) & 1; }

	struct RefCntPtr
	{
		uint16_t refCnt;
		uint16_t length;
		char text[1];
	};
	
	union
	{
		RefCntPtr *_p;
		char _shortString[4];
	};
};

inline String operator+(const String &s1, const char *s2)
{
	return String(s1, s2);
}

inline String operator+(const char *s1, const String &s2)
{
	return String(s1, s2);
}

inline String operator+(const String &s1, const String &s2)
{
	return String(s1, s2);
}

#endif
