#ifndef _ERROR_H_
#define _ERROR_H_

#include <stdint.h>
#include "LineNumber.h"

typedef const char *ErrorCode;

extern ErrorCode TokenTooLong;
extern ErrorCode InvalidRealNumber;
extern ErrorCode UnrecognizedToken;
extern ErrorCode OutOfMemory;
extern ErrorCode OutOfProgramMemory;
extern ErrorCode UnexpectedEndOfText;
extern ErrorCode ExpectedEOL;
extern ErrorCode UnrecognizedStatement;
extern ErrorCode InvalidExpression;
extern ErrorCode EmptyExpression;
extern ErrorCode UnmatchedParenBracketBrace;
extern ErrorCode IllegalVariableSpec; // e.g. a Dim statement has something unrecognizable in it
extern ErrorCode MissingEndBracket;
extern ErrorCode MissingEndParen;
extern ErrorCode MissingOpenBrace;
extern ErrorCode MissingEndBrace;
extern ErrorCode ExpectedEQ; // i.e. for an assignment
extern ErrorCode IllegalOperand;
extern ErrorCode ArrayIndexMustBeInt;
extern ErrorCode ArrayIndexMustBePositive;
extern ErrorCode ArrayIndexOutOfRange;
extern ErrorCode WrongNumberOfIndexes;
extern ErrorCode UninitializedVariable;
extern ErrorCode PreviouslyDefined;
extern ErrorCode NoMatchingIf;
extern ErrorCode NoMatchingFor;
extern ErrorCode NoMatchingWhile;
extern ErrorCode NoMatchingRepeat;
extern ErrorCode NoMatchingFunction;
extern ErrorCode NotInLoop;
extern ErrorCode IllegalFileName;
extern ErrorCode NotAnArray;
extern ErrorCode NotAStruct;
extern ErrorCode CantOpenFile;
extern ErrorCode ExpectedTo;
extern ErrorCode NextVariableMismatch;
extern ErrorCode MustBeNumeric;
extern ErrorCode MustBeInteger;
extern ErrorCode MustBeArrayInitializer;
extern ErrorCode LineTooLong;
extern ErrorCode PrecedingElse;
extern ErrorCode ExpectedLoopType;
extern ErrorCode NoProgramLoaded;
extern ErrorCode MustReturnValue;
extern ErrorCode MustNotReturnValue;
extern ErrorCode NoNestedProcs;
extern ErrorCode NoParameterList;
extern ErrorCode DivideByZero;
extern ErrorCode VariableAlreadyExists;
extern ErrorCode ProcedureAlreadyExists;
extern ErrorCode WrongNumberOfArguments;
extern ErrorCode TooManyDimensions;
extern ErrorCode MissingProcedureName;
extern ErrorCode UndefinedProcedure;
extern ErrorCode IllegalArgument;
extern ErrorCode ConsoleOnly;
extern ErrorCode DebugModeOnly;
extern ErrorCode NotInProgram;
extern ErrorCode BuiltInConstant;
extern ErrorCode AssignmentToStructIndex;
extern ErrorCode IllegalStructDef;
extern ErrorCode StructNotDefined;
extern ErrorCode NotAMember;
extern ErrorCode ByRefParameter;
extern ErrorCode ExpectedOnErrorCall;
extern ErrorCode ExpectedSubroutineOr0;
extern ErrorCode NotImplemented;
extern ErrorCode IllegalXOROperand;
extern ErrorCode ExpectedString;
extern ErrorCode TooManyIncludes;
extern ErrorCode OnlyOneOptionBase;

// Not really errors: just signals to the Executive
extern ErrorCode ReturnFromSubroutine;
extern ErrorCode Breakpoint;
extern ErrorCode ResumeRunning;
extern ErrorCode StopExecution;
extern ErrorCode AlreadyPrinted;

#define IsSignalMessage(errorCode) (*errorCode == '#')

#ifdef __cplusplus

class Error
{
public:
	Error() : _message(0) {}
	Error(const char *message, uint16_t lineNumber, uint16_t column) : 
		_message(message), _lineNumber(lineNumber), _column(column)
	{}

	operator bool() const {return _message != 0;}

	const char *Message() const {return _message;}
	uint16_t LineNumber() const {return ExtractLineNumber(_lineNumber);}
	uint16_t FileIndex() const { return ExtractFileIndex(_lineNumber); }
	uint16_t Column() const {return _column;}

private:
	const char *_message;
	uint16_t _lineNumber;
	uint16_t _column;
};

extern Error NoError;

#endif

#endif
