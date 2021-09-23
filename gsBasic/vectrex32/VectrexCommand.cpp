/* 
 * File:   VectrexCommand.cpp
 * Author: Bob
 * 
 * Created on March 15, 2016, 2:50 PM
 */
#include "VectrexCommand.h"
#include "../Value.h"
#include "../Array.h"
#include "Matrix.h"
#include "Camera.h"
#include "VectorFont.h"
#include <vectrex/vectrexInterface.h>

list<Value> commands;
ErrorCode VectrexCommand::_errorCode;
const VectrexCommand *VectrexCommand::_hugeDeltaSprite;

ErrorCode HugeDelta = "E520: X or Y coordinate is too large";
ErrorCode OutOfDPRAM = "E521: Not enough Dual Port RAM memory for all the Vectrex commands";
ErrorCode ModeIsNotInteger = "E522: Mode (DrawTo or MoveTo) in array is not an integer";
ErrorCode CoordinatesAreNotNumeric = "E523: X or Y coordinate in array is not numeric";
ErrorCode InvalidMagnification = "E524: Magnification value is illegal";
ErrorCode InvalidValue = "E525: Invalid value";


int b_currentTextSize_width=1;
int b_currentTextSize_height=-1;
int b_currentIntensity = -1;
int b_currentX = 0;
int b_currentY = 0;
int b_currentScale = 0xff;

// if you "calculate" the coordinates by really doing scale and stength using a vectrex - than take the "ORIGINAL" values
// if you calculate the value via MATH, than the scale must have a correction "value" - here "2"
// this is just "vectrex" experience!
#define ADDX(x) b_currentX=b_currentX+(x)*(b_currentScale+2)
#define ADDY(y) b_currentY=b_currentY+(y)*(b_currentScale+2)
#define SETX(x) b_currentX=(x)*b_currentScale
#define SETY(y) b_currentY=(y)*b_currentScale


void ToHex(char *out, int value)
{
	char ch = (char) (((value >> 4) & 0xf) + '0');
	if (ch > '9') ch = (char) (ch - '0' - 10 + 'a');
	*out++ = ch;
	ch = (char) ((value & 0xf) + '0');
	if (ch > '9') ch = (char) (ch - '0' - 10 + 'a');
	*out = ch;
}

template <>
void Array<int32_t>::PlacementNew(void *addr)
{
}
template <>
void Array<VectrexCommandTransformable::ModePoint<int32_t, 2> >::PlacementNew(void *addr)
{
}

template <>
void Array<VectrexCommandTransformable::ModePoint<int, 2> >::PlacementNew(void *addr)
{
}

template <>
void Array<VectrexCommandTransformable::ModePoint<float, 2> >::PlacementNew(void *addr)
{
}

template <>
void Array<VectrexCommandTransformable::ModePoint<float, 3> >::PlacementNew(void *addr)
{
}

void VectrexCommand::SetError(ErrorCode code) 
{
    _errorCode = code;
}

void VectrexCommand::DumpHeader(const char *commandType) const
{
    fprintf(stdout, "%s: %s;\r\n",
        commandType, Enabled() ? "visible" : "not drawn");
}

void VectrexCommandPrint2List::Compile(bool) const 
{
    if (Enabled())
    {
        const uint16_t *dims = _value.Dimensions();
        size_t arrayIndex = 0;
        bool success = true;
        uint8_t originalScale = b_currentScale;
        
        for (size_t row = 0; row < dims[0] && success; ++row, arrayIndex += dims[1])
        {
            int x, y;
            _value[arrayIndex].TypeBehaviors()->ToInteger(_value[arrayIndex], &x);
            _value[arrayIndex + 1].TypeBehaviors()->ToInteger(_value[arrayIndex + 1], &y);
            // Set the scale to 0x7f, move to the text location, then set the scale to 30 for the text
            
            
//           success = vi.CompileReset0Ref(writer) &&
SETX(0);
SETY(0);
//                vi.CompileSetScale(writer, uint8_t(0x7f)) && 
b_currentScale = 0x7f;
//                vi.CompileMoveto(writer, int8_t(x), int8_t(y)) &&
ADDX(x);
ADDY(y);
//                vi.CompileSetScale(writer, 15);            
b_currentScale = 15;
            
              Value lines = TextToLines(_value[arrayIndex + 2].ToString().c_str());
              VectrexCommandLines linesSprite(lines);
              linesSprite.Magnification(_magnifications[0], _magnifications[1], _magnifications[2]);
              linesSprite.SetTranslation(_translate);
              linesSprite.SetClippingRect(_clippingRect);
              linesSprite.SetRotation(GetRotation());
              linesSprite.Compile(true);
        }
        
//        success = success && vi.CompileSetScale(writer, originalScale) && vi.CompileReset0Ref(writer);
b_currentScale = originalScale;
        
        if (!success)
        {
            VectrexCommand::SetError(OutOfDPRAM);
        }
    }
}

