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

#if !defined(AFX_GLWND_H__AC51E68B_53F0_4503_87B4_B2EFDE884AB1__INCLUDED_)
#define AFX_GLWND_H__AC51E68B_53F0_4503_87B4_B2EFDE884AB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include <gl/gl.h>
#include <gl/glu.h>
#include <il/ilut.h>
#include "defineGL.h"

/////////////////////////////////////////////////////////////////////////////
// defines
/////////////////////////////////////////////////////////////////////////////
#define NUM_CYCLES			7
#define RESTART_CYCLE		1
#define SLICE_LENGTH		25
#define SLICES_PER_SECOND	(1000 / SLICE_LENGTH)
#define CAMERA_FAR			15.0
#define CAMERA_NEAR			4.5

/////////////////////////////////////////////////////////////////////////////
// CGLWnd window
/////////////////////////////////////////////////////////////////////////////
class CGLWnd : public CWnd
{
// Construction
public:
	CGLWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGLWnd();

	/////////////////////////////////////////////////////////////////////////
	// drawing
	/////////////////////////////////////////////////////////////////////////
	void DrawScene();
	void OnTimer();
	void resizeGL(int iWidth, int iHeight);

protected:
	/////////////////////////////////////////////////////////////////////////
	// OpenGL initialization
	/////////////////////////////////////////////////////////////////////////
	void createGL(void);
	void CreateRGBPalette();
	BOOL setPixelFormat(void);
	unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);

	/////////////////////////////////////////////////////////////////////////
	// display lists handling
	/////////////////////////////////////////////////////////////////////////
	void buildLists(void);

	/////////////////////////////////////////////////////////////////////////
	// texture reading function
	/////////////////////////////////////////////////////////////////////////
	void readTextures(int iTex, GLuint *pList, CString strFile, BOOL bMip = TRUE);

	/////////////////////////////////////////////////////////////////////////
	// cycle handling
	/////////////////////////////////////////////////////////////////////////
	void setCycle();
	void handleCycle();

protected:
	//drawing needs
	CPalette    m_cPalette;
	CClientDC   *m_pDC;
	HGLRC		m_hrc;

	//timer resolution
	UINT m_uiTimerRes;
	UINT m_uiTimer;

	//rotation
	float m_dXRot;
	float m_dYRot;
	float m_dZRot;

	//delta rotations
	float m_dX;
	float m_dY;
	float m_dZ;

	//fade value
	float m_dFade;
	float m_dF;

	//display lists
	GLuint m_lists[6];

	//about box textures
	GLuint m_texAbout[GL_TEX_ABOUT];

	//current cycle
	int m_iCycle;

	//tick counter
	int m_iTickCounter;

	//cycle lengths
	int m_iCycleLengths[NUM_CYCLES];

	//pause lengths after a cycle
	int m_iCyclePauses[NUM_CYCLES];

	//are we in a cycle or a pause?
	BOOL m_bInCycle;

	// Generated message map functions
protected:
	//{{AFX_MSG(CGLWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_GLWND_H__AC51E68B_53F0_4503_87B4_B2EFDE884AB1__INCLUDED_)
