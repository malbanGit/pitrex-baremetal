
#include "GSBCommon.h"
#include "Statements.h"
#include "Reader.h"
#include "Environment.h"
#include "Lexer.h"
#include "Parser.h"
#include "Executive.h"
#include "CallSpec.h"
#include "Utilities.h"
#include "Heap.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif

#include <vectrex/vectrexInterface.h>
#include "vio.h"
typedef DIR Dir_t;

#ifdef __cplusplus
}
#endif


Statement *Statement::CreateStatement(const Token &token, Error &error)
{
	/**
	 * FAC ("factory") is a convenient macro that turns its argument into a test for a keyword, followed by
	 * returning a pointer to a class that can compile and execute that keyword
	 */
#define FAC(keyword) \
	if (strcasecmp(token.ValueIdentifier(), #keyword) == 0) \
	{ \
		Statement *stmt = new Statement##keyword(); \
		if (stmt == null) \
			error = Error(OutOfProgramMemory, token.LineNumber(), token.Column()); \
		return stmt; \
	}

	error = NoError;

	if (token.Type() == Token::Comment)
		return new StatementComment();

	if (token.Type() == Token::Identifier)
	{
		FAC(Bp) // Breakpoint
		FAC(Call)
		FAC(Chain)
		FAC(Continue)
		FAC(Dim)
		FAC(Dir)
		FAC(Else)
		FAC(Elseif)
		FAC(Endfunction)
		FAC(Endif)
		FAC(Endsub)
		FAC(Endwhile)
		FAC(Exit)
		FAC(For)
		FAC(Function)
		FAC(If)
		FAC(Include)
		FAC(Let)
		FAC(Load)
		FAC(Mem)
		FAC(New)
		FAC(Next)
		FAC(On)
		FAC(Option)
		FAC(Print)
		FAC(Reload)
		FAC(Repeat)
		FAC(Resume)
		FAC(Return)
		FAC(Run)
		FAC(Step)
		FAC(Stop)
		FAC(Struct)
		FAC(Sub)
		FAC(Troff) // Trace Off
		FAC(Tron)  // Trace On
		FAC(Until)
		FAC(While)
	}

	error = Error(UnrecognizedStatement, token.LineNumber(), token.Column());

	return null;
}

Error StatementAssignment::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	Token token;
	tokenReader->Next(token);

	// If the first token isn't "Let"
	if (strcasecmp(token.ValueIdentifier(), "Let") != 0)
	{
		// Don't eat the first token
		tokenReader->Prev();
	}

	Error error = VarSpec::Build(tokenReader, compileEnvironment, token.LineNumber(), token.Column(), VarSpec::LValue, &_lvalue);
	if (error)
		return error;
	else
	{
		if (!tokenReader->Next(token))
		{
			return Error(ExpectedEQ, _lvalue->LineNumber(), _lvalue->Column());
		}
		else if (token.Type() != Token::EQ)
			return Error(ExpectedEQ, token.LineNumber(), token.Column());
		else
		{
			error = Expression::Build(tokenReader, compileEnvironment, _rvalue, token.LineNumber(), token.Column());
			if (error)
			{
				return error;
			}
		}
	}

	return NoError;
}

Error StatementAssignment::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	size_t stackSize = runTimeEnvironment.calculationStack.size();
	Error error = Expression::Execute(_rvalue, exec, runTimeEnvironment);
	if (error)
	{
		if (error.LineNumber() == ExtractLineNumber(0xffff) && error.Column() == 0xffff)
			error = Error(error.Message(), LineNumber(), Column());
		return error;
	}
	if (runTimeEnvironment.calculationStack.size() != stackSize + 1)
		return Error(MustReturnValue, _rvalue->LineNumber(), _rvalue->Column());

	Value *pvalue;
	error = _lvalue->Reference(exec, runTimeEnvironment, &pvalue);
	if (error)
	{
		runTimeEnvironment.calculationStack.pop_back();
		return error;
	}
	if (pvalue->IsByRef())
		pvalue = pvalue->Dereference();

	if (pvalue->IsByteReference())
	{
		if (runTimeEnvironment.calculationStack.back().IsInteger())
		{
			pvalue->WriteToByte((uint8_t) runTimeEnvironment.calculationStack.back().Integer());
		}
		else
			return Error(MustBeInteger, _rvalue->LineNumber(), _rvalue->Column());
	}
	else
		*pvalue = runTimeEnvironment.calculationStack.back();
	runTimeEnvironment.calculationStack.pop_back();

	return NoError;
}

// Process Breakpoint commands
Error StatementBp::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	Error error;

	// Eat the 'BP'
	Token token, intToken;
	tokenReader->Next(token);
	_breakpointLine = token.LineNumber();
	_breakpointColumn = token.Column();

	if (tokenReader->Next(token))
	{
		const char *identifier;

		switch (token.Type())
		{
		case Token::Int :
			_breakpointLine = (uint16_t) token.ValueInt();
			_type = SET;
			break;

		case Token::String :
			_breakpointFileName = token.ValueString();
			if (tokenReader->Next(intToken))
			{
				if (intToken.Type() == Token::Int)
				{
					_breakpointLine = (uint16_t) intToken.ValueInt();
					_type = SET;
				}
				else
					error = Error(IllegalArgument, intToken.LineNumber(), intToken.Column());
			}
			else
				error = Error(ExpectedEOL, token.LineNumber(), uint16_t(token.Column() + _breakpointFileName.length() + 2));
			break;

		case Token::Identifier :
			identifier = token.ValueIdentifier();
			if (strcasecmp(identifier, "c") == 0 || strcasecmp(identifier, "clear") == 0)
			{
				if (tokenReader->Next(token))
				{
					if (token.Type() == Token::String)
					{
						_breakpointFileName = token.ValueString();
						if (tokenReader->Next(intToken))
						{
							if (intToken.Type() == Token::Int)
							{
								_breakpointLine = (uint16_t) intToken.ValueInt();
								_type = CLEAR;
							}
							else
								error = Error(IllegalArgument, intToken.LineNumber(), intToken.Column());
						}
						else
							error = Error(ExpectedEOL, token.LineNumber(), token.Column());
					}
					else if (token.Type() == Token::Int)
					{
						_breakpointLine = (uint16_t) token.ValueInt();
						_type = CLEAR;
					}
					else
						error = Error(IllegalArgument, token.LineNumber(), token.Column());
				}
				// Else (no argument for the Clear) we clear all breakpoints
				else
					_type = CLEARALL;
			}
			break;

		default :
			error = Error(IllegalArgument, token.LineNumber(), token.Column());
			break;
		}
	}
	// Else (no arguments) print the list of breakpoints
	else
		_type = PRINT;

	return error;
}