void VectrexCommandPrint2List::Dump() const
{
    DumpHeader((char *)"Text2List");
    fputs((char *)"\r\n", stdout);
}

void VectrexCommandPrint2List::GetVectrexData(Array<int32_t> *data, size_t *count, bool blines, bool eliminateTrailingMove, bool print) const
{
    const uint16_t *dims = _value.Dimensions();
    size_t arrayIndex = 0;
    bool success = true;
    Array<int32_t> rowData;
    size_t rowCount = 0;
    
    data->resize(0);
    *count = 0;

    for (size_t row = 0; row < dims[0] && success; ++row, arrayIndex += dims[1])
    {
        int x, y;
        _value[arrayIndex].TypeBehaviors()->ToInteger(_value[arrayIndex], &x);
        _value[arrayIndex + 1].TypeBehaviors()->ToInteger(_value[arrayIndex + 1], &y);

        if (success)
        {
            Value lines = TextToLines(_value[arrayIndex + 2].ToString().c_str());
            VectrexCommandLines linesSprite(lines);
            linesSprite.Magnification(_magnifications[0], _magnifications[1], _magnifications[2]);
            linesSprite.SetTranslation(_translate);
            linesSprite.SetClippingRect(_clippingRect);
            linesSprite.SetRotation(GetRotation());
            linesSprite.GetVectrexData(&rowData, &rowCount, blines, eliminateTrailingMove, print);
            data->append(rowData);
            *count += rowCount;
        }

    }
}

VectrexCommandPrint2Str::VectrexCommandPrint2Str(const char *text) : 
    _lines(TextToLines(text)) 
{
}

void VectrexCommandPrint2Str::Compile(bool) const 
{
    if (Enabled())
    {
        uint8_t originalScale = b_currentScale;
        bool success = true;
b_currentScale = 15;
        if (success)
        {
            _lines.Compile(true);
//            success = vi.CompileSetScale(writer, originalScale) && vi.CompileReset0Ref(writer);
b_currentScale = originalScale;
        }
        if (!success)
        {
            VectrexCommand::SetError(OutOfDPRAM);
        }
    }
}

void VectrexCommandPrint2Str::Dump() const
{
    DumpHeader((char *)"Text2 (vector text)");
    fputs((char *)"\r\n", stdout);
}

VectrexCommandPrintStr::VectrexCommandPrintStr(const char *text)
{
    _len = strlen(text);
    _text = (char *) malloc(_len + 1);
    strcpy(_text, text);
}

void VectrexCommandPrintStr::Compile(bool) const
{
    if (Enabled())
    {
      // ATTENTION
      // position is NOT updated by this!
      v_printStringRaster16(b_currentX, b_currentY, (char *) _text, b_currentTextSize_width, b_currentTextSize_height, 0);
SETX(0);
SETY(0);
    }
};

void VectrexCommandPrintStr::Dump() const
{
    DumpHeader((char *)"Text");
    fputs(_text, stdout);
    fputs((char *)"\r\n", stdout);
}

void VectrexCommandPrintList::Compile(bool) const
{
    if (Enabled() && !_texts[0].IsUninitialized())
    {
        // Figure out the size of the memory block we need
        const uint16_t *dims = _texts.Dimensions();
        size_t size = 0;
        int count = 0, row = 0;
        while (count < dims[0] && _texts[row].IsNumeric() && _texts[row + 1].IsNumeric() && !_texts[row + 2].IsUninitialized())
        {
            size += 2 + _texts[row + 2].ToString().length() + 1;
            row += dims[1];
            ++count;
        }
        Array<char> data;
        data.resize(size);
        int i = 0;
        row = 0;
        int out = 0;
        while (i < count)
        {
            int x, y;
            _texts[row].TypeBehaviors()->ToInteger(_texts[row], &x);
            _texts[row + 1].TypeBehaviors()->ToInteger(_texts[row + 1], &y);
            String text = _texts[row + 2].ToString();
            v_printStringRaster16(x<<7,y<<7, (char *) text.c_str(),b_currentTextSize_width, b_currentTextSize_height, 0);
            row += dims[1];
            ++i;
        }
SETX(0);
SETY(0);
    }
};

