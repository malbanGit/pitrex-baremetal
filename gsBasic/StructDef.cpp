#include "StructDef.h"
#include "Environment.h"

ErrorCode StructDef::Construct(Executive &, RunTimeEnvironment & runTimeEnvironment, int argumentCount) const
{
	// Instantiate a structure
	Value value(this);
	// Initialize the members
	if (argumentCount)
	{
		int i;
		for (i = argumentCount - 1; i >= 0; --i)
		{
			value.MemberAt(i) = runTimeEnvironment.calculationStack.back();
			runTimeEnvironment.calculationStack.pop_back();
		}
		for (i = (int) MemberCount() - 1; i >= argumentCount; --i)
		{
			value.MemberAt(i) = 0;
		}
	}
	runTimeEnvironment.SetReturnValue(value);
	return 0;
}

int StructDef::NameToIndex(const char * name) const
{
	void *found = bsearch(name, _memberNames.data(), _memberNames.size(), sizeof(NameIndex), NameIndex::Compare);
	if (found)
		return ((NameIndex *) found)->index;
	return -1;
}

const String &StructDef::IndexToName(size_t index) const
{
	static String empty;
	for (size_t i = 0; i < _memberNames.size(); ++i)
	{
		if (_memberNames[i].index == index)
			return _memberNames[i].name;
	}
	return empty;
}

void StructDef::Sort()
{
	bool swapped;
	do
	{
		swapped = false;
		// This is a short list, so we do a dumb sort
		for (size_t i = 1; i < _memberNames.size(); ++i)
		{
			if (_memberNames[i - 1].name > _memberNames[i].name)
			{
				NameIndex temp = _memberNames[i];
				_memberNames[i] = _memberNames[i - 1];
				_memberNames[i - 1] = temp;
				swapped = true;
			}
		}
	} while (swapped);
}

int StructDef::NameIndex::Compare(const void * key, const void * elem)
{
	const char *skey = (const char *) key;
	const NameIndex *ni = (const NameIndex *) elem;
	return strcmp(skey, ni->name.c_str());
}