Error StatementBp::Execute(Executive &exec, RunTimeEnvironment &)
{
	return exec.BreakpointCommand(_type, _breakpointLine, (uint16_t) exec.SourceFileIndex(_breakpointFileName));
}

StatementCall::~StatementCall()
{
}

Error StatementCall::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'Call'
	Token token;
	tokenReader->Next(token);

	// Compile the CallSpec
	Error error = _callSpec.Build(tokenReader, compileEnvironment);
	return error;
}

Error StatementCall::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	size_t oldStackSize = runTimeEnvironment.calculationStack.size();
	Error error = _callSpec.Execute(exec, runTimeEnvironment);
	// If the stack size changed (i.e. we called a function that had a return value)
	// clean it up
	if (runTimeEnvironment.calculationStack.size() == oldStackSize + 1)
	{
		runTimeEnvironment.calculationStack.pop_back();
	}
	return error;
}

uint16_t StatementCall::RawLineNumber() const 
{
	return _callSpec.LineNumber(); 
}

#ifdef _DEBUG
void StatementCall::Dump()
{
	puts("Call");
	_callSpec.Dump();
}
#endif

Error StatementChain::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	Token token;

	// Eat the 'Chain'
	tokenReader->Next(token);

	Error error = Expression::Build(tokenReader, compileEnvironment, _fileName, token.LineNumber(), token.Column());
	if (error)
		return error;

	// Check for a "rerun" token
	if (tokenReader->Next(token))
	{
		if (token.Type() == Token::Identifier && strcasecmp(token.ValueIdentifier(), "rerun") == 0)
			_rerun = true;
		else
			tokenReader->Prev();
	}

	return NoError;
}

Error StatementChain::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	// Evaluate our file name
	Error error = Expression::Execute(_fileName, exec, runTimeEnvironment);
	if (!error)
	{
		Value fileName = runTimeEnvironment.calculationStack.back();
		runTimeEnvironment.calculationStack.pop_back();
		if (!fileName.IsString())
			error = Error(IllegalArgument, _fileName->LineNumber(), _fileName->Column());
		else
		{
			error = exec.Chain(fileName.ToString(), _rerun);
			if (error)
				error = Error(error.Message(), _fileName->LineNumber(), _fileName->Column());
		}
	}

	return error;
}

#ifdef _DEBUG
void StatementChain::Dump()
{
	puts("Chain");
	_fileName->Dump();
}
#endif

Error StatementComment::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	Token token;
	if (tokenReader->Next(token))
	{
		tokenReader->Prev();
	}

	return NoError;
}

Error StatementLoopBreak::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'Continue'
	Token token;
	tokenReader->Next(token);
	_lineNumber = token.LineNumber();
	uint16_t column = token.Column();

	// Get the token that says what sort of loop we're continuing
	if (!tokenReader->Next(token))
		return Error(ExpectedLoopType, _lineNumber, column + 8);
	ControlStructure::Type_t loopType;
	if (strcasecmp(token.ValueIdentifier(), "For") == 0)
	{
		loopType = ControlStructure::FOR;
	}
	else if (strcasecmp(token.ValueIdentifier(), "While") == 0)
	{
		loopType = ControlStructure::WHILE;
	}
	else if (strcasecmp(token.ValueIdentifier(), "Repeat") == 0)
	{
		loopType = ControlStructure::REPEAT;
	}
	else
		return Error(ExpectedLoopType, LineNumber(), Column() + 8);

	// Make sure we're in a FOR, WHILE, or REPEAT
	int loopStructure = int(compileEnvironment.ControlStructureStack().size()) - 1;
	while (loopStructure >= 0 && compileEnvironment.ControlStructureStack()[loopStructure]->Type() != loopType)
		--loopStructure;
	if (loopStructure < 0)
		return Error(NotInLoop, _lineNumber, column);

	_openingStatement = (StatementLoop *) compileEnvironment.ControlStructureStack()[loopStructure]->OpeningStatement();

	return NoError;
}

Error StatementContinue::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	_openingStatement->Continue(exec, runTimeEnvironment);
	return NoError;
}

Error StatementDim::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	bool done = false;
	Token token;

	// Eat the 'Dim'
	tokenReader->Next(token);
	Token prevToken = token;

	while (!done)
	{
		// Make sure there's a token
		if (tokenReader->Next(token))
			tokenReader->Prev();
		else
			return Error(UnexpectedEndOfText, prevToken.LineNumber(), prevToken.Column());

		VarSpec *varSpec;
		Error error = VarSpec::Build(tokenReader, compileEnvironment, token.LineNumber(), token.Column(), VarSpec::DimReference, &varSpec);
		if (error)
		{
			delete varSpec;
			return error;
		}
		else
		{
			_varSpecs.push_back((VarSpecWithElements *) varSpec);
			if (!tokenReader->Next(token) || token.Type() != Token::Comma)
				done = true;
		}
		_varSpecs.shrink_to_fit();
		prevToken = token;
	}
	return NoError;
}

Error StatementDim::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	for (size_t i = 0; i < _varSpecs.size(); ++i)
	{
		Error error = _varSpecs[i]->Dim(exec, runTimeEnvironment);
		if (error)
			return error;
	}
	return NoError;
}

#ifdef _DEBUG
void StatementDim::Dump()
{
	puts("Dim");
	for (size_t i = 0; i < _varSpecs.size(); ++i)
		_varSpecs[i]->Dump();
}
#endif

Error StatementDir::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	// Eat the 'Dir'
	Token token;
	tokenReader->Next(token);

	return NoError;
}

Error StatementDir::Execute(Executive &, RunTimeEnvironment &)
{
#ifdef __XC32
	Dir_t *dir = opendir(CurrentDirectoryName());
#else
	Dir_t *dir = opendir((char *) ".");
#endif
	if (dir)
	{
		/*struct */dirent *de;
		while ((de = readdir(dir)) != null)
		{
			// Print the file or directory name as long as it's not . or ..
			if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
			{
				FPUTS(de->d_name, stdout);
				if (de->d_type == DT_DIR)
					FPUTS("/\r\n", stdout);
				else
					FPUTS("\r\n", stdout);
			}
		}

		closedir(dir);
	}
	return NoError;
}

