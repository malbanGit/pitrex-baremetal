
#include "Environment.h"
#include "Statements.h"
#include "CallSpec.h"
#include "Value.h"
#include "Error.h"
#include "BuiltInFunctions.h"

int ARRAY_BASE = 1;

/**
 * Given a variable name, look up the storage location of the variable.
 */
ErrorCode VariableScope::FindVariable(const String &name, bool isLocalContext, VariableHandle *handle)
{
	for (size_t i = 0; i < _variableNames.size(); ++i)
	{
		if (_variableNames[i] == name)
		{
			*handle = VariableHandle(i, isLocalContext);
			return 0;
		}
	}
	return UninitializedVariable;
}

ErrorCode VariableScope::AllocateVariable(const String &name, bool isLocalContext, VariableHandle *handle)
{
	*handle = VariableHandle(_variableNames.size(), isLocalContext);
	_variableNames.push_back(name);
	return 0;
}

RunTimeScope::RunTimeScope(int numberOfVariables, StatementOn *onErrorCall) :
	_onErrorCall(onErrorCall)
{
	resize(numberOfVariables);
}

const Value &RunTimeScope::GetValue(VariableHandle handle)
{
	int index = handle.ToIndex();
	RequiredSize(index + 1);
	return (*this)[index]; 
}

/**
* Returns a reference to the value. This allows the caller to re-assign a value to
* a variable. WARNING: this pointer has a short lifetime! If the array of values is enlarged
* (via a realloc) this pointer will become invalid.
*/
Value &RunTimeScope::GetValueReference(VariableHandle handle)
{
	int index = handle.ToIndex();
	RequiredSize(index + 1);
	return (*this)[index];
}

void RunTimeScope::SetValue(VariableHandle handle, const Value &value) 
{
	int index = handle.ToIndex();
	RequiredSize(index + 1);
	(*this)[index] = value;
}

void RunTimeScope::RequiredSize(size_t size)
{
	if (this->size() < size)
	{
		resize(size);
	}
}

RunTimeEnvironment::RunTimeEnvironment() :
	_localScope(null), _globalScope(new RunTimeScope(0, null)), _validReturnValue(false)
{
}

RunTimeEnvironment::~RunTimeEnvironment()
{
	Clear();
	delete _globalScope;
}

/**
 * Given a variable handle, get the variable's value
 */
const Value &RunTimeEnvironment::GetValue(VariableHandle handle)
{
	if (handle.IsGlobal())
		return _globalScope->GetValue(handle);
	else
		return _localScope->GetValue(handle);
}

Value &RunTimeEnvironment::GetValueReference(VariableHandle handle) 
{ 
	if (handle.IsGlobal())
		return _globalScope->GetValueReference(handle);
	else
		return _localScope->GetValueReference(handle);
}

void RunTimeEnvironment::SetValue(VariableHandle handle, const Value &value)
{
	if (handle.IsGlobal())
		_globalScope->SetValue(handle, value);
	else
		_localScope->SetValue(handle, value);
}

StatementOn *RunTimeEnvironment::GetOnErrorCall()
{
	if (_localScope)
		return _localScope->GetOnErrorCall();
	else
		return _globalScope->GetOnErrorCall();
}

void RunTimeEnvironment::SetOnErrorCall(StatementOn *onErrorCall)
{
	if (_localScope)
		_localScope->SetOnErrorCall(onErrorCall);
	else
		_globalScope->SetOnErrorCall(onErrorCall);
}

void RunTimeEnvironment::Reset()
{
  // Clear the variables
	delete _localScope;
	_localScope = null;
	delete _globalScope;
	_globalScope = null;


_returnValue.Clear();
	_lastError = NoError;

	// Clear the calculation stack (it should be empty anyway)
	calculationStack.resize(0);

	_globalScope = new RunTimeScope(0, null);
}

void RunTimeEnvironment::Clear()
{
	Reset();
	compileEnvironment.Clear();
}

const RunTimeScope *RunTimeEnvironment::CreateLocalScope(int numberOfVariables)
{
	RunTimeScope *oldScope = _localScope;
	_localScope = new RunTimeScope(numberOfVariables, oldScope ? oldScope->GetOnErrorCall() : _globalScope->GetOnErrorCall());
	return oldScope;
}

void RunTimeEnvironment::DestroyLocalScope(const RunTimeScope *previousScope)
{
	delete _localScope;
	_localScope = (RunTimeScope *) previousScope;
}

