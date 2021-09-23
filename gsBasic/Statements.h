#ifndef _STATEMENTS_H_
#define _STATEMENTS_H_

#include <stdio.h>
#include "AST.h"
#include "Expression.h"
#include "ExpressionList.h"
#include "VarSpec.h"
#include "CallSpec.h"
#include "Environment.h"
#include "LineNumber.h"


class Statement : public AST
{
public:
	static Statement *CreateStatement(const Token &token, Error &error);

	void SetStatementNumber(size_t number) {_statementNumber = int(number);}
	int StatementNumber() const {return _statementNumber;}

	virtual bool IsProcedure() const { return false; }

	uint16_t LineNumber() const { return ExtractLineNumber(RawLineNumber()); }
	virtual uint16_t Column() const { return 0; }
	uint16_t FileIndex() const { return ExtractFileIndex(RawLineNumber()); }

protected:
	virtual uint16_t RawLineNumber() const = 0;
	Statement() : _statementNumber(-1) {}

private:
	int _statementNumber;
};

class StatementLoop : public Statement
{
public:
	virtual Error Continue(Executive &exec, RunTimeEnvironment &runTimeEnvironment) = 0;
	virtual Error Exit(Executive &exec, RunTimeEnvironment &runTimeEnvironment) = 0;

	uint16_t Column() const { return _column; }

	void SetLocation(uint16_t lineNumber, uint16_t column) { _lineNumber = lineNumber; _column = column; }

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

protected:
	uint16_t _lineNumber;
	uint16_t _column;
};

// The macro that CreateStatement uses needs StatementAssignment to be aliased to StatementLet
#define StatementLet StatementAssignment

class StatementAssignment : public Statement
{
public:
	StatementAssignment() : _lvalue(null), _rvalue(null) {}
	virtual ~StatementAssignment() {delete _lvalue; Expression::Delete(_rvalue);}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump() 
	{
		if (_lvalue) 
			_lvalue->Dump(); 
		puts(" <- "); 
		if (_rvalue && Expression::IsExpressionPointer(_rvalue))
			_rvalue->Dump();
		else
			puts(" constant");
	}
#endif

protected:
	uint16_t RawLineNumber() const { return _lvalue->LineNumber(); }

private:
	VarSpec *_lvalue;
	Expression *_rvalue;
};

class StatementBp : public Statement
{
public:
	StatementBp() {}

	typedef enum { SET, CLEAR, PRINT, CLEARALL } BreakpointCommand;

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &);

	uint16_t RawLineNumber() const { return _breakpointLine; }
	virtual uint16_t Column() const { return _breakpointColumn; }
	const String &SourceFile() const { return _breakpointFileName; }

#ifdef _DEBUG
	virtual void Dump() { puts("Bp"); }
#endif

private:
	BreakpointCommand _type;
	uint16_t _breakpointLine;
	uint16_t _breakpointColumn;
	String _breakpointFileName;
};

class StatementCall : public Statement
{
public:
	StatementCall() {}
	~StatementCall();

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	uint16_t RawLineNumber() const;

#ifdef _DEBUG
	virtual void Dump();
#endif

private:
	CallSpec _callSpec;
};

class StatementChain : public Statement
{
public:
	StatementChain() : _fileName(null), _rerun(false) {}
	~StatementChain() { Expression::Delete(_fileName); }

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump();
#endif

protected:
	uint16_t RawLineNumber() const { return _fileName->LineNumber(); }

private:
	Expression *_fileName;
	bool _rerun;
};

class StatementComment : public Statement
{
public:
	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &, RunTimeEnvironment &) {return NoError;}

#ifdef _DEBUG
	virtual void Dump() {puts("Comment");}
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }
};

class StatementLoopBreak : public Statement
{
public:
	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

	StatementLoop *_openingStatement;
	uint16_t _lineNumber;
};

class StatementContinue : public StatementLoopBreak
{
public:
	virtual Error Execute(Executive &exec, RunTimeEnvironment &);

#ifdef _DEBUG
	virtual void Dump() {puts("Continue");}
#endif
};

