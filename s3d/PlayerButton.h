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

#if !defined(AFX_PLAYERBUTTON_H__D5D367BD_E3D8_45DE_A66B_382DDEF9F6D5__INCLUDED_)
#define AFX_PLAYERBUTTON_H__D5D367BD_E3D8_45DE_A66B_382DDEF9F6D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayerButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HoverStatic.h"
#include "BtnST.h"
#include "AnimateButton.h"

/////////////////////////////////////////////////////////////////////////////
// CPlayerButton window

class CPlayerButton : public CAnimateButton
{
// Construction
public:
	CPlayerButton();

// Attributes
public:

// Operations
public:

	//sets the index of this button
	void setIndex(int i);
	void setOutline(COLORREF c) {m_colorOutline = c; Invalidate();}

	//update the resource counts
	void updateCounts(void);

	//update the goods display
	void updateDisplay(void);

	//update autohide stuff
	void updateAutohide(BOOL bFirst = FALSE);
	void handleAutohide(void);

	//calculate button width
	int buttonWidth(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerButton)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPlayerButton();

	// Generated message map functions
protected:

	//create functions
	void createText(void);
	void createIcons(void);
	void createRes(void);
	void createStock(void);

	//cleanup
	void resetAll(void);
	void cleanup(void);

	void setToolTipText(CString str);
	void setResTipText(int iRes, CString str);

	//overrides
	void moveOffset(int iOffset);
	void showItems(BOOL bShow);
	
	//large fonts in use?
	BOOL m_bLarge;

	//get the index
	int m_iIndex;

	//saved off point total
	int m_iPoints;

	//saved off dev cards total
	int m_iCards;

	//saved off resource totals
	int m_iRes[5];

	//jungles
	int m_nJungles;

	//the number of soldiers played
	int m_iSoldiers;

	//the stock amounts
	int m_iShip;
	int m_iRoad;
	int m_iSettle;
	int m_iCity;

	//road length
	int m_iRoadLength;

	//has longest road
	BOOL m_bLongestRoad;

	//are we tracking the mouse?
	BOOL m_bTracking;

	//auto-hide offsets and variables
	BOOL m_bAutohideNet;
	BOOL m_bAutohideOther;

	//colors for this madness
	COLORREF m_color;
	COLORREF m_colorOutline;

	//largest army
	CButtonST m_Army;
	CHoverStatic m_A;

	//longest road
	CButtonST m_Road;
	CHoverStatic m_R;

	//number of points
	CButtonST m_Points;
	CHoverStatic m_P;

	//bitmap pictures
	CStatic m_stGoods[6];

	//jungle picture
	CStatic m_stJungle;

	//stock pictures
	CStatic m_stShips;
	CStatic m_stRoads;
	CStatic m_stSettle;
	CStatic m_stCity;

	//tooltip for the dev card button
	CToolTipCtrl m_JungleTip;
	CToolTipCtrl m_ToolTip;
	CToolTipCtrl m_ResTips[5];

	//hoverstatic information
	CHoverStatic m_PlayerName;
	CHoverStatic m_hGoods[6];

	//jungle info
	CHoverStatic m_hJungle;

	//hoverstatic stock info
	CHoverStatic m_hShips;
	CHoverStatic m_hRoads;
	CHoverStatic m_hSettle;
	CHoverStatic m_hCity;

	//{{AFX_MSG(CPlayerButton)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERBUTTON_H__D5D367BD_E3D8_45DE_A66B_382DDEF9F6D5__INCLUDED_)
