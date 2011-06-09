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
#include "TilesTabCtrl.h"
#include "Maps3DView.h"
#include "define.h"
#include "defineTab.h"
#include "defineMaps3D.h"
#include "defineGame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTilesTabCtrl

/////////////////////////////////////////////////////////////////////////////
// constructor
/////////////////////////////////////////////////////////////////////////////
CTilesTabCtrl::CTilesTabCtrl()
{
	m_pListLand = NULL;
	m_pListSea = NULL;
	m_pListSpecial = NULL;
	m_pListChits = NULL;
	m_pCurrent = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// destructor
/////////////////////////////////////////////////////////////////////////////
CTilesTabCtrl::~CTilesTabCtrl()
{
	//cleanup
	deleteListCtrls();
}


BEGIN_MESSAGE_MAP(CTilesTabCtrl, CCustomTabCtrl)
	//{{AFX_MSG_MAP(CTilesTabCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTilesTabCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
// tab creation
/////////////////////////////////////////////////////////////////////////////
int CTilesTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect rect;
	int nDeflate;
	CImageList *pImageList;

	if (CCustomTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//add the tabs
	InsertItem(0, "Land");
	InsertItem(1, "Sea");
	InsertItem(2, "Special");
	InsertItem(3, "Chits");

	//set deflate
	nDeflate = TAB_CURVE >> 1;

	//get client area
	GetClientRect(&rect);
	rect.DeflateRect(nDeflate, nDeflate, nDeflate, nDeflate + TAB_ITEM_HEIGHT);

	//retrieve the land image list
	pImageList = VIEW->imageList(IMAGE_LAND_32);

	//create it
	createListCtrl(TEX_LAND_SIZE, RES_TIMBER, &m_pListLand, pImageList, rect, LIST_ICON_SPACING);

	//retrieve the sea image list
	pImageList = VIEW->imageList(IMAGE_SEA_32);

	//create it
	createListCtrl(TEX_SEA_SIZE, RES_OCEAN, &m_pListSea, pImageList, rect, LIST_ICON_SPACING);

	//retrieve the special image list
	pImageList = VIEW->imageList(IMAGE_SPECIAL_32);

	//create it
	createListCtrl(TEX_SPECIAL_SIZE, RES_RANDOM, &m_pListSpecial, pImageList, rect, LIST_ICON_SPACING);

	//chits
	pImageList = VIEW->imageList(IMAGE_CHITS_32);

	//create it
	createListCtrl(TEX_CHITS_SIZE, RES_CHIT_2, &m_pListChits, pImageList, rect, LIST_CHIT_SPACING);

	//show current
	m_pCurrent = m_pListLand;
	m_pCurrent->ShowWindow(SW_SHOW);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// list control creation
/////////////////////////////////////////////////////////////////////////////
void CTilesTabCtrl::createListCtrl(int nSize, int nOffset, CCustomListCtrl **pList, CImageList *pImage, CRect &rect, int nSpacing)
{
	int i;

	//create the list control
	(*pList) = new CCustomListCtrl;
	(*pList)->Create(LIST_FLAGS | CHILD, rect, this, 0);
	(*pList)->SetImageList(pImage, LVSIL_NORMAL);
	(*pList)->SetIconSpacing(nSpacing, 1);
	(*pList)->setIconSize(nSpacing);
	(*pList)->ShowWindow(SW_HIDE);

	//insert items
	for(i = 0; i < nSize; i++)
	{
		(*pList)->InsertItem(i, "", i);
		(*pList)->SetItemData(i, nOffset + i);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle tab message
/////////////////////////////////////////////////////////////////////////////
void CTilesTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//hide current
	if(NULL != m_pCurrent)
	{
		m_pCurrent->resetSel();
		m_pCurrent->ShowWindow(SW_HIDE);
	}

	//no res selected
	VIEW->setResource(RES_BLANK);

	//get new
	switch(GetCurSel())
	{
	case 0: m_pCurrent = m_pListLand; break;
	case 1: m_pCurrent = m_pListSea; break;
	case 2: m_pCurrent = m_pListSpecial; break;
	case 3: m_pCurrent = m_pListChits; break;
	}

	//show new
	if(NULL != m_pCurrent)
	{
		m_pCurrent->ShowWindow(SW_SHOW);
	}
	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// cleanup
/////////////////////////////////////////////////////////////////////////////
void CTilesTabCtrl::deleteListCtrls()
{
	if(NULL != m_pListLand)
	{
		delete m_pListLand;
		m_pListLand = NULL;
	}

	if(NULL != m_pListSea)
	{
		delete m_pListSea;
		m_pListSea = NULL;
	}

	if(NULL != m_pListSpecial)
	{
		delete m_pListSpecial;
		m_pListSpecial = NULL;
	}

	if(NULL != m_pListChits)
	{
		delete m_pListChits;
		m_pListChits = NULL;
	}
}