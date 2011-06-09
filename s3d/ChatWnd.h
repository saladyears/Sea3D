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
#include "ChatControl.h"
#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CChatWnd
/////////////////////////////////////////////////////////////////////////////
class CChatWnd : public CWnd
{
	DECLARE_DYNAMIC(CChatWnd)

public:
	CChatWnd();
	virtual ~CChatWnd();

	void setControls(void);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSendButton();
public:
	afx_msg void OnPaint();

protected:
	//chat controls
	CChatControl m_SendEdit;
	CChatControl m_ChatEdit;
	CChatControl m_MessageEdit;

	//send button
	CButtonST m_SendButton;

	//button font
	CFont m_font;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


