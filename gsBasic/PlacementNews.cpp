// This is a kludge to allow us to use MSVC's memory leak detection. It normally
// doesn't work with placement new, so we move all the calls to placement new into
// a separate file

#include "Array.h"
#include "Environment.h"
#include "Token.h"
#include "Value.h"
#include "StructDef.h"
#ifdef DBG_NEW
#undef new
#endif
#include <new>

template <class T>
void Array<T>::PlacementNew(void *addr)
{
	new (addr) T;
}

template class Array<String>;

template <>
void Array<int>::PlacementNew(void *addr)
{
	*((int *) addr) = 0;
}

template <>
void Array<char>::PlacementNew(void *addr)
{
	*((char *) addr) = 0;
}

template <>
void Array<uint8_t>::PlacementNew(void *addr)
{
	*((uint8_t *) addr) = 0;
}

template <>
void Array<uint16_t>::PlacementNew(void *addr)
{
	*((uint16_t *) addr) = 0;
}

template <>
void Array<size_t>::PlacementNew(void *addr)
{
	*((size_t *) addr) = 0;
}

template <>
void Array<Value>::PlacementNew(void *addr)
{
	new (addr) Value;
}

template <>
void Array<StructDef::NameIndex>::PlacementNew(void *addr)
{
	new (addr) StructDef::NameIndex;
}

template <>
void Array<Token>::PlacementNew(void *addr)
{
	new (addr) Token;
}

#define PtrSpecialization(cls) \
	class cls; \
	template <> \
	void Array<cls *>::PlacementNew(void *addr) \
	{ \
		*((cls **) addr) = 0; \
	}

PtrSpecialization(CallSpec)
PtrSpecialization(ControlStructure)
PtrSpecialization(Expression)
PtrSpecialization(Statement)
PtrSpecialization(StatementElseif)
PtrSpecialization(StatementProcedure)
PtrSpecialization(StructDef)
PtrSpecialization(Symbol)
PtrSpecialization(Token)
PtrSpecialization(VariableScope)
PtrSpecialization(Variable)
PtrSpecialization(VarSpec)
PtrSpecialization(VarSpecWithElements)
