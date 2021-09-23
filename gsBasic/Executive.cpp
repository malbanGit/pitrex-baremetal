
#include <ctype.h>
#include <stdexcept>
#include "Executive.h"
#include "Lexer.h"
#include "Parser.h"
#include "Utilities.h"
#include "Token.h"
#include "CallSpec.h"
#include "LineNumber.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <vectrex/vectrexInterface.h>
#include <uspi.h>
#include "vio.h"

#ifdef __cplusplus
}
#endif
extern "C" void ClearVectrex();
extern int disableSprites;


// Reset the outer application, i.e. the app that's embedding GSBasic.
// This function must be implemented by the outer app and should reset
// anything related to the GSBasic loaded program

extern bool ctrlC;

Executive::Executive(GSBASICNotifications *notifications, bool echoOn, char newlineChar) :
	_notifications(notifications), _echoOn(echoOn), _consoleNewlineChar(newlineChar), _state(Immediate), _debugDepth(0),
    _steps(-1), _lineHistoryTail(0), _lineHistoryPointer(0), _quiet(false), _firstPrompt(true),
	_trace(false), _chainRun(false)
{
    memset(_lineHistory, 0, sizeof(_lineHistory));
	notifications->RegisterConstants();
	notifications->NewState(_state);
}

Executive::~Executive()
{
	_program.clear();
}

void Executive::Main(int argc, char *argv[])
{
  
  bool exit = false;
  clearVectrexOut();

  for (int i = 1; i < argc; ++i)
  {
      if (strcmp(argv[i], "-q") == 0)
          _quiet = true;
  }

  // If there's an autorun.bas file
  FILE *autorun = fopen("autorun.bas", "rt");
  if (autorun)
  {
      VPRINT("autorun.bas found... loading...\n\r");
      Error error = Load("autorun.bas", autorun);
      if (!error)
      {
          Run();
      }
      else
      {
        VPRINT("Load error...\n\r");
      }
  }
  
  while (1)
  {
    ImmediateMode(&exit);
  }
}

void Executive::ImmediateMode(bool *exit)
{
  disableSprites = 1;
	*exit = false;
	while (!*exit)
	{
		Array<String> lines;
		PtrOwnerArray<Statement> statements;

		if (!_quiet && _chainTo.empty() && !_chainRun)
		{
			if (_state == Immediate)
			{
				if (_firstPrompt)
				{
					// Output a greeting, e.g. "Product Foo Copyright (c)2016 ..."
					// const char *Greeting is defined in the embedding app
                    _notifications->DisplayGreeting();
					_firstPrompt = false;
				}
				else
					FPUTS("\r\nReady\r\n\n", stdout);
			}
			else
              VPRINT("\r\nDebug (depth = %i)\r\n", _debugDepth);
//				Print(stdout, "\r\nDebug (depth = ", _debugDepth, ")\r\n\n");
		}
		
        if (vectrexMode) showVectrexOut();
        v_WaitRecal();
		
		size_t lineOffset = 0;
		Error error;
		if (_chainTo.empty() && !_chainRun && _toLoad.empty())
		{
			error = ReadBlock(_runTime, statements, lines, lineOffset, stdin, stdout, !_quiet, true, _consoleNewlineChar, 0, exit);
		}

		if (ctrlC)
		{
			ctrlC = false;
			FPUTS("^C\r\n", stdout);
            continue;
		}

		if (error)
		{
			_runTime.compileEnvironment.ClearCallSpecs();
		}

		else if (!_toLoad.empty())
		{
			error = Load(_toLoad);
			_toLoad.clear();
		}

		else if (statements.size() || !_chainTo.empty() || _chainRun)
		{
			error = _runTime.compileEnvironment.ResolveCallSpecs();

			// If the user typed in a procedure, we need to save it as part of the program
			if (statements.size() && statements[0]->IsProcedure())
			{
				for (size_t i = 0; i < statements.size(); ++i)
				{
					statements[i]->SetStatementNumber(_program.size());
					_program.push_back(statements[i]);
					statements[i] = null;
				}
			}

			// Else (not a procedure)
			else
			{
				StatementLoad *load;
				_immediatePC = 0;

				// If we're doing the Load part of a CHAIN command
				if (!_chainTo.empty())
				{
					statements.clear();
					error = Load(_chainTo);
					_chainTo.clear();
					if (!error)
						_chainRun = true;
				}
				// Else if we're doing the Run part of a Chain
				else if (_chainRun)
				{
					_chainRun = false;
					error = Run();
				}
				// else if it's a Reload command
				else if (dynamic_cast<StatementReload *>(statements[0]))
				{
					statements.clear();
					error = Reload();
				}
				// Else if it's a load command
				else if ((load = dynamic_cast<StatementLoad *>(statements[0])) != nullptr)
				{
					String fileName = load->FileName();
					statements.clear();
					error = Load(fileName);
                    
                    if (_state == Immediate)
                      continue;
				}
				else
				{
					error = Execute(statements, _immediatePC);
				}
				
				if (error.Message() == ResumeRunning)
                {
                    break;
                }
				if (error.Message() == StopExecution)
				{
                      *exit = true;
                      break;
				}
				// Display the error message, except if it's AlreadyPrinted
				else 

                  if (error)
					DisplayError(stdout, lines, error, false);
			}
		}
		statements.clear();
	}
    disableSprites = 0;
	//ClearVectrex();
}

