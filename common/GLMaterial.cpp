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

#include "Stdafx.h"
#include "GLMaterial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

GLMaterial::GLMaterial(double r, double g, double b, double a)
{
	SetMaterial(r, g, b);
	init();
	fA = (GLfloat) a;
}

GLMaterial::GLMaterial(COLORREF c)
{
	SetMaterial(c);
	init();
}

GLMaterial::GLMaterial(int r, int g, int b)
{
	SetMaterial(r, g, b);
	init();
}

GLMaterial::GLMaterial()
{
	//default is solid gray
	SetMaterial(0.5, 0.5, 0.5);
	init();
}

GLMaterial::~GLMaterial()
{
	// no cleanup necessary
}

void GLMaterial::init()
{
	fA	= 1.0;
	fAa = 1.0;

	//emission values
	fRe = 0.;
	fGe = 0.;
	fBe = 0.;
	fAe = 0.;

	//specular values
	fRs = 0.;
	fGs = 0.;
	fBs = 0.;
	fAs = 0.;
}

void GLMaterial::SetMaterial(double r, double g, double b, double a)
{
	fR = (GLfloat) r;
	fG = (GLfloat) g;
	fB = (GLfloat) b;
	fA = (GLfloat) a;

	fRa = (GLfloat) r;
	fGa = (GLfloat) g;
	fBa = (GLfloat) b;
	fAa = (GLfloat) a;
}

void GLMaterial::SetMaterial(COLORREF c)
{
	fR	= (GLfloat) ((double) GetRValue(c) / 255.0);
	fG	= (GLfloat) ((double) GetGValue(c) / 255.0);
	fB	= (GLfloat) ((double) GetBValue(c) / 255.0);

	fRa	= (GLfloat) ((double) GetRValue(c) / 255.0);
	fGa	= (GLfloat) ((double) GetGValue(c) / 255.0);
	fBa	= (GLfloat) ((double) GetBValue(c) / 255.0);
}

void GLMaterial::SetMaterial(int r, int g, int b)
{
	fR	= (GLfloat) ((double) r / 255.0);
	fG	= (GLfloat) ((double) g / 255.0);
	fB	= (GLfloat) ((double) b / 255.0);
	
	fRa	= (GLfloat) ((double) r / 255.0);
	fGa	= (GLfloat) ((double) g / 255.0);
	fBa	= (GLfloat) ((double) b / 255.0);
}

void GLMaterial::SetAmbient(COLORREF c)
{
	fRa	= (GLfloat) (GetRValue(c) / 255.0);
	fGa	= (GLfloat) (GetGValue(c) / 255.0);
	fBa	= (GLfloat) (GetBValue(c) / 255.0);
}

void GLMaterial::SetDiffuse(COLORREF c)
{
	fR	= (GLfloat) ((double) GetRValue(c) / 255.0);
	fG	= (GLfloat) ((double) GetGValue(c) / 255.0);
	fB	= (GLfloat) ((double) GetBValue(c) / 255.0);
}

void GLMaterial::SetEmission(COLORREF c)
{
	fRe	= (GLfloat) ((double) GetRValue(c) / 255.0);
	fGe	= (GLfloat) ((double) GetGValue(c) / 255.0);
	fBe	= (GLfloat) ((double) GetBValue(c) / 255.0);
}

void GLMaterial::SetSpecular(COLORREF c)
{
	fRs	= (GLfloat) ((double) GetRValue(c) / 255.0);
	fGs	= (GLfloat) ((double) GetGValue(c) / 255.0);
	fBs	= (GLfloat) ((double) GetBValue(c) / 255.0);
}

void GLMaterial::MultAmbient(double d)
{
	fRa	*= (GLfloat) d; if(fRa < 0.) fRa = 0.; if(fRa > 1.0) fRa = 1.0;
	fGa	*= (GLfloat) d; if(fGa < 0.) fGa = 0.; if(fGa > 1.0) fGa = 1.0;
	fBa	*= (GLfloat) d; if(fBa < 0.) fBa = 0.; if(fBa > 1.0) fBa = 1.0;
}

void GLMaterial::MultDiffuse(double d)
{
	fR	*= (GLfloat) d; if(fR < 0.) fR = 0.; if(fR > 1.0) fR = 1.0;
	fG	*= (GLfloat) d; if(fG < 0.) fG = 0.; if(fG > 1.0) fG = 1.0;
	fB	*= (GLfloat) d; if(fB < 0.) fB = 0.; if(fB > 1.0) fB = 1.0;
}

void GLMaterial::UseMaterial()
{
	GLfloat amb[] = {fRa, fGa, fBa, fAa};
	GLfloat dif[] = {fR, fG, fB, fA};
	GLfloat emiss[] = {fRe, fGe, fBe, fAe};
	GLfloat spec[] = {fRs, fGs, fBs, fAs};

	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT, GL_EMISSION, emiss);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialfv(GL_FRONT, GL_SHININESS, &fPol);
}

GLMaterial::operator COLORREF (void)
{
	int r = (int)(fR * 255.0);
	int g = (int)(fG * 255.0);
	int b = (int)(fB * 255.0);

	return RGB(r,g,b);
}