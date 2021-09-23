
#ifndef _VARSPEC_H_
#define _VARSPEC_H_

#include "AST.h"
#include "Environment.h"
#include "ExpressionList.h"
#include "GSBString.h"
#include "Pool.h"

class Executive;
class VarSpecWithElements;

/**
 * A VarSpec is a reference to a variable. It can appear in an expression, as the lvalue in an assignment 
 * FOR loop, in a DIM statement. So there will be many VarSpecs in the code referring to the same variable
 * (which exists at run-time)
 */
class VarSpec : public ExpressionWithLocation
{
public:
	virtual ~VarSpec() {}

	// Called at build time to compile an lvalue variable reference or a DIM
	typedef enum {RValue, LValue, DimReference} ReferenceType;

	Error Build(TokenReader *, CompileEnvironment &) { return Error(NotImplemented, 0, 0); }

	static Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment, uint16_t lineNumber, uint16_t column, ReferenceType referenceType, VarSpec **varSpec);
	static ErrorCode Build(const String &varName, CompileEnvironment &compileEnvironment, ReferenceType referenceType, VarSpec **varSpec);

	// Called at execution time to push the value of the variable onto the calculation stack
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);
	// Called at execution time to push a reference to the variable onto the calculation stack
	Error ByRefExecute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	// Called at execution time to dimension an array
	virtual Error Dim(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	virtual bool HasArrayIndexes() const { return false; }
	virtual bool HasMemberSpec() const { return false; }

	// Called at execution time to get a reference to the variable's value.
	// We want to be able to change the Value a variable or array
	// element has, so we need a Value *. We'll write the Value * into pvalue,
	// so we need pvalue pased in as a Value **.
	virtual Error Reference(Executive &exec, RunTimeEnvironment &runTimeEnvironment, Value **pvalue);

	const Value &GetValue(RunTimeEnvironment &runTime) const;
	void SetValue(const Value &value, RunTimeEnvironment &runTime);

	bool Match(const VarSpec &other) { return _variable == other._variable; }

#ifdef _DEBUG
	virtual void Dump();
#endif

#pragma push_macro("new")
#undef new
	void *operator new(size_t) {return _pool.Allocate();}
	void *operator new(size_t, int, const char *, int) {return _pool.Allocate(); }
	void operator delete(void *p) { _pool.Free((VarSpec *) p); }
	void operator delete(void *p, int, const char *, int) { _pool.Free((VarSpec *) p); }
#pragma pop_macro("new")

	class ElementSpec
	{
	public:
		ElementSpec() : _next(null) {}
		virtual ~ElementSpec() { delete _next; }

		virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment, uint16_t lineNumber, uint16_t column) = 0;
		virtual Error ResolveIndex(const Value &value, int *index, Executive &exec, RunTimeEnvironment &runTimeEnvironment) const = 0;

		void AppendSpec(ElementSpec *spec) { if (_next) _next->AppendSpec(spec); else _next = spec; }

		virtual bool IsEmpty() const = 0;

		virtual uint16_t LineNumber() const = 0;
		virtual uint16_t Column() const = 0;

		virtual void Dump() {}

	private:
		friend class VarSpec;
		friend class VarSpecWithElements;
		ElementSpec *_next;
	};

	class ArrayIndices : public ElementSpec
	{
	public:
		Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment, uint16_t lineNumber, uint16_t column);
		Error ResolveIndex(const Value &value, int *index, Executive &exec, RunTimeEnvironment &runTimeEnvironment) const;

		bool IsEmpty() const { return _arrayIndex.size() == 0; }
		uint16_t LineNumber() const { return _arrayIndex.LineNumber(); }
		uint16_t Column() const { return _arrayIndex.Column(); }

	protected:
		// If a VarSpec appears in an expression, this will be the list of array indexes.
		// If a VarSpec appears in a DIM, this will be the list of dimensions. Of course,
		// if a VarSpec is a scalar, this list will be empty.
		ExpressionList _arrayIndex;
	};

	class DimArrayIndices : public ArrayIndices
	{
	public:
		Error ResolveIndex(const Value &value, uint16_t *dims, size_t *dimsSize, Executive &exec, RunTimeEnvironment &runTimeEnvironment) const;
	};

	class StructMember : public ElementSpec
	{
	public:
		Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment, uint16_t lineNumber, uint16_t column);
		Error ResolveIndex(const Value &value, int *index, Executive &exec, RunTimeEnvironment &runTimeEnvironment) const;

		bool IsEmpty() const { return false; }

		uint16_t LineNumber() const { return _lineNumber; }
		uint16_t Column() const { return _column; }

	private:
		String _memberName;
		uint16_t _lineNumber, _column;
	};

protected:
	VarSpec() {}

	VariableHandle _variable;

	static PoolAllocator<VarSpec> _pool;
};

class VarSpecWithElements : public VarSpec
{
public:
	~VarSpecWithElements() { delete _spec; }

	// Called at execution time to dimension an array
	Error Dim(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	virtual bool HasArrayIndexes() const
	{
		ArrayIndices *arrayIndices = dynamic_cast<ArrayIndices *>(_spec);
		return arrayIndices && !arrayIndices->IsEmpty();
	}
	virtual bool HasMemberSpec() const
	{
		StructMember *structMember = dynamic_cast<StructMember *>(_spec);
		return structMember && !structMember->IsEmpty();
	}

	// Called at execution time to get a reference to the variable's value.
	// We want to be able to change the Value a variable or array
	// element has, so we need a Value *. We'll write the Value * into pvalue,
	// so we need pvalue pased in as a Value **.
	virtual Error Reference(Executive &exec, RunTimeEnvironment &runTimeEnvironment, Value **pvalue);

#pragma push_macro("new")
#undef new
	void *operator new(size_t) { return _vsepool.Allocate(); }
	void *operator new(size_t, int, const char *, int) { return _vsepool.Allocate(); }
	void operator delete(void *p) { _vsepool.Free((VarSpecWithElements *) p); }
	void operator delete(void *p, int, const char *, int) { _vsepool.Free((VarSpecWithElements *) p); }
#pragma pop_macro("new")

private:
	friend class VarSpec;

	VarSpecWithElements() : _spec(null) {}
	ElementSpec *_spec;

	static PoolAllocator<VarSpecWithElements> _vsepool;
};


#endif