Error StatementElseif::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'else' or 'elseif'
	Token token;
	tokenReader->Next(token);

	if (strcasecmp(token.ValueIdentifier(), "elseif") == 0)
	{
		Error error = Expression::Build(tokenReader, compileEnvironment, _exprOrLine.expression, token.LineNumber(), token.Column());
		if (error)
			return error;

		// Eat the optional THEN
		if (tokenReader->Next(token) && (token.Type() != Token::Identifier || strcasecmp(token.ValueIdentifier(), "then") != 0))
			tokenReader->Prev();
	}
	else
		_exprOrLine.lineNumber = token.LineNumber();

	// Make sure we're in an IF
	if (compileEnvironment.ControlStructureStack().size() == 0 || compileEnvironment.ControlStructureStack().back()->Type() != ControlStructure::IF)
	{
		return Error(NoMatchingIf, LineNumber(), Column());
	}
	StatementIf *ifStmt = (StatementIf *) compileEnvironment.ControlStructureStack().back()->OpeningStatement();

	// Tell the IF statement about us
	Error error = ifStmt->AddElse(this);

	return error;
}

/**
 * Elseif gets executed when the previous block of code falls through to it. As such,
 * we sholud just jump to the Endif. (So how does the Elseif code block get executed?
 * The If statement evalutes our expression and jumps to the statment after us.)
 */
Error StatementElseif::Execute(Executive &exec, RunTimeEnvironment &)
{
	exec.Goto(_endif);
	return NoError;
}

Error StatementElseif::EvaluateExpression(Executive &exec, RunTimeEnvironment &runTimeEnvironment) 
{
	if (IsElse())
	{
		runTimeEnvironment.calculationStack.push_back(-1);
		return NoError;
	}
	else
		return Expression::Execute(_exprOrLine.expression, exec, runTimeEnvironment);
}

#ifdef _DEBUG
void StatementElseif::Dump()
{
	if (IsElse())
		puts("Else");
	else
	{
		puts("Elseif");
		_exprOrLine.expression->Dump();
		puts("Then");
	}
}
#endif

Error StatementEndProcedure::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'Endfunction' or 'Endsub'
	Token token;
	tokenReader->Next(token);
	SetLocation(token.LineNumber(), token.Column());

	// Make sure we're in a Function
	if (compileEnvironment.ControlStructureStack().size() == 0 || compileEnvironment.ControlStructureStack().back()->Type() != _type)
	{
		return Error(NoMatchingFunction, LineNumber(), Column());
	}
	StatementProcedure *procStmt = (StatementProcedure *) compileEnvironment.ControlStructureStack().back()->OpeningStatement();

	// Tell the Function/Sub statement about us
	procStmt->SetEndproc(this);

	// Exit the Function control structure
	compileEnvironment.ControlStructureStack().pop_back();
	procStmt->SetVariableScope(compileEnvironment.ExitVariableScope());

	return NoError;
}

Error StatementEndfunction::Execute(Executive &, RunTimeEnvironment &)
{
	return Error(MustReturnValue, LineNumber(), Column());
}

Error StatementEndif::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'endif'
	Token token;
	tokenReader->Next(token);

	_lineNumber = token.LineNumber();

	// Make sure we're in an IF
	if (compileEnvironment.ControlStructureStack().size() == 0 || compileEnvironment.ControlStructureStack().back()->Type() != ControlStructure::IF)
	{
		return Error(NoMatchingIf, token.LineNumber(), token.Column());
	}
	StatementIf *ifStmt = (StatementIf *) compileEnvironment.ControlStructureStack().back()->OpeningStatement();

	// Tell the IF statement about us
	ifStmt->SetEndif(this);

	// Exit the IF control structure
	delete compileEnvironment.ControlStructureStack().back();
	compileEnvironment.ControlStructureStack().back() = null;
	compileEnvironment.ControlStructureStack().pop_back();

	return NoError;
}

Error StatementEndif::Execute(Executive &, RunTimeEnvironment &)
{
	return NoError;
}

#ifdef _DEBUG
void StatementEndif::Dump()
{
	puts("Endif");
}
#endif

Error StatementEndsub::Execute(Executive &exec, RunTimeEnvironment &)
{
	return exec.ReturnFromSub();
}

Error StatementEndwhile::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'endwhile'
	Token token;
	tokenReader->Next(token);

	_lineNumber = token.LineNumber();

	// Make sure we're in an WHILE
	if (compileEnvironment.ControlStructureStack().size() == 0 || compileEnvironment.ControlStructureStack().back()->Type() != ControlStructure::WHILE)
	{
		return Error(NoMatchingWhile, LineNumber(), Column());
	}
	_while = (StatementWhile *) compileEnvironment.ControlStructureStack().back()->OpeningStatement();

	// Tell the WHILE statement about us
	_while->SetEndwhile(this);

	// Exit the WHILE control structure
	delete compileEnvironment.ControlStructureStack().back();
	compileEnvironment.ControlStructureStack().back() = null;
	compileEnvironment.ControlStructureStack().pop_back();

	return NoError;
}

Error StatementEndwhile::Execute(Executive &exec, RunTimeEnvironment &)
{
	exec.Goto(_while);
	return NoError;
}

#ifdef _DEBUG
void StatementEndwhile::Dump()
{
	puts("Endwhile");
}
#endif

Error StatementExit::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	_openingStatement->Exit(exec, runTimeEnvironment);
	return NoError;
}

StatementFor::~StatementFor()
{
	delete _var;
	Expression::Delete(_from);
	Expression::Delete(_to);
	Expression::Delete(_step);
	delete _endValue;
	delete _stepValue;
}

