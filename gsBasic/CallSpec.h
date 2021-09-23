
#ifndef _CALLSPEC_H_
#define _CALLSPEC_H_

#include "AST.h"
#include "ExpressionList.h"
#include "GSBString.h"
#include "BuiltInFunctions.h"

class StatementProcedure;
class StructDef;

class CallSpec : public ExpressionWithLocation
{
public:
	CallSpec() : _arguments(null), _type(Uninitialized), _proc(null) {}
	virtual ~CallSpec() {delete _arguments;}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment, const ExpressionList *arguments);

	const char *CallName() const;

	Error SetProcedure(StatementProcedure *proc);
	Error SetProcedure(const BuiltInFunctionInfo *fn);
	Error SetProcedure(StructDef *structDef);

#ifdef _DEBUG
	virtual void Dump();
#endif

private:
	String _callName;
	ExpressionList *_arguments;
	enum {Uninitialized, Proc, BuiltIn, Struct} _type;
	union
	{
		StatementProcedure *_proc;
		const BuiltInFunctionInfo *_builtIn;
		const StructDef *_structDef;
	};
};

#endif
