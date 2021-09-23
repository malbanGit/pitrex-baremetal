/* 
 * File:   Matrix.h
 * Author: Bob
 *
 * Created on September 25, 2016, 11:20 AM
 */

#ifndef MATRIX_H
#define	MATRIX_H

#include <stdlib.h>
#include <string.h>

class Matrix {
public:
    Matrix(size_t rows, size_t columns);
    Matrix(const Matrix& orig);
    ~Matrix();
    
    // Initialize the rows of a matrix. There should be columns * rows arguments, all floats
    void Initialize(float first, float f1,float f2, float f3=0, float f4=0, float f5=0, float f6=0, float f7=0, float f8=0, 
                    float f9=0, float f10=0, float f11=0, float f12=0, float f13=0, float f14=0, float f15=0);

//    void Initialize(float first, ...);
    void Initialize(float *src)
    {
        memcpy(_data, src, sizeof(float) * Rows() * Columns());
    }
    
    void Store(float *dest)
    {
        memcpy(dest, _data, sizeof(float) * Rows() * Columns());
    }
    
    void Store(float *dest, size_t count)
    {
        memcpy(dest, _data, sizeof(float) * count);
    }
    
    float *operator[](size_t index) {return _data + index * _columns;}
    const float *operator[](size_t index) const {return _data + index * _columns;}
    
    Matrix &operator*=(float scalar);  
    // arg matrix must have the same dimensions as *this. It is not checked!
    Matrix &operator+=(const Matrix &arg);
    Matrix &operator=(const Matrix &src);
    
    size_t Rows() const {return _rows;}
    size_t Columns() const {return _columns;}
    
private:
    size_t _rows, _columns;
    float *_data;
};

Matrix operator*(const Matrix &left, const Matrix &right);
Matrix operator*(float scalar, const Matrix &right);

#endif	/* MATRIX_H */

