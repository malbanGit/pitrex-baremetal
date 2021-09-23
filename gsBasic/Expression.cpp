
#include "Expression.h"
#include "Reader.h"
#include "CallSpec.h"
#include "VarSpec.h"
#include "Constant.h"
#include "Value.h"
#include "ArrayInitializer.h"
#include <stdio.h>

PoolAllocator<BinaryExpression> BinaryExpression::_pool;

Error Expression::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment, Expression *&expression,
	uint16_t lineNumber, uint16_t column)
{
	Array<Token> operators;
	ExpressionPtrOwnerArray terms;
	Token current, last;

	expression = null;

	// Push an lParen on the calculationStack
	Token lParen(Token::LParen, lineNumber, column, 0);
	operators.push_back(lParen);
	int parenNesting = 0;

	bool validToken = tokenReader->Next(current);
	if (validToken)
	{
		lineNumber = current.LineNumber();
		column = current.Column();

		do
		{
			// If this is an operator
			if (Token::IsOperator(current.Type()))
			{
				// Check for unary + and -
				if (current.Type() == Token::PLUS || current.Type() == Token::MINUS)
				{
					// If this is a + or - at the beginning of the expression or after an operator (other than a right paren/brace/bracket)
					if (last.Type() == Token::Uninitialized ||
						(last.Type() < Token::EndOfOperators && !Token::IsRightBracket(last.Type())))
					{
						// It's unary
						current.ModifyType(current.Type() == Token::PLUS ? Token::UNARYPLUS : Token::UNARYMINUS);
					}
				}

				Error error = PopOperators(operators, terms, &current);
				if (error)
					return error;

				// If the token is an RParen
				if (current.Type() == Token::RParen)
				{
					// If there's no LParen to match this RParen
					if (--parenNesting < 0)
					{
						// We've gone beyond the end of the expression, e.g. the RParen
						// belongs to a CallSpec. Return the RParen and exit
						tokenReader->Prev();
						break;
					}
				}
				// Else push the new operator
				else
				{
					operators.push_back(current);
					if (current.Type() == Token::LParen)
						++parenNesting;
				}
			}

			// Else (not an operator)
			else
			{
				// If the previous token wasn't an operator or it was a right paren
				if (last.Type() != Token::Uninitialized && (last.Type() > Token::EndOfOperators || last.Type() == Token::RParen))
				{
					// We might've encountered a comma (e.g. we're in an expression list) or
					// an error (e.g. there's an identifier where there should not be).
					// Put the token back and bail out.
					tokenReader->Prev();
					break;
				}

				// If this is an identifier
				if (current.Type() == Token::Identifier)
				{
					// It's a variable, an array reference, or a function call. Look ahead to
					// see which
					Token next;
					Expression *spec;
					// Look ahead to see if there an lparen
					bool nextValid = tokenReader->Next(next);
					tokenReader->Prev();
					// Back up so the next token will be the identifier, and compile
					tokenReader->Prev();
					Error error;
					// If there is, it's a function call. Otherwise, it's a variable
					if (nextValid && next.Type() == Token::LParen)
					{
						spec = new CallSpec();
						error = spec->Build(tokenReader, compileEnvironment);
					}
					else
					{
						VarSpec *vs;
						error = VarSpec::Build(tokenReader, compileEnvironment, current.LineNumber(), current.Column(), VarSpec::LValue, &vs);
						spec = vs;
					}
					if (error)
					{
						Expression::Delete(spec);
						return error;
					}
					terms.push_back(spec);
				}

				// Else if it's a constant
				else if (current.Type() == Token::Int || current.Type() == Token::Float || current.Type() == Token::String ||
					current.Type() == Token::BuiltInConstant)
				{
					if (current.Type() == Token::Int)
					{
						int i = current.ValueInt();
						if (operators.size() && operators.back().Type() == Token::UNARYMINUS)
						{
							i = -i;
							operators.pop_back();
						}
						terms.push_back(GenerateIntConstant(i));
					}
					else if (current.Type() == Token::Float)
					{
						float f = current.ValueFloat();
						if (operators.size() && operators.back().Type() == Token::UNARYMINUS)
						{
							f = -f;
							operators.pop_back();
						}
						terms.push_back(GenerateFloatConstant(f));
					}
					else if (current.Type() == Token::String)
						terms.push_back(new ConstantString((current.ValueString())));
					else if (current.Type() == Token::BuiltInConstant)
					{
						const Value &v = current.ValueBuiltInConstant();
						if (v.IsInteger())
						{
							int i = v.Integer();
							if (operators.size() && operators.back().Type() == Token::UNARYMINUS)
							{
								i = -i;
								operators.pop_back();
							}
							terms.push_back(GenerateIntConstant(i));
						}
						else if (v.IsFloat())
						{
							float f = v.Float();
							if (operators.size() && operators.back().Type() == Token::UNARYMINUS)
							{
								f = -f;
								operators.pop_back();
							}
							terms.push_back(GenerateFloatConstant(f));
						}
						else if (v.IsString())
							terms.push_back(new ConstantString(v.String()));
						else if (v.IsUninitialized())
							terms.push_back(new ConstantNil());
                        else
                            terms.push_back(Constant::Create(v));
					}
				}

				// Else if it's an array initializer, i.e. {<expression list>}
				else if (current.Type() == Token::LBrace)
				{
					tokenReader->Prev();
					ArrayInitializer *ai = new ArrayInitializer();
					Error error = ai->Build(tokenReader, compileEnvironment);
					if (error)
					{
						delete ai;
						return error;
					}
					terms.push_back(ai);
				}

				else
				{
					return Error(UnrecognizedToken, current.LineNumber(), current.Column());
				}

				// If the top operator is unary, pop the operand and operator
				if (operators.back().Type() > Token::UnaryOps && operators.back().Type() < Token::EndOfUnaryOps)
				{
					Error error = PopOperators(operators, terms, &operators.back());
					if (error)
						return error;
				}
			}

			last = current;
			validToken = tokenReader->Next(current);
		} while (validToken);
	}

	if (terms.size() == 0)
	{
		return Error(EmptyExpression, lineNumber, column);
	}

	Token rParen(Token::RParen, lineNumber, column, 0);
	Error error = PopOperators(operators, terms, &rParen);

	// If there are any operators left or there's not exactly one term, it's an error
	if (error)
		return error;
	else if (operators.size() > 0 || terms.size() != 1)
	{
		return Error(InvalidExpression, last.LineNumber(), last.Column());
	}

	// Copy the expression tree we've produced.
	expression = terms.back();
	terms.back() = null;
	if (IsExpressionPointer(expression) && expression->LineNumber() == 0)
	{
		expression->SetLocation(lineNumber, column);
	}
	terms.pop_back();

	return NoError;
}

