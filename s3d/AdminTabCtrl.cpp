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


#include "stdafx.h"
#include "Settlers.h"
#include "AdminTabCtrl.h"
#include "defineTab.h"
#include ".\admintabctrl.h"


// CAdminTabCtrl

IMPLEMENT_DYNAMIC(CAdminTabCtrl, CCustomTabCtrl)
CAdminTabCtrl::CAdminTabCtrl()
{
	m_pChatWnd = NULL;
}

CAdminTabCtrl::~CAdminTabCtrl()
{
	//cleanup
	if(NULL != m_pChatWnd)
	{
		delete m_pChatWnd;
		m_pChatWnd = NULL;
	}
}


BEGIN_MESSAGE_MAP(CAdminTabCtrl, CCustomTabCtrl)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CAdminTabCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
// set things up
/////////////////////////////////////////////////////////////////////////////
int CAdminTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int nDeflate;
	CRect rect;

	if (CCustomTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	//insert needed tabs
	InsertItem(0, "Chat");

	//set deflate
	nDeflate = TAB_CURVE >> 1;

	//get client area
	GetClientRect(&rect);
	rect.DeflateRect(nDeflate, nDeflate, nDeflate, nDeflate + TAB_ITEM_HEIGHT);

	//create the chat window
	m_pChatWnd = new CChatWnd;
	m_pChatWnd->Create(NULL, NULL, CHILD, rect, this, 0);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// reset the cursor
/////////////////////////////////////////////////////////////////////////////
BOOL CAdminTabCtrl::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// set new window size
/////////////////////////////////////////////////////////////////////////////
void CAdminTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	int nDeflate;
	CRect rect;

	CCustomTabCtrl::OnSize(nType, cx, cy);

	//reset window position
	SetWindowPos(&wndTop, 0, 0, cx, cy, SWP_SHOWWINDOW);
	
	//set deflate
	nDeflate = TAB_CURVE >> 1;

	//get client area
	GetClientRect(&rect);
	rect.DeflateRect(nDeflate, nDeflate, nDeflate, nDeflate + TAB_ITEM_HEIGHT);

	//resize the chat window
	m_pChatWnd->SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
}
