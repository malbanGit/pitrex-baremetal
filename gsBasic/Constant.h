#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include "Value.h"
#include "Error.h"
#include "Environment.h"
#include "Expression.h"
#include "Pool.h"
#include "GSBString.h"

class Constant : public Expression
{
protected:
	Constant() {}

public:
	static Constant *Create(Token *token);
	static Constant *Create(const ::Value &value);
	~Constant() {}

	virtual Error Build(TokenReader *, CompileEnvironment &) {return NoError;}

	virtual uint16_t LineNumber() const {return 0;}
	virtual uint16_t Column() const { return 0; }
	virtual uint16_t LastLineNumber() const { return 0; }
	virtual uint16_t LastColumn() const { return 0; }
	virtual void SetLocation(uint16_t, uint16_t) {}
};

class ConstantInt : public Constant
{
public:
	ConstantInt(int i) : _i(i) {}

	virtual Error Execute(Executive &, RunTimeEnvironment &runTimeEnvironment)
	{
		runTimeEnvironment.calculationStack.push_back(_i);
		return NoError;
	}

#ifdef _DEBUG
	virtual void Dump() { ::Value(_i).Dump(); }
#endif

#pragma push_macro("new")
#undef new
	void *operator new(size_t) { return _pool.Allocate(); }
	void *operator new(size_t, int, const char *, int) { return _pool.Allocate(); }
	void operator delete(void *p) { _pool.Free((ConstantInt *) p); }
	void operator delete(void *p, int, const char *, int) { _pool.Free((ConstantInt *) p); }
#pragma pop_macro("new")

private:
	int _i;

	static PoolAllocator<ConstantInt> _pool;
};

class ConstantFloat : public Constant
{
public:
	ConstantFloat(float f) : _f(f) {}

	virtual Error Execute(Executive &, RunTimeEnvironment &runTimeEnvironment)
	{
		runTimeEnvironment.calculationStack.push_back(_f);
		return NoError;
	}

#ifdef _DEBUG
	virtual void Dump() { ::Value(_f).Dump(); }
#endif

#pragma push_macro("new")
#undef new
	void *operator new(size_t) { return _pool.Allocate(); }
	void *operator new(size_t, int, const char *, int) { return _pool.Allocate(); }
	void operator delete(void *p) { _pool.Free((ConstantFloat *) p); }
	void operator delete(void *p, int, const char *, int) { _pool.Free((ConstantFloat *) p); }
#pragma pop_macro("new")

private:
	float _f;

	static PoolAllocator<ConstantFloat> _pool;
};

class ConstantString : public Constant
{
public:
	ConstantString(const char *s) : _s(s) {}
	ConstantString(const String &s) : _s(s) {}

	virtual Error Execute(Executive &, RunTimeEnvironment &runTimeEnvironment)
	{
		runTimeEnvironment.calculationStack.push_back(_s);
		return NoError;
	}

#ifdef _DEBUG
	virtual void Dump() { ::Value(_s).Dump(); }
#endif

#pragma push_macro("new")
#undef new
	void *operator new(size_t) { return _pool.Allocate(); }
	void *operator new(size_t, int, const char *, int) { return _pool.Allocate(); }
	void operator delete(void *p) { _pool.Free((ConstantString *) p); }
	void operator delete(void *p, int, const char *, int) { _pool.Free((ConstantString *) p); }
#pragma pop_macro("new")

private:
	String _s;

	static PoolAllocator<ConstantString, 10> _pool;
};

class ConstantNil : public Constant
{
public:
	ConstantNil() {}

	virtual Error Execute(Executive &, RunTimeEnvironment &runTimeEnvironment)
	{
		runTimeEnvironment.calculationStack.push_back(::Value());
		return NoError;
	}

#ifdef _DEBUG
	virtual void Dump() { ::Value().Dump(); }
#endif
};

class ConstantValue : public Constant
{
public:
	ConstantValue(const ::Value &value) : _value(value) {}

	virtual Error Execute(Executive &, RunTimeEnvironment &runTimeEnvironment)
	{
		runTimeEnvironment.calculationStack.push_back(_value);
		return NoError;
	}

#ifdef _DEBUG
	virtual void Dump() { _value.Dump(); }
#endif
    
private:
    ::Value _value;
};

#endif
