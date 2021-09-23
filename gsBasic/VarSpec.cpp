
#include "VarSpec.h"
#include "Reader.h"
#include "Token.h"
#include "ExpressionList.h"
#include "Executive.h"
#include <stdio.h> 

PoolAllocator<VarSpec> VarSpec::_pool;

PoolAllocator<VarSpecWithElements> VarSpecWithElements::_vsepool;

Error VarSpec::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment, uint16_t lineNumber, uint16_t column, ReferenceType referenceType, VarSpec **pvarSpec)
{
	*pvarSpec = NULL;
	VarSpec *varSpec = NULL;

	Token token;
	tokenReader->Next(token);
	if (token.Type() != Token::Identifier)
	{
		return Error(IllegalVariableSpec, token.LineNumber(), token.Column());
	}
	String varName = token.ValueIdentifier();

	if (tokenReader->Next(token))
	{
		// If there's an array bracket or a dot
		if (token.Type() == Token::LBracket || token.Type() == Token::DOT)
		{
			VarSpecWithElements *vse = new VarSpecWithElements();
			*pvarSpec = varSpec = vse;

			tokenReader->Prev();

			while (tokenReader->Next(token))
			{
				ElementSpec *spec = null;

				// If there's an array index spec
				if (token.Type() == Token::LBracket)
				{
					// Read the expression list of array indexes
					if (referenceType == ReferenceType::DimReference)
						spec = new DimArrayIndices();
					else
						spec = new ArrayIndices();
					Error error = spec->Build(tokenReader, compileEnvironment, token.LineNumber(), token.Column());
					if (error)
					{
						delete spec;
						return error;
					}
					Token endBracket;
					if (!tokenReader->Next(endBracket) || endBracket.Type() != Token::RBracket)
					{
						error = Error(MissingEndBracket, spec->LineNumber(), spec->Column());
						delete spec;
						return error;
					}
				}

				// Else if there's a dot for a struct spec
				else if (token.Type() == Token::DOT)
				{
					// Get the member name
					spec = new StructMember();
					Error error = spec->Build(tokenReader, compileEnvironment, token.LineNumber(), token.Column());
					if (error)
					{
						delete spec;
						return error;
					}
				}

				else
				{
					tokenReader->Prev();
					break;
				}

				// Save the spec
				if (vse->_spec == null)
					vse->_spec = spec;
				else
					vse->_spec->AppendSpec(spec);
			}
		}
	}
	if (varSpec == NULL)
	{
		*pvarSpec = varSpec = new VarSpec();
		tokenReader->Prev();
	}

	varSpec->SetLocation(lineNumber, column);
	bool includeGlobalScope = referenceType != DimReference;
	bool allocateIfNotFound = referenceType != RValue;
	ErrorCode error = compileEnvironment.FindOrAllocateVariable(varName, includeGlobalScope, allocateIfNotFound, &varSpec->_variable);
	if (error)
		return Error(error, lineNumber, column);

	return NoError;
}

ErrorCode VarSpec::Build(const String &varName, CompileEnvironment &compileEnvironment, ReferenceType referenceType, VarSpec **varSpec)
{
	*varSpec = new VarSpec();
	return compileEnvironment.FindOrAllocateVariable(varName, referenceType != DimReference, referenceType != RValue, &(*varSpec)->_variable);
}

/**
 * VarSpecs can be evaluated as rvalues, lvalues, or definitions as in a DIM statement.
 * Here, we evaluate the rvalue and push it on the calculation stack
 */
Error VarSpec::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	Value *pvalue;
	Error error = Reference(exec, runTimeEnvironment, &pvalue);
	if (error)
    {
        return error;
      
    }

	if (pvalue->IsByRef())
		pvalue = pvalue->Dereference();

	const Value &value = *pvalue;
	if (value.IsUninitialized())
    {
        return Error(UninitializedVariable, LineNumber(), Column());
    }

	// Push the variable's value on the calculation stack
	if (value.IsByteReference())
      
		runTimeEnvironment.calculationStack.push_back(value.ByteValue());
	else
		runTimeEnvironment.calculationStack.push_back(value);

	return NoError;
}

// Push a reference to the variable onto the calculation stack
Error VarSpec::ByRefExecute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	Value *pvalue;
	Error error = Reference(exec, runTimeEnvironment, &pvalue);
	if (error)
		return error;

	runTimeEnvironment.calculationStack.push_back(Value(pvalue));

	return NoError;
}

