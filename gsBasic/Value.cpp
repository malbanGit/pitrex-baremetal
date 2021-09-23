
#include "Value.h"
#include "GSBString.h"
#include "Array.h"
#include <stdio.h>

Value::Value(const uint16_t *dimensions, int numberOfDimensions) :
	_typeBehaviors(TypeBehaviorsArray::Singleton())
{
	_d = new DataRefCntPtr_t;
	_d->refCnt = 1;
	ArrayData_t *ad = new ArrayData_t;
	_d->data = ad;
	ad->dimensionCount = uint16_t(numberOfDimensions);
	ad->totalSize = 1;

	int i;
	for (i = 0; i < numberOfDimensions; ++i)
	{
		ad->dimensions[i] = dimensions[i];
		ad->totalSize *= dimensions[i];
	}
    
	if (ad->totalSize)
		ad->values = new Value[ad->totalSize];
	else
		ad->values = null;
}

Value::Value(const StructDef *structDef) :
	_typeBehaviors(TypeBehaviorsStruct::Singleton())
{
	_d = new DataRefCntPtr_t;
	_d->refCnt = 1;
	StructData_t *sd = new StructData_t;
	_d->data = sd;
	sd->structDef = structDef;

	sd->values = new Value[structDef->MemberCount()];
}

Value &Value::operator=(const Value &src)
{
	if (&src != this)
	{
		if (!IsByteReference())
		{
			_typeBehaviors->DestroyValue(this);
			_typeBehaviors = src.TypeBehaviors();
		}
		src.TypeBehaviors()->CopyTo(src, *this);
	}
	return *this;
}

Value &Value::operator=(int i)
{
	_typeBehaviors->DestroyValue(this);
	_typeBehaviors = TypeBehaviorsInt::Singleton();
	_i = i;
	return *this;
}

Value &Value::operator=(float f)
{
	_typeBehaviors->DestroyValue(this);
	_typeBehaviors = TypeBehaviorsFloat::Singleton();
	_f = f;
	return *this;
}

Value &Value::operator=(const char *s)
{
	_typeBehaviors->DestroyValue(this);
	_typeBehaviors = TypeBehaviorsString::Singleton();
	_s = new StringRefCntPtr_t;
	_s->s = s;
	_s->refCnt = 1;
	return *this;
}

Value &Value::operator=(const ::String &s)
{
	_typeBehaviors->DestroyValue(this);
	_typeBehaviors = TypeBehaviorsString::Singleton();
	_s = new StringRefCntPtr_t;
	_s->s = s;
	_s->refCnt = 1;
	return *this;
}

Value &Value::operator=(GSDataObject *obj)
{
	_typeBehaviors->DestroyValue(this);
	_typeBehaviors = TypeBehaviorsHandle::Singleton();
	_d = new DataRefCntPtr_t;
	_d->data = obj;
	_d->refCnt = 1;
	return *this;
}

Value &Value::operator=(Value *byRef)
{
	_typeBehaviors->DestroyValue(this);
	_typeBehaviors = TypeBehaviorsByRef::Singleton();
	_byRef = byRef;
	return *this;
}

int Value::MemberIndex(const::String & memberName) const
{
	StructData_t *sd = (StructData_t *) _d->data;
	return sd->structDef->NameToIndex(memberName.c_str());
}