Error Executive::New()
{
	if (_state == Running)
		return Error(ConsoleOnly, 1, 1);
	else if (_state == Debug)
	{
		_toLoad = "\xff";
		return Error(Stop(), 0, 0);
	}

	_currentProgram = _previousLoad = "";

	_runTime.Clear();
	_program.clear();
	_includeFiles.clear();
	_breakpoints.clear();
	ARRAY_BASE = 1;
	Trace(false);

	// Reset the outer application, i.e. the app that's embedding GSBasic.
	// This function must be implemented by the outer app and should reset
	// anything related to the GSBasic loaded program
	_notifications->RuntimeReset();

	return NoError;
}
int isInFreeing = 0;
Error Executive::Load(const String &fileName, FILE *file)
{
    if (_state == Running)
		return Error(ConsoleOnly, 1, 1);
	else if (_state == Debug)
	{
		_toLoad = fileName;
		return Error(Stop(), 0, 0);
	}

    // Open the file
    if (file == NULL)
        file = fopen(fileName.c_str(), "rt");
    // If we couldn't open it
	if (!file)
	{
        // Try adding a .bas extension
		String withExt(fileName, ".bas");
		file = fopen(withExt.c_str(), "rt");
		if (file)
			_currentProgram = _previousLoad = withExt;
		else
			return Error(CantOpenFile, 1, 1);
	}
	else
		_currentProgram = _previousLoad = fileName;

    ClearVectrex();
    isInFreeing = 1;
    _runTime.Clear();
	_program.clear();
	_includeFiles.clear();
	_breakpoints.clear();
	ARRAY_BASE = 1;
	Trace(false);
    isInFreeing = 0;

	// Reset the outer application, i.e. the app that's embedding GSBasic.
	// This function must be implemented by the outer app and should reset
	// anything related to the GSBasic loaded program
	_notifications->RuntimeReset();
    
	Array<String> lines;
	size_t lineOffset = 0;
	bool exit = false;

	do
	{
		Error error = ReadBlock(_runTime, _program, lines, lineOffset, file, stdout, false, false, '\n', 0, &exit);
		if (error)
		{
			fclose(file);
            _currentProgram.clear();
			_runTime.Clear();
			DisplayError(stdout, lines, error, true);
			return Error(AlreadyPrinted, 0, 0);
		}
	} while (!feof(file));

	fclose(file);

	Error error = _runTime.compileEnvironment.ResolveCallSpecs();
	if (error)
	{
		DisplayError(stdout, lines, error, true);
		error = Error(AlreadyPrinted, 0, 0);
	}

	return error;
}

