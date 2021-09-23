#include "Constant.h"

PoolAllocator<ConstantInt> ConstantInt::_pool;
PoolAllocator<ConstantFloat> ConstantFloat::_pool;
PoolAllocator<ConstantString, 10> ConstantString::_pool;

Constant *Constant::Create(Token *token)
{
	switch (token->Type())
	{
	case Token::Int :
	{
		int i = token->ValueInt();
		return new ConstantInt(i);
	}

	case Token::Float :
		return new ConstantFloat(token->ValueFloat());

	case Token::String :
		return new ConstantString(token->ValueString());

	case Token::BuiltInConstant :
		return Create(token->ValueBuiltInConstant());
        
    default :
        return null;
	}
}

Constant *Constant::Create(const ::Value &value)
{
	if (value.IsInteger())
		return new ConstantInt(value.Integer());

	if (value.IsFloat())
		return new ConstantFloat(value.Float());

	if (value.IsString())
		return new ConstantString(value.String());

	if (value.IsUninitialized())
		return new ConstantNil();

	return new ConstantValue(value);
}
