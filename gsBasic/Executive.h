#ifndef _EXECUTIVE_H_
#define _EXECUTIVE_H_

#include "Array.h"
#include "Statements.h"
#include "GSBString.h"
#include "Error.h"
#include "Token.h"
#include "Environment.h"
#include "GSBASICNotifications.h"
#include <stdio.h>

class StatementProcedure;
class TokenReader;

class Executive
{
public:
    /**
     * 
     * @param echoOn true if characters the user types should be echoed back
     * @param newlineChar the character that the console sends up when the user hits Enter
     */
	Executive(GSBASICNotifications *notifications, bool echoOn, char newlineChar);
	~Executive();

	void Main(int argc, char *argv[]);

	Error New();
	Error Load(const String &fileName, FILE *file = NULL);
	Error Reload();
	Error Run();
	ErrorCode Resume();
	ErrorCode Stop();
	Error Step(int stepCount);

	void Trace(bool trace) { _trace = trace; }
	Error BreakpointCommand(StatementBp::BreakpointCommand cmd, uint16_t lineNumber, uint16_t fileIndex);

	void Goto(Statement *stmt);
	void GotoFollowingStatement(Statement *stmt);
	Error Gosub(StatementProcedure *stmt, RunTimeEnvironment &runTime);
	Error ReturnFromSub();
	Error BreakIntoDebugger();

	const String &SourceFileName(size_t fileIndex) const;
	size_t SourceFileIndex(const String &fileName) const;

	void TraceStatement(Statement * stmt);

	Error Chain(const String &fileName, bool rerun);
    
    // Let the enclosing app print extra info about memory usage
    void PrintMemoryUsage();

private:
	void ImmediateMode(bool *exit);
	void DebugMode(bool *exit);

	Error ReadBlock(RunTimeEnvironment &runTime, PtrArray<Statement> &statements, Array<String> &lines, 
		size_t &lineNumberOffset, FILE *input, FILE *output, bool showPrompt, bool userInput, char eolChar, 
		size_t includeIndex, bool *exit);

	Error Include(const String &fileName, uint16_t lineNumber, uint16_t column);

	ErrorCode ReadStatementText(FILE *input, FILE *output, bool showPrompt, Array<String> &lines, char eolChar);
	Error Parse(TokenReader &reader, Statement *&statement, CompileEnvironment &compileEnvironment);

	void DisplayError(FILE *output, const Array<String> &lines, const Error &error, bool printLineNumber);

	Error Execute(const PtrArray<Statement> &codeBlock, int &pc);

	GSBASICNotifications *_notifications;
	bool _echoOn;
    // This is the character the console sends us when the user hits Enter
    char _consoleNewlineChar;
    
	RunTimeEnvironment _runTime;

	State_t _state;
	int _debugDepth;

	String _currentProgram, _previousLoad;
	PtrOwnerArray<Statement> _program;
	Array<String> _includeFiles;
	int _programPC;



	int _immediatePC;

	// The number of statements to execute before breaking into the debugger. -1 means 'infinite'
	int _steps;

	int *_activeProgramCounter; // Either _programPC or _immediatePC
	const PtrArray<Statement> *_currentCodeBlock;
    
    char _lineHistory[10][MAX_LINE_LENGTH + 1];
    int _lineHistoryTail, _lineHistoryPointer;

	// Indicates whether we should print prompts
	bool _quiet;

	bool _firstPrompt;

	bool _trace;
	Array<size_t> _breakpoints;

	Array<String> _chainStack;
	String _chainTo;
	bool _chainRun;

	String _toLoad;
};

#endif
