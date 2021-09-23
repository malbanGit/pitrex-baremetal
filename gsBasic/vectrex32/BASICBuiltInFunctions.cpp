//TODO:
// Samples AYSFX and YM files are played in "vectrex speed"
// other sounds are playing in BASIC speed


/* 
 * File:   BASICBuiltInFunctions.cpp
 * Author: Bob
 * 
 * Created on November 15, 2015, 4:23 PM
 */

#include "../GSList.h"
#include <algorithm>
#include <fcntl.h>
extern "C" int close(int);
//#include "VectrexInterface.h"
//#include "ServiceVectrex.h"
#include "VectrexCommand.h"
#include "VectrexSound.h"

//#include <exception>
//#include "nvm.h"
//#include "peripheral/nvm/plib_nvm.h"

//extern "C" bool CONSOLE_CharWaiting();

#include "../BuiltInFunctions.h"
#include "../Value.h"
#include "../Array.h"
#include "../Environment.h"
#include "Version.h"
#include "../Utilities.h"
//#include "CustomDirent.h"
#include "Camera.h"
#include "Calculations.h"
#include "VectorFont.h"

using namespace Calculations;

int disableSprites=0;
//extern Value music;
//extern bool musicChanged;

// dummy values, these belong in ServcieVectrex
Value peekArray;
int peekAddress;
int peekLength;
bool peekRequest;

// TODO, this really is part of the console app!
//extern bool breakOnControlC;
bool breakOnControlC;

Value music;
VYMMusic *lastYM = 0;

bool musicChanged=false;


ErrorCode VectrexNotResponding = "E500: Vectrex not responding";
ErrorCode MissedRefresh = "E501: Too much processing to keep up with refresh rate";
// Upgrade failure messages are all caps so they can be displayed on the Vectrex
ErrorCode NoUpgradeFile = "E502: NO UPGRADE FILE FOUND";
ErrorCode UnableToOpenUpgradeFile = "E503: UNABLE TO OPEN UPGRADE FILE";
ErrorCode UpgradeFailed = "E504: UPGRADE FAILED";
extern ErrorCode OutOfDPRAM;

extern int b_currentX; // VectrexCommand.c
extern int b_currentY;
extern int b_currentScale;

int vectrexRefreshRate = 50; // servicevectrex.c
int joystickType = 0, joystickControllers = 0, joystickDirections = 0; // ServiceVectrexBody.cpp
int _compiledSpriteCount=0;
int *compiledSpriteCount=&_compiledSpriteCount;

uint32_t MAJOR_VERSION = 1;
uint32_t MINOR_VERSION = 27;


static bool ValidMemoryAddress(const unsigned char *p);

static void AddCommand(VectrexCommand *obj)
{
    commands.push_back(Value(obj));
}

static list<Value>::iterator FindCommand(VectrexCommand *obj)
{
    list<Value>::iterator i;
    for (i = commands.begin(); i != commands.end(); ++i)
    {
        if (i->Handle() == obj)
            break;
    }
    return i;
}

static void RemoveCommand(VectrexCommand *obj, bool lockDrawingList)
{
    auto i = FindCommand(obj);
    if (i != commands.end())
    {
        commands.erase(i);
    }
}

extern int clearPipelineIssued; // vectrexInterface.c
static void _ClearScreen()
{
  commands.clear();
  clearPipelineIssued = 1;
}

static ErrorCode ClearScreen(Executive &, RunTimeEnvironment &, int)
{
    _ClearScreen();
    return 0;
}
// Return the version multiplied by 100, so a version of 1.02 is returned as 102
static ErrorCode Version(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value version = int(MAJOR_VERSION * 100 + MINOR_VERSION);
    SetReturnValue(runTimeEnvironment, version);
    return 0;
}

extern "C"
void ClearVectrex()
{
    _ClearScreen();
    camera.Reset();
    vectrexRefreshRate = 30;
    v_setClientHz(vectrexRefreshRate);
    v_noSound();
    if (music.IsHandle() && dynamic_cast<VectrexMusicPlayer *>(music.Handle()) != null)
    {
      VectrexMusicPlayer *mplayer = dynamic_cast<VectrexMusicPlayer *>(music.Handle());
      mplayer->setPlaying(false);
    }
    if (lastYM != 0)
    {
      v_stopYM();
      lastYM = 0;
    }

    music.Clear();
	musicChanged = true;
}


// Return the screen frame rate (refreshes per second)
// rate = GetFrameRate()
static ErrorCode GetFrameRate(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value rate(vectrexRefreshRate);
    SetReturnValue(runTimeEnvironment, rate);
    return 0;
}
// call SetFrameRate(framesPerSecond)
static ErrorCode SetFrameRate(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vrate = runTimeEnvironment.calculationStack.back();
    if (!vrate.IsNumeric())
        return IllegalArgument;
    
    int rate;
    vrate.TypeBehaviors()->ToInteger(vrate, &rate);
    
    vectrexRefreshRate = std::min(150, rate);
//v_setRefresh(std::min(150, rate));
    v_setClientHz(vectrexRefreshRate);
    SetReturnValue(runTimeEnvironment, Value(0));
    
    return 0;
}

// cmd = IntensitySprite(intensity) or
static ErrorCode IntensitySprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vintensity = runTimeEnvironment.calculationStack.back();
    if (!vintensity.IsNumeric())
        return IllegalArgument;
    
    int intensity;
    vintensity.TypeBehaviors()->ToInteger(vintensity, &intensity);
    
    VectrexCommand *obj = new VectrexCommandIntensity(intensity);
    AddCommand(obj);
    SetReturnValue(runTimeEnvironment, Value(commands.back()));

    return 0;
}

static ErrorCode DistancePointToLine2D(RunTimeEnvironment &rte, const Value &arg1, const Value &arg2)
{
    float arg1Values[2], arg2Values[2][2];
    ErrorCode error = arg1.GetArrayValues(arg1Values);
    if (error)
        return error;
    error = arg2.GetArrayValues((float *) arg2Values);
    if (error)
        return error;
    
    // If the line that the user passed in is actually a point, calculate the point-point distance
    if (arg2Values[0][0] == arg2Values[1][0] && arg2Values[0][1] == arg2Values[1][1])
    {
        float deltaX = arg1Values[0] - arg2Values[0][0], deltaY = arg1Values[1] - arg2Values[0][1];
        float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
        SetReturnValue(rte, distance);
        return 0;
    }
    
    /*
    
     find the distance from the point (arg1[0],arg1[1]) to the line
     determined arg2[1][1] the points (arg2[0][0],arg2[0][1]) and (arg2[1][0],arg2[1][1])
    
     distanceSegment = distance from the point to the line segment
     distanceLine = distance from the point to the line (assuming
                                            infinite extent in both directions
    
    Let the point be C (Cx,Cy) and the line be AB (Ax,Ay) to (Bx,By).
    Let P be the point of perpendicular projection of C on AB.  The parameter
    r, which indicates P's position along AB, is computed arg2[1][1] the dot product 
    of AC and AB divided arg2[1][1] the square of the length of AB:

    (1)    AC dot AB
        r = ---------  
            ||AB||^2

    r has the following meaning:

        r=0      P = A
        r=1      P = B
        r<0      P is on the backward extension of AB
        r>1      P is on the forward extension of AB
        0<r<1    P is interior to AB

    The length of a line segment in d dimensions, AB is computed arg2[1][1]:

        L = sqrt( (Bx-Ax)^2 + (By-Ay)^2 + ... + (Bd-Ad)^2)

    so in 2D:  

        L = sqrt( (Bx-Ax)^2 + (By-Ay)^2 )

    and the dot product of two vectors in d dimensions, U dot V is computed:

        D = (Ux * Vx) + (Uy * Vy) + ... + (Ud * Vd)

    so in 2D:  

        D = (Ux * Vx) + (Uy * Vy) 

    So (1) expands to:

            (Cx-Ax)(Bx-Ax) + (Cy-Ay)(By-Ay)
        r = -------------------------------
                          L^2

    The point P can then be found:

        Px = Ax + r(Bx-Ax)
        Py = Ay + r(By-Ay)

    And the distance from A to P = r*L.

    Use another parameter s to indicate the location along PC, with the 
    following meaning:
          s<0      C is left of AB
          s>0      C is right of AB
          s=0      C is on AB

    Compute s as follows:

            (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
        s = -----------------------------
                        L^2


    Then the distance from C to P = |s|*L.

    */

    
    double r_numerator = (arg1Values[0] - arg2Values[0][0]) * (arg2Values[1][0] - arg2Values[0][0]) + 
        (arg1Values[1] - arg2Values[0][1]) * (arg2Values[1][1] - arg2Values[0][1]);
    double r_denominator = (arg2Values[1][0] - arg2Values[0][0]) * (arg2Values[1][0] - arg2Values[0][0]) + 
        (arg2Values[1][1] - arg2Values[0][1]) * (arg2Values[1][1] - arg2Values[0][1]);
    double r = r_numerator / r_denominator;

    //double px = arg2Values[0][0] + r * (arg2Values[1][0] - arg2Values[0][0]);
    //double py = arg2Values[0][1] + r * (arg2Values[1][1] - arg2Values[0][1]);

    double s =  ((arg2Values[0][1] - arg1Values[1]) * (arg2Values[1][0] - arg2Values[0][0]) -
        (arg2Values[0][0] - arg1Values[0]) * (arg2Values[1][1] - arg2Values[0][1]) ) / r_denominator;

    float distanceLine = float(fabs(s) * sqrtf((float) r_denominator)), distanceSegment;

    //
    // (xx,yy) is the point on the lineSegment closest to (arg1[0],arg1[1])
    //
    //double xx;
    //double yy;

    if ( (r >= 0) && (r <= 1) )
    {
        distanceSegment = distanceLine;
        //xx = px;
        //yy = py;
    }
    else
    {

        double dist1 = (arg1Values[0]-arg2Values[0][0])*(arg1Values[0]-arg2Values[0][0]) + 
            (arg1Values[1]-arg2Values[0][1])*(arg1Values[1]-arg2Values[0][1]);
        double dist2 = (arg1Values[0]-arg2Values[1][0]) * (arg1Values[0]-arg2Values[1][0]) + 
            (arg1Values[1]-arg2Values[1][1]) * (arg1Values[1]-arg2Values[1][1]);
        if (dist1 < dist2)
        {
            //xx = arg2Values[0][0];
            //yy = arg2Values[0][1];
            distanceSegment = sqrtf((float) dist1);
        }
        else
        {
            //xx = arg2Values[1][0];
            //yy = arg2Values[1][1];
            distanceSegment = sqrtf((float) dist2);
        }
    }

    SetReturnValue(rte, distanceSegment);
    return 0;
}

static ErrorCode DistancePointToLine3D(RunTimeEnvironment &rte, const Value &arg1, const Value &arg2)
{
    float arg1Values[3], arg2Values[2][3];
    ErrorCode error = arg1.GetArrayValues(arg1Values);
    if (error)
        return error;
    error = arg2.GetArrayValues((float *) arg2Values);
    if (error)
        return error;
    
    // If the line that the user passed in is actually a point, calculate the point-point distance
    if (arg2Values[0][0] == arg2Values[1][0] && arg2Values[0][1] == arg2Values[1][1] && arg2Values[0][2] == arg2Values[1][2])
    {
        float deltaX = arg1Values[0] - arg2Values[0][0];
        float deltaY = arg1Values[1] - arg2Values[0][1];
        float deltaZ = arg1Values[2] - arg2Values[0][2];
        float distance = sqrtf(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
        SetReturnValue(rte, distance);
        return 0;
    }
    
    /*
    
     find the distance from the point (arg1[0], arg1[1], arg1[2]) to the line
     determined by the points (arg2[0][0], arg2[0][1], arg2[0][2]) and (arg2[1][0],arg2[1][1], arg2[1][2])
     */
    Calculations::Point p = {arg1Values[0], arg1Values[1], arg1Values[2]};
    Segment s = {
        {arg2Values[0][0], arg2Values[0][1], arg2Values[0][2]}, 
        {arg2Values[1][0],arg2Values[1][1], arg2Values[1][2]}};
    float distance = dist_Point_to_Segment(p, s);
    SetReturnValue(rte, distance);
    return 0;
}

static ErrorCode DistancePointToPoint2D(RunTimeEnvironment &rte, const Value &arg1, const Value &arg2)
{
    float arg1Values[2], arg2Values[2];
    ErrorCode error = arg1.GetArrayValues(arg1Values);
    if (error)
        return error;
    error = arg2.GetArrayValues(arg2Values);
    if (error)
        return error;
    float deltaX = arg1Values[0] - arg2Values[0], deltaY = arg1Values[1] - arg2Values[1];
    float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);
    SetReturnValue(rte, distance);
    return 0;
}