class StatementDim : public Statement
{
public:
	StatementDim() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump();
#endif

protected:
	uint16_t RawLineNumber() const { return _varSpecs[0]->LineNumber(); }

private:
	PtrOwnerArray<VarSpec> _varSpecs;
};

class StatementDir : public Statement
{
public:
	StatementDir() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump() {}
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }
};

class StatementIf;

#define StatementElse StatementElseif
class StatementEndif;

class StatementElseif : public Statement
{
public:
	StatementElseif() : _endif(null) { _exprOrLine.expression = null; }
	~StatementElseif() { if (!IsElse()) Expression::Delete(_exprOrLine.expression); }

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	Error EvaluateExpression(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	void SetEndif(StatementEndif *stmt) {_endif = stmt;}

	bool IsElse() const {return (long(_exprOrLine.expression) & 0xffff0000) == 0;}

protected:
	uint16_t RawLineNumber() const { return IsElse() ? _exprOrLine.lineNumber : Expression::SLineNumber(_exprOrLine.expression); }

#ifdef _DEBUG
	virtual void Dump();
#endif

private:
	union
	{
		Expression *expression;
		uint16_t lineNumber;
	} _exprOrLine;
	StatementEndif *_endif;
};

class StatementEndProcedure : public Statement
{
public:
	StatementEndProcedure(ControlStructure::Type_t type) : _type(type) {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);

	uint16_t Column() const { return _column; }

	void SetLocation(uint16_t lineNumber, uint16_t column) { _lineNumber = lineNumber; _column = column; }

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

protected:
	uint16_t _lineNumber;
	uint16_t _column;

private:
	ControlStructure::Type_t _type;
	StatementProcedure *_procedure;
};

class StatementFunction;

class StatementEndfunction : public StatementEndProcedure
{
public:
	StatementEndfunction() : StatementEndProcedure(ControlStructure::FUNCTION) {}

	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump() {puts("Endfunction");}
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }
};

class StatementEndif : public Statement
{
public:
	StatementEndif() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump();
#endif

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

private:
	uint16_t _lineNumber;
};

class StatementEndsub : public StatementEndProcedure
{
public:
	StatementEndsub() : StatementEndProcedure(ControlStructure::SUB) {}

	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump() { puts("Endsub"); }
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }
};

class StatementWhile;

class StatementEndwhile : public Statement
{
public:
	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump();
#endif

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

private:
	StatementWhile *_while;
	uint16_t _lineNumber;
};

class StatementExit : public StatementLoopBreak
{
public:
	virtual Error Execute(Executive &exec, RunTimeEnvironment &);

#ifdef _DEBUG
	virtual void Dump() {puts("Exit");}
#endif
};

class StatementNext;