Error Expression::Build(int i, Expression *&expression)
{
	expression = GenerateIntConstant(i);
	return NoError;
}

Error Expression::Build(const char *s, Expression *&expression)
{
	expression = new ConstantString(s);
	return NoError;
}

void Expression::Delete(Expression *expr)
{
	if ((long(expr) & 3) == ExpressionPointer)
		delete expr;
}

Expression *Expression::GenerateIntConstant(int i)
{
	// If we can encode it in a single int
	uint32_t highBits = i & 0xe0000000;
	if (highBits == 0 || highBits == 0xe0000000)
		return (Expression *) ((i << 2) | IntConstant);
	else
		return new ConstantInt(i);
}

Expression *Expression::GenerateFloatConstant(float f)
{
	union
	{ 
		int i;
		float f;
	} u;
	u.f = f;
    // See if we can drop the 2 high bits of the unbiased exponent
    uint32_t sign = u.i & 0x80000000;
    uint32_t exponentExcess127 = ((u.i >> 23) & 0xff);
    int32_t exponent = int32_t(exponentExcess127) - 127;
    uint32_t mantissa = u.i & 0x7fffff;
    int32_t upperExponent = exponent >> 5;
	if (upperExponent == 0 || upperExponent == -1)
	{
		uint32_t encoding = (sign | ((((exponent & 0x3f) << 23) | mantissa) << 2) | FloatConstant);
		return (Expression *) encoding;
	}
	else
		return new ConstantFloat(u.f);
}



/**
 * When we've encountered an operator, we need to pop higher-priority operators off the calculationStack,
 * combine them with their operands from the 'term' calculationStack, assemble them into a (sub-)expression,
 * and push the result onto the 'term' calculationStack
 */