static ErrorCode DistancePointToPoint3D(RunTimeEnvironment &rte, const Value &arg1, const Value &arg2)
{
    float arg1Values[3], arg2Values[3];
    ErrorCode error = arg1.GetArrayValues(arg1Values);
    if (error)
        return error;
    error = arg2.GetArrayValues(arg2Values);
    if (error)
        return error;
    float deltaX = arg1Values[0] - arg2Values[0];
    float deltaY = arg1Values[1] - arg2Values[1];
    float deltaZ = arg1Values[2] - arg2Values[2];
    float distance = sqrtf(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
    SetReturnValue(rte, distance);
    return 0;
}

// distance = Distance(point, point) - each point is an array of {x, y} or (x, y, z)
// distance = Distance(point, line segment) - line is an array {{x1, y1}, {x2, y2}} or {{x1, y1, z2}, {x2, y2, z2}}
// distance = Distance(line segment, point)
static ErrorCode Distance(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &arg1 = runTimeEnvironment.calculationStack.back(); 
    Value &arg2 = runTimeEnvironment.calculationStack.back(1);
    if (!arg1.IsArray() || !arg2.IsArray())
        return IllegalArgument;
    // If one is 2D and the other is 3D, it's an error
    int arg1Dimension = arg1.Dimensions()[arg1.DimensionsSize() - 1];
    if (arg1Dimension != arg2.Dimensions()[arg2.DimensionsSize() - 1])
        return IllegalArgument;
    // If they're a dimension other than 2 or 3, it's an error
    if (arg1Dimension < 2 || arg1Dimension > 3)
        return IllegalArgument;
    // Make sure we've identified the argument that is a point. The other argument
    // can be a point or a line
    Value *point, *other;
    if (arg1.DimensionsSize() == 1)
    {
        point = &arg1;
        other = &arg2;
    }
    else if (arg2.DimensionsSize() == 1)
    {
        point = &arg2;
        other = &arg1;
    }
    // Else (neither argument is a point)
    else
        return IllegalArgument;
    // If the other argument is a point
    if (other->DimensionsSize() == 1)
    {
        if (arg1Dimension == 2)
            return DistancePointToPoint2D(runTimeEnvironment, *point, *other);
        else
            return DistancePointToPoint3D(runTimeEnvironment, *point, *other);
    }
    // Else if the other argument is a line
    else if (other->DimensionsSize() == 2 && other->Dimensions()[0] == 2)
    {
        if (arg1Dimension == 2)
            return DistancePointToLine2D(runTimeEnvironment, *point, *other);
        else
            return DistancePointToLine3D(runTimeEnvironment, *point, *other);
    }
    else
        return IllegalArgument;
}
// Reset the vector integrators, i.e. resets the "cursor" to the center of the screen
static ErrorCode ReturnToOriginSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    VectrexCommand *cmd = new VectrexCommandReset0Ref();
    AddCommand(cmd);
    SetReturnValue(runTimeEnvironment, Value(commands.back()));
    return 0;
}


// sprite = TextListSprite(<list>)
// <list> is an array where each row is {x, y, value}. The value can be a string
// or a number. WARNING: a y coordinate of zero for any string except the first,
// will terminate the text list
// x and y coordinates are absolute in scale 0x7f
static ErrorCode TextListSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &arg = runTimeEnvironment.calculationStack.back();
    if (!arg.IsArray())
        return IllegalArgument;
    const uint16_t *dims = arg.Dimensions();
    if (arg.DimensionsSize() != 2 || dims[1] < 3)
        return IllegalArgument;
    VectrexCommand *obj = new VectrexCommandPrintList(arg);
    AddCommand(obj);
    SetReturnValue(runTimeEnvironment, Value(commands.back()));
    return 0;
}

// sprite = TextSizeSprite(<array>)
// <array> is an array with two elements, the X size and the Y size of text
static ErrorCode TextSizeSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &arg = runTimeEnvironment.calculationStack.back();
    if (!arg.IsArray())
        return IllegalArgument;
    const uint16_t *dims = arg.Dimensions();
    if (arg.DimensionsSize() != 1 || dims[0] != 2)
        return IllegalArgument;
    VectrexCommand *obj = new VectrexCommandTextSize(arg);
    AddCommand(obj);
    SetReturnValue(runTimeEnvironment, Value(commands.back()));
    return 0;
}


// controllerInfo = WaitForFrame(joyType, controllers, directions)
// controllerInfo = WaitForFrame(joyType, controllers, directions, joystickResolution)
// Waits until the screen has been refreshed. Returns controller information in an array.
// The array is 2x6. Row 1 has the inputs for controller 1, row 2 has the inputs for
// controller 2. In each row, the six elements are: Joystick X, Joystick Y, Button 1
// (1 if it's pressed, 0 if it isn't), Button 2, Button 3, and Button 4
// joyType is:
//    0 - not used
//    1 - digital
//    2 - analog
// controllers is:
//    0 - none
//    1 - controller 1
//    2 - controller 2
//    3 - controllers 1 and 2
// directions is:
//    0 - none
//    1 - X
//    2 - Y
//    3 - X and Y
// joystickResolution is 0, 1, 2, 4, 8, 16, 32, 64, or 128. 0 is highest
//    resolution, 128 is lowest
//#include <typeinfo>
static ErrorCode WaitForFrame(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
  ErrorCode error = 0;
    bool success = true;
    // Get the arguments passed in
    Value vresolution;
    if (count == 4)
    {
        vresolution = runTimeEnvironment.calculationStack.back();
        runTimeEnvironment.calculationStack.pop_back();
    }
    else
        vresolution = Value(VectrexInterface::MediumHigh);
    Value &vdirections = runTimeEnvironment.calculationStack.back();
    Value &vcontrollers = runTimeEnvironment.calculationStack.back(1);
    Value &vtype = runTimeEnvironment.calculationStack.back(2);
    if (!vtype.IsInteger() || !vcontrollers.IsInteger() || 
        !vdirections.IsInteger() || !vresolution.IsInteger())
        return IllegalArgument;
    
    // Set global variables specifying the joystick reading specs
    // joystickType, joystickControllers, joystickDirections, and joystickResolution
    // are used when compiling the code for th4e 6809.
    // So block any context switches while we're setting the globals
//    LockDrawingList();
    joystickType = vtype.Integer();
    joystickControllers = vcontrollers.Integer();

    // If the game supports 3D glasses and we haven't detected them yet,
    // check the signals coming on the 2nd joystick port
    //if (camera.Is3D() && !camera.GlassesDetected())
    //    joystickControllers |= 2;

    joystickDirections = vdirections.Integer();
/*    
    int res = vresolution.Integer();
    if (res >= 64)
        res = 64;
    else if (res >= 32)
        res = 32;
    else if (res >= 16)
        res = 16;
    else if (res >= 8)
        res = 8;
    else if (res >= 4)
        res = 4;
    else if (res >= 2)
        res = 2;
    else if (res < 0)
        res = 0;
    joystickResolution = VectrexInterface::JoystickResolution(res);
    VectrexCommand::ClearError();
    // We're done changing the global variables
    UnlockDrawingList();
*/    
    
    if (joystickControllers == 1)
    {
      if (joystickType & 2)
      {
        v_readJoystick1Analog(); // not neededin IRQ mode
        v_enableJoystickAnalog(1,1,0,0);
      }
      else  
      {
        v_readJoystick1Digital(); // not neededin IRQ mode
        v_enableJoystickDigital(1,1,0,0);
      }
      
    }
    else if (joystickControllers == 3)
    {
      if (joystickType & 2)
      {
        v_readJoystick1Analog(); // not neededin IRQ mode
        v_enableJoystickAnalog(1,1,1,1);
      }
      else  
      {
        v_readJoystick1Digital(); // not neededin IRQ mode
        v_enableJoystickDigital(1,1,1,1);
      }
      
    }
    else if (joystickControllers == 2)
    {
      if (joystickType & 2)
      {
        v_readJoystick1Analog(); // not neededin IRQ mode
        v_enableJoystickAnalog(0,0,1,1);
      }
      else  
      {
        v_readJoystick1Digital(); // not neededin IRQ mode
        v_enableJoystickDigital(0,0,1,1);
      }
    }

    //musicChanged
    // But it's otherwise invalid
    
    if (music.IsHandle() && dynamic_cast<VectrexMusicPlayer *>(music.Handle()) != null)
    {
      VectrexMusicPlayer *mplayer = dynamic_cast<VectrexMusicPlayer *>(music.Handle());
      mplayer->play();
    }
    

    v_doSound();    // not needed in IRQ Mode
    v_readButtons(); // not neededin IRQ mode
    v_WaitRecal();

    if (lastYM != 0)
    {
      if (!v_isYMPlaying() ) lastYM = 0;
    }

    b_currentX = 0;
    b_currentY = 0;
    b_currentScale = 0xff;

    
    static uint16_t dims[] = {2, 6};
    Value va(dims, 2);
  
/*    
    // Read the returned controller values
    VectrexInterface &vi = VectrexInterface::Singleton();
    int row = 0;
    for (uint8_t controller = 1; controller <= 2; ++controller, row += 6)
    {
        if (joystickControllers & (1 << (controller - 1)))
        {
            if (joystickDirections & 1)
                va[row] = vi.JoyX(controller);
            if (joystickDirections & 2)
                va[row + 1] = vi.JoyY(controller);
        }
        for (int button = 1; button <= 4; ++button)
        {
            va[row + button + 1] = vi.ButtonData(controller, button);
        }
    }
*/
    
    va[0] = currentJoy1X;
    va[0+1] = currentJoy1Y;
    va[6] = currentJoy2X;
    va[6+1] = currentJoy2Y;

    
    va[0 + 1 + 1] = (currentButtonState&1)?1:0;
    va[0 + 2 + 1] = (currentButtonState&2)?1:0;
    va[0 + 3 + 1] = (currentButtonState&4)?1:0;
    va[0 + 4 + 1] = (currentButtonState&8)?1:0;
    va[6 + 1 + 1] = (currentButtonState&16)?1:0;
    va[6 + 2 + 1] = (currentButtonState&32)?1:0;
    va[6 + 3 + 1] = (currentButtonState&64)?1:0;
    va[6 + 4 + 1] = (currentButtonState&128)?1:0;
    

    
    // If the game supports 3D glasses and we haven't detected the glasses yet
    /*if (camera.Is3D() && !camera.GlassesDetected())
    {
        // Look for them by checking whether button 4 is pressed
        if (vi.ButtonData(2, 4))
            camera.SetGlassesDetected(true);
    }*/
   
    
    /****************************************************************************/
    // Compile the drawing list
    if (!disableSprites)
    {
        VectrexCommand::ClearError();
        *compiledSpriteCount = 0;
        auto i = commands.begin();
        while (i != commands.end() && success)
        {
            // Find the next enabled command
            auto nextI = i;
            int skipCount = 0;
            VectrexCommand *nextCommand;
            do
            {
                ++nextI;
                ++skipCount;
                if (nextI == commands.end())
                    nextCommand = null;
                else
                    nextCommand = (VectrexCommand *) nextI->Handle();
            } while (nextCommand && nextCommand->Enabled() == false);

            bool eliminateTrailingMove;
            if (nextCommand == null || dynamic_cast<VectrexCommandReset0Ref *>(nextCommand) != null)
                eliminateTrailingMove = true;
            else
                eliminateTrailingMove = false;


            VectrexCommand *vc = (VectrexCommand *) i->Handle();
//            printf("calling: %s->%s\n\r", typeid(*vc).name() , vc->GetUserData().ToString().c_str());
            vc->Compile(eliminateTrailingMove);
//            printf("\n\r");
            error = VectrexCommand::GetError();
/*            
if (error != 0)
{
  printf("WaitFrame ERROR(command: %s): %s \n\r", typeid(*vc).name() ,error);
  vc->Dump();
}
*/
            // Save the current point in the executable code
            if (error == 0)
            {
                ++*compiledSpriteCount;
                i = nextI;
                *compiledSpriteCount += skipCount;
            }
            else
                success = false;
        }
        if (!success)
            error = VectrexCommand::GetError();
    }
    /****************************************************************************/
    
    SetReturnValue(runTimeEnvironment, va);
    return error;
}

