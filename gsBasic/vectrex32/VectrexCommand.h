/* 
 * File:   VectrexCommand.h
 * Author: Bob
 *
 * Created on March 15, 2016, 2:50 PM
 */

#ifndef VECTREXCOMMAND_H
#define VECTREXCOMMAND_H
#include <math.h>

#include "../Array.h"
#include "../GSList.h"
#include <stdint.h>
#include <algorithm>
#include "../Value.h"
#include "../Utilities.h"
#include "Calculations.h"
#include "Matrix.h"
//#include "6809biosInterface.h"


class ExecutableWriter;
class VectrexInterface;

extern ErrorCode ModeIsNotInteger;
extern ErrorCode CoordinatesAreNotNumeric;
extern ErrorCode HugeDelta;
extern ErrorCode InvalidMagnification;

extern void ToHex(char *out, int value);

class VectrexCommand : public GSDataObject
{
public:
    VectrexCommand() : _enabled(true) {}
    VectrexCommand(bool enabled) : _enabled(enabled) {}
    virtual ~VectrexCommand() {}
    
    static void ClearError() {_errorCode = 0;}
    static void SetError(ErrorCode code);
    static ErrorCode GetError() {return _errorCode;}
    static const VectrexCommand *GetHugeDeltaSprite() {return _hugeDeltaSprite;}
    static void ClearHugeDeltaSprite() {_hugeDeltaSprite = null;}
    
    virtual void Compile(bool eliminateTrailingMove) const = 0;
    bool Enabled() const {return _enabled;}
    void Enable(bool enable) {_enabled = enable;}
    
    const Value &GetUserData() const {return _userData;}
    void SetUserData(const Value &d) {_userData = d;}
    
    virtual void Dump() const = 0;
   
protected:
    void DumpHeader(const char *commandType) const;
    
private:
    VectrexCommand(const VectrexCommand& orig); // unimplemented
    bool _enabled;
    Value _userData;
    
protected:
    static ErrorCode _errorCode;
    static const VectrexCommand *_hugeDeltaSprite;
};

class VectrexCommandIntensity : public VectrexCommand
{
public:
    VectrexCommandIntensity(int intensity) : _intensity(intensity) {}
    
    virtual void Compile(bool eliminateTrailingMove) const;
    virtual void Dump() const;

    void Intensity(int intensity) {_intensity = intensity;}
    
private:
    int _intensity;
};

class VectrexCommandMove : public VectrexCommand
{
public:
    VectrexCommandMove(int x, int y) : _x(x), _y(y) {}
    
    virtual void Compile(bool eliminateTrailingMove) const;
    virtual void Dump() const;

    void Move(int x, int y) {_x = x; _y = y;}
    
private:
    int _x, _y;
};

class VectrexCommandPrintStr : public VectrexCommand
{
public:
    VectrexCommandPrintStr(const char *text);
    ~VectrexCommandPrintStr() {free(_text);}
    
    virtual void Compile(bool eliminateTrailingMove) const;
    virtual void Dump() const;

private:
    char *_text;
    size_t _len;
};

/**
 * Each row in the text array has {x, y, text}.
 */
class VectrexCommandPrintList : public VectrexCommand
{
public:
    VectrexCommandPrintList(const Value &texts) : _texts(texts) {}
    
    virtual void Compile(bool eliminateTrailingMove) const;
    virtual void Dump() const;

private:
    Value _texts;
};

/**
 * Each row in the text array has {x, y, text}.
 */
class VectrexCommandTextSize : public VectrexCommand
{
public:
    VectrexCommandTextSize(const Value &textSize) : _textSize(textSize) {}
    
    virtual void Compile(bool eliminateTrailingMove) const;
    virtual void Dump() const;
    
private:
    Value _textSize;
};

class VectrexCommandReset0Ref : public VectrexCommand
{
public:
    virtual void Compile(bool eliminateTrailingMove) const;
    virtual void Dump() const;
};

