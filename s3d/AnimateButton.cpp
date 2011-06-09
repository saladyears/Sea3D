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
#include "AnimateButton.h"
#include "SettlersView.h"
#include <Mmsystem.h>
#include "defineIni.h"

/////////////////////////////////////////////////////////////////////////////
// timer call back function
/////////////////////////////////////////////////////////////////////////////
void CALLBACK timerDraw(UINT /*uID*/, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{
	if(NULL != dwUser)
	{
		((CAnimateButton *) dwUser)->OnTimer();
	}
}

/////////////////////////////////////////////////////////////////////////////
// timer call back function
/////////////////////////////////////////////////////////////////////////////
void CALLBACK timerOnce(UINT /*uID*/, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{
	if(NULL != dwUser)
	{
		((CAnimateButton *) dwUser)->OnOneshot();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAnimateButton
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CAnimateButton, CWnd)

/////////////////////////////////////////////////////////////////////////////
// constructor
/////////////////////////////////////////////////////////////////////////////
CAnimateButton::CAnimateButton()
{
	//reset offset
	m_nOffset = 0;
	m_nOffsetDraw = 0;
	m_nOffsetAmount = 0;

	//not tracking yet
	m_bTracking = FALSE;

	//mousable
	m_bMouseable = TRUE;

	//not animating
	m_bIsAnimating = FALSE;
	m_bShowBuildOptions = FALSE;

	//set up timer
	timeBeginPeriod(TARGET_RESOLUTION);

	//set default speeds
	m_nSpeed1 = 1;
	m_nSpeed2 = 4;
	m_nSpeed3 = 16;
	m_nSpeed4 = 24;
}

/////////////////////////////////////////////////////////////////////////////
// destructor
/////////////////////////////////////////////////////////////////////////////
CAnimateButton::~CAnimateButton()
{
	//prevent crashes on exit
	stopAnimation();

	//shut down timer
	timeEndPeriod(TARGET_RESOLUTION);
}


BEGIN_MESSAGE_MAP(CAnimateButton, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CAnimateButton message handlers

/////////////////////////////////////////////////////////////////////////////
// set the mouse events
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::setMouseEvents()
{
	TRACKMOUSEEVENT tme;

	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = 1;
	m_bTracking = _TrackMouseEvent(&tme);
}

/////////////////////////////////////////////////////////////////////////////
// handle autohide
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::updateAutohide()
{
	//read all the auto-hide values
	m_bAutohide = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE, FALSE, INI_FILE);

	//handle the drawing
	handleAutohide();
}

/////////////////////////////////////////////////////////////////////////////
// override
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::showItems(BOOL /*bShow*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// override
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::moveOffset(int /*nOffset*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// override
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::handleAutohide()
{
}

/////////////////////////////////////////////////////////////////////////////
// override
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::updateAnimate()
{

}

/////////////////////////////////////////////////////////////////////////////
// override
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::startShow(BOOL bShow)
{
	CWnd::ShowWindow(bShow ? SW_SHOW : SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////
// handle drawing
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::drawOffset(int nOffset)
{
	if(nOffset != m_nOffset)
	{
		//reset offsets
		m_nOffset = nOffset;

		//if we're not animating, just move it
		if(FALSE == m_bAnimate)
		{
			//set draw offset
			m_nOffsetDraw = nOffset;

			//move the window
			moveOffset(nOffset);

			//show the goods if necessary
			showItems((nOffset == 0));
		}
		//otherwise, kick off the animation
		else
		{
			startAnimation();
		}
	}

	//set the focus back
	if(VIEW) VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle one shots
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::startOneshot(void)
{
	//start the one shot timer, 10 milliseconds
	m_uiTimerOneshot = timeSetEvent(10, 5, timerOnce, (DWORD) this, TIME_ONESHOT);
}

/////////////////////////////////////////////////////////////////////////////
// handle animation
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::startAnimation()
{
	//stop any current timer
	timeKillEvent(m_uiTimer);

	//hide items
	showItems(FALSE);
	RedrawWindow();

	//start the call timer, 20 fps
	m_uiTimer = timeSetEvent(50, 5, timerDraw, (DWORD) this, TIME_PERIODIC);

	//we are animating
	m_bIsAnimating = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// stop animation
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::stopAnimation()
{
	//not animating
	m_bIsAnimating = FALSE;

	//shut down the event
	timeKillEvent(m_uiTimer);

	//show items
	showItems((m_nOffset == 0));

	//show build options if needed
	if(TRUE == m_bShowBuildOptions)
	{
		if(VIEW) VIEW->showBuildOptions(TRUE);
		m_bShowBuildOptions = FALSE;
	}

	//ensure the view has the focus
	if(VIEW) VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle timer events
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::OnTimer()
{
	int nDif;
	BOOL bIn;

	//find current difference 
	nDif = abs(m_nOffsetDraw - m_nOffset);

	//see if we're done
	if(0 == nDif)
	{
		stopAnimation();
		return;
	}

	//determine if we're sliding in or out
	bIn = (m_nOffsetDraw < m_nOffset);

	//if we're within 5 pixels, slow to 1 pixel per frame
	if(4 >= nDif) m_nDX = m_nSpeed1;
	else if(17 >= nDif)	m_nDX = m_nSpeed2;
	else if(65 >= nDif)	m_nDX = m_nSpeed3;
	else m_nDX = m_nSpeed4;

	//set the true DX
	if(FALSE == bIn)
	{
		m_nDX = -m_nDX;
	}
	
	//set the draw offset
	m_nOffsetDraw += m_nDX;

	//move the window
	moveOffset(m_nOffsetDraw);
}

/////////////////////////////////////////////////////////////////////////////
// handle one shots
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::OnOneshot()
{
	//shut down the event
	timeKillEvent(m_uiTimerOneshot);

	setMouseEvents();
}

/////////////////////////////////////////////////////////////////////////////
// prevent flicker
/////////////////////////////////////////////////////////////////////////////
BOOL CAnimateButton::OnEraseBkgnd(CDC* /*pDC*/)
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// handle mouse tracking
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(FALSE == m_bTracking && TRUE == m_bAutohide && TRUE == m_bMouseable)
	{
		setMouseEvents();
		drawOffset();
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// handle leaving
/////////////////////////////////////////////////////////////////////////////
LRESULT CAnimateButton::OnMouseLeave(WPARAM /*wparam*/, LPARAM /*lparam*/)
{
	CPoint point;
	CRect rect;

	//check to see if we need to leave
	GetWindowRect(&rect);

	//get current point
	GetCursorPos(&point);

	//no longer tracking
	m_bTracking = FALSE;

	//see if we're still in
	if(TRUE == rect.PtInRect(point))
	{
		//do the oneshot
		startOneshot();
	}
	else
	{
		//handle the drawing
		handleAutohide();
	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// handle animation
/////////////////////////////////////////////////////////////////////////////
BOOL CAnimateButton::ShowWindow(int nCmdShow)
{
	//set the focus back
	if(VIEW) VIEW->SetFocus();

	//handle default if not
	if(FALSE == m_bAnimate)
	{
		return CWnd::ShowWindow(nCmdShow);
	}
	else
	{
		//ensure visibility
		CWnd::ShowWindow(SW_SHOW);

		//start it up
		startShow((nCmdShow == SW_SHOW));
		return TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle character messages
/////////////////////////////////////////////////////////////////////////////
void CAnimateButton::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//send all char messages to the main window
	if(VIEW) VIEW->PostMessage(WM_CHAR, (UINT) nChar, MAKELPARAM(nRepCnt, nFlags));
}