Error StatementFor::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'for'
	Token token;
	tokenReader->Next(token);
	SetLocation(token.LineNumber(), token.Column());

	Error error = VarSpec::Build(tokenReader, compileEnvironment, token.LineNumber(), token.Column(), VarSpec::LValue, &_var);
	if (error)
		return error;

	// Get the '='
	if (!tokenReader->Next(token))
		return Error(ExpectedEQ, _var->LineNumber(), _var->Column());
	if (token.Type() != Token::EQ)
		return Error(ExpectedEQ, token.LineNumber(), token.Column());

	error = Expression::Build(tokenReader, compileEnvironment, _from, token.LineNumber(), token.Column());
	if (error)
		return error;

	// Get the 'To' expression
	if (!tokenReader->Next(token))
		return Error(ExpectedTo, _from->LineNumber(), _from->Column());
	if (strcasecmp(token.ValueIdentifier(), "To"))
		return Error(ExpectedTo, token.LineNumber(), token.Column());

	error = Expression::Build(tokenReader, compileEnvironment, _to, token.LineNumber(), token.Column());
	if (error)
		return error;

	// Check for a STEP
	if (tokenReader->Next(token))
	{
		if (token.Type() == Token::Identifier && strcasecmp(token.ValueIdentifier(), "step") == 0)
		{
			error = Expression::Build(tokenReader, compileEnvironment, _step, token.LineNumber(), token.Column());
			if (error)
				return error;
		}
		else
			tokenReader->Prev();
	}

	// Allocate a variable that we can use for the calculated end value. To get a unique name, convert the 
	// 'this' pointer to a string
	VarSpec::Build("end" + String::itoa(int(this)), compileEnvironment, VarSpec::DimReference, &_endValue);

	// Allocate a variable that we can use for the calculated step value. To get a unique name, convert the 
	// 'this' pointer to a string
	VarSpec::Build("step" + String::itoa(int(this)), compileEnvironment, VarSpec::DimReference, &_stepValue);

	compileEnvironment.ControlStructureStack().push_back(new ControlStructure(ControlStructure::FOR, this));

	return NoError;
}

Error StatementFor::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	// Assign the variable from the start value
	Error error = Expression::Execute(_from, exec, runTimeEnvironment);
	if (error)
	{
		runTimeEnvironment.calculationStack.pop_back();
		return error;
	}
	Value &startValue = runTimeEnvironment.calculationStack.back();
	_var->SetValue(startValue, runTimeEnvironment);
	bool isNumeric = startValue.IsNumeric();
	runTimeEnvironment.calculationStack.pop_back();

	if (!isNumeric)
	{
		return Error(MustBeNumeric, _from->LineNumber(), _from->Column());
	}

	error = Expression::Execute(_to, exec, runTimeEnvironment);
	if (error)
		return error;
	Value &to = runTimeEnvironment.calculationStack.back();
	_endValue->SetValue(to, runTimeEnvironment);
	isNumeric = to.IsNumeric();
	runTimeEnvironment.calculationStack.pop_back();
	if (!isNumeric)
	{
		return Error(MustBeNumeric, _to->LineNumber(), _to->Column());
	}

	if (_step)
	{
		error = Expression::Execute(_step, exec, runTimeEnvironment);
		if (error)
			return error;
		Value &step = runTimeEnvironment.calculationStack.back();
		_stepValue->SetValue(step, runTimeEnvironment);
		if (step.IsInteger())
		{
			_stepValueIsNegative = (step.Integer() < 0);
			isNumeric = true;
		}
		else if (step.IsFloat())
		{
			_stepValueIsNegative = (step.Float() < 0);
			isNumeric = true;
		}
		else
			isNumeric = false;
		runTimeEnvironment.calculationStack.pop_back();
		if (!isNumeric)
		{
			return Error(MustBeNumeric, _step->LineNumber(), _step->Column());
		}
	}
	else
	{
		static Value one(1);
		_stepValue->SetValue(one, runTimeEnvironment);
		_stepValueIsNegative = false;
	}

	if (!LoopAgain(runTimeEnvironment))
		Exit(exec, runTimeEnvironment);

	return error;
}

Error StatementFor::Continue(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	if (StepAndLoopAgain(exec, runTimeEnvironment))
		exec.GotoFollowingStatement(this);
	else
		exec.GotoFollowingStatement(_next);
	return NoError;
}

Error StatementFor::Exit(Executive &exec, RunTimeEnvironment &)
{
	exec.GotoFollowingStatement(_next);
	return NoError;
}

bool StatementFor::LoopAgain(RunTimeEnvironment &runTimeEnvironment) const
{
	const Value &loopValue = _var->GetValue(runTimeEnvironment);
	if (loopValue.IsInteger())
	{
		int loopInt = loopValue.Integer();
		const Value &endValue = _endValue->GetValue(runTimeEnvironment);
		if (endValue.IsInteger())
		{
			int endInt = endValue.Integer();
			if (_stepValueIsNegative)
				return loopInt >= endInt;
			else
				return loopInt <= endInt;
		}
		else if (endValue.IsFloat())
		{
			float endFloat = endValue.Float();
			if (_stepValueIsNegative)
				return loopInt >= endFloat;
			else
				return loopInt <= endFloat;
		}
		else
			return false;
	}

	else if (loopValue.IsFloat())
	{
		float loopFloat = loopValue.Float();
		const Value &endValue = _endValue->GetValue(runTimeEnvironment);
		float endFloat;
		endValue.TypeBehaviors()->ToFloat(endValue, &endFloat);

		if (_stepValueIsNegative)
			return loopFloat >= endFloat;
		else
			return loopFloat <= endFloat;
	}

	else
		return false;
}

bool StatementFor::StepAndLoopAgain(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	// Increment the loop counter
	Value *loopValue;
	_var->Reference(exec, runTimeEnvironment, &loopValue);
	if (loopValue->IsInteger())
	{
		const Value &stepValue = _stepValue->GetValue(runTimeEnvironment);
		if (stepValue.IsInteger())
		{
			*loopValue = loopValue->Integer() + stepValue.Integer();
		}
		else
		{
			*loopValue = loopValue->Integer() + stepValue.Float();
		}
	}
	else if (loopValue->IsFloat())
	{
		const Value &stepValue = _stepValue->GetValue(runTimeEnvironment);
		float step;
		stepValue.TypeBehaviors()->ToFloat(stepValue, &step);
		*loopValue = loopValue->Float() + step;
	}
	else
		return false;

	return LoopAgain(runTimeEnvironment);
}

#ifdef _DEBUG
void StatementFor::Dump()
{
	puts("For");
	_var->Dump();
	puts("=");
	_from->Dump();
	puts("To");
	_to->Dump();
}
#endif

