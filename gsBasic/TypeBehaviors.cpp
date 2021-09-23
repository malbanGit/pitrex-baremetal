
#include "TypeBehaviors.h"
#include "Value.h"
#include "Error.h"
#include "Array.h"
#include "Utilities.h"
#include <math.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <vectrex/vectrexInterface.h>
#include "vio.h"
#ifdef __cplusplus
}
#endif

TypeBehaviorsInt TypeBehaviorsInt::_singleton;
TypeBehaviorsFloat TypeBehaviorsFloat::_singleton;
TypeBehaviorsString TypeBehaviorsString::_singleton;
TypeBehaviorsArray TypeBehaviorsArray::_singleton;
TypeBehaviorsStruct TypeBehaviorsStruct::_singleton;
TypeBehaviorsHandle TypeBehaviorsHandle::_singleton;
TypeBehaviorsByRef TypeBehaviorsByRef::_singleton;
TypeBehaviorsByteReference TypeBehaviorsByteReference::_singleton;
TypeBehaviorsUninitialized TypeBehaviorsUninitialized::_singleton;



void TypeBehaviors::DeepCopy(const Value &src, Value &dest, bool) const 
{
	dest = src; 
}

Error TypeBehaviors::Not(const Value &value, uint16_t, uint16_t, Value &result) const
{
	result = !ToBool(value);
	return NoError;
}

Error TypeBehaviors::NotEqual(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	Value eq;
	Error error = Equal(arg1, arg2, line, column, eq);
	if (!error)
		eq.TypeBehaviors()->Not(eq, line, column, result);
	return error;
}

Error TypeBehaviors::GreaterThanOrEqual(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{ 
	return arg2.TypeBehaviors()->LessThanOrEqual(arg2, arg1, line, column, result); 
}

Error TypeBehaviors::GreaterThan(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	return arg2.TypeBehaviors()->LessThan(arg2, arg1, line, column, result); 
}

Error TypeBehaviors::And(const Value &arg1, const Value &arg2, uint16_t, uint16_t, Value &result) const
{
	result = ToBool(arg1) && arg2.TypeBehaviors()->ToBool(arg2);
	return NoError;
}

Error TypeBehaviors::Or(const Value &arg1, const Value &arg2, uint16_t, uint16_t, Value &result) const
{
	result = ToBool(arg1) || arg2.TypeBehaviors()->ToBool(arg2);
	return NoError;
}

Error TypeBehaviors::Xor(const Value &arg1, const Value &arg2, uint16_t, uint16_t, Value &result) const
{
	result = ToBool(arg1) ^ arg2.TypeBehaviors()->ToBool(arg2);
	return NoError;
}

void TypeBehaviors::Print(const Value &value) const
{
	String s = value.ToString();
	if (!s.empty())
		FPUTS2(value.ToString().c_str(), stdout);
}

/**************************************************************************
 * Integer Operators
 *************************************************************************/

void TypeBehaviorsInt::DestroyValue(Value *) const
{
}

void TypeBehaviorsInt::CopyTo(const Value &src, Value &dest) const
{
	dest._i = src._i;
}

Error TypeBehaviorsInt::UnaryPlus(const Value &value, uint16_t, uint16_t, Value &result) const
{
	result = value._i;
	return NoError;
}

Error TypeBehaviorsInt::UnaryMinus(const Value &value, uint16_t, uint16_t, Value &result) const
{
	result = -value._i; 
	return NoError; 
}

Error TypeBehaviorsInt::Minus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (arg2.IsInteger())
		result = arg1._i - arg2._i;
	else if (arg2.IsFloat())
		result = arg1._i - arg2._f;
	else
		return Error(IllegalOperand, line, column);
	return NoError;
}

Error TypeBehaviorsInt::Plus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (arg2.IsString())
	{
		result = arg1.ToString() + arg2.ToString();
		return NoError;
	}
	if (arg2.IsInteger())
		result = arg1._i + arg2._i;
	else if (arg2.IsFloat())
		result = arg1._i + arg2._f;
	else if (arg2.IsArray())
		return arg2.TypeBehaviors()->Plus(arg2, arg1, line, column, result);
	else
		return Error(IllegalOperand, line, column);
	return NoError;
}

