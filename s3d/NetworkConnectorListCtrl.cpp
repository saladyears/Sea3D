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
#include "NetworkConnectorListCtrl.h"
#include "NetworkConnectorDialog.h"
#include "defineColors.h"
#include "defineConnector.h"
#include "defineDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkConnectorListCtrl

CNetworkConnectorListCtrl::CNetworkConnectorListCtrl()
{
}

CNetworkConnectorListCtrl::~CNetworkConnectorListCtrl()
{
}


BEGIN_MESSAGE_MAP(CNetworkConnectorListCtrl, CSortListCtrl)
	//{{AFX_MSG_MAP(CNetworkConnectorListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkConnectorListCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
// startup information
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorListCtrl::PreSubclassWindow() 
{
	//set the column information
	InsertColumn(0, "", LVCFMT_CENTER, 45);
	InsertColumn(1, "Ver", LVCFMT_CENTER, 39, 1);
	InsertColumn(2, "Game Name", LVCFMT_LEFT, 130, 2);
	InsertColumn(3, "Host Player", LVCFMT_LEFT, 75, 3);
	InsertColumn(4, "Map Name", LVCFMT_LEFT, 100, 4);
	InsertColumn(5, "Max", LVCFMT_CENTER, 40, 5);
	InsertColumn(6, "IP Address", LVCFMT_LEFT, 100, 7);
	InsertColumn(7, "Port", LVCFMT_LEFT, 40);
	
	CSortListCtrl::PreSubclassWindow();
}

///////////////////////////////////////////////////////////////////////////////
// overridden drawing for the control
///////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nState;
	int nItem;
	int nX, nY;
	CImageList *pList;
	CDC* pDC = NULL;
	CRect rect;

	//get the pointer
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we set this to something else below.
    *pResult = CDRF_DODEFAULT;

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
		//set the font
		SelectObject(pLVCD->nmcd.hdc, m_font);

		// Tell Windows to paint the control itself.
		*pResult = CDRF_NEWFONT | CDRF_NOTIFYSUBITEMDRAW;
	}
	else if((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		//see what row we are
		nItem = pLVCD->nmcd.dwItemSpec;

		//if it's the first sub item, draw needed bitmaps
		if(0 == pLVCD->iSubItem)
		{
			//get our rectangle
			GetSubItemRect(nItem, 0, LVIR_BOUNDS, rect);

			//set the offsets
			nX = rect.left + 1;
			nY = rect.top + 1;

			//get the image list
			pList = GetImageList(LVSIL_NORMAL);

			//get the state
			nState = GetItemData(nItem);

			//get device context
			pDC = CDC::FromHandle(pLVCD->nmcd.hdc);

			//draw bitmaps
			if((CONNECTOR_LIST_LADDER & nState) && (NULL != pList))
			{
				pList->DrawIndirect(pDC, CONNECT_LADDER, CPoint(nX + 1, nY), CSize(11, 11), CPoint(0, 0), ILD_NORMAL, SRCCOPY, CLR_NONE);
			}

			//private
			if((CONNECTOR_LIST_LOCK & nState) && (NULL != pList))
			{
				pList->DrawIndirect(pDC, CONNECT_LOCK, CPoint(nX + 15, nY), CSize(11, 11), CPoint(0, 0), ILD_NORMAL, SRCCOPY, CLR_NONE);
			}

			//restart
			if((CONNECTOR_LIST_RESTART & nState) && (NULL != pList))
			{
				pList->DrawIndirect(pDC, CONNECT_RESTART, CPoint(nX + 30, nY), CSize(11, 11), CPoint(0, 0), ILD_NORMAL, SRCCOPY, CLR_NONE);
			}

			//draw normal
			*pResult = CDRF_SKIPDEFAULT;
		}
		else
		{
			//check for highlight
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

			//draw normal
			*pResult = CDRF_DODEFAULT;
		}
	}
}
