/* 
 * File:   Camera.h
 * Author: Bob
 *
 * Created on October 10, 2016, 8:27 AM
 */

#ifndef CAMERA_H
#define	CAMERA_H

#include "../Value.h"

typedef enum {BOTH_OPEN, LEFT_OPEN, RIGHT_OPEN} ShutterControl;

class Camera : public GSDataObject
{
public:
    Camera();
    
    void Rotate(float pitch, float roll, float yaw)
    {
        _angles[0] += pitch; 
        _angles[1] += roll; 
        _angles[2] += yaw; 
    }
    const float *GetRotation() const {return _angles;}
    void SetRotation(float pitch, float roll, float yaw) 
    {
        _angles[0] = pitch; 
        _angles[1] = roll; 
        _angles[2] = yaw; 
    }
    
    void SetTranslation(const Value &v) {_translate = v;}
    const Value &GetTranslation() const {return _translate;}
    
    void SetFocalLength(float f) {_focalLength = f;}
    float GetFocalLength() const {return _focalLength;}
    
    // The number to multiply World dimensions by to get Vectrex units
    // (Depends on the world units the user is using, like meters or feet,
    // and the Vectrex scale he's using)
    void SetToUnits(float toUnits) {_toUnits = toUnits;
    }
    float GetToUnits() const {return _toUnits;}
    
    void Set3D(float ocularDistance, float convergence)
    {
        _ocularDistance = ocularDistance;
        _convergence = convergence;
        _convergence2DOffset = -1;
    }
    bool Is3D() const {return _ocularDistance != 0;}
    float GetOcularDistance() const {return _ocularDistance;}
    float GetConvergenceDistance() const {return _convergence;}
    void SetEyeSelection(ShutterControl eyeSelection) {_eyeSelection = eyeSelection;}
    ShutterControl GetEyeSelection() const {return _eyeSelection;}
    
    void SetGlassesDetected(bool detected) {_glassesDetected = detected;}
    bool GlassesDetected() const {return _glassesDetected;}

    // When we calculate how much to shift the 2D projected image in order to
    // do convergence, cache it here.
    bool IsValidConvergence2DOffset() const {return _convergence2DOffset >= 0;}
    void SetConvergence2DOffset(float convergence2DOffset) {_convergence2DOffset = convergence2DOffset;}
    float GetConvergence2DOffset() const {return _convergence2DOffset;}
    
    void Reset();
    
private:
    Camera(const Camera& orig); // unimplemented
    
    float _angles[3]; // pitch, roll, yaw
    Value _translate;
    float _focalLength;
    float _toUnits;
    float _ocularDistance, _convergence;
    ShutterControl _eyeSelection;
    float _convergence2DOffset;
    
    bool _glassesDetected;
};

extern Camera camera;

#endif	/* CAMERA_H */

