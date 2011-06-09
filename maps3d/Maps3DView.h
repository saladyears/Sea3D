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

#if !defined(AFX_MAPS3DVIEW_H__D9142C00_9791_4779_901D_88B38398EBD3__INCLUDED_)
#define AFX_MAPS3DVIEW_H__D9142C00_9791_4779_901D_88B38398EBD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "Maps3DDoc.h"
#include "GLMapView.h"
#include "ControlPanelWnd.h"
#include "defineGame.h"

/////////////////////////////////////////////////////////////////////////////
// main maps view
/////////////////////////////////////////////////////////////////////////////
class CMaps3DView : public CGLMapView
{
protected: // create from serialization only
	CMaps3DView();
	DECLARE_DYNCREATE(CMaps3DView)

// Attributes
public:
	CMaps3DDoc* GetDocument();

	//return the control panel
	CControlPanelWnd *controlPanel(void) {return m_pControl;}

	//image lists
	CImageList *imageList(int nList);

	//resource drawing
	void setResource(int n);
	int getResource(void) {return m_nRes;}

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaps3DView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMaps3DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//Implementation
protected:
	//create all needed image lists
	void createImageLists(void);

	//create a certain set of image lists
	void createImageGroup(int nSize, CString strFile, CImageList **p16, CImageList **p32, CImageList **p64);

	//texture reading function
	void readIL(int iSize, CString strFile, ILuint *pIL);

	//cleanup
	void deleteImageLists(void);

	//cursors
	void loadCursors(void);
	void deleteCursors(void);

//Data members
protected:
	//the currently selected tile resource
	int m_nRes;

	//the control panel
	CControlPanelWnd *m_pControl;

	//image lists 16x16
	CImageList *m_pImageLand16;
	CImageList *m_pImageSea16;
	CImageList *m_pImageSpecial16;
	CImageList *m_pImageChits16;

	//image lists 32x32
	CImageList *m_pImageLand32;
	CImageList *m_pImageSea32;
	CImageList *m_pImageSpecial32;
	CImageList *m_pImageChits32;

	//image lists 64x64
	CImageList *m_pImageLand64;
	CImageList *m_pImageSea64;
	CImageList *m_pImageSpecial64;
	CImageList *m_pImageChits64;

	//the cursors
	HCURSOR m_hCursors[RES_SIZE + RES_CHIT_SIZE];

// Generated message map functions
protected:
	//{{AFX_MSG(CMaps3DView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Maps3DView.cpp
inline CMaps3DDoc* CMaps3DView::GetDocument()
   { return (CMaps3DDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPS3DVIEW_H__D9142C00_9791_4779_901D_88B38398EBD3__INCLUDED_)
