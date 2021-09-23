
#ifndef _LEXER_H_
#define _LEXER_H_

#include "GSBCommon.h"
#include "Token.h"
#include "Error.h"
#include "Reader.h"

class Lexer
{
public:
	Lexer(Reader<char> &reader, size_t initialLineNumber, size_t fileIndex);
	~Lexer();

	Token NextToken(Error &error);

	void TreatDotAsFileName(bool enable) { _treatDotAsFileName = enable; }

private:
	Lexer &operator=(const Lexer &);

	Reader<char> &_reader;
	size_t _initialLineNumber;
	bool _treatDotAsFileName;
	size_t _fileIndex;
};

#endif
