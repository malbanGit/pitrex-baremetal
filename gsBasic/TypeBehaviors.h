
#ifndef _TYPEBEHAVIORS_H
#define _TYPEBEHAVIORS_H


#include <stdint.h>
#include "Error.h"
#include "GSBString.h"
template <class T> class Array;
class Value;

class TypeBehaviors
{
public:
	virtual void DestroyValue(Value *value) const = 0;
	virtual void CopyTo(const Value &src, Value &dest) const = 0;
	virtual void DeepCopy(const Value &src, Value &dest, bool /*sourceWillBeDestroyed*/) const;

	virtual bool IsArray() const = 0;

	virtual Error UnaryPlus(const Value &value, uint16_t line, uint16_t column, Value &result) const = 0;
	virtual Error UnaryMinus(const Value &value, uint16_t line, uint16_t column, Value &result) const = 0;
	Error Not(const Value &value, uint16_t line, uint16_t column, Value &result) const;

	virtual Error Plus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const = 0;
	virtual Error Minus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const = 0;
	virtual Error Mul(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const = 0;
	virtual Error Div(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const = 0;
	virtual Error Mod(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const = 0;
	virtual Error Exponent(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const = 0;

	virtual Error LessThan(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const = 0;
	virtual Error LessThanOrEqual(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const = 0;
	virtual Error Equal(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const = 0;
	Error NotEqual(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	Error GreaterThanOrEqual(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	Error GreaterThan(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;

	Error And(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	Error Or(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	Error Xor(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;

	virtual Error BitwiseAND(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error BitwiseOR(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error BitwiseXOR(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalXOROperand, line, column); }
	virtual Error BitwiseNOT(const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }

	virtual int ToBool(const Value &value) const = 0;
	virtual ErrorCode ToInteger(const Value &value, int *i) const = 0;
	virtual ErrorCode ToFloat(const Value &value, float *f) const = 0;
	virtual String ToString(const Value &value) const = 0;

   	ErrorCode ToValue(const Value &value, float *f) const {return ToFloat(value, f);}
   	ErrorCode ToValue(const Value &value, int *i) const {return ToInteger(value, i);}

	virtual void Print(const Value &value) const;
};

class TypeBehaviorsUninitialized : public TypeBehaviors
{
public:
	static const TypeBehaviors *Singleton()
	{
		return &_singleton;
	}

	virtual void CopyTo(const Value &, Value &) const {}

	virtual void DestroyValue(Value *) const {}
	virtual bool IsArray() const { return false; }
	virtual Error UnaryPlus(const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error UnaryMinus(const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }

	virtual Error Plus(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Minus(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Mul(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Div(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Mod(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Exponent(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }

	virtual Error LessThan(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error LessThanOrEqual(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Equal(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }

	virtual ErrorCode ToInteger(const Value &, int *) const { return IllegalOperand; }
	virtual ErrorCode ToFloat(const Value &, float *) const { return IllegalOperand; }
	virtual int ToBool(const Value &) const { return 0; }
	virtual String ToString(const Value &) const { return "<uninitialized>"; }

private:
	static TypeBehaviorsUninitialized _singleton;
};

class TypeBehaviorsInt : public TypeBehaviors
{
public:
	static const TypeBehaviors *Singleton()
	{
		return &_singleton;
	}

	virtual void DestroyValue(Value *value) const;
	virtual void CopyTo(const Value &src, Value &dest) const;

	virtual bool IsArray() const { return false; }
	virtual Error UnaryPlus(const Value &value, uint16_t line, uint16_t column, Value &result) const;
	virtual Error UnaryMinus(const Value &value, uint16_t, uint16_t, Value &result) const;

	virtual Error Plus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Minus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Mul(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Div(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Mod(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Exponent(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;

	virtual Error BitwiseAND(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error BitwiseOR(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error BitwiseXOR(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error BitwiseNOT(const Value &arg1, uint16_t line, uint16_t column, Value &result) const;

	virtual Error LessThan(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error LessThanOrEqual(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Equal(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;

	virtual ErrorCode ToInteger(const Value &value, int *i) const;
	virtual ErrorCode ToFloat(const Value &value, float *f) const;
	virtual int ToBool(const Value &value) const;
	virtual String ToString(const Value &value) const;

private:
	static TypeBehaviorsInt _singleton;
};

class TypeBehaviorsFloat : public TypeBehaviors
{
public:
	static const TypeBehaviors *Singleton()
	{
		return &_singleton;
	}

	virtual void DestroyValue(Value *value) const;
	virtual void CopyTo(const Value &src, Value &dest) const;

	virtual bool IsArray() const { return false; }
	virtual Error UnaryPlus(const Value &value, uint16_t line, uint16_t column, Value &result) const;
	virtual Error UnaryMinus(const Value &value, uint16_t, uint16_t, Value &result) const;

	virtual Error Plus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Minus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Mul(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Div(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Mod(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Exponent(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;

	virtual Error LessThan(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error LessThanOrEqual(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Equal(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;

	virtual ErrorCode ToInteger(const Value &value, int *i) const;
	virtual ErrorCode ToFloat(const Value &value, float *f) const;
	virtual int ToBool(const Value &value) const;
	virtual String ToString(const Value &value) const;

private:
	static TypeBehaviorsFloat _singleton;
};

class TypeBehaviorsString : public TypeBehaviors
{
public:
	static const TypeBehaviors *Singleton()
	{
		return &_singleton;
	}

	virtual void DestroyValue(Value *value) const;
	virtual void CopyTo(const Value &src, Value &dest) const;

	virtual bool IsArray() const { return false; }
	virtual Error UnaryPlus(const Value &value, uint16_t line, uint16_t column, Value &result) const;
	virtual Error UnaryMinus(const Value &value, uint16_t, uint16_t, Value &result) const;

	virtual Error Plus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Minus(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Mul(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Div(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Mod(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Exponent(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;

	virtual Error LessThan(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error LessThanOrEqual(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;
	virtual Error Equal(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;

	virtual ErrorCode ToInteger(const Value &value, int *i) const;
	virtual ErrorCode ToFloat(const Value &value, float *f) const;
	virtual int ToBool(const Value &value) const;
	virtual String ToString(const Value &value) const;

private:
	static TypeBehaviorsString _singleton;
};

class TypeBehaviorsArray : public TypeBehaviors
{
public:
	static const TypeBehaviors *Singleton()
	{
		return &_singleton;
	}

	virtual void DestroyValue(Value *value) const;
	virtual void CopyTo(const Value &src, Value &dest) const;
	virtual void DeepCopy(const Value &src, Value &dest, bool sourceWillBeDestroyed) const;

	virtual bool IsArray() const { return true; }
	virtual Error UnaryPlus(const Value &, uint16_t line, uint16_t column, Value &) const;
	virtual Error UnaryMinus(const Value &, uint16_t line, uint16_t column, Value &) const;

	virtual Error Plus(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const;
	virtual Error Minus(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const;
	virtual Error Mul(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const;
	virtual Error Div(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const;
	virtual Error Mod(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Exponent(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }

	virtual Error LessThan(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error LessThanOrEqual(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Equal(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }

	virtual ErrorCode ToInteger(const Value &, int *) const { return MustBeNumeric; }
	virtual ErrorCode ToFloat(const Value &, float *) const { return MustBeNumeric; }
	virtual int ToBool(const Value &) const { return 0; }
	virtual String ToString(const Value &) const { return "<array>"; }

	virtual void Print(const Value &value) const;

private:
	void Print(const Value &varray, const uint16_t *dims, uint16_t dimsSize, size_t dimIndex, size_t *rowStart) const;
	static TypeBehaviorsArray _singleton;
};

class TypeBehaviorsStruct : public TypeBehaviors
{
public:
	static const TypeBehaviors *Singleton()
	{
		return &_singleton;
	}

	virtual void DestroyValue(Value *value) const;
	virtual void CopyTo(const Value &src, Value &dest) const;
	virtual void DeepCopy(const Value &src, Value &dest, bool sourceWillBeDestroyed) const;

	virtual bool IsArray() const { return false; }
	virtual Error UnaryPlus(const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error UnaryMinus(const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }

	virtual Error Plus(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Minus(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Mul(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Div(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Mod(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Exponent(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }

	virtual Error LessThan(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error LessThanOrEqual(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Equal(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;

	virtual ErrorCode ToInteger(const Value &, int *) const { return MustBeNumeric; }
	virtual ErrorCode ToFloat(const Value &, float *) const { return MustBeNumeric; }
	virtual int ToBool(const Value &) const { return 0; }
	virtual String ToString(const Value &) const { return "<struct>"; }

	virtual void Print(const Value &value) const;

private:
	static TypeBehaviorsStruct _singleton;
};

// A pointer to a C++ object
class TypeBehaviorsHandle : public TypeBehaviors
{
public:
	static const TypeBehaviors *Singleton()
	{
		return &_singleton;
	}

	virtual void DestroyValue(Value *value) const;
	virtual void CopyTo(const Value &src, Value &dest) const;

	virtual bool IsArray() const { return false; }
	virtual Error UnaryPlus(const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error UnaryMinus(const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }

	virtual Error Plus(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Minus(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Mul(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Div(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Mod(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Exponent(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }

	virtual Error LessThan(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error LessThanOrEqual(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Equal(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const;

	virtual ErrorCode ToInteger(const Value &, int *) const { return MustBeNumeric; }
	virtual ErrorCode ToFloat(const Value &, float *) const { return MustBeNumeric; }
	virtual int ToBool(const Value &) const { return 0; }
	virtual String ToString(const Value &) const { return "Handle"; }

private:
	static TypeBehaviorsHandle _singleton;
};

class TypeBehaviorsByRef : public TypeBehaviors
{
public:
	static const TypeBehaviors *Singleton()
	{
		return &_singleton;
	}

	virtual void DestroyValue(Value *) const {}
	virtual void CopyTo(const Value &src, Value &dest) const;

	virtual bool IsArray() const { return false; }
	virtual Error UnaryPlus(const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error UnaryMinus(const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }

	virtual Error Plus(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Minus(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Mul(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Div(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Mod(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Exponent(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }

	virtual Error LessThan(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error LessThanOrEqual(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(IllegalOperand, line, column); }
	virtual Error Equal(const Value &arg1, const Value &arg2, uint16_t line, uint16_t column, Value &result) const;

	virtual ErrorCode ToInteger(const Value &, int *) const { return MustBeNumeric; }
	virtual ErrorCode ToFloat(const Value &, float *) const { return MustBeNumeric; }
	virtual int ToBool(const Value &) const { return 0; }
	virtual String ToString(const Value &) const { return "Value ref"; }

private:
	static TypeBehaviorsByRef _singleton;
};


class TypeBehaviorsByteReference : public TypeBehaviors
{
public:
	static const TypeBehaviors *Singleton()
	{
		return &_singleton;
	}

	virtual void DestroyValue(Value *) const {}
	virtual void CopyTo(const Value &src, Value &dest) const;

	virtual bool IsArray() const { return false; }
	virtual Error UnaryPlus(const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }
	virtual Error UnaryMinus(const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }

	virtual Error Plus(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }
	virtual Error Minus(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }
	virtual Error Mul(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }
	virtual Error Div(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }
	virtual Error Mod(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }
	virtual Error Exponent(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }

	virtual Error BitwiseAND(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }
	virtual Error BitwiseOR(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }
	virtual Error BitwiseXOR(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }
	virtual Error BitwiseNOT(const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }

	virtual Error LessThan(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }
	virtual Error LessThanOrEqual(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }
	virtual Error Equal(const Value &, const Value &, uint16_t line, uint16_t column, Value &) const { return Error(NotImplemented, line, column); }

	virtual ErrorCode ToInteger(const Value &, int *) const { return NotImplemented; }
	virtual ErrorCode ToFloat(const Value &, float *) const { return NotImplemented; }
	virtual int ToBool(const Value &) const { return false; }
	virtual String ToString(const Value &) const;

private:
	static TypeBehaviorsByteReference _singleton;
};


#endif