Error Executive::Include(const String &fileName, uint16_t lineNumber, uint16_t column)
{
	// Check whether we've already included the file
	for (size_t i = 0; i < _includeFiles.size(); ++i)
	{
		if (String::CompareCaseInsensitive(fileName, _includeFiles[i]) == 0)
			return NoError;
	}

	if (_includeFiles.size() == 8)
		return Error(TooManyIncludes, lineNumber, column);

	// See if we can open the file
	FILE *file = fopen(fileName.c_str(), "rt");
	// If we couldn't open it
	if (!file)
		return Error(CantOpenFile, lineNumber, column);

	// Add the file name to the list
	_includeFiles.push_back(fileName);
	size_t includeFileIndex = _includeFiles.size();

	Array<String> lines;
	size_t lineOffset = 0;
	bool exit = false;

	do
	{
		Error error = ReadBlock(_runTime, _program, lines, lineOffset, file, stdout, false, false, '\n', includeFileIndex, &exit);
		if (error)
		{
			fclose(file);
			return error;
		}
	} while (!feof(file));

	fclose(file);

	return NoError;
}

const String &Executive::SourceFileName(size_t fileIndex) const
{
	if (fileIndex && fileIndex <= _includeFiles.size())
		return _includeFiles[fileIndex - 1];
	else
		return _currentProgram;
}

size_t Executive::SourceFileIndex(const String &fileName) const
{
	for (size_t i = 0; i < _includeFiles.size(); ++i)
	{
		if (String::CompareCaseInsensitive(_includeFiles[i], fileName) == 0)
			return i + 1;
	}
	return 0;
}

Error Executive::Reload()
{
	if (!_previousLoad)
		return Error(NoProgramLoaded, 0, 0);
	return Load(_previousLoad);
}

Error Executive::Run()
{
	if (_state != Immediate)
		return Error(ConsoleOnly, 0, 0);
	if (!_currentProgram)
		return Error(NoProgramLoaded, 0, 0);

    
	// Reset the outer application, i.e. the app that's embedding GSBasic.
	// This function must be implemented by the outer app and should reset
	// anything related to the GSBasic loaded program
	_notifications->RuntimeReset();
	_runTime.Reset();

	_programPC = 0;
	State_t prevState = _state;
	_state = Running;
	_notifications->NewState(_state);

disableSprites = 0;
      Error error = Execute(_program, _programPC);
disableSprites = 1;

//    if (error)
//      VPRINT("Run() - Error %i %s \n\r", error.Message(), error.Message());
    if (error && error.Message() != Breakpoint && error.Message() != StopExecution)
	{
        Array<String> lines;
		DisplayError(stdout, lines, error, true);
		// If there was an error, don't return to a previously run program
		_chainStack.empty();
		error = Error(AlreadyPrinted, 0, 0);
	}
	// Else if the program has terminated, and the program was run by a CHAIN <program> RERUN
	else if ((!error || error.Message() == StopExecution) && !_chainStack.empty() && _chainTo.empty())
	{
		_chainTo = _chainStack.back();
		_chainStack.pop_back();
	}
	_state = prevState;
	_notifications->NewState(_state);
	return error;
}

ErrorCode Executive::Resume()
{
	if (_state != Debug)
		return DebugModeOnly;
	_steps = -1;
	return ResumeRunning;
}

Error Executive::Step(int stepCount)
{
	if (_state == Running)
		return Error(NotInProgram, 0, 0);
	_steps = stepCount;
	if (_state == Immediate)
		return Run();
	else
		return Error(ResumeRunning, 0, 0);
}

ErrorCode Executive::Stop()
{
	// If the user typed the Stop command (as opposed to it being hit in a program)
	if (_state != Running)
		// The user has control; don't rerun any program we chained from
		_chainStack.clear();
    _notifications->RuntimeReset();
	return StopExecution;
}