Error TypeBehaviorsInt::Mul(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (arg2.IsInteger())
		result = arg1._i * arg2._i;
	else if (arg2.IsFloat())
		result = arg1._i * arg2._f;
	else if (arg2.IsArray())
		return arg2.TypeBehaviors()->Mul(arg2, arg1, line, column, result);
	else
		return Error(IllegalOperand, line, column);
	return NoError;
}

Error TypeBehaviorsInt::Div(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	int i1 = arg1._i;
	if (arg2.IsInteger())
	{
		int i2 = arg2._i;
		if (i2)
			result = i1 / i2;
		else
			return Error(DivideByZero, line, column);
	}
	else if (arg2.IsFloat())
	{
		float f2 = arg2._f;
		if (f2 != 0)
			result = i1 / f2;
		else
			return Error(DivideByZero, line, column);
	}
	else
		return Error(IllegalOperand, line, column);
	return NoError;
}

Error TypeBehaviorsInt::Mod(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	int i1 = arg1._i;
	if (arg2.IsInteger())
	{
		int i2 = arg2._i;
		if (i2)
			result = i1 % i2;
		else
			return Error(DivideByZero, line, column);
	}
	else if (arg2.IsFloat())
	{
		float f2 = arg2._f;
		if (f2 != 0)
			result = fmodf((float) i1, f2);
		else
			return Error(DivideByZero, line, column);
	}
	else
		return Error(IllegalOperand, line, column);
	return NoError;
}

Error TypeBehaviorsInt::Exponent(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	float f1 = (float) arg1._i;
	float f2;
	if (arg2.IsInteger())
	{
		f2 = (float) arg2._i;
	}
	else if (arg2.IsFloat())
	{
		f2 = arg2._f;
	}
	else
		return Error(IllegalOperand, line, column);
	result = (float) powf(f1, f2);
	return NoError;
}

Error TypeBehaviorsInt::BitwiseAND(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (!arg2.IsInteger())
		return Error(IllegalOperand, line, column);

	result = arg1.Integer() & arg2.Integer();
	return NoError;
}

Error TypeBehaviorsInt::BitwiseOR(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (!arg2.IsInteger())
		return Error(IllegalOperand, line, column);

	result = arg1.Integer() | arg2.Integer();
	return NoError;
}

Error TypeBehaviorsInt::BitwiseXOR(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (!arg2.IsInteger())
		return Error(IllegalXOROperand, line, column);

	result = arg1.Integer() ^ arg2.Integer();
	return NoError;
}

Error TypeBehaviorsInt::BitwiseNOT(const Value &arg1, uint16_t, uint16_t, Value &result) const
{
	result = ~arg1.Integer();
	return NoError;
}

Error TypeBehaviorsInt::LessThan(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (arg2.IsInteger())
		result = arg1._i < arg2._i;
	else if (arg2.IsFloat())
		result = arg1._i < arg2._f;
	else
		return Error(IllegalOperand, line, column);
	return NoError;
}

Error TypeBehaviorsInt::LessThanOrEqual(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (arg2.IsInteger())
		result = arg1._i <= arg2._i;
	else if (arg2.IsFloat())
		result = arg1._i <= arg2._f;
	else
		return Error(IllegalOperand, line, column);
	return NoError;
}

Error TypeBehaviorsInt::Equal(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (arg2.IsInteger())
		result = arg1._i == arg2._i;
	else if (arg2.IsFloat())
		result = arg1._i == arg2._f;
	else
		return Error(IllegalOperand, line, column);
	return NoError;
}

ErrorCode TypeBehaviorsInt::ToInteger(const Value &value, int *i) const
{
	*i = value._i;
	return 0;
}

ErrorCode TypeBehaviorsInt::ToFloat(const Value &value, float *f) const
{
	*f = (float) value._i;
	return 0;
}

int TypeBehaviorsInt::ToBool(const Value &value) const
{
	return value._i != 0;
}

String TypeBehaviorsInt::ToString(const Value &value) const
{
	return String::itoa(value._i);
}


/**************************************************************************
* Floating Point Operators
*************************************************************************/

void TypeBehaviorsFloat::DestroyValue(Value *) const
{
}

void TypeBehaviorsFloat::CopyTo(const Value &src, Value &dest) const
{
	dest._f = src._f;
}

