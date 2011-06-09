/* 
 * Settlers3D - Copyright (C) 2001-2003 Jason Fugate (jfugate@settlers3d.net) 
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the Free 
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version. 
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 * for more details. 
 */

#include "stdafx.h"
#include "settlers.h"
#include "SortListCtrl.h"
#include "XLogfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//string comparison function
int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl

CSortListCtrl::CSortListCtrl()
{
	//set the default sort
	::ZeroMemory(m_iSort, sizeof(m_iSort));
	::ZeroMemory(m_bReverse, sizeof(m_bReverse));

	//no selection
	m_nSelection = -1;

	//callbacks
	m_lpfnClick = NULL;
	m_dwCookie = 0;
}

CSortListCtrl::~CSortListCtrl()
{
	//delete any font object
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSortListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdrawList)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
// initialize everything
/////////////////////////////////////////////////////////////////////////////
void CSortListCtrl::PreSubclassWindow() 
{
	XLOGFONT font;

	//create a font
	font = GetLogFont(13, "Tahoma");
	
	//set the font to what we want
	m_font.CreateFontIndirect((LOGFONT *) &font);
	
	CListCtrl::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// handle custom drawing
/////////////////////////////////////////////////////////////////////////////
void CSortListCtrl::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.

	if(CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if(CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		//set the font
		SelectObject(pLVCD->nmcd.hdc, m_font);

		// Tell Windows to paint the control itself.
		*pResult = CDRF_NEWFONT;
	}
}

/////////////////////////////////////////////////////////////////////////////
// try selection
/////////////////////////////////////////////////////////////////////////////
void CSortListCtrl::OnClick(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	POSITION pos;
	CPoint point;
	LVHITTESTINFO lvHit;

	if(GetStyle() & LVS_SINGLESEL)
	{
		//get current mouse position
		GetCursorPos(&point);
		ScreenToClient(&point);

		//set it
		lvHit.pt = point;
		
		//perform hittest
		SubItemHitTest(&lvHit);
		
		//set the selection
		m_nSelection = lvHit.iItem;
	}
	else
	{
		//see if there are any items selected
		pos = GetFirstSelectedItemPosition();
		
		//see if we have no selection
		if(NULL == pos)
		{
			m_nSelection = -1;
		}
		else
		{
			//set the selection
			m_nSelection = GetNextSelectedItem(pos);

			//see if there's multiple selections
			if(NULL != pos)
			{
				m_nSelection = -2;
			}
		}
	}

	//redraw
	Invalidate();

	//try callback
	if(NULL != m_lpfnClick)
	{
		m_lpfnClick(m_dwCookie);
	}
}

/////////////////////////////////////////////////////////////////////////////
// set sorting parameters
/////////////////////////////////////////////////////////////////////////////
void CSortListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	//move the sort actions along
	if(pNMListView->iSubItem != m_iSort[0])
	{
		m_iSort[2] = m_iSort[1];
		m_iSort[1] = m_iSort[0];
		m_iSort[0] = pNMListView->iSubItem;

		//move along the reverses
		m_bReverse[2] = m_bReverse[1];
		m_bReverse[1] = m_bReverse[0];
		m_bReverse[0] = FALSE;
	}
	//clicks on the same header: reverse the sort order
	else
	{
		m_bReverse[0] = !m_bReverse[0];
	}

	//call the sort function
	sort();
	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// sort correctly
/////////////////////////////////////////////////////////////////////////////
void CSortListCtrl::sort()
{
	int i;
	int iItems;
	int nSaveSel;
	vector <int> iSave;

	//get the number of items
	iItems = GetItemCount();
	iSave.resize(iItems);

	//save current selection
	nSaveSel = m_nSelection;

	//save and set the item data
	for(i = 0; i < iItems; i++)
	{
		iSave[i] = GetItemData(i);
		SetItemData(i, (DWORD) i);
	}

	//now sort
	SortItems(Compare, (LPARAM) this);

	//reset the saved selection
	for(i = 0; i < iItems; i++)
	{
		if((int) GetItemData(i) == nSaveSel)
		{
			m_nSelection = i;
		}
	}

	//now set the item data back correctly
	for(i = 0; i < iItems; i++)
	{
		//set saved selection
		SetItemData(i, iSave[GetItemData(i)]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// sort function
/////////////////////////////////////////////////////////////////////////////
int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int i;
	int iRet;
	int iLength1, iLength2;
	BOOL bNumeric;
	CString str1, str2;
	CString zeroes;
	CSortListCtrl *pList;
	 
	//get the list control
	pList = (CSortListCtrl *) lParamSort;

	//set the zeroes string to use as a buffer
	zeroes = "00000000000000000000000000000000";

	//run through each sort selection
	for(i = 0; i < 3; i++)
	{
		//get the sort column
		str1 = pList->GetItemText(lParam1, pList->m_iSort[i]);
		str2 = pList->GetItemText(lParam2, pList->m_iSort[i]);

		//see if they're numeric
		if((FALSE == str1.IsEmpty()) && (FALSE == str2.IsEmpty()) &&
			(TRUE == IS_NUMBER(str1[0])) && (TRUE == IS_NUMBER(str2[0])))
		{
			bNumeric = TRUE;
		}
		else
		{
			bNumeric = FALSE;
		}

		//pad numeric values
		if(TRUE == bNumeric)
		{
			//get string lenghts
			iLength1 = str1.GetLength();
			iLength2 = str2.GetLength();

			//pad the shorter string with zeroes
			if(iLength1 < iLength2)
			{
				str1 = zeroes.Left(iLength2 - iLength1) + str1;
			}
			else if(iLength2 < iLength1)
			{
				str2 = zeroes.Left(iLength1 - iLength2) + str2;
			}
		}

		//if they're not equal, sort
		iRet = stricmp(str1, str2);

		if(0 != iRet)
		{
			//see if it needs to be reversed
			if(pList->m_bReverse[i])
			{
				iRet = -iRet;
			}

			return iRet;
		}
		//otherwise, continue
	}

	//if we get here, it means that all sort columns were exactly the same
	return 0;
}