Error StatementProcedure::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'Function' or 'Sub'
	Token token;
	tokenReader->Next(token);
	_lineNumber = token.LineNumber();
	uint16_t column = token.Column();

	// Make sure we're not inside a control structure
	if (compileEnvironment.ControlStructureStack().size())
		return Error(NoNestedProcs, LineNumber(), Column());

	// Get the procedure name
	if (tokenReader->Next(token) && token.Type() == Token::Identifier)
	{
		_name = token.ValueIdentifier();
		// Make sure it's not a duplicate
		if (compileEnvironment.FindProcedure(_name) || FindBuiltInFunction(_name.c_str()) || compileEnvironment.FindStruct(_name.c_str()))
        {
            return Error(ProcedureAlreadyExists, token.LineNumber(), token.Column());
        }
	}
	else
		return Error(MissingProcedureName, LineNumber(), Column());

	compileEnvironment.ControlStructureStack().push_back(new ControlStructure(_type, this));

	compileEnvironment.EnterVariableScope();

	// Get the (optional) parameter list
	if (tokenReader->Next(token) && token.Type() == Token::LParen)
	{
		Array<String> names;

		while (tokenReader->Next(token) && token.Type() == Token::Identifier)
		{
			if (strcasecmp(token.ValueIdentifier(), "ByRef") == 0)
			{
				_byRefs |= 1 << names.size();
				if (!tokenReader->Next(token) || token.Type() != Token::Identifier)
					return Error(IllegalVariableSpec, token.LineNumber(), token.Column());
			}
			names.push_back((char *) token.ValueIdentifier());

			tokenReader->Prev();
			VarSpec *param;
			VarSpec::Build(tokenReader, compileEnvironment, token.LineNumber(), token.Column(), VarSpec::DimReference, &param);
			_parameters.push_back(param);
			// The parameters should just be variable names, with no array indexes or struct members
			if (param->HasArrayIndexes() || param->HasMemberSpec())
				return Error(IllegalVariableSpec, param->LineNumber(), param->Column());
			// There should be no duplicate names
			for (size_t i = 0; i < names.size() - 1; ++i)
			{
				if (strcasecmp(names[i].c_str(), names.back().c_str()) == 0)
					return Error(PreviouslyDefined, param->LineNumber(), param->Column());
			}

			if (!tokenReader->Next(token) || token.Type() != Token::Comma)
				break;
		}

		if (token.Type() != Token::RParen)
			return Error(MissingEndParen, token.LineNumber(), token.Column());
	}

	// Else (there's no parentheses) this better be a subroutine (not a function)
	else if (_type != ControlStructure::SUB)
		return Error(NoParameterList, _lineNumber, column);

	compileEnvironment.RegisterProcedure(this);
	_parameters.shrink_to_fit();

	return NoError;
}

Error StatementProcedure::Execute(Executive &exec, RunTimeEnvironment &)
{
	exec.GotoFollowingStatement(_endProc);
	return NoError;
}

void StatementProcedure::SetEndproc(Statement *endProc) 
{
	_endProc = endProc;
}

#ifdef _DEBUG
void StatementFunction::Dump()
{
	puts("Function");
}
#endif

Error StatementIf::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'if'
	Token token;
	tokenReader->Next(token);
	SetLocation(token.LineNumber(), token.Column());

	Error error = Expression::Build(tokenReader, compileEnvironment, _expression, token.LineNumber(), token.Column());
	if (error)
		return error;

	// Eat the optional THEN
	if (tokenReader->Next(token) && (token.Type() != Token::Identifier || strcasecmp(token.ValueIdentifier(), "then") != 0))
		tokenReader->Prev();

	compileEnvironment.ControlStructureStack().push_back(new ControlStructure(ControlStructure::IF, this));
	_elses.shrink_to_fit();

	return NoError;
}

Error StatementIf::AddElse(StatementElseif *stmt) 
{
	if (_elses.size() && _elses.back()->IsElse())
		return Error(PrecedingElse, stmt->LineNumber(), stmt->Column());
	_elses.push_back(stmt);
	return NoError;
}

void StatementIf::SetEndif(StatementEndif *stmt) 
{
	_endif = stmt;
	for (size_t i = 0; i < _elses.size(); ++i)
		_elses[i]->SetEndif(stmt);
}

Error StatementIf::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	// Evaluate our conditional expression
	Error error = Expression::Execute(_expression, exec, runTimeEnvironment);
	if (!error)
	{
		// If the condition was false
		int result = runTimeEnvironment.calculationStack.back().TypeBehaviors()->ToBool(runTimeEnvironment.calculationStack.back());
		runTimeEnvironment.calculationStack.pop_back();
		if (!result)
		{
			// Evaluate the conditions for the elseifs until we find one that's true
			for (size_t i = 0; i < _elses.size(); ++i)
			{
				exec.TraceStatement(_elses[i]);
				error = _elses[i]->EvaluateExpression(exec, runTimeEnvironment);
				if (error)
					return error;
				// If the condition was true
				result = runTimeEnvironment.calculationStack.back().TypeBehaviors()->ToBool(runTimeEnvironment.calculationStack.back());
				runTimeEnvironment.calculationStack.pop_back();
				if (result)
				{
					exec.GotoFollowingStatement(_elses[i]);
					return NoError;
				}
			}

			// None of the Elseifs were true; jump to the Endif
			exec.GotoFollowingStatement(_endif);
		}		
	}
	return error;
}

#ifdef _DEBUG
void StatementIf::Dump()
{
	puts("If");
	_expression->Dump();
}
#endif

Error StatementInclude::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	// Eat the 'include'
	Token token, fileName;
	tokenReader->Next(token);
	if (!tokenReader->Next(fileName) || fileName.Type() != Token::String)
	{
		return Error(ExpectedString, token.LineNumber(), token.Column());
	}
	_fileName = fileName.ValueString();
	return NoError;
}

#ifdef _DEBUG
void StatementInclude::Dump()
{
	puts("Include");
	fputs(_fileName.c_str(), stdout);
}
#endif

/*
Error StatementLabel::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	Token token;
	tokenReader->Next(token);
	SetLocation(token.LineNumber(), token.Column());
	_label = token.ValueIdentifier();
	tokenReader->Next(token);
	return NoError;
}

#ifdef _DEBUG
void StatementLabel::Dump() 
{
	fputs("Label ", stdout);
	puts((const char *) _label);
}
#endif
*/

