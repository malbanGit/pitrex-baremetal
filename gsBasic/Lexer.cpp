
#include "GSBCommon.h"
#include "Lexer.h"
#include "Token.h"
#include "Error.h"
#include "BuiltInConstants.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

Lexer::Lexer(Reader<char> &reader, size_t initialLineNumber, size_t fileIndex) :
	_reader(reader), _initialLineNumber(initialLineNumber), _treatDotAsFileName(false),
	_fileIndex(fileIndex)
{
}


Lexer::~Lexer()
{
}

Token Lexer::NextToken(Error &error)
{
#define CHECK_TOKEN_OVERFLOW \
	{ \
		if (tt >= tokenText + sizeof(tokenText)) \
		{ \
			error = Error(TokenTooLong, tokenLine, tokenStartColumn); \
			return nullToken; \
		} \
	}

	error = Error();

	Token nullToken;
	char tokenText[MAX_TOKEN_LEN + 1], *tt = tokenText;

	char c, peekC;
	if (!_reader.Next(c)) return nullToken;
	while (isspace(c))
	{
		if (!_reader.Next(c)) return nullToken;
	}

	uint16_t tokenLine = uint16_t(_reader.LineNumber() + _initialLineNumber);
	uint16_t tokenStartColumn = _reader.Column();

	// If the character is a letter or an underscore, get the identifier
	if (isalpha(c) || c == '_')
	{
		Token::TokenType type = Token::Identifier;
		do
		{
			*tt++ = c;
			if (c == '.')
				type = Token::FileName;
			CHECK_TOKEN_OVERFLOW;
		} while (_reader.Next(c) && (isalnum(c) || c == '_' || (c == '.' && _treatDotAsFileName)));
		*tt = '\0';
		_reader.Prev();

		if (strcasecmp("And", tokenText) == 0)
			return  Token(Token::And, tokenLine, tokenStartColumn, _fileIndex);
		if (strcasecmp("Mod", tokenText) == 0)
			return Token(Token::MOD, tokenLine, tokenStartColumn, _fileIndex);
		if (strcasecmp("Or", tokenText) == 0)
			return Token(Token::Or, tokenLine, tokenStartColumn, _fileIndex);
		if (strcasecmp("Xor", tokenText) == 0)
			return Token(Token::Xor, tokenLine, tokenStartColumn, _fileIndex);
		if (strcasecmp("Not", tokenText) == 0)
			return Token(Token::Not, tokenLine, tokenStartColumn, _fileIndex);
		if (strcasecmp("IsNil", tokenText) == 0)
			return Token(Token::IsNil, tokenLine, tokenStartColumn, _fileIndex);
		if (strcasecmp("REM", tokenText) == 0)
		{
			while (c != '\n' && _reader.Next(c)) ;
			return Token(Token::Comment, tokenLine, tokenStartColumn, _fileIndex);
		}
		const Value *builtInConstant = FindBuiltInConstant(tokenText);
		if (builtInConstant)
			return Token(builtInConstant, tokenLine, tokenStartColumn, _fileIndex);

		return Token(type, tokenText, tt - tokenText, tokenLine, tokenStartColumn, _fileIndex);
	}

	// Else if the character is a ' of if the two characters are //, get the rest of the line as a comment
	else if (c == '\'' || (c == '/' && _reader.Peek(peekC) && peekC == '/'))
	{
		while (c != '\n' && _reader.Next(c)) ;
		return Token(Token::Comment, tokenLine, tokenStartColumn, _fileIndex);
	}

	// Else if the character is a digit, get the number
	else if (isdigit(c))
	{
		do
		{
			*tt++ = c;
			CHECK_TOKEN_OVERFLOW;
		} while (_reader.Next(c) && isdigit(c));

		// If this is a hexadecimal number
		if ((c == 'x' || c == 'X') && tokenText[0] == '0' && tt - tokenText == 1)
		{
			tt = tokenText;
			while (_reader.Next(c) && isxdigit(c))
			{
				*tt++ = c;
				CHECK_TOKEN_OVERFLOW;
			}
			// Need at least one hex digit
			if (tt == tokenText) 
			{
				error = Error(UnexpectedEndOfText, tokenLine, tokenStartColumn);
				return nullToken;
			}
			_reader.Prev();

			*tt = '\0';
            uint32_t acc = 0;
            tt = tokenText;
            while (*tt)
            {
                acc *= 16;
                if (*tt <= '9')
                    acc += *tt - '0';
                else if (*tt <= 'F')
                    acc += *tt - 'A' + 10;
                else
                    acc += *tt - 'a' + 10;
                ++tt;
            }
			return Token(int(acc), tokenLine, tokenStartColumn, _fileIndex);
		}

		// Else if this is a float
		else if (c == '.' || c == 'e' || c == 'E')
		{
			if (c == '.')
			{
				*tt++ = '.';
				CHECK_TOKEN_OVERFLOW;

				while (_reader.Next(c) && isdigit(c))
				{
					*tt++ = c;
					CHECK_TOKEN_OVERFLOW;
				}
			}
			if (c == 'e' || c == 'E')
			{
				*tt++ = c;
				if (!_reader.Next(c))
				{
					error = Error(UnexpectedEndOfText, tokenLine, tokenStartColumn);
					return nullToken;
				}
				if (c == '-' || c == '+')
				{
					*tt++ = c;
					CHECK_TOKEN_OVERFLOW;
					if (!_reader.Next(c))
					{
						error = Error(UnexpectedEndOfText, tokenLine, tokenStartColumn);
						return nullToken;
					}
				}
				if (!isdigit(c))
				{
					error = Error(InvalidRealNumber, tokenLine, tokenStartColumn);
					return nullToken;
				}
				do
				{
					*tt++ = c;
					CHECK_TOKEN_OVERFLOW;
				} while (_reader.Next(c) && isdigit(c));
			}

			_reader.Prev();

			*tt = '\0';
			float value = float(strtof(tokenText, null));
			return Token(value, tokenLine, tokenStartColumn, _fileIndex);
		}

		// Else (I guess it's a decimal int)
		else
		{
			_reader.Prev();
			*tt = '\0';
			int value = atoi(tokenText);
			return Token(value, tokenLine, tokenStartColumn, _fileIndex);
		}
	}

	// Else if the character is a $ (another marker for a hexadecimal number)
	else if (c == '$')
	{
		while (_reader.Next(c) && isxdigit(c))
		{
			*tt++ = c;
			CHECK_TOKEN_OVERFLOW;
		}
		if (tt == tokenText)
		{
			error = Error(UnexpectedEndOfText, tokenLine, tokenStartColumn);
			return nullToken;
		}
		_reader.Prev();

		*tt = '\0';
		int value = strtol(tokenText, &tt, 16);
		return Token(value, tokenLine, tokenStartColumn, _fileIndex);
	}

	// Else if it's a string
	else if (c == '"')
	{
		int len = 0, maxLen = MAX_TOKEN_LEN;
		char *str = (char *) malloc(maxLen);

		while (_reader.Next(c) && c != '"')
		{
			if (len == maxLen - 2)
			{
				str = (char *) realloc(str, maxLen + MAX_TOKEN_LEN);
				if (str == null)
				{
					error = Error(OutOfMemory, tokenLine, tokenStartColumn);
					return nullToken;
				}
				maxLen += MAX_TOKEN_LEN;
			}
			str[len++] = c;
		}
		if (c != '"')
		{
            free(str);
			error = Error(UnexpectedEndOfText, tokenLine, tokenStartColumn);
			return nullToken;
		}

		str[len] = '\0';

		Token token(Token::String, str, len, tokenLine, tokenStartColumn, _fileIndex);
		free(str);
		return token;
	}

	// Else
	else
	{
		*tt++ = c;
		*tt = '\0';
		_reader.Next(*tt);
		*++tt = '\0';

		// Check if it's an operator
		for (int i = 0; *operatorKeywords[i].word; ++i)
		{
			if (strncmp(operatorKeywords[i].word, tokenText, strlen(operatorKeywords[i].word)) == 0)
			{
				// If the operator is only one character long, backup the second character we read
				if (operatorKeywords[i].word[1] == '\0')
					_reader.Prev();
				return Token(operatorKeywords[i].type, tokenLine, tokenStartColumn, _fileIndex);
			}
		}

		// We have no idea what this token is
		error = Error(UnrecognizedToken, tokenLine, tokenStartColumn);
		return nullToken;
	}
}
