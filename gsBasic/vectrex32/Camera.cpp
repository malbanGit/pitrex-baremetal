/* 
 * File:   Camera.cpp
 * Author: Bob
 * 
 * Created on October 10, 2016, 8:27 AM
 */

#include "Camera.h"

Camera camera;

Camera::Camera()
{
    Reset();
}

void Camera::Reset()
{
    _focalLength = 50;
    // Assume Vectrex scale 64, so 324 Vectrex units draws a line 0.097 meters long on the screen
    _toUnits = float(324 / 0.097);
    _angles[0] = 0;
    _angles[1] = 0;
    _angles[2] = 0; // Pointed into the screen
    _ocularDistance = 0;
    _convergence = 0;
    _eyeSelection = BOTH_OPEN;
    _convergence2DOffset = -1;
    _glassesDetected = false;
}