Error TypeBehaviorsFloat::UnaryPlus(const Value &value, uint16_t, uint16_t, Value &result) const
{
	result = value._f;
	return NoError;
}

Error TypeBehaviorsFloat::UnaryMinus(const Value &value, uint16_t, uint16_t, Value &result) const
{
	result = -value._f;
	return NoError;
}

Error TypeBehaviorsFloat::Plus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	float f1 = arg1._f, f2;
	if (arg2.IsString())
	{
		result = arg1.ToString() + arg2.ToString();
		return NoError;
	}
	if (arg2.IsFloat())
		f2 = arg2._f;
	else if (arg2.IsInteger())
		f2 = (float) arg2._i;
	else if (arg2.IsArray())
		return arg2.TypeBehaviors()->Plus(arg2, arg1, line, column, result);
	else
		return Error(IllegalOperand, line, column);
	result = f1 + f2;
	return NoError;
}

Error TypeBehaviorsFloat::Minus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	float f1 = arg1._f, f2;
	if (arg2.IsFloat())
		f2 = arg2._f;
	else if (arg2.IsInteger())
		f2 = (float) arg2._i;
	else
		return Error(IllegalOperand, line, column);
	result = f1 - f2;
	return NoError;
}

Error TypeBehaviorsFloat::Mul(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	float f1 = arg1._f, f2;
	if (arg2.IsFloat())
		f2 = arg2._f;
	else if (arg2.IsInteger())
		f2 = (float) arg2._i;
	else if (arg2.IsArray())
		return arg2.TypeBehaviors()->Mul(arg2, arg1, line, column, result);
	else
		return Error(IllegalOperand, line, column);
	result = f1 * f2;
	return NoError;
}

Error TypeBehaviorsFloat::Div(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	float f1 = arg1._f, f2;
	if (arg2.IsFloat())
		f2 = arg2._f;
	else if (arg2.IsInteger())
		f2 = (float) arg2._i;
	else
		return Error(IllegalOperand, line, column);
	if (f2 != 0)
		result = (f1 / f2);
	else
		return Error(DivideByZero, line, column);
	return NoError;
}

Error TypeBehaviorsFloat::Mod(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	float f1 = arg1._f, f2;
	if (arg2.IsFloat())
		f2 = arg2._f;
	else if (arg2.IsInteger())
		f2 = (float) arg2._i;
	else
		return Error(IllegalOperand, line, column);
	if (f2 != 0)
		result = fmodf(f1, f2);
	else
		return Error(DivideByZero, line, column);
	return NoError;
}

Error TypeBehaviorsFloat::Exponent(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	float f1 = arg1._f;
	float f2;
	if (arg2.IsInteger())
	{
		f2 = (float) arg2._i;
	}
	else if (arg2.IsFloat())
	{
		f2 = arg2._f;
	}
	else
		return Error(IllegalOperand, line, column);
	result = powf(f1, f2);
	return NoError;
}

Error TypeBehaviorsFloat::LessThan(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	float f1 =arg1._f, f2;
	if (arg2.IsFloat())
		f2 = arg2._f;
	else if (arg2.IsInteger())
		f2 = (float) arg2._i;
	else
		return Error(IllegalOperand, line, column);
	result = (f1 < f2);
	return NoError;
}

Error TypeBehaviorsFloat::LessThanOrEqual(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	float f1 = arg1._f, f2;
	if (arg2.IsFloat())
		f2 =arg2._f;
	else if (arg2.IsInteger())
		f2 = (float) arg2._i;
	else
		return Error(IllegalOperand, line, column);
	result = (f1 <= f2);
	return NoError;
}

Error TypeBehaviorsFloat::Equal(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	float f1 = arg1._f, f2;
	if (arg2.IsFloat())
		f2 = arg2._f;
	else if (arg2.IsInteger())
		f2 = (float) arg2._i;
	else
		return Error(IllegalOperand, line, column);
	result = (f1 == f2);
	return NoError;
}

ErrorCode TypeBehaviorsFloat::ToInteger(const Value &value, int *i) const
{
	*i = (int) value._f;
	return 0;
}

ErrorCode TypeBehaviorsFloat::ToFloat(const Value &value, float *f) const
{
	*f = value._f;
	return 0;
}