/**
* Dimension the array at runtime. Called by StatementDim::Execute
*/
Error VarSpec::Dim(Executive &, RunTimeEnvironment &)
{
	return NoError;
}

/**
* Dimension the array at runtime. Called by StatementDim::Execute
*/
Error VarSpecWithElements::Dim(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	if (_spec)
	{
		const static Value dummy;
		uint16_t dims[MAX_ARRAY_DIMENSIONS];
		size_t dimsSize;
		Error error = ((DimArrayIndices *) _spec)->ResolveIndex(dummy, dims, &dimsSize, exec, runTimeEnvironment);
		if (error)
			return error;
		// Destroy the old value before we allocate a new value
		runTimeEnvironment.SetValue(_variable, dummy);
		runTimeEnvironment.SetValue(_variable, Value(dims, dimsSize));
	}
	// Else (DIMing a scalar) don't need to do anything

	return NoError;
}

/**
 * VarSpecs can be evaluated as rvalues, lvalues, or definitions as in a DIM statement.
 * Here, we get the lvalue for an assignment
 * Called at execution time to get a reference to the variable's value.
 * We want to be able to change the Value a variable or array
 * element has, so we need a Value *. We'll write the Value * into pvalue,
 * so we need pvalue pased in as a Value **. 
 */
Error VarSpec::Reference(Executive &, RunTimeEnvironment &runTimeEnvironment, Value **pvalue)
{
	*pvalue = &runTimeEnvironment.GetValueReference(_variable);
	if ((*pvalue)->IsByRef())
		*pvalue = (*pvalue)->Dereference();

	return NoError;
}

/**
 * VarSpecs can be evaluated as rvalues, lvalues, or definitions as in a DIM statement.
 * Here, we get the lvalue for an assignment
 * Called at execution time to get a reference to the variable's value.
 * We want to be able to change the Value a variable or array
 * element has, so we need a Value *. We'll write the Value * into pvalue,
 * so we need pvalue pased in as a Value **.
 */
Error VarSpecWithElements::Reference(Executive &exec, RunTimeEnvironment &runTimeEnvironment, Value **pvalue)
{
	static Value persistentReturnValue;

	*pvalue = &runTimeEnvironment.GetValueReference(_variable);
	if ((*pvalue)->IsByRef())
		*pvalue = (*pvalue)->Dereference();

	int index = 0;
	ElementSpec *spec = _spec;
	while (spec)
	{
		Error error = spec->ResolveIndex(**pvalue, &index, exec, runTimeEnvironment);
		if (error)
		{
			if (error.Column() == 0)
				return Error(error.Message(), LineNumber(), Column());
			return error;
		}

		// If we're indexing a string
		if ((*pvalue)->IsString())
		{
			persistentReturnValue = (uint8_t *) &(*pvalue)->String()[index];
			*pvalue = &persistentReturnValue;
		}
		// Else (indexing an array or selecting the member of a struct)
		else
			*pvalue = &(**pvalue)[index];

		spec = spec->_next;
	}

	return NoError;
}

const Value &VarSpec::GetValue(RunTimeEnvironment &runTime) const
{
	return runTime.GetValue(_variable);
}

void VarSpec::SetValue(const Value &value, RunTimeEnvironment &runTime)
{
	runTime.SetValue(_variable, value);
}

#ifdef _DEBUG
void VarSpec::Dump() 
{
	puts("VarSpec"); 
}
#endif

Error VarSpecWithElements::ArrayIndices::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment, uint16_t lineNumber, uint16_t column)
{
	Error error = _arrayIndex.Build(tokenReader, compileEnvironment, false, lineNumber, column);
	if (!error && _arrayIndex.Expressions().size() > MAX_ARRAY_DIMENSIONS)
		error = Error(TooManyDimensions, lineNumber, column);
	return error;
}

