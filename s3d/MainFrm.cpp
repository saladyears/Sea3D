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

#include "MainFrm.h"
#include "SettlersView.h"
#include "defineDraw.h"
#include "defineFile.h"
#include "defineIni.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CDC *pDC;
	ILuint ilToolbar[TOOLBAR_SIZE];

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
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
	ilBindImage(ilToolbar[TOOLBAR]);
	
	//load the bitmaps
	m_imgHot.Create(22, 22, ILC_COLOR24 | ILC_MASK, 8, 0);
	m_imgHot.Add(CBitmap::FromHandle(ilutConvertToHBitmap(pDC->GetSafeHdc())), COLOR_TRANSP);

	//set the bitmap list
	m_wndToolbar.GetToolBarCtrl().SetHotImageList(&m_imgHot);
	
	//bind the image
	ilBindImage(ilToolbar[TOOLBAR_GRAY]);

	//load the bitmaps
	m_imgCold.Create(22, 22, ILC_COLOR24 | ILC_MASK, 8, 0);
	m_imgCold.Add(CBitmap::FromHandle(ilutConvertToHBitmap(pDC->GetSafeHdc())), COLOR_TRANSP);

	//clean up il
	ilDeleteImages(TOOLBAR_SIZE, ilToolbar);
	
	//set the bitmap list
	m_wndToolbar.GetToolBarCtrl().SetImageList(&m_imgCold);
	m_wndToolbar.ModifyStyle(0, TBSTYLE_LIST | TBSTYLE_FLAT);
	m_wndToolbar.SetButtons(NULL, 8);

	// set up each toolbar button
	m_wndToolbar.SetButtonInfo(0, ID_FILE_NEW, TBSTYLE_BUTTON, 0);
	m_wndToolbar.SetButtonText(0, "New");
	m_wndToolbar.SetButtonInfo(1, ID_FILE_OPEN, TBSTYLE_BUTTON, 1);
	m_wndToolbar.SetButtonText(1, "Open");
	m_wndToolbar.SetButtonInfo(2, ID_DATABASE_PLAYER, TBSTYLE_BUTTON, 2);
	m_wndToolbar.SetButtonText(2, "Players");
	m_wndToolbar.SetButtonInfo(3, ID_DATABASE_GAME, TBSTYLE_BUTTON, 3);
	m_wndToolbar.SetButtonText(3, "Games");
	m_wndToolbar.SetButtonInfo(4, ID_DATABASE_MAPS, TBSTYLE_BUTTON, 4);
	m_wndToolbar.SetButtonText(4, "Maps");
	m_wndToolbar.SetButtonInfo(5, ID_OPTIONS_GENERAL, TBSTYLE_BUTTON, 5);
	m_wndToolbar.SetButtonText(5, "Settings");
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
	LoadBarState(TOOLBAR_MAIN);

	//release context
	ReleaseDC(pDC);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

/////////////////////////////////////////////////////////////////////////////
// handle closing
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	//see if we close
	if((nID == SC_CLOSE) && VIEW)
	{
		if(IDNO == MessageBox("Do you really want to quit?", "Quit game?", MB_YESNO)) 
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

	//see if we're maximizing
	if(SC_MAXIMIZE == nID)
	{
		//write out the value
		WritePrivateProfileString(INI_DISPLAY, INI_MAXIMIZED, "1", INI_FILE);
	}
	else if(SC_RESTORE == nID)
	{
		//write out the value
		WritePrivateProfileString(INI_DISPLAY, INI_MAXIMIZED, "0", INI_FILE);
	}

	CWnd::OnSysCommand(nID, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// don't allow resizes to miniscule dimensions
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	int nX, nY, nHeight;
	int nBordersWidth = 16, nBordersHeight = 56;

	CWnd::OnGetMinMaxInfo(lpMMI);

	//get the panel size
	if(VIEW && CONTROLPANEL)
	{
		//calculate maximum x width
		nX = VIEW->playerWidth() + 4 + CONTROLS_WIDTH + PANEL_WIDTH + nBordersWidth;

		//calculate maximum y width via players
		nHeight = (GAME->m_players.size() * (VIEW->playerHeight() + 6) + VIEW->bankHeight());

		//compare against other heights
		nY = __max(nHeight, (CONTROLS_HEIGHT + 4 + CONTROL_PANEL_GOODS_BUTTON + 4 + ADMIN_HEIGHT));
		nY += nBordersHeight;

		//set the min track size based on the size of the panel
		lpMMI->ptMinTrackSize = CPoint(nX, nY);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle clicking in here
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CMDIFrameWnd::OnLButtonDown(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// save bar state
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::DestroyWindow() 
{
	SaveBarState(TOOLBAR_MAIN);
	
	return CMDIFrameWnd::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// save/load bar state when opening/closing a child frame
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::saveToolbarState(BOOL bLoad)
{
	if(FALSE == bLoad)
	{
		SaveBarState(TOOLBAR_MAIN);

		//hide the toolbar
		m_wndToolbar.ShowWindow(SW_HIDE);
	}
	else
	{
		LoadBarState(TOOLBAR_MAIN);
	}
}
