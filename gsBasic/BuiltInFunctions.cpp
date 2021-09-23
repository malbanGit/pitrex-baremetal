#include "GSBCommon.h"
#include "BuiltInFunctions.h"
#include "Executive.h"
#include "Environment.h"
#include "Value.h"
#include <math.h>
#include <stdlib.h>
#include <algorithm>


#undef max

#ifdef __cplusplus
extern "C" {
#endif

#include <vectrex/vectrexInterface.h>
typedef DIR Dir_t;

#ifdef __cplusplus
}
#endif


extern "C" unsigned RandomSeed(); // Must be implemented in user code, i.e. the embedding app

static ErrorCode Abs(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &vnum = runTimeEnvironment.calculationStack.back();
	if (!vnum.IsNumeric())
	{
		return MustBeNumeric;
	}
	else if (vnum.IsInteger())
	{
		if (vnum.Integer() < 0)
			vnum = -vnum.Integer();
	}
	else
	{
		if (vnum.Float() < 0)
			vnum = -vnum.Float();
	}

	runTimeEnvironment.SetReturnValue(vnum);
	return 0;
}

// Append two arrays
static ErrorCode AppendArrays(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &varg2 = runTimeEnvironment.calculationStack.back();
	Value &varg1 = runTimeEnvironment.calculationStack.back(1);

	if (!varg1.IsArray() || !varg2.IsArray())
	{
		return IllegalArgument;
	}

	// Make sure the arrays have the same number of dimensions and the sizes of
	// their dimensions match (except for the first dimension - that doesn't have to match)
	const uint16_t *dim1 = varg1.Dimensions(), *dim2 = varg2.Dimensions();
	if (varg1.DimensionsSize() != varg2.DimensionsSize())
	{
		return IllegalArgument;
	}
	Array<uint16_t> newDims;
	newDims.push_back(dim1[0] + dim2[0]);
	int rowSize = 1;
	int i;
	for (i = 1; i < (int) varg1.DimensionsSize(); ++i)
	{
		if (dim1[i] != dim2[i])
			return IllegalArgument;
		newDims.push_back(dim1[i]);
		rowSize *= dim1[i];
	}

	// Create an array big enough to hold both
	Value merged(newDims.Raw(), newDims.size());
	int array1Index = 0;
	for (i = 0; i < dim1[0]; ++i)
	{
		for (int j = 0; j < rowSize; ++j)
		{
			merged[array1Index] = varg1[array1Index];
			++array1Index;
		}
	}
	int array2Index = 0, mergedIndex = array1Index;
	for (i = 0; i < dim2[0]; ++i)
	{
		for (int j = 0; j < rowSize; ++j)
		{
			merged[mergedIndex]= varg2[array2Index];
			++mergedIndex;
			++array2Index;
		}
	}

	runTimeEnvironment.SetReturnValue(merged);

	return 0;
}

static ErrorCode Asc(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &vtext = runTimeEnvironment.calculationStack.back();
	if (!vtext.IsString())
		return IllegalArgument;
	int asc = (uint8_t) vtext.String().c_str()[0];
	runTimeEnvironment.SetReturnValue(Value(asc));
	return 0;
}

static ErrorCode ByteArray(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &vnum = runTimeEnvironment.calculationStack.back();
	if (!vnum.IsInteger() || vnum.Integer() < 0)
		return IllegalArgument;
	String s('\0', vnum.Integer());
	runTimeEnvironment.SetReturnValue(Value(s));

	return 0;
}

static ErrorCode Chr(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	ErrorCode code = 0;
	Value &vchr = runTimeEnvironment.calculationStack.back();
	if (!vchr.IsInteger())
		code = MustBeInteger;
	else
	{
		int chr = vchr.Integer();
		if (chr > 255 || chr < 0)
			code = IllegalArgument;
		else
		{
			char buf[2] = {char(chr)};
			Value vs = buf;
			runTimeEnvironment.SetReturnValue(vs);
		}
	}

	return code;
}

static ErrorCode ClearLastError(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	runTimeEnvironment.ClearLastError();
	return 0;
}