Error StatementLoad::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	// Eat the 'Load'
	Token token;
	tokenReader->Next(token);
	SetLocation(token.LineNumber(), token.Column());

	tokenReader->TreatDotAsFileName(true);
	if (tokenReader->Next(token))
	{
		tokenReader->TreatDotAsFileName(false);
		if (token.Type() == Token::String)
			_fileName = token.ValueString();
		else if (token.Type() == Token::Identifier)
			_fileName = token.ValueIdentifier();
		else if (token.Type() == Token::FileName)
			_fileName = token.ValueFileName();
		else
			return Error(IllegalFileName, token.LineNumber(), token.Column());
		return NoError;
	}

	else
		tokenReader->TreatDotAsFileName(false);

	return Error(UnexpectedEndOfText, LineNumber(), Column());
}

Error StatementLoad::Execute(Executive &exec, RunTimeEnvironment &)
{
	Error error = exec.Load(_fileName);
	if (error && error.LineNumber() == 0)
		return Error(error.Message(), LineNumber(), Column());
	return error;
}

Error StatementMem::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	// Eat the 'mem'
	Token token;
	tokenReader->Next(token);

	return NoError;
}

Error StatementMem::Execute(Executive &exec, RunTimeEnvironment &)
{
	size_t used, free, largestFree;
	MemoryUsage(&used, &free, &largestFree);
//    Print(stdout, "Used: ", int(used), " bytes; free: ", int(free), " bytes; ");
//    Print(stdout, "largest free block: ", int(largestFree), " bytes\r\n");
    VPRINT("Used: %i bytes; free: %i bytes; ", int(used), int(free));
    VPRINT("largest free block: %i bytes\r\n", int(largestFree));
    
    
    exec.PrintMemoryUsage();
	return NoError;
}

Error StatementNew::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	// Eat the 'new'
	Token token;
	tokenReader->Next(token);

	return NoError;
}

Error StatementNew::Execute(Executive &exec, RunTimeEnvironment &)
{
	Error error = exec.New();
	return error;
}

Error StatementNext::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	Error error;

	// Eat the 'next'
	Token token;
	tokenReader->Next(token);
	_lineNumber = token.LineNumber();

	// Make sure we're in a FOR
	if (compileEnvironment.ControlStructureStack().size() == 0 || compileEnvironment.ControlStructureStack().back()->Type() != ControlStructure::FOR)
	{
		return Error(NoMatchingFor, LineNumber(), Column());
	}
	_for = (StatementFor *) compileEnvironment.ControlStructureStack().back()->OpeningStatement();
	_for->SetNext(this);
	delete compileEnvironment.ControlStructureStack().back();
	compileEnvironment.ControlStructureStack().back() = null;
	compileEnvironment.ControlStructureStack().pop_back();

	// Get the optional variable spec
	if (tokenReader->Next(token))
	{
		tokenReader->Prev();
		VarSpec *varSpec;
		error = VarSpec::Build(tokenReader, compileEnvironment, token.LineNumber(), token.Column(), VarSpec::LValue, &varSpec);
		if (!error && !_for->_var->Match(*varSpec))
			error = Error(NextVariableMismatch, varSpec->LineNumber(), varSpec->Column());
		delete varSpec;
	}

	return error;
}

Error StatementNext::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	return _for->Continue(exec, runTimeEnvironment);
}

#ifdef _DEBUG
void StatementNext::Dump()
{
	puts("Next");
}
#endif

#ifdef _DEBUG
void StatementMem::Dump()
{
	puts("Mem");
}
#endif

StatementOn::~StatementOn()
{
	delete _callSpec; 
}

// Statement ON ERROR
Error StatementOn::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'On'
	Token token;
	tokenReader->Next(token);
	_lineNumber = token.LineNumber();

	// Eat "Error"
	if (tokenReader->Next(token) && token.Type() == Token::Identifier && strcasecmp(token.ValueIdentifier(), "error") == 0)
	{
		// Eat "Call"
		if (tokenReader->Next(token) && token.Type() == Token::Identifier && strcasecmp(token.ValueIdentifier(), "call") == 0)
		{
			// Get the thing we're calling
			if (tokenReader->Next(token))
			{
				// It could be a zero (ON ERROR CALL 0)
				if (token.Type() == Token::Int && token.ValueInt() == 0)
				{
					_callSpec = null;
					return NoError;
				}
				// Or it could be the name of a subroutine
				if (token.Type() == Token::Identifier)
				{
					tokenReader->Prev();
					_callSpec = new CallSpec();
					return _callSpec->Build(tokenReader, compileEnvironment);
				}
			}

			return Error(ExpectedSubroutineOr0, token.LineNumber(), token.Column());
		}
	}

	return Error(ExpectedOnErrorCall, _lineNumber, token.Column());
}

// Statement ON ERROR
Error StatementOn::Execute(Executive &, RunTimeEnvironment &runTimeEnvironment)
{
	if (_callSpec)
		runTimeEnvironment.SetOnErrorCall(this);
	else
		runTimeEnvironment.SetOnErrorCall(0);
	return NoError;
}

// Statement ON ERROR
Error StatementOn::ExecuteErrorHandler(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	size_t oldStackSize = runTimeEnvironment.calculationStack.size();

    // Temporarily suspend On Error. If you get an error in the error handler, you break
    runTimeEnvironment.SetOnErrorCall(0); 
	Error rtn = _callSpec->Execute(exec, runTimeEnvironment);
    runTimeEnvironment.SetOnErrorCall(this);
	// If the stack size changed (i.e. we called a function that had a return value)
	// clean it up
	if (runTimeEnvironment.calculationStack.size() == oldStackSize + 1)
	{
		runTimeEnvironment.calculationStack.pop_back();
	}
	return rtn;
}

#ifdef _DEBUG
void StatementOn::Dump() 
{
	puts("On Error Call"); 
	if (_callSpec) 
		_callSpec->Dump(); 
}
#endif

int StatementOption::_optionBaseCount;

