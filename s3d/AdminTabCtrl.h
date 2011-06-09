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
#include "CustomTabCtrl.h"
#include "ChatWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CAdminTabCtrl
/////////////////////////////////////////////////////////////////////////////
class CAdminTabCtrl : public CCustomTabCtrl
{
	DECLARE_DYNAMIC(CAdminTabCtrl)

public:
	CAdminTabCtrl();
	virtual ~CAdminTabCtrl();

	void setControls(void) {if(NULL != m_pChatWnd) m_pChatWnd->setControls();}

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//the chat window
	CChatWnd *m_pChatWnd;
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


