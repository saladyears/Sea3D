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

#if !defined(AFX_GLMAPVIEW_H__6E764081_0CE4_4181_8E98_83A9BDCAAD0E__INCLUDED_)
#define AFX_GLMAPVIEW_H__6E764081_0CE4_4181_8E98_83A9BDCAAD0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GLMapView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "GLBaseView.h"
#include "S3DMap.h"
#include "defineMaps3D.h"

/////////////////////////////////////////////////////////////////////////////
// CGLMapView view
/////////////////////////////////////////////////////////////////////////////
class CGLMapView : public CGLBaseView
{
protected:
	CGLMapView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGLMapView)

// Attributes
public:
	//set a new drawing mode
	void setDrawMode(int nMode);

// Operations
public:
	/////////////////////////////////////////////////////////////////////////
	// drawing related functions
	/////////////////////////////////////////////////////////////////////////
	void DrawScene();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLMapView)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGLMapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CGLMapView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//Implementation
protected:
	//drawing functions
	void drawBase(void);
	void drawNormal(void);
	void drawResPaint(void);
	void drawPortPaint(void);
	void drawChitPaint(void);

	//drawing helpers
	void drawTile(int nTile);
	void drawChit(int nTile);
	void drawTileOutline(int nTile, GLMaterial *mat, int iWidth = 3.0);

	//reset GL view settings
	void resetEye(void);

	//selection
	void setSelection(BOOL b);
	void SelectObject(int id);
	void handleSelect(void);

	//selection particulars
	void selectResPaint(void);
	void selectPortPaint(void);
	void selectChitPaint(void);

	//reset map variable state
	void resetMapState(void);

	//reset tile draw array
	void resetTileState(int nInit = GL_TILE_BIT_LIGHT, BOOL bNoSelectBorders = TRUE);

//Data members
protected:
	//the current map
	CS3DMap *m_pMap;

	//the current draw mode
	int m_nMode;

	//the number of tiles in the map
	int m_nTiles;

	//the last selected tile
	int m_nLastTile;

	//the tile draw commands
	vector <int> m_vnTiles;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLMAPVIEW_H__6E764081_0CE4_4181_8E98_83A9BDCAAD0E__INCLUDED_)