CompileEnvironment::CompileEnvironment()
{
	_variableScopes.push_back(new VariableScope);
}

void CompileEnvironment::Clear() 
{
	_toBeResolved.clear();
	_procedures.clear();
	_structs.clear();
	_controlStructureStack.clear();
	_variableScopes.clear();
	_variableScopes.push_back(new VariableScope);
}

int CompileEnvironment::CallSpecCompare(CallSpec * const &a, CallSpec * const &b)
{
	return strcmp(a->CallName(), b->CallName());
}

Error CompileEnvironment::ResolveCallSpecs()
{
	// Sort the CallSpecs by name
	_toBeResolved.sort(CallSpecCompare);
	// Go through each CallSpec
	for (size_t i = 0; i < _toBeResolved.size(); ++i)
	{
		// Find the procedure this CallSpec is calling. First, check the built-in functions
		const BuiltInFunctionInfo *fn = FindBuiltInFunction(_toBeResolved[i]->CallName());
		if (fn)
		{
			// Go through all the calls to this procedure
			size_t j;
			for (j = i; j < _toBeResolved.size() && _toBeResolved[i]->CallName() == _toBeResolved[j]->CallName(); ++j)
			{
				Error error = _toBeResolved[j]->SetProcedure(fn);
				if (error)
				{
					_toBeResolved.clear();
					return error;
				}
			}
			i = j - 1;
		}

		else
		{
			// If it's not a built-in function, try the user functions
			StatementProcedure *proc = FindProcedure(_toBeResolved[i]->CallName());
            
            if (proc)
            {
                // Go through all the calls to this procedure
                size_t j;
                for (j = i; j < _toBeResolved.size() && _toBeResolved[i]->CallName() == _toBeResolved[j]->CallName(); ++j)
                {
                    Error error = _toBeResolved[j]->SetProcedure(proc);
					if (error)
					{
						_toBeResolved.clear();
						return error;
					}
				}
                i = j - 1;
            }

			// Else see if it's the name of a struct
			else
			{
				StructDef *structDef = FindStruct(_toBeResolved[i]->CallName());
				if (structDef)
				{
					// Go through all the calls to this struct constructor
					size_t j;
					for (j = i; j < _toBeResolved.size() && _toBeResolved[i]->CallName() == _toBeResolved[j]->CallName(); ++j)
					{
						Error error = _toBeResolved[j]->SetProcedure(structDef);
						if (error)
						{
							_toBeResolved.clear();
							return error;
						}
					}
					i = j - 1;
				}
			}
		}
	}
	_toBeResolved.clear();
    
    return NoError;
}

void CompileEnvironment::ClearCallSpecs()
{
	_toBeResolved.clear();
}

StatementProcedure *CompileEnvironment::FindProcedure(const String &name)
{
	for (size_t procIndex = 0; procIndex < _procedures.size(); ++procIndex)
	{
		if (_procedures[procIndex]->Name().strcasecmp(name.c_str()) == 0)
			return _procedures[procIndex];
	}
	return null;
}

StructDef *CompileEnvironment::FindStruct(const String &name)
{
	for (size_t structIndex = 0; structIndex < _structs.size(); ++structIndex)
	{
		if (_structs[structIndex]->Name() == name)
			return _structs[structIndex];
	}
	return null;
}

ErrorCode CompileEnvironment::FindOrAllocateVariable(const String &name, bool includeGlobalScope, bool allocateIfNotFound, VariableHandle *handle)
{
	bool isLocalContext = _variableScopes.size() > 1;

	// See if the variable exists in the current scope
	ErrorCode error = _variableScopes.back()->FindVariable(name, isLocalContext, handle);
	// If the current scope is a local scope
	if (isLocalContext && error && includeGlobalScope)
	{
		error = _variableScopes[0]->FindVariable(name, false, handle);
	}
	// If we couldn't find the variable
	if (error && allocateIfNotFound)
	{
		// Allocate it in the current context
		error = _variableScopes.back()->AllocateVariable(name, isLocalContext, handle);
	}
	return error;
}

VariableScope *CompileEnvironment::ExitVariableScope() 
{
	VariableScope *vs = _variableScopes.back(); 
	_variableScopes.back() = null; 
	_variableScopes.pop_back(); 
	return vs; 
}
