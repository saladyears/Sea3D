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
#include "DropListCtrl.h"
#include "DropStatic.h"
#include "NewGameDialog.h"
#include "PlayerDatabase.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDropListCtrl

CDropListCtrl::CDropListCtrl()
{
}

CDropListCtrl::~CDropListCtrl()
{
}


BEGIN_MESSAGE_MAP(CDropListCtrl, CPlayerListCtrl)
	//{{AFX_MSG_MAP(CDropListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropListCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
// data processing
/////////////////////////////////////////////////////////////////////////////
void CDropListCtrl::ProcessData(CString strData)
{
	int idx;
	int iIns;
	int iTab;
	int iIndex;
	CDropStatic *pButton;

	//get the button in question
	pButton = ((CNewGameDialog *) GetParent())->getDropButton();

	//remove the data from the button
	if(pButton) pButton->dropItem();

	//unformat the text
	iTab = strData.ReverseFind('\t');

	//get the index
	idx = atoi(strData.Right(strData.GetLength() - iTab - 1));

	//chop it off
	strData = strData.Left(iTab);
	
	//get the id
	iIndex = atoi(strData);

	//insert the text in
	iIns = InsertItem(GetItemCount(), PLAYERS->getPlayer(iIndex).getCommaName());
	SetItemData(iIns, iIndex);

	//reset the selection
	setSelection();

	//call the sort function
	sort();
}

/////////////////////////////////////////////////////////////////////////////
// drag handling
/////////////////////////////////////////////////////////////////////////////
HRESULT CDropListCtrl::dragOver()
{
	//see if we're dragging over this thing
	if(((CNewGameDialog *) GetParent())->getDrag()) 
	{
		return S_FALSE;
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// drag handling
/////////////////////////////////////////////////////////////////////////////
HRESULT CDropListCtrl::dragLeave()
{
	//see if we're dragging over this thing
	if(((CNewGameDialog *) GetParent())->getDrag()) 
	{
		return S_FALSE;
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// drag handling
/////////////////////////////////////////////////////////////////////////////
HRESULT CDropListCtrl::dragEnter()
{
	//see if we're dragging over this thing
	if(((CNewGameDialog *) GetParent())->getDrag()) 
	{
		return S_FALSE;
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// drag handling
/////////////////////////////////////////////////////////////////////////////
void CDropListCtrl::drop()
{
}

/////////////////////////////////////////////////////////////////////////////
// handle custom drawing
/////////////////////////////////////////////////////////////////////////////
void CDropListCtrl::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
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