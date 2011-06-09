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
#include "settlers.h"
#include "MapListCtrl.h"
#include "MapDatabase.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapListCtrl

CMapListCtrl::CMapListCtrl()
{
}

CMapListCtrl::~CMapListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMapListCtrl, CSortListCtrl)
	//{{AFX_MSG_MAP(CMapListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapListCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
// setup
/////////////////////////////////////////////////////////////////////////////
void CMapListCtrl::PreSubclassWindow() 
{
	CSortListCtrl::PreSubclassWindow();

	//insert columns
	InsertColumn(0, "Map Name", LVCFMT_LEFT, 150);
	InsertColumn(1, "O", LVCFMT_CENTER, 20, 1);
	InsertColumn(2, "# Players", LVCFMT_CENTER, 60, 2);
	InsertColumn(3, "Points", LVCFMT_CENTER, 45, 3);
	InsertColumn(4, "Map Creator", LVCFMT_LEFT, 120, 4);

	//set the full row selection
	SetExtendedStyle(LVS_EX_FULLROWSELECT);

	//read the maps
	loadMaps();
}

/////////////////////////////////////////////////////////////////////////////
// load maps
/////////////////////////////////////////////////////////////////////////////
void CMapListCtrl::loadMaps()
{
	int i;

	//run through the map list
	for(i = 0; i < MAPS->numMaps(); i++)
	{
		//load it up
		MAPS->loadMap(i);

		//populate table
		populateRow(i);
	}

	//reset selection
	m_nSelection = -1;
}

/////////////////////////////////////////////////////////////////////////////
// populate map data
/////////////////////////////////////////////////////////////////////////////
void CMapListCtrl::populateRow(int iIndex)
{
	CString str;

	//insert the row
	InsertItem(iIndex, MAP->m_strMapName);
	SetItemText(iIndex, 1, MAP->m_bOfficial ? "Y" : "N");
	
	if(MAP->m_iMinPlayers == MAP->m_iMaxPlayers)
	{
		str.Format("%d", MAP->m_iMinPlayers);
	}
	else
	{
		str.Format("%d-%d", MAP->m_iMinPlayers, MAP->m_iMaxPlayers);
	}
	SetItemText(iIndex, 2, str);

	str.Format("%d", MAP->m_iPoints);
	SetItemText(iIndex, 3, str);

	SetItemText(iIndex, 4, MAP->m_strMapCreator);

	//set item data
	SetItemData(iIndex, (DWORD) iIndex);
}

/////////////////////////////////////////////////////////////////////////////
// handle custom drawing
/////////////////////////////////////////////////////////////////////////////
void CMapListCtrl::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nItem;
	CDC* pDC = NULL;
	CRect rect;
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.

	if(CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		GetClientRect(&rect);

		//get device context
		pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		
		//clear background
		pDC->FillSolidRect(&rect, COLOR_BLACK);

		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if(CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		//set the font
		SelectObject(pLVCD->nmcd.hdc, m_font);

		//get item
		nItem = (int) pLVCD->nmcd.dwItemSpec;

		//set text color and highlight
		if(nItem == m_nSelection)
		{
			pLVCD->clrTextBk = ::GetSysColor(COLOR_HIGHLIGHT);
			pLVCD->clrText = COLOR_WHITE;
		}
		else
		{
			pLVCD->clrTextBk = COLOR_BLACK;
			pLVCD->clrText = COLOR_WHITE;
		}

		// Tell Windows to paint the control itself.
		*pResult = CDRF_NEWFONT;
	}
}