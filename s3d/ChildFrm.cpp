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

#include "ChildFrm.h"
#include "SettlersView.h"
#include "defineFile.h"
#include "defineColors.h"
#include ".\childfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SYSCOMMAND()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_CHAR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATSTRUCT cs

	cs.style &= ~WS_MAXIMIZEBOX;
	cs.style &= ~WS_MINIMIZEBOX;
	cs.style &= ~WS_SYSMENU;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

/////////////////////////////////////////////////////////////////////////////
// handle closing
/////////////////////////////////////////////////////////////////////////////
void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	SetFocus();

	//see if we close
	if(nID == SC_CLOSE)
	{
		if(IDNO == ::MessageBox(NULL, "Do you really want to quit?", "Quit game?", MB_YESNO | MB_SETFOREGROUND))
		{
			return;
		}

		//disconnect if needed
		if(NULL != VIEW)
		{
			if(TRUE == VIEW->IsConnected())
			{
				VIEW->CloseConnection();
			}

			//see if the connector needs to be updated
			if((TRUE == GAME->m_bServer) && (TRUE == CONNECTOR->getConnector()))
			{
				CONNECTOR->postServer(CONNECTOR_DELETE);
			}
		}
	}

	CWnd::OnSysCommand(nID, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// handle toolbar creation
/////////////////////////////////////////////////////////////////////////////
int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CDC *pDC;
	ILuint ilToolbar[TOOLBAR_SIZE];

	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
		if (!m_wndToolbar.CreateEx(this))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//get device context
	pDC = GetDC();

	//set the render mode
	ilutRenderer(ILUT_WIN32);

	//read in the toolbar stuff
	APP->readIL(TOOLBAR_SIZE, FILE_APP, &ilToolbar[0]);

	// set up toolbar properties
	m_wndToolbar.GetToolBarCtrl().SetButtonWidth(50, 150);
	m_wndToolbar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	//bind the image
	ilBindImage(ilToolbar[TOOLBARCHILD]);
	
	//load the bitmaps
	m_imgHot.Create(22, 22, ILC_COLOR24 | ILC_MASK, 8, 0);
	m_imgHot.Add(CBitmap::FromHandle(ilutConvertToHBitmap(pDC->GetSafeHdc())), COLOR_TRANSP);

	//set the bitmap list
	m_wndToolbar.GetToolBarCtrl().SetHotImageList(&m_imgHot);

	//bind the image
	ilBindImage(ilToolbar[TOOLBARCHILD_GRAY]);

	//load the bitmaps
	m_imgCold.Create(22, 22, ILC_COLOR24 | ILC_MASK, 8, 0);
	m_imgCold.Add(CBitmap::FromHandle(ilutConvertToHBitmap(pDC->GetSafeHdc())), COLOR_TRANSP);
	
	//clean up il
	ilDeleteImages(TOOLBAR_SIZE, ilToolbar);

	//set the bitmap list
	m_wndToolbar.GetToolBarCtrl().SetImageList(&m_imgCold);
	m_wndToolbar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_LIST);
	m_wndToolbar.SetButtons(NULL, 8);

	// set up each toolbar button
	m_wndToolbar.SetButtonInfo(0, ID_FILE_SAVE, TBSTYLE_BUTTON, 0);
	m_wndToolbar.SetButtonText(0, "Save");
	m_wndToolbar.SetButtonInfo(1, ID_OPTIONS_GENERAL, TBSTYLE_BUTTON, 1);
	m_wndToolbar.SetButtonText(1, "Settings");
	m_wndToolbar.SetButtonInfo(2, ID_GL_SETTINGS, TBSTYLE_BUTTON, 2);
	m_wndToolbar.SetButtonText(2, "OpenGL");
	m_wndToolbar.SetButtonInfo(3, ID_OPTIONS_GAME_HISTORY, TBSTYLE_BUTTON, 3);
	m_wndToolbar.SetButtonText(3, "History");
	m_wndToolbar.SetButtonInfo(4, ID_OPTIONS_STATS, TBSTYLE_BUTTON, 4);
	m_wndToolbar.SetButtonText(4, "Stats");
	m_wndToolbar.SetButtonInfo(5, ID_OPTIONS_LEGEND, TBSTYLE_BUTTON, 5);
	m_wndToolbar.SetButtonText(5, "Legend");
	m_wndToolbar.SetButtonInfo(6, ID_HELP, TBSTYLE_BUTTON, 6);
	m_wndToolbar.SetButtonText(6, "Help");
	m_wndToolbar.SetButtonInfo(7, ID_APP_ABOUT, TBSTYLE_BUTTON, 7);
	m_wndToolbar.SetButtonText(7, "About");

	CRect rectToolBar;

	// set up toolbar button sizes
	m_wndToolbar.GetItemRect(0, &rectToolBar);
	m_wndToolbar.SetSizes(rectToolBar.Size(), CSize(23,22));
	m_wndToolbar.SetWindowText("Toolbar");

	//set bar style
	m_wndToolbar.SetBarStyle(m_wndToolbar.GetBarStyle() |
		CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY  | CBRS_SIZE_DYNAMIC);

	//allow docking
	m_wndToolbar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolbar);

	//load bar state
	LoadBarState(TOOLBAR_CHILD);

	//cleanup
	ReleaseDC(pDC);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// handle toolbar saving
/////////////////////////////////////////////////////////////////////////////
BOOL CChildFrame::DestroyWindow() 
{
	SaveBarState(TOOLBAR_CHILD);
	
	return CMDIChildWnd::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// keyboard handling
/////////////////////////////////////////////////////////////////////////////
void CChildFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	VIEW->PostMessage(WM_CHAR, (WPARAM) nChar, MAKELPARAM(nRepCnt, nFlags));
}
