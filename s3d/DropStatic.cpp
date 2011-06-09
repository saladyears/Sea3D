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
#include "DropStatic.h"
#include "NewGameDialog.h"
#include "DropListCtrl.h"
#include "PlayerDatabase.h"
#include "defineColors.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDropStatic::CDropStatic()
{
	//null pointer
	m_pList = NULL;

	//index is -1
	m_iIndex = -1;

	//not dragging
	m_bDrag = FALSE;
}

CDropStatic::~CDropStatic()
{

}

BEGIN_MESSAGE_MAP(CDropStatic, CHoverStatic)
	//{{AFX_MSG_MAP(CDropStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// process the dropped string
/////////////////////////////////////////////////////////////////////////////
void CDropStatic::ProcessData(CString strData)
{
	int idx;
	int iTab;
	BOOL bText;
	CString strItem;
	CDropStatic *pButton;

	//see if we've got text
	bText = !GetText().IsEmpty();

	//save old in case we need it later
	strItem.Format("%d\t%d", m_iIndex, -2);

	//unformat the text
	iTab = strData.ReverseFind('\t');

	//get the index
	idx = atoi(strData.Right(strData.GetLength() - iTab - 1));

	//chop it off
	strData = strData.Left(iTab);
	
	//remove from the list
	if(idx >= 0)
	{
		m_pList->DeleteItem(idx);

		//increment players
		((CNewGameDialog *) GetParent())->incPlayers();

		//see if we've got something
		if(bText)
		{
			//fool it
			m_bDrag = TRUE;
			((CNewGameDialog *) GetParent())->setDropButton(this);
			m_pList->ProcessData(strItem);
		}
	}
	//we came from a button
	else if(idx == -1)
	{
		pButton = ((CNewGameDialog *) GetParent())->getDropButton();

		if(NULL != pButton)
		{
			//see if we've got something
			if(bText)
			{
				//add it back
				pButton->dropItem();
				pButton->ProcessData(strItem);
				pButton->Invalidate();
			}
			else
			{
				pButton->dropItem();
			}
		}

		//increment players
		((CNewGameDialog *) GetParent())->incPlayers();
	}

	//get the index
	m_iIndex = atoi(strData);

	//set the text
	SetText(PLAYERS->getPlayer(m_iIndex).getName());
}

/////////////////////////////////////////////////////////////////////////////
// handle dragging
/////////////////////////////////////////////////////////////////////////////
void CDropStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//reset all dragging
	((CNewGameDialog *) GetParent())->resetAllDrags();
	
	//turn on dragging
	if(!GetText().IsEmpty()) 
	{
		m_bDrag = TRUE;
		((CNewGameDialog *) GetParent())->setDropButton(this);
	}

	//set the mouse capture
	SetCapture();

	CHoverStatic::OnLButtonDown(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// handle dragging
/////////////////////////////////////////////////////////////////////////////
void CDropStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rect;
	CString strItem;

	//default handling
	CHoverStatic::OnMouseMove(nFlags, point);

	//see if we're dragging
	if(!m_bDrag) return;

	//create the drag&drop source and data objects
	COleDropSource *pDropSource = new COleDropSource;
	COleDataSource *pDataSource = new COleDataSource;

	//get the item text
	strItem.Format("%d\t%d", m_iIndex, -1);

	//get the bounding rectangle
	GetClientRect(&rect);

	TRY
	{
		//create the shared file
		CSharedFile file(GMEM_ZEROINIT|GMEM_DDESHARE|GMEM_MOVEABLE);
		TRY
		{
			//create the archive
			CArchive ar(&file, CArchive::store);
			TRY
			{
				// for CF_TEXT use a flat ASCII-stream
				ar.WriteString(strItem);    
				ar.Close();
			}
			CATCH_ALL(eInner)
			{
				// exception while writing into or closing the archive
				ASSERT(FALSE);
			}
			END_CATCH_ALL;
		}
		CATCH_ALL(eMiddle)
		{
			// exception in the destructor of ar
			ASSERT(FALSE);
		}
		END_CATCH_ALL;

		// put the file object into the data object
		pDataSource->CacheGlobalData(CF_TEXT, file.Detach());
		pDataSource->DoDragDrop(DROPEFFECT_MOVE|DROPEFFECT_COPY, &rect, pDropSource);
	}
	CATCH_ALL(eOuter)
	{
		// exception while destructing the file
		ASSERT(FALSE);
	}
	END_CATCH_ALL;

	delete pDropSource;
	delete pDataSource;
}

/////////////////////////////////////////////////////////////////////////////
// handle dragging
/////////////////////////////////////////////////////////////////////////////
void CDropStatic::dropItem()
{
	if(m_bDrag)
	{
		//remove data
		SetText("");
		m_iIndex = -1;

		//turn off dragging
		m_bDrag = FALSE;

		//decrement players
		((CNewGameDialog *) GetParent())->decPlayers();

		//no longer dragging
		((CNewGameDialog *) GetParent())->setDropButton(NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// drag/drop handler
/////////////////////////////////////////////////////////////////////////////
HRESULT CDropStatic::dragOver()
{
	//see if we're the origin
	if(m_bDrag) return S_FALSE;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// drag/drop handler
/////////////////////////////////////////////////////////////////////////////
HRESULT CDropStatic::dragLeave()
{
	//see if we're the origin
	if(m_bDrag) return S_FALSE;

	//return to the border
	m_colorBorder = COLOR_BLACK;
	Invalidate();

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// drag/drop handler
/////////////////////////////////////////////////////////////////////////////
HRESULT CDropStatic::dragEnter()
{
	//see if we're the origin
	if(m_bDrag) return S_FALSE;

	//set a gold outline
	m_colorBorder = COLOR_GOLDOUTLINE;
	Invalidate();

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// drag/drop handler
/////////////////////////////////////////////////////////////////////////////
void CDropStatic::drop()
{
	//return to the border
	m_colorBorder = COLOR_BLACK;
	Invalidate();
}