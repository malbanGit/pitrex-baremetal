#ifndef _AST_H_
#define _AST_H_

#include "Error.h"
#include "GSBCommon.h"
class Executive;
class TokenReader;
class CompileEnvironment;
class RunTimeEnvironment;

class AST
{
public:
	virtual ~AST() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment) = 0;
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment) = 0;

#ifdef _DEBUG
	virtual void Dump() = 0;
#endif

protected:
	AST() {};
};

#endif