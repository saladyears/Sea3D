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

#include "SettlersDoc.h"
#include "SettlersView.h"
#include "GLSettingsDialog.h"
#include "MultiUseDialog.h"
#include "NetworkMessageDialog.h"
#include "GameDatabase.h"
#include <bass.h>
#include "SettingsGeneralDialog.h"
#include "SettingsPlayerDisplayDialog.h"
#include "defineDraw.h"
#include "MainFrm.h"
#include "defineIni.h"
#include "defineFile.h"
#include "defineMap.h"
#include "defineColors.h"
#include ".\settlersview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// call back functions
/////////////////////////////////////////////////////////////////////////////
void callbackBalloon(DWORD dwCookie);

/////////////////////////////////////////////////////////////////////////////
// CSettlersView

IMPLEMENT_DYNCREATE(CSettlersView, CView)

BEGIN_MESSAGE_MAP(CSettlersView, CNetworkView)
	//{{AFX_MSG_MAP(CSettlersView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_GL_SETTINGS, OnGlSettings)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_OPTIONS_GAME_HISTORY, OnOptionsGameHistory)
	ON_COMMAND(ID_OPTIONS_STATS, OnOptionsStats)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_OPTIONS_DISPLAY, OnOptionsDisplay)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SYS_MESSAGES, OnUpdateOptionsSysMessages)
	ON_COMMAND(ID_OPTIONS_SYS_MESSAGES, OnOptionsSysMessages)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_OPTIONS_GENERAL, OnOptionsGeneral)
	ON_COMMAND(ID_TURN_CONTINUE, OnTurnContinue)
	ON_COMMAND(ID_OPTIONS_LEGEND, OnOptionsLegend)
	ON_COMMAND(ID_TOUR_QUIT, OnTourQuit)
	ON_COMMAND(ID_EDIT_SCREENSHOT, OnEditScreenshot)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_OPTIONS_MAP_INFO, OnOptionsMapInfo)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, ID_PLAYER1, ID_PLAYER1 + 7, HandlePlayerClick)
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettlersView construction/destruction

CSettlersView::CSettlersView()
{
	//no players yet
	m_iPlayers = 0;

	//no incomplete game index yet
	m_iIndex = -1;

	//no balloons yet
	m_iBalloon = 0;

	//controls button
	m_pControlsButton = NULL;

	//actions button
	m_pActionsButton = NULL;

	//messages button
	m_pMessagesButton = NULL;

	//admin button
	m_pAdminButton = NULL;

	//dialog pointers
	m_pGLSettingsDlg = NULL;
	m_pUndoDlg = NULL;
	m_pStatsDlg = NULL;
	m_pLegendDlg = NULL;
	m_pMapInfoDlg = NULL;

	//not initialized
	m_bInit = FALSE;

	//tiny windows
	m_tinyR = NULL;
	m_tinySh = NULL;
	m_tinyS = NULL;
	m_tinyC = NULL;
	m_tinyDev = NULL;
	m_tinyMove = NULL;
}

CSettlersView::~CSettlersView()
{
	clearIL();
}

BOOL CSettlersView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CGLView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSettlersView drawing

