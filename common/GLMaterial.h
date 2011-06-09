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

#if !defined(AFX_GLMATERIAL_H__0159C8A2_F474_11D1_B505_004095E04020__INCLUDED_)
#define AFX_GLMATERIAL_H__0159C8A2_F474_11D1_B505_004095E04020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <gl/gl.h>
#include <gl/glu.h>

class GLMaterial
{
public: // construction & destruction
	GLMaterial();
	GLMaterial(COLORREF c);
	GLMaterial(int r, int g, int b);
	GLMaterial(double r, double g, double b, double a=1.0);
	virtual ~GLMaterial();

public: // implementation functions
	inline double GetR(void) { return fR; }
	inline double GetG(void) { return fG; }
	inline double GetB(void) { return fB; }
	inline double GetA(void) { return fA; }

	inline double GetRe(void) { return fRe; }
	inline double GetGe(void) { return fGe; }
	inline double GetBe(void) { return fBe; }
	inline double GetAe(void) { return fAe; }

	void MultAmbient(double d);
	void MultDiffuse(double d);
	void SetAmbient(COLORREF c);
	void SetSpecular(COLORREF c);
	void SetEmission(COLORREF c);
	void SetDiffuse(COLORREF c);
	void SetMaterial(double r, double g, double b, double a=1.0);
	void SetMaterial(COLORREF c);
	void SetMaterial(int r, int g, int b);
	inline void SetR(double r) { fR = (GLfloat) r; }
	inline void SetG(double g) { fG = (GLfloat) g; }
	inline void SetB(double b) { fB = (GLfloat) b; }
	inline void SetA(double a) { fA = (GLfloat) a; }
	inline void SetRe(double r) { fRe = (GLfloat) r; }
	inline void SetGe(double g) { fGe = (GLfloat) g; }
	inline void SetBe(double b) { fBe = (GLfloat) b; }
	inline void SetAe(double a) { fAe = (GLfloat) a; }
	inline void SetPolish(double d) {fPol = (GLfloat) d;}

	void UseMaterial(); // send GL code directly

public: // operators
	operator COLORREF ();

private: 

	void init();
	
	//ambient values
	GLfloat fRa;
	GLfloat fGa;
	GLfloat fBa;
	GLfloat fAa;

	// data members
	GLfloat fR;
	GLfloat fG;
	GLfloat fB;
	GLfloat fA;

	//emission values
	GLfloat fRe;
	GLfloat fGe;
	GLfloat fBe;
	GLfloat fAe;

	//specular values
	GLfloat fRs;
	GLfloat fGs;
	GLfloat fBs;
	GLfloat fAs;

	//polish
	GLfloat fPol;
};

#endif // !defined(AFX_GLMATERIAL_H__0159C8A2_F474_11D1_B505_004095E04020__INCLUDED_)