void VectrexCommandPrintList::Dump() const
{
    DumpHeader((char *)"Text List");
    _texts.Print();
    fputs((char *)"\r\n", stdout);
}

void VectrexCommandTextSize::Compile(bool) const
{
    if (Enabled() && _textSize.IsArray() && _textSize.DimensionsSize() == 1 &&
        _textSize.Dimensions()[0] == 2)
    {
      int textSize[2];
      _textSize.GetArrayValues(textSize);
      b_currentTextSize_width = textSize[0];
      b_currentTextSize_height = -textSize[1];
    }
};

void VectrexCommandTextSize::Dump() const
{
    DumpHeader((char *)"Text Size");
    _textSize.Print();
    fputs((char *)"\r\n", stdout);
}

void VectrexCommandReset0Ref::Compile(bool) const
{
    if (Enabled())
    {
      SETX(0);
      SETY(0);
    }
};

void VectrexCommandReset0Ref::Dump() const
{
    DumpHeader((char *)"ReturnToOrigin");
    fputs((char *)"\r\n", stdout);
}

void VectrexCommandScale::Compile(bool) const
{
    if (Enabled())
    {
        b_currentScale = _scale;
        if (_ratio != 0)
            camera.SetToUnits(_ratio);
    }
}

void VectrexCommandScale::Dump() const
{
    DumpHeader((char *)"Scale");
    fprintf(stdout, "Set scale to %i", _scale);
}
void VectrexCommandMove::Compile(bool eliminateTrailingMove) const
{
    bool valid = true;
    if (Enabled())
    {
      ADDX(_x);
      ADDY(_y);
    }
}

void VectrexCommandMove::Dump() const
{
    DumpHeader((char *)"Move");
    fprintf(stdout, "Move to (%d, %d)\r\n", _x, _y);
}



void VectrexCommandTransformable2D::Rotate(float angle) 
{
    _angle += angle; 
    if (_angle >= 360)
        _angle -= int(_angle / 360) * 360;
    if (_angle < 0)
        _angle -= floor(_angle / 360) * 360;
}

float VectrexCommandTransformable2D::GetRotation() const 
{
    float angle = fmodf(_angle, 360);
    if (angle < 0)
        angle += 360;
    return angle;
}

void VectrexCommandTransformable2D::GetVectrexData(Array<int32_t> *data, size_t *count, bool lines, bool eliminateTrailingMove, bool print) const
{
    if (_angle != 0 || _magnifications[0] != 1.0 || _magnifications[1] != 1.0 || !_clippingRect.IsUninitialized())
    {
        Array<ModePoint<float, 2> > rawData;
        GetRawData(&rawData);
        if (!_errorCode || print)
        {
            Magnify(rawData);
            Rotate(rawData);
            Translate(rawData);
            Clip(rawData, lines);
            if (print)
                PrintRawData(rawData);
            ToVectrexCoordinates<float>(rawData, data, count, lines, eliminateTrailingMove, print);
        }
    }
    else
    {
        Array<ModePoint<int, 2> > rawData;
        GetRawData(&rawData);
        Translate(rawData);
        if (print)
            PrintRawData(rawData);
        if (!_errorCode || print)
            ToVectrexCoordinates<int>(rawData, data, count, lines, eliminateTrailingMove, print);
    }
}

void VectrexCommandTransformable2D::DumpHeader(const char *spriteType) const
{
    String angle = String::ftoa(_angle);
    String magx = String::ftoa(_magnifications[0]);
    String magy = String::ftoa(_magnifications[1]);
    String magz = String::ftoa(_magnifications[2]);
    fprintf(stdout, "%s: %s; angle %s; magnification {%s, %s, %s}\r\n",
            spriteType, Enabled() ? "visible" : "not drawn", angle.c_str(), 
            magx.c_str(), magy.c_str(), magz.c_str());
    
    _value.TypeBehaviors()->Print(_value);

    fputs("\r\nTranslate: ", stdout);
    _translate.TypeBehaviors()->Print(_translate);

    fputs("\r\nClipping Rect: ", stdout);
    _clippingRect.TypeBehaviors()->Print(_clippingRect);

    fputs("\r\n", stdout);
}


