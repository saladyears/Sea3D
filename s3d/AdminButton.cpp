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
#include "AdminButton.h"
#include "SettlersView.h"
#include "defineDraw.h"
#include "defineColors.h"
#include "defineTab.h"
#include "defineIni.h"
#include ".\adminbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CAdminButton
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CAdminButton, CAnimateButton)
CAdminButton::CAdminButton()
{
	m_bDraggable = FALSE;
	m_bDragging = FALSE;
	m_hCursor = NULL;
	m_cSizeDirection = 0;

	//update the animate flag
	updateAnimate();
}

CAdminButton::~CAdminButton()
{
	//cleanup
	if(NULL != m_pTabAdmin)
	{
		delete m_pTabAdmin;
		m_pTabAdmin = NULL;
	}
}


BEGIN_MESSAGE_MAP(CAdminButton, CAnimateButton)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CAdminButton message handlers

/////////////////////////////////////////////////////////////////////////////
// window setup
/////////////////////////////////////////////////////////////////////////////
BOOL CAdminButton::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= BS_OWNERDRAW;

	return CAnimateButton::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// window setup
/////////////////////////////////////////////////////////////////////////////
int CAdminButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CAnimateButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	//read admin width & height from .ini file
	int nWidth, nHeight;
	nWidth= GetPrivateProfileInt(INI_DISPLAY, INI_EMBED_WIDTH, PANEL_WIDTH, INI_FILE);
	nHeight = GetPrivateProfileInt(INI_DISPLAY, INI_EMBED_HEIGHT, ADMIN_HEIGHT, INI_FILE);

	//create the tab
	m_pTabAdmin = new CAdminTabCtrl;
	m_pTabAdmin->Create(TAB_FLAGS | CHILD, CRect(0, 0, nWidth, nHeight), this, 0);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// handle drawing
/////////////////////////////////////////////////////////////////////////////
void CAdminButton::OnPaint()
{
	CRect rect;
	CPaintDC dc(this); // device context for painting

	//get the client area
	GetClientRect(&rect);

	//fill the background
	dc.FillSolidRect(rect, COLOR_BLACK);
}

/////////////////////////////////////////////////////////////////////////////
// animate the show/hide
/////////////////////////////////////////////////////////////////////////////
void CAdminButton::startShow(BOOL bShow)
{
	CRect rect;

	//get dimensions
	GetClientRect(&rect);

	if(TRUE == bShow)
	{
		drawOffset(0);
	}
	else
	{
		drawOffset(rect.Width() + 1);
	}
}

