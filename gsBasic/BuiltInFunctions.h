#ifndef _BUILTINFUNCTIONS_H_
#define _BUILTINFUNCTIONS_H_

#include "Error.h"
#include "GSBCommon.h"
#include <stddef.h>

class Executive;
class RunTimeEnvironment;
class Value;

typedef ErrorCode (*BuiltInFunction)(Executive &exec, RunTimeEnvironment &rte, int numberOfArguments);

struct BuiltInFunctionInfo
{
	const char *name;
	BuiltInFunction fn;
	uint16_t minimumNumberOfArguments, maximumNumberOfArguments;
};

extern const BuiltInFunctionInfo *FindBuiltInFunction(const char *name);

// Let another module register built-in functions. So if GSBasic is embedded in another
// app, this lets that app add built-in functions to GSBasic
// THE LIST OF BUILT-IN FUNCTIONS MUST BE IN ALPHABETICAL ORDER!!!!
void RegisterBuiltInFunctions(const BuiltInFunctionInfo *builtInFunctions, int builtInFunctionsCount);

void SetReturnValue(RunTimeEnvironment &runTimeEnvironment, const Value &value);

#endif //_BUILTINFUNCTIONS_H_