// Copy the values of an array
static ErrorCode CopyArray(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value varg1 = runTimeEnvironment.calculationStack.back();

	if (!varg1.IsArray())
	{
		return IllegalArgument;
	}

	const uint16_t *dims = varg1.Dimensions();

	// Create a new array
	Value copy(dims, varg1.DimensionsSize());
	for (size_t i = 0; i < varg1.NumberOfElements(); ++i)
	{
		copy[i] = varg1[i];
	}

	runTimeEnvironment.SetReturnValue(copy);

	return 0;
}

// Deep copy an array or struct
static ErrorCode DeepCopy(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	const Value &varg = runTimeEnvironment.calculationStack.back();
	Value copy;
	varg.TypeBehaviors()->DeepCopy(varg, copy, true);
	runTimeEnvironment.SetReturnValue(copy);

	return 0;
}

// Return an array of file names
static ErrorCode Dir(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
#ifdef __XC32
	Dir_t *dir = opendir(CurrentDirectoryName());
#else
	Dir_t *dir = opendir((char *) ".");
#endif

	Array<String> fileNames;
	if (dir)
	{
		/*struct*/ dirent *de;
		while ((de = readdir(dir)) != null)
		{
			// Save the file or directory name as long as it's not . or ..
			if ((strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) && (de->d_name[0] != '.'))
			{
				fileNames.push_back(String(de->d_name));
				if (de->d_type == DT_DIR)
					fileNames.back() += "/";
			}
		}

		closedir(dir);
	}
	fileNames.sort(&String::CompareCaseInsensitive);
	uint16_t dims[1] = {uint16_t(fileNames.size())};
	Value v(dims, 1);
	for (size_t i = 0; i < fileNames.size(); ++i)
	{
		v[i] = fileNames[i];
	}
	runTimeEnvironment.SetReturnValue(v);

	return 0;
}

// Convert a number to a float
static ErrorCode Float(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	ErrorCode code = 0;
	Value &vnum = runTimeEnvironment.calculationStack.back();
	if (vnum.IsInteger())
		runTimeEnvironment.SetReturnValue(Value(float(vnum.Integer())));
	else if (vnum.IsFloat())
		runTimeEnvironment.SetReturnValue(vnum);
	else
		code = MustBeNumeric;

	return code;
}

static ErrorCode GetLastError(Executive &exec, RunTimeEnvironment &runTimeEnvironment, int)
{
	static uint16_t dims[] = {5};
	Value verror(dims, 1);

	Error error = runTimeEnvironment.GetLastError();
	if (error.Message())
	{
		verror[0] = Value(atoi(error.Message() + 1));
		verror[1] = Value(error.Message() + 6);
	}
	else
	{
		verror[0] = Value((int) 0);
		verror[1] = Value("");
	}
	verror[2] = Value((int) error.LineNumber());
	size_t fileIndex = error.FileIndex();
	verror[3] = Value((int) error.Column());
	verror[4] = Value(exec.SourceFileName(fileIndex));

	runTimeEnvironment.SetReturnValue(verror);

	return 0;
}

// Find one string within another
// pos = Instr("Hello", "l") returns 3 (first position is 1)
// pos = Instr(4, "Hello", "l") returns 4
static ErrorCode Instr(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
	Value &vneedle = runTimeEnvironment.calculationStack.back();
	Value &vhaystack = runTimeEnvironment.calculationStack.back(1);
    Value vstart(1);
    if (count == 3)
        vstart = runTimeEnvironment.calculationStack.back(2);
    if (!vneedle.IsString() || !vhaystack.IsString() || !vstart.IsInteger() || vstart.Integer() < 1)
        return IllegalArgument;
    const char *haystack = vhaystack.String().c_str();
    unsigned int start = (unsigned int) vstart.Integer();
    const char *pos;
    if (start > vhaystack.String().length())
        runTimeEnvironment.SetReturnValue(Value(0));
    else
    {
        pos = strstr(haystack + start - 1, vneedle.String().c_str());
        if (pos)
            runTimeEnvironment.SetReturnValue(Value(int(pos - haystack + 1)));
        else
            runTimeEnvironment.SetReturnValue(Value(0));
    }
    return 0;
}

// Convert a number to an int
static ErrorCode Int(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	ErrorCode code = 0;
	Value &vnum = runTimeEnvironment.calculationStack.back();
	if (vnum.IsInteger())
		runTimeEnvironment.SetReturnValue(vnum);
	else if (vnum.IsFloat())
		runTimeEnvironment.SetReturnValue(Value(int(vnum.Float())));
	else
		code = MustBeNumeric;

	return code;
}