/////////////////////////////////////////////////////////////////////////////
// actually move the thing
/////////////////////////////////////////////////////////////////////////////
void CAdminButton::moveOffset(int nOffset)
{
	CRect rect;
	CRect rectAdmin;
	
	//get view dimensions
	VIEW->GetClientRect(&rect);
	GetClientRect(&rectAdmin);

	//resize
	SetWindowPos(NULL, rect.Width() - rectAdmin.Width() - 1 + nOffset, rect.Height() - rectAdmin.Height(), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

/////////////////////////////////////////////////////////////////////////////
// update the animate value
/////////////////////////////////////////////////////////////////////////////
void CAdminButton::updateAnimate()
{
	CRect rect;

	m_bAnimate = GetPrivateProfileInt(INI_GENERAL, INI_ANIMATE, 1, INI_FILE);

	//reset offset if needed
	if(FALSE == m_bAnimate)
	{
		//hide the window if needed
		if(0 != m_nOffset)
		{
			CWnd::ShowWindow(SW_HIDE);
		}

		//reset offset
		drawOffset(0);
	}
	//see if it needs to be moved
	else if(NULL != m_hWnd)
	{
		//get client area
		GetClientRect(&rect);

		if(FALSE == IsWindowVisible())
		{
			drawOffset(rect.Width() + 1);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle mouse move events
//  the sent point is the the change in pixels from the upper left corner of
//  the window.
/////////////////////////////////////////////////////////////////////////////
void CAdminButton::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	CRect window;
	CRect rectView;

	//if we're dragging, move the window
	if(TRUE == m_bDragging)
	{
		//get client rect
		GetClientRect(&rect);

		//get window rect
		GetWindowRect(&window);
		VIEW->ScreenToClient(&window);

		//convert the point to view coords
		ClientToScreen(&point);
		VIEW->ScreenToClient(&point);

		switch(m_cSizeDirection)
		{
		case SIZE_LEFT:
			point.y = window.top;
			break;
		case SIZE_TOP:
			point.x = window.left;
			break;
		case SIZE_TOPLEFT:
			break;
		}

		//cap at panel width
		if((window.right - point.x) < PANEL_WIDTH)
		{
			point.x = window.right - PANEL_WIDTH;
		}

		// same for panel height (min to ADMIN_HEIGHT)
		if((window.bottom - point.y) < ADMIN_HEIGHT)
		{
			point.y = window.bottom - ADMIN_HEIGHT;
		}

		VIEW->GetClientRect(&rectView);

		// Put a limit on how large, too (half the screen)...
		if((window.right - point.x) > rectView.Width() - 4 )
		{
			point.x = window.right - (rectView.Width() - 4);
		}

		// same for panel height
		if((window.bottom - point.y) > rectView.Height() - CONTROLS_HEIGHT - CONTROL_PANEL_GOODS_BUTTON - 12 )
		{
			point.y = window.bottom - (rectView.Height() - CONTROLS_HEIGHT - CONTROL_PANEL_GOODS_BUTTON - 12);
		}

		TRACE1("Width: %d\t", window.right - point.x);
		TRACE1("Height: %d\n", window.bottom - point.y);

		//set the new window position if needed
		if(point.x != window.left || point.y != window.top)
		{
			SetWindowPos(&wndTop, point.x, point.y, window.right - point.x, window.bottom - point.y, SWP_SHOWWINDOW);
		}
	}
	//otherwise, see where we are
	else
	{
		//load the correct cursor
		if(point.x < 4 && point.y < 4)
		{
			m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE);
			m_bDraggable = TRUE;
			m_cSizeDirection = SIZE_TOPLEFT;
		}
		else if(point.x < 4)
		{
			m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
			m_bDraggable = TRUE;
			m_cSizeDirection = SIZE_LEFT;
		}
		else if(point.y < 4)
		{
			m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZENS);
			m_bDraggable = TRUE;
			m_cSizeDirection = SIZE_TOP;
		}
		else
		{
			m_hCursor = NULL;
			m_bDraggable = FALSE;
		}
	}

	CAnimateButton::OnMouseMove(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// set the cursor properly
/////////////////////////////////////////////////////////////////////////////
BOOL CAdminButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(NULL != m_hCursor)
	{
		//set it
		::SetCursor(m_hCursor);
		return TRUE;
	}
	else
	{
		return CAnimateButton::OnSetCursor(pWnd, nHitTest, message);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle dragging
/////////////////////////////////////////////////////////////////////////////
void CAdminButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(TRUE == m_bDraggable)
	{
		//not draggable anymore
		m_bDraggable = FALSE;

		//we are now dragging
		m_bDragging = TRUE;

		//capture the mouse
		SetCapture();
	}

	CAnimateButton::OnLButtonDown(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// handle dragging
/////////////////////////////////////////////////////////////////////////////
void CAdminButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect rect;
	CString str;

	//if we are dragging, release it
	if(TRUE == m_bDragging)
	{
		//get size
		GetClientRect(&rect);

		//create string
		str.Format("%d", rect.Width());

		//set the new size
		WritePrivateProfileString(INI_DISPLAY, INI_EMBED_WIDTH, str, INI_FILE);

		//create string
		str.Format("%d", rect.Height());

		//set the new size
		WritePrivateProfileString(INI_DISPLAY, INI_EMBED_HEIGHT, str, INI_FILE);

		//no longer dragging
		m_bDragging = FALSE;

		//release mouse
		ReleaseCapture();
	}

	CAnimateButton::OnLButtonUp(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// handle resizes
/////////////////////////////////////////////////////////////////////////////
void CAdminButton::OnSize(UINT nType, int cx, int cy)
{
	CAnimateButton::OnSize(nType, cx, cy);

	if((NULL != m_pTabAdmin) && (NULL != m_pTabAdmin->m_hWnd))
	{
		m_pTabAdmin->PostMessage(WM_SIZE, (WPARAM) 0, MAKELPARAM(cx, cy));
	}
}
