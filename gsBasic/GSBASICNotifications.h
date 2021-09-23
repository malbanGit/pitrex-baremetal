
#ifndef _GSBASICNOTIFICATIONS_H
#define _GSBASICNOTIFICATIONS_H

typedef enum { Immediate, Debug, Running } State_t;

#ifdef __cplusplus

class GSBASICNotifications
{
public:
	virtual ~GSBASICNotifications() {}

	// Register platform-specific BASIC constants
	virtual void RegisterConstants() {}
    
    virtual void DisplayGreeting() {}

	virtual void NewState(State_t) {}

	// The BASIC program's run-time variables are being cleared
	virtual void RuntimeReset() {}
    
    virtual void PrintMemoryUsage() {}
    
    virtual void FatalError(const char *) {}
};

#endif

#endif