// Given a variable and a dimension index, return the lower bound of that dimension.
// Since OPTION BASE is global, it just returns the option base value
static ErrorCode LBound(Executive &, RunTimeEnvironment &runTimeEnvironment, int numberOfArguments)
{
	ErrorCode code = 0;

	Value dim;
	size_t depth = 0;
	if (numberOfArguments == 2)
	{
		dim = runTimeEnvironment.calculationStack.back();
		depth = 1;
	}
	else
		dim = 1;
	Value &var = runTimeEnvironment.calculationStack.back(depth);

	if (!dim.IsInteger())
		code = MustBeInteger;
	else
	{
		int result = ARRAY_BASE - 1;
		int idim = dim.Integer();
		if (idim < 1)
			return IllegalArgument;
		if (var.IsArray())
		{
			result = ARRAY_BASE;
		}

		runTimeEnvironment.SetReturnValue(Value(result));
	}

	return code;
}

// l = Left(string, n)
// Return the left n characters of string
static ErrorCode Left(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &vn = runTimeEnvironment.calculationStack.back();
	Value &vs = runTimeEnvironment.calculationStack.back(1);
	if (!vn.IsInteger() || !vs.IsString())
		return IllegalArgument;

	int n = vn.Integer();
	if (n < 0)
		return IllegalArgument;
	String s = vs.ToString();
	if (n >= (int) s.length())
		runTimeEnvironment.SetReturnValue(vs);
	else
		runTimeEnvironment.SetReturnValue(Value(s.Substr(0, n)));

	return 0;
}

// l = Len(string)
// Return the length of string
static ErrorCode Len(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &vs = runTimeEnvironment.calculationStack.back();
	if (!vs.IsString())
		return IllegalArgument;

	runTimeEnvironment.SetReturnValue((int) vs.String().length());

	return 0;
}

static ErrorCode Max(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &varg2 = runTimeEnvironment.calculationStack.back();
	Value &varg1 = runTimeEnvironment.calculationStack.back(1);

	Value vresult;
	Error error = varg1.TypeBehaviors()->GreaterThan(varg1, varg2, 0, 0, vresult);
	if (!error)
	{
		if (vresult.Integer())
		{
			runTimeEnvironment.SetReturnValue(varg1);
		}
		else
		{
			runTimeEnvironment.SetReturnValue(varg2);
		}
	}

	return error.Message();
}

// m = Mid(string, start[, n])
// Return n characters of string starting at position start (first character is start == 1)
// If n is not specified, return from start to the end of the string
static ErrorCode Mid(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
	Value vn;
	size_t depth = 0;
	if (count == 3)
	{
		vn = runTimeEnvironment.calculationStack.back();
		depth = 1;
	}
	Value &vstart = runTimeEnvironment.calculationStack.back(depth);
	Value vs = runTimeEnvironment.calculationStack.back(depth + 1);
	if ((count == 3 && !vn.IsInteger()) || !vstart.IsInteger() || !vs.IsString())
		return IllegalArgument;

	int start = vstart.Integer() - 1;
	String s = vs.ToString();
	int n = (count == 3) ? vn.Integer() : std::max((int) s.length() - start, 0);
	if (start < 0 || n < 0)
		return IllegalArgument;
	runTimeEnvironment.SetReturnValue(Value(s.Substr(start, n)));

	return 0;
}

static ErrorCode Min(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &varg2 = runTimeEnvironment.calculationStack.back();
	Value &varg1 = runTimeEnvironment.calculationStack.back(1);

	Value vresult;
	Error error = varg1.TypeBehaviors()->LessThan(varg1, varg2, 0, 0, vresult);
	if (!error)
	{
		if (vresult.Integer())
		{
			runTimeEnvironment.SetReturnValue(varg1);
		}
		else
		{
			runTimeEnvironment.SetReturnValue(varg2);
		}
	}

	return error.Message();
}

// Takes the Euclidean norm of a matrix
static ErrorCode Norm(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &mat = runTimeEnvironment.calculationStack.back();
	if (!mat.IsArray())
		return IllegalArgument;
	Value norm(0.0), sqr;
	for (size_t i = 0; i < mat.NumberOfElements(); ++i)
	{
		Error error = mat[i].TypeBehaviors()->Mul(mat[i], mat[i], 0, 0, sqr);
		if (error)
			return error.Message();
		error = norm.TypeBehaviors()->Plus(norm, sqr, 0, 0, norm);
		if (error)
			return error.Message();
	}
	SetReturnValue(runTimeEnvironment, Value(sqrtf(norm.Float())));
	return 0;
}