class VectrexCommandScale : public VectrexCommand
{
public:
    VectrexCommandScale(int scale, float ratio) : _scale(scale), _ratio(ratio) {}
    
    virtual void Compile(bool eliminateTrailingMove) const;
    virtual void Dump() const;

    void Scale(int scale, float ratio) {_scale = scale; _ratio = ratio;}
    
private:
    int _scale;
    // The ratio between World coordinates and length on the screen at the given scale
    // For example, if the scale is 64 and the World coordinates are in meters, a line 
    // that's 324 units long is about 0.097 meters long on the screen. So ratio would
    // be 324 / 0.097
    float _ratio;
};

/**
 * If the array's second dimension is 2, each row is (x, y). If the second dimension
 * is 3, each row is (extraData, x, y)
 */
class VectrexCommandTransformable : public VectrexCommand
{
protected:
    VectrexCommandTransformable(const Value &data) : 
        _value(data)
    {
       _magnifications[0] = _magnifications[1] = _magnifications[2] = 1.0;
    }
        
public:
    void SetClippingRect(const Value &v) {_clippingRect = v;}
    
    void GetMagnification(float *x, float *y, float *z) const 
    {
        *x = _magnifications[0];
        *y = _magnifications[1];
        *z = _magnifications[2];
    }
    void Magnification(float magX, float magY, float magZ) 
    {
        _magnifications[0] = magX;
        _magnifications[1] = magY;
        _magnifications[2] = magZ;
    }
    
    void SetTranslation(const Value &v) {_translate = v;}
    
    virtual void GetVectrexData(Array<int32_t> *data, size_t *count, bool lines, bool eliminateTrailingMove, bool print = false) const = 0;

    template <class T, int dimension>
    struct ModePoint
    {
        int mode; // 0 is MoveTo, non-zero is DrawTo
        T coordinates[dimension];
    };
    
    ErrorCode SetData(Value &newData)
    {
        if (newData.DimensionsSize() != _value.DimensionsSize())
            return IllegalArgument;
        const uint16_t *newDimensions = newData.Dimensions();
        const uint16_t *oldDimensions = _value.Dimensions();
        if (oldDimensions[1] != newDimensions[1])
            return IllegalArgument;
        _value = newData;
        return 0;
    }
    
protected:
    template <class T>
    static bool PtInRect(const T rect[2][2], T x, T y)
    {
        return rect[0][0] <= x && rect[1][0] >= x && rect[0][1] <= y && rect[1][1] >= y;
    }
    template <class T>
    static void NormalizeRect(T rect[2][2])
    {
        if (rect[0][0] > rect[1][0]) std::swap(rect[0][0], rect[1][0]);
        if (rect[0][1] > rect[1][1]) std::swap(rect[0][1], rect[1][1]);
    }

    // forceFirstMove makes sure the first vertex is a Move; we insert a move to (0, 0) if necessary
    template <class T, int dimension>
    void GetRawData(Array<ModePoint<T, dimension> > *rawData, bool forceFirstMove = false) const
    {
        size_t totalRows = _value.Dimensions()[0];
        size_t width = _value.Dimensions()[1];
        Array<ModePoint<T, dimension> > &modePoints = *rawData;
        modePoints.reserve(totalRows * 2);
        modePoints.resize(totalRows);

        // The array of points might have a mode as the first element of each row.
        // The x and y coordinates will be the last two elements in the row
        bool dataIncludesMode = (width > dimension);

        // Read the values into modePoints
        size_t destIndex = 0, srcIndex = 0;
        ErrorCode error = 0;
        for (size_t i = 0; i < totalRows && error == 0; ++i)
        {
            if (dataIncludesMode)
            {
                error = _value[srcIndex].TypeBehaviors()->ToInteger(_value[srcIndex], &modePoints[destIndex].mode);
                if (error)
                {
                    VectrexCommand::SetError(ModeIsNotInteger);
                    return;
                }
                ++srcIndex;
            }
            else
                modePoints[destIndex].mode = 1;
            
            // If we need to force the first vertex to be a Move
            if (forceFirstMove && i == 0 && modePoints[destIndex].mode)
            {
                modePoints[destIndex].mode = 0;
                modePoints[destIndex].coordinates[0] = modePoints[destIndex].coordinates[1] =
                    modePoints[destIndex].coordinates[2] = 0;
                ++destIndex;
                modePoints[destIndex].mode = 1;
            }
            
            for (int j = 0; j < dimension; ++j)
            {
                error = _value[srcIndex].TypeBehaviors()->ToValue(_value[srcIndex], &modePoints[destIndex].coordinates[j]);
                if (error)
                {
                    VectrexCommand::SetError(CoordinatesAreNotNumeric);
                    return;
                }
                ++srcIndex;
            }
            ++destIndex;
        }
    }
    
