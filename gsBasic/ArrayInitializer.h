
#ifndef _ARRAYINITIALIZER_H_
#define _ARRAYINITIALIZER_H_

#include "GSBCommon.h"
#include "ExpressionList.h"
#include "Pool.h"

class ArrayInitializer : public ExpressionWithLocation
{
public:
	ArrayInitializer() : _dimensionCount(0) {}
	~ArrayInitializer() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump() { _expressionList.Dump(); }
#endif

	virtual uint16_t LastLineNumber() const { return _expressionList.LastLineNumber(); }
	virtual uint16_t LastColumn() const { return _expressionList.LastColumn(); }

#pragma push_macro("new")
#undef new
	void *operator new(size_t) {return _pool.Allocate();}
	void *operator new(size_t, int, const char *, int) {return _pool.Allocate(); }
	void operator delete(void *p) { _pool.Free((ArrayInitializer *) p); }
	void operator delete(void *p, int, const char *, int) { _pool.Free((ArrayInitializer *) p); }
#pragma pop_macro("new")

private:
	ErrorCode GetDimensions(uint16_t *dimensions, int dimension) const;
	Error CopyValues(Executive &exec, RunTimeEnvironment &runTimeEnvironment, 
		Value &va, ExpressionList *_expressionList, int dimension, int *arrayOffset);

	ExpressionList _expressionList;
	uint16_t _dimensions[MAX_ARRAY_DIMENSIONS];
	uint16_t _dimensionCount;

	static PoolAllocator<ArrayInitializer> _pool;
};

#endif //!_ARRAYINITIALIZER_H_