// sprite = LinesSprite(<array>)
// The array is two dimensional, with the second dimension having a size of 2 or 3
// If the size is 2, they are (x, y) coordinates. If it's 3, they are (mode, x, y)
// The mode is 0 for "move to", non-zero for "draw to"
static ErrorCode LinesSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &arg = runTimeEnvironment.calculationStack.back();
    if (!arg.IsArray())
        return IllegalArgument;
    if (arg.DimensionsSize() != 2 || (arg.Dimensions()[1] != 2 && arg.Dimensions()[1] != 3))
        return IllegalArgument;
    VectrexCommand *obj = new VectrexCommandLines(arg);
    AddCommand(obj);
    SetReturnValue(runTimeEnvironment, Value(commands.back()));
    return 0;
}


// Creates a sprite that moves the position on the screen
// relative to the current position.
// x and y can be larger than 127 and -128. If they are, multiple Moveto calls
// are sent to the Vectrex
// pseudosprite = MoveSprite(x, y)
static ErrorCode MoveSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vy = runTimeEnvironment.calculationStack.back();
    Value &vx = runTimeEnvironment.calculationStack.back(1);
    if (!vx.IsNumeric() || !vy.IsNumeric())
        return IllegalArgument;
    
    int x, y;
    vx.TypeBehaviors()->ToInteger(vx, &x);
    vy.TypeBehaviors()->ToInteger(vy, &y);
    
    VectrexCommandMove *obj = new VectrexCommandMove(x, y);
    AddCommand(obj);
    SetReturnValue(runTimeEnvironment, Value(commands.back()));
    return 0;
}

// sprite = ScaleSprite(scaleValue) or
// sprite = ScaleSprite(scaleValue, worldToUnitsRatio)
static ErrorCode ScaleSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
    Value vratio;
    if (count == 2)
    {
        vratio = runTimeEnvironment.calculationStack.back();
        runTimeEnvironment.calculationStack.pop_back();
    }

    Value &vscale = runTimeEnvironment.calculationStack.back();
     if (!vscale.IsNumeric() || (count == 2 && !vratio.IsNumeric()))
        return IllegalArgument;
    
    int scale;
    vscale.TypeBehaviors()->ToInteger(vscale, &scale);
    float ratio = 0;
    if (!vratio.IsUninitialized())
        vratio.TypeBehaviors()->ToFloat(vratio, &ratio);
    
    VectrexCommand *obj = new VectrexCommandScale(scale, ratio);
    AddCommand(obj);
    SetReturnValue(runTimeEnvironment, Value(commands.back()));

    return 0;
}

// call SpriteClip(sprite, rect)
// call SpriteClip(sprite, nil}
// rect is {{left, top}, {right, bottom}}
static ErrorCode SpriteClip(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &varg = runTimeEnvironment.calculationStack.back();
    Value &vsprite = runTimeEnvironment.calculationStack.back(1);
    if (!vsprite.IsHandle() || (!varg.IsArray() && !varg.IsUninitialized()))
        return IllegalArgument;
    VectrexCommandTransformable *sprite = dynamic_cast<VectrexCommandTransformable *>(vsprite.Handle());
    if (!sprite)
        return IllegalArgument;
    if (varg.IsArray() && 
        (varg.DimensionsSize() != 2 || varg.Dimensions()[0] != 2 || varg.Dimensions()[1] != 2))
        return IllegalArgument;
    sprite->SetClippingRect(varg);
    return 0;
}

// call SpriteEnable(sprite, {true | false})
static ErrorCode SpriteEnable(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vvisibility = runTimeEnvironment.calculationStack.back();
    Value &vsprite = runTimeEnvironment.calculationStack.back(1);
    if (!vsprite.IsHandle() || !vvisibility.IsInteger())
        return IllegalArgument;
    VectrexCommand *sprite = dynamic_cast<VectrexCommand *>(vsprite.Handle());
    if (sprite)
    {
        int visibility;
        vvisibility.TypeBehaviors()->ToInteger(vvisibility, &visibility);
        sprite->Enable(visibility != 0);
        return 0;
    }
    else
        return IllegalArgument;
}

// mag = SpriteGetMagnification(sprite)
static ErrorCode SpriteGetMagnification(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vsprite = runTimeEnvironment.calculationStack.back();
    if (!vsprite.IsHandle())
        return IllegalArgument;
    VectrexCommandTransformable *sprite = dynamic_cast<VectrexCommandTransformable *>(vsprite.Handle());
    if (sprite)
    {
        float x, y, z;
        sprite->GetMagnification(&x, &y, &z);
        // If the x, y, and z magnifications are the same
        if (x == y && x == z)
        {
            // Return a single value
            Value mag(x);
            SetReturnValue(runTimeEnvironment, mag);
        }
        // Else if this is a 3D sprite
        else if (dynamic_cast<VectrexCommandTransformable3D *>(sprite))
        {
            uint16_t dims[1] = {3};
            Value mags(dims, 1);
            mags[0] = x;
            mags[1] = y;
            mags[2] = z;
            SetReturnValue(runTimeEnvironment, mags);
        }
        // Else (it's a 2D sprite)
        else
        {
            uint16_t dims[1] = {2};
            Value mags(dims, 1);
            mags[0] = x;
            mags[1] = y;
            SetReturnValue(runTimeEnvironment, mags);
        }
        return 0;
    }
    else
    {
        return IllegalArgument;
    }
}

// angle = SpriteGetRotation(sprite2D)
// angles = SpriteGetRotation(sprite3D) - an array with pitch, roll, yaw
static ErrorCode SpriteGetRotation(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vsprite = runTimeEnvironment.calculationStack.back();
    if (!vsprite.IsHandle())
        return IllegalArgument;
    VectrexCommandTransformable2D *sprite = dynamic_cast<VectrexCommandTransformable2D *>(vsprite.Handle());
    if (sprite)
    {
        Value angle(sprite->GetRotation());
        SetReturnValue(runTimeEnvironment, angle);
        return 0;
    }
    else
    {
        VectrexCommandTransformable3D *sprite3d = dynamic_cast<VectrexCommandTransformable3D *>(vsprite.Handle());
        if (sprite3d)
        {
            uint16_t dimensions[1] = {3};
            Value angles(dimensions, 1);
            angles[0] = sprite3d->GetRotation()[0];
            angles[1] = sprite3d->GetRotation()[1];
            angles[2] = sprite3d->GetRotation()[2];
            SetReturnValue(runTimeEnvironment, angles);
            return 0;
        }
        else
            return IllegalArgument;
    }
}

// this changes for an already "queued" scale command the scale, without recompiling it...
// call SpriteSetScale(scaleCmd, newScaleValue) or
// call SpriteSetScale(scaleCmd, newScaleValue, newRatio)
static ErrorCode SpriteScale(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
    Value vratio;
    if (count == 3)
    {
        vratio = runTimeEnvironment.calculationStack.back();
        runTimeEnvironment.calculationStack.pop_back();
    }
    Value &vscale = runTimeEnvironment.calculationStack.back();
    Value &vcmd = runTimeEnvironment.calculationStack.back(1);
    if (!vscale.IsNumeric() || (count == 3 && !vratio.IsNumeric()))
        return IllegalArgument;
    
    int scale;
    vscale.TypeBehaviors()->ToInteger(vscale, &scale);
    float ratio = 0;
    if (!vratio.IsUninitialized())
        vratio.TypeBehaviors()->ToFloat(vratio, &ratio);
    
    if (!vcmd.IsHandle())
        return IllegalArgument;
    VectrexCommandScale *cmd = dynamic_cast<VectrexCommandScale *>(vcmd.Handle());
    if (cmd)
    {
        cmd->Scale(scale, ratio);
    }
    else
        return IllegalArgument;
    return 0;
}


static ErrorCode SpriteSetRotation2D(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vangle = runTimeEnvironment.calculationStack.back();
    Value &vsprite = runTimeEnvironment.calculationStack.back(1);
    if (!vsprite.IsHandle() || !vangle.IsNumeric())
        return IllegalArgument;
    VectrexCommandTransformable2D *sprite = dynamic_cast<VectrexCommandTransformable2D *>(vsprite.Handle());
    if (sprite)
    {
        float angle;
        vangle.TypeBehaviors()->ToFloat(vangle, &angle);
        sprite->SetRotation(angle);
        return 0;
    }
    else
        return IllegalArgument;
}

static ErrorCode SpriteSetRotation3D(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vyaw = runTimeEnvironment.calculationStack.back();
    Value &vroll = runTimeEnvironment.calculationStack.back(1);
    Value &vpitch = runTimeEnvironment.calculationStack.back(2);
    Value &vsprite = runTimeEnvironment.calculationStack.back(3);
    if (!vsprite.IsHandle() || !vpitch.IsNumeric() || !vroll.IsNumeric() || !vyaw.IsNumeric())
        return IllegalArgument;
    VectrexCommandTransformable3D *sprite = dynamic_cast<VectrexCommandTransformable3D *>(vsprite.Handle());
    if (sprite)
    {
        float pitch, roll, yaw;
        vpitch.TypeBehaviors()->ToFloat(vpitch, &pitch);
        vroll.TypeBehaviors()->ToFloat(vroll, &roll);
        vyaw.TypeBehaviors()->ToFloat(vyaw, &yaw);
        sprite->SetRotation(pitch, roll, yaw);
        return 0;
    }
    else
        return IllegalArgument;
}

// Sets the absolute angle of rotation, in degrees
// call SpriteSetRotation(sprite, angle)
// call SpriteSetRotation(sprite, pitch, yaw, roll)
static ErrorCode SpriteSetRotation(Executive &exec, RunTimeEnvironment &runTimeEnvironment, int count)
{
    if (count == 2)
        return SpriteSetRotation2D(exec, runTimeEnvironment, count);
    else if (count == 4)
        return SpriteSetRotation3D(exec, runTimeEnvironment, count);
    else
        return WrongNumberOfArguments;
}


// call SpriteTranslate(sprite, {x, y})
static ErrorCode SpriteTranslate(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &varg = runTimeEnvironment.calculationStack.back();
    Value &vsprite = runTimeEnvironment.calculationStack.back(1);
    if (!vsprite.IsHandle() || !varg.IsArray())
        return IllegalArgument;
    VectrexCommandTransformable2D *sprite2D = dynamic_cast<VectrexCommandTransformable2D *>(vsprite.Handle());
    if (sprite2D)
    {
        if (varg.IsArray() && 
            (varg.DimensionsSize() != 1 || varg.Dimensions()[0] != 2))
            return IllegalArgument;
        sprite2D->SetTranslation(varg);
    }
    else
    {
        VectrexCommandTransformable3D *sprite3D = dynamic_cast<VectrexCommandTransformable3D *>(vsprite.Handle());
        if (sprite3D)
        {
            if (varg.IsArray() && 
                (varg.DimensionsSize() != 1 || varg.Dimensions()[0] != 3))
                return IllegalArgument;
            sprite3D->SetTranslation(varg);
        }
        else
            return IllegalArgument;
    }
    return 0;
}


static ErrorCode SpriteRotate2D(RunTimeEnvironment &runTimeEnvironment)
{
    Value &vangle = runTimeEnvironment.calculationStack.back();
    Value &vsprite = runTimeEnvironment.calculationStack.back(1);
    if (!vsprite.IsHandle() || !vangle.IsNumeric())
        return IllegalArgument;
    VectrexCommandTransformable2D *sprite = dynamic_cast<VectrexCommandTransformable2D *>(vsprite.Handle());
    if (sprite)
    {
        float angle;
        vangle.TypeBehaviors()->ToFloat(vangle, &angle);
        sprite->Rotate(angle);
        return 0;
    }
    else
        return IllegalArgument;
}

// call SpriteRotate(sprite3D, pitch, roll, yaw)
static ErrorCode SpriteRotate3D(RunTimeEnvironment &runTimeEnvironment)
{
    Value &vyaw = runTimeEnvironment.calculationStack.back();
    Value &vroll = runTimeEnvironment.calculationStack.back(1);
    Value &vpitch = runTimeEnvironment.calculationStack.back(2);
    Value &vsprite = runTimeEnvironment.calculationStack.back(3);
    if (!vsprite.IsHandle() || !vpitch.IsNumeric() || !vroll.IsNumeric() || !vyaw.IsNumeric())
        return IllegalArgument;
    VectrexCommandTransformable3D *sprite = dynamic_cast<VectrexCommandTransformable3D *>(vsprite.Handle());
    if (sprite)
    {
        float pitch, roll, yaw;
        vpitch.TypeBehaviors()->ToFloat(vpitch, &pitch);
        vroll.TypeBehaviors()->ToFloat(vroll, &roll);
        vyaw.TypeBehaviors()->ToFloat(vyaw, &yaw);
        sprite->Rotate(pitch, roll, yaw);
        return 0;
    }
    else
        return IllegalArgument;
}

