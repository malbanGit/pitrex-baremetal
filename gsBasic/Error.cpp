#include "Error.h"
#include "GSBCommon.h"

ErrorCode TokenTooLong = "E100: Tokens must be less than 32 characters long";
ErrorCode InvalidRealNumber = "E101: Invalid real number";
ErrorCode UnrecognizedToken = "E102: Unrecognized token";
ErrorCode OutOfMemory = "E103: Out of memory";
ErrorCode OutOfProgramMemory = "E104: Out of program memory; program memory will be cleared";
ErrorCode UnexpectedEndOfText = "E105: Unexpected end of text";
ErrorCode ExpectedEOL = "E106: Expected end of line";
ErrorCode UnrecognizedStatement = "E107: Unrecognized statement";
ErrorCode InvalidExpression = "E108: Invalid expression";
ErrorCode EmptyExpression = "E109: Empty expression";
ErrorCode IllegalVariableSpec = "E110: Illegal variable specification"; // e.g. a Dim statement has something unrecognizable in it
ErrorCode MissingEndBracket = "E111: Missing end bracket";
ErrorCode MissingEndParen = "E112: Missing end parentheses";
ErrorCode MissingOpenBrace = "E113: Missing open brace";
ErrorCode MissingEndBrace = "E114: Missing end brace";
ErrorCode ExpectedEQ = "E115: Expected ="; // i.e. for an assignment
ErrorCode IllegalOperand = "E116: Illegal operand";
ErrorCode ArrayIndexMustBeInt = "E117: Array index must be an integer";
ErrorCode ArrayIndexMustBePositive = "E118: Array index must be greater than 0";
ErrorCode ArrayIndexOutOfRange = "E119: Array index out of range";
ErrorCode WrongNumberOfIndexes = "E120: Wrong number of indices inside []";
ErrorCode UninitializedVariable = "E121: Variable is used before being assigned a value";
ErrorCode PreviouslyDefined = "E122: Variable already exists";
ErrorCode NoMatchingIf = "E123: Can't find the matching IF statement";
ErrorCode NoMatchingFor = "E124: Can't find the matching FOR statement";
ErrorCode NoMatchingWhile = "E125: Can't find the matching WHILE statement";
ErrorCode NoMatchingRepeat = "E126: Can't find the matching REPEAT statement";
ErrorCode NoMatchingFunction = "E127: Can't find the matching FUNCTION statement";
ErrorCode NotInLoop = "E128: Not inside a loop";
ErrorCode IllegalFileName = "E129: Illegal file name";
ErrorCode NotAnArray = "E130: Variable is not an array";
ErrorCode NotAStruct = "E131: Variable is not a struct";
ErrorCode CantOpenFile = "E132: Can't open file";
ErrorCode ExpectedTo = "E133: Expected 'TO'";
ErrorCode NextVariableMismatch = "E134: Variable name does not match the variable in the FOR statement";
ErrorCode MustBeNumeric = "E135: Value must be numeric";
ErrorCode MustBeInteger = "E136: Value must be an integer";
ErrorCode MustBeArrayInitializer = "E137: Value must be an array initializer";
ErrorCode LineTooLong = "E138: Line is too long";
ErrorCode PrecedingElse = "E139: There is an ELSE statement before this";
ErrorCode ExpectedLoopType = "E140: Expected a loop type (FOR, WHILE, REPEAT)";
ErrorCode NoProgramLoaded = "E141: No program loaded";
ErrorCode MustReturnValue = "E142: Function must return a value";
ErrorCode MustNotReturnValue = "E143: Subroutines may not return a value";
ErrorCode NoNestedProcs = "E144: Functions and subroutines can not be nested";
ErrorCode NoParameterList = "E145: Functions must have parameter lists";
ErrorCode DivideByZero = "E146: Divide by zero error";
ErrorCode VariableAlreadyExists = "E147: Variable already exists";
ErrorCode ProcedureAlreadyExists = "E148: Subroutine, function, built-in function, or struct already exists";
ErrorCode WrongNumberOfArguments = "E149: Wrong number of arguments";
#define DimensionsMessage(max) ErrorCode TooManyDimensions = "E150: Arrays may not have more than " max " dimensions";
DimensionsMessage(STRINGIZE(MAX_ARRAY_DIMENSIONS))
ErrorCode MissingProcedureName = "E151: Subroutine or function is missing its name";
ErrorCode UndefinedProcedure = "E152: Subroutine or function is not defined";
ErrorCode ConsoleOnly = "E153: This command can only be executed when the program is not Running";
ErrorCode DebugModeOnly = "E154: This command can only be given in Debug mode";
ErrorCode NotInProgram = "E155: Only allowed in Immediate or Debug modes";
ErrorCode BuiltInConstant = "E156: Illegal use of a built-in constant";
ErrorCode IllegalArgument = "E157: Illegal argument";
ErrorCode AssignmentToStructIndex = "E158: Cannot assign value to struct member index";
ErrorCode IllegalStructDef = "E159: Illegal structure definition";
ErrorCode StructNotDefined = "E160: Structure not yet defined";
ErrorCode NotAMember = "E161: Struct has no member with this name";
ErrorCode ByRefParameter = "E162: ByRef argument requires a variable to be passed";
ErrorCode ExpectedOnErrorCall = "E163: Expected 'ON ERROR CALL'";
ErrorCode ExpectedSubroutineOr0 = "E164: Expected subroutine name or 0";
ErrorCode NotImplemented = "E165: Not implemented";
ErrorCode IllegalXOROperand = "E166: Illegal operand. '^' is now bitwise XOR, not exponent; use pow()";
ErrorCode ExpectedString = "E167: Expected string";
ErrorCode TooManyIncludes = "E168: Too many Include files";
ErrorCode OnlyOneOptionBase = "E169: OPTION BASE may appear only once in your program.";

ErrorCode ReturnFromSubroutine = "#A signal to return from a subroutine";
ErrorCode Breakpoint = "#A signal to break execution";
ErrorCode ResumeRunning = "#A signal to resume";
ErrorCode StopExecution = "#A signal to Stop Execution";
ErrorCode AlreadyPrinted = "#A signal that Message was previously displayed";

Error NoError;
