#ifndef _READER_H_
#define _READER_H_

#include "GSBString.h"
#include "Array.h"
#include <stdint.h>
#include <stdio.h>

template <class T>
class Reader
{
public:
	virtual ~Reader() {}
	virtual bool Next(T &next) = 0;
	virtual bool Prev() = 0;
	virtual bool Peek(T &next) = 0;
	
private:
	Reader<T> &operator=(const Reader<T> &);
};

template<>
class Reader<char>
{
public:
	Reader() : _lineNumber(1), _column(0) {}

	virtual bool Next(char &next) = 0;
	virtual bool Prev() = 0;
	virtual bool Peek(char &next) = 0;

	uint16_t LineNumber() const {return _lineNumber;}
	uint16_t Column() const {return _column;}

protected:
	uint16_t _lineNumber;
	uint16_t _column;
};

class StringReader : public Reader<char>
{
public:
	StringReader(const Array<String> &lines) : _lines(lines) {}

	bool Next(char &next) 
	{
		// Find the next line with text in it
		while (_lineNumber <= int(_lines.size()) && _column >= int(_lines[_lineNumber - 1].length()))
		{
			if (++_lineNumber > int(_lines.size()))
				return false;
			_column = 0;
		}
		if (_lineNumber <= int(_lines.size()))
		{
			next = _lines[_lineNumber - 1].c_str()[_column++];
			return true;
		}
		else
			return false;
	}
	bool Prev()
	{
		while (--_column < 0 && _lineNumber > 1)
		{
			--_lineNumber;
			_column = uint16_t(_lines[_lineNumber -1].length() - 1);
		}
		return true;
	}
	virtual bool Peek(char &next)
	{
		// Find the next line with text in it
		uint16_t localLineNumber = _lineNumber;
		uint16_t localColumn = _column;
		while (localLineNumber <= int(_lines.size()) && _column >= int(_lines[localLineNumber - 1].length()))
		{
			if (++localLineNumber > int(_lines.size()))
				return false;
			localColumn = 0;
		}
		if (localLineNumber <= int(_lines.size()))
		{
			next = _lines[localLineNumber - 1].c_str()[localColumn++];
			return true;
		}
		else
			return false;
	}

private:
	StringReader &operator=(const StringReader &); // Unimplemented

	const Array<String> &_lines;
};

class FileReader : public Reader<char>
{
public:
	FileReader(FILE *file) : _file(file), _charReturned(false) {}

	bool Next(char &next) 
	{
		if (_charReturned)
		{
			next = _prev;
			_charReturned = false;
			return true;
		}
		int ic = fgetc(_file);
		if (ic == EOF)
			return false;
		_prev = next = char(ic);
		if (next == '\n')
		{
			++_lineNumber;
			_column = 0;
		}
		else
			++_column;
		return true;
	}
    
	bool Prev()
	{
		_charReturned = true;
		--_column;
		return true;
	}

private:
	FILE *_file;
	char _prev;
	bool _charReturned;
};

#endif