void VectrexCommandDots::Compile(bool eliminateTrailingMove) const
{
    if (Enabled())
    {
        Array<int32_t> data;
        size_t count;
        GetVectrexData(&data, &count, eliminateTrailingMove, false);
        signed int *d = (signed int *) data.data();
        for (int i=0;i<count; i++)
        {
          ADDY(*(d++));
          ADDX(*(d++)); // ATTENTION: increment in macro
          v_directDraw32(b_currentX, b_currentY, b_currentX, b_currentY, b_currentIntensity);
        }
    }
}

void VectrexCommandDots::Dump() const
{
    DumpHeader((char *)"Dots");
    fputs((char *)"\r\n", stdout);
}

void VectrexCommandIntensity::Compile(bool) const
{
    if (Enabled())
    {
      b_currentIntensity = uint8_t(_intensity);
      v_setBrightness(b_currentIntensity);
    }
}

void VectrexCommandIntensity::Dump() const
{
    DumpHeader((char *)"Intensity");
    fprintf(stdout, "Set intensity to %i\r\n", _intensity);
}
void VectrexCommandLines::Compile(bool eliminateTrailingMove) const
{
    if (Enabled())
    {
        Array<int32_t> data;
        size_t count;
        GetVectrexData(&data, &count, true, eliminateTrailingMove, false);
        signed int *d = (signed int *) data.data();
        for (int i=0;i<count; i++)
        {
          if (*d == 0) // pattern = 0 -> MOVE
          {
            ADDY(*(d+1)); // attention MACRO and ++!!!
            ADDX(*(d+2)); // attention MACRO and ++!!!
          }
          else if (*d == -1) // pattern = 0xff -> FULL line
          {
            v_directDraw32(b_currentX, b_currentY, ADDX(*(d+2)), ADDY(*(d+1)), b_currentIntensity);
          }
          else  // patterned line
          {
            v_directDraw32Patterned(b_currentX, b_currentY, ADDX(*(d+2)), ADDY(*(d+1)), b_currentIntensity, *d);
          }
          d+=3;
        }
    }
}

void VectrexCommandLines::Dump() const
{
    DumpHeader((char *)"Lines");
    fputs((char *)"\r\n", stdout);
}




void VectrexCommandTransformable3D::Rotate(float pitch, float roll, float yaw) 
{
    _angles[0] += pitch;
    _angles[1] += roll;
    _angles[2] += yaw;
    
    for (size_t i = 0; i < 3; ++i)
    {
        if (_angles[i] >= 360)
            _angles[i] -= int(_angles[i] / 360) * 360;
        if (_angles[i] < 0)
            _angles[i] -= floor(_angles[i] / 360) * 360;
    }
}

void VectrexCommandTransformable3D::NormalizeAngles()
{
    for (int i = 0; i < 3; ++i)
    {
        _angles[i] = fmodf(_angles[i], 360);
        if (_angles[i] < 0)
            _angles[i] += 360;
    }
}

Matrix VectrexCommandTransformable3D::GetMagnificationMatrix(const float *mags) const
{
    // The matrix for magnifying. 
    Matrix R(4, 4);
    R.Initialize(
        mags[0], 0, 0, 0,
        0, mags[1], 0, 0,
        0, 0, mags[2], 0,
        0, 0, 0, 1
        );
    
    return R;
}

Matrix VectrexCommandTransformable3D::GetRotationMatrix(const float *angles) const
{
    static const float degToRad = float(3.1415926 / 180);
    float pitch = angles[0] * degToRad, roll = angles[1] * degToRad, yaw = angles[2] * degToRad;
    float s1 = sinf(yaw), c1 = cosf(yaw);
    float s2 = sinf(roll), c2 = cosf(roll);
    float s3 = sinf(pitch), c3 = cosf(pitch);
    
    // The matrix for rotating. See https://www.youtube.com/watch?v=n6yeak4FITs and
    // https://en.wikipedia.org/wiki/Euler_angles. We're using Tait-Bryan YZX
    Matrix R(4, 4);
    R.Initialize(
        c1 * c2, s1 * s3 - c1 * c3 * s2, c3 * s1 + c1 * s2 * s3, 0,
        s2, c2 * c3, -c2 * s3, 0,
        -c2 * s1, c1 * s3 + c3 * s1 * s2, c1 * c3 - s1 * s2 * s3, 0,
        0, 0, 0, 1
        );
    
    return R;
}

Matrix VectrexCommandTransformable3D::GetTranslationMatrix(float *translate) const
{
    // The matrix for translating
    Matrix T(4, 4);
    T.Initialize(
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        translate[0], translate[1], translate[2], 0
        );
    
    return T;
}
    