Error VarSpecWithElements::ArrayIndices::ResolveIndex(const Value &value, int *index, Executive &exec, RunTimeEnvironment &runTimeEnvironment) const
{
	if (!value.IsArray() && !value.IsString())
	{
		return Error(NotAnArray, LineNumber(), Column());
	}
	uint16_t dims[MAX_ARRAY_DIMENSIONS];
	size_t dimsSize = _arrayIndex.size();

	for (size_t i = 0; i < _arrayIndex.size(); ++i)
	{
		Error error = Expression::Execute(_arrayIndex.Expressions()[i], exec, runTimeEnvironment);
		if (error)
			return error;
		Value v = runTimeEnvironment.calculationStack.back();
		runTimeEnvironment.calculationStack.pop_back();
		int dimIndex;
		if (v.IsNumeric())
		{
			v.TypeBehaviors()->ToInteger(v, &dimIndex);
			dims[i] = (uint16_t) dimIndex;
		}
		else
		{
			return Error(ArrayIndexMustBeInt, LineNumber(), Column());
		}
		if (dimIndex < ARRAY_BASE || dimIndex >= 65535)
		{
			return Error(ArrayIndexOutOfRange, LineNumber(), Column());
		}
	}

	// If it's an array
	if (value.IsArray())
	{
		// Get the dimensions of the array as it was created
		if (value.DimensionsSize() != dimsSize)
			return Error(WrongNumberOfIndexes, LineNumber(), Column());

		// Calculate the index
		const uint16_t *dimensions = value.Dimensions();
		*index = 0;
		for (size_t i = 0; i < dimsSize; ++i)
		{
			// Handle multidimensional arrays by multiplying the index calculated so far by the
			// size of the next dimension. This will produce the equivalent index into a
			// one dimensional array
			if (i > 0)
				*index *= dimensions[i];

			if (dims[i] >= dimensions[i] + ARRAY_BASE)
			{
				return Error(ArrayIndexOutOfRange, LineNumber(), Column());
			}
			*index += dims[i] - ARRAY_BASE;
		}
	}

	// Else (it's a string)
	else
	{
		if (dimsSize != 1)
			return Error(WrongNumberOfIndexes, LineNumber(), Column());
		if (dims[0] >= value.String().length() + ARRAY_BASE)
			return Error(ArrayIndexOutOfRange, LineNumber(), Column());
		*index = dims[0] - ARRAY_BASE;
	}

	return NoError;
}

Error VarSpecWithElements::DimArrayIndices::ResolveIndex(const Value &, uint16_t *dims, size_t *dimsSize, Executive &exec, RunTimeEnvironment &runTimeEnvironment) const
{
	// Go through each of the array indexes that were in the Dim statement
	for (size_t i = 0; i < _arrayIndex.Expressions().size(); ++i)
	{
		// Calculate the size of the dimension
		Error error = Expression::Execute(_arrayIndex.Expressions()[i], exec, runTimeEnvironment);
		if (error)
			return error;
		Value dim = runTimeEnvironment.calculationStack.back();
		runTimeEnvironment.calculationStack.pop_back();
		// Dimension sizes must be integers
		if (!dim.IsInteger())
		{
			return Error(ArrayIndexMustBeInt, LineNumber(), Column());
		}
		int dimValue = dim.Integer();
		// Allow the first dimension to be 0, e.g. DIM a[0, 4]. But don't allow
		// other dimensions to be zero. An array with zero rows is useful if you're
		// building an array from scratch with AppendArrays
		if (dimValue < 0 || (dimValue == 0 && i > 0))
		{
			auto &expr = _arrayIndex.Expressions()[i];
			if (Expression::IsExpressionPointer(expr))
				return Error(ArrayIndexMustBePositive, expr->LineNumber(), expr->Column());
			else
				return Error(ArrayIndexMustBePositive, LineNumber(), Column());
		}
		dims[i] = (uint16_t) dimValue;
	}
	*dimsSize = _arrayIndex.Expressions().size();

	return NoError;
}

Error VarSpecWithElements::StructMember::Build(TokenReader *tokenReader, CompileEnvironment &, uint16_t lineNumber, uint16_t column)
{
	_lineNumber = lineNumber;
	_column = column;
	Token token;
	if (!tokenReader->Next(token))
		return Error(UnexpectedEndOfText, lineNumber, column);
	if (token.Type() != Token::Identifier)
		return Error(IllegalVariableSpec, token.LineNumber(), token.Column());
	_memberName = token.ValueIdentifier();
	return NoError;
}

Error VarSpecWithElements::StructMember::ResolveIndex(const Value &value, int *index, Executive &, RunTimeEnvironment &) const
{
	if (!value.IsStruct())
	{
		return Error(NotAStruct, LineNumber(), Column());
	}
	int memberIndex = value.MemberIndex(_memberName);
	if (memberIndex == -1)
		return Error(NotAMember, LineNumber(), Column());
	*index = memberIndex;
	return NoError;
}