void CSettlersView::OnDraw(CDC* pDC)
{
	CSettlersDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CGLView::OnDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CSettlersView printing

BOOL CSettlersView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSettlersView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSettlersView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSettlersView diagnostics

#ifdef _DEBUG
void CSettlersView::AssertValid() const
{
	CGLView::AssertValid();
}

void CSettlersView::Dump(CDumpContext& dc) const
{
	CGLView::Dump(dc);
}

CSettlersDoc* CSettlersView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSettlersDoc)));
	return (CSettlersDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSettlersView message handlers

/////////////////////////////////////////////////////////////////////////////
// handle startup
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnInitialUpdate() 
{
	int nWidth;
	int nHeight;
	CRect rect, null(0, 0, 0, 0);

	CGLView::OnInitialUpdate();
	
	//tell the app we exist
	APP->setView(this);

	//get coordinates
	GetClientRect(&rect);

	//build the IL arrays
	buildIL();

	//create the tiny windows
	m_tinyR = new CTinyWnd();
	m_tinyR->setType(GL_ROAD);
	m_tinyR->Create(NULL, "", CHILD, null, this, 0, NULL);
	m_tinyR->SetWindowPos(&wndTop, 0, 0, GL_TINY_SIZE, GL_TINY_SIZE, SWP_SHOWWINDOW);
	m_tinyR->ShowWindow(SW_HIDE);
	m_tinyR->init();

	m_tinySh = new CTinyWnd();
	m_tinySh->setType(GL_SHIP);
	m_tinySh->Create(NULL, "", CHILD, null, this, 0, NULL);
	m_tinySh->SetWindowPos(&wndTop, 0, 0, GL_TINY_SIZE, GL_TINY_SIZE, SWP_SHOWWINDOW);
	m_tinySh->ShowWindow(SW_HIDE);
	m_tinySh->init();

	m_tinyS = new CTinyWnd();
	m_tinyS->setType(GL_SETTLEMENT);
	m_tinyS->Create(NULL, "", CHILD, null, this, 0, NULL);
	m_tinyS->SetWindowPos(&wndTop, 0, 0, GL_TINY_SIZE, GL_TINY_SIZE, SWP_SHOWWINDOW);
	m_tinyS->ShowWindow(SW_HIDE);
	m_tinyS->init();

	m_tinyC = new CTinyWnd();
	m_tinyC->setType(GL_CITY);
	m_tinyC->Create(NULL, "", CHILD, null, this, 0, NULL);
	m_tinyC->SetWindowPos(&wndTop, 0, 0, GL_TINY_SIZE, GL_TINY_SIZE, SWP_SHOWWINDOW);
	m_tinyC->ShowWindow(SW_HIDE);
	m_tinyC->init();

	m_tinyDev = new CTinyWnd();
	m_tinyDev->setType(GL_DEVCARD);
	m_tinyDev->Create(NULL, "Wnd", CHILD, null, this, 0, NULL);
	m_tinyDev->SetWindowPos(&wndTop, 0, 0, GL_TINY_SIZE, GL_TINY_SIZE, SWP_SHOWWINDOW);
	m_tinyDev->ShowWindow(SW_HIDE);
	m_tinyDev->init();

	//ship move window
	m_tinyMove = new CTinyWnd();
	m_tinyMove->setType(GL_SHIP_MOVE);
	m_tinyMove->Create(NULL, "Wnd", CHILD, null, this, 0, NULL);
	m_tinyMove->SetWindowPos(&wndTop, 0, 0, GL_TINY_SIZE, GL_TINY_SIZE, SWP_SHOWWINDOW);
	m_tinyMove->ShowWindow(SW_HIDE);
	m_tinyMove->init();

	//controls button
	m_pControlsButton = new CControlsButton;
	m_pControlsButton->Create(NULL, "", CHILD, null, this, 0);
	m_pControlsButton->SetWindowPos(&wndTop, 0, 0, CONTROLS_WIDTH, CONTROLS_HEIGHT, SWP_SHOWWINDOW);
	
	//actions button
	m_pActionsButton = new CActionsButton;
	m_pActionsButton->Create(NULL, "", CHILD, null, this, 0);
	m_pActionsButton->SetWindowPos(&wndTop, 0, 0, PANEL_WIDTH, CONTROL_PANEL_GOODS_BUTTON + 4, SWP_SHOWWINDOW);

	//messages button
	m_pMessagesButton = new CMessageButton;
	m_pMessagesButton->Create(NULL, "", CHILD, null, this, 0);
	m_pMessagesButton->SetWindowPos(&wndTop, 0, 0, PANEL_WIDTH, CONTROLS_HEIGHT, SWP_SHOWWINDOW);

	//read admin width & height from .ini file
	nWidth = GetPrivateProfileInt(INI_DISPLAY, INI_EMBED_WIDTH, PANEL_WIDTH, INI_FILE);
	nHeight = GetPrivateProfileInt(INI_DISPLAY, INI_EMBED_HEIGHT, ADMIN_HEIGHT, INI_FILE);

	//sanity
	if(nWidth < PANEL_WIDTH)
	{
		nWidth = PANEL_WIDTH;
	}
	if(nHeight < ADMIN_HEIGHT)
	{
		nHeight = ADMIN_HEIGHT;
	}

	//admin button
	m_pAdminButton = new CAdminButton;
	m_pAdminButton->Create(NULL, "", CHILD, null, this, 0);
	m_pAdminButton->SetWindowPos(&wndTop, 0, 0, nWidth, nHeight, SWP_SHOWWINDOW);

	//create the dialogs
	m_pGLSettingsDlg = new CGLSettingsDialog(this);
	m_pGLSettingsDlg->Create(IDD_GL_SETTINGS_DLG);
	m_pGLSettingsDlg->ShowWindow(SW_HIDE);

	m_pUndoDlg = new CUndoDialog(this);
	m_pUndoDlg->Create(IDD_UNDO_DLG, this);
	m_pUndoDlg->ShowWindow(SW_HIDE);

	m_pStatsDlg = new CStatsDialog(this);
	m_pStatsDlg->Create(IDD_STATS_DLG, this);
	m_pStatsDlg->ShowWindow(SW_HIDE);

	m_pLegendDlg = new CLegendDialog(this);
	m_pLegendDlg->Create(IDD_LEGEND_DIALOG, this);
	m_pLegendDlg->ShowWindow(SW_HIDE);

	m_pMapInfoDlg = new CMapInfoDialog(this);
	m_pMapInfoDlg->Create(IDD_MAP_INFO_DIALOG, this);
	m_pMapInfoDlg->ShowWindow(SW_HIDE);

	//tour handling
	if(TRUE == APP->inTour())
	{
		//set the callback
		m_balloon.setCallBack(callbackBalloon, (DWORD) this);
	}

	//initialized
	m_bInit = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// restart the undo dialog
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::resetUndo()
{
	if(NULL != m_pUndoDlg)
	{
		delete m_pUndoDlg;
		m_pUndoDlg = NULL;
	}

	//create it
	if(NULL == m_pUndoDlg)
	{
		m_pUndoDlg = new CUndoDialog(this);
		m_pUndoDlg->Create(IDD_UNDO_DLG, this);
		m_pUndoDlg->ShowWindow(SW_HIDE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle shutdown
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnDestroy() 
{
	//reset toolbar state
	((CMainFrame *) AfxGetApp()->GetMainWnd())->saveToolbarState(TRUE);

	//close down the current balloon
	if(NULL != m_balloon.m_hWnd)
	{
		m_balloon.SendMessage(WM_CLOSE);
	}

	CNetworkView::OnDestroy();

	//tell the app we no longer exist
	APP->setView(NULL);

	if(m_pControlsButton)
	{
		delete m_pControlsButton;
		m_pControlsButton = NULL;
	}
	if(m_pActionsButton)
	{
		delete m_pActionsButton;
		m_pActionsButton = NULL;
	}
	if(m_pMessagesButton)
	{
		delete m_pMessagesButton;
		m_pMessagesButton = NULL;
	}
	if(m_pAdminButton)
	{
		delete m_pAdminButton;
		m_pAdminButton = NULL;
	}
	//tiny windows
	if(m_tinyR)
	{
		delete m_tinyR;
		m_tinyR = NULL;
	}
	if(m_tinySh)
	{
		delete m_tinySh;
		m_tinySh = NULL;
	}
	if(m_tinyS)
	{
		delete m_tinyS;
		m_tinyS = NULL;
	}
	if(m_tinyC)
	{
		delete m_tinyC;
		m_tinyC = NULL;
	}
	if(m_tinyDev)
	{
		delete m_tinyDev;
		m_tinyDev = NULL;
	}
	if(m_tinyMove)
	{
		delete m_tinyMove;
		m_tinyMove = NULL;
	}

	//delete dialogs
	if(m_pGLSettingsDlg)
	{
		delete m_pGLSettingsDlg;
		m_pGLSettingsDlg = NULL;
	}
	if(m_pUndoDlg)
	{
		delete m_pUndoDlg;
		m_pUndoDlg = NULL;
	}
	if(m_pStatsDlg)
	{
		delete m_pStatsDlg;
		m_pStatsDlg = NULL;
	}
	if(m_pLegendDlg)
	{
		delete m_pLegendDlg;
		m_pLegendDlg = NULL;
	}
	if(m_pMapInfoDlg)
	{
		delete m_pMapInfoDlg;
		m_pMapInfoDlg = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle resizes
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnSize(UINT nType, int cx, int cy) 
{
	int nState;
	CRect rect;
	CGLView::OnSize(nType, cx, cy);
	
	//tiny windows
	if(m_tinySh && m_tinySh->m_hWnd)
	{
		m_tinySh->SetWindowPos(&wndTop, cx - (5 * GL_TINY_SIZE) - 16, CONTROLS_HEIGHT + 4, GL_TINY_SIZE, GL_TINY_SIZE, SWP_NOZORDER);
		m_tinySh->Invalidate();
	}

	if(m_tinyR && m_tinyR->m_hWnd)
	{
		m_tinyR->SetWindowPos(&wndTop, cx - (4 * GL_TINY_SIZE) - 12, CONTROLS_HEIGHT + 4, GL_TINY_SIZE, GL_TINY_SIZE, SWP_NOZORDER);
		m_tinyR->Invalidate();
	}

	if(m_tinyS && m_tinyS->m_hWnd)
	{
		m_tinyS->SetWindowPos(&wndTop, cx - (3 * GL_TINY_SIZE) - 8, CONTROLS_HEIGHT + 4, GL_TINY_SIZE, GL_TINY_SIZE, SWP_NOZORDER);
		m_tinyS->Invalidate();
	}

	if(m_tinyC && m_tinyC->m_hWnd)
	{
		m_tinyC->SetWindowPos(&wndTop, cx - (2 * GL_TINY_SIZE) - 4, CONTROLS_HEIGHT + 4, GL_TINY_SIZE, GL_TINY_SIZE, SWP_NOZORDER);
		m_tinyC->Invalidate();
	}
	if(m_tinyDev && m_tinyDev->m_hWnd)
	{
		m_tinyDev->SetWindowPos(&wndTop, cx - GL_TINY_SIZE, CONTROLS_HEIGHT + 4, GL_TINY_SIZE, GL_TINY_SIZE, SWP_NOZORDER);
		m_tinyDev->Invalidate();
	}
	if(m_tinyMove && m_tinyMove->m_hWnd)
	{
		m_tinyMove->SetWindowPos(&wndTop, cx - GL_TINY_SIZE, CONTROLS_HEIGHT + 8 + GL_TINY_SIZE, GL_TINY_SIZE, GL_TINY_SIZE, SWP_NOZORDER);
		m_tinyMove->Invalidate();
	}

	if(m_pControlsButton && m_pControlsButton->m_hWnd)
	{
		//set state
		nState = m_pControlsButton->getOffset() ? SWP_HIDEWINDOW : SWP_SHOWWINDOW;
		m_pControlsButton->SetWindowPos(&wndTop, cx - PANEL_WIDTH - CONTROLS_WIDTH - 4 - 1, 0, 0, 0, SWP_NOZORDER| SWP_NOSIZE | nState);
	}

	if(m_pActionsButton && m_pActionsButton->m_hWnd)
	{
		//set state
		nState = m_pActionsButton->getOffset() ? SWP_HIDEWINDOW : SWP_SHOWWINDOW;
		m_pActionsButton->SetWindowPos(&wndTop, cx - PANEL_WIDTH - 1, CONTROLS_HEIGHT + 4, 0, 0, SWP_NOZORDER | SWP_NOSIZE | nState);
	}

	if(m_pMessagesButton && m_pMessagesButton->m_hWnd)
	{
		//set state
		nState = m_pMessagesButton->getOffset() ? SWP_HIDEWINDOW : SWP_SHOWWINDOW;
		m_pMessagesButton->SetWindowPos(&wndTop, cx - PANEL_WIDTH - 1, 0, 0, 0, SWP_NOZORDER| SWP_NOSIZE | nState);
	}

	if(m_pAdminButton && m_pAdminButton->m_hWnd)
	{
		//get dimensions
		m_pAdminButton->GetClientRect(&rect);

		//set state
		nState = m_pAdminButton->getOffset() ? SWP_HIDEWINDOW : SWP_SHOWWINDOW;

		// Now make sure the chat window isn't too big to fit on the screen...
		CString str;
		if( cx - rect.Width() - 1 <= 0 )
		{
			rect.right = cx - 4;
			str.Format("%d", rect.Width());
			//set the new size
			WritePrivateProfileString(INI_DISPLAY, INI_EMBED_WIDTH, str, INI_FILE);
		}
		if( cy - rect.Height() - 1 <= 0 )
		{
			rect.bottom = cy - CONTROLS_HEIGHT - CONTROL_PANEL_GOODS_BUTTON - 12;
			str.Format("%d", rect.Height());
			//set the new size
			WritePrivateProfileString(INI_DISPLAY, INI_EMBED_HEIGHT, str, INI_FILE);
		}
		m_pAdminButton->SetWindowPos(&wndTop, cx - rect.Width() - 1, cy - rect.Height() - 1, 0, 0, SWP_NOZORDER| SWP_NOSIZE | nState);
	}
}

/////////////////////////////////////////////////////////////////////////////
// initialize the control panel
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::initPanel()
{
	CRect rect;

	//initialize and show
	m_control.initControl();

	//set up player controls
	initPlayers();

	//initialize chat and admin windows
	initChatAdmin();
	
	//get the window rectangle
	APP->m_pMainWnd->GetWindowRect(&rect);

	//tell the main window to move (but not really) so that min/max info gets 
	//initialized
	APP->m_pMainWnd->MoveWindow(rect, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// initialize the chat and admin buttons
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::initChatAdmin()
{
	BOOL bEmbed;

	//hide the admin button if needed
	if(GAME_MODE_HOTSEAT == m_pGame->m_iMode)
	{
		m_pAdminButton->setAnimate(FALSE);
		m_pAdminButton->ShowWindow(SW_HIDE);
	}
	else
	{
		//get the embedded value
		bEmbed = GetPrivateProfileInt(INI_GENERAL, INI_EMBED_CHAT, 1, INI_FILE);

		//do stuff
		if(TRUE == bEmbed)
		{
			if(NULL != m_pChatWindow)
			{
				m_pChatWindow->ShowWindow(SW_HIDE);
			}
			m_pAdminButton->setControls();
			m_pAdminButton->ShowWindow(SW_SHOW);
		}
		else
		{
			if(NULL != m_pChatWindow)
			{
				m_pChatWindow->ShowWindow(SW_SHOW);
				m_pChatWindow->setControls();
			}
			m_pAdminButton->ShowWindow(SW_HIDE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle undo pressing
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnEditUndo() 
{
	undo();
}

/////////////////////////////////////////////////////////////////////////////
// update menu
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(MESSAGES && MESSAGES->undoEnabled());
}

/////////////////////////////////////////////////////////////////////////////
// update build options
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::showBuildOptions(BOOL bShow)
{
	BOOL bAuto;
	BOOL bSea;

	//get auto-trade value from .ini file (default to true)
	bAuto = GetPrivateProfileInt(INI_GENERAL, INI_AUTOTRADE, TRUE, INI_FILE);

	//if we're in the special build phase, auto-trading must be off
	if(TRUE == m_bSpecialBuild)
	{
		bAuto = FALSE;
	}

	//ensure that the actions window can't be stuck open when build options are
	//showing
	if((TRUE == bShow) && (0 == m_pActionsButton->getOffset()))
	{
		m_pActionsButton->ShowWindow(SW_HIDE);
	}

	//see if we're animating
	if((TRUE == ACTIONS->isAnimating()) && (TRUE == bShow))
	{
		bShow = FALSE;
		m_pActionsButton->setBuildOptions();
	}

	//see if we're seafarers
	bSea = MAP->m_iMode & MAP_SEAFARERS;

	showRoad(bShow, bAuto);
	showShip(bShow && bSea, bAuto);
	showSettlement(bShow, bAuto);
	showCity(bShow, bAuto);
	showDevCard(bShow, bAuto);

	if(TRUE == bSea)
	{
		showMove(bShow);
	}
}

/////////////////////////////////////////////////////////////////////////////
// show initital road/ship options
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::showInitialOptions(BOOL bShow)
{
	//show the roads
	if(m_tinyR && m_tinyR->m_hWnd)
	{
		m_tinyR->updateTiny(TRUE);
		m_tinyR->ShowWindow(SW_HIDE);
		m_tinyR->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}

	//show the ships
	if(m_tinySh && m_tinySh->m_hWnd)
	{
		m_tinySh->updateTiny(TRUE);
		m_tinySh->ShowWindow(SW_HIDE);
		m_tinySh->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// tiny window
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::showRoad(BOOL bShow, BOOL bAuto)
{
	int iTrades;

	//check to see if roads can be displayed
	if(TRUE == bShow)
	{
		if(!prepareRoads() || !m_pGame->m_players[m_iPlayer].m_nStockRoads || TRUE == m_pGame->m_players[m_iPlayer].m_player.getAI())
		{
			bShow = FALSE;
		}
		else
		{
			iTrades = tradeToBank(GL_ROAD, TRUE);

			//if auto trading is off, we must have a zero in iTrades
			if(0 > iTrades || (FALSE == bAuto && 0 != iTrades))
			{
				bShow = FALSE;
			}
		}
	}

	if(m_tinyR && m_tinyR->m_hWnd)
	{
		m_tinyR->updateTiny();
		m_tinyR->ShowWindow(SW_HIDE);
		m_tinyR->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// tiny window
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::showShip(BOOL bShow, BOOL bAuto)
{
	int iTrades;

	//check to see if roads can be displayed
	if(TRUE == bShow)
	{
		if(!prepareShips() || !m_pGame->m_players[m_iPlayer].m_nStockShips || TRUE == m_pGame->m_players[m_iPlayer].m_player.getAI())
		{
			bShow = FALSE;
		}
		else
		{
			iTrades = tradeToBank(GL_SHIP, TRUE);

			//if auto trading is off, we must have a zero in iTrades
			if(0 > iTrades || (FALSE == bAuto && 0 != iTrades))
			{
				bShow = FALSE;
			}
		}
	}

	if(m_tinySh && m_tinySh->m_hWnd)
	{
		m_tinySh->updateTiny();
		m_tinySh->ShowWindow(SW_HIDE);
		m_tinySh->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// tiny window
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::showSettlement(BOOL bShow, BOOL bAuto) 
{
	int iTrades;

	if(TRUE == bShow)
	{
		if(!prepareSettlements() || !m_pGame->m_players[m_iPlayer].m_nStockSettle || TRUE == m_pGame->m_players[m_iPlayer].m_player.getAI())
		{
			bShow = FALSE;
		}
		else
		{
			iTrades = tradeToBank(GL_SETTLEMENT, TRUE);

			//if auto trading is off, we must have a zero in iTrades
			if(0 > iTrades || (FALSE == bAuto && 0 != iTrades))
			{
				bShow = FALSE;
			}
		}
	}

	if(m_tinyS && m_tinyS->m_hWnd) 
	{
		m_tinyS->updateTiny();
		m_tinyS->ShowWindow(SW_HIDE);
		m_tinyS->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// tiny window
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::showCity(BOOL bShow, BOOL bAuto) 
{
	int iTrades;

	if(TRUE == bShow)
	{
		if(!prepareCities() || !m_pGame->m_players[m_iPlayer].m_nStockCities || TRUE == m_pGame->m_players[m_iPlayer].m_player.getAI())
		{
			bShow = FALSE;
		}
		else
		{
			iTrades = tradeToBank(GL_CITY, TRUE);

			//if auto trading is off, we must have a zero in iTrades
			if(0 > iTrades || (FALSE == bAuto && 0 != iTrades))
			{
				bShow = FALSE;
			}
		}
	}

	if(m_tinyC && m_tinyC->m_hWnd) 
	{
		m_tinyC->updateTiny();
		m_tinyC->ShowWindow(SW_HIDE);
		m_tinyC->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// tiny window
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::showDevCard(BOOL bShow, BOOL bAuto)
{
	int iTrades;

	if(TRUE == bShow)
	{
		if(FALSE == checkForDevCards() || TRUE == m_pGame->m_players[m_iPlayer].m_player.getAI())
		{
			bShow = FALSE;
		}
		else
		{
			iTrades = tradeToBank(GL_DEVCARD, TRUE);

			//if auto trading is off, we must have a zero in iTrades
			if(0 > iTrades || (FALSE == bAuto && 0 != iTrades))
			{
				bShow = FALSE;
			}
		}
	}

	if(m_tinyDev && m_tinyDev->m_hWnd) 
	{
		m_tinyDev->updateTiny();
		m_tinyDev->ShowWindow(SW_HIDE);
		m_tinyDev->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// show/hide the ship move button
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::showMove(BOOL bShow)
{
	if(TRUE == bShow)
	{
		if(FALSE == prepareShipMove() || TRUE == m_pGame->m_players[m_iPlayer].m_player.getAI())
		{
			bShow = FALSE;
		}
	}

	if(m_tinyMove && m_tinyMove->m_hWnd) 
	{
		m_tinyMove->updateTiny();
		m_tinyMove->ShowWindow(SW_HIDE);
		m_tinyMove->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// set gl settings for the window
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnGlSettings() 
{
	CMenu *pMenu;
	CMenu *pSubmenu;
	BOOL bVisible;
	
	//get the windows's current visibility
	bVisible = m_pGLSettingsDlg->IsWindowVisible();

	//get the current menu
	pMenu = AfxGetMainWnd()->GetMenu();

	//get the submenu
	pSubmenu = pMenu->GetSubMenu(2);

	//set the checked status
	pSubmenu->CheckMenuItem(1, bVisible ? (MF_UNCHECKED | MF_BYPOSITION) : (MF_CHECKED | MF_BYPOSITION));

	//show or hide the window
	m_pGLSettingsDlg->ShowWindow(bVisible ? SW_HIDE : SW_SHOW);
}

/////////////////////////////////////////////////////////////////////////////
// show/hide the game history window
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnOptionsGameHistory() 
{
	CMenu *pMenu;
	CMenu *pSubmenu;
	BOOL bVisible;
	
	//get the windows's current visibility
	bVisible = m_pUndoDlg->IsWindowVisible();

	//get the current menu
	pMenu = AfxGetMainWnd()->GetMenu();

	//get the submenu
	pSubmenu = pMenu->GetSubMenu(3);

	//set the checked status
	pSubmenu->CheckMenuItem(0, bVisible ? (MF_UNCHECKED | MF_BYPOSITION) : (MF_CHECKED | MF_BYPOSITION));

	//show or hide the window
	m_pUndoDlg->ShowWindow(bVisible ? SW_HIDE : SW_SHOW);
}

/////////////////////////////////////////////////////////////////////////////
// show/hide the stats window
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnOptionsStats() 
{
	CMenu *pMenu;
	CMenu *pSubmenu;
	BOOL bVisible;
	
	//get the windows's current visibility
	bVisible = m_pStatsDlg->IsWindowVisible();

	//get the current menu
	pMenu = AfxGetMainWnd()->GetMenu();

	//get the submenu
	pSubmenu = pMenu->GetSubMenu(3);

	//set the checked status
	pSubmenu->CheckMenuItem(1, bVisible ? (MF_UNCHECKED | MF_BYPOSITION) : (MF_CHECKED | MF_BYPOSITION));

	//show or hide the window
	m_pStatsDlg->ShowWindow(bVisible ? SW_HIDE : SW_SHOW);
}

/////////////////////////////////////////////////////////////////////////////
// show/hide the legend window
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnOptionsLegend() 
{
	CMenu *pMenu;
	CMenu *pSubmenu;
	BOOL bVisible;
	CString str;

	//get the windows's current visibility
	bVisible = m_pLegendDlg->IsWindowVisible();

	//get the current menu
	pMenu = AfxGetMainWnd()->GetMenu();

	//get the submenu
	pSubmenu = pMenu->GetSubMenu(3);

	//set the checked status
	pSubmenu->CheckMenuItem(2, bVisible ? (MF_UNCHECKED | MF_BYPOSITION) : (MF_CHECKED | MF_BYPOSITION));

	//show or hide the window
	m_pLegendDlg->ShowWindow(bVisible ? SW_HIDE : SW_SHOW);

	//set .ini file value
	str = bVisible ? "1" : "0";
	
	//write it out
	WritePrivateProfileString(INI_GENERAL, INI_LEGEND, str, INI_FILE);
}

/////////////////////////////////////////////////////////////////////////////
// show/hide the map info window
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnOptionsMapInfo() 
{
	CMenu *pMenu;
	CMenu *pSubmenu;
	BOOL bVisible;

	//get the windows's current visibility
	bVisible = m_pMapInfoDlg->IsWindowVisible();

	//get the current menu
	pMenu = AfxGetMainWnd()->GetMenu();

	//get the submenu
	pSubmenu = pMenu->GetSubMenu(3);

	//set the checked status
	pSubmenu->CheckMenuItem(3, bVisible ? (MF_UNCHECKED | MF_BYPOSITION) : (MF_CHECKED | MF_BYPOSITION));

	//show or hide the window
	m_pMapInfoDlg->ShowWindow(bVisible ? SW_HIDE : SW_SHOW);
}

/////////////////////////////////////////////////////////////////////////////
// build the IL data
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::buildIL()
{
	BOOL bPossible;

	//see if we can even do euro files
	bPossible = checkForEuro();

	//tiny pictures
	APP->readIL(TINY_SIZE, FILE_TINY, m_ilTiny);

	//res images
	APP->readIL(IMG_SIZE, getEFile(FILE_RES, bPossible), m_ilRes);

	//history images
	APP->readIL(HISTORY_SIZE, getEFile(FILE_HISTORY, bPossible), m_ilHistory);
	
	//control panel
	APP->readIL(BUTTON_SIZE, FILE_CONTROL, m_ilControl);
	
	//dice array
	APP->readIL(DICE_SIZE, FILE_DICE, m_ilDice);

	//deck array
	APP->readIL(DECK_SIZE << 1, getEFile(FILE_DECK, bPossible), m_ilDeck);

	//cards array
	APP->readIL(CARD_SIZE, getEFile(FILE_CARDS, bPossible), m_ilCards);
}

/////////////////////////////////////////////////////////////////////////////
// clear IL data
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::clearIL(void)
{
	//delete the IL images
	ilDeleteImages(TINY_SIZE, m_ilTiny);
	ilDeleteImages(IMG_SIZE, m_ilRes);
	ilDeleteImages(HISTORY_SIZE, m_ilHistory);
	ilDeleteImages(BUTTON_SIZE, m_ilControl);
	ilDeleteImages(DICE_SIZE, m_ilDice);
	ilDeleteImages(DECK_SIZE << 1, m_ilDeck);
	ilDeleteImages(CARD_SIZE, m_ilCards);
}

/////////////////////////////////////////////////////////////////////////////
// set the euro textures file
/////////////////////////////////////////////////////////////////////////////
CString CSettlersView::getEFile(CString str, BOOL bPossible)
{
	BOOL bE;
	CString strE;

	//sanity
	if(FALSE == bPossible)
	{
		return str;
	}

	//see if we need the 'e'
	bE = GetPrivateProfileInt(INI_OPENGL, INI_EURO, FALSE, INI_FILE);

	//add it
	if(TRUE == bE)
	{
		str.Insert(str.GetLength() - 4, 'e');
	}

	return str;
}

/////////////////////////////////////////////////////////////////////////////
// get a bitmap
/////////////////////////////////////////////////////////////////////////////
HBITMAP CSettlersView::getTinyImage(int iImage, HDC hDC)
{
	//set the render mode
	ilutRenderer(ILUT_WIN32);

	//bind the image
	ilBindImage(m_ilTiny[iImage]);

	return ilutConvertToHBitmap(hDC);
}

/////////////////////////////////////////////////////////////////////////////
// get a bitmap
/////////////////////////////////////////////////////////////////////////////
HBITMAP CSettlersView::getResImage(int iImage, HDC hDC)
{
	//set the render mode
	ilutRenderer(ILUT_WIN32);

	//bind the image
	ilBindImage(m_ilRes[iImage]);

	return ilutConvertToHBitmap(hDC);
}

/////////////////////////////////////////////////////////////////////////////
// get a bitmap
/////////////////////////////////////////////////////////////////////////////
HBITMAP CSettlersView::getHistoryImage(int iImage, HDC hDC)
{
	//set the render mode
	ilutRenderer(ILUT_WIN32);

	//bind the image
	ilBindImage(m_ilHistory[iImage]);

	return ilutConvertToHBitmap(hDC);
}

/////////////////////////////////////////////////////////////////////////////
// get a bitmap
/////////////////////////////////////////////////////////////////////////////
HBITMAP CSettlersView::getControlImage(int iImage, HDC hDC)
{
	//set the render mode
	ilutRenderer(ILUT_WIN32);

	//bind the image
	ilBindImage(m_ilControl[iImage]);

	return ilutConvertToHBitmap(hDC);
}

/////////////////////////////////////////////////////////////////////////////
// get a bitmap
/////////////////////////////////////////////////////////////////////////////
HBITMAP CSettlersView::getDiceImage(int iImage, HDC hDC)
{
	//set the render mode
	ilutRenderer(ILUT_WIN32);

	//bind the image
	ilBindImage(m_ilDice[iImage]);

	return ilutConvertToHBitmap(hDC);
}

/////////////////////////////////////////////////////////////////////////////
// get a bitmap
/////////////////////////////////////////////////////////////////////////////
HBITMAP CSettlersView::getDeckImage(int iImage, HDC hDC)
{
	//set the render mode
	ilutRenderer(ILUT_WIN32);

	//bind the image
	ilBindImage(m_ilDeck[iImage]);

	return ilutConvertToHBitmap(hDC);
}

/////////////////////////////////////////////////////////////////////////////
// get a bitmap
/////////////////////////////////////////////////////////////////////////////
HBITMAP CSettlersView::getCardsImage(int iImage, HDC hDC)
{
	//set the render mode
	ilutRenderer(ILUT_WIN32);

	//bind the image
	ilBindImage(m_ilCards[iImage]);

	return ilutConvertToHBitmap(hDC);
}

/////////////////////////////////////////////////////////////////////////////
// handle shut down
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnFileClose() 
{
	if(IDNO == ::MessageBox(NULL, "Do you really want to quit?", "Quit game?", MB_YESNO))
	{
		return;
	}

	//disconnect if needed
	if(TRUE == IsConnected())
	{
		CloseConnection();
	}

	//see if the connector needs to be updated
	if((TRUE == m_pGame->m_bServer) && (TRUE == CONNECTOR->getConnector()))
	{
		CONNECTOR->postServer(CONNECTOR_DELETE);
	}

	//close down
	APP->CloseAllDocuments(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// handle shut down
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnAppExit() 
{
	if(IDNO == ::MessageBox(NULL, "Do you really want to quit?", "Quit game?", MB_YESNO))
	{
		return;
	}

	//disconnect if needed
	if(TRUE == IsConnected())
	{
		CloseConnection();
	}

	//see if the connector needs to be updated
	if((TRUE == m_pGame->m_bServer) && (TRUE == CONNECTOR->getConnector()))
	{
		CONNECTOR->postServer(CONNECTOR_DELETE);
	}

	APP->m_pMainWnd->SendMessage(WM_CLOSE);
}

/////////////////////////////////////////////////////////////////////////////
// initialize player buttons
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::initPlayers(void)
{
	int i;
	int iWidth;
	BOOL bLarge;
	BOOL bShow;
	CRect rect;
	CString str;

	//get the number of players
	m_iPlayers = m_pGame->m_players.size();

	//clear all old windows
	for(i = 0; i < 6; i++)
	{
		if(NULL != m_play[i].m_hWnd)
		{
			m_play[i].DestroyWindow();
		}
	}

	//destroy the bank
	if(NULL != m_bank.m_hWnd)
	{
		m_bank.DestroyWindow();
	}

	//delete and recreate the legend window
	if(NULL != m_pLegendDlg)
	{
		//get show value
		bShow = m_pLegendDlg->IsWindowVisible();
		delete m_pLegendDlg;
		m_pLegendDlg = new CLegendDialog();
		m_pLegendDlg->Create(IDD_LEGEND_DIALOG, this);
		m_pLegendDlg->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}

	//get large fonts values
	bLarge = GetPrivateProfileInt(INI_GENERAL, INI_LARGE_FONTS, 0, INI_FILE);

	//calculate button width
	if(TRUE == bLarge)
	{
		if(MAP->m_iMode & MAP_SEAFARERS) iWidth = LARGE_PLAYER_BUTTON_WIDTH_SEA;
		else iWidth = LARGE_PLAYER_BUTTON_WIDTH;
	}
	else
	{
		if(MAP->m_iMode & MAP_SEAFARERS) iWidth = PLAYER_BUTTON_WIDTH_SEA;
		else iWidth = PLAYER_BUTTON_WIDTH;
	}

	//create starting rect
	if(TRUE == bLarge)
	{
		rect = CRect(0, 0, iWidth, LARGE_PLAYER_BUTTON_HEIGHT);
	}
	else
	{
		rect = CRect(0, 0, iWidth, PLAYER_BUTTON_HEIGHT);
	}

	//create all the buttons
	for(i = 0; i < m_iPlayers; i++)
	{
		m_play[i].Create(NULL, "", CHILD, rect, this, ID_PLAYER1 + i);
		m_play[i].ShowWindow(SW_HIDE);
		m_play[i].setIndex(i);
		m_play[i].updateCounts();
		m_play[i].ShowWindow(SW_SHOW);

		if(TRUE == bLarge)
		{
			rect.OffsetRect(0, LARGE_PLAYER_BUTTON_HEIGHT + 3);
		}
		else
		{
			rect.OffsetRect(0, PLAYER_BUTTON_HEIGHT + 6);
		}
	}

	//create the bank button
	if(TRUE == bLarge)
	{
		rect.right = rect.left + LARGE_BANK_BUTTON_WIDTH;
		rect.bottom = rect.top + LARGE_BANK_BUTTON_HEIGHT;
	}
	else
	{
		rect.right = rect.left + BANK_BUTTON_WIDTH;
		rect.bottom = rect.top + BANK_BUTTON_HEIGHT;
	}
	m_bank.Create(NULL, "", CHILD, rect, this, ID_PLAYER1 + i);

	//set index
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		m_pLegendDlg->setIndex(m_pGame->m_iIndex);
	}
	else
	{
		m_pLegendDlg->setIndex(-1);
	}

	//update display
	updateLegend();
}

/////////////////////////////////////////////////////////////////////////////
// refresh the player buttons
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::HandlePlayerClick(UINT nID)
{
	//refresh the player window when it's clicked on
	int iPlayer;

	//calculate the player
	iPlayer = nID - ID_PLAYER1;

	//redraw
	if(iPlayer < m_iPlayers)
	{
		m_play[iPlayer].Invalidate();
	}
	//otherwise update the bank
	else
	{
		m_bank.Invalidate();
	}
}

/////////////////////////////////////////////////////////////////////////////
// refresh the player buttons
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnOptionsDisplay() 
{
	CSettingsPlayerDisplayDialog dlg;

	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// update player counts
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::updateCounts(int nPlayer)
{
	//update player counts
	if(NULL != m_play[nPlayer].m_hWnd)
	{
		m_play[nPlayer].updateCounts(); 
		m_play[nPlayer].Invalidate(); 
	}

	//update the bank
	if(NULL != m_bank.m_hWnd)
	{
		m_bank.updateCounts();
	}
}

/////////////////////////////////////////////////////////////////////////////
// update player displayers
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::updatePlayerDisplay()
{
	int i;

	//update all player displays
	for(i = 0; i < m_iPlayers; i++)
	{
		m_play[i].updateDisplay();
		m_play[i].handleAutohide();
	}
}

/////////////////////////////////////////////////////////////////////////////
// init network stuff once everything's done
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::initNetwork()
{
	//init the stat dialog
	STAT->init();

	//set mode
	m_iMode = GL_NORMAL;

	//disable the undo
	m_pMessagesButton->enableUndo(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// can only display options in network mode
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnUpdateOptionsSysMessages(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GAME_MODE_NETWORK == m_pGame->m_iMode);
}

/////////////////////////////////////////////////////////////////////////////
// run the sys message dialog
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnOptionsSysMessages() 
{
	CNetworkMessageDialog dlg;

	//run it
	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// save this game to the incomplete database
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnFileSave() 
{
	//wait cursor
	CWaitCursor wait;

	//if our index is -1, we haven't saved yet, so just add it
	if(-1 == m_iIndex)
	{
		m_iIndex = DATABASE->addGame(m_pGame, UNDO, this, FILE_INGAME, FILE_INKEY);
	}
	//otherwise, update the saved game
	else
	{
		m_iIndex = DATABASE->updateGame(m_iIndex, m_pGame, UNDO, this);
	}
}

/////////////////////////////////////////////////////////////////////////////
// can only enable the save if we're in a game
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	BOOL bUpdate;

	//figure out if we should update
	bUpdate =  ((NULL != m_pGame) && 
				(GAME_COMPLETE != m_pGame->m_iState) &&
				(m_iMode == GL_WAIT) &&
				(GAME_MODE_NETWORK != m_pGame->m_iMode)
				);

	pCmdUI->Enable(bUpdate);
}

/////////////////////////////////////////////////////////////////////////////
// once the game is over, we need to remove the saved game from the list
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::removeSave()
{
	if(-1 != m_iIndex)
	{
		DATABASE->deleteIncompleteGame(m_iIndex);
	}
}

/////////////////////////////////////////////////////////////////////////////
// run the general options dialog box
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnOptionsGeneral() 
{
	BOOL bB1, bB2, bA1, bA2;
	BOOL bAnimate1, bAnimate2;
	BOOL bEmbed1, bEmbed2;
	CSettingsGeneralDialog dlg;

	//get current large fonts value
	bB1 = GetPrivateProfileInt(INI_GENERAL, INI_LARGE_FONTS, 0, INI_FILE);
	bB2 = GetPrivateProfileInt(INI_OPENGL, INI_EURO, 0, INI_FILE);
	bAnimate1 = GetPrivateProfileInt(INI_GENERAL, INI_ANIMATE, 1, INI_FILE);
	bEmbed1 = GetPrivateProfileInt(INI_GENERAL, INI_EMBED_CHAT, 1, INI_FILE);

	dlg.DoModal();

	//get new large fonts value
	bA1 = GetPrivateProfileInt(INI_GENERAL, INI_LARGE_FONTS, 0, INI_FILE);
	bA2 = GetPrivateProfileInt(INI_OPENGL, INI_EURO, 0, INI_FILE);
	bAnimate2 = GetPrivateProfileInt(INI_GENERAL, INI_ANIMATE, 1, INI_FILE);
	bEmbed2 = GetPrivateProfileInt(INI_GENERAL, INI_EMBED_CHAT, 1, INI_FILE);

	//see if we need to reload euro textures
	if(bB2 != bA2)
	{
		CWaitCursor wait;
		clearIL();
		buildIL();

		//reset GL textures
		setEuro();
		rebuildTextures();
		rebuildOtherTextures();
	}

	//run dif
	if(bB1 != bA1 || bB2 != bA2)
	{
		initPlayers();
	}

	//change animation if needed
	if(bAnimate1 != bAnimate2)
	{
		ACTIONS->updateAnimate();
		CONTROLS->updateAnimate();
		ADMIN->updateAnimate();
	}

	//change embedding if needed
	if(bEmbed1 != bEmbed2)
	{
		if(TRUE == bEmbed2)
		{
			m_pChatWindow->ShowWindow(SW_HIDE);
			m_pAdminButton->setControls();
			m_pAdminButton->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pAdminButton->ShowWindow(SW_HIDE);
			m_pChatWindow->setControls();
			m_pChatWindow->ShowWindow(SW_SHOW);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// show/hide the legend
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::updateLegend()
{
	int iValue;

	//get the .ini data
	iValue = GetPrivateProfileInt(INI_GENERAL, INI_LEGEND, 0, INI_FILE);

	//display
	if(TRUE == iValue)
	{
		OnOptionsLegend();
	}
}

/////////////////////////////////////////////////////////////////////////////
// keyboard handling
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnTurnBank() 
{
	m_control.OnBankButton();
}

/////////////////////////////////////////////////////////////////////////////
// keyboard handling
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnTurnEnd() 
{
	m_control.OnEndButton();
}

/////////////////////////////////////////////////////////////////////////////
// keyboard handling
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnTurnPlay() 
{
	m_control.OnPlayButton();
}

/////////////////////////////////////////////////////////////////////////////
// keyboard handling
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnTurnTrade() 
{
	m_control.OnTradeButton();
}

/////////////////////////////////////////////////////////////////////////////
// keyboard handling
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnTurnRoll() 
{
	m_control.OnRollButton();
}

/////////////////////////////////////////////////////////////////////////////
// keyboard handling
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnTurnContinue() 
{
	m_control.OnGoodsButton();
}

/////////////////////////////////////////////////////////////////////////////
// see if they're cheating
/////////////////////////////////////////////////////////////////////////////
BOOL CSettlersView::isCheating()
{
	return (GAME_MODE_NETWORK == m_pGame->m_iMode && m_pGame->m_iIndex != getPlayer());
}

/////////////////////////////////////////////////////////////////////////////
// tour callback function
/////////////////////////////////////////////////////////////////////////////
void callbackBalloon(DWORD dwCookie)
{
	//send the message
	if(NULL != dwCookie)
	{
		((CSettlersView *) dwCookie)->handleBalloon();
		((CSettlersView *) dwCookie)->Invalidate();
	}
}

/////////////////////////////////////////////////////////////////////////////
// tour handling
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::closeBalloon()
{
	//close down the current balloon, which triggers the next one
	if(NULL != m_balloon.m_hWnd)
	{
		m_balloon.SendMessage(WM_CLOSE);
	}

	//cheap fix
	if(3 <= m_iBalloon && m_iBalloon <= 10)
	{
		handleBalloon();
	}
}

/////////////////////////////////////////////////////////////////////////////
// tour handling
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::handleBalloon()
{
	int x, y;
	int iFlags = 0;
	int iDelay = 0;
	CRect rect;
	CString str;
	CWnd *pWnd = NULL;

	//sanity
	if(FALSE == APP->inTour())
	{
		return;
	}

	//get client rect
	GetClientRect(&rect);

	//set defaults
	x = rect.Width(); y = rect.Height();
	pWnd = this;

	switch(m_iBalloon)
	{
	case 0:
		str = "This board probably looks vaguely familiar...  This is the main Sea3D window, where the game is played.\nRight now, Sea3D is waiting for the first player to place their first settlement.  You can see\nthis by the fact that a lot of colored circles are on the board in the places where you can place\na settlement.\n\nClick a colored circle where you want to place a settlement.";
		break;
	case 1:
		str = "Now that you've placed an initial settlement, you need to place a road.  You can do this by\nclicking on or near one of the lines around the settlement you just placed.\n\nClick on or near one of the colored lines to place a road.";
		break;
	case 2:
		str = "Now you've got the hang of it.  Just repeat this process until you've placed roads and\nsettlements for both players.";
		break;
	case 3:	case 4:	case 5: 
	case 6: case 7: case 8:
	case 9:
		//increment the counter
		m_iBalloon++;
		return;
		break;
	case 10:
		str = "Let's take a look at some of the other neat features of Sea3D, like the ability to\nrotate, zoom, and pan the map.\n\nTo rotate the map, right-click anywhere in the map, hold down the mouse button,\nand drag the mouse around.\n\nLeft-click anywhere in the map to continue.";
		iFlags = CBalloonHelp::unCLOSE_ON_LBUTTON_DOWN;
		break;
	case 11:
		str = "Now try zooming.\n\nEither rotate your mousewheel, or hold down the right mouse button, press the Ctrl key, and\ndrag the mouse up and down to zoom the map.\n\nLeft-click anywhere in the map to continue.";
		iFlags = CBalloonHelp::unCLOSE_ON_LBUTTON_DOWN;
		break;
	case 12:
		str = "And now, try panning the map.\n\nHold down both mouse buttons and move the mouse around the map.";
		iFlags = CBalloonHelp::unDELAY_CLOSE | CBalloonHelp::unCLOSE_ON_RBUTTON_DOWN;
		iDelay = 5000;
		break;
	case 13:
		str = "Here are just a few more features of Sea3D.\n\nLeft-click anywhere in the map to continue.";
		iFlags = CBalloonHelp::unCLOSE_ON_LBUTTON_DOWN;
		break;
	case 14:
		str = "These are the player buttons which show all the information for each player, like how many points\nthey have, how many soldiers they've played, and how many roads they've built.\n\nLeft-click anywhere in the map to continue.";
		pWnd = &(m_play[0]);
		x = PLAYER_BUTTON_WIDTH; y = PLAYER_BUTTON_HEIGHT;
		iFlags = CBalloonHelp::unCLOSE_ON_LBUTTON_DOWN;
		break;
	case 15:
		str = "This is the bank button, which shows you approxmiately how many of each resource is left in the bank,\njust like looking at the piles of cards in the board game!\n\nLeft-click anywhere in the map to continue.";
		pWnd = &m_bank;
		x = BANK_BUTTON_WIDTH; y = BANK_BUTTON_HEIGHT;
		iFlags = CBalloonHelp::unCLOSE_ON_LBUTTON_DOWN;
		break;
	case 16:
		CONTROLS->GetClientRect(&rect);
		str = "These are the turn action buttons, which you use to play cards, trade with the bank and others, and\nend your turn when you are finished.  Any actions that are available to you on your turn will\nbe lit up and colored.\n\nLeft-click anywhere in the map to continue.";
		pWnd = CONTROLS;
		x = rect.Width() >> 1, y = int(rect.Height() * .78);
		iFlags = CBalloonHelp::unCLOSE_ON_LBUTTON_DOWN;
		break;
	case 17:
		str = "Up here are the menus and toolbar which allow you to save the game, tinker with OpenGL settings,\nlook at the game history, and even look at game statistics while the game is going!\n\nLeft-click anywhere in the map to continue.";
		x = 300; y = 0;
		iFlags = CBalloonHelp::unCLOSE_ON_LBUTTON_DOWN;
		break;
	case 18:
		ACTIONS->GetClientRect(&rect);
		str = "And now, on to the game!  Every time it is your turn, you need to click on the roll button to roll\nthe dice and get things going.\n\nClick the roll button to continue.";
		pWnd = ACTIONS;
		x = rect.Width() >> 1, y = int(rect.Height() * .35);
		break;
	case 19:
		ACTIONS->GetClientRect(&rect);
		str = "Sea3D shows you what goods everyone receives, or tells you to place the robber, depending on the roll.\nAfter you've finished examining who got what, click again in the box to continue.\n\nClick in the goods received box to continue.";
		pWnd = ACTIONS;
		x = rect.Width() >> 1, y = int(rect.Height() * .35);
		break;
	case 20:
		str = "That's the end of the tour!  You're on your own now, but there's help if you need it.  You can go to\nthe Help menu or click Help on the toolbar at anytime to find out more about any part of Sea3D.\n\nClick anywhere to complete the tour.";
		x = 350; y = 0;
		iFlags = CBalloonHelp::unCLOSE_ON_LBUTTON_DOWN;
		break;
	default:
		return;
		break;
	}

	//add to text
	if(20 != m_iBalloon)
	{
		str += "\n\nPress Esc to cancel the tour.";
	}
	else
	{
		//we're done
		APP->setTour(FALSE);
	}

	//create the button
	m_balloon.Create("Sea3D Tour",
		str, 
		CPoint(x, y),
		iFlags | CBalloonHelp::unSHOW_INNER_SHADOW,
		pWnd,
		"",
		iDelay);

	//increment the balloon counter
	m_iBalloon++;
}

/////////////////////////////////////////////////////////////////////////////
// handle a tour quit
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnTourQuit() 
{
	//sanity
	if(FALSE == APP->inTour())
	{
		return;
	}

	//turn off the tour
	APP->setTour(FALSE);

	//shut down the balloon
	if(NULL != m_balloon.m_hWnd)
	{
		m_balloon.SendMessage(WM_CLOSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// take a screenshot!
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnEditScreenshot() 
{
	DWORD dwFlags;
	
	//set flags
	dwFlags = OFN_NONETWORKBUTTON | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	//first we need to get a file name from them
	CFileDialog dlg(FALSE, ".bmp", NULL, dwFlags, SCREENSHOT_FILTER, this);

	//run the dialog
	if(IDCANCEL == dlg.DoModal())
	{
		return;
	}

	//try it
	if(FALSE == screenShot(dlg.GetPathName()))
	{
		MessageBox("Could not save screenshot.", "Screenshot");
	}
}

/////////////////////////////////////////////////////////////////////////////
// call the helpsystem
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnHelp() 
{
	CString str;

	if(FALSE == m_pGame->m_bInGame)
	{
		str = HELP_STARTSEL;
	}
	else
	{
		str = HELP_GL;
	}

	//run help
	if(TRUE == APP->hasHelp())
	{
    #if _MSC_VER >= 1300
		::HtmlHelp(GetDesktopWindow()->GetSafeHwnd(), CString(FILE_HELP) + ">" + str, HH_DISPLAY_TOPIC, NULL);
    #else
		HtmlHelp(GetDesktopWindow()->GetSafeHwnd(), CString(FILE_HELP) + ">" + str, HH_DISPLAY_TOPIC, NULL);
    #endif // _MSC_VER > 1300
	}
	else
	{
		::ShellExecute(NULL, NULL, CString(URL_HELP) + str, NULL, NULL, SW_SHOWNORMAL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// save off the current camera settings
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnFileSaveCamera() 
{
	CString str;

	//create the camera string
	str.Format("%.4f, %.4f, %.4f", m_dCamera[0], m_dCamera[1], m_dCamera[2]);
	WritePrivateProfileString(INI_OPENGL, INI_CAMERA_POS, str, INI_FILE);

	//create the rotation string
	str.Format("%.4f, %.4f", m_dxRot, m_dyRot);
	WritePrivateProfileString(INI_OPENGL, INI_ROTATION, str, INI_FILE);

	//create the look point string
	str.Format("%.4f, %.4f, %.4f", m_dLook[0], m_dLook[1], m_dLook[2]);
	WritePrivateProfileString(INI_OPENGL, INI_LOOK_POS, str, INI_FILE);
}

/////////////////////////////////////////////////////////////////////////////
// clear saved off the current camera settings
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnFileClearCamera() 
{
	WritePrivateProfileString(INI_OPENGL, INI_CAMERA_POS, NULL, INI_FILE);
	WritePrivateProfileString(INI_OPENGL, INI_ROTATION, NULL, INI_FILE);
	WritePrivateProfileString(INI_OPENGL, INI_LOOK_POS, NULL, INI_FILE);
}

/////////////////////////////////////////////////////////////////////////////
// keyboard handler
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bHandle = FALSE;

	switch(nChar)
	{
	//Ctrl+A - save camera location
	case 1: OnFileSaveCamera(); bHandle = TRUE; break;
	//Ctrl+B - run the bank dialog
	case 2: if(CONTROLS->Bank().IsWindowEnabled() && (FALSE == isCheating())) {OnTurnBank(); bHandle = TRUE;} break;
	//Ctrl+E - end turn
	case 5: if(CONTROLS->End().IsWindowEnabled() && (FALSE == isCheating())) {OnTurnEnd(); bHandle = TRUE;} break;
	//Ctrl+M - clear camera
	case 13: OnFileClearCamera(); bHandle = TRUE; break;
	//Ctrl+P - play card
	case 16: if(CONTROLS->Play().IsWindowEnabled() && (FALSE == isCheating())) {OnTurnPlay(); bHandle = TRUE;} break;
	//Ctrl+R - roll
	case 18: if((TRUE == m_control.canRoll()) && (FALSE == isCheating())) {OnTurnRoll(); bHandle = TRUE;} break;
	//Ctrl+S - save game
	case 19: OnFileSave(); bHandle = TRUE; break;
	//Ctrl+R - trade
	case 20: if(CONTROLS->Trade().IsWindowEnabled() && (FALSE == isCheating())) {OnTurnTrade(); bHandle = TRUE;} break;
	//spacebar - continue turn
	case 32: if((TRUE == m_control.canContinue()) && (FALSE == isCheating())) {OnTurnContinue(); bHandle = TRUE;} break;
	}

	//send it to the chat window if it wasn't handled
	if((FALSE == bHandle) && (NULL != m_pSend))
	{
		m_pSend->PostMessage(WM_CHAR, (WPARAM) nChar, MAKELPARAM(nRepCnt, nFlags));
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle keys being pressed
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//check for return
	if((VK_RETURN == nChar) && (NULL != m_pSend))
	{
		m_pSend->GetParent()->PostMessage(WM_COMMAND, IDC_SEND_BUTTON, 0);
	}

	//check for backspace
	if((VK_BACK == nChar) && (NULL != m_pSend))
	{
		m_pSend->PostMessage(WM_KEYDOWN, (WPARAM) nChar, MAKELPARAM(nRepCnt, nFlags));
	}

	//allow arrows to continue through
	if((VK_LEFT == nChar) || (VK_UP == nChar) || (VK_RIGHT == nChar) || (VK_DOWN == nChar))
	{
		CNetworkView::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle keys being pressed
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnKeyUp(UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	//do nothing
}

/////////////////////////////////////////////////////////////////////////////
// never lose focus
/////////////////////////////////////////////////////////////////////////////
void CSettlersView::OnKillFocus(CWnd* pNewWnd)
{
	if(NULL != pNewWnd)
	{
	}
}