    template <class T, int dimension>
    void Magnify(Array<ModePoint<T, dimension> > &modePoints) const
    {
        if (_magnifications[0] != 1.0 || _magnifications[1] != 1.0 || (dimension == 3 && _magnifications[2] != 1.0))
        {
            for (auto modePoint = modePoints.begin(); modePoint != modePoints.end(); ++modePoint)
            {
                for (int i = 0; i < dimension; ++i)
                    modePoint->coordinates[i] = T(modePoint->coordinates[i] * _magnifications[i]);
            }
        }
    }
   
    template <class T, int dimension>
    void Translate(Array<ModePoint<T, dimension> > &modePoints, const T *translation) const
    {
        for (auto modePoint = modePoints.begin(); modePoint != modePoints.end(); ++modePoint)
        {
            for (int i = 0; i < dimension; ++i)
                modePoint->coordinates[i] += translation[i];
        }
    }
    
    template <class T, int dimension>
    void Translate(Array<ModePoint<T, dimension> > &modePoints) const
    {
        if (_translate.IsArray() && _translate.DimensionsSize() == 1 && _translate.Dimensions()[0] == dimension)
        {
            T translate[dimension];
            if (_translate.GetArrayValues((T *) translate) == 0)
            {
                Translate(modePoints, translate);
            }
        }
    }

    template <class T>
    void Clip(Array<ModePoint<T, 2> > &modePoints, bool lines) const
    {
        Clip(modePoints, _clippingRect, lines);
    }