// Rotates the sprite <angle> degrees
// call SpriteRotate(sprite2D, angle)
// call SpriteRotate(sprite3D, pitch, roll, yaw)
static ErrorCode SpriteRotate(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
    if (count == 4)
        return SpriteRotate3D(runTimeEnvironment);
    else if (count == 2)
        return SpriteRotate2D(runTimeEnvironment);
    else
        return WrongNumberOfArguments;
}


// call SpriteSetMagnification(sprite, magnification[, magnificationY[, magnificationZ]])
static ErrorCode SpriteSetMagnification(Executive &, RunTimeEnvironment &runTimeEnvironment, int argc)
{
    Value &vsprite = runTimeEnvironment.calculationStack.back(argc - 1);
    if (!vsprite.IsHandle())
        return IllegalArgument;
    VectrexCommandTransformable *sprite = dynamic_cast<VectrexCommandTransformable *>(vsprite.Handle());
    VectrexCommandTransformable3D *sprite3D = dynamic_cast<VectrexCommandTransformable3D *>(sprite);
    if (!sprite3D && argc == 4)
        return WrongNumberOfArguments;

    Value &vmagz = runTimeEnvironment.calculationStack.back();
    Value &vmagy = runTimeEnvironment.calculationStack.back(argc == 4 ? 1 : 0);
    Value &vmagx = runTimeEnvironment.calculationStack.back(argc - 2);
    if (!vmagx.IsNumeric() || !vmagy.IsNumeric() || !vmagz.IsNumeric())
        return IllegalArgument;
    
    if (sprite)
    {
        float magx, magy, magz;
        vmagx.TypeBehaviors()->ToFloat(vmagx, &magx);
        vmagy.TypeBehaviors()->ToFloat(vmagy, &magy);
        vmagz.TypeBehaviors()->ToFloat(vmagz, &magz);
        sprite->Magnification(magx, magy, magz);
        return 0;
    }
    else
        return IllegalArgument;
}


// Return an array of vertices for a regular polygon with the given number of sides
// and a radius of 1.
// array = RegularPolygon(5, 20[, initial rotation]) ' 5 sides, radius 20
static ErrorCode RegularPolygon(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
    // If the user has passed in an initial rotation
    float initialRotation = 0;
    bool error = false;
    if (count == 3)
    {
        Value &vrotation = runTimeEnvironment.calculationStack.back();
        if (vrotation.IsNumeric())
        {
            vrotation.TypeBehaviors()->ToFloat(vrotation, &initialRotation);
            // Convert to rads
            initialRotation = float(initialRotation * 3.1415926 / 180);
        }
        else
            error = true;
        runTimeEnvironment.calculationStack.pop_back();
    }
    else
        initialRotation = 0;
    Value &vradius = runTimeEnvironment.calculationStack.back();
    Value &vsides = runTimeEnvironment.calculationStack.back(1);
    if (error)
        return IllegalArgument;
    int sides;
    float radius;
    if (vsides.IsNumeric())
        vsides.TypeBehaviors()->ToInteger(vsides, &sides);
    else
        return IllegalArgument;
    if (vradius.IsNumeric())
        vradius.TypeBehaviors()->ToFloat(vradius, &radius);
    else
        return IllegalArgument;
    
    if (sides < 3)
        return IllegalArgument;
    
    uint16_t dimensions[2] = {uint16_t(sides + 1), 3};
    Value va(dimensions, 2);
    float tau = float(3.1415926 * 2);
    int row = 0;
    int action = 0; // First point is a "Move to"
    for (int i = 0; i <= sides; ++i)
    {
        va[row] = action;
        float angle = tau * i / sides + initialRotation;
        va[row + 1] = cos(angle) * radius;
        va[row + 2] = sin(angle) * radius;
        row += 3;
        action = 1; // Subsequent points are "Draw to"
    }
    SetReturnValue(runTimeEnvironment, va);
    return 0;
}

// call Offset(array, x, y)
// call Offset(array, x, y, z)
// Adds x, y, and optionally z to the coordinates in array. If Offset finds uninitialized 
// values in the array, it stops
static ErrorCode Offset(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
    Value vz;
    if (count == 4)
    {
        vz = runTimeEnvironment.calculationStack.back();
        runTimeEnvironment.calculationStack.pop_back();
    }
    Value &vy = runTimeEnvironment.calculationStack.back();
    Value &vx = runTimeEnvironment.calculationStack.back(1);
    Value &varray = runTimeEnvironment.calculationStack.back(2);
    if (!vx.IsNumeric() || !vy.IsNumeric() || !varray.IsArray() ||
        (!vz.IsUninitialized() && !vz.IsNumeric()))
        return IllegalArgument;
    
    const uint16_t *dims = varray.Dimensions();
    uint16_t dimsSize = varray.DimensionsSize();
    
    int xIndex, yIndex, zIndex;
    // If we're doing 2D
    if (count == 3)
    {
        if (dimsSize != 2 || dims[1] < 2 || dims[1] > 3)
            return IllegalArgument;
        xIndex = dims[1] - 2;
        yIndex = dims[1] - 1;
        zIndex = -1;
    }
    // Else (3D)
    else
    {
        if (dimsSize != 2 || dims[1] != 4)
            return IllegalArgument;
        xIndex = dims[1] - 3;
        yIndex = dims[1] - 2;
        zIndex = dims[1] - 1;
    }
    
    float offsetX, offsetY, offsetZ = 0;
    vx.TypeBehaviors()->ToFloat(vx, &offsetX);
    vy.TypeBehaviors()->ToFloat(vy, &offsetY);
    if (vz.IsNumeric())
        vz.TypeBehaviors()->ToFloat(vz, &offsetZ);
    
    // Go through each row
    int rowIndex = 0;
    for (int i = 0; i < dims[0]; ++i, rowIndex += dims[1])
    {
        if (varray[rowIndex + xIndex].IsUninitialized() || varray[rowIndex + yIndex].IsUninitialized())
            break;
        if (!varray[rowIndex + xIndex].IsNumeric() || !varray[rowIndex + yIndex].IsNumeric())
            return IllegalArgument;
        if (count == 4)
        {
            if (varray[rowIndex + zIndex].IsUninitialized())
                break;
            if (!varray[rowIndex + zIndex].IsNumeric())
                return IllegalArgument;
        }
        float x, y;
        varray[rowIndex + xIndex].TypeBehaviors()->ToFloat(varray[rowIndex + xIndex], &x);
        varray[rowIndex + yIndex].TypeBehaviors()->ToFloat(varray[rowIndex + yIndex], &y);
        x += offsetX;
        y += offsetY;
        varray[rowIndex + xIndex] = x;
        varray[rowIndex + yIndex] = y;
        if (count == 4)
        {
            float z;
            varray[rowIndex + zIndex].TypeBehaviors()->ToFloat(varray[rowIndex + zIndex], &z);
            z += offsetZ;
            varray[rowIndex + zIndex] = z;
        }
    }
    
    return 0;
}
// Modifies a sprite that moves the position on the screen
// relative to the current position.
// x and y can be larger than 127 and -128. If they are, multiple Moveto calls
// are sent to the Vectrex
// call SpriteMove(pseudosprite, x, y)
static ErrorCode SpriteMove(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vy = runTimeEnvironment.calculationStack.back();
    Value &vx = runTimeEnvironment.calculationStack.back(1);
    Value &vsprite = runTimeEnvironment.calculationStack.back(2);
    if (!vx.IsNumeric() || !vy.IsNumeric())
        return IllegalArgument;
    
    int x, y;
    vx.TypeBehaviors()->ToInteger(vx, &x);
    vy.TypeBehaviors()->ToInteger(vy, &y);
    
    if (!vsprite.IsHandle())
        return IllegalArgument;
    VectrexCommandMove *obj = dynamic_cast<VectrexCommandMove *>(vsprite.Handle());
    obj->Move(x, y);

    return 0;
}

// call Sound(registerSettingsArray)
// The array is either a 1 dimensional array, 14 elements long (matching the
// 14 sound registers on the AY-3-8914), or a 2 dimensional array where each row is 
// a register number and value
static ErrorCode Sound(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &varg = runTimeEnvironment.calculationStack.back();
    
    if (!varg.IsArray())
        return IllegalArgument;
    VectrexInterface &vi = VectrexInterface::Singleton();
    const uint16_t *dims = varg.Dimensions();
    if (varg.DimensionsSize() == 1)
    {
        if (dims[0] < 14)
            return IllegalArgument;
        for (size_t registerIndex = 0; registerIndex < dims[0]; ++registerIndex)
        {
            if (!varg[registerIndex].IsInteger())
                return IllegalArgument;
            int value = varg[registerIndex].Integer();
            // Don't let the user write to the I/O bits in register 7
            if (registerIndex == 7)
                value &= 0x3f;
            
            
v_writePSG_double_buffered(uint8_t(registerIndex), uint8_t(value));
            
//            if (!vi.WriteSoundRegister(uint8_t(registerIndex), uint8_t(value)))
//                return IllegalArgument;
        }
    }
    else if (varg.DimensionsSize() == 2)
    {
        if (dims[1] != 2)
            return IllegalArgument;
        for (size_t i = 0; i < dims[0]; ++i)
        {
            size_t arrayIndex = i + i;
            if (!varg[arrayIndex].IsInteger() || !varg[arrayIndex + 1].IsInteger())
                return IllegalArgument;
            int registerIndex = varg[arrayIndex].Integer();
            if (registerIndex < 0 || registerIndex > 13)
                return IllegalArgument;
            uint8_t value = uint8_t(varg[arrayIndex + 1].Integer());
            // Don't let the user write to the I/O bits in register 7
            if (registerIndex == 7)
                value &= 0x3f;
v_writePSG_double_buffered(uint8_t(registerIndex), uint8_t(value));
//            if (!vi.WriteSoundRegister(uint8_t(registerIndex), uint8_t(value)))
//                return IllegalArgument;
        }
    }
    else
        return IllegalArgument;
    return 0;
}



static ErrorCode CameraGetFocalLength(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    runTimeEnvironment.SetReturnValue(Value(camera.GetFocalLength()));
    return 0;
}

// angles = CameraGetRotation() - an array with pitch, roll, yaw
static ErrorCode CameraGetRotation(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    uint16_t dimensions[1] = {3};
    Value angles(dimensions, 1);
    angles[0] = camera.GetRotation()[0];
    angles[1] = camera.GetRotation()[1];
    angles[2] = camera.GetRotation()[2];
    SetReturnValue(runTimeEnvironment, angles);
    return 0;
 }

// Sets the absolute angle of rotation, in degrees
// call CameraSetRotation(pitch, yaw, roll)
static ErrorCode CameraRotate(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vyaw = runTimeEnvironment.calculationStack.back();
    Value &vroll = runTimeEnvironment.calculationStack.back(1);
    Value &vpitch = runTimeEnvironment.calculationStack.back(2);
    if (!vpitch.IsNumeric() || !vroll.IsNumeric() || !vyaw.IsNumeric())
        return IllegalArgument;
    float pitch, roll, yaw;
    vpitch.TypeBehaviors()->ToFloat(vpitch, &pitch);
    vroll.TypeBehaviors()->ToFloat(vroll, &roll);
    vyaw.TypeBehaviors()->ToFloat(vyaw, &yaw);
    camera.Rotate(pitch, roll, yaw);
    return 0;
}

static ErrorCode CameraSet3D(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &convergence = runTimeEnvironment.calculationStack.back();
    Value &ocularDistance = runTimeEnvironment.calculationStack.back(1);
    if (!ocularDistance.IsNumeric() || !convergence.IsNumeric())
        return IllegalArgument;
    float fOcularDistance = ocularDistance.IsFloat() ? ocularDistance.Float() : ocularDistance.Integer();
    float fConvergence = convergence.IsFloat() ? convergence.Float() : convergence.Integer();
    if (fOcularDistance < 0 || fConvergence < 0)
        return IllegalArgument;
    camera.Set3D(ocularDistance.IsFloat() ? ocularDistance.Float() : ocularDistance.Integer(),
       convergence.IsFloat() ? convergence.Float() : convergence.Integer());
    return 0;
}

