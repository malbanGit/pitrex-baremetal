#ifndef _BUILTINCONSTANTS_H_
#define _BUILTINCONSTANTS_H_

#include "Value.h"

extern const Value *FindBuiltInConstant(const char *name);

struct BuiltInConstantInfo
{
	const char *name;
	Value value;
};

// Let another module register built-in constants. So if GSBasic is embedded in another
// app, this lets that app add built-in constants to GSBasic
// THE LIST OF BUILT-IN CONSTANTS MUST BE IN ALPHABETICAL ORDER!!!!
void RegisterBuiltInConstants(const BuiltInConstantInfo *builtInConstants, int builtInConstantsCount);

#endif //!_BUILTINCONSTANTS_H_