// Calculate x ^ y
static ErrorCode Pow(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &vy = runTimeEnvironment.calculationStack.back();
	Value &vx = runTimeEnvironment.calculationStack.back(1);
	if (!vx.IsNumeric() || !vy.IsNumeric())
		return IllegalArgument;
	Value result;
	vx.TypeBehaviors()->Exponent(vx, vy, 0, 0, result);
	runTimeEnvironment.SetReturnValue(result);

	return 0;
}

// r = Right(string, n)
// Return the right n characters of string
static ErrorCode Right(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &vn = runTimeEnvironment.calculationStack.back();
	Value &vs = runTimeEnvironment.calculationStack.back(1);

	if (!vn.IsInteger() || !vs.IsString())
		return IllegalArgument;

	int n = vn.Integer();
	String s = vs.ToString();
	if (n < 0)
		return IllegalArgument;
	if (n >= (int) s.length())
		runTimeEnvironment.SetReturnValue(vs);
	else
		runTimeEnvironment.SetReturnValue(Value(s.Substr(s.length() - n, n)));

	return 0;
}

static ErrorCode Round(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	ErrorCode code = 0;
	Value &vnum = runTimeEnvironment.calculationStack.back();
	if (vnum.IsInteger())
	{
		runTimeEnvironment.SetReturnValue(vnum);
	}
	else if (vnum.IsFloat())
	{
		float f = vnum.Float();
		runTimeEnvironment.SetReturnValue(Value(roundf(f)));
	}
	else
		code = MustBeNumeric;

	return code;
}

// s = Sgn(x)
// Returns 0, -1, or  based on whether x is ==, <, or > 0
static ErrorCode Sgn(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &vx = runTimeEnvironment.calculationStack.back();
	int result;
	if (vx.IsInteger())
	{
		if (vx.Integer() == 0)
			result = 0;
		else if (vx.Integer() < 0)
			result = -1;
		else
			result = 1;
	}
	else if (vx.IsFloat())
	{
		if (vx.Float() == 0)
			result = 0;
		else if (vx.Float() < 0)
			result = -1;
		else
			result = 1;
	}
	else
		return IllegalArgument;

	runTimeEnvironment.SetReturnValue(Value(result));
	return 0;
}

// Given a variable and a dimension index, return the size (number of elements) of that dimension.
// E.g.: 
//    dim a[10, 11]
//    d1 = dimension(a, 1) ' returns 10
//    d2 = dimension(a, 2) ' returns 11
//    d3 = dimension(a, 3) ' returns 0
//    b = 1
//    d2 = dimension(b, 1) ' returns 0
static ErrorCode Size(Executive &, RunTimeEnvironment &runTimeEnvironment, int numberOfArguments)
{
	ErrorCode code = 0;

	Value dim;
	size_t depth = 0;
	if (numberOfArguments == 2)
	{
		dim = runTimeEnvironment.calculationStack.back();
		depth = 1;
	}
	else
		dim = 1;
	Value &var = runTimeEnvironment.calculationStack.back(depth);

	if (!dim.IsInteger())
		code = MustBeInteger;
	else
	{
		int result = 0;
		int idim = dim.Integer();
		if (idim < 1)
			return IllegalArgument;
		if (var.IsArray())
		{
			const uint16_t *dimensions = var.Dimensions();
			if (idim >= 1 && idim <= (int) var.DimensionsSize())
				result = dimensions[idim - 1];
		}

		runTimeEnvironment.SetReturnValue(Value(result));
	}

	return code;
}

static ErrorCode Sqrt(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	ErrorCode code = 0;
	Value &vnum = runTimeEnvironment.calculationStack.back();
    float f;
    code = vnum.TypeBehaviors()->ToFloat(vnum, &f);
    if (code == 0)
	{
		runTimeEnvironment.SetReturnValue(sqrtf(f));
	}

	return code;
}

