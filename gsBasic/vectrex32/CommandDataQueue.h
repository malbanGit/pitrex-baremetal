/* 
 * File:   CommandDataQueue.h
 * Author: Bob
 *
 * Created on February 23, 2020, 10:37 AM
 */

#ifndef COMMANDDATAQUEUE_H
#define	COMMANDDATAQUEUE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus

class CommandDataQueue
{
public:
    static CommandDataQueue *Singleton(); 
    
    enum {CommandDataSize = 2000};
//    enum {CommandDataSize = drawing_command_data_end - drawing_command_data};
    enum {WaitForFrame = 1, Executable = 2, Ready = 4};
    typedef struct _CommandData
    {  
        void SetReady() {status |= Ready;}
        bool IsReady() const {return (status & Ready);}
        bool IsExecutable() const {return (status & Executable);}
        bool IsWaitForFrame() const {return (status & WaitForFrame);}
        bool IsEmpty() const {return size == 0;}
        
        uint8_t buf[CommandDataSize];
        uint16_t start, size;
        
    private:
        friend class CommandDataQueue;
        uint8_t status;
    } CommandData;
    
    // Returns the front element
    const CommandData &front() const {return _commandData[_front];}
    // Removes the front element
    void pop();
    
    // Adds a tail element. Returns its data buffer.
    uint8_t *push(bool waitForFrame, bool executable);
    CommandData &back() 
    {
//        __builtin_disable_interrupts();
        size_t back = _front + _count - 1;
//        __builtin_enable_interrupts();
        if (back >= COMMAND_DATA_QUEUE_SIZE)
            back -= COMMAND_DATA_QUEUE_SIZE;
        return _commandData[back];
    }
    
    bool isEmpty() const {return (_count == 0);}
    bool isFull() const {return (_count == COMMAND_DATA_QUEUE_SIZE);}
   
private:
    CommandDataQueue();

    enum {COMMAND_DATA_QUEUE_SIZE = 5};
    CommandData _commandData[COMMAND_DATA_QUEUE_SIZE];
    volatile size_t _front, _count;
};

#endif
#endif	/* COMMANDDATAQUEUE_H */