Error Executive::ReadBlock(RunTimeEnvironment &runTime, PtrArray<Statement> &statements, Array<String> &lines, 
	size_t &lineNumberOffset, FILE *input, FILE *output, bool showPrompt, bool userInput, 
	char eolChar, size_t fileIndex, bool *exit)
{
	size_t controlStructureDepth = runTime.compileEnvironment.ControlStructureStack().size();
	Error error;
	// Read lines and parse into statements until the control structure depth has returned to
	// where it started (e.g. if we read an IF statement, keep reading statements until we get
	// to the ENDIF)
	do
	{
		// Read lines of text until we reach one that doesn't end in an underscore.
		// This, presumably, is one complete statement
		lines.resize(0);
		ErrorCode ec = ReadStatementText(input, output, showPrompt, lines, eolChar);
		if (ctrlC)
		{
			runTime.compileEnvironment.ControlStructureStack().resize(0);
			runTime.compileEnvironment.ClearCallSpecs();
			break;
		}
		if (ec)
		{
			puts(ec);
			continue;
		}

		// Get a token reader
		TokenReader tokenReader(lines, lineNumberOffset, fileIndex);
		lineNumberOffset += lines.size();
		// Parse the tokens into a statement
		Statement *stmt = null;
		error = Parse(tokenReader, stmt, runTime.compileEnvironment);
		if (error)
		{
			delete stmt;
			DisplayError(output, lines, error, !userInput);
			if (error.Message() == OutOfProgramMemory)
			{
				_program.clear();
				_includeFiles.clear();
			}
			return Error(AlreadyPrinted, 0, 0);
		}
		if (stmt && dynamic_cast<StatementComment *>(stmt) == null)
		{
			stmt->SetStatementNumber(statements.size());
			statements.push_back(stmt);

			StatementInclude *inc;
			if ((inc = dynamic_cast<StatementInclude *>(stmt)) != null)
			{
				error = Include(inc->FileName(), inc->LineNumber(), inc->Column());
				if (error)
				{
					DisplayError(output, lines, error, !userInput);
					return Error(AlreadyPrinted, 0, 0);
				} 
			}
		}
		else
			delete stmt;

		if (showPrompt && output && runTime.compileEnvironment.ControlStructureStack().size() > controlStructureDepth)
			FPUTS("&>", output);
        
	} while (!error && runTime.compileEnvironment.ControlStructureStack().size() > controlStructureDepth && !feof(input));

	if (runTime.compileEnvironment.ControlStructureStack().size() > controlStructureDepth)
		return Error(UnexpectedEndOfText, statements.back()->LineNumber(), statements.back()->Column());

	return NoError;
}