// Compare two strings. Returns -1, 0, or 1
// diff = StrComp("Hi", "Bye") ' Case-sensitive comparison
// diff = StrComp("Hi", "Bye", 0) ' Case-sensitive comparison
// diff = StrComp("Hi", "Bye", 1) ' Case-insensitive comparison
static ErrorCode StrComp(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
	Value vci(0);
	size_t depth = 0;
	if (count == 3)
	{
		vci = runTimeEnvironment.calculationStack.back();
		depth = 1;
	}
	Value &varg2 = runTimeEnvironment.calculationStack.back(depth);
	Value &varg1 = runTimeEnvironment.calculationStack.back(depth + 1);

	if (!varg1.IsString() || !varg2.IsString() || !vci.IsInteger())
		return IllegalArgument;

	int diff;
	if (vci.Integer())
		diff = String::CompareCaseInsensitive(varg1.String(), varg2.String());
	else
		diff = String::Compare(varg1.String(), varg2.String());
	if (diff < 0) diff = -1;
	else if (diff > 0) diff = 1;

	runTimeEnvironment.SetReturnValue(Value(diff));

	return 0;
}

// Return a string copy of the argument, converted to lower case
static ErrorCode ToLower(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &vs = runTimeEnvironment.calculationStack.back();
	if (!vs.IsString())
		return IllegalArgument;

	Value lower("");
	char *dup = strdup(vs.String().c_str());
	if (dup)
	{
		char *p = dup;
		while (*p)
		{
			*p = char(tolower(*p));
			++p;
		}
		lower = dup;
		free(dup);
	}

	runTimeEnvironment.SetReturnValue(lower);

	return 0;
}

// Return a string copy of the argument, converted to upper case
static ErrorCode ToUpper(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &vs = runTimeEnvironment.calculationStack.back();
	if (!vs.IsString())
		return IllegalArgument;

	Value upper("");
	char *dup = strdup(vs.String().c_str());
	if (dup)
	{
		char *p = dup;
		while (*p)
		{
			*p = char(toupper(*p));
			++p;
		}
		upper = dup;
		free(dup);
	}

	runTimeEnvironment.SetReturnValue(upper);

	return 0;
}

// Truncate a number's fractional part
static ErrorCode Truncate(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	ErrorCode code = 0;
	Value &vnum = runTimeEnvironment.calculationStack.back();
	if (vnum.IsInteger())
		runTimeEnvironment.SetReturnValue(Value((float) vnum.Integer()));
	else if (vnum.IsFloat())
		runTimeEnvironment.SetReturnValue(Value(float((int) vnum.Float())));
	else
		code = MustBeNumeric;

	return code;
}

// Given a variable and a dimension index, return the size of that dimension.
// E.g.: 
//    dim a[10, 11]
//    d1 = dimension(a, 1) ' returns 10
//    d2 = dimension(a, 2) ' returns 11
//    d3 = dimension(a, 3) ' returns 0
//    b = 1
//    d2 = dimension(b, 1) ' returns 0
static ErrorCode UBound(Executive &exec, RunTimeEnvironment &runTimeEnvironment, int numberOfArguments)
{
	ErrorCode code = Size(exec, runTimeEnvironment, numberOfArguments);
	if (code == 0  && ARRAY_BASE == 0)
	{
		bool valid;
		const Value &result = runTimeEnvironment.RetrieveReturnValue(&valid);
		runTimeEnvironment.SetReturnValue(Value(result.Integer() - 1));
	}

	return code;
}

// number = Val(string)
static ErrorCode Val(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &arg = runTimeEnvironment.calculationStack.back();
	if (!arg.IsString())
		return IllegalArgument;
	char *end;
	double val = strtod(arg.String().c_str(), &end);
	SetReturnValue(runTimeEnvironment, Value(val));
	return 0;
}

static ErrorCode Rand(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	runTimeEnvironment.SetReturnValue(Value(rand()));
	return 0;
}

static ErrorCode Randomize(Executive &, RunTimeEnvironment &runTimeEnvironment, int argc)
{
	if (argc)
	{
		Value &arg = runTimeEnvironment.calculationStack.back();
		if (!arg.IsInteger())
			return IllegalArgument;
		srand((unsigned int) arg.Integer());
	}
	else
		srand(RandomSeed());
	return 0;
}

