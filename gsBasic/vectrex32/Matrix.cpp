/* 
 * File:   Matrix.cpp
 * Author: Bob
 * 
 * Created on September 25, 2016, 11:20 AM
 */

#include "Matrix.h"
#include <math.h>
#include <stdarg.h>
//#include <cstdarg>
//#include <stdargs>

#include <stdio.h>

#ifdef __DEBUG
#define ASSERT(x) if (!(x)) __asm__ volatile (" sdbbp 0");
#else
#define ASSERT(x)
#endif

Matrix::Matrix(size_t rows, size_t columns) : 
    _rows(rows), _columns(columns), _data(new float [rows * columns])
{
}

Matrix::Matrix(const Matrix &src) : _data(0)
{
    *this = src;
}

Matrix::~Matrix()
{
    delete [] _data;
}

void Matrix::Initialize(float first, float f1,float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15)
{
  int c=0;
  float *data = _data;
  size_t count = Rows() * Columns();
  *(data+(c++)) = (float) first;
  if (c<count) *(data+(c++)) = (float) f1;
  if (c<count) *(data+(c++)) = (float) f2;
  if (c<count) *(data+(c++)) = (float) f3;
  if (c<count) *(data+(c++)) = (float) f4;
  if (c<count) *(data+(c++)) = (float) f5;
  if (c<count) *(data+(c++)) = (float) f6;
  if (c<count) *(data+(c++)) = (float) f7;
  if (c<count) *(data+(c++)) = (float) f8;
  if (c<count) *(data+(c++)) = (float) f9;
  if (c<count) *(data+(c++)) = (float) f10;
  if (c<count) *(data+(c++)) = (float) f11;
  if (c<count) *(data+(c++)) = (float) f12;
  if (c<count) *(data+(c++)) = (float) f13;
  if (c<count) *(data+(c++)) = (float) f14;
  if (c<count) *(data+(c++)) = (float) f15;
}
/*
// list on PiTrex is always null!
void Matrix::Initialize(float first, ...)
{
    float *data = _data;
    size_t count = Rows() * Columns();
    va_list list;
    va_start(list, first);
    *data++ = first;
    while (--count)
    {
        *data++ = (float) va_arg(list, double);
    }
    va_end(list);
}
*/
Matrix &Matrix::operator*=(float scalar)
{
    for (size_t index = 0; index < Rows() * Columns(); ++index)
    {
        _data[index] *= scalar;
    }
    return *this;
}

// arg matrix must have the same dimensions as *this. It is not checked!
Matrix &Matrix::operator+=(const Matrix &arg)
{
    ASSERT(Rows() == arg.Rows() && Columns() == arg.Columns());
    for (size_t index = 0; index < Rows() * Columns(); ++index)
    {
        _data[index] += arg._data[index];
    }
    return *this;
}

Matrix &Matrix::operator=(const Matrix &src)
{
    if (this != &src)
    {
        delete _data;
        _rows = src.Rows();
        _columns = src.Columns();
        _data = new float [src.Rows() * src.Columns()];
        memcpy(_data, src._data, sizeof(float) * src.Rows() * src.Columns());
    }
    return *this;
}

Matrix operator*(const Matrix &left, const Matrix &right)
{
    ASSERT(left.Columns() == right.Rows());
    Matrix result(left.Rows(), right.Columns());
    
    for (size_t leftRow = 0; leftRow < left.Rows(); ++leftRow)
    {
        for (size_t rightColumn = 0; rightColumn < right.Columns(); ++rightColumn)
        {
            float accumulator = 0;
            for (size_t leftColumn = 0; leftColumn < left.Columns(); ++leftColumn)
            {
                accumulator += left[leftRow][leftColumn] * right[leftColumn][rightColumn];
            }
            result[leftRow][rightColumn] = accumulator;
        }
    }
    
    return result;
}


Matrix operator*(float scalar, const Matrix &right)
{
    Matrix result(right.Rows(), right.Columns());
    
    for (size_t row = 0; row < right.Rows(); ++row)
    {
        for (size_t column = 0; column < right.Columns(); ++column)
        {
            result[row][column] = right[row][column] * scalar;
        }
    }
    
    return result;
}