static ErrorCode CameraSetFocalLength(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vfl = runTimeEnvironment.calculationStack.back();
    if (!vfl.IsNumeric())
        return IllegalArgument;
    float fl;
    vfl.TypeBehaviors()->ToFloat(vfl, &fl);
    camera.SetFocalLength(fl);
    return 0;
}

// Sets the absolute angle of rotation, in degrees
// call CameraSetRotation(pitch, yaw, roll)
static ErrorCode CameraSetRotation(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vyaw = runTimeEnvironment.calculationStack.back();
    Value &vroll = runTimeEnvironment.calculationStack.back(1);
    Value &vpitch = runTimeEnvironment.calculationStack.back(2);
    if (!vpitch.IsNumeric() || !vroll.IsNumeric() || !vyaw.IsNumeric())
        return IllegalArgument;
    float pitch, roll, yaw;
    vpitch.TypeBehaviors()->ToFloat(vpitch, &pitch);
    vroll.TypeBehaviors()->ToFloat(vroll, &roll);
    vyaw.TypeBehaviors()->ToFloat(vyaw, &yaw);
    camera.SetRotation(pitch, roll, yaw);
    return 0;
}

// call CameraTranslate(sprite, {x, y})
static ErrorCode CameraTranslate(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &varg = runTimeEnvironment.calculationStack.back();
    if (!varg.IsArray())
        return IllegalArgument;
    if (varg.IsArray() && 
        (varg.DimensionsSize() != 1 || varg.Dimensions()[0] != 3))
        return IllegalArgument;
    camera.SetTranslation(varg);
    return 0;
}

// rotated = VectorRotate(1by2array, angle)
// rotated = VectorRotate(1by3array, pitch, roll, yaw)
static ErrorCode VectorRotate(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
    static const float degToRad = float(3.14159 / 180);
    if (count == 4)
    {
        Value &vyaw = runTimeEnvironment.calculationStack.back();
        Value &vroll = runTimeEnvironment.calculationStack.back(1);
        Value &vpitch = runTimeEnvironment.calculationStack.back(2);
        Value &vvector = runTimeEnvironment.calculationStack.back(3);
        if (!vpitch.IsNumeric() || !vroll.IsNumeric() || !vyaw.IsNumeric() || !vvector.IsArray())
            return IllegalArgument;
        if (vvector.DimensionsSize() != 1 || vvector.Dimensions()[0] != 3 ||
            !vvector[0].IsNumeric() || !vvector[1].IsNumeric() || !vvector[2].IsNumeric())
            return IllegalArgument;
        float pitch, roll, yaw;
        vpitch.TypeBehaviors()->ToFloat(vpitch, &pitch);
        vroll.TypeBehaviors()->ToFloat(vroll, &roll);
        vyaw.TypeBehaviors()->ToFloat(vyaw, &yaw);
        pitch *= degToRad;
        roll *= degToRad;
        yaw *= degToRad;
        float s3 = sin(pitch), c3 = cos(pitch);
        float s2 = sin(roll), c2 = cos(roll);
        float s1 = sin(yaw), c1 = cos(yaw);
        float x, y, z;
        vvector[0].TypeBehaviors()->ToFloat(vvector[0], &x);
        vvector[1].TypeBehaviors()->ToFloat(vvector[1], &y);
        vvector[2].TypeBehaviors()->ToFloat(vvector[2], &z);
        uint16_t dims[] = {3};
        Value result(dims, 1);
        result[0] = x * c1 * c2 + y * s2 - z * c2 * s1;
        result[1] = x * s1 * s3 - c1 * c3 * s2 + y * c2 * c3 + z * (c1 * s3 + c3 * s1 * s2);
        result[2] = x * (c3 * s1 + c1 * s2 * s3) - y * c2 * s3 + z *(c1 * c3 - s1 * s2 * s3);
        SetReturnValue(runTimeEnvironment, result);
    }
    else
    {
        Value &vangle = runTimeEnvironment.calculationStack.back();
        Value &vvector = runTimeEnvironment.calculationStack.back(1);
        if (!vangle.IsNumeric()|| !vvector.IsArray())
            return IllegalArgument;
        if (vvector.DimensionsSize() != 1 || vvector.Dimensions()[0] != 2 ||
            !vvector[0].IsNumeric() || !vvector[1].IsNumeric())
            return IllegalArgument;
        float angle, x, y;
        vangle.TypeBehaviors()->ToFloat(vangle, &angle);
        vvector[0].TypeBehaviors()->ToFloat(vvector[0], &x);
        vvector[1].TypeBehaviors()->ToFloat(vvector[1], &y);
        angle *= degToRad;
        float c = cos(angle), s = sin(angle);
        uint16_t dims[] = {2};
        Value result(dims, 1);
        result[0] = x * c - y * s;
        result[1] = x * s + y * c;
        SetReturnValue(runTimeEnvironment, result);
    }
    return 0;
}

// sprite = Lines25DSprite(<array>)
// 2.5D objects are flat but get placed in 3D space.
// The array is two dimensional, with the second dimension having a size of 2 or 3
// If the size is 3, they are (x, y) coordinates. If it's 4, they are (mode, x, y)
// The mode is 0 for "move to", non-zero for "draw to"
static ErrorCode Lines25DSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &arg = runTimeEnvironment.calculationStack.back();
    if (!arg.IsArray())
        return IllegalArgument;
    if (arg.DimensionsSize() != 2 || (arg.Dimensions()[1] != 2 && arg.Dimensions()[1] != 3))
        return IllegalArgument;
    VectrexCommand *obj = new VectrexCommandLines25D(arg);
    AddCommand(obj);
    SetReturnValue(runTimeEnvironment, Value(commands.back()));
    return 0;
}

// sprite = Lines3DSprite(<array>)
// The array is two dimensional, with the second dimension having a size of 3 or 4
// If the size is 3, they are (x, y, z) coordinates. If it's 4, they are (mode, x, y, z)
// The mode is 0 for "move to", non-zero for "draw to"
static ErrorCode Lines3DSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &arg = runTimeEnvironment.calculationStack.back();
    if (!arg.IsArray())
        return IllegalArgument;
    if (arg.DimensionsSize() != 2 || (arg.Dimensions()[1] != 3 && arg.Dimensions()[1] != 4))
        return IllegalArgument;
    VectrexCommand *obj = new VectrexCommandLines3D(arg);
    AddCommand(obj);
    SetReturnValue(runTimeEnvironment, Value(commands.back()));
    return 0;
}



// sprite = Text2ListSprite(<list>)
// <list> is an array where each row is {x, y, value}. The value can be a string
// or a number. WARNING: a y coordinate of zero for any string except the first,
// will terminate the text list
// x and y coordinates are absolute in scale 0x7f
// Text2ListSprite differs from TextListSprite in that it uses a transformable
// vector font
static ErrorCode Text2ListSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &arg = runTimeEnvironment.calculationStack.back();
    if (!arg.IsArray())
        return IllegalArgument;
    const uint16_t *dims = arg.Dimensions();
    if (arg.DimensionsSize() != 2 || dims[1] < 3)
        return IllegalArgument;
    VectrexCommand *obj = new VectrexCommandPrint2List(arg);
    AddCommand(obj);
    SetReturnValue(runTimeEnvironment, Value(commands.back()));
    return 0;
}

// sprite = Text2Sprite(<text>)
// Draws text with vector font
static ErrorCode Text2Sprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &arg = runTimeEnvironment.calculationStack.back();
    VectrexCommand *obj = new VectrexCommandPrint2Str(arg.ToString().c_str());
    AddCommand(obj);
    SetReturnValue(runTimeEnvironment, Value(commands.back()));
    return 0;
}
// Return the dimensions of the vector font
// metrics = GetFontMetrics()
// metrics[1] = char width
// metrics[2] = char height
// metrics[3] = horizontal advance
// metrics[4] = line height
// metrics[5] = ascent
static ErrorCode GetFontMetrics(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    uint16_t dims[1] = {5};
    Value metrics(dims, 1);
    metrics[0] = vectorFontTable.charWidth * FONT_MAGNIFICATION;
    metrics[1] = vectorFontTable.charHeight * FONT_MAGNIFICATION;
    metrics[2] = vectorFontTable.advance * FONT_MAGNIFICATION;
    metrics[3] = vectorFontTable.lineHeight * FONT_MAGNIFICATION;
    metrics[4] = vectorFontTable.ascent * FONT_MAGNIFICATION;
    SetReturnValue(runTimeEnvironment, metrics);
    return 0;
}
// lines = TextToLines(<text>)
// Returns an array of {mode, x, y} for drawing the characters of the string
static ErrorCode TextToLines(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &arg = runTimeEnvironment.calculationStack.back();
    String str = arg.ToString();
    Value rtn = TextToLines(str.c_str());
    SetReturnValue(runTimeEnvironment, rtn);
    return 0;
}

// sprite = TextSprite(<text>)
static ErrorCode TextSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &arg = runTimeEnvironment.calculationStack.back();
    VectrexCommand *obj = new VectrexCommandPrintStr(arg.ToString().c_str());
    AddCommand(obj);
    SetReturnValue(runTimeEnvironment, Value(commands.back()));
    return 0;
}

// call PtInRect(rect, pt)
// rect is {{x1, y1}, {x2, y2}} and pt is {x, y}
// call PtInRect(x1, y1, x2, y2, x, y)
// Returns true if the point is within the rectangle, inclusive of the edges.
// The rect does not need to be normalized
static ErrorCode PtInRect(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
    float pt[2], rect[2][2];
    if (count == 2)
    {
        Value &vpt = runTimeEnvironment.calculationStack.back();
        Value &vrect = runTimeEnvironment.calculationStack.back(1);
        if (!vpt.IsArray() || !vrect.IsArray())
            return IllegalArgument;
        const uint16_t *ptdims = vpt.Dimensions();
        if (vpt.DimensionsSize() != 1 || ptdims[0] != 2)
            return IllegalArgument;
        const uint16_t *rectdims = vrect.Dimensions();
        if (vrect.DimensionsSize() != 2 || rectdims[0] != 2 || rectdims[1] != 2)
            return IllegalArgument;
        ErrorCode error = vpt.GetArrayValues(pt);
        if (error)
            return error;
        error = vrect.GetArrayValues((float *) rect);
        if (error)
            return error;
    }
    else if (count == 6)
    {
        Value &vy = runTimeEnvironment.calculationStack.back();
        Value &vx = runTimeEnvironment.calculationStack.back(1);
        Value &vy2 = runTimeEnvironment.calculationStack.back(2);
        Value &vx2 = runTimeEnvironment.calculationStack.back(3);
        Value &vy1 = runTimeEnvironment.calculationStack.back(4);
        Value &vx1 = runTimeEnvironment.calculationStack.back(5);
        if (!vx1.IsNumeric() || !vy1.IsNumeric() || !vx2.IsNumeric() || !vy2.IsNumeric() || !vx.IsNumeric() || !vy.IsNumeric())
            return IllegalArgument;
        vx1.TypeBehaviors()->ToFloat(vx1, &rect[0][0]);
        vy1.TypeBehaviors()->ToFloat(vy1, &rect[0][1]);
        vx2.TypeBehaviors()->ToFloat(vx2, &rect[1][0]);
        vy2.TypeBehaviors()->ToFloat(vy2, &rect[1][1]);
        vx.TypeBehaviors()->ToFloat(vx, &pt[0]);
        vy.TypeBehaviors()->ToFloat(vy, &pt[1]);
    }
    else
        return WrongNumberOfArguments;
    
    if (rect[0][0] > rect[1][0])
        std::swap(rect[0][0], rect[1][0]);
    if (rect[0][1] > rect[1][1])
        std::swap(rect[0][1], rect[1][1]);
    
    bool in = (pt[0] >= rect[0][0] && pt[0] <= rect[1][0]) && (pt[1] >= rect[0][1] && pt[1] <= rect[1][1]);
    SetReturnValue(runTimeEnvironment, Value(in));
    
    return 0;
}