Matrix VectrexCommandTransformable3D::GetCameraRotationMatrix(const float *angles) const
{
    static const float degToRad = float(3.1415926 / 180);
    // We negate the angles since we're really rotating the world into the camera's view,
    // rather than rotating the camera
    float pitch = -angles[0] * degToRad, roll = -angles[1] * degToRad, yaw = -angles[2] * degToRad;
    float s1 = sinf(yaw), c1 = cosf(yaw);
    float s2 = sinf(roll), c2 = cosf(roll);
    float s3 = sinf(pitch), c3 = cosf(pitch);
    
    // The matrix for rotating. See https://www.youtube.com/watch?v=n6yeak4FITs and
    // https://en.wikipedia.org/wiki/Euler_angles. We're using Tait-Bryan YZX
    Matrix R(4, 4);
    R.Initialize(
        c1 * c2, s1 * s3 - c1 * c3 * s2, c3 * s1 + c1 * s2 * s3, 0,
        s2, c2 * c3, -c2 * s3, 0,
        -c2 * s1, c1 * s3 + c3 * s1 * s2, c1 * c3 - s1 * s2 * s3, 0,
        0, 0, 0, 1
        );
    
    return R;
}


// a and b are endpoints of a line that crosses z == 0. We return the coordinates
// of the point at z == 0
void VectrexCommandTransformable3D::ZeroCrossingPoint(const ModePoint<float, 3> &a, const ModePoint<float, 3> &b, ModePoint<float, 3> *result) const
{
    float ratio = a.coordinates[2] / (a.coordinates[2] - b.coordinates[2]);
    result->coordinates[0] = (b.coordinates[0] - a.coordinates[0]) * ratio + a.coordinates[0];
    result->coordinates[1] = (b.coordinates[1] - a.coordinates[1]) * ratio + a.coordinates[1];
    result->coordinates[2] = 0;
}

// Eliminate line segments that are behind the camera
void VectrexCommandTransformable3D::ZClipping(const Array<ModePoint<float, 3> > &modePoints, Array<ModePoint<float, 3> > &newModePoints) const
{
    newModePoints.reserve(modePoints.size());

    // Go through each vertex
    for (size_t i = 0; i < modePoints.size(); ++i)
    {
        // If this vertex is behind the camera
        if (modePoints[i].coordinates[2] < 0)
        {
            // If we're drawing to this vertex
            if (modePoints[i].mode)
            {
                // If the previous vertex was in front of the camera
                if (i > 0 && modePoints[i - 1].coordinates[2] >= 0)
                {
                    // Make a DrawTo vertex at the last visible point
                    ModePoint<float, 3> zeroCross;
                    ZeroCrossingPoint(modePoints[i - 1], modePoints[i], &zeroCross);
                    zeroCross.mode = 1;
                    newModePoints.push_back(zeroCross);

                    // Move the rest of the way to this vertex
                    newModePoints.push_back(modePoints[i]);
                    newModePoints.back().mode = 0;
                }
                
                // Else (the previous vertex was also behind the camera)
                else
                {
                    if (newModePoints.size())
                    {
                        // The previous new vertex will be a MoveTo. Have it move to this new vertex
                        newModePoints.back().coordinates[0] = modePoints[i].coordinates[0];
                        newModePoints.back().coordinates[1] = modePoints[i].coordinates[1];
                        newModePoints.back().coordinates[2] = modePoints[i].coordinates[2];
                    }
                    else
                    {
                        newModePoints.push_back(modePoints[i]);
                        newModePoints.back().mode = 0;
                    }
                }
            }
            
            // Else (we're moving to a vertex behind the camera)
            else
            {
                // If the previous new vertex is a move, replace it with this one
                if (newModePoints.size() && newModePoints.back().mode == 0)
                {
                    newModePoints.back().coordinates[0] = modePoints[i].coordinates[0];
                    newModePoints.back().coordinates[1] = modePoints[i].coordinates[1];
                    newModePoints.back().coordinates[2] = modePoints[i].coordinates[2];
                }
                // Else move to this point
                else
                {
                    newModePoints.push_back(modePoints[i]);
                }
            }
        }
        
        // Else (this vertex is in front of the camera)
        else
        {
            // If the previous new vertex is behind the camera
            if (newModePoints.size() && newModePoints.back().coordinates[2] < 0)
            {
                // If this vertex is a MoveTo, have the previous new vertex move here
                if (modePoints[i].mode == 0)
                {
                    newModePoints.back().coordinates[0] = modePoints[i].coordinates[0];
                    newModePoints.back().coordinates[1] = modePoints[i].coordinates[1];
                    newModePoints.back().coordinates[2] = modePoints[i].coordinates[2];
                }
                // Else (this is a DrawTo)
                else
                {
                    // The previous vertex should be a MoveTo. Have it move to the 
                    // zero-crossing point of this line
                    ZeroCrossingPoint(modePoints[i - 1], modePoints[i], &newModePoints.back());
                    // And then draw to this vertex
                    newModePoints.push_back(modePoints[i]);
                }
            }
            // Else (both this vertex and the previous one are in front of the camera)
            else
            {
                newModePoints.push_back(modePoints[i]);
            }
        }
    }
}