class StatementFor : public StatementLoop
{
public:
	StatementFor() : _var(null), _from(null), _to(null), _step(null), _endValue(null), _stepValue(null) {}
	~StatementFor();

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	virtual Error Continue(Executive &exec, RunTimeEnvironment &runTimeEnvironment);
	virtual Error Exit(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	void SetNext(StatementNext *next) {_next = next;}

#ifdef _DEBUG
	virtual void Dump();
#endif

protected:
	uint16_t RawLineNumber() const { return _var->LineNumber(); }

private:
	friend class StatementNext;
	friend class StatementContinue;
	friend class StatementExit;
	bool LoopAgain(RunTimeEnvironment &runTimeEnvironment) const;
	bool StepAndLoopAgain(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	VarSpec *_var;
	Expression *_from, *_to, *_step;
	VarSpec *_endValue, *_stepValue;
	bool _stepValueIsNegative;
	bool _exitLoop;
	StatementNext *_next;
};

class StatementProcedure : public Statement
{
public:
	StatementProcedure() : _endProc(null), _type(ControlStructure::SUB), _byRefs(0), _variableScope(null) {}
	StatementProcedure(ControlStructure::Type_t type) : _endProc(null), _type(type), _byRefs(0), _variableScope(null) {}
	virtual ~StatementProcedure() { delete _variableScope; }

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	void SetEndproc(Statement *endProc);

	const PtrOwnerArray<VarSpec> &Parameters() const { return _parameters; }
	bool IsByRefArgument(int index) const { return (_byRefs & (1 << index)) != 0; }

	const String &Name() const {return _name;}

	void SetVariableScope(VariableScope *scope) { _variableScope = scope; }
	VariableScope *GetVariableScope() { return _variableScope; }
	int LocalVariableCount() const { return _variableScope->VariableCount(); }

	bool IsProcedure() const { return true; }

#ifdef _DEBUG
	virtual void Dump() { puts(_name.c_str()); }
#endif

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

private:
	String _name;
	Statement *_endProc;
	ControlStructure::Type_t _type;
	PtrOwnerArray<VarSpec> _parameters;
	// Bit flags indicating which parameters are passed ByRef
	uint32_t _byRefs;
	VariableScope *_variableScope;
	uint16_t _lineNumber;
};

class StatementFunction : public StatementProcedure
{
public:
	StatementFunction() : StatementProcedure(ControlStructure::FUNCTION) {}

#ifdef _DEBUG
	virtual void Dump();
#endif
};

class StatementIf : public Statement
{
public:
	StatementIf() : _expression(null), _endif(null) {}
	~StatementIf() { Expression::Delete(_expression); }

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	Error AddElse(StatementElseif *stmt);
	void SetEndif(StatementEndif *stmt);

	PtrArray<StatementElseif> &Elses() {return _elses;}

#ifdef _DEBUG
	virtual void Dump();
#endif

	uint16_t Column() const { return _column; }

	void SetLocation(uint16_t lineNumber, uint16_t column) { _lineNumber = lineNumber; _column = column; }

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

protected:
	uint16_t _lineNumber;
	uint16_t _column;

	Expression *_expression;
	PtrArray<StatementElseif> _elses;
	StatementEndif *_endif;
};

class StatementInclude : public Statement
{
public:
	StatementInclude() {}
	~StatementInclude() { }

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &, RunTimeEnvironment &) { return NoError; }

	const String &FileName() const { return _fileName; }

#ifdef _DEBUG
	virtual void Dump();
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }

private:
	String _fileName;
};

/*class StatementLabel : public Statement
{
public:
	StatementLabel() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment) {return NoError;}

#ifdef _DEBUG
	virtual void Dump();
#endif

private:
	String _label;
};*/

class StatementLoad : public Statement
{
public:
	StatementLoad() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	const String &FileName() const { return _fileName; }

#ifdef _DEBUG
	virtual void Dump() {fputs("Load ", stdout); puts(_fileName.c_str());}
#endif

	uint16_t Column() const { return _column; }

	void SetLocation(uint16_t lineNumber, uint16_t column) { _lineNumber = lineNumber; _column = column; }

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

protected:
	uint16_t _lineNumber;
	uint16_t _column;

private:
	String _fileName;
};

class StatementMem : public Statement
{
public:
	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump();
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }
};

class StatementNew : public Statement
{
public:
	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump() {}
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }
};

class StatementNext : public Statement
{
public:
	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump();
#endif

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

private:
	StatementFor *_for;
	uint16_t _lineNumber;
};

// Statement ON ERROR
class StatementOn : public Statement
{
public:
	StatementOn() : _callSpec(null) {}
	virtual ~StatementOn();

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	Error ExecuteErrorHandler (Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	uint16_t Column() const { return _column; }

#ifdef _DEBUG
	virtual void Dump();
#endif

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

private:
	CallSpec *_callSpec;
	uint16_t _lineNumber;
	uint16_t _column;
};

// Statement OOPTION BASE {0|1}
class StatementOption : public Statement
{
public:
	StatementOption() { ++_optionBaseCount; }
	virtual ~StatementOption() { --_optionBaseCount; }

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump();
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }

	static int _optionBaseCount;
};

class StatementPrint : public Statement
{
public:
	StatementPrint() {}
	virtual ~StatementPrint() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump() {puts("Print"); _expressionList.Dump();}
#endif

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

private:
	ExpressionList _expressionList;
	uint16_t _lineNumber;
};