// Removes a sprite from the drawing list
// call RemoveSprite(sprite)
static ErrorCode RemoveSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vsprite = runTimeEnvironment.calculationStack.back();
    if (!vsprite.IsHandle())
        return IllegalArgument;
    VectrexCommand *sprite = dynamic_cast<VectrexCommand *>(vsprite.Handle());
    if (sprite)
    {
        RemoveCommand(sprite, true);
        return 0;
    }
    else
        return IllegalArgument;
}
// Sets the user data
// call SpriteSetUserData(sprite, value)
static ErrorCode SpriteSetUserData(Executive &exec, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &data = runTimeEnvironment.calculationStack.back();
    Value &vcmd = runTimeEnvironment.calculationStack.back(1);
    if (!vcmd.IsHandle())
        return IllegalArgument;
    VectrexCommand *cmd = dynamic_cast<VectrexCommand *>(vcmd.Handle());
    if (cmd)
        cmd->SetUserData(data);
    else
        return IllegalArgument;
    return 0;
}
// call SpriteGetUserData(intensityCmd, newIntensity)
static ErrorCode SpriteGetUserData(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vcmd = runTimeEnvironment.calculationStack.back();
    if (!vcmd.IsHandle())
        return IllegalArgument;
    VectrexCommand *cmd = dynamic_cast<VectrexCommand *>(vcmd.Handle());
    if (cmd)
    {
        SetReturnValue(runTimeEnvironment, cmd->GetUserData());
    }
    else
        return IllegalArgument;
    return 0;
}

// call SpriteIntensity(intensityCmd, newIntensity)
static ErrorCode SpriteIntensity(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vintensity = runTimeEnvironment.calculationStack.back();
    Value &vcmd = runTimeEnvironment.calculationStack.back(1);
    if (!vintensity.IsNumeric())
        return IllegalArgument;
    if (!vcmd.IsHandle())
        return IllegalArgument;
    
    int intensity;
    vintensity.TypeBehaviors()->ToInteger(vintensity, &intensity);

    VectrexCommandIntensity *cmd = dynamic_cast<VectrexCommandIntensity *>(vcmd.Handle());
    if (cmd)
    {
        cmd->Intensity(intensity);
    }
    else
        return IllegalArgument;
    return 0;
}

// call SpriteSetMagnification(sprite, magnification)
static ErrorCode SpriteSetData(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vdata = runTimeEnvironment.calculationStack.back();
    if (!vdata.IsArray())
        return IllegalArgument;
    Value &vsprite = runTimeEnvironment.calculationStack.back(1);
    if (!vsprite.IsHandle())
        return IllegalArgument;
    VectrexCommandTransformable *sprite = dynamic_cast<VectrexCommandTransformable *>(vsprite.Handle());
    if (!sprite)
        return IllegalArgument;
    
    return sprite->SetData(vdata);
}


// removes a sprite from the drawing list and re-inserts it after the pos sprite
// call PutSpriteAfter(pos, sprite)
static ErrorCode PutSpriteAfter(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vsprite = runTimeEnvironment.calculationStack.back();
    Value &vpos = runTimeEnvironment.calculationStack.back(1);
    if (!vsprite.IsHandle() || !vpos.IsHandle())
        return IllegalArgument;
    VectrexCommand *sprite = dynamic_cast<VectrexCommand *>(vsprite.Handle());
    VectrexCommand *pos = dynamic_cast<VectrexCommand *>(vpos.Handle());
    if (sprite && pos)
    {
        RemoveCommand(sprite, false);
        auto i = FindCommand(pos);
        if (i != commands.end())
            ++i;
        commands.insert(i, vsprite);
        return 0;
    }
    else
        return IllegalArgument;
}

// removes a sprite from the drawing list and re-inserts it before the pos sprite
// call PutSpriteBefore(pos, sprite)
static ErrorCode PutSpriteBefore(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vsprite = runTimeEnvironment.calculationStack.back();
    Value &vpos = runTimeEnvironment.calculationStack.back(1);
    if (!vsprite.IsHandle() || !vpos.IsHandle())
        return IllegalArgument;
    VectrexCommand *sprite = dynamic_cast<VectrexCommand *>(vsprite.Handle());
    VectrexCommand *pos = dynamic_cast<VectrexCommand *>(vpos.Handle());
    if (sprite && pos)
    {
        RemoveCommand(sprite, false);
        auto i = FindCommand(pos);
        if (i == commands.end())
            commands.push_front(vsprite);
        else
            commands.insert(i, vsprite);
        return 0;
    }
    else
        return IllegalArgument;
}

//
// call SpritePrintVectors(sprite) - last argument of GetVectrexData is TRUE!
static ErrorCode SpritePrintVectors(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vsprite = runTimeEnvironment.calculationStack.back();
    if (!vsprite.IsHandle())
        return IllegalArgument;
    VectrexCommandTransformable *sprite = dynamic_cast<VectrexCommandTransformable *>(vsprite.Handle());
    if (sprite)
    {
        Array<int32_t> data;
        size_t count;
        VectrexCommand::ClearError();
        sprite->GetVectrexData(&data, &count, dynamic_cast<VectrexCommandDots *>(sprite) == null, true, true);
        return 0;
    }
    else
        return IllegalArgument;
}


// In Vectrex-format music, specifies up to three notes to be played simultaneously
// call ABC(aNote{, bNote{, cNote}).
static ErrorCode ABC(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
    bool ints = true;
    // The high 8 bits are 0, 1 or two, indicating 1, 2 or 3 notes
    uint32_t accumulatedNote = (count - 1) << 24; 
    while (count--)
    {
        Value &vnote = runTimeEnvironment.calculationStack.back();
        if (vnote.IsInteger())
            accumulatedNote |= uint8_t(vnote.Integer()) << (8 * count);
        else
            ints = false;
        runTimeEnvironment.calculationStack.pop_back();
    }
    if (!ints)
        return IllegalArgument;
    runTimeEnvironment.SetReturnValue(Value(int(accumulatedNote)));
    return 0;
}
static ErrorCode DotsSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &arg = runTimeEnvironment.calculationStack.back();
    if (!arg.IsArray())
        return IllegalArgument;
    if (arg.DimensionsSize() != 2 || arg.Dimensions()[1] != 2)
        return IllegalArgument;
    VectrexCommand *obj = new VectrexCommandDots(arg);
    AddCommand(obj);
    SetReturnValue(runTimeEnvironment, Value(commands.back()));
    return 0;
}

// sprite = DrawingListSprite(n) - Return the n-th sprite in the drawing list
static ErrorCode DrawingListSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vindex = runTimeEnvironment.calculationStack.back();
    if (!vindex.IsInteger())
        return IllegalArgument;

    int index = vindex.Integer();
    if (vindex.Integer() < 1 || vindex.Integer() > (int) commands.size())
        return IllegalArgument;
    
    auto iter = commands.begin();
    while (--index)
        ++iter;
    SetReturnValue(runTimeEnvironment, *iter);
    
    return 0;
}

// call DumpSprite(sprite)
static ErrorCode DumpSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    const VectrexCommand *sprite;
    Value &vsprite = runTimeEnvironment.calculationStack.back();
    if (!vsprite.IsHandle())
        return IllegalArgument;

    sprite = dynamic_cast<VectrexCommand *>(vsprite.Handle());
    if (!sprite)
        return IllegalArgument;
    sprite->Dump();
    fputs((char *)"User Data: ", stdout);
    sprite->GetUserData().Dump();
    return 0;
}

static ErrorCode DumpSprites(Executive &, RunTimeEnvironment &, int)
{
    int index = 1;
    for (auto i = commands.begin(); i != commands.end(); ++i, ++index)
    {
        fprintf(stdout, "[%d]: ", index);
        const VectrexCommand *vc = (const VectrexCommand *) i->Handle();
        vc->Dump();
        fputs((char *)"User Data: ", stdout);
        vc->GetUserData().Dump();
        fputs((char *)"\r\n\r\n", stdout);
    }
    return 0;
}

// Return the operating system tick count
// tick = GetTickCount()
extern uint32_t v_micros(); // vectrexInterface.h
static ErrorCode GetTickCount(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    uint32_t tick = (uint32_t) v_micros();
    tick = tick *  (  ( (float)960)/1000000.0);
    
    // 960 times per second    
    
    SetReturnValue(runTimeEnvironment, Value((int) tick));
    return 0;
}

// Renames a file
// print Rename(oldpath, newpath)
static ErrorCode Rename(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &newpath = runTimeEnvironment.calculationStack.back();
    Value &oldpath = runTimeEnvironment.calculationStack.back(1);
    if (!newpath.IsString() || !oldpath.IsString())
        return IllegalArgument;
    int rtn = rename(oldpath.String().c_str(), newpath.String().c_str());
    SetReturnValue(runTimeEnvironment, Value(~rtn));
    
    return 0;
}

// Removes a file
// print Remove(fileName)
static ErrorCode Remove(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &fileName = runTimeEnvironment.calculationStack.back();
    if (!fileName.IsString())
        return IllegalArgument;
    SetReturnValue(runTimeEnvironment, Value(remove(fileName.String().c_str())));
    
    return 0;
}


// Close a file using the same arguments as C's fclose
static ErrorCode FClose(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vfile = runTimeEnvironment.calculationStack.back();
    if (!vfile.IsHandle())
        return IllegalArgument;
    FilePointer *fp = dynamic_cast<FilePointer *>(vfile.Handle());
    if (fp == null || fp->FileP() == null)
        return IllegalArgument;
    int rtn = fclose(fp->FileP());
    fp->Invalidate();
    SetReturnValue(runTimeEnvironment, Value(rtn));

    return 0;
}

// Indicates whether we've reached the end of the file
static ErrorCode FEOF(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vfile = runTimeEnvironment.calculationStack.back();
    if (!vfile.IsHandle())
        return IllegalArgument;
    FilePointer *fp = dynamic_cast<FilePointer *>(vfile.Handle());
    if (fp == null || fp->FileP() == null)
        return IllegalArgument;
//    if (fp->FileP() == stdin)
//        SetReturnValue(runTimeEnvironment, Value(!CONSOLE_CharWaiting()));
//    else
        SetReturnValue(runTimeEnvironment, Value(feof(fp->FileP())));

    return 0;
}

// Reads a character from a file
static ErrorCode FGetc(Executive &, RunTimeEnvironment &runTimeEnvironment, int argCount)
{
    Value &vfile = runTimeEnvironment.calculationStack.back();
    if (!vfile.IsHandle())
        return IllegalArgument;
    FilePointer *fp = dynamic_cast<FilePointer *>(vfile.Handle());
    if (fp == null || fp->FileP() == null)
        return IllegalArgument;
    int ch = fgetc(fp->FileP());
    SetReturnValue(runTimeEnvironment, Value(ch));
    return 0;
}

// Open a file using the same arguments as C's fopen
static ErrorCode FOpen(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    static const Value emptyValue;

    Value &mode = runTimeEnvironment.calculationStack.back();
    Value &name = runTimeEnvironment.calculationStack.back(1);
    if (!mode.IsString() || !name.IsString())
        return IllegalArgument;
    FILE *file = fopen(name.String().c_str(), mode.String().c_str());
    if (file)
        runTimeEnvironment.SetReturnValue(Value(new FilePointer(file)));
    else
        runTimeEnvironment.SetReturnValue(emptyValue);
    
    return 0;
}

// Read a string from a file
static ErrorCode FRead(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vfile = runTimeEnvironment.calculationStack.back();
    Value &size = runTimeEnvironment.calculationStack.back(1);
    if (!vfile.IsHandle() || !size.IsInteger())
        return IllegalArgument;
    FilePointer *fp = dynamic_cast<FilePointer *>(vfile.Handle());
    if (fp == null || fp->FileP() == null)
        return IllegalArgument;
    String buf('\0', size.Integer());
    size_t read = fread(buf.buffer(), 1, size.Integer(), fp->FileP());
    if (read < (size_t) size.Integer())
        buf = String(buf.c_str(), read);
    runTimeEnvironment.SetReturnValue(Value(buf));
    
    return 0;
}

static ErrorCode FSeek(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vorigin = runTimeEnvironment.calculationStack.back();
    Value &voffset = runTimeEnvironment.calculationStack.back(1);
    Value &vfile = runTimeEnvironment.calculationStack.back(2);
    if (!vorigin.IsInteger() || !voffset.IsInteger() || !vfile.IsHandle())
        return IllegalArgument;
    FilePointer *fp = dynamic_cast<FilePointer *>(vfile.Handle());
    if (fp == null || fp->FileP() == null)
        return IllegalArgument;
    int result = fseek(fp->FileP(), voffset.Integer(), vorigin.Integer());
    runTimeEnvironment.SetReturnValue(Value(result));
    
    return 0;
}

