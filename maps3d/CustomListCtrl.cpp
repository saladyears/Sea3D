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
#include "CustomListCtrl.h"
#include "Maps3DView.h"
#include "define.h"
#include "defineColors.h"
#include "defineTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomListCtrl

CCustomListCtrl::CCustomListCtrl()
{
	m_nSelection = -1;
	m_nIconSize = 0;
}

CCustomListCtrl::~CCustomListCtrl()
{
}


BEGIN_MESSAGE_MAP(CCustomListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCustomListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdrawList)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomListCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
// handle custom drawing
/////////////////////////////////////////////////////////////////////////////
void CCustomListCtrl::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nItem;
	CSize sizeSpacing;
	CDC *pDC;
	CRect rect;
	CImageList *pImage;
	COLORREF color;

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
	else if(CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		//get the image list
		pImage = GetImageList(LVSIL_NORMAL);

		//get the item
		nItem = pLVCD->nmcd.dwItemSpec;

		//get the item spacing
		sizeSpacing = SetIconSpacing(0, 0);
		SetIconSpacing(sizeSpacing);

		//get device context
		pDC = CDC::FromHandle(pLVCD->nmcd.hdc);

		//get the item rect
		GetItemRect(nItem, &rect, LVIR_ICON);

		//set normal color
		color = TAB_COLOR_BACKGROUND;

		//offset the rect if needed
		if(LIST_CHIT_SPACING == m_nIconSize)
		{
			rect.OffsetRect(3, 0);
		}

		//fill selection background
		if(nItem == m_nSelection)
		{
			//get highlight color
			color = ::GetSysColor(COLOR_HIGHLIGHT);

			//fill it with the selected color
			pDC->FillSolidRect(&rect, color);
		}

		//offset the rect if needed
		if(LIST_CHIT_SPACING == m_nIconSize)
		{
			rect.OffsetRect(-3, 0);
		}

		//now draw the item
		if(NULL != pImage)
		{
			pImage->DrawIndirect(pDC, nItem, CPoint(rect.left + 4, rect.top + 2), CSize(32, 32), CPoint(0, 0));
		}

		// Tell Windows to paint the control itself.
		*pResult = CDRF_SKIPDEFAULT;
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle selection
/////////////////////////////////////////////////////////////////////////////
void CCustomListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int i;
	int nRes = RES_BLANK;
	CRect rect;
	CPoint point;

	//get current mouse position
	GetCursorPos(&point);
	ScreenToClient(&point);

	//reset the selection
	m_nSelection = -1;

	//run throuch each item
	for(i = 0; i < GetItemCount(); i++)
	{
		GetItemRect(i, &rect, LVIR_ICON);

		//check it
		if(TRUE == rect.PtInRect(point))
		{
			m_nSelection = i;
			break;
		}
	}

	//get the item data
	if(-1 != m_nSelection)
	{
		nRes = GetItemData(m_nSelection);
	}

	//set it in the view
	VIEW->setResource(nRes);

	//redraw
	Invalidate();
}