
#ifndef _Environment_H_
#define _Environment_H_

#include "GSBCommon.h"
#include "Array.h"
#include "Error.h"
#include "GSBString.h"
#include "Value.h"
#include "ValueArray.h"
#include "StructDef.h"

class Statement;
class StatementOn;
class StatementProcedure;
class StatementStruct;
class VarSpec;
class CallSpec;
class Value;

extern int ARRAY_BASE;

/**
 * Keep track of control structures (IF, FOR, FUNCTION, etc) as we parse their opening statements and
 * their closing statements
 */
class ControlStructure
{
public:
	typedef enum {NONE, IF, ELSEIF, /*ELSE, Else is just an Elseif with no condition */ WHILE, FOR, REPEAT, SUB, FUNCTION} Type_t;

	ControlStructure(Type_t type, Statement *openingStatement) : _openingStatement(openingStatement), _type(type) {}

	Statement *OpeningStatement() const {return _openingStatement;}
	Type_t Type() const {return _type;}

private:
	Statement *_openingStatement; // IF, ELSEIF, ELSE, FOR, FUNCTION, SUB, WHILE
	Type_t _type;
};

/**
* A VariableHandle allows us to access variables in their contexts. The most significant bit
* is 0 for global variables and 1 for local variables. The next MSB is 0 for writable or
* 1 for constant. The remaining bits are an index into RunTimeEnvironment's variableStack, 
* offset by the frame pointer
*/
class VariableHandle
{
public:
	VariableHandle() : _handle(-1) {}
	VariableHandle(int index, bool isLocalContext) : _handle(isLocalContext ? (index | 0x80000000) : index) {}
	bool IsGlobal() const { return _handle >= 0; }
	int ToIndex() const { return _handle & 0x3fffffff; }
	bool IsConstant() const { return (_handle & 0x40000000) != 0; }

	bool operator==(const VariableHandle &other) { return _handle == other._handle; }

private:
	int _handle;
};

/**
* Maintain a list of variable names declared in the current context. This is compile-time information.
*/
class VariableScope
{
public:
	VariableScope() {}
	~VariableScope() {}

	void Clear() { _variableNames.resize(0); }

	ErrorCode FindVariable(const String &name, bool isLocalContext, VariableHandle *handle);
	ErrorCode AllocateVariable(const String &name, bool isLocalContext, VariableHandle *handle);

	int VariableCount() const { return _variableNames.size(); }

private:
	Array<String> _variableNames;
};


/**
 * This is the CompileEnvironment for a block of compiled code 
 */
class CompileEnvironment
{
public:
	CompileEnvironment();
	~CompileEnvironment() {Clear();}

	PtrArray<ControlStructure> &ControlStructureStack() {return _controlStructureStack;}

	void Clear();

	void RegisterProcedure(StatementProcedure *proc) {_procedures.push_back(proc);}
	void QueueResolveCallSpec(CallSpec *callSpec) {_toBeResolved.push_back(callSpec);}
	Error ResolveCallSpecs();
	void ClearCallSpecs();
	StatementProcedure *FindProcedure(const String &name);

	void RegisterStruct(StructDef *structDef) { _structs.push_back(structDef); }
	StructDef *FindStruct(const String &name);

	ErrorCode FindOrAllocateVariable(const String &name, bool includeGlobalScope, bool allocateIfNotFound, VariableHandle *handle);

	void EnterVariableScope() { _variableScopes.push_back(new VariableScope()); }
	void EnterVariableScope(VariableScope *variableScope) { _variableScopes.push_back(variableScope); }
	VariableScope *ExitVariableScope();
	
private:
	static int CallSpecCompare(CallSpec * const &a, CallSpec * const &b);

	friend class RunTimeEnvironment;
	PtrOwnerArray<ControlStructure> _controlStructureStack;
	PtrArray<CallSpec> _toBeResolved;
	PtrArray<StatementProcedure> _procedures;
	PtrOwnerArray<StructDef> _structs;

	PtrOwnerArray<VariableScope> _variableScopes;
};

class RunTimeScope : private Array<Value>
{
public:
	RunTimeScope(int numberOfVariables, StatementOn *onErrorCall);

	const Value &GetValue(VariableHandle handle);
	/**
	 * Returns a pointer to the value. This allows the caller to re-assign a value to
	 * a variable. WARNING: this pointer has a short lifetime! If the array of values is enlarged
	 * (via a realloc) this pointer will become invalid.
	 */
	Value &GetValueReference(VariableHandle handle);
	void SetValue(VariableHandle handle, const Value &value);

	StatementOn *GetOnErrorCall() { return _onErrorCall; }
	void SetOnErrorCall(StatementOn *onErrorCall) { _onErrorCall = onErrorCall; }

private:
	void RequiredSize(size_t size);
	Array<Value> _variableValues;
	StatementOn *_onErrorCall;
};

class RunTimeEnvironment
{
public:
	RunTimeEnvironment();
	~RunTimeEnvironment();

	const Value &GetValue(VariableHandle handle);
	/**
	* Returns a reference to the value. This allows the caller to re-assign a value to
	* a variable. WARNING: this pointer has a short lifetime! If the array of values is enlarged
	* (via a realloc) this pointer will become invalid.
	*/
	Value &GetValueReference(VariableHandle handle);
	void SetValue(VariableHandle handle, const Value &value);

	StatementOn *GetOnErrorCall();
	void SetOnErrorCall(StatementOn *onErrorCall);

	void SetLastError(const Error &error) { _lastError = error; }
	const Error &GetLastError() const { return _lastError; }
	void ClearLastError() { _lastError = NoError; }

	/**
	 * Set the scope to local or global
	 */
	const RunTimeScope *CreateLocalScope(int numberOfVariables);
	void DestroyLocalScope(const RunTimeScope *previousScope);
	void SetReturnValue(const Value &value) { _returnValue = value; _validReturnValue = true;}
	const Value &RetrieveReturnValue(bool *valid) const
    {
        *valid = _validReturnValue; 
        return _returnValue;
	}
	void ClearReturnValue()
	{
		_returnValue.Clear();
		_validReturnValue = false;
	}

	CompileEnvironment compileEnvironment;
	ValueArray calculationStack;

	// Reset clears the runtime state of the BASIC program
	void Reset();

	// Clear does a Reset but also clears the compile-time state of the program
	void Clear();

private:
	RunTimeEnvironment &operator=(const RunTimeEnvironment &);

	// Linked lists of variables in the current scope
	RunTimeScope *_localScope;
	RunTimeScope *_globalScope;
	Value _returnValue; // Used to return a value from a function
    bool _validReturnValue;
	Error _lastError;
};

#endif