static ErrorCode FTell(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vfile = runTimeEnvironment.calculationStack.back();
    if (!vfile.IsHandle())
        return IllegalArgument;
    FilePointer *fp = dynamic_cast<FilePointer *>(vfile.Handle());
    if (fp == null || fp->FileP() == null)
        return IllegalArgument;
    int result = (int) ftell(fp->FileP());
    runTimeEnvironment.SetReturnValue(Value(result));
    
    return 0;
}

// Write data to a file
static ErrorCode FWrite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vfile = runTimeEnvironment.calculationStack.back();
    Value &size = runTimeEnvironment.calculationStack.back(1);
    Value &str = runTimeEnvironment.calculationStack.back(2);
    if (!vfile.IsHandle() || !str.IsString() || !size.IsInteger())
        return IllegalArgument;
    FilePointer *fp = dynamic_cast<FilePointer *>(vfile.Handle());
    if (fp == null || fp->FileP() == null)
        return IllegalArgument;
    runTimeEnvironment.SetReturnValue(Value((int) fwrite(str.String().c_str(), 1, size.Integer(), fp->FileP())));
    
    return 0;
}

// Reads a string from a file
static ErrorCode FGets(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vfile = runTimeEnvironment.calculationStack.back();
    if (!vfile.IsHandle())
        return IllegalArgument;
    FilePointer *fp = dynamic_cast<FilePointer *>(vfile.Handle());
    if (fp == null || fp->FileP() == null)
        return IllegalArgument;
    char text[256];
    text[0] = '\0';
    fgets(text, sizeof(text), fp->FileP());
    SetReturnValue(runTimeEnvironment, Value(text));
    return 0;
}

// Write a string to a file
static ErrorCode FPuts(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vfile = runTimeEnvironment.calculationStack.back();
    Value &str = runTimeEnvironment.calculationStack.back(1);
    if (!vfile.IsHandle() || !str.IsString())
        return IllegalArgument;
    FilePointer *fp = dynamic_cast<FilePointer *>(vfile.Handle());
    if (fp == null || fp->FileP() == null)
        return IllegalArgument;
    runTimeEnvironment.SetReturnValue(Value(fputs((char *)str.String().c_str(), fp->FileP())));
    
    return 0;
}

























/*************************************************************************/
// Functions which are not and will not be implemented follow (dummies)
// they all give a "info" to standard out, and return 0!
/*************************************************************************/

// call Upgrade
// call Upgrade(1) - silent
// Upgrade the firmware using a file on the disk
static ErrorCode Upgrade(Executive &, RunTimeEnvironment &runTimeEnvironment, int argCount)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'Upgrade()' not supported on PiTrex!\n\r");
    return 0;
}

// call Downgrade - If there's an earlier version in flash, clobber our BFxSEQ
// call Downgrade(1) - Any argument causes us to undo an upgrade we just installed
static ErrorCode Downgrade(Executive &, RunTimeEnvironment &, int argCount)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'Downgrade()' not supported on PiTrex!\n\r");
    return 0;
}

// call PatchCode(address, byteArray)
// Copies the the 16K page that contains the address to RAM, modifies the RAM with
// the patch, erases the page, and re-burns it. The byteArray must fit within the
// page (i.e. not cross a page boundary)
// address is 0xBD000000 to 0xBD1FFFFF
static ErrorCode PatchCode(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'PatchCode()' not supported on PiTrex!\n\r");
    return 0;
}

// To peek at Vectrex memory:
// dim array[2]
// call Peek(addr, length, array)
// 32K <= addr < 64K
// Read Vectrex memory location(s) into the array. This is asynchronous: array is
// not filled in until sometime after this call returns. The array should have two
// elements. The first will be set to 1 when the data becomes valid. The
// second element will be set to an array of integers.
//
// If you're running a BASIC program, you need to call Peek, then WaitForFrame,
// then wait for array[1] to be set to 1
//
// To peek at PIC32 memory:
// value = Peek(addr)
// Address is a valid PIC32MZ2048EFH064 address
//
// To peek at DPRAM:
// value = Peek(addr)
// 0 <= addr < 2K. The read is synchronous
static ErrorCode Peek(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
  static int shown = 0;
  if (!shown++) 
    printf("Function 'Peek()' on PiTrex always return 0!\n\r");

    // If we're peeking at the Vectrex's memory
    if (count == 3)
    {
        Value &varray = runTimeEnvironment.calculationStack.back();
        Value &vlength = runTimeEnvironment.calculationStack.back(1);
        Value &vaddr = runTimeEnvironment.calculationStack.back(2);
        if (!vlength.IsInteger() || !vaddr.IsInteger() || 
            !varray.IsArray() || varray.DimensionsSize() != 1 || varray.Dimensions()[0] != 2 ||
            vlength.Integer() < 1 || vlength.Integer() > 16/*VectrexInterface::PeekBufferSize()*/ )
        {
            return IllegalArgument;
        }

        varray[0] = Value(0);
        uint16_t dims[1] = {uint16_t(vlength.Integer())};
        varray[1] = Value(dims, 1);

        // Set up the peekdata parameters; ServiceVectrex will get the data
        peekArray = varray;
        peekAddress = vaddr.Integer();
        peekLength = vlength.Integer();
        peekRequest = true;

        for (int i = 0; i < peekLength; ++i)
            peekArray[1][i] = 0; // all zeros!
        peekArray[0] = 1;
    }
    
    // Else if we're peeking at the PIC32's memory or the DPRAM
    else if (count == 1)
    {
        runTimeEnvironment.SetReturnValue(Value(0));
    }
    else
        return WrongNumberOfArguments;
    return 0;
}


// Poke a 32 bit word into PIC32 RAM or a byte into DPRAM (address 0-2047)
static ErrorCode Poke(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'Poke()' not supported on PiTrex!\n\r");
    return 0;
}


// Poke a 16 bit value into DPRAM (address 0-2047). Big-endian
static ErrorCode PokeW(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'PokeW()' not supported on PiTrex!\n\r");
    return 0;
}

// value = JSR(address)
// Do a function call to the machine code at the specified address. Returns the
// integer value
// value = JSR(array)
// Write an array of ints to memory and call it as a subroutine
static ErrorCode JSR(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'JSR()' not supported on PiTrex!\n\r");
    return 0;
}

// theoretically we could
// emulate a code section - but we are not going to!
static ErrorCode CodeSprite(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'CodeSprite()' not supported on PiTrex!\n\r");
    return 0;
}
// x = GetFlashChecksum()- returns calculated flash checksum
// x = GetFlashChecksum(1) - returns expected flash checksum
static ErrorCode GetFlashChecksum(Executive &, RunTimeEnvironment &runTimeEnvironment, int argCount)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'GetFlashChecksum()' not supported on PiTrex!\n\r");
    SetReturnValue(runTimeEnvironment, "0x00000000");
    return 0;
}

static ErrorCode GetCompiledSpriteCount(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'GetCompiledSpriteCount()' not supported on PiTrex!\n\r");
    runTimeEnvironment.SetReturnValue(Value(0));
    return 0;
}

static ErrorCode DumpMemory(const unsigned char *start, const unsigned char *end, bool showAddresses, unsigned int startAddress, bool showAscii, bool showHex, bool sectors)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'DumpMemory()' not supported on PiTrex!\n\r");
    return 0;
}

// Dumps the contents of the disk. If there is no parameter, only the hex
// values are printed. If there's a parameter, ASCII gets printed. If the
// parameter is 0, no hex is printed.
static ErrorCode DumpDisk(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'DumpDisk()' not supported on PiTrex!\n\r");
    runTimeEnvironment.SetReturnValue(Value(0));
    return 0;
}

// Dumps the contents of the DPRAM. If there is no parameter, only the hex
// values are printed. If there's a parameter, ASCII gets printed. If the
// parameter is 0, no hex is printed.
static ErrorCode DumpDPRAM(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'DumpDPRAM()' not supported on PiTrex!\n\r");
    runTimeEnvironment.SetReturnValue(Value(0));
    return 0;
}

// Dumps the contents of the DPRAM mirror buffer. The parameter indicates
// how far back in history to go (zero means the current frame, 1 means the previous
// frame, etc. up to the maximum mirror depth)
static ErrorCode DumpDPRAMMirror(Executive &, RunTimeEnvironment &runTimeEnvironment, int count)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'DumpDPRAMMirror()' not supported on PiTrex!\n\r");
    runTimeEnvironment.SetReturnValue(Value(0));
    return 0;
}

// Dumps the contents of memory. 
static ErrorCode DumpMem(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'DumpMem()' not supported on PiTrex!\n\r");
    runTimeEnvironment.SetReturnValue(Value(0));
    return 0;
}

static ErrorCode DumpFlashErrors(Executive &, RunTimeEnvironment &, int)
{
  static int shown = 0;
  if (shown++) return 0;
    printf("Function 'DumpFlashErrors()' not supported on PiTrex!\n\r");
    return 0;
}

















/*************************************************************************/
// Functions which must be implemented follow
/*************************************************************************/







// Creates an Explosion object from four values
// The values are:
//Byte 0 = Tone and noise channel enables
//    Bit 0 = Tone channel #
//     1 = Tone channel #
//     2 = Tone channel #
//     3 = Noise source #
//     4 = Noise source #
//     5 = Noise source #
//Byte 1 = Noise source sweep
//    = 0 ? Sweep frequency UP
//    > 0 ? Sweep frequency DOWN
//    < 0 ? Inhibit frequency sweep
//Byte 2 = Volume sweep
//    = 0 ? Sweep volume UP
//    > 0 ? Sweep volume DOWN
//    < 0 ? Inhibit volume sweep
//Byte 3 = Explosion duration
//    $01 ? Longest explosion duration
//    $80 ? Shortest explosion duration
static ErrorCode Explosion(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
  // TODO
    Value &byte3 = runTimeEnvironment.calculationStack.back();
    Value &byte2 = runTimeEnvironment.calculationStack.back(1);
    Value &byte1 = runTimeEnvironment.calculationStack.back(2);
    Value &byte0 = runTimeEnvironment.calculationStack.back(3);
    if (!byte0.IsInteger() || !byte1.IsInteger() || !byte2.IsInteger() || !byte3.IsInteger())
        return IllegalArgument;

    VectrexMusicPlayer *expl = new VectrexMusicPlayer(byte0.Integer(), byte1.Integer(), byte2.Integer(), byte3.Integer());
    SetReturnValue(runTimeEnvironment, Value(expl));

    return 0;
}

