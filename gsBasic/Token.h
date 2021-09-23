
#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "GSBCommon.h"
#include "Reader.h"
#include <stdint.h>

class Value;
class Lexer;

#define PRECEDENCE_GROUP 8

class Token
{
public:
	typedef enum
	{
		LParen = PRECEDENCE_GROUP * 0,

		UnaryOps = PRECEDENCE_GROUP * 1,
		UNARYPLUS,
		UNARYMINUS,
		Not,
		BitwiseNOT,
		IsNil,
		// ParenGroup is for array initialization, so that a = { ( {1, 2, 3} ) } will put an array in a[1]
		ParenGroup,
		EndOfUnaryOps,

		MUL = PRECEDENCE_GROUP * 2,
		DIV,
		MOD,

		PLUS = PRECEDENCE_GROUP * 3,
		MINUS,

		LE = PRECEDENCE_GROUP * 4,
		GE,
		LT,
		GT,

		EQ = PRECEDENCE_GROUP * 5,
		EQCmp,
		NE,

		BitwiseAND = PRECEDENCE_GROUP * 6,
		BitwiseXOR = PRECEDENCE_GROUP * 7,
		BitwiseOR = PRECEDENCE_GROUP * 8,

		And = PRECEDENCE_GROUP * 9,
		Xor = PRECEDENCE_GROUP * 10,
		Or = PRECEDENCE_GROUP * 11,

		RParen = PRECEDENCE_GROUP * 12,

		EndOfOperators,

		LBracket,
		LBrace, // 83
		RBracket,
		RBrace,

		DOT,

		// Variable names
		Identifier,

		// Label delimiter
		Colon,
		// For lists
		Comma, // 88
//		Semicolon,

		// Constants
		Int,
		Float,
		String,
		BuiltInConstant,

		FileName,

		Comment, // 94
		Uninitialized,
	} TokenType;

	static bool IsOperator(TokenType type) {return type < EndOfOperators;}
	static bool IsUnaryOperator(TokenType type) {return type > UnaryOps && type < EndOfUnaryOps;}
	static bool IsLeftBracket(TokenType type) {return type == LParen || type == LBrace || type == LBracket;}
	static bool IsRightBracket(TokenType type) {return type == RParen || type == RBrace || type == RBracket;}
	static bool BracketsMatch(TokenType left, TokenType right) {return left == right - (RParen - LParen);}
	static int OperatorPrecedence(TokenType type) {return type / PRECEDENCE_GROUP;}

	Token() : _tokenType(Uninitialized) {}
	Token(TokenType type, char *text, int len, uint16_t lineNumber, uint16_t column, size_t fileIndex);
	Token(int value, uint16_t lineNumber, uint16_t column, size_t fileIndex);
	Token(float value, uint16_t lineNumber, uint16_t column, size_t fileIndex);
	Token(const Value *value, uint16_t lineNumber, uint16_t column, size_t fileIndex);
	Token(TokenType type, uint16_t lineNumber, uint16_t column, size_t fileIndex);
	Token(const Token &src);

	~Token();

	Token &operator=(const Token &src);

	bool HasString() const {return _tokenType == Identifier || _tokenType == String || _tokenType == FileName;}
	TokenType Type() const {return _tokenType;}
	uint16_t LineNumber() const {return _lineNumber;}
	uint16_t Column() const {return _column;}
	int ValueInt() const {return _integer;}
	float ValueFloat() const {return _float;}
	const char *ValueString() const {return Type() == String ? _tokenText : null;}
	const char *ValueIdentifier() const {return Type() == Identifier ? _tokenText : null;}
	const char *ValueFileName() const {return Type() == FileName ? _tokenText : null;}
	const Value &ValueBuiltInConstant() const;

	void ModifyType(TokenType type) {_tokenType = type;}

#ifdef _DEBUG
	void Dump();
#endif

private:
	TokenType _tokenType;
	union
	{
		char *_tokenText;
		int _integer;
		float _float;
		const Value *_value;
	};
	uint16_t _lineNumber;
	uint16_t _column;
};


struct Keyword
{
	const char *word;
	Token::TokenType type;
};
extern struct Keyword operatorKeywords[];


class TokenReader
{
public:
	TokenReader(const Array<String> &lines, size_t initialLineNumber, size_t fileIndex);
	~TokenReader();
	bool Next(Token &next);
	bool Prev();
	bool Peek(Token &next);

	// Dot can be used for a struct member or in a file name (since LOAD
	// can work without quotes). So this kludge allows StatementLoad to
	// convey how to interpret a dot
	void TreatDotAsFileName(bool enable);

private:
	StringReader _stringReader;
	Lexer *_lexer;
	Token _tokens[3];
	size_t _nextIndex;
};

#endif
