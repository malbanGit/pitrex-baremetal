
#include "ExpressionList.h"
#include "Reader.h"
#include "Expression.h"
#include "VarSpec.h"
#include <stdio.h>

PoolAllocator<ExpressionList>ExpressionList:: _pool;

Error ExpressionList::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment, bool nestParens, uint16_t lineNumber, uint16_t column)
{
	Token token, nextToken;

	do
	{
		if (token.Type() != Token::Uninitialized)
		{
			lineNumber = token.LineNumber();
			column = token.Column();
		}
		Expression *expr;
		// Remember whether the next token is an LParen
		bool successfulPeek = tokenReader->Peek(nextToken);
		Error error = Expression::Build(tokenReader, compileEnvironment, expr, lineNumber, column);
		if (error)
		{
			Expression::Delete(expr);
			return error;
		}
		// nestParens and ParenGroup is for array initialization, so that a = { ( {1, 2, 3} ) } will put an array in a[1]
		if (successfulPeek && nestParens && nextToken.Type() == Token::LParen)
		{
			UnaryExpression *parenGroup = new UnaryExpression(Token::ParenGroup, expr);
			parenGroup->SetOperatorLocation(nextToken.LineNumber(), nextToken.Column());
			expr = parenGroup;
		}
		_expressions.push_back(expr);
	} while (tokenReader->Next(token) && token.Type() == Token::Comma);
	_expressions.shrink_to_fit();
	tokenReader->Prev();
	return NoError;
}


Error ExpressionList::Build(VarSpec *varSpec)
{
	_expressions.push_back(varSpec);
	return NoError;
}

void ExpressionList::Add(Expression *expr)
{
	_expressions.push_back(expr);
}


uint16_t ExpressionList::LineNumber() const 
{
	if (_expressions.size())
	{
		const Expression *expr = _expressions[0];
		if (Expression::IsExpressionPointer(expr))
			return expr->LineNumber();
	}

	return 0;
}

uint16_t ExpressionList::Column() const 
{
	if (_expressions.size())
	{
		const Expression *expr = _expressions[0];
		if (Expression::IsExpressionPointer(expr))
			return expr->Column();
	}

	return 0;
}

uint16_t ExpressionList::LastLineNumber() const
{
	for (int i = (int) _expressions.size() - 1; i >= 0; --i)
	{
		const Expression *expr = _expressions[i];
		if (Expression::IsExpressionPointer(expr))
			return expr->LastLineNumber();
	}

	return 0;
}

uint16_t ExpressionList::LastColumn() const
{
	for (int i = (int) _expressions.size() - 1; i >= 0; --i)
	{
		const Expression *expr = _expressions[i];
		if (Expression::IsExpressionPointer(expr))
			return expr->LastColumn();
	}

	return 0;
}


#ifdef _DEBUG
void ExpressionList::Dump()
{
    printf("ExpressionList: %p\n\r", &_expressions);
    printf("ExpressionList Size: %i\n\r", _expressions.size());
    puts("ExpressionList[");
	for (size_t i = 0; i < _expressions.size(); ++i)
    {
    printf("ExpressionList[%i]: %p\n\r",i, &_expressions[i]);
//printf("ExpressionList Class: %s\n\r", typeid(*_expressions[i]).name());     
    
    
//        _expressions[i]->Dump();
    }
	puts("]");
}
#endif
