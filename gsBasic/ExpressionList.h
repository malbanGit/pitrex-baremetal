#ifndef _EXPRESSIONLIST_H_
#define _EXPRESSIONLIST_H_

#include "AST.h"
#include "Expression.h"
#include "Token.h"
#include "Pool.h"

class Expression;
class Executive;
class VarSpec;

class ExpressionList
{
public:
	ExpressionList() {}

	// If nestParens is true, then expressions surrounded by parentheses will be nested inside a
	// ParenGroup operator
	Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment, bool nestParens, uint16_t lineNumber, uint16_t column);

	// Build a single-element ExpressionList whose expression is a VarSpec
	Error Build(VarSpec *varSpec);

	const ExpressionPtrOwnerArray &Expressions() const {return _expressions;}

	void Add(Expression *expr);

	size_t size() const { return _expressions.size(); }

#pragma push_macro("new")
#undef new
	void *operator new(size_t) {return _pool.Allocate();}
	void *operator new(size_t, int, const char *, int) {return _pool.Allocate(); }
	void operator delete(void *p) { _pool.Free((ExpressionList *) p); }
	void operator delete(void *p, int, const char *, int) { _pool.Free((ExpressionList *) p); }
#pragma pop_macro("new")

#ifdef _DEBUG
	void Dump();
#endif

	uint16_t LineNumber() const;
	uint16_t Column() const;
	uint16_t LastLineNumber() const;
	uint16_t LastColumn() const;

private:
	ExpressionPtrOwnerArray _expressions;

	static PoolAllocator<ExpressionList> _pool;
};

#endif