    template <class T>
    void Clip(Array<ModePoint<T, 2> > &modePoints, const Value &vclippingRect, bool lines) const
    {
        T clippingRect[2][2];
        bool clipping = (vclippingRect.GetArrayValues((T *) clippingRect) == 0);
        if (clipping)
        {
            NormalizeRect(clippingRect);
            // If we're drawing dots
            if (!lines)
            {
                size_t srcIndex = 0;
                // Find the first point that needs to be clipped
                while (srcIndex < modePoints.size() && PtInRect(clippingRect, modePoints[srcIndex].coordinates[0], modePoints[srcIndex].coordinates[1]))
                    ++srcIndex;
                // Remove clipped points from the array
                int destIndex = srcIndex;
                ++srcIndex;
                while (srcIndex < modePoints.size())
                {
                    if (PtInRect(clippingRect, modePoints[srcIndex].coordinates[0], modePoints[srcIndex].coordinates[1]))
                        modePoints[destIndex++] = modePoints[srcIndex];
                    ++srcIndex;
                }
                modePoints.resize(destIndex);
            }

            // Else (we're drawing lines)
            else
            {
                T lastX = 0, lastY = 0;
                for (size_t i = 0; i < modePoints.size(); ++i)
                {
                    // If we're supposed to draw to this point
                    if (modePoints[i].mode)
                    {
                        double x0clip, y0clip, x1clip, y1clip;
                        bool draw = Calculations::LiangBarsky(clippingRect[0][0], clippingRect[1][0], clippingRect[0][1], clippingRect[1][1],
                                                lastX, lastY, modePoints[i].coordinates[0], modePoints[i].coordinates[1],
                                                x0clip, y0clip, x1clip, y1clip);

                        if (draw)
                        {
                            // If the beginning of the line has been clipped, we need to do a move
                            // to the start of the line segment
                            if (x0clip != lastX || y0clip != lastY)
                            {
                                // If the previous point was a MoveTo, we can just change its endpoint
                                if (i > 0 && modePoints[i - 1].mode == 0)
                                {
                                    modePoints[i - 1].coordinates[0] = T(x0clip);
                                    modePoints[i - 1].coordinates[1] = T(y0clip);
                                }
                                // Else we need to insert a move
                                else
                                {
                                    ModePoint<T, 2> newMove = {0, {T(x0clip), T(y0clip)}};
                                    modePoints.insert(modePoints.begin() + i, newMove);
                                    ++i;
                                }
                            }
                            // If the end of the line has been clipped
                            if (x1clip != modePoints[i].coordinates[0] || y1clip != modePoints[i].coordinates[1])
                            {
                                // Draw to the end of the clipped segment
                                ModePoint<T, 2> lineSegment = {1, {T(x1clip), T(y1clip)}};
                                modePoints.insert(modePoints.begin() + i, lineSegment);
                                ++i;
                                // And then move to the original endpoint
                                modePoints[i].mode = 0;
                            }
                        }
                        // Else (the whole line is clipped) move to the new point so we're
                        // in position for future draws
                        else
                        {
							// if the previous vertex was a MoveTo
							if (i > 0 && modePoints[i - 1].mode == 0)
							{
								// We can delete it
								modePoints.erase(modePoints.begin() + i - 1);
								--i;
							}

							modePoints[i].mode = 0;
                        }
                    }

					// Else (we're moving to this point)
					else
					{
						// if the previous vertex was a MoveTo
						if (i > 0 && modePoints[i - 1].mode == 0)
						{
							// We can delete it
							modePoints.erase(modePoints.begin() + i - 1);
							--i;
						}
					}

                    lastX = modePoints[i].coordinates[0];
                    lastY = modePoints[i].coordinates[1];
                }
            }
        }
    }
    
    template <class T, int dimension>
    void PrintRawData(Array<ModePoint<T, dimension> > &modePoints) const
    {
        for (auto i = modePoints.begin(); i != modePoints.end(); ++i)
        {
            fputs(i->mode? "mode = DrawTo" : "mode = MoveTo", stdout);
            Print(stdout, ", x = ", i->coordinates[0], ", y = ", i->coordinates[1]);
            if (dimension == 3)
                Print(stdout, ", z = ", i->coordinates[2]);
            Print(stdout, "\r\n");
        }
    }
    
