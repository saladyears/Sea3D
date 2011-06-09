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

#if !defined(AFX_GOODSBUTTON_H__735F68D5_5222_4DEE_9B37_C326F718BFB2__INCLUDED_)
#define AFX_GOODSBUTTON_H__735F68D5_5222_4DEE_9B37_C326F718BFB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GoodsButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HoverStatic.h"
#include "BtnSt.h"

/////////////////////////////////////////////////////////////////////////////
// CGoodsButton window

class CGoodsButton : public CButton
{
// Construction
public:
	CGoodsButton();

// Attributes
public:

	//create the buttons
	void createControls(BOOL bVolcano = FALSE);

	//set color background
	void setBackground(int iPlayer, COLORREF color);

	//show/hide a goods window
	void showWindow(int iPlayer, int iShow);

	//set goods text
	void setText(int iPlayer, CString strText);

	//set dice rolls
	void setDice(int iDie1, int iDie2);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGoodsButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGoodsButton();

	// Generated message map functions
protected:
	//rol buttons
	CButtonST m_btnRoll1;
	CButtonST m_btnRoll2;

	//goods buttons
	CHoverStatic m_btnGoods[6];

	//hand cursor
	HCURSOR m_hCursor;

	//are we a volcano button?
	BOOL m_bVolcano;

	//{{AFX_MSG(CGoodsButton)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnGoodsStatic();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOODSBUTTON_H__735F68D5_5222_4DEE_9B37_C326F718BFB2__INCLUDED_)
