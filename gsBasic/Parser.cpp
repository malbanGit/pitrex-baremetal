#include "Parser.h"
#include "Token.h"
#include "Statements.h"

Parser::Parser(TokenReader *tokenReader) :
	_tokenReader(tokenReader)
{
}


Parser::~Parser()
{
}

Statement *Parser::NextStatement(Error &error, CompileEnvironment &compileEnvironment)
{
	error = Error();

	Token token;
	if (_tokenReader->Next(token))
	{
		Statement *stmt = Statement::CreateStatement(token, error);
		// If CreateStatement couldn't figure out what this is
		if (error.Message() == UnrecognizedStatement && token.Type() == Token::Identifier)
		{
			// Look ahead one token
			Token nextToken;
			if (_tokenReader->Next(nextToken))
			{
//				Token::TokenType nextType = nextToken->Type();
				_tokenReader->Prev();
				// If it's a colon, this is a label
/*				if (nextType == Token::Colon)
					stmt = new StatementLabel();
				// Else it's probably an assignment
				else*/
				{
					stmt = new StatementAssignment();
					if (stmt == null)
						error = Error(OutOfProgramMemory, token.LineNumber(), token.Column());
				}
			}
		}
		else if (error.Message() == UnrecognizedStatement && token.Type() == Token::BuiltInConstant)
		{
			error = Error(BuiltInConstant, error.LineNumber(), error.Column());
		}

		if (stmt)
		{
			_tokenReader->Prev();
			error = stmt->Build(_tokenReader, compileEnvironment);
			if (error)
			{
				delete stmt;
				stmt = null;
			}
			return stmt;
		}
		else if (!error)
			error = Error(UnrecognizedStatement, token.LineNumber(), token.Column());
	}

	return null;
}
