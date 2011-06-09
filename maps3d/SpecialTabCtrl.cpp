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
#include "maps3d.h"
#include "SpecialTabCtrl.h"
#include "define.h"
#include "defineTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecialTabCtrl

CSpecialTabCtrl::CSpecialTabCtrl()
{
	m_pSpecialWnd = NULL;
}

CSpecialTabCtrl::~CSpecialTabCtrl()
{
	if(NULL != m_pSpecialWnd)
	{
		delete m_pSpecialWnd;
		m_pSpecialWnd = NULL;
	}
}


BEGIN_MESSAGE_MAP(CSpecialTabCtrl, CCustomTabCtrl)
	//{{AFX_MSG_MAP(CSpecialTabCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecialTabCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
int CSpecialTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	int nDeflate;
	CRect rect;

	if (CCustomTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//add the tabs
	InsertItem(0, "Ports");
	InsertItem(1, "Random");
	InsertItem(2, "Hidden");
	InsertItem(3, "Chitless");

	//set deflate
	nDeflate = TAB_CURVE >> 1;

	//get client area
	GetClientRect(&rect);
	rect.DeflateRect(nDeflate, nDeflate, nDeflate, nDeflate + TAB_ITEM_HEIGHT);

	//create the random window
	m_pSpecialWnd = new CSpecialWnd;
	m_pSpecialWnd->Create(NULL, "", CHILD, rect, this, 0);
	m_pSpecialWnd->ShowWindow(SW_SHOW);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// handle tab message
/////////////////////////////////////////////////////////////////////////////
void CSpecialTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//update the special window
	m_pSpecialWnd->setMode(GetCurSel());

	*pResult = 0;
}
