#ifndef CONTROLS_H
#define CONTROLS_H

#include "point3d.h"

//#include "SDL.h"

class controls
{
public:
    
    controls();
    ~controls();

    Point3d getLeftStick(int player);
    Point3d getRightStick(int player);
    bool getTriggerButton(int player);
    bool getCreditButton();
    bool getStart1Button();
    bool getStart2Button();

private:

    void convertArrowKeysToSticks(Point3d& leftStick, Point3d& rightStick);

    Point3d readKeyboardJoystickLeftStick(int player);
    Point3d readKeyboardJoystickRightStick(int player);
    bool readKeyboardJoystickTrigger(int player);
    bool readKeyboardCredit();
    bool readKeyboardStart1();
    bool readKeyboardStart2();

    Point3d readXBoxJoystickLeftStick(int player);
    Point3d readXBoxJoystickRightStick(int player);
    bool readXBoxJoystickTrigger(int player);
    bool readXBoxCredit();
    bool readXBoxStart1();
    bool readXBoxStart2();

//    SDL_Joystick* mJoystick1;
//    SDL_Joystick* mJoystick2;
//    void* mJoystick1;
//    void* mJoystick2;

    int mNumJoysticks;

};

#endif // CONTROLS_H