ErrorCode Executive::ReadStatementText(FILE *input, FILE *output, bool showPrompt, Array<String> &lines, char eolChar)
{
	lines.resize(0);
	char buf[MAX_LINE_LENGTH + 1];
	int len = 0;
	bool getMore = false;
    bool echo = _echoOn && input == stdin;
    bool enableHistory = (input == stdin);
    
	do
	{
		getMore = true;
		int c =  fgetc(input);
        
        if ((vectrexMode) && (input==stdin) &&(_state != Running) )
        {
          FPUTC(c,stdout);
          showVectrexOut();
          v_WaitRecal();
        }
        
        // If we got an Escape (the first character of a VT-100 sequence)
        if (enableHistory && c == '\x1b')
        {
            c =  fgetc(input);
            if (c == '[')
            {
                c =  fgetc(input);
                if (c == 'A')
                    c = '\x10'; // Ctrl+P to do a Previous
                else if (c == 'B')
                    c = '\x0e'; // Ctrl+N to do a Next
                else
                    continue;
            }
        }
        // If we got an Ctrl+P or Ctrl+N (previous or next in history)
        if (enableHistory && (c == '\x10' || c == '\x0e'))
        {
            size_t attempts = 0;
            // If it's a Cursor Up
            if (c == '\x10')
            {
                do
                {
                    if (--_lineHistoryPointer < 0)
                        _lineHistoryPointer = countof(_lineHistory);
                    ++attempts;
                } while (_lineHistory[_lineHistoryPointer][0] == '\0' && attempts < countof(_lineHistory));
            }
            // Else if it's a Cursor Down
            else if (c == '\x0e')
            {
                do
                {
                    if ((size_t) ++_lineHistoryPointer >= countof(_lineHistory))
                        _lineHistoryPointer = 0;
                    ++attempts;
                } while (_lineHistory[_lineHistoryPointer][0] == '\0' && attempts < countof(_lineHistory));
            }

            // Get the length of the history line
            int newLen = strlen(_lineHistory[_lineHistoryPointer]);
            if (newLen == 0)
                continue;
            // Backspace over whatever the user has already entered
            if (echo)
            {
                memset(buf, '\b', len);
                buf[len] = '\0';
                // If the old length is greater than the new length
                if (len > newLen)
                {
                    // Erase the excess characters
                    FPUTS(buf + newLen, stdout);
                    for (int i = newLen; i < len; ++i)
                        FPUTC(' ', stdout);
                }
                // Backspace to the beginning of the line
                FPUTS(buf, stdout);
                // Write the line from the history
                FPUTS(_lineHistory[_lineHistoryPointer], stdout);
            }
            // Use the history line instead
            strcpy(buf, _lineHistory[_lineHistoryPointer]);
            len = newLen;
            continue;
        }
		// If we got a Ctrl+C
		if (ctrlC || (input == stdin && c == '\x3'))
		{
			lines.resize(0);
			ctrlC = true;
			return 0;
		}
		if (c == EOF)
        {
          c = eolChar;
        }
        // If the user typed a printable character
		if (c >= ' ' && c <= '~')
		{
			buf[len++] = char(c);
            if (echo)
            {
                FPUTC(c, stdout);
            }
			if (len == sizeof(buf) - 1)
				return LineTooLong;
		}
        // Else if it's a backspace
        else if (c == 127/*'\b'*/)
        {
            if (len)
            {
              if (printBackspace())
              {
                if (echo)
                    FPUTS("\b \b", stdout);
                buf[--len] = '\0';
              }
            }
        }
        // Else if user hit Enter
		else if (c == eolChar)
		{
            // Check whether we need to echo a line feed
            if (echo && eolChar == '\r')
                FPUTS("\r\n", stdout);
            
			// Trim white space at the end of the line
			while (len && isspace(buf[len - 1]))
				--len;
			// Check for a line continuation character
			if (buf[len - 1] == '_')
			{
				--len;
				getMore = true;
			}
			else
				getMore = false;
            
            buf[len] = '\0';

            // Save the line in the history
            if (enableHistory)
            {
                if (len)
                {
                    // If the line is the same as the preceding line, don't save it
                    int i = _lineHistoryTail - 1;
                    if (i < 0)
                        i = countof(_lineHistory) - 1;
                    if (strcmp(buf, _lineHistory[i]))
                    {
                        strcpy(_lineHistory[_lineHistoryTail], buf);
                        if ((size_t) ++_lineHistoryTail >= countof(_lineHistory))
                            _lineHistoryTail = 0;
                    }
                }
                _lineHistoryPointer = _lineHistoryTail;
            }
            
			buf[len++] = '\n';
			buf[len] = '\0';
            
 			lines.push_back(buf);
			len = 0;

			if (getMore && showPrompt)
				FPUTS("_>", output);
		}
	} while (getMore && !ctrlC);
	return 0;
}

Error Executive::Parse(TokenReader &reader, Statement *&stmt, CompileEnvironment &compileEnvironment)
{
	Parser parser(&reader);
	Error error;

	stmt = parser.NextStatement(error, compileEnvironment);
	if (!error)
	{
		// Make sure we're at the end of the line
		Token token;
		if (reader.Next(token) && token.Type() != Token::Comment)
			error = Error(ExpectedEOL, token.LineNumber(), token.Column());
	}
	return error;
}

void Executive::DisplayError(FILE *output, const Array<String> &lines, const Error &error, bool printLineNumber)
{
	if (output && error.Message() != AlreadyPrinted)
	{
		if (!printLineNumber && lines.size() >= error.LineNumber())
		{
			FPUTS(lines[error.LineNumber() - 1].c_str(), output);
            FPUTC('\r', output);
			for (int i = 0; i < int(error.Column()) - 1; ++i)
				fputc(' ', output);
			FPUTS("^ ", output);
		}
		else if (error.LineNumber() && error.Column())
		{
			size_t fileIndex = error.FileIndex();
			if (fileIndex && fileIndex <= _includeFiles.size())
			{
//				Print(output, ("In file " + _includeFiles[fileIndex - 1] + ", line ").c_str(), error.LineNumber(), ", column ", error.Column(), ": ");
                VFPRINT(output, "In file %s, line %i, column %i: ", _includeFiles[fileIndex - 1],error.LineNumber() , error.Column());
			}
			else
            {
//                Print(output, "At line ", error.LineNumber(), ", column ", error.Column(), ": ");
                VFPRINT(output, "At line %i, column %i: ", error.LineNumber(), error.Column());
                char m1[255];
                sprintf(m1,"AT LINE %i, COLUMN %i: %s",error.LineNumber() , error.Column(), error.Message());
                v_message(m1);
            }
		}
		FPUTS(error.Message(), output);
		FPUTC('\n', output);
	}
}