Error Expression::PopOperators(Array<Token> &operators, ExpressionPtrOwnerArray &terms, Token *current)
{
	// While there are higher priority operators than the current one
	// (but binary operators associate right and unary operator associate left)
	while (!operators.empty() && 
		Token::OperatorPrecedence(current->Type()) >= Token::OperatorPrecedence(operators.back().Type()) &&
		(!Token::IsUnaryOperator(current->Type()) || !Token::IsUnaryOperator(operators.back().Type())))
	{
		// Pop the operator at the top of the calculationStack
		Token poppedOp = operators.back();
		operators.pop_back();

		// If it's an LParen
		if (poppedOp.Type() == Token::LParen)
		{
			// If the current token isn't an RParen, return the LParen
			if (current->Type() != Token::RParen)
			{
				operators.push_back(poppedOp);
			}
			// Stop popping operators
			break;
		}

		// Else if it's a unary operator
		else if (Token::IsUnaryOperator(poppedOp.Type()))
		{
			// If there's nothing on the terms calculationStack, it's an error
			if (terms.empty())
			{
				return Error(InvalidExpression, current->LineNumber(), current->Column());
			}
			Expression *poppedTerm = terms.back();
			terms.back() = null;
			terms.pop_back();

			UnaryExpression *unaryExpression = new UnaryExpression(poppedOp.Type(), poppedTerm);
			unaryExpression->SetOperatorLocation(poppedOp.LineNumber(), poppedOp.Column());
			terms.push_back(unaryExpression);
		}

		// Else (binary operator)
		else
		{
			if (terms.size() <= 1)
			{
				return Error(InvalidExpression, current->LineNumber(), current->Column());
			}
			Expression *term2 = terms.back();
			terms.back() = null;
			terms.pop_back();
			Expression *term1 = terms.back();
			terms.back() = null;
			terms.pop_back();
			BinaryExpression *binaryExpression = new BinaryExpression(term1, poppedOp.Type(), term2);
			binaryExpression->SetOperatorLocation(poppedOp.LineNumber(), poppedOp.Column());
			terms.push_back(binaryExpression);
		}
	}

	return NoError;
}

Error Expression::Execute(Expression *expr, Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	int expressionType = long(expr) & 3;
	switch (expressionType)
	{
	case ExpressionPointer:
		return expr->Execute(exec, runTimeEnvironment);
	case IntConstant:
	{
		// The integer is encoded in the high 30 bits by shifting it
		runTimeEnvironment.calculationStack.push_back(int(long(expr) >> 2));
		return NoError;
	}
	case FloatConstant:
	{
		// The float is encoded in the high 30 bits; the high 3 bits in the
        // unbiased exponent had to be the same so that it would be safe to
        // discard two of them
		union
		{
			int i;
			float f;
		} u;
        // Get the unbiased exponent with its two high bits missing
		uint32_t encoding = int(expr);
        uint32_t exponent = (encoding >> 25) & 0x3f;
        // Extend bit 5 to be the sign bit
        if (exponent & 0x20)
            exponent |= 0xffffffc0;
		u.i = (encoding & 0x80000000) | // sign bit
			((exponent + 127) << 23) |
			((encoding >> 2) & 0x7fffff); // mantissa;
		runTimeEnvironment.calculationStack.push_back(u.f);
		return NoError;
	}
	default:
		return NoError;
	}
}

VarSpec *Expression::CastToVarSpec(Expression *expr)
{
	int expressionType = long(expr) & 3;
	if (expressionType == ExpressionPointer)
		return dynamic_cast<VarSpec *>(expr);
	return null;
}
Error UnaryExpression::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	Error error;

	size_t stackSize = runTimeEnvironment.calculationStack.size();
	error = Expression::Execute(_operand, exec, runTimeEnvironment);
	if (_operator == Token::IsNil && error.Message() == UninitializedVariable &&
		dynamic_cast<VarSpec *>(_operand))
	{
		runTimeEnvironment.calculationStack.push_back(1);
		return NoError;
	}
	if (error)
		return error;
	if (runTimeEnvironment.calculationStack.size() != stackSize + 1)
		return Error(MustReturnValue, _operand->LineNumber(), _operand->Column());

	Value &right = runTimeEnvironment.calculationStack.back();

	// Handle the unary operators
	switch (_operator)
	{
	case Token::UNARYPLUS:
		error = right.TypeBehaviors()->UnaryPlus(right, LineNumber(), Column(), right);
		break;

	case Token::UNARYMINUS :
		error = right.TypeBehaviors()->UnaryMinus(right, LineNumber(), Column(), right);
		break;

	case Token::Not :
		error = right.TypeBehaviors()->Not(right, LineNumber(), Column(), right);
		break;

	case Token::BitwiseNOT :
		error = right.TypeBehaviors()->BitwiseNOT(right, LineNumber(), Column(), right);
		break;

	case Token::IsNil :
		right = 0;
		break;

	// ParenGroup is for array initialization, so that a = { ( {1, 2, 3} ) } will put an array in a[1]
	case Token::ParenGroup :
		break;

	default:
		runTimeEnvironment.calculationStack.pop_back();
		error = Error(NotImplemented, LineNumber(), Column());
		break;
	}

	return error;
}