// Statement OPTION BASE
Error StatementOption::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	// Eat the 'Option'
	Token option;
	tokenReader->Next(option);

	// Eat "Base"
	Token token;
	if (tokenReader->Next(token))
	{
		if (token.Type() == Token::Identifier && strcasecmp(token.ValueIdentifier(), "base") == 0)
		{
			// Get the base value
			Token num;
			if (tokenReader->Next(num))
			{
				// It could be a zero (ON ERROR CALL 0)
				if (num.Type() == Token::Int && (num.ValueInt() == 0 || num.ValueInt() == 1))
				{
					int newBase = num.ValueInt();
					if (_optionBaseCount > 1)
						return Error(OnlyOneOptionBase, num.LineNumber(), num.Column());
					ARRAY_BASE = newBase;
					return NoError;
				}
				return Error(IllegalArgument, num.LineNumber(), num.Column());
			}
			return Error(UnexpectedEndOfText, token.LineNumber(), token.Column());
		}
		return Error(IllegalArgument, token.LineNumber(), token.Column());
	}

	return Error(UnexpectedEndOfText, option.LineNumber(), option.Column());
}

// Statement OPTION BASE
Error StatementOption::Execute(Executive &, RunTimeEnvironment &)
{
	return NoError;
}

#ifdef _DEBUG
void StatementOption::Dump()
{
	puts("OPTION BASE");
}
#endif

Error StatementPrint::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'Print'
	Token token;
	tokenReader->Next(token);
	_lineNumber = token.LineNumber();

	Error error = _expressionList.Build(tokenReader, compileEnvironment, false, token.LineNumber(), token.Column());
	// Allow an empty expression list (so that Print just prints a newline)
	if (error.Message() == EmptyExpression)
		error = NoError;
	return error;
}

Error StatementPrint::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	for (size_t i = 0; i < _expressionList.Expressions().size(); ++i)
	{
		size_t stackSize = runTimeEnvironment.calculationStack.size();
		Error error = Expression::Execute(_expressionList.Expressions()[i], exec, runTimeEnvironment);
		if (error)
			return error;
		if (runTimeEnvironment.calculationStack.size() != stackSize + 1)
			return Error(MustReturnValue, _expressionList.Expressions()[i]->LineNumber(), _expressionList.Expressions()[i]->Column());
		Value &result = runTimeEnvironment.calculationStack.back();
		result.Print();
		runTimeEnvironment.calculationStack.pop_back();
	}
	FPUTS("\r\n", stdout);
	return NoError;
}

Error StatementReload::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	// Eat the 'Reload'
	Token token;
	tokenReader->Next(token);

	return NoError;
}

Error StatementReload::Execute(Executive &exec, RunTimeEnvironment &)
{
	return exec.Reload();
}

Error StatementRepeat::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'Repeat'
	Token token;
	tokenReader->Next(token);
	SetLocation(token.LineNumber(), token.Column());

	compileEnvironment.ControlStructureStack().push_back(new ControlStructure(ControlStructure::REPEAT, this));

	return NoError;
}

Error StatementRepeat::Execute(Executive &, RunTimeEnvironment &)
{
	return NoError;
}

Error StatementRepeat::Continue(Executive &exec, RunTimeEnvironment &)
{
	exec.Goto(this);
	return NoError;
}

Error StatementRepeat::Exit(Executive &exec, RunTimeEnvironment &)
{
	exec.GotoFollowingStatement(_until);
	return NoError;
}

#ifdef _DEBUG
void StatementRepeat::Dump()
{
	puts("Repeat");
}
#endif

Error StatementResume::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	// Eat the 'Resume'
	Token token;
	tokenReader->Next(token);

	return NoError;
}

Error StatementResume::Execute(Executive &exec, RunTimeEnvironment &)
{
	return Error(exec.Resume(), 0, 0);
}

Error StatementReturn::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'Return'
	Token token;
	tokenReader->Next(token);
	_lineNumber = token.LineNumber();

	// Find out whether we're in a function or a subroutine
	if (compileEnvironment.ControlStructureStack().size() == 0)
		return Error(NoMatchingFunction, token.LineNumber(), token.Column());
	ControlStructure::Type_t type = compileEnvironment.ControlStructureStack()[0]->Type();
	if (type != ControlStructure::FUNCTION && type != ControlStructure::SUB)
		return Error(NoMatchingFunction, token.LineNumber(), token.Column());

	if (type == ControlStructure::FUNCTION)
	{
		Error error = Expression::Build(tokenReader, compileEnvironment, _expression, token.LineNumber(), token.Column());
		return error;
	}
	else
	{
		// We're in a subroutine, so there should be no return value, i.e. nothing after the "return" statement
		if (tokenReader->Next(token))
			return Error(MustNotReturnValue, token.LineNumber(), token.Column());
		return NoError;
	}
}

Error StatementReturn::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	if (_expression)
	{
		Error error = Expression::Execute(_expression, exec, runTimeEnvironment);
		if (error)
			return error;
		runTimeEnvironment.SetReturnValue(runTimeEnvironment.calculationStack.back());
		runTimeEnvironment.calculationStack.pop_back();
	}

	return exec.ReturnFromSub();
}

Error StatementRun::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	// Eat the 'Run'
	Token token;
	tokenReader->Next(token);

	return NoError;
}

Error StatementRun::Execute(Executive &exec, RunTimeEnvironment &)
{
	return exec.Step(-1);
}

Error StatementStep::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	// Eat the 'Step'
	Token token;
	tokenReader->Next(token);
	SetLocation(token.LineNumber(), token.Column());

	// If there's another token, assume it's a count of steps
	if (tokenReader->Next(token))
	{
		if (token.Type() == Token::Int)
			_count = token.ValueInt();
		else
			return Error(MustBeInteger, token.LineNumber(), token.Column());
	}
	return NoError;
}

Error StatementStep::Execute(Executive &exec, RunTimeEnvironment &)
{
	Error error = exec.Step(_count);
	// If there's no error, or if the error contains line and column numbers
	if (!error || error.LineNumber() || error.Column())
		// Return the error
		return error;
	// Otherwise, there's an error code with no position information
	return Error(error.Message(), LineNumber(), Column());
}

Error StatementStop::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	// Eat the 'Stop'
	Token token;
	tokenReader->Next(token);
	SetLocation(token.LineNumber(), token.Column());
	return NoError;
}

Error StatementStop::Execute(Executive &exec, RunTimeEnvironment &)
{
	return Error(exec.Stop(), LineNumber(), Column());
}

#ifdef _DEBUG
void StatementSub::Dump()
{
	puts("Function");
}
#endif

