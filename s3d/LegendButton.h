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

#if !defined(AFX_LEGENDBUTTON_H__348709EF_DFFF_410D_894D_88E4B54A7B91__INCLUDED_)
#define AFX_LEGENDBUTTON_H__348709EF_DFFF_410D_894D_88E4B54A7B91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LegendButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HoverStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CLegendButton window

class CLegendButton : public CButton
{
// Construction
public:
	CLegendButton();

// Attributes
public:

// Operations
public:

	//sets the index of this button
	void setIndex(int i);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLegendButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLegendButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLegendButton)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	//index
	int m_iIndex;

	//bitmaps for resources
	CStatic m_btnRoad[2];
	CStatic m_btnShip[2];
	CStatic m_btnSettle[4];
	CStatic m_btnCity[2];
	CStatic m_btnDev[3];

	//bitmaps for images
	CStatic m_stRoad;
	CStatic m_stShip;
	CStatic m_stSettle;
	CStatic m_stCity;
	CStatic m_stDev;

	//hoverstatics
	CHoverStatic m_hRoad;
	CHoverStatic m_hShip;
	CHoverStatic m_hSettle;
	CHoverStatic m_hCity;
	CHoverStatic m_hDev;

	//equals signs
	CHoverStatic m_hEquals[5];

	//city costs
	CHoverStatic m_hCityCost[2];

	//outline color
	COLORREF m_color;

	//large fonts value
	BOOL m_bLarge;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEGENDBUTTON_H__348709EF_DFFF_410D_894D_88E4B54A7B91__INCLUDED_)