Error Executive::Execute(const PtrArray<Statement> &codeBlock, int &pc)
{
	Error error;
	bool exit = false;

	_activeProgramCounter = &pc;
	_currentCodeBlock = &codeBlock;

	while (*_activeProgramCounter >= 0 && *_activeProgramCounter < int(codeBlock.size()) && !exit)
	{
		Statement *stmt = codeBlock[(*_activeProgramCounter)++];
        unsigned char key =USPiKeyboardLastKeyDown();
        if (_state == Running) if (key == 0x03)  
        {
          ctrlC = 1;
        }
        
        
		if (ctrlC)
		{
            if (vectrexMode) 
            {
              clearVectrexOut();
//              ClearVectrex();
              v_WaitRecal();
            }
			ctrlC = false;
			uint16_t fileIndex = stmt->FileIndex();
			if (fileIndex && fileIndex <= _includeFiles.size())
//				Print(stdout, ("^C\r\nBreak in file " + _includeFiles[fileIndex - 1] + ", line ").c_str(), stmt->LineNumber(), "\r\n");
                VFPRINT(stdout, "^C\r\nBreak in file %s, line %i ", _includeFiles[fileIndex - 1], stmt->LineNumber());
			else
//				Print(stdout, "^C\r\nBreak at line ", stmt->LineNumber(), "\r\n");
                VFPRINT(stdout, "^C\r\nBreak in line %i ", stmt->LineNumber());
			DebugMode(&exit);
		}
		else if (_steps != -1)
		{
			if (--_steps == 0)
			{
				uint16_t fileIndex = stmt->FileIndex();
				if (fileIndex && fileIndex <= _includeFiles.size())
//					Print(stdout, ("\r\nBreak in file " + _includeFiles[fileIndex - 1] + ", line ").c_str(), stmt->LineNumber(), "\r\n");
                VFPRINT(stdout, "C\r\nBreak in file %s, line %i ", _includeFiles[fileIndex - 1], stmt->LineNumber());
				else
//					Print(stdout, "\r\nBreak at line ", stmt->LineNumber(), "\r\n");
                VFPRINT(stdout, "C\r\nBreak in line %i ", stmt->LineNumber());
				DebugMode(&exit);
			}
		}
		else if (_state == Running)
		{
          
          
          
			bool found = false;
			for (size_t i = 0; i < _breakpoints.size() && !found; ++i)
			{
				if (_breakpoints[i] == EncodeLineNumber(stmt->FileIndex(), stmt->LineNumber()))
					found = true;
			}
			if (found)
			{
				size_t fileIndex = stmt->FileIndex();
                if (fileIndex && fileIndex <= _includeFiles.size())
//                  Print(stdout, ("\r\nBreak in file " + _includeFiles[fileIndex - 1] + ", line ").c_str(), stmt->LineNumber(), "\r\n");
                VFPRINT(stdout, "C\r\nBreak in file %s, line %i ", _includeFiles[fileIndex - 1], stmt->LineNumber());
				else
//                  Print(stdout, "\r\nBreak at line ", stmt->LineNumber(), "\r\n");
                VFPRINT(stdout, "C\r\nBreak in line %i ", stmt->LineNumber());
				DebugMode(&exit);
			}
		}
		if (exit)
			continue;

		TraceStatement(stmt);
		error = stmt->Execute(*this, _runTime);
		if (error)
		{
          void *p2=stmt;
          
			if (error.Message() == Breakpoint)
			{
				ctrlC = false;
				error = NoError;
				size_t fileIndex = stmt->FileIndex();
				if (fileIndex && fileIndex <= _includeFiles.size())
//                  Print(stdout, ("\r\nBreak in file " + _includeFiles[fileIndex - 1] + ", line ").c_str(), stmt->LineNumber(), "\r\n");
                VFPRINT(stdout, "C\r\nBreak in file %s, line %i ", _includeFiles[fileIndex - 1], stmt->LineNumber());
				else
//                  Print(stdout, "\r\nBreak at line ", stmt->LineNumber(), "\r\n");
                VFPRINT(stdout, "C\r\nBreak in line %i ",stmt->LineNumber());
				DebugMode(&exit);
				continue;
			}
			else if (IsSignalMessage(error.Message()))
				return error;

			// Else (it's a real error)
			else
			{
				if (_state == Running)
				{
//                VPRINT("Execute terminated, '%s', line: %i\n\r",  error.Message(), error.LineNumber());
                  
                  
					_runTime.SetLastError(error);

					// If there's an error handler
					StatementOn *onError = _runTime.GetOnErrorCall();
					if (onError)
					{
    Error orgError = error;
						error = onError->ExecuteErrorHandler(*this, _runTime);
						if (error.Message() == Breakpoint)
						{
							ctrlC = false;
							error = NoError;
							size_t fileIndex = stmt->FileIndex();
							if (fileIndex && fileIndex <= _includeFiles.size())
//                  Print(stdout, ("\r\nBreak in file " + _includeFiles[fileIndex - 1] + ", line ").c_str(), stmt->LineNumber(), "\r\n");
                VFPRINT(stdout, "C\r\nBreak in file %s, line %i ", _includeFiles[fileIndex - 1], stmt->LineNumber());
							else
//                  Print(stdout, "\r\nBreak at line ", stmt->LineNumber(), "\r\n");
                VFPRINT(stdout, "C\r\nBreak in line %i ", stmt->LineNumber());
							DebugMode(&exit);
							continue;
						}
						else if (error.Message() == StopExecution)
                        {
                          
                            return orgError;
                        }
						else if (error)
                        {
                            break;
                        }
                        else
                        {
                char m1[255];
                sprintf(m1,"AT LINE %i, COLUMN %i: %s (CONTINUING)",orgError.LineNumber() , orgError.Column(), orgError.Message());
                v_message(m1);
                        }
					}
					// Else (no error handler) stop executing
					else
						break;
				}

				// Else (not in Run state)
				else
					break;
			}
		}
	}

	if (error)
    {
        return error;
    }
	else if (exit)
    {
        return Error(StopExecution, 0, 0);
    }
   return NoError;
}

