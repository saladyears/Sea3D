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

#if !defined(AFX_GLBASEVIEW_H__73DD52E2_D974_4F9D_8AA6_818B540FD041__INCLUDED_)
#define AFX_GLBASEVIEW_H__73DD52E2_D974_4F9D_8AA6_818B540FD041__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GLBaseView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "GLMaterial.h"
#include <il\ilut.h>
#include "vector.h"
#include "defineGL.h"
#include "defineGame.h"

/////////////////////////////////////////////////////////////////////////////
// CGLBaseView view

class CGLBaseView : public CView
{
protected:
	CGLBaseView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGLBaseView)

// Attributes
public:
	void setGouraud(void);
	void setDetail(void);
	void setEuro(void);
	void setHighlight(void);
	void setHighlightColor(void);
	void setWhiteOutline(void);
	void setWhiteOutlineColor(void);
	void setLongestOutlineColor(void);

	//textures
	inline BOOL getMipMap(void) {return m_bMip;}
	inline int getDetail(void) {return m_iDetail;}

	//hit testing for mouseovers
	void setHitTest(BOOL b);

	//turn seafarers borders on and off
	void setBorders();

	//turn seafarers tiles on and off
	void setSeaTiles();
	
// Operations
public:

	//textures
	void setMipMap();
	void rebuildTextures();
	void rebuildOtherTextures();

	//determine if they have euro tiles installed
	BOOL checkForEuro(void);

	//determine if they have sea borders installed
	BOOL checkForSeaBorders(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLBaseView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGLBaseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	/////////////////////////////////////////////////////////////////////////
	// OpenGL initialization
	/////////////////////////////////////////////////////////////////////////
	void createGL(void);
	void resizeGL(int iWidth, int iHeight);
	void CreateRGBPalette();
	BOOL setPixelFormat(void);
	unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);

	/////////////////////////////////////////////////////////////////////////
	// selection
	/////////////////////////////////////////////////////////////////////////
	void selectGL(int x, int y);

	/////////////////////////////////////////////////////////////////////////
	// overrides
	/////////////////////////////////////////////////////////////////////////
	virtual void DrawScene(void);
	virtual void SelectObject(int id);
	virtual void resetEye(void);

	/////////////////////////////////////////////////////////////////////////
	// texture functions
	/////////////////////////////////////////////////////////////////////////
	void readTextures(int iTex, GLuint *pList, CString strFile, BOOL bMip = TRUE);
	void buildTileTextures(void);
	void buildOtherTextures(void);

	void clearOtherTextures(void);

	/////////////////////////////////////////////////////////////////////////
	// display list related functions
	/////////////////////////////////////////////////////////////////////////
	void buildLists(void);
	void buildTiles(void);
	void buildSettlement(void);
	void buildCity(void);
	void buildRoad(void);
	void buildRobber(void);
	void buildSeaport(void);
	void buildBorders(void);
	void buildShip(void);
	void buildShipOutline(void);
	void buildExtra(void);
	CVector calcPointVector(int i, int j);

	/////////////////////////////////////////////////////////////////////////
	// model functions
	/////////////////////////////////////////////////////////////////////////
	void road(double x, double y, double z, double dRot);
	void ship(double x, double y, double z, double dRot);
	void shipoutline(double x, double y, double z, double dRot);
	void hexagon(double x, double y, double z, double dRot);
	void hexagonfan(double x, double y, double z, double dRot, int iName = 0);
	void hexagonoutline(double x, double y, double z);
	void settlement(double x, double y, double z);
	void city(double x, double y, double z);
	void disc(double x, double y, double z, double dScale = 1.0);
	void ring(double x, double y, double z, double dThickness, double dScale = 1.0);
	void cylinder(double x, double y, double z, double dScale = 1.0);
	void discoutline(double x, double y, double z, double dScale = 1.0);
	void adjust(int i, double &x, double &z);
	void adjustroad(int i, double &x, double &z);
	void robber(double x, double y, double z);
	void seaport(double x, double y, double z, double dRot);
	void border(int iBorder, double x, double y, double z, double dRot);
	void extra(double x, double y, double z, double dRot);

protected:
	//graphics deatil
	int m_iDetail;

	//euro textures?
	BOOL m_bEuro;

	//rotation
	CPoint m_ptOld;
	GLdouble m_dxRot;
	GLdouble m_dyRot;

	//camera
	float m_dCamera[3];

	//look point
	float m_dLook[3];

	//drawing needs
	CPalette    m_cPalette;
	CClientDC   *m_pDC;
	HGLRC		m_hrc;

	//quadric object
	GLUquadricObj *m_pQuad;

	//selection?
	BOOL m_bUseSelection;

	//hit tests on mouse moves?
	BOOL m_bHitTest;

	//highlight selectable objects?
	BOOL m_bHighlight;

	//outline white color player
	BOOL m_bWhiteOutline;

	//selection
	int m_iSelection;

	//draw the seafarers borders?
	BOOL m_bBorders;

	//draw the seafarers ports?
	BOOL m_bSeaTiles;

	//textures
	BOOL m_bMip;
	GLuint m_texBase[GL_BASE_TEXTURES];
	GLuint m_texChip34[GL_TEX_CHIP34];
	GLuint m_texChip56[GL_TEX_CHIP56];
	GLuint m_texChipSea[GL_TEX_CHIPSEA];
	GLuint m_texSeaPorts[GL_TEX_SEAPORTS];
	GLuint m_texSeaBorders[GL_TEX_SEABORDERS];

	//piece textures
	GLuint m_texPieces[GL_TEX_PIECES];

	//display lists
	GLuint m_lists[GL_DISPLAY_LISTS];

	//use gouraud?
	BOOL m_bGouraud;

	//gouraud normals
	CVector m_normal[GL_ROBBER_STACK][GL_ROBBER_SLICE];

	//material colors needed
	GLMaterial glGray;
	GLMaterial glWhite;
	GLMaterial glGold;
	GLMaterial glRed;
	GLMaterial glEdge;
	GLMaterial glBlack;
	GLMaterial glHighlight;
	GLMaterial glWhiteOutline;
	GLMaterial glLongestOutline;
	GLMaterial glLava;

	// Generated message map functions
protected:
	//{{AFX_MSG(CGLBaseView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLBASEVIEW_H__73DD52E2_D974_4F9D_8AA6_818B540FD041__INCLUDED_)
