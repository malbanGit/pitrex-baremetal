
#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_ 

#include "AST.h"
#include "Token.h"
#include "Array.h"
#include "Pool.h"
#include "TypeBehaviors.h"

class VarSpec;
class ExpressionPtrOwnerArray;

class Expression : public AST
{
	// The low two bits of an Expression* will have one of these values in it. This allows us
	// to compactly store integer and float constants
	enum {ExpressionPointer, IntConstant, FloatConstant};

public:
	Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment) = 0;
	static Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment, Expression *&expression, uint16_t lineNumber, uint16_t column);
	static Error Build(int i, Expression *&expression);
	static Error Build(const char *s, Expression *&expression);

	static Error Execute(Expression *expr, Executive &exec, RunTimeEnvironment &runTimeEnvironment);
	static VarSpec *CastToVarSpec(Expression *expr);
	static void Delete(Expression *expr);
	static bool IsExpressionPointer(const Expression *expr) { return (long(expr) & 3) == ExpressionPointer; }
    static uint16_t SLineNumber(const Expression *expr) {return IsExpressionPointer(expr) ? expr->LineNumber() : 0;}
    static uint16_t SColumn(const Expression *expr) {return IsExpressionPointer(expr) ? expr->Column() : 0;}

	virtual uint16_t LineNumber() const = 0;
	virtual uint16_t Column() const = 0;
	virtual uint16_t LastLineNumber() const = 0;
	virtual uint16_t LastColumn() const = 0;
	virtual void SetLocation(uint16_t lineNumber, uint16_t column) = 0;

protected:
	Expression() {}
	~Expression() {}

private:
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment) = 0;

	static Error PopOperators(Array<Token> &operators, ExpressionPtrOwnerArray &terms, Token *current);

	static Expression *GenerateIntConstant(int i);
	static Expression *GenerateFloatConstant(float f);
};

class ExpressionPtrOwnerArray : public PtrArray<Expression, 3>
{
public:
	~ExpressionPtrOwnerArray() { resize(0); }
	virtual void resize(size_t size);
};

class ExpressionWithLocation : public Expression
{
public:
	~ExpressionWithLocation() {}

	uint16_t LineNumber() const { return _lineNumber; }
	uint16_t Column() const { return _column; }
	uint16_t LastLineNumber() const { return _lineNumber; }
	uint16_t LastColumn() const { return _column; }
	void SetLocation(uint16_t lineNumber, uint16_t column) { _lineNumber = lineNumber; _column = column; }

protected:
	ExpressionWithLocation() {}

	uint16_t _lineNumber;
	uint16_t _column;
};

class UnaryExpression : public ExpressionWithLocation
{
public:
	UnaryExpression(Token::TokenType op, Expression *operand) : _operator(op), _operand(operand) {}
	virtual ~UnaryExpression() 
	{
		Expression::Delete(_operand);
	}
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);
	virtual Error Build(TokenReader *, CompileEnvironment &) {return Error(NotImplemented, 0, 0);}

#ifdef _DEBUG
	virtual void Dump();
#endif

	void SetOperatorLocation(uint16_t lineNumber, uint16_t column) { _lineNumber = lineNumber; _column = column; }
	virtual uint16_t LastLineNumber() const { return _operand->LastLineNumber(); }
	virtual uint16_t LastColumn() const { return _operand->LastColumn(); }

private:
	Token::TokenType _operator;
	Expression *_operand;
};

class BinaryExpression : public ExpressionWithLocation
{
public:
	BinaryExpression(Expression *left, Token::TokenType op, Expression *right) :
		_executor(OperatorToExecutionMethod(op)), 
#ifdef _DEBUG
		_operator(op),
#endif
		_left(left), _right(right) {}
	virtual ~BinaryExpression() 
	{
		Expression::Delete(_left);
		Expression::Delete(_right);
	}
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);
	virtual Error Build(TokenReader *, CompileEnvironment &) {return Error(NotImplemented, 0, 0);}

#ifdef _DEBUG
	virtual void Dump();
#endif

	void SetOperatorLocation(uint16_t lineNumber, uint16_t column) { _lineNumber = lineNumber; _column = column; }
	virtual uint16_t LastLineNumber() const 
	{
		if (Expression::IsExpressionPointer(_right))
			return _right->LastLineNumber();
		else
			return LineNumber();
	}
	virtual uint16_t LastColumn() const 
	{ 
		if (Expression::IsExpressionPointer(_right))
			return _right->LastColumn();
		else
			return Column();
	}

#pragma push_macro("new")
#undef new
	void *operator new(size_t) {return _pool.Allocate();}
	void *operator new(size_t, int, const char *, int) {return _pool.Allocate(); }
	void operator delete(void *p) { _pool.Free((BinaryExpression *) p); }
	void operator delete(void *p, int, const char *, int) { _pool.Free((BinaryExpression *) p); }
#pragma pop_macro("new")

private:
	typedef Error (TypeBehaviors::*ExecutionMethod)(const Value &, const Value &, uint16_t, uint16_t, Value &) const;
	static ExecutionMethod OperatorToExecutionMethod(Token::TokenType op);
	ExecutionMethod _executor;
#ifdef _DEBUG
	Token::TokenType _operator;
#endif
	Expression *_left, *_right;

	static PoolAllocator<BinaryExpression> _pool;
};

#endif
