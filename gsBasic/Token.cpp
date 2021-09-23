#include "Token.h"
#include "GSBString.h"
#include "Value.h"
#include "Lexer.h"
#include <string.h>
#include <stdio.h>

struct Keyword operatorKeywords[] = {
	{"(", Token::LParen},
	{")", Token::RParen},
	{"[", Token::LBracket},
	{"]", Token::RBracket},
	{"{", Token::LBrace},
	{"}", Token::RBrace},
	{",", Token::Comma},
	{"==", Token::EQCmp},
	{"=", Token::EQ},
	{"<=", Token::LE},
	{">=", Token::GE},
	{"<>", Token::NE},
	{"!=", Token::NE},
	{"<", Token::LT},
	{">", Token::GT},
	{"+", Token::PLUS},
	{"-", Token::MINUS},
	{"+", Token::UNARYPLUS},
	{"-", Token::UNARYMINUS},
	{"*", Token::MUL},
	{"/", Token::DIV},
	{"^", Token::BitwiseXOR},
	{"&", Token::BitwiseAND},
	{"|", Token::BitwiseOR},
	{"~", Token::BitwiseNOT},
	{":", Token::Colon},
	{".", Token::DOT},
//	{";", Token::Semicolon},
	{""}
};

Token::Token(TokenType type, char *text, int len, uint16_t lineNumber, uint16_t column, size_t fileIndex) :
	_tokenType(type), _lineNumber(EncodeLineNumber(fileIndex, lineNumber)), _column(column)
{
	_tokenText = new char[len + 1];
	strncpy(_tokenText, text, len);
	_tokenText[len] = '\0';
}

Token::Token(int value, uint16_t lineNumber, uint16_t column, size_t fileIndex) :
	_tokenType(Int), _lineNumber(EncodeLineNumber(fileIndex, lineNumber)), _column(column)
{
	_integer = value;
}

Token::Token(float value, uint16_t lineNumber, uint16_t column, size_t fileIndex) :
	_tokenType(Float), _lineNumber(EncodeLineNumber(fileIndex, lineNumber)), _column(column)
{
	_float = value;
}

Token::Token(const Value *value, uint16_t lineNumber, uint16_t column, size_t fileIndex) :
	_tokenType(BuiltInConstant), _lineNumber(EncodeLineNumber(fileIndex, lineNumber)), _column(column)
{
	_value = value;
}

Token::Token(TokenType type, uint16_t lineNumber, uint16_t column, size_t fileIndex) :
	_tokenType(type), _lineNumber(EncodeLineNumber(fileIndex, lineNumber)), _column(column)
{
}

Token::Token(const Token &src) :
	_tokenType(src._tokenType), _lineNumber(src._lineNumber), _column(src._column) 
{
	if (src.HasString())
	{
		_tokenText = new char[strlen(src._tokenText) + 1];
		strcpy(_tokenText, src._tokenText);
	}
	else
		_integer = src._integer;
}

Token::~Token()
{
	if (HasString())
		delete [] _tokenText;
}

Token &Token::operator=(const Token &src)
{
	if (this != &src)
	{
		if (HasString())
			delete [] _tokenText;
		_tokenType = src._tokenType;
		_lineNumber = src._lineNumber;
		_column = src._column;
		if (src.HasString())
		{
			_tokenText = new char[strlen(src._tokenText) + 1];
			strcpy(_tokenText, src._tokenText);
		}
		else
			_integer = src._integer;
	}
	return *this;
}

const Value &Token::ValueBuiltInConstant() const 
{
	static Value nil;
	return Type() == BuiltInConstant ? *_value : nil; 
}

#ifdef _DEBUG
void Token::Dump()
{
	if (Type() == String)
	{
		putc('"', stdout);
		fputs(_tokenText, stdout);
		fputs("\"", stdout);
	}
	else if (Type() == Identifier)
		fputs(_tokenText, stdout);
	else if (Type() == Int)
		fputs(String::itoa(_integer).c_str(), stdout);
	else if (Type() == Float)
		fputs(String::ftoa(_float).c_str(), stdout);
	else if (Type() == BuiltInConstant)
		fputs(_value->ToString().c_str(), stdout);
	else if (IsOperator(Type()))
	{
		for (int i = 0; *operatorKeywords[i].word; ++i)
		{
			if (operatorKeywords[i].type == Type())
			{
				fputs(operatorKeywords[i].word, stdout);
				break;
			}
		}
	}
	else
	{
		fputs("Token type ", stdout);
		fputs(String::itoa(Type()).c_str(), stdout);
	}
    fputs("\r\n", stdout);
}

#endif

TokenReader::TokenReader(const Array<String> &lines, size_t initialLineNumber, size_t fileIndex) :
	_stringReader(lines), _lexer(new Lexer(_stringReader, initialLineNumber, fileIndex)), _nextIndex(countof(_tokens))
{
}

TokenReader::~TokenReader() 
{
	delete _lexer; 
}

bool TokenReader::Next(Token &next)
{
	if (_nextIndex >= countof(_tokens))
	{
		Error error;
		Token temp = _lexer->NextToken(error);
		if (!error)
		{
			next = temp;
			for (size_t i = 1; i < countof(_tokens); ++i)
				_tokens[i - 1] = _tokens[i];
			_tokens[countof(_tokens) - 1] = next;
			return next.Type() != Token::Uninitialized;
		}
		// We've read past the end of the tokens. Increment _nextIndex so that it's 2 past the 
		// end of the _tokens. This is so when someone calls Prev(), we're still at the end of the tokens
		else
			++_nextIndex;
		return false;
	}
	else
	{
		next = _tokens[_nextIndex++];
		return next.Type() != Token::Uninitialized;
	}
}

bool TokenReader::Prev()
{
	if (_nextIndex > 0)
	{
		--_nextIndex;
		return true;
	}
	return false;
}

bool TokenReader::Peek(Token &next)
{
	if (Next(next))
	{
		Prev();
		return true;
	}
	return false;
}

void TokenReader::TreatDotAsFileName(bool enable) 
{
	_lexer->TreatDotAsFileName(enable); 
}