// Project a 3D sprite onto a 2D surface using the perspective projection
void VectrexCommandTransformable3D::Project2D(const Array<ModePoint<float, 3> > &modePoints, Array<ModePoint<float, 2> > &projection) const
{
    // The user is sitting about 0.5 meters from the Vectrex screen
    static const float distanceToScreen = 0.5;
    float unitsPerWorldCoord = camera.GetToUnits();
    double factor = camera.GetFocalLength() / 50 * distanceToScreen * unitsPerWorldCoord;
    projection.resize(modePoints.size());
    for (size_t i = 0; i < projection.size();  ++i)
    {
        projection[i].mode = modePoints[i].mode;
        volatile float perspectiveRatio;
        float z = modePoints[i].coordinates[2];
        // The default camera focal length is 50mm, which photographers consider
        // a "normal" view. We need to convert that into a "normal" view for the Vectrex.
        // So we scale 50mm to the distance between the user and the Vectrex screen.
        perspectiveRatio = float(factor / (distanceToScreen + z));
        projection[i].coordinates[0] = perspectiveRatio * modePoints[i].coordinates[0];
        projection[i].coordinates[1] = perspectiveRatio * modePoints[i].coordinates[1];
    }
}

void VectrexCommandTransformable3D::GetProjectedVectrexData(
    bool lines, bool print, Array<ModePoint<float, 2> > &projectedData) const
{
    Array<ModePoint<float, 3> > rawData, zclipped;

    GetRawData<float, 3>(&rawData, true);
    if (print)
    {
        puts("Raw data (meters):\r");
        PrintRawData(rawData);
    }
    if (!_errorCode || print)
    {
        // Get the location of the object relative to the camera
        float translate[3] = {0}, cameraTranslate[3] = {0};
        if (_translate.IsArray() && _translate.DimensionsSize() == 1 && _translate.Dimensions()[0] == 3)
        {
            if (_translate.GetArrayValues(translate) != 0)
                translate[0] = translate[1] = translate[2] = 0;
        }
        const Value &cameraTranslation = camera.GetTranslation();
        if (cameraTranslation.IsArray() && cameraTranslation.DimensionsSize() == 1 && cameraTranslation.Dimensions()[0] == 3)
        {
            if (cameraTranslation.GetArrayValues(cameraTranslate) == 0)
            {
                translate[0] -= cameraTranslate[0];
                translate[1] -= cameraTranslate[1];
                translate[2] -= cameraTranslate[2];
            }
            
        }
        
        Matrix xform = GetMagnificationMatrix(_magnifications);
        xform = xform * GetRotationMatrix(_angles);
        // Translate based on the object's translation, offset by the camera's translation
        xform += GetTranslationMatrix(translate);
        Matrix cameraRotation = GetCameraRotationMatrix(camera.GetRotation());
        xform = xform * cameraRotation;
        
        Matrix coordinates(1, 4);
        size_t i;
        for (i = 0; i < rawData.size(); ++i)
        {
            coordinates.Initialize(rawData[i].coordinates[0],
                    rawData[i].coordinates[1],
                    rawData[i].coordinates[2], (double) 1.0);
            Matrix transformed(coordinates * xform);
            transformed.Store(rawData[i].coordinates, 3);
        }

        // If we're using a 3D camera (separate left and right views)
        /*if (camera.Is3D() && camera.GlassesDetected())
        {
            // We're using parallel cameras and we'll adjust the
            // convergence point after we've projected 3D onto 2D
            // Translate the object to account for the active eye.
            float eyeOffset = camera.GetOcularDistance() / 2;
            if (camera.GetEyeSelection() == RIGHT_OPEN)
                eyeOffset = -eyeOffset;
            for (size_t i = 0; i < rawData.size(); ++i)
            {
                rawData[i].coordinates[0] += eyeOffset;
            }
        }*/
        
        if (print)
        {
            puts("View coordinates: (meters)\r");
            PrintRawData(rawData);
        }

        ZClipping(rawData, zclipped);
        if (print)
        {
            puts("z-clipped coordinates (meters):\r");
            PrintRawData(zclipped);
        }

        Project2D(zclipped, projectedData);

        // If we're using a 3D camera (separate left and right views)
        /*if (camera.Is3D() && camera.GlassesDetected())
        {
            // Translate the object to do convergence
            // We shot the image using parallel cameras and now we adjust the
            // convergence point after we've projected 3D onto 2D
            if (!camera.IsValidConvergence2DOffset())
            {
                // Figure out how much to translate the object to converge it.
                // We do this by imagining a point in the center, between the eyes,
                // at the convergence distance. We calculate how that would be 
                // viewed by the left camera and project it onto 2D. 
                // Its X coordinate in the 2D projection is how much we have to
                // move objects to converge them.
                ModePoint<float, 3> convergencePoint;
                convergencePoint.coordinates[0] = camera.GetOcularDistance() / 2;
                convergencePoint.coordinates[1] = 0;
                convergencePoint.coordinates[2] = camera.GetConvergenceDistance();
                Array<ModePoint<float, 3> > convergencePointVector;
                Array<ModePoint<float, 2> > convergenceProjection;
                convergencePointVector.push_back(convergencePoint);
                Project2D(convergencePointVector, convergenceProjection);

                // Cache the result of the calculation
                camera.SetConvergence2DOffset(convergenceProjection[0].coordinates[0]);
            }
            // Get the cached result of the convergence calculation
            float converge2D = camera.GetConvergence2DOffset();
            // When we're looking out the left eye, we need to translate objects to the
            // left (negative X direction) to converge them. (When looking out the
            // right eye, we translate them to the right)
            if (camera.GetEyeSelection() == LEFT_OPEN)
                converge2D = -converge2D;
            for (size_t i = 0; i < projection.size(); ++i)
            {
                projection[i].coordinates[0] += converge2D;
            }
        }*/
        if (print)
        {
            puts("Projected coordinates: (units)\r");
            PrintRawData(projectedData);
        }
    }
    
    // Else (there's an error code)
    else 
    {
        if (print)
        {
            fputs("Processing terminated due to error: ", stdout);
            puts(_errorCode);
        }
    }
}
    