Error BinaryExpression::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	Error error;

	size_t stackSize = runTimeEnvironment.calculationStack.size();
	error = Expression::Execute(_left, exec, runTimeEnvironment);
	if (error)
		return error;
	if (runTimeEnvironment.calculationStack.size() != stackSize + 1)
		return Error(MustReturnValue, _left->LineNumber(), _left->Column());

	Value *left = &runTimeEnvironment.calculationStack.back();

	// Optimize evaluation of AND and OR by determining whether the right operand
	// needs to be evaluated
	if (_executor == &TypeBehaviors::And)
	{
		if (left->TypeBehaviors()->ToBool(*left) == 0)
		{
			*left = 0;
			return NoError;
		}
	}
	else if (_executor == &TypeBehaviors::Or)
	{
		if (left->TypeBehaviors()->ToBool(*left) == 1)
		{
			*left = 1;
			return NoError;
		}
	}

	error = Expression::Execute(_right, exec, runTimeEnvironment);
	if (error)
	{
		runTimeEnvironment.calculationStack.pop_back();
		return error;
	}
	if (runTimeEnvironment.calculationStack.size() != stackSize + 2)
	{
		runTimeEnvironment.calculationStack.pop_back();
		return Error(MustReturnValue, _right->LineNumber(), _right->Column());
	}

	// The calculation stack might've been reallocated, so reload the pointer to left
	left = &runTimeEnvironment.calculationStack[runTimeEnvironment.calculationStack.size() - 2];
	Value *right = &runTimeEnvironment.calculationStack.back();

	error = (left->TypeBehaviors()->*_executor)(*left, *right, _lineNumber, _column, *left);

	runTimeEnvironment.calculationStack.pop_back();

	if (error)
		runTimeEnvironment.calculationStack.pop_back();

	return error;
}

#ifdef _DEBUG
void UnaryExpression::Dump()
{
	_operand->Dump();
	for (int i = 0; *operatorKeywords[i].word; ++i)
	{
		if (operatorKeywords[i].type == _operator)
		{
			puts(operatorKeywords[i].word);
			break;
		}
	}
}

void BinaryExpression::Dump()
{
	_left->Dump();
	_right->Dump();
	for (int i = 0; *operatorKeywords[i].word; ++i)
	{
		if (operatorKeywords[i].type == _operator)
		{
			puts(operatorKeywords[i].word);
			break;
		}
	}
}

#endif

BinaryExpression::ExecutionMethod BinaryExpression::OperatorToExecutionMethod(Token::TokenType op)
{
	switch (op)
	{
	case Token::MUL:
		return &TypeBehaviors::Mul;

	case Token::DIV:
		return &TypeBehaviors::Div;

	case Token::MOD:
		return &TypeBehaviors::Mod;

	case Token::PLUS:
		return &TypeBehaviors::Plus;

	case Token::MINUS:
		return &TypeBehaviors::Minus;

	case Token::LE:
		return &TypeBehaviors::LessThanOrEqual;

	case Token::GE:
		return &TypeBehaviors::GreaterThanOrEqual;

	case Token::LT:
		return &TypeBehaviors::LessThan;

	case Token::GT:
		return &TypeBehaviors::GreaterThan;

	case Token::EQ:
	case Token::EQCmp :
		return &TypeBehaviors::Equal;

	case Token::NE:
		return &TypeBehaviors::NotEqual;

	case Token::And:
		return &TypeBehaviors::And;

	case Token::Xor:
		return &TypeBehaviors::Xor;

	case Token::Or:
		return &TypeBehaviors::Or;

	case Token::BitwiseAND :
		return &TypeBehaviors::BitwiseAND;

	case Token::BitwiseXOR :
		return &TypeBehaviors::BitwiseXOR;

	case Token::BitwiseOR :
		return &TypeBehaviors::BitwiseOR;

	default:
		return null;
	}
}

void ExpressionPtrOwnerArray::resize(size_t newSize)
{
	size_t oldSize = size();
	for (size_t i = newSize; i < oldSize; ++i)
		Expression::Delete(operator[](i));
	PtrArray<Expression, 3>::resize(newSize);
}