Error StatementStruct::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	bool done = false;
	Token token;

	// Eat the 'Struct'
	tokenReader->Next(token);
	Token prevToken = token;

	// Get the name of the struct
	if (!tokenReader->Next(token))
		return Error(IllegalStructDef, prevToken.LineNumber(), prevToken.Column());
	if (token.Type() != Token::Identifier)
		return Error(IllegalStructDef, token.LineNumber(), token.Column());
	const char *name = token.ValueIdentifier();
	// Make sure the name isn't already in use
	if (compileEnvironment.FindProcedure(name) || FindBuiltInFunction(name) || compileEnvironment.FindStruct(name))
		return Error(ProcedureAlreadyExists, token.LineNumber(), token.Column());

	StructDef *structDef = new StructDef(token.ValueIdentifier());

	// Get the open brace
	if (!tokenReader->Next(token))
		return Error(IllegalStructDef, prevToken.LineNumber(), prevToken.Column());
	prevToken = token;

	// If this looks like a structure definition
	if (token.Type() == Token::LBrace)
	{
		Array<String> memberNames;
		while (!done)
		{
			// Make sure there's a token
			if (!tokenReader->Next(token))
				return Error(UnexpectedEndOfText, prevToken.LineNumber(), prevToken.Column());
			if (token.Type() != Token::Identifier)
				return Error(IllegalStructDef, token.LineNumber(), token.Column());

			memberNames.push_back(token.ValueIdentifier());
			if (!tokenReader->Next(token))
				return Error(MissingEndBrace, prevToken.LineNumber(), prevToken.Column());
			if (token.Type() == Token::RBrace)
				done = true;
			else if (token.Type() != Token::Comma)
				return Error(MissingEndBrace, LineNumber(), Column());
		}
		structDef->SetMemberNames(memberNames);

		compileEnvironment.RegisterStruct(structDef);
	}

	else
		return Error(IllegalStructDef, prevToken.LineNumber(), prevToken.Column());

	return NoError;
}

Error StatementStruct::Execute(Executive &, RunTimeEnvironment &)
{
	return NoError;
}

#ifdef _DEBUG
void StatementStruct::Dump()
{
/*	Print(stdout, "Struct ", _structDef.Name().c_str());
	Print(stdout, " {");
	size_t i;
	for (size_t i = 0; i < _structDef.MemberCount() - 1; ++i)
	{
		Print(stdout, _structDef.IndexToName(i).c_str(), ", ");
	}
	Print(stdout, _structDef.IndexToName(i).c_str(), "}\r\n");*/
}
#endif

Error StatementTroff::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	// Eat the 'Troff'
	Token token;
	tokenReader->Next(token);
	return NoError;
}

Error StatementTroff::Execute(Executive &exec, RunTimeEnvironment &)
{
	exec.Trace(false);
	return NoError;
}

Error StatementTron::Build(TokenReader *tokenReader, CompileEnvironment &)
{
	// Eat the 'Tron'
	Token token;
	tokenReader->Next(token);
	return NoError;
}

Error StatementTron::Execute(Executive &exec, RunTimeEnvironment &)
{
	exec.Trace(true);
	return NoError;
}

Error StatementUntil::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'Until'
	Token token;
	tokenReader->Next(token);

	// Make sure we're in an REPEAT
	if (compileEnvironment.ControlStructureStack().size() == 0 || compileEnvironment.ControlStructureStack().back()->Type() != ControlStructure::REPEAT)
	{
		return Error(NoMatchingRepeat, token.LineNumber(), token.Column());
	}
	_repeat = (StatementRepeat *) compileEnvironment.ControlStructureStack().back()->OpeningStatement();

	// Tell the REPEAT statement about us
	_repeat->SetUntil(this);

	// Exit the REPEAT control structure
	delete compileEnvironment.ControlStructureStack().back();
	compileEnvironment.ControlStructureStack().back() = null;
	compileEnvironment.ControlStructureStack().pop_back();

	Error error = Expression::Build(tokenReader, compileEnvironment, _expression, token.LineNumber(), token.Column());
	if (error)
		return error;

	return NoError;
}

Error StatementUntil::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	// Evaluate the expression
	Error error = Expression::Execute(_expression, exec, runTimeEnvironment);
	if (error)
		return error;
	Value &cond = runTimeEnvironment.calculationStack.back();
	int truth = cond.TypeBehaviors()->ToBool(cond);
	runTimeEnvironment.calculationStack.pop_back();

	if (!truth)
		exec.Goto(_repeat);

	return NoError;
}

#ifdef _DEBUG
void StatementUntil::Dump()
{
	puts("Until");
	_expression->Dump();
}
#endif

Error StatementWhile::Build(TokenReader *tokenReader, CompileEnvironment &compileEnvironment)
{
	// Eat the 'While'
	Token token;
	tokenReader->Next(token);
	SetLocation(token.LineNumber(), token.Column());

	Error error = Expression::Build(tokenReader, compileEnvironment, _expression, token.LineNumber(), token.Column());
	if (error)
		return error;

	// Eat the optional DO
	if (tokenReader->Next(token) && (token.Type() != Token::Identifier || strcasecmp(token.ValueIdentifier(), "do") != 0))
		tokenReader->Prev();

	compileEnvironment.ControlStructureStack().push_back(new ControlStructure(ControlStructure::WHILE, this));

	return NoError;
}

Error StatementWhile::Execute(Executive &exec, RunTimeEnvironment &runTimeEnvironment)
{
	// Evaluate the expression
	Error error = Expression::Execute(_expression, exec, runTimeEnvironment);
	if (error)
		return error;
	Value &cond = runTimeEnvironment.calculationStack.back();
	int truth = cond.TypeBehaviors()->ToBool(cond);
	runTimeEnvironment.calculationStack.pop_back();

	if (!truth)
		Exit(exec, runTimeEnvironment);

	return NoError;
}

Error StatementWhile::Continue(Executive &exec, RunTimeEnvironment &)
{
	exec.Goto(this);
	return NoError;
}

Error StatementWhile::Exit(Executive &exec, RunTimeEnvironment &)
{
	exec.GotoFollowingStatement(_endWhile);
	return NoError;
}

#ifdef _DEBUG
void StatementWhile::Dump()
{
	puts("While");
	_expression->Dump();
}
#endif
