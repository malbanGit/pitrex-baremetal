
#ifndef _PARSER_H_
#define _PARSER_H_

#include "GSBCommon.h"
#include "Reader.h"
#include "Environment.h"
#include "Error.h"

class Token;
class Statement;
class TokenReader;

class Parser
{
public:
	Parser(TokenReader *tokenReader);
	~Parser();

	Statement *NextStatement(Error &error, CompileEnvironment &compileEnvironment);

private:
	TokenReader *_tokenReader;
};

#endif