class StatementReload : public Statement
{
public:
	StatementReload() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump() {puts("Reload");}
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }
};

class StatementUntil;

class StatementRepeat : public StatementLoop
{
public:
	StatementRepeat() : _until(null) {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	void SetUntil(StatementUntil *until) {_until = until;}

	virtual Error Continue(Executive &exec, RunTimeEnvironment &runTimeEnvironment);
	virtual Error Exit(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump();
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }

private:
	StatementUntil *_until;
};

class StatementResume : public Statement
{
public:
	StatementResume() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump() { puts("Resume"); }
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }
};

class StatementReturn : public Statement
{
public:
	StatementReturn() : _expression(null) {}
	virtual ~StatementReturn() {Expression::Delete(_expression);}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump() {puts("Return"); _expression->Dump();}
#endif

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

private:
	Expression *_expression;
	uint16_t _lineNumber;
};

class StatementRun : public Statement
{
public:
	StatementRun() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump() {puts("Run");}
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }
};

/**
* Single step in the debugger
*/
class StatementStep : public Statement
{
public:
	StatementStep() : _count(1) {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &);

#ifdef _DEBUG
	virtual void Dump() { puts("Step"); }
#endif

	uint16_t Column() const { return _column; }

	void SetLocation(uint16_t lineNumber, uint16_t column) { _lineNumber = lineNumber; _column = column; }

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

protected:
	uint16_t _lineNumber;
	uint16_t _column;

private:
	int _count;
};

/**
* Define constants to simulate structures
*/
class StatementStruct : public Statement
{
public:
	StatementStruct() : _lineNumber(0), _column(0) {}
	~StatementStruct() { }

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &);

#ifdef _DEBUG
	virtual void Dump();
#endif

	uint16_t Column() const { return _column; }

	void SetLocation(uint16_t lineNumber, uint16_t column) { _lineNumber = lineNumber; _column = column; }

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

protected:
	uint16_t _lineNumber;
	uint16_t _column;
};

/**
* Exit the program
*/
class StatementStop : public Statement
{
public:
	StatementStop() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &);

#ifdef _DEBUG
	virtual void Dump() { puts("Stop"); }
#endif

	uint16_t Column() const { return _column; }

	void SetLocation(uint16_t lineNumber, uint16_t column) { _lineNumber = lineNumber; _column = column; }

protected:
	uint16_t RawLineNumber() const { return _lineNumber; }

protected:
	uint16_t _lineNumber;
	uint16_t _column;
};

class StatementSub : public StatementProcedure
{
public:
	StatementSub() : StatementProcedure(ControlStructure::SUB) {}

#ifdef _DEBUG
	virtual void Dump();
#endif
};

class StatementTroff : public Statement
{
public:
	StatementTroff() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &);

#ifdef _DEBUG
	virtual void Dump() { puts("Troff"); }
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }
};

class StatementTron : public Statement
{
public:
	StatementTron() {}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &);

#ifdef _DEBUG
	virtual void Dump() { puts("Tron"); }
#endif

protected:
	uint16_t RawLineNumber() const { return 0; }
};

class StatementUntil : public Statement
{
public:
	StatementUntil() : _expression(null), _repeat(null) {}
	~StatementUntil() {Expression::Delete(_expression);}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump();
#endif

protected:
	uint16_t RawLineNumber() const { return Expression::SLineNumber(_expression); }

private:
	Expression *_expression;
	StatementRepeat *_repeat;
};

class StatementWhile : public StatementLoop
{
public:
	StatementWhile() : _expression(null), _endWhile(null) {}
	~StatementWhile() {Expression::Delete(_expression);}

	virtual Error Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment);
	virtual Error Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

	void SetEndwhile(StatementEndwhile *endWhile) {_endWhile = endWhile;}

	virtual Error Continue(Executive &exec, RunTimeEnvironment &runTimeEnvironment);
	virtual Error Exit(Executive &exec, RunTimeEnvironment &runTimeEnvironment);

#ifdef _DEBUG
	virtual void Dump();
#endif

private:
	Expression *_expression;
	StatementEndwhile *_endWhile;
};

#endif
