/*
 *  Settlers3D - Copyright (C) 2001-2003 Jason Fugate (jfugate@settlers3d.net)
 * 
 *  This program is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as published by the Free 
 *  Software Foundation; either version 2 of the License, or (at your option) 
 *  any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 *  for more details.
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>

#define M_PI 3.1415926535897932384626433832795
#define piOver180 M_PI/180

class CVector
{
  public:
    // Constructors
    CVector()
    { v[0] = v[1] = v[2] = 0; }

    CVector(double x, double y, double z)
    { v[0] = x; v[1] = y; v[2] = z; }

    CVector(CVector &vec)
    { v[0] = vec[0]; v[1] = vec[1]; v[2] = vec[2]; }

    CVector(const double *vec)
    { v[0] = vec[0]; v[1] = vec[1]; v[2] = vec[2]; }

    // Arithmetic operators
    CVector operator+(CVector &vec)
    { return CVector(v[0] + vec[0], v[1] + vec[1], v[2] + vec[2]); }
    CVector operator+(double val)
    { return CVector(v[0] + val, v[1] + val, v[2] + val); }

    CVector operator-(CVector &vec)
    { return CVector(v[0] - vec[0], v[1] - vec[1], v[2] - vec[2]); }
    CVector operator-(double val)
    { return CVector(v[0] - val, v[1] - val, v[2] - val); }

    CVector operator*(CVector &vec)
    { return CVector(v[0] * vec[0], v[1] * vec[1], v[2] * vec[2]); }
    CVector operator*(double val)
    { return CVector(v[0] * val, v[1] * val, v[2] * val); }

    CVector operator/(CVector &vec)
    { return CVector(v[0] / vec[0], v[1] / vec[1], v[2] / vec[2]); }
    CVector operator/(double val)
    { return CVector(v[0] / val, v[1] / val, v[2] / val); }

    CVector &operator+=(CVector &vec)
    { *this = *this + vec; return *this; }
    CVector &operator+=(double val)
    { *this = *this + val; return *this; }
    
    CVector &operator-=(CVector &vec)
    { *this = *this - vec; return *this; }
    CVector &operator-=(double val)
    { *this = *this - val; return *this; }

    CVector &operator*=(CVector &vec)
    { *this = *this * vec; return *this; }
    CVector &operator*=(double val)
    { *this = *this * val; return *this; }

    CVector &operator/=(CVector &vec)
    { *this = *this / vec; return *this; }
    CVector &operator/=(double val)
    { *this = *this / val; return *this; }

    // Access operators
    const double &operator[](int i) const
    { return v[i]; }

    double &operator[](int i)
    { return v[i]; }

    operator double*(void)
    { return v; }

    inline void Set(double x, double y, double z)
    { v[0] = x; v[1] = y; v[2] = z; }

    inline void Clear(void)
    { v[0] = v[1] = v[2] = 0; }

    inline void Invert(void)
    { 
      v[0] = -v[0]; 
      v[1] = -v[1]; 
      v[2] = -v[2];
    }

    inline void Scale(double amnt)
    { 
      v[0] = v[0] * amnt; 
      v[1] = v[1] * amnt; 
      v[2] = v[2] * amnt;
    }

    inline void Scale(double x, double y, double z)
    { 
      v[0] *= x; 
      v[1] *= y; 
      v[2] *= z;
    }

    inline double Length(void)
    { return (double) sqrt(v[0]*v[0] + v[1]*v[1] + v[2] * v[2]); }

    inline void Normalize(void)
    {
      double len = this->Length();

      if (len == 0.0)
        return;

      double reciplen = (double)1.0/len;

      v[0] *= reciplen;
      v[1] *= reciplen;
      v[2] *= reciplen;
    }

    inline void RotateX(double angle)
    {
      double s = (double)sin(angle*piOver180);
      double c = (double)cos(angle*piOver180);
      double y = v[1];
      double z = v[2];

      v[0] = v[0];
      v[1] = (y * c) - (z * s);
      v[2] = (y * s) + (z * c);
    }

    inline void RotateY(double angle)
    {
      double s = (double)sin(angle*piOver180);
      double c = (double)cos(angle*piOver180);
      double x = v[0];
      double z = v[2];

      v[0] = (x * c) + (z * s);
      v[1] = v[1];
      v[2] = (z * c) - (x * s);
  
    }

    inline void RotateZ(double angle)
    {
      double s = (double)sin(angle*piOver180);
      double c = (double)cos(angle*piOver180);
      double x = v[0];
      double y = v[1];

      v[0] = (x * c) - (y * s);
      v[1] = (y * c) + (x * s);
      v[2] = v[2];

    }
  private:
    double v[3];
};

inline CVector CrossProduct(CVector v1, CVector v2)
{
  CVector res;

  res.Set((v1[1] * v2[2]) - (v1[2] * v2[1]),
          (v1[2] * v2[0]) - (v1[0] * v2[2]),
          (v1[0] * v2[1]) - (v1[1] * v2[0]));

  return res;
}

inline double DotProduct(CVector v1, CVector v2)
{
  return (v1[0] * v2[0]) + 
         (v1[1] * v2[1]) +
         (v1[2] * v2[2]);
}

#endif