void VectrexCommandTransformable3D::GetVectrexData(Array<int32_t> *data, size_t *count, 
    bool lines, bool eliminateTrailingMove, bool print) const
{
    // Make a clipping rect that includes the screen, measured in units at scale 64
    static uint16_t dims[2] = {2, 2};
    static Value clippingRect(dims, 2);
    if (clippingRect[0].IsUninitialized())
    {
        clippingRect[0] = Value(-268.0);
        clippingRect[1] = Value(-324.0);
        clippingRect[2] = Value(268.0);
        clippingRect[3] = Value(324.0);
    }
    
    Array<ModePoint<float, 2> > projection;
    GetProjectedVectrexData(lines, print, projection);

    if (_clippingRect.IsUninitialized())
        Clip(projection, clippingRect, lines);
    else
        Clip(projection, lines);
    if (print)
    {
        puts("Clipped coordinates (units):\r");
        PrintRawData(projection);
    }
    ToVectrexCoordinates<float>(projection, data, count, lines, eliminateTrailingMove, print);
}

void VectrexCommandTransformable3D::DumpHeader(const char *spriteType) const
{
    String pitch = String::ftoa(_angles[0]), roll = String::ftoa(_angles[1]), yaw = String::ftoa(_angles[2]);
    String magx = String::ftoa(_magnifications[0]);
    String magy = String::ftoa(_magnifications[1]);
    String magz = String::ftoa(_magnifications[2]);
    fprintf(stdout, "%s: %s; pitch %s, roll %s, yaw %s; magnification {%s, %s, %s}\r\n",
            spriteType, Enabled() ? "visible" : "not drawn", pitch.c_str(), roll.c_str(), yaw.c_str(), 
            magx.c_str(), magy.c_str(), magz.c_str());
    _value.TypeBehaviors()->Print(_value);
    fputs("\r\nTranslate: ", stdout);
    _translate.TypeBehaviors()->Print(_translate);
    fputs("\r\nClipping Rect: ", stdout);
    _clippingRect.TypeBehaviors()->Print(_clippingRect);
    fputs("\r\n", stdout);
}



