#include "CallSpec.h"
#include "Reader.h"
#include "Token.h"
#include "ExpressionList.h"
#include "Executive.h"
#include <stdio.h>

Error CallSpec::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	Token token;

	// Get the name of the function or subroutine we're calling
	if (tokenReader->Next(token) && token.Type() != Token::Identifier)
	{
		return Error(IllegalVariableSpec, token.LineNumber(), token.Column());
	}
	_callName = token.ValueIdentifier();
	SetLocation(token.LineNumber(), token.Column());

	// Get the argument list
	if (tokenReader->Next(token) && token.Type() == Token::LParen)
	{
		_arguments = new ExpressionList();
		Error error = _arguments->Build(tokenReader, compileEnvironment, false, token.LineNumber(), token.Column());
		if (error && error.Message() != EmptyExpression)
			return error;
		if (tokenReader->Next(token))
		{
			if (token.Type() != Token::RParen)
				return Error(MissingEndParen, token.LineNumber(), token.Column());
		}
		else
			return Error(MissingEndParen, LineNumber(), Column());
	}

	else
		tokenReader->Prev();

	compileEnvironment.QueueResolveCallSpec(this);

	return NoError;
}

Error CallSpec::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	return Execute(exec, runTimeEnvironment, _arguments);
}

Error CallSpec::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment, const ExpressionList *arguments)
{
	if (_type == Uninitialized)
		return Error(UndefinedProcedure, LineNumber(), Column());
	size_t argCount = arguments ? arguments->Expressions().size() : 0;
	if ((_type == Proc && _proc->Parameters().size() != argCount))
		return Error(WrongNumberOfArguments, LineNumber(), Column());
    if (_type == BuiltIn)
	{
		if (argCount < _builtIn->minimumNumberOfArguments)
			return Error(WrongNumberOfArguments, LineNumber(), Column());
		if (argCount > _builtIn->maximumNumberOfArguments && argCount > _builtIn->minimumNumberOfArguments)
			return Error(WrongNumberOfArguments, LineNumber(), Column());
	}
	else if (_type == Struct)
	{
		if (argCount > _structDef->MemberCount())
			return Error(WrongNumberOfArguments, LineNumber(), Column());
	}

	Error error = NoError;
	int argumentCount = 0;
	size_t oldCalcStackSize = runTimeEnvironment.calculationStack.size();

	const RunTimeScope *oldScope = null;

	// Evaluate all the subroutine/function arguments.
	if (arguments)
	{
		// Evaluate all the arguments and leave them on the stack
		for (argumentCount = 0; argumentCount < int(arguments->Expressions().size()) && !error; ++argumentCount)
		{
			size_t stackSize = runTimeEnvironment.calculationStack.size();
			if (_type == Proc && _proc->IsByRefArgument(argumentCount))
			{
				Expression *expr = arguments->Expressions()[argumentCount];
				VarSpec *varSpec = Expression::CastToVarSpec(expr);
				if (varSpec == null)
					return Error(ByRefParameter, LineNumber(), Column());
				error = varSpec->ByRefExecute(exec, runTimeEnvironment);
			}

			// Else (call by value)
			else
			{
				error = Expression::Execute(arguments->Expressions()[argumentCount], exec, runTimeEnvironment);
			}
			if (!error && runTimeEnvironment.calculationStack.size() != stackSize + 1)
            {
                auto &arg = arguments->Expressions()[argumentCount];
                if (Expression::IsExpressionPointer(arg))
                    error = Error(MustReturnValue, arg->LineNumber(), arg->Column());
                else
                    error = Error(MustReturnValue, LineNumber(), Column());
            }
		}

		if (_type == Proc && !error)
		{
			// Create the local scope
			oldScope = runTimeEnvironment.CreateLocalScope(_proc->LocalVariableCount());

			// Assign the arguments to the parameters
			for (int i = 0; i < argumentCount && !error; ++i)
			{
				VarSpec *parameter = _proc->Parameters()[i];
				parameter->SetValue(runTimeEnvironment.calculationStack[oldCalcStackSize + i], runTimeEnvironment);
				runTimeEnvironment.calculationStack[oldCalcStackSize + i].Clear();
			}
		}
	}

	// Else (no arguments in the call and no parameters in the procedure)
	else if (_type == Proc)
		oldScope = runTimeEnvironment.CreateLocalScope(_proc->LocalVariableCount());

	if (!error)
	{
		if (_type == Proc)
		{
			error = exec.Gosub(_proc, runTimeEnvironment);
			runTimeEnvironment.DestroyLocalScope(oldScope);
		}
		else if (_type == BuiltIn)
		{
			ErrorCode code = (*_builtIn->fn)(exec, runTimeEnvironment, argumentCount);
			if (code)
				error = Error(code, LineNumber(), Column());
		}
		else
		{
			ErrorCode code = _structDef->Construct(exec, runTimeEnvironment, argumentCount);
			if (code)
				error = Error(code, LineNumber(), Column());
		}
	}

	// Clean up the calc stack
	runTimeEnvironment.calculationStack.resize(oldCalcStackSize);

	if (!error)
	{
        bool valid;
		const Value &returnValue = runTimeEnvironment.RetrieveReturnValue(&valid);
		if (valid)
			runTimeEnvironment.calculationStack.push_back(returnValue);
		runTimeEnvironment.ClearReturnValue();
	}

	return error;
}

const char *CallSpec::CallName() const 
{
	if (!_callName.empty())
		return _callName.c_str();
	else if (_proc)
		return _proc->Name().c_str();
	else
		return _builtIn->name;
}

Error CallSpec::SetProcedure(const BuiltInFunctionInfo *fn)
{
	_callName.clear();
	_builtIn = fn;
	_type = BuiltIn;
	return NoError;
}

Error CallSpec::SetProcedure(StatementProcedure *proc)
{
	_callName.clear();
	_proc = proc;
	_type = Proc;
	return NoError;
}

Error CallSpec::SetProcedure(StructDef *structDef)
{
	_callName.clear();
	_structDef = structDef;
	_type = Struct;
	return NoError;
}

#ifdef _DEBUG
void CallSpec::Dump() 
{
	puts("CallSpec"); 
	puts(CallName()); 
	if (_arguments) 
	{
		_arguments->Dump(); 
	}
}
#endif
