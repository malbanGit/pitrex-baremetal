#ifndef _STRUCTDEF_H
#define _STRUCTDEF_H

#include "Array.h"
#include "GSBString.h"
#include "Error.h"
#include <stdlib.h>

class Executive;
class RunTimeEnvironment;

class StructDef
{
public:
	StructDef(const char *name) : _structName(name) {}
	~StructDef() {}

	const String &Name() const { return _structName; }
	void SetStructName(const String &name) { _structName = name; }
	void SetMemberNames(const Array<String> &members) 
	{
		_memberNames.resize(members.size());
		for (size_t i = 0; i < members.size(); ++i)
		{
			_memberNames[i].name = members[i];
			_memberNames[i].index = i;
		}
		Sort();
	}

	size_t MemberCount() const { return _memberNames.size(); }

	ErrorCode Construct(Executive &exec, RunTimeEnvironment &runTimeEnvironment, int argumentCount) const;

	int NameToIndex(const char *name) const;
	const String &IndexToName(size_t index) const;

	struct NameIndex
	{
		String name;
		size_t index;

		static int Compare(const void *key, const void *elem);
	};

private:
	void Sort();

	String _structName;
	Array<NameIndex> _memberNames;
};

#endif