static ErrorCode Trig(RunTimeEnvironment &runTimeEnvironment, float (*fn)(float))
{
	Value &varg = runTimeEnvironment.calculationStack.back();

    float arg;
    ErrorCode error = varg.TypeBehaviors()->ToFloat(varg, &arg);
	if (!error)
	{
		float result = (*fn)(arg);
		runTimeEnvironment.SetReturnValue(Value(result));
	}
	return error;
}

static ErrorCode ACos(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &varg = runTimeEnvironment.calculationStack.back();

    float arg;
    ErrorCode error = varg.TypeBehaviors()->ToFloat(varg, &arg);
    if (arg <-1.0) arg = -1.0;
    if (arg > 1.0) arg =  1.0;
	if (!error)
	{
		float result = acosf(arg);
		runTimeEnvironment.SetReturnValue(Value(result));
	}

	return error;
}

static ErrorCode ASin(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &varg = runTimeEnvironment.calculationStack.back();
    float arg;
    ErrorCode error = varg.TypeBehaviors()->ToFloat(varg, &arg);
    
    if (arg <-1.0) arg = -1.0;
    if (arg > 1.0) arg =  1.0;
    
    if (!error)
    {
        float result = asinf(arg);
        runTimeEnvironment.SetReturnValue(Value(result));
    }
    return error;
}

static ErrorCode ATan(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    return Trig(runTimeEnvironment, atanf);
}

static ErrorCode ATan2(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &vx = runTimeEnvironment.calculationStack.back();
	Value &vy = runTimeEnvironment.calculationStack.back(1);

    float x, y;
    ErrorCode error = vy.TypeBehaviors()->ToFloat(vy, &y);
	if (!error)
	{
        error = vx.TypeBehaviors()->ToFloat(vx, &x);
		float result = atan2f(y, x);
		runTimeEnvironment.SetReturnValue(Value(result));
	}
	return error;
}

static ErrorCode Cos(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    return Trig(runTimeEnvironment, cosf);
}

static ErrorCode Cross(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &arg2 = runTimeEnvironment.calculationStack.back();
	Value &arg1 = runTimeEnvironment.calculationStack.back(1);
	if (!arg1.IsArray() || arg1.DimensionsSize() != 1 || arg1.Dimensions()[0] != 3)
		return IllegalArgument;
	if (!arg2.IsArray() || arg2.DimensionsSize() != 1 || arg2.Dimensions()[0] != 3)
		return IllegalArgument;
	for (size_t i = 0; i < 3; ++i)
	{
		if (!arg1[i].IsNumeric() || !arg2[i].IsNumeric())
			return IllegalArgument;
	}

	// Create a 1-D array with three elements
	uint16_t dim = 3;
	Value result(&dim, 1);

	// Compute the cross product. We've already checked that everything's numeric,
	// so there shouldn't be any possibility of errors
	Value product;
	arg1[1].TypeBehaviors()->Mul(arg1[1], arg2[2], 0, 0, result[0]);
	arg1[2].TypeBehaviors()->Mul(arg1[2], arg2[1], 0, 0, product);
	result[0].TypeBehaviors()->Minus(result[0], product, 0, 0, result[0]);

	arg1[2].TypeBehaviors()->Mul(arg1[2], arg2[0], 0, 0, result[1]);
	arg1[0].TypeBehaviors()->Mul(arg1[0], arg2[2], 0, 0, product);
	result[1].TypeBehaviors()->Minus(result[1], product, 0, 0, result[1]);

	arg1[0].TypeBehaviors()->Mul(arg1[0], arg2[1], 0, 0, result[2]);
	arg1[1].TypeBehaviors()->Mul(arg1[1], arg2[0], 0, 0, product);
	result[2].TypeBehaviors()->Minus(result[2], product, 0, 0, result[2]);

	runTimeEnvironment.SetReturnValue(result);
	return 0;
}

static ErrorCode Sin(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    return Trig(runTimeEnvironment, sinf);
}

static ErrorCode Tan(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    return Trig(runTimeEnvironment, tanf);
}