    template <class T>
    void ToVectrexCoordinates(Array<ModePoint<T, 2> > &modePoints, Array<int32_t> *data, 
        size_t *count, bool lines, bool eliminateTrailingMove, bool print) const
    {
        volatile int index;
        index = 0;
        *count = 0;

        // Change absolute coordinates to relative coordinates and compile them
        // to a Vectrex-compatible array
        data->resize(0);
        size_t size = modePoints.size() * (lines ? 3 : 2);
        data->reserve(size);
        T lastX = 0;
        T lastY = 0;
        for (auto i = modePoints.begin(); i != modePoints.end(); ++i, ++index)
        {
            // If the current point is a MoveTo
            if (i->mode == 0)
            {
                // If this is the last point in the array and we're eliminating trailing moves
                if (i + 1 == modePoints.end())
                {
                    if (eliminateTrailingMove)
                        continue;
                }
                // Else (not the last point) if the next point is also a MoveTo, 
                // we can ignore the current point 
                else if ((i + 1)->mode == 0)
                    continue;
            }
            T curX = i->coordinates[0], curY = i->coordinates[1];
            uint8_t mode = i->mode == 1 ? 0xff : i->mode;
            int diffX = int(i->coordinates[0] - lastX);
            int diffY = int(i->coordinates[1] - lastY);
/*            
            // If any of the coordinates are too large to pass to the 6809
            if (diffX > 127 || diffX < -128 || diffY > 127 || diffY < -128)
            {
                // For lines, if there's one that's too long we can insert shorter lines to 
                // kludge it
                if (lines)
                {
                    int divisor = (std::max(abs(diffX), abs(diffY)) + 126) / 127;
                    if (divisor > 50)
                    {
                        _hugeDeltaSprite = this;
                        VectrexCommand::SetError(HugeDelta);
                        return;
                    }
                    
                    int xStep = diffX / divisor, yStep = diffY / divisor;
                    while (diffX > 127 || diffX < -128 || diffY > 127 || diffY < -128)
                    {
                        if (lines)
                            data->push_back(mode);
                        data->push_back(uint8_t(yStep));
                        data->push_back(uint8_t(xStep));
                        ++*count;
                        diffX -= xStep;
                        diffY -= yStep;
                    }
                    
                }
                
                // Else (not lines; dots) we can't insert more dots
                else
                {
                    _hugeDeltaSprite = this;
                    VectrexCommand::SetError(HugeDelta);
                    return;
                }
            }
*/            
            if (lines)
                data->push_back(mode);
            data->push_back(int32_t(diffY));
            data->push_back(int32_t(diffX));
            ++*count;
            lastX = curX;
            lastY = curY;
        }
        
        if (print)
        {
            puts("Vectrex coordinates:");
            int32_t *p = data->begin();
            size_t rows = data->size() / 3;
            while (rows--)
            {
                static char coordText[] = "x = 0x  , y = 0x  \r\n";
                if (lines)
                {
                    static char modeText[] = "mode = 0x  , ";
                    ToHex(modeText + 9, *p++);
                    fputs(modeText, stdout);
                }
                ToHex(coordText + 16, *p++);
                ToHex(coordText + 6, *p++);
                fputs(coordText, stdout);
            }
        }
    }

    Value _value;
    float _magnifications[3];
    Value _translate;
    Value _clippingRect;
};

class VectrexCommandTransformable2D : public VectrexCommandTransformable
{
protected:
    VectrexCommandTransformable2D(const Value &data) : 
        VectrexCommandTransformable(data), _angle(0)
    {}
    
public:        
    void Rotate(float angle);
    float GetRotation() const;
    void SetRotation(float angle) {_angle = angle; }
    
    template <class T>
    void Rotate(Array<ModePoint<T, 2> > &modePoints) const
    {
        if (_angle)
        {
            float rads = float(_angle * 3.1415926 / 180);
            float cos = cosf(rads);
            float sin = sinf(rads);
            for (auto i = modePoints.begin(); i != modePoints.end(); ++i)
            {
                float tempx = float(i->coordinates[0] * cos - i->coordinates[1] * sin);
                i->coordinates[1] = T(i->coordinates[0] * sin + i->coordinates[1] * cos);
                i->coordinates[0] = T(tempx);
            }
        }
    }
    
    void GetVectrexData(Array<int32_t> *data, size_t *count, bool lines, bool eliminateTrailingMove, bool print = false) const;
    
    void DumpHeader(const char *spriteType) const;
    
private:
    float _angle;
};

class VectrexCommandDots : public VectrexCommandTransformable2D
{
public:
    VectrexCommandDots(const Value &dots) : VectrexCommandTransformable2D(dots) {}
    
    virtual void Compile(bool eliminateTrailingMove) const;
    virtual void Dump() const;
};

class VectrexCommandLines : public VectrexCommandTransformable2D
{
public:
    // lines is a 2D array, with the second dimension having three items:
    //    mode - 0 means move to, non-zero means draw to
    //    x
    //    y
    VectrexCommandLines(const Value &lines) : VectrexCommandTransformable2D(lines) {}
    
    virtual void Compile(bool eliminateTrailingMove) const;
    virtual void Dump() const;
};

