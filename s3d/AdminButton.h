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

#pragma once

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "AnimateButton.h"
#include "AdminTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CAdminButton
/////////////////////////////////////////////////////////////////////////////
class CAdminButton : public CAnimateButton
{
	DECLARE_DYNAMIC(CAdminButton)

public:
	CAdminButton();
	virtual ~CAdminButton();

	void setControls(void) {if(NULL != m_pTabAdmin) m_pTabAdmin->setControls();}

	//overrides
	void updateAnimate(void);

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	//overrides
	void startShow(BOOL bShow);
	void moveOffset(int nOffset);
public:
	afx_msg void OnPaint();

protected:
	//the tab control
	CAdminTabCtrl *m_pTabAdmin;

	//current cursor
	HCURSOR m_hCursor;

	//can we drag?
	BOOL m_bDraggable;
	BOOL m_bDragging;
	// Which way to size? (SIZE_LEFT, SIZE_TOP, SIZE_TOPLEFT)
	char m_cSizeDirection;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