// a = Transpose(array)
// Transposes columns and rows
static ErrorCode Transpose(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
	Value &arg = runTimeEnvironment.calculationStack.back();
	uint16_t dimSize = arg.DimensionsSize();
	if (!arg.IsArray() || dimSize > 2)
		return IllegalArgument;

	const uint16_t *dims = arg.Dimensions();
	uint16_t origRows, origCols;
	if (dimSize == 1)
	{
		origRows = 1;
		origCols = dims[0];
	}
	else
	{
		origRows = dims[0];
		origCols = dims[1];
	}
	uint16_t newDims[2] = {origCols, origRows}, newDimSize = dimSize;
	// If the original array had only one column, the new array will have only one row and be one dimensional
	if (origCols == 1)
	{
		newDimSize = 1;
		newDims[0] = newDims[1];
	}
	else
		newDimSize = 2;

	Value result(newDims, newDimSize);

	size_t origIndex = 0;
	for (uint16_t origRow = 0; origRow < origRows; ++origRow)
	{
		for (uint16_t origCol = 0; origCol < origCols; ++origCol)
		{
			result[origCol * origRows + origRow] = arg[origIndex];
			++origIndex;
		}
	}

	SetReturnValue(runTimeEnvironment, result);
	return 0;
}

// THIS LIST MUST BE IN ALPHABETICAL ORDER!!!!
static const BuiltInFunctionInfo builtInFunctions[] = {
	{"abs", Abs, 1},
	{"acos", ACos, 1},
	{"appendarrays", AppendArrays, 2},
	{"asc", Asc, 1},
	{"asin", ASin, 1},
    {"atan", ATan, 1},
    {"atan2", ATan2, 2},
	{"ByteArray", ByteArray, 1},
	{"chr", Chr, 1},
	{"ClearLastError", ClearLastError, 0},
	{"copyarray", CopyArray, 1},
	{"cos", Cos, 1},
    {"cross", Cross, 2},
	{"deepcopy", DeepCopy, 1},
	{"dir", Dir, 0},
	{"float", Float, 1},
	{"GetLastError", GetLastError, 0},
    {"instr", Instr, 2, 3},
	{"int", Int, 1},
	{"lbound", LBound, 1, 2},
	{"left", Left, 2},
	{"len", Len, 1},
	{"max", Max, 2},
	{"mid", Mid, 2, 3},
	{"min", Min, 2},
	{"norm", Norm, 1},
	{"pow", Pow, 2},
	{"rand", Rand, 0},
	{"randomize", Randomize, 0, 1},
	{"right", Right, 2},
	{"round", Round, 1},
	{"sgn", Sgn, 1},
	{"sin", Sin, 1},
	{"size", Size, 1, 2},
	{"sqrt", Sqrt, 1},
	{"strcomp", StrComp, 2, 3},
	{"tan", Tan, 1},
	{"tolower", ToLower, 1},
	{"toupper", ToUpper, 1},
	{"transpose", Transpose, 1},
	{"truncate", Truncate, 1},
	{"ubound", UBound, 1, 2},
	{"val", Val, 1},
};

static const BuiltInFunctionInfo *registeredBuiltInFunctions;
static int registeredBuiltInFunctionsCount;

// Let another module register built-in functions. So if GSBasic is embedded in another
// app, this lets that app add built-in functions to GSBasic
// THE LIST OF BUILT-IN FUNCTIONS MUST BE IN ALPHABETICAL ORDER!!!!
void RegisterBuiltInFunctions(const BuiltInFunctionInfo *externBuiltInFunctions, int externBuiltInFunctionsCount)
{
    registeredBuiltInFunctions = externBuiltInFunctions;
    registeredBuiltInFunctionsCount = externBuiltInFunctionsCount;
}

static int BuiltInFunctionCmp(const void *key, const void *elem)
{
    const char *name = (const char *) key;
    const BuiltInFunctionInfo *info = (const BuiltInFunctionInfo *) elem;
    return strcasecmp(name, info->name);
}

const BuiltInFunctionInfo *FindBuiltInFunction(const char *name)
{
    void *found = bsearch(name, builtInFunctions, countof(builtInFunctions), sizeof(struct BuiltInFunctionInfo), BuiltInFunctionCmp);
    if (found == null && registeredBuiltInFunctions)
    {
        found = bsearch(name, registeredBuiltInFunctions, registeredBuiltInFunctionsCount, sizeof(struct BuiltInFunctionInfo), BuiltInFunctionCmp);
    }
    if (found)
    {
        const BuiltInFunctionInfo *info = (const BuiltInFunctionInfo *) found;
        return info;
    }
    
    return null;
}

void SetReturnValue(RunTimeEnvironment &runTimeEnvironment, const Value &value)
{
    runTimeEnvironment.SetReturnValue(value);
}
