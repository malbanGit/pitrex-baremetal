
#ifndef _VALUE_H
#define _VALUE_H

#include "TypeBehaviors.h"
#include "StructDef.h"


#ifdef __cplusplus
extern "C" {
#endif
#include <vectrex/vectrexInterface.h>

  #ifdef __cplusplus
}
#endif


template <class T> class Array;
class String;

class GSDataObject
{
public:
	virtual ~GSDataObject() {}
};

class Value
{
private:
	struct RefCntPtr_t
	{
		int refCnt;
	};
	struct DataRefCntPtr_t : public RefCntPtr_t
	{
		void *data;
	};
	struct StringRefCntPtr_t : public RefCntPtr_t
	{
		String s;
	};
	struct ArrayData_t
	{
		Value *values;
		uint16_t dimensions[MAX_ARRAY_DIMENSIONS];
		uint16_t dimensionCount;
		size_t totalSize;
	};
	struct StructData_t
	{
		Value *values;
		const StructDef *structDef;
	};

public:
	Value() : _typeBehaviors(TypeBehaviorsUninitialized::Singleton()) {}
	Value(int i) : _typeBehaviors(TypeBehaviorsInt::Singleton()) { _i = i; }
	Value(float f) : _typeBehaviors(TypeBehaviorsFloat::Singleton()) { _f = f; }
	Value(double f) : _typeBehaviors(TypeBehaviorsFloat::Singleton()) { _f = float(f); }
	Value(uint8_t *s) : _typeBehaviors(TypeBehaviorsByteReference::Singleton()) { _byte = s; }
	Value(const char *s) : _typeBehaviors(TypeBehaviorsUninitialized::Singleton()) { *this = s; }
	Value(const ::String &s) : _typeBehaviors(TypeBehaviorsUninitialized::Singleton()) { *this = s; }
	Value(const Value &src) : _typeBehaviors(TypeBehaviorsUninitialized::Singleton()) { *this = src; }
	Value(const uint16_t *dimensions, int numberOfDimensions);
	Value(const StructDef *sdh);
	Value(GSDataObject *obj) : _typeBehaviors(TypeBehaviorsUninitialized::Singleton()) { *this = obj; }
	Value(Value *pvalue) : _typeBehaviors(TypeBehaviorsByRef::Singleton()) { *this = pvalue; }
	~Value() { 
      _typeBehaviors->DestroyValue(this); }

	const ::TypeBehaviors *TypeBehaviors() const 
	{
		return _typeBehaviors;
	}

	Value &operator=(const Value &src);
	Value &operator=(int i);
	Value &operator=(float f);
	Value &operator=(const char *s);
	Value &operator=(const ::String &s);
	Value &operator=(GSDataObject *obj);
	Value &operator=(Value *byRef);

	bool IsUninitialized() const { return _typeBehaviors == TypeBehaviorsUninitialized::Singleton(); }
	bool IsInteger() const { return _typeBehaviors == TypeBehaviorsInt::Singleton(); }
	bool IsFloat() const { return _typeBehaviors == TypeBehaviorsFloat::Singleton(); }
	bool IsNumeric() const { return IsInteger() || IsFloat(); }
	bool IsString() const { return _typeBehaviors == TypeBehaviorsString::Singleton(); }
	bool IsArray() const { return _typeBehaviors == TypeBehaviorsArray::Singleton(); }
	bool IsStruct() const { return _typeBehaviors == TypeBehaviorsStruct::Singleton(); }
	bool IsHandle() const { return _typeBehaviors == TypeBehaviorsHandle::Singleton(); }
	bool IsByRef() const { return _typeBehaviors == TypeBehaviorsByRef::Singleton(); }
	bool IsByteReference() const { return _typeBehaviors == TypeBehaviorsByteReference::Singleton(); }

	void Clear() 
	{
		_typeBehaviors->DestroyValue(this); 
		_typeBehaviors = TypeBehaviorsUninitialized::Singleton(); 
	}

	void Dump() const { Print(); }
	void Print() const { _typeBehaviors->Print(*this); }

	// This only works if the Value is an integer
	int Integer() const { return _i; }
	// This only works if the Value is a float
	float Float() const { return _f; }
	// This only works if the Value is a string
	const ::String &String() const { return _s->s; }
	::String &String() { return _s->s; }
	// This only works if the Value is a ByRef
	Value *Dereference() const { return _byRef; }
	// This only works if the Value is an byte reference
	int ByteValue() const { return *_byte; }
	void WriteToByte(uint8_t value) { *_byte = value; }
	// This only works if the Value is a handle
	GSDataObject *Handle() const { return (GSDataObject *) _d->data; }

	::String ToString() const { return _typeBehaviors->ToString(*this); }

	// Array access functions. The Value must be an array
	const uint16_t *Dimensions() const { return ((ArrayData_t *) _d->data)->dimensions; }
	uint16_t DimensionsSize() const { return ((ArrayData_t *) _d->data)->dimensionCount; }
    size_t NumberOfElements() const {return ((ArrayData_t *) _d->data)->totalSize;}

	// Returns the specified item. This treats the ValueArray as a one-dimensional array
	const Value &operator[](int i) const { return((ArrayData_t *) _d->data)->values[i]; }
	Value &operator[](int i) { return((ArrayData_t *) _d->data)->values[i]; }

    // Read the values in a 2D array, convert to the specified type, and put them in a C array
    template <class T>
    ErrorCode GetArrayValues(T *results) const
    {
        if (!IsArray())
            return IllegalArgument;
        ErrorCode error;
        for (size_t i = 0; i < NumberOfElements(); ++i)
        {
            error = (*this)[i].TypeBehaviors()->ToValue((*this)[i], results + i);
            if (error)
                return error;
        }

        return 0;
    }

	// Struct access functions
	const StructDef *StructType() const { return ((StructData_t *) _d->data)->structDef; }
	int MemberIndex(const ::String &memberName) const;
	Value &MemberAt(int i) { return ((StructData_t *) _d->data)->values[i]; }
	const Value &MemberAt(int i) const { return ((StructData_t *) _d->data)->values[i]; }

private:
	const ::TypeBehaviors *_typeBehaviors;

	void Release() { _typeBehaviors->DestroyValue(this); }

	friend class TypeBehaviorsInt;
	friend class TypeBehaviorsFloat;
	friend class TypeBehaviorsString;
	friend class TypeBehaviorsArray;
	friend class TypeBehaviorsStruct;
	friend class TypeBehaviorsHandle;
	friend class TypeBehaviorsByRef;
	friend class TypeBehaviorsByteReference;
	union
	{
		int _i;
		float _f;
		DataRefCntPtr_t *_d;
		StringRefCntPtr_t *_s;
		Value *_byRef;
		uint8_t *_byte;
	};
    static_assert(sizeof(float) == sizeof(int), "Float and int should be the same size!");
};

class FilePointer : public GSDataObject
{
public:
    FilePointer(FILE *fp) : _fp(fp) {}
    ~FilePointer() 
    {
      if (FileP() != 0)
        fclose(FileP());
      _fp = null;
      
    }
    
    void Invalidate() {_fp = null;}
    FILE *FileP() {return _fp;}
    
private:
    FILE *_fp;
};

#endif