int TypeBehaviorsFloat::ToBool(const Value &value) const
{
	return value._f != 0;
}

String TypeBehaviorsFloat::ToString(const Value &value) const
{
    float f = value._f;
	return String::ftoa(f);
}


/**************************************************************************
* String Operators
*************************************************************************/

void TypeBehaviorsString::DestroyValue(Value *value) const
{
	if (value->_s && --value->_s->refCnt == 0)
	{
		delete value->_s;
	}
	value->_s = null;
}

void TypeBehaviorsString::CopyTo(const Value &src, Value &dest) const
{
	dest._s = src._s;
	++dest._s->refCnt;
}

Error TypeBehaviorsString::UnaryPlus(const Value &, uint16_t line, uint16_t column, Value &) const
{
	return Error(MustBeNumeric, line, column);
}

Error TypeBehaviorsString::UnaryMinus(const Value &, uint16_t line, uint16_t column, Value &) const
{
	return Error(MustBeNumeric, line, column);
}

Error TypeBehaviorsString::Plus(const Value &arg1, const Value &arg2, uint16_t, uint16_t, Value &result) const
{
	result = arg1.ToString() + arg2.ToString();
	return NoError;
}

Error TypeBehaviorsString::Minus(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const
{
	return Error(IllegalOperand, line, column);
}

Error TypeBehaviorsString::Mul(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const
{
	return Error(IllegalOperand, line, column);
}

Error TypeBehaviorsString::Div(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const
{
	return Error(IllegalOperand, line, column);
}

Error TypeBehaviorsString::Mod(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const
{
	return Error(IllegalOperand, line, column);
}

Error TypeBehaviorsString::Exponent(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const
{
	return Error(IllegalOperand, line, column);
}

Error TypeBehaviorsString::LessThan(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (arg2.IsString())
	{
		result = arg1.ToString() < arg2.ToString();
		return NoError;
	}
	else
		return Error(IllegalOperand, line, column);
}

Error TypeBehaviorsString::LessThanOrEqual(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (arg2.IsString())
	{
		result = arg1.ToString() <= arg2.ToString();
		return NoError;
	}
	else
		return Error(IllegalOperand, line, column);
}

Error TypeBehaviorsString::Equal(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (arg2.IsString())
	{
		result = arg1.ToString() == arg2.ToString();
		return NoError;
	}
	else
		return Error(IllegalOperand, line, column);
}

ErrorCode TypeBehaviorsString::ToInteger(const Value &, int *) const
{
	return MustBeNumeric;
}

ErrorCode TypeBehaviorsString::ToFloat(const Value &, float *) const
{
	return MustBeNumeric;
}

int TypeBehaviorsString::ToBool(const Value &value) const
{
	return value.ToString().operator bool();
}

String TypeBehaviorsString::ToString(const Value &value) const
{
	return value._s->s;
}

/**************************************************************************
* Array Operators
*************************************************************************/

void TypeBehaviorsArray::DestroyValue(Value *value) const
{
	if (--(value->_d->refCnt) == 0)
	{
		Value::ArrayData_t *ad = (Value::ArrayData_t *) value->_d->data;
		delete [] ad->values;
		delete ad;
		delete value->_d;
	}
	value->_d = null;
}

void TypeBehaviorsArray::CopyTo(const Value &src, Value &dest) const
{
	dest._d = src._d;
	++dest._d->refCnt;
}

void TypeBehaviorsArray::DeepCopy(const Value &src, Value &dest, bool sourceWillBeDestroyed) const
{
	// If the source is going to be destroyed after this, and it's the only one pointing to the data
	if (sourceWillBeDestroyed && src._d->refCnt == 1)
		// We can just copy the reference
		CopyTo(src, dest);

	// Else (we really need to do a deep copy)
	else
	{
		Value::ArrayData_t *arrayData = (Value::ArrayData_t *) src._d->data;
		dest = Value(arrayData->dimensions, arrayData->dimensionCount);
		Value::ArrayData_t *destArrayData = (Value::ArrayData_t *) dest._d->data;
		for (size_t i = 0; i < arrayData->totalSize; ++i)
			arrayData->values[i].TypeBehaviors()->DeepCopy(arrayData->values[i], destArrayData->values[i], false);
	}
}

Error TypeBehaviorsArray::UnaryPlus(const Value &value, uint16_t, uint16_t, Value &result) const
{
	result = value;
	return NoError;
}

Error TypeBehaviorsArray::UnaryMinus(const Value &arg1, uint16_t line, uint16_t column, Value &result) const
{
	const uint16_t *dims = arg1.Dimensions();
	uint16_t dimSize = arg1.DimensionsSize();
	Value working(dims, dimSize);
	for (size_t i = 0; i < working.NumberOfElements(); ++i)
	{
		Error error = arg1[i].TypeBehaviors()->UnaryMinus(arg1[i], line, column, working[i]);
		if (error)
			return error;
	}
	result = working;

	return NoError;
}

Error TypeBehaviorsArray::Plus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (arg2.IsNumeric())
	{
		const uint16_t *dims = arg1.Dimensions();
		uint16_t dimSize = arg1.DimensionsSize();
		Value working(dims, dimSize);
		for (size_t i = 0; i < working.NumberOfElements(); ++i)
		{
			Error error = arg1[i].TypeBehaviors()->Plus(arg1[i], arg2, line, column, working[i]);
			if (error)
				return error;
		}
		result = working;

		return NoError;
	}

	else if (arg2.IsArray())
	{
		const uint16_t *dims = arg1.Dimensions(), *dims2 = arg2.Dimensions();
		uint16_t dimSize = arg1.DimensionsSize(), dimSize2 = arg2.DimensionsSize();
		if (dimSize == dimSize2 && memcmp(dims, dims2, dimSize * sizeof(uint16_t)) == 0)
		{
			Value working(dims, dimSize);
			for (size_t i = 0; i < working.NumberOfElements(); ++i)
			{
				Error error = arg1[i].TypeBehaviors()->Plus(arg1[i], arg2[i], line, column, working[i]);
				if (error)
					return error;
			}
			result = working;

			return NoError;
		}
	}

	return Error(IllegalOperand, line, column);
}

Error TypeBehaviorsArray::Minus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const 
{
	if (arg2.IsNumeric())
	{
		const uint16_t *dims = arg1.Dimensions();
		uint16_t dimSize = arg1.DimensionsSize();
		Value working(dims, dimSize);
		for (size_t i = 0; i < working.NumberOfElements(); ++i)
		{
			Error error = arg1[i].TypeBehaviors()->Minus(arg1[i], arg2, line, column, working[i]);
			if (error)
				return error;
		}
		result = working;

		return NoError;
	}

	else if (arg2.IsArray())
	{
		const uint16_t *dims = arg1.Dimensions(), *dims2 = arg2.Dimensions();
		uint16_t dimSize = arg1.DimensionsSize(), dimSize2 = arg2.DimensionsSize();
		if (dimSize == dimSize2 && memcmp(dims, dims2, dimSize * sizeof(uint16_t)) == 0)
		{
			Value working(dims, dimSize);
			for (size_t i = 0; i < working.NumberOfElements(); ++i)
			{
				Error error = arg1[i].TypeBehaviors()->Minus(arg1[i], arg2[i], line, column, working[i]);
				if (error)
					return error;
			}
			result = working;

			return NoError;
		}
	}

	return Error(IllegalOperand, line, column);
}

Error TypeBehaviorsArray::Mul(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const 
{
	if (arg2.IsNumeric())
	{
		const uint16_t *dims = arg1.Dimensions();
		uint16_t dimSize = arg1.DimensionsSize();
		Value working(dims, dimSize);
		for (size_t i = 0; i < working.NumberOfElements(); ++i)
		{
			Error error = arg1[i].TypeBehaviors()->Mul(arg1[i], arg2, line, column, working[i]);
			if (error)
				return error;
		}
		result = working;

		return NoError;
	}

	else if (arg2.IsArray())
	{
		const uint16_t *dims = arg1.Dimensions(), *dims2 = arg2.Dimensions();
		uint16_t dimSize = arg1.DimensionsSize(), dimSize2 = arg2.DimensionsSize();

		Value product;

		// If both arrays are one dimensional and have the same number of elements
		if (dimSize == 1 && dimSize2 == 1 && dims[0] == dims2[0])
		{
			Value total(0);
			for (size_t i = 0; i < dims[0]; ++i)
			{
				Error error = arg1[i].TypeBehaviors()->Mul(arg1[i], arg2[i], line, column, product);
				if (error)
					return error;
				error = total.TypeBehaviors()->Plus(total, product, line, column, total);
				if (error)
					return error;
			}
			
			result = total;
			return NoError;
		}

		// Else at least one of the arguments is a 2D array. The number of columns 
		// of the first must equal the number of rows of the second
		else if (dimSize <= 2 && dimSize2 <= 2 && dims[dimSize - 1] == dims2[0])
		{
			uint16_t leftRows, leftCols, rightCols, resultRows, resultCols;
			if (dimSize == 1)
			{
				leftRows = 1;
				leftCols = dims[0];
			}
			else
			{
				leftRows = dims[0];
				leftCols = dims[1];
			}
			if (dimSize2 == 1)
			{
				rightCols = dims2[0];
			}
			else
			{
				rightCols = dims2[1];
			}
			resultRows = leftRows;
			resultCols = rightCols;
			uint16_t resultDimSize, resultDims[2];
			if (resultRows == 1)
			{
				resultDimSize = 1;
				resultDims[0] = resultCols;
			}
			else
			{
				resultDimSize = 2;
				resultDims[0] = resultRows;
				resultDims[1] = resultCols;
			}

			// Create the working (ultimately the result) matrix to the correct size
			Value working(resultDims, resultDimSize);
			for (size_t i = 0; i < working.NumberOfElements(); ++i)
				working[i] = 0;

			// Go through all the rows of the left matrix
			for (size_t leftRow = 0; leftRow < leftRows; ++leftRow)
			{
				size_t leftRowIndex = leftRow * leftCols;
				size_t resultRowIndex = leftRow * resultCols;
				// Go through all the columns of the right matrix
				for (size_t rightCol = 0; rightCol < rightCols; ++rightCol)
				{
					working[resultRowIndex + rightCol] = 0;
					// Go through all the columns of the left matrix
					for (size_t leftCol = 0; leftCol < leftCols; ++leftCol)
					{
						// Multiply the (leftRow, leftCol) in the left matrix by the (rightRow, rightCol) in the right matrix
						Error error = arg1[leftRowIndex + leftCol].TypeBehaviors()->Mul(arg1[leftRowIndex + leftCol],
							arg2[leftCol * rightCols + rightCol], line, column, product);
						if (error)
							return error;
						// Accumulate it in the results (leftRow, rightCol)
						error = working[resultRowIndex + rightCol].TypeBehaviors()->Plus(working[resultRowIndex + rightCol], product,
							line, column, working[resultRowIndex + rightCol]);
						if (error)
							return error;
					}
				}
			}

			result = working;

			return NoError;
		}
	}

	return Error(IllegalOperand, line, column);
}

Error TypeBehaviorsArray::Div(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	const uint16_t *dims = arg1.Dimensions();
	uint16_t dimSize = arg1.DimensionsSize();
	Value working(dims, dimSize);
	for (size_t i = 0; i < working.NumberOfElements(); ++i)
	{
		Error error = arg1[i].TypeBehaviors()->Div(arg1[i], arg2, line, column, working[i]);
		if (error)
			return error;
	}
	result = working;

	return NoError;
}

void TypeBehaviorsArray::Print(const Value &varray) const
{
	const uint16_t *dims = varray.Dimensions();
	size_t rowStart = 0;
	return Print(varray, dims, varray.DimensionsSize(), 0, &rowStart);
}

void TypeBehaviorsArray::Print(const Value &array, const uint16_t *dims, uint16_t dimsSize, size_t dimIndex, size_t *rowStart) const
{
	FPUTC2('{', stdout);
	for (int i = 0; i < dims[dimIndex]; ++i)
	{
		if (dimIndex + 1 < dimsSize && dims[dimIndex + 1])
			Print(array, dims, dimsSize, dimIndex + 1, rowStart);
		else
		{
			const Value &v = array[*rowStart];
			v.Print();
			++(*rowStart);
		}
        if (i + 1 < dims[dimIndex])
            FPUTS2(", ", stdout);
	}
	FPUTC2('}', stdout);
}

/**************************************************************************
* Struct Operators
*************************************************************************/

void TypeBehaviorsStruct::DestroyValue(Value *value) const
{
	if (--(value->_d->refCnt) == 0)
	{
		Value::StructData_t *sd = (Value::StructData_t *) value->_d->data;
		delete[] sd->values;
		delete sd;
		delete value->_d;
	}
	value->_d = null;
}

void TypeBehaviorsStruct::CopyTo(const Value &src, Value &dest) const
{
	dest._d = src._d;
	++dest._d->refCnt;
}

void TypeBehaviorsStruct::DeepCopy(const Value &src, Value &dest, bool sourceWillBeDestroyed) const
{
	// If the source is going to be destroyed after this, and it's the only one pointing to the data
	if (sourceWillBeDestroyed && src._d->refCnt == 1)
		// We can just copy the reference
		CopyTo(src, dest);
	// Else (we really need to do a deep copy)
	else
	{
		Value::StructData_t *structData = (Value::StructData_t *) src._d->data;
		dest = Value(structData->structDef);
		Value::StructData_t *destStructData = (Value::StructData_t *) dest._d->data;
		for (size_t i = 0; i < structData->structDef->MemberCount(); ++i)
			structData->values[i].TypeBehaviors()->DeepCopy(structData->values[i], destStructData->values[i], false);
	}
}

Error TypeBehaviorsStruct::Equal(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (!arg2.IsStruct())
	{
		return Error(IllegalOperand, line, column);
	}

	Value::StructData_t *arg1StructData = (Value::StructData_t *) arg1._d->data;
	Value::StructData_t *arg2StructData = (Value::StructData_t *) arg2._d->data;
	if (arg1StructData->structDef != arg2StructData->structDef)
		result = 0;

	else
	{
		result = 1;

		const StructDef *structDef = arg1StructData->structDef;
		for (size_t i = 0; i < structDef->MemberCount(); ++i)
		{
			Error error = arg1StructData->values[i].TypeBehaviors()->Equal(arg1StructData->values[i], arg2StructData->values[i], line, column, result);
			if (error)
				return error;
			if (result.Integer() == 0)
				return NoError;
		}
	}

	return NoError;
}

void TypeBehaviorsStruct::Print(const Value &value) const
{
	Value::StructData_t *structData = (Value::StructData_t *) value._d->data;
	const StructDef *structDef = structData->structDef;
	FPUTC2('{', stdout);
	for (size_t i = 0; i < structDef->MemberCount(); ++i)
	{
		const String &name = structDef->IndexToName(i);
//        ::Print(stdout, name.c_str(), ": ");
        VPRINT2("%s: ", name.c_str());
		structData->values[i].Print();
		if (i < structDef->MemberCount() - 1)
//      ::Print(stdout, ", ");
      VPRINT2(", ");
	}
	FPUTS2("}", stdout);
}

/**************************************************************************
* Handle Operators
*************************************************************************/

void TypeBehaviorsHandle::DestroyValue(Value *value) const
{
	if (--value->_d->refCnt == 0)
	{
		delete (GSDataObject *) value->_d->data;
		delete value->_d;
	}
	value->_d = null;
}

void TypeBehaviorsHandle::CopyTo(const Value &src, Value &dest) const
{
	dest._d = src._d;
	++dest._d->refCnt;
}

Error TypeBehaviorsHandle::Equal(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (arg2.IsHandle())
	{
		result = arg1._d->data == arg2._d->data;
		return NoError;
	}
	else
		return Error(IllegalOperand, line, column);
}


/**************************************************************************
* ByRef Operators
*************************************************************************/

void TypeBehaviorsByRef::CopyTo(const Value &src, Value &dest) const
{
	dest._byRef = src._byRef; 
}

Error TypeBehaviorsByRef::Equal(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const
{
	if (arg2.IsByRef())
	{
		result = arg1._byRef == arg2._byRef;
		return NoError;
	}
	else
		return Error(IllegalOperand, line, column);
}


/**************************************************************************
 * ByteReference Operators
 *************************************************************************/

void TypeBehaviorsByteReference::CopyTo(const Value &src, Value &dest) const
{
	dest._byte = src._byte;
}

String TypeBehaviorsByteReference::ToString(const Value &) const
{
	return String("");
}