void VectrexCommandLines3D::Compile(bool eliminateTrailingMove) const
{
    if (Enabled())
    {
        Array<int32_t> data;
        size_t count;
        GetVectrexData(&data, &count, true, eliminateTrailingMove);
        
        signed int *d = (signed int *) data.data();
        for (int i=0;i<count; i++)
        {
          if (*d == 0) // pattern = 0 -> MOVE
          {
            ADDY(*(d+1)); 
            ADDX(*(d+2)); 
          }
          else if (*d == -1) // pattern = 0xff -> FULL line
          {
            v_directDraw32(b_currentX, b_currentY, ADDX(*(d+2)), ADDY(*(d+1)), b_currentIntensity);
          }
          else  // patterned line
          {
            v_directDraw32Patterned(b_currentX, b_currentY, ADDX(*(d+2)), ADDY(*(d+1)), b_currentIntensity, *d);
          }
          d+=3;
        }
    }
}

void VectrexCommandLines3D::Dump() const
{
    DumpHeader((char *)"Lines3D");
    fputs((char *)"\r\n", stdout);
}


uint16_t VectrexCommandLines25D::_dims2[1] = {2};

VectrexCommandLines25D::VectrexCommandLines25D(const Value &lines) :
    VectrexCommandTransformable3D(Dummy3DLines()), _2DLines(lines),
    _translation(_dims2, 1)
{
    _2DLines.SetTranslation((Value &) _translation);
}

void VectrexCommandLines25D::GetVectrexData(Array<int32_t> *data, size_t *count, bool lines, bool eliminateTrailingMove, bool print)
{    
    Array<ModePoint<float, 2> > projectedData;
    *count = 0;
    if (print)
        puts("3D stand-in:\r");
    VectrexCommandTransformable3D::GetProjectedVectrexData(true, print, projectedData);
    if (_errorCode == 0 && projectedData.size() >= 2)
    {
        // Apply the transform, scaling, and clipping done to the 3D line to the 2D object
        int originX = projectedData[0].coordinates[0], originY = projectedData[0].coordinates[1];
        int deltaY = projectedData[projectedData.size() - 1].coordinates[1] - originY;
        float scale = float(deltaY);

        _translation[0] = float(originX);
        _translation[1] = float(originY);
        _2DLines.Magnification(scale, scale, scale);
        _2DLines.SetClippingRect(_clippingRect);

        // Get the Vectrex data for the 2D object
        if (print)
            puts("2D sprite:\r");
        _2DLines.GetVectrexData(data, count, lines, eliminateTrailingMove, print);
    }
}

void VectrexCommandLines25D::Compile(bool eliminateTrailingMove) const
{
    if (Enabled())
    {
        Array<int32_t> data;
        size_t count;
        GetVectrexData(&data, &count, true, eliminateTrailingMove, false);
//        GetVectrexData(&data, &count, true, false, true);
        signed int *d = (signed int *) data.data();
        for (int i=0;i<count; i++)
        {
          if (*d == 0) // pattern = 0 -> MOVE
          {
            ADDY(*(d+1)); // attention MACRO and ++!!!
            ADDX(*(d+2)); // attention MACRO and ++!!!
          }
          else if (*d == -1) // pattern = 0xff -> FULL line
          {
            v_directDraw32(b_currentX, b_currentY, ADDX(*(d+2)), ADDY(*(d+1)), b_currentIntensity);
          }
          else  // patterned line
          {
            v_directDraw32Patterned(b_currentX, b_currentY, ADDX(*(d+2)), ADDY(*(d+1)), b_currentIntensity, *d);
          }
          d+=3;
        }
    }
}

const Value &VectrexCommandLines25D::Dummy3DLines()
{
    static Value dummy3DLines;
    if (dummy3DLines.IsUninitialized())
    {
        uint16_t dimensions[2] = {2, 4};
        dummy3DLines = Value(dimensions, 2);
        // MoveTo 0, 0, 0
        dummy3DLines[0] = int(0);
        dummy3DLines[1] = (float) 0.0;
        dummy3DLines[2] = (float) 0.0;
        dummy3DLines[3] = (float) 0.0;
        // DrawTo 0, 1, 0
        dummy3DLines[4] = int(1);
        dummy3DLines[5] = (float) 0.0;
        dummy3DLines[6] = (float) 1.0;
        dummy3DLines[7] = (float) 0.0;
    }
    
    return dummy3DLines;
}

void VectrexCommandLines25D::Dump() const
{
    DumpHeader((char *)"Lines25D");
    fputs((char *)"\r\n", stdout);
}
