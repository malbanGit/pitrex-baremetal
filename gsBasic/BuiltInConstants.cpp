#include "BuiltInConstants.h"
#include "GSBCommon.h"

static const BuiltInConstantInfo *registeredBuiltInConstants;
static int registeredBuiltInConstantsCount;

// Let another module register built-in functions. So if GSBasic is embedded in another
// app, this lets that app add built-in functions to GSBasic
// THE LIST OF BUILT-IN FUNCTIONS MUST BE IN ALPHABETICAL ORDER!!!!
void RegisterBuiltInConstants(const BuiltInConstantInfo *externBuiltInConstants, int externBuiltInConstantsCount)
{
	registeredBuiltInConstants = externBuiltInConstants;
	registeredBuiltInConstantsCount = externBuiltInConstantsCount;
}

static int BuiltInConstantCmp(const void *key, const void *elem)
{
	const char *name = (const char *) key;
	const BuiltInConstantInfo *info = (const BuiltInConstantInfo *) elem;
    
	return strcasecmp(name, info->name);
}

const Value *FindBuiltInConstant(const char *name)
{
    // THIS LIST MUST BE IN ALPHABETICAL ORDER!!!!
    static const BuiltInConstantInfo builtInConstants[] = {
        {"false", Value(0)},
        {"nil", Value()},
        {"pi", Value((float) 3.1415926)},
        {"true", Value(1)},
    };

    void *found = bsearch(name, builtInConstants, countof(builtInConstants), sizeof(struct BuiltInConstantInfo), BuiltInConstantCmp);
	if (found == null && registeredBuiltInConstants)
	{
		found = bsearch(name, registeredBuiltInConstants, registeredBuiltInConstantsCount, sizeof(struct BuiltInConstantInfo), BuiltInConstantCmp);
	}
	if (found)
	{
		const BuiltInConstantInfo *info = (const BuiltInConstantInfo *) found;
		return &info->value;
	}

	return null;
}
