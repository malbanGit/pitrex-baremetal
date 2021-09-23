#include "ArrayInitializer.h"
#include "Value.h"
#include "Environment.h"

PoolAllocator<ArrayInitializer> ArrayInitializer::_pool;

Error ArrayInitializer::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	memset(_dimensions, 0, sizeof(_dimensions));

	// Eat the opening brace
	Token token;
	tokenReader->Next(token);

	uint16_t lineNumber = token.LineNumber(), column = token.Column();
	SetLocation(lineNumber, column);

	Error error =_expressionList.Build(tokenReader, compileEnvironment, true, lineNumber, column);
	if (!error)
	{
		// Get the next token. It should be a right brace '}'
		if (tokenReader->Next(token))
		{
			lineNumber = token.LineNumber();
			column = token.Column();
			if (token.Type() != Token::RBrace)
				error = Error(MissingEndBrace, lineNumber, column);
			else
			{
				// Find out how many dimensions this array initializer has, and their sizes
				ErrorCode errorCode = GetDimensions(_dimensions, 0);
				if (errorCode)
					error = Error(errorCode, _expressionList.LineNumber(), _expressionList.Column());
				else
				{
					_dimensionCount = MAX_ARRAY_DIMENSIONS;
					while (_dimensionCount && _dimensions[_dimensionCount - 1] == 0)
						--_dimensionCount;
				}
			}
		}
		else
        {
            uint16_t line = _expressionList.LastLineNumber(), col = _expressionList.LastColumn();
            if (line == 0)
            {
                line = lineNumber;
                col = column;
            }
			error = Error(MissingEndBrace, line, col);
        }
	}

	return error;
}

Error ArrayInitializer::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	// Create an array value 
	Value va(_dimensions, _dimensionCount);

	// Evaluate the expression list and put the results in the array
	int arrayOffset = 0;
	Error error = CopyValues(exec, runTimeEnvironment, va, &_expressionList, 0, &arrayOffset);
	if (error)
		return error;

	runTimeEnvironment.calculationStack.push_back(va);

	return error;
}

// Find out what the dimensions of the array initializer are
ErrorCode ArrayInitializer::GetDimensions(uint16_t *dimensions, int dimension) const
{
	uint16_t dim = (uint16_t)_expressionList.Expressions().size();
	if (dim > *dimensions)
		*dimensions = dim;
	for (int i = 0; i < dim; ++i)
	{
		const Expression *expr = _expressionList.Expressions()[i];
		if (Expression::IsExpressionPointer(expr) && dynamic_cast<const ArrayInitializer *>(expr))
		{
			if (dimension + 1 < MAX_ARRAY_DIMENSIONS)
			{
				const ArrayInitializer *ai = (const ArrayInitializer *) expr;
				ErrorCode errorCode = ai->GetDimensions(dimensions + 1, dimension + 1);
				if (errorCode)
					return errorCode;
			}
			else
				return TooManyDimensions;
		}
	}

	return 0;
}

Error ArrayInitializer::CopyValues(Executive &exec, RunTimeEnvironment &runTimeEnvironment, 
	Value &va, ExpressionList *expressionList, int dimension, int *arrayOffset)
{
	// Get the number of elements that are initialized: either the dimension of the array or the
	// number of expressions in the expression list, whichever is smaller
	int initializedCount = _dimensions[dimension];
	if (expressionList == null)
		initializedCount = 0;
	else if ((int) expressionList->Expressions().size() < initializedCount)
		initializedCount = expressionList->Expressions().size();
	int i;
	// Initialize the array items that have corresponding expressions
	for (i = 0; i < initializedCount; ++i)
	{
		Error error;
		Expression *expr = expressionList->Expressions()[i];
		// Get the array item to initialize
		Value &value = va[*arrayOffset];
		// If there is another dimension to the array, then we need to initialize an
		// entire row, not just a scalar
		if (dimension < (int) va.DimensionsSize() - 1)
		{
			// The expression needs to be an array initializer
			if (Expression::IsExpressionPointer(expr) && dynamic_cast<const ArrayInitializer *>(expr) == null)
				return Error(MustBeArrayInitializer, expr->LineNumber(), expr->Column());
			error = CopyValues(exec, runTimeEnvironment, va, &((ArrayInitializer *) expr)->_expressionList, dimension + 1, arrayOffset);
		}
		// Else (no more dimensions)
		else
		{
			// Evaluate the expression and store it in the array item
			error = Expression::Execute(expr, exec, runTimeEnvironment);
			if (!error)
			{
				value = runTimeEnvironment.calculationStack.back();
				runTimeEnvironment.calculationStack.pop_back();
			}
			else
				value = 0;
			++*arrayOffset;
		}
		if (error)
			return error;
	}

	// Initialize the rest of the array items to zero
	for (i = initializedCount; i < _dimensions[dimension]; ++i)
	{
		Value &value = va[*arrayOffset];

		// If there is another dimension to the array, then we need to initialize an
		// entire row, not just a scalar
		if (dimension < (int) va.DimensionsSize() - 1)
		{
			Error error = CopyValues(exec, runTimeEnvironment, va, null, dimension + 1, arrayOffset);
			if (error)
				return error;
		}
		// Else (no more dimensions) initialize the array item to zero
		else
		{
			value = 0;
			++*arrayOffset;
		}

	}

	return NoError;
}