// Returns 1 if music is currently playing
static ErrorCode MusicIsPlaying(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{  
    if (lastYM != 0)
    {
      if (v_isYMPlaying())
      {
        SetReturnValue(runTimeEnvironment, Value(1));
        return 0;
      }
    }

    if (music.IsHandle() && dynamic_cast<VectrexMusicPlayer *>(music.Handle()) != null)
    {
      VectrexMusicPlayer *mplayer = dynamic_cast<VectrexMusicPlayer *>(music.Handle());
      SetReturnValue(runTimeEnvironment, Value(mplayer->isPlaying()?1:0   ));
    }
    else
    {
      SetReturnValue(runTimeEnvironment, Value(0));
    }
    return 0;
}
//#include "VectrexInterface.h"

// call Play(music)
// call Play(nil)
// play(explosion)
static ErrorCode Play(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vmusic = runTimeEnvironment.calculationStack.back();
    v_noSound(); // start fresh!
    // If the argument is not nil
    if (!vmusic.IsUninitialized())
    {
      // if its neither a piece of music (BASIC song)
      // nor a player (built in song)
      // than the argument is illegal!
      if (!vmusic.IsHandle() || 
          ((dynamic_cast<VectrexMusicPlayer *>(vmusic.Handle()) == null)     
          && (dynamic_cast<VMusic *>(vmusic.Handle()) == null)
          && (dynamic_cast<VYMMusic *>(vmusic.Handle()) == null)
          )
         ) return IllegalArgument;

      // a BASIC piece of music is in the handle
      // create a player fpr that and play it
      VMusic *mus = dynamic_cast<VMusic *>(vmusic.Handle());
      if (mus != null)
      {
          v_stopYM();
          lastYM = 0;
          // if there is an old player "active"
          // stop and remove the old player
          if (music.IsHandle() && dynamic_cast<VectrexMusicPlayer *>(music.Handle()) != null)
          {
            VectrexMusicPlayer *oldplayer = dynamic_cast<VectrexMusicPlayer *>(music.Handle());
            oldplayer->stop();
            music.Clear();
          }
          
          // create a new player and start playing
          VectrexMusicPlayer *mplayer = new VectrexMusicPlayer(mus);
          mplayer->setPlaying(true);
          music = mplayer;
          musicChanged = true;
          return 0;
      }
      
      // this is a ym piece of music
      VYMMusic *vym = dynamic_cast<VYMMusic *>(vmusic.Handle());
      if (vym != null)
      {
          // if there is an old player "active"
          // stop and remove the old player
          if (music.IsHandle() && dynamic_cast<VectrexMusicPlayer *>(music.Handle()) != null)
          {
            VectrexMusicPlayer *oldplayer = dynamic_cast<VectrexMusicPlayer *>(music.Handle());
            oldplayer->stop();
            music.Clear();
          }
          v_stopYM();
          lastYM = 0;

          if (vym->isLoaded())
          {
            v_initYM((unsigned char *)vym->getData(), vym->getSize(), 0); // 0 is not loop, 1 is loop      
            lastYM = vym;
          }
          return 0;
      }
      
      // the handle is a music player (built in music)
      if (music.IsHandle() && dynamic_cast<VectrexMusicPlayer *>(music.Handle()) != null)
      {
        VectrexMusicPlayer *oldplayer = dynamic_cast<VectrexMusicPlayer *>(music.Handle());
        VectrexMusicPlayer *newplayer = dynamic_cast<VectrexMusicPlayer *>(vmusic.Handle());
        // if there is an old player, remmove it

        oldplayer->stop();
        if (oldplayer !=newplayer)
        {
          music.Clear();
        }
      }
    }
    else
    {
      // new value is nil
      if (music.IsHandle() && dynamic_cast<VectrexMusicPlayer *>(music.Handle()) != null)
      {
        VectrexMusicPlayer *oldplayer = dynamic_cast<VectrexMusicPlayer *>(music.Handle());
        oldplayer->stop();
        music.Clear();
      }
      
      if (lastYM != 0)
      {
          v_stopYM();
          lastYM = 0;
      }
    }
    
    music = vmusic;
    musicChanged = true;

    if (music.IsHandle() && dynamic_cast<VectrexMusicPlayer *>(music.Handle()) != null)
    {
      VectrexMusicPlayer *mplayer = dynamic_cast<VectrexMusicPlayer *>(music.Handle());
      mplayer->setPlaying(true);
      v_stopYM();
      lastYM = 0;
    }
    return 0;
}
// Playsample(sample)
static ErrorCode PlaySample(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vsample = runTimeEnvironment.calculationStack.back();
    if (vsample.IsUninitialized())
    {
       v_stopSamplePlaying();
       return 0;
    }

    // if its neither a piece of music (BASIC song)
    // nor a player (built in song)
    // than the argument is illegal!
    if (!vsample.IsHandle() || (dynamic_cast<VSample *>(vsample.Handle()) == null)     )
        return IllegalArgument;

    VSample *sample = dynamic_cast<VSample *>(vsample.Handle());
    if (sample == null)
        return IllegalArgument;

    v_playIRQSample((char*)sample->getData(), sample-> getSize(), sample-> getRate(), PLAY_ONCE);
    
    return 0;
}
// sfx the AYSFX data
// channel 1,2,3 of PSG
static ErrorCode PlayAYSFX(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &channel = runTimeEnvironment.calculationStack.back();
    Value &sfx = runTimeEnvironment.calculationStack.back(1);
    
    if (!channel.IsInteger()) return IllegalArgument;
    if (!sfx.IsHandle() || (dynamic_cast<VAYSFX *>(sfx.Handle()) == null)) return IllegalArgument;
    VAYSFX *aysfx = dynamic_cast<VAYSFX *>(sfx.Handle());
    
    unsigned char* data = aysfx->getData();
    if (data != 0)
    {
      v_playSFXStart(data, channel.Integer()-1, 0); // 0 is once, != 0 isloops, not available as of now from basic
    }

    SetReturnValue(runTimeEnvironment, Value(0));
    return 0;
}


//extern VMusic IntroMusic;
// Creates a Music object from a 2D array
// Each row of the array is a {note, duration} pair. The note can be up to three
// channels synthesized with the ABC function
static ErrorCode Music(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &vnotes = runTimeEnvironment.calculationStack.back();
    if (!vnotes.IsArray() || vnotes.DimensionsSize() != 2 || vnotes.Dimensions()[1] != 2)
        return IllegalArgument;
    
    VectrexMusicPlayer *mplayer = new VectrexMusicPlayer(vnotes);
    SetReturnValue(runTimeEnvironment, Value(mplayer));
    return 0;
}

static ErrorCode Sample(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &rate = runTimeEnvironment.calculationStack.back();
    Value &filename = runTimeEnvironment.calculationStack.back(1);
    if (!rate.IsInteger()) return IllegalArgument;
    if (!filename.IsString()) return IllegalArgument;
    
    VSample *sample = new VSample((unsigned char*)filename.String().c_str(), rate.Integer());

    if (!sample->isLoaded())
    {
      printf("Sample error - sample not loaded!\n\r");
      delete sample;
      return IllegalFileName;
    }
    SetReturnValue(runTimeEnvironment, Value(sample));
    return 0;
}

static ErrorCode YMMusic(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &filename = runTimeEnvironment.calculationStack.back(0);
    if (!filename.IsString()) return IllegalArgument;
    
    VYMMusic *music = new VYMMusic((unsigned char*)filename.String().c_str());

    if (!music->isLoaded())
    {
      printf("YMMusic error - ym not loaded!\n\r");
      delete music;
      return IllegalFileName;
    }
    SetReturnValue(runTimeEnvironment, Value(music));
    return 0;
}

static ErrorCode AYSFX(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    Value &aysfxData = runTimeEnvironment.calculationStack.back();

    if (!aysfxData.IsArray() || aysfxData.DimensionsSize() != 1)
        return IllegalArgument;
    
    VAYSFX *aysfx = new VAYSFX(aysfxData);
    SetReturnValue(runTimeEnvironment, Value(aysfx));
    return 0;
}
static ErrorCode BreakOnControlC(Executive &, RunTimeEnvironment &runTimeEnvironment, int)
{
    printf("Function 'BreakOnControlC()' is not correctly implemented on PiTrex!\n\r");
    Value &arg = runTimeEnvironment.calculationStack.back();
    if (!arg.IsInteger())
        return IllegalArgument;
    breakOnControlC = arg.Integer() != 0;
    return 0;
}


void RegisterVectrexBuiltInFunctions()
{
// THIS LIST MUST BE IN ALPHABETICAL ORDER (case-insensitive)
// THIS LIST MUST BE IN ALPHABETICAL ORDER (case-insensitive)
static const BuiltInFunctionInfo builtInFunctions[] =
{
    {"ABC", ABC, 1, 3},
    {"AYSFX", AYSFX, 1},
    {"BreakOnControlC", BreakOnControlC, 1},
    {"CameraGetFocalLength", CameraGetFocalLength, 0},
    {"CameraGetRotation", CameraGetRotation, 0},
    {"CameraRotate", CameraRotate, 3},
    {"CameraSet3D", CameraSet3D, 2},
    {"CameraSetFocalLength", CameraSetFocalLength, 1},
    {"CameraSetRotation", CameraSetRotation, 3},
    {"CameraTranslate", CameraTranslate, 1},
    {"ClearScreen", ClearScreen, 0},
    {"CodeSprite", CodeSprite, 1},
    {"Distance", Distance, 2},
    {"DotsSprite", DotsSprite, 1},
    {"Downgrade", Downgrade, 0, 1},
    {"DrawingListSprite", DrawingListSprite, 1},
    {"DumpDisk", DumpDisk, 0, 1},
    {"DumpDPRAM", DumpDPRAM, 0, 1},
    {"DumpDPRAMMirror", DumpDPRAMMirror, 1, 1},
    {"DumpFlashErrors", DumpFlashErrors, 0},
    {"DumpMem", DumpMem, 2},
    {"DumpSprite", DumpSprite, 1},
    {"DumpSprites", DumpSprites, 0},
    {"Explosion", Explosion, 4},
    {"FClose", FClose, 1},
    {"FEOF", FEOF, 1},
    {"FGetc", FGetc, 1},
    {"FGets", FGets, 1},
    {"FOpen", FOpen, 2},
    {"FPuts", FPuts, 2},
    {"FRead", FRead, 2},
    {"FSeek", FSeek, 3},
    {"FTell", FTell, 1},
    {"FWrite", FWrite, 3},
    {"GetCompiledSpriteCount", GetCompiledSpriteCount, 0},
    {"GetFlashChecksum", GetFlashChecksum, 0, 1},
    {"GetFontMetrics", GetFontMetrics, 0},
    {"GetFrameRate", GetFrameRate, 0},
    {"GetTickCount", GetTickCount, 0},
    {"IntensitySprite", IntensitySprite, 1},
    {"JSR", JSR, 1},
    {"Lines25DSprite", Lines25DSprite, 1},
    {"Lines3DSprite", Lines3DSprite, 1},
    {"LinesSprite", LinesSprite, 1},
    {"MoveSprite", MoveSprite, 2},
    {"Music", Music, 1},
    {"MusicIsPlaying", MusicIsPlaying, 0},
    {"Offset", Offset, 3, 4},
    {"PatchCode", PatchCode, 2},
    {"Peek", Peek, 1, 3},
    {"Play", Play, 1},
    {"PlayAYSFX", PlayAYSFX, 2},
    {"PlaySample", PlaySample, 1},
    {"Poke", Poke, 2},
    {"PokeW", PokeW, 2},
    {"PtInRect", PtInRect, 2, 6},
    {"PutSpriteAfter", PutSpriteAfter, 2},
    {"PutSpriteBefore", PutSpriteBefore, 2},
    {"RegularPolygon", RegularPolygon, 2, 3},
    {"Remove", Remove, 1}, // delete file
    {"RemoveSprite", RemoveSprite, 1},
    {"Rename", Rename, 2}, // rename file
    {"ReturnToOriginSprite", ReturnToOriginSprite, 0},
    {"Sample", Sample, 2},
    {"ScaleSprite", ScaleSprite, 1, 2},
    {"SetFrameRate", SetFrameRate, 1},
    {"Sound", Sound, 1},
    {"SpriteClip", SpriteClip, 2},
    {"SpriteEnable", SpriteEnable, 2},
    {"SpriteGetMagnification", SpriteGetMagnification, 1},
    {"SpriteGetRotation", SpriteGetRotation, 1},
    {"SpriteGetUserData", SpriteGetUserData, 1},
    {"SpriteIntensity", SpriteIntensity, 2},
    {"SpriteMove", SpriteMove, 3},
    {"SpritePrintVectors", SpritePrintVectors, 1},
    {"SpriteRotate", SpriteRotate, 2, 4},
    {"SpriteScale", SpriteScale, 2, 3},
    {"SpriteSetData", SpriteSetData, 2},
    {"SpriteSetMagnification", SpriteSetMagnification, 2, 4},
    {"SpriteSetRotation", SpriteSetRotation, 2, 4},
    {"SpriteSetUserData", SpriteSetUserData, 2},
    {"SpriteTranslate", SpriteTranslate, 2},
    {"Text2ListSprite", Text2ListSprite, 1},
    {"Text2Sprite", Text2Sprite, 1},
    {"TextListSprite", TextListSprite, 1},
    {"TextSizeSprite", TextSizeSprite, 1},
    {"TextSprite", TextSprite, 1},
    {"TextToLines", TextToLines, 1},
    {"Upgrade", Upgrade, 0, 1},
    {"VectorRotate", VectorRotate, 2, 4},
    {"Version", Version, 0},
    {"WaitForFrame", WaitForFrame, 3, 4},
    {"YMMusic", YMMusic, 1},
};
      RegisterBuiltInFunctions(builtInFunctions, countof(builtInFunctions));
  
}

/*
// Define a class that, when instantiated, registers the built in functions.
// Then instantiate it. This way, the functions get registered without app code
// having to explicitly call a method.
class AutoRegisterFunctions
{
public:
    AutoRegisterFunctions() 
    {
printf("AUTOREGISTER...\n\r");
      RegisterBuiltInFunctions(builtInFunctions, countof(builtInFunctions));
    }
};

AutoRegisterFunctions _autoRegisterFunctions;
*/