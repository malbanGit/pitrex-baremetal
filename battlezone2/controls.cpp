#include "controls.h"
#include "mathutils.h"

extern "C" 
{
  #include <vectrex/vectrexInterface.h>
}


#define AXIS_MAX 32768

const float CLAMPVALUE = .3;

controls::controls()
{
    printf("Initing controls\n");
/*
    // Look for an xbox controller to use
    mNumJoysticks = SDL_NumJoysticks();

    TCHAR s[256];
    printf("Found %d joysticks\n", mNumJoysticks);

    mJoystick1 = NULL;
    mJoystick2 = NULL;

    if (mNumJoysticks==1)
    {
        mJoystick1 = SDL_JoystickOpen(0);
        mJoystick2 = mJoystick1;
    }
    if (mNumJoysticks==2)
    {
        mJoystick1 = SDL_JoystickOpen(0);
        mJoystick2 = SDL_JoystickOpen(1);
    }
*/    
}

controls::~controls()
{
// Supposed to call these but it crashes if you do <_<
//    SDL_JoystickClose(mJoystick1);
//    SDL_JoystickClose(mJoystick2);
}

Point3d controls::getLeftStick(int player)
{
    return readKeyboardJoystickLeftStick(player) + readXBoxJoystickLeftStick(player);
}

Point3d controls::getRightStick(int player)
{
    return readKeyboardJoystickRightStick(player) + readXBoxJoystickRightStick(player);
}

bool controls::getTriggerButton(int player)
{
    return readKeyboardJoystickTrigger(player) || readXBoxJoystickTrigger(player);
}

bool controls::getCreditButton()
{
    return readKeyboardCredit() || readXBoxCredit();
}

bool controls::getStart1Button()
{
    return readKeyboardStart1() || readXBoxStart1();
}

bool controls::getStart2Button()
{
    return readKeyboardStart2() || readXBoxStart2();
}

// -------------------------------------------------
// Keyboard controller code

void controls::convertArrowKeysToSticks(Point3d& leftStick, Point3d& rightStick)
{
  /*
    bool up = (::GetAsyncKeyState(VK_UP) & 0x8000);
    bool down = (::GetAsyncKeyState(VK_DOWN) & 0x8000);
    bool left = (::GetAsyncKeyState(VK_LEFT) & 0x8000);
    bool right = (::GetAsyncKeyState(VK_RIGHT) & 0x8000);

    leftStick = Point3d(0, 0, 0);
    rightStick = Point3d(0, 0, 0);

    if (up && !down && !left && !right)
    {
        // straight forward
        leftStick.x = 1;
        rightStick.x = 1;
    }
    else if (!up && down && !left && !right)
    {
        // straight back
        leftStick.x = -1;
        rightStick.x = -1;
    }
    else if (!up && !down && left && !right)
    {
        // hard left
        leftStick.x = -1;
        rightStick.x = 1;
    }
    else if (!up && !down && !left && right)
    {
        // hard right
        leftStick.x = 1;
        rightStick.x = -1;
    }

    else if (up && !down && left && !right)
    {
        // left forward
        leftStick.x = 0;
        rightStick.x = 1;
    }
    else if (up && !down && !left && right)
    {
        // right forward
        leftStick.x = 1;
        rightStick.x = 0;
    }

    else if (!up && down && left && !right)
    {
        // left reverse
        leftStick.x = -1;
        rightStick.x = 0;
    }
    else if (!up && down && !left && right)
    {
        // right reverse
        leftStick.x = 0;
        rightStick.x = -1;
    }
*/
}

Point3d controls::readKeyboardJoystickLeftStick(int player)
{
    Point3d leftStick, rightStick;
/*    
    convertArrowKeysToSticks(leftStick, rightStick);

    bool up = (::GetAsyncKeyState('Q') & 0x8000);
    bool down = (::GetAsyncKeyState('A') & 0x8000);

    if (up) leftStick.x = 1;
    if (down) leftStick.x = -1;
*/
    leftStick.x = currentJoy2X;
    
    
    return leftStick;
}

Point3d controls::readKeyboardJoystickRightStick(int player)
{
    Point3d leftStick, rightStick;
/*  
    convertArrowKeysToSticks(leftStick, rightStick);

    bool up = (::GetAsyncKeyState('W') & 0x8000);
    bool down = (::GetAsyncKeyState('S') & 0x8000);

    if (up) rightStick.x = 1;
    if (down) rightStick.x = -1;
*/    
    rightStick.x = currentJoy1X;

    return rightStick;
}

bool controls::readKeyboardJoystickTrigger(int player)
{
//    return (::GetAsyncKeyState(VK_CONTROL) & 0x8000);
    return (currentButtonState&0x08)==0x08; // button 4
}

bool controls::readKeyboardCredit()
{
//    return (::GetAsyncKeyState('3') & 0x8000);
    return (currentButtonState&0x04)==0x04; // button 3
}

bool controls::readKeyboardStart1()
{
//    return (::GetAsyncKeyState('1') & 0x8000);
    return (currentButtonState&0x01)==0x01; // button 1
}

bool controls::readKeyboardStart2()
{
//    return (::GetAsyncKeyState('2') & 0x8000);
    return (currentButtonState&0x10)==0x10; // button 1 - joy 2
}

// -------------------------------------------------
// Xbox controller code

bool controls::readXBoxCredit()
{
/*  
    SDL_Event event;
    SDL_PollEvent(&event);

    return SDL_JoystickGetButton(mJoystick1, 0) || SDL_JoystickGetButton(mJoystick2, 0);
*/
    return false;
}

bool controls::readXBoxStart1()
{
/*  
    SDL_Event event;
    SDL_PollEvent(&event);

    return SDL_JoystickGetButton(mJoystick1, 1) || SDL_JoystickGetButton(mJoystick2, 1);
*/
    return false;
}

bool controls::readXBoxStart2()
{
/*  
    SDL_Event event;
    SDL_PollEvent(&event);

    return SDL_JoystickGetButton(mJoystick1, 2) || SDL_JoystickGetButton(mJoystick2, 2);
*/
    return false;
    
}

Point3d controls::readXBoxJoystickLeftStick(int player)
{
    Point3d vector;
/*  
    SDL_Event event;
    SDL_PollEvent(&event);

    vector.x = -(float)(SDL_JoystickGetAxis((player==0)?mJoystick1:mJoystick2, 1)) / AXIS_MAX;
    vector.y = -(float)(SDL_JoystickGetAxis((player==0)?mJoystick1:mJoystick2, 0)) / AXIS_MAX;
    vector.z = 0;

    if (fabs(vector.x) < CLAMPVALUE)
    {
        vector.x = 0;
    }
    if (fabs(vector.y) < CLAMPVALUE)
    {
        vector.y = 0;
    }
*/
    return vector;
}

Point3d controls::readXBoxJoystickRightStick(int player)
{
    Point3d vector;
/*    
    SDL_Event event;
    SDL_PollEvent(&event);

    vector.x = -(float)(SDL_JoystickGetAxis((player==0)?mJoystick1:mJoystick2, 3)) / AXIS_MAX;
    vector.y = -(float)(SDL_JoystickGetAxis((player==0)?mJoystick1:mJoystick2, 4)) / AXIS_MAX;
    vector.z = 0;

    if (fabs(vector.x) < CLAMPVALUE)
    {
        vector.x = 0;
    }
    if (fabs(vector.y) < CLAMPVALUE)
    {
        vector.y = 0;
    }
*/

    return vector;
}

bool controls::readXBoxJoystickTrigger(int player)
{
/*  
    SDL_Event event;
    SDL_PollEvent(&event);

    float triggerVal = (float)(SDL_JoystickGetAxis((player==0)?mJoystick1:mJoystick2, 2)) / AXIS_MAX;

    if (fabs(triggerVal) > CLAMPVALUE)
    {
        return true;
    }
*/
    return false;
}