void Executive::TraceStatement(Statement * stmt)
{
	if (_trace && stmt->LineNumber() && _state == Running)
	{
		size_t fileIndex = stmt->FileIndex();
		if (fileIndex == 0)
	//		Print(stdout, "[", stmt->LineNumber(), "]");
	               VFPRINT(stdout, "[%i] ", buf, stmt->LineNumber());
      	else
		{
			--fileIndex;
			char buf[100];
			buf[0] = '[';
			strcpy(&buf[1], _includeFiles[fileIndex].c_str());
			int len = _includeFiles[fileIndex].length() + 1;
			buf[len++] = ':';
			buf[len] = '\0';
//			Print(stdout, buf, stmt->LineNumber(), "]");
                VFPRINT(stdout, "%s %i] ", buf, stmt->LineNumber());
		}
	}
}

void Executive::DebugMode(bool *exit)
{
	State_t prevState = _state;
	int *oldPC = _activeProgramCounter;
	_state = Debug;
	_notifications->NewState(_state);
	++_debugDepth;
	ImmediateMode(exit);
	--_debugDepth;
	_state = prevState;
	_notifications->NewState(_state);
	_activeProgramCounter = oldPC;
}

void Executive::Goto(Statement *stmt) 
{
	*_activeProgramCounter = stmt->StatementNumber();
}

void Executive::GotoFollowingStatement(Statement *stmt) 
{
	*_activeProgramCounter = stmt->StatementNumber() + 1;
}

Error Executive::Gosub(StatementProcedure *stmt, RunTimeEnvironment &runTime)
{
	int oldProgramCounter = *_activeProgramCounter;
	*_activeProgramCounter = stmt->StatementNumber() + 1;

	runTime.compileEnvironment.EnterVariableScope(stmt->GetVariableScope());

	Error error = Execute(_program, *_activeProgramCounter);

	runTime.compileEnvironment.ExitVariableScope();

	*_activeProgramCounter = oldProgramCounter;
	if (error.Message() == ReturnFromSubroutine)
		return NoError;
	else
		return error;
}