class VectrexCommandTransformable3D : public VectrexCommandTransformable
{
protected:
    VectrexCommandTransformable3D(const Value &data) : 
        VectrexCommandTransformable(data)
    {
        _angles[0] = _angles[1] = _angles[2] = 0.0;
    }
    
public:        
    void Rotate(float pitch, float roll, float yaw);
    const float *GetRotation() const {((VectrexCommandTransformable3D *) this)->NormalizeAngles(); return _angles;}
    void SetRotation(float pitch, float roll, float yaw) 
    {
        _angles[0] = pitch; 
        _angles[1] = roll; 
        _angles[2] = yaw; 
    }
    
    void ZClipping(const Array<ModePoint<float, 3> > &modePoints, Array<ModePoint<float, 3> > &newModePoints) const;
    void ZeroCrossingPoint(const ModePoint<float, 3> &a, const ModePoint<float, 3> &b, ModePoint<float, 3> *result) const;
    
    void GetVectrexData(Array<int32_t> *data, size_t *count, bool lines, bool eliminateTrailingMove, bool print = false) const;
    
    void DumpHeader(const char *spriteType) const;
    
protected:   
    void GetProjectedVectrexData(
        bool lines, bool print, Array<ModePoint<float, 2> > &projectedData) const;
    
private:
    Matrix GetMagnificationMatrix(const float *mags) const;
    Matrix GetRotationMatrix(const float *angles) const;
    Matrix GetTranslationMatrix(float *translate) const;
    
    void NormalizeAngles();
    
    void Project2D(const Array<ModePoint<float, 3> > &modePoints, Array<ModePoint<float, 2> > &projection) const;
    Matrix GetCameraRotationMatrix(const float *angles) const;

    float _angles[3]; // pitch, roll, yaw
};

class VectrexCommandLines3D : public VectrexCommandTransformable3D
{
public:
    // lines is a 2D array, with the second dimension having four items:
    //    mode - 0 means move to, non-zero means draw to
    //    x
    //    y
    //    z
    VectrexCommandLines3D(const Value &lines) : VectrexCommandTransformable3D(lines) {}
    
    virtual void Compile(bool eliminateTrailingMove) const;
    virtual void Dump() const;
};

class VectrexCommandLines25D : public VectrexCommandTransformable3D
{
public:
    // lines is a 2D array, with the second dimension having three items:
    //    mode - 0 means move to, non-zero means draw to
    //    x
    //    y
    VectrexCommandLines25D(const Value &lines);
    
    virtual void Compile(bool eliminateTrailingMove) const;

    void GetVectrexData(Array<int32_t> *data, size_t *count, bool lines, bool eliminateTrailingMove, bool print = false) const
    {
        ((VectrexCommandLines25D *) this)->GetVectrexData(data, count, lines, eliminateTrailingMove, print);
    }
    virtual void Dump() const;

private:
    void GetVectrexData(Array<int32_t> *data, size_t *count, bool lines, bool eliminateTrailingMove, bool print);
    static const Value &Dummy3DLines();
    static uint16_t _dims2[1];
    VectrexCommandLines _2DLines;
    Value _translation;
};

// Draws text with a vector font
class VectrexCommandPrint2Str : public VectrexCommand
{
public:
    VectrexCommandPrint2Str(const char *text);
    
    virtual void Compile(bool eliminateTrailingMove) const;
    virtual void Dump() const;

private:
    VectrexCommandLines _lines;
};

/**
 * Each row in the text array has {x, y, text}.
 */
class VectrexCommandPrint2List : public VectrexCommandTransformable2D
{
public:
    VectrexCommandPrint2List(const Value &texts) : VectrexCommandTransformable2D(texts) {}
    
    virtual void Compile(bool eliminateTrailingMove) const;
    virtual void Dump() const;

    void GetVectrexData(Array<int32_t> *data, size_t *count, bool lines, bool eliminateTrailingMove, bool print = false) const;
};

class VectrexCommand6809Code : public VectrexCommand
{
public:
    VectrexCommand6809Code(const Value &bytes) : _bytes(bytes) {}
    
    virtual void Compile(bool eliminateTrailingMove) const;
    virtual void Dump() const;

private:
    Value _bytes;
};

extern list<Value> commands;

#endif	/* VECTREXCOMMAND_H */