Error Executive::ReturnFromSub()
{
	// Not really an error, just a signal to the Gosub routine
	return Error(ReturnFromSubroutine, 0, 0);
}

Error Executive::BreakIntoDebugger()
{
	// Not really an error, just a signal to the Executive
	return Error(Breakpoint, 0, 0);
}

Error Executive::Chain(const String &fileName, bool rerun)
{
	// Open the file
	FILE *file = fopen(fileName.c_str(), "rt");
	// If we couldn't open it
	if (!file)
	{
		// Try adding a .bas extension
		String withExt(fileName, ".bas");
		file = fopen(withExt.c_str(), "rt");
	}
	if (file)
	{
		fclose(file);
		if (rerun && _state == Running)
			_chainStack.push_back(_currentProgram);
		_chainTo = fileName;

		return Error(Stop(), 0, 0);
	}
	else
		return Error(CantOpenFile, 0, 0);
}

void Executive::PrintMemoryUsage()
{
    _notifications->PrintMemoryUsage();
}

static int UIntCompare(const size_t &a, const size_t &b)
{
	return a - b;
}

Error Executive::BreakpointCommand(StatementBp::BreakpointCommand cmd, uint16_t lineNumber, uint16_t fileIndex)
{
	size_t i;

	if (_state == Running)
	{
		// This is a bit of a kludge. If the 'BP' command is in the user's program, it
		// should not have any arguments. So it will have been compiled as a "print breakpoints"
		// command. If it was compiled as something else, that means it had arguments
		if (cmd == StatementBp::PRINT)
			return BreakIntoDebugger();
		else
			return Error(IllegalArgument, 1, 1);
	}

	switch (cmd)
	{
	case StatementBp::PRINT :
		if (_breakpoints.size() == 0)
			FPUTS("No breakpoints set\r\n", stdout);
		else
		{
			_breakpoints.sort(UIntCompare);
			FPUTS("Breakpoints are set at lines:\r\n", stdout);
			for (i = 0; i < _breakpoints.size(); ++i)
			{
				uint16_t bp = (uint16_t) _breakpoints[i];
				uint16_t bpFileIndex = ExtractFileIndex(bp);
				bp = ExtractLineNumber(bp);
				if (bpFileIndex && bpFileIndex <= _includeFiles.size())
//					Print(stdout, ("   " + _includeFiles[bpFileIndex - 1] + "[").c_str(), int(bp), "]\r\n");
                VFPRINT(stdout, "   %s[%i]\r\n", _includeFiles[bpFileIndex - 1], int(bp));
				else
//					Print(stdout, "   ", int(_breakpoints[i]), "\r\n");
                VFPRINT(stdout, "   %i\r\n", int(_breakpoints[i]));
			}
		}
		break;

	case StatementBp::CLEARALL :
		_breakpoints.clear();
		FPUTS("Breakpoints cleared\r\n", stdout);
		break;

	case StatementBp::SET:
		// Check whether the breakpoint already exists
		for (i = 0; i < _breakpoints.size(); ++i)
		{
			if (_breakpoints[i] == EncodeLineNumber(fileIndex, lineNumber))
			{
				FPUTS("Breakpoint was already set at this line\r\n", stdout);
				return NoError;
			}
		}
		// Check that the specified line exists
		for (i = 0; i < _program.size(); ++i)
		{
			if (_program[i]->LineNumber() == lineNumber && _program[i]->FileIndex() == fileIndex)
			{
				_breakpoints.push_back(EncodeLineNumber(fileIndex, lineNumber));
				FPUTS("Breakpoint set\r\n", stdout);
				return NoError;
			}
		}
		FPUTS("Line number does not exist\r\n", stdout);
		break;

	case StatementBp::CLEAR:
		// Find the breakpoint
		for (i = 0; i < _breakpoints.size(); ++i)
		{
			if (_breakpoints[i] == EncodeLineNumber(fileIndex, lineNumber))
			{
				_breakpoints[i] = _breakpoints.back();
				_breakpoints.pop_back();
				FPUTS("Breakpoint cleared\r\n", stdout);
				return NoError;
			}
		}
		FPUTS("There was no breakpoint at this line\r\n", stdout);
		break;
	}

	return NoError;
}
