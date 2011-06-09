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

#if !defined(AFX_SETTLERSVIEW_H__1C7EF9D0_78EC_4522_AFF8_C11BD5E46798__INCLUDED_)
#define AFX_SETTLERSVIEW_H__1C7EF9D0_78EC_4522_AFF8_C11BD5E46798__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// includes
/////////////////////////////////////////////////////////////////////////////
#include "NetworkView.h"
#include "TinyWnd.h"
#include "ControlPanel.h"
#include "GLSettingsDialog.h"
#include "SettlersDoc.h"
#include "NewGameDialog.h"
#include "UndoDialog.h"
#include "StatsDialog.h"
#include "PlayerButton.h"
#include "BankButton.h"
#include "LegendButton.h"
#include "LegendDialog.h"
#include "MapInfoDialog.h"
#include "ControlsButton.h"
#include "ActionsButton.h"
#include "MessageButton.h"
#include "AdminButton.h"
#include <il\il.h>
#include "BalloonHelp.h"
#include "defineDisplay.h"
#include "defineHistory.h"

/////////////////////////////////////////////////////////////////////////////
// the main view class
/////////////////////////////////////////////////////////////////////////////
class CSettlersView : public CNetworkView
{
protected: // create from serialization only
	CSettlersView();
	DECLARE_DYNCREATE(CSettlersView)

// Attributes
public:
	CSettlersDoc* GetDocument();

	//are we initialized?
	BOOL isInit(void) {return m_bInit;}

	//controls button
	CControlsButton *getControlsButton(void) {return m_pControlsButton;}

	//actions button
	CActionsButton *getActionsButton(void) {return m_pActionsButton;}

	//messages button
	CMessageButton *getMessageButton(void) {return m_pMessagesButton;}

	//admin button
	CAdminButton *getAdminButton(void) {return m_pAdminButton;}

	//return panels
	CControlPanel *getControlPanel(void) {return &m_control;}

	//returns the undo window
	CUndoDialog *getUndoDialog(void) {return m_pUndoDlg;}

	//returns the stat window
	CStatsDialog *getStatDialog(void) {return m_pStatsDlg;}

	//initalizes the panel
	void initPanel(void);

	//init network mode
	void initNetwork(void);

	//initialize chat window/admin button
	void initChatAdmin(void);

	//auto save game
	void autoSave(void) {OnFileSave();}
	void removeSave(void);

	//update legend display
	void updateLegend(void);

	//update player button autohide
	void updateAutohide(void) {for(int i=0; i < m_iNumPlayers; m_play[i++].updateAutohide()); m_bank.updateAutohide();}

	//reset the undo dialog
	void resetUndo(void);

	//tiny windows
	CTinyWnd *getTinyR(void) {return m_tinyR;}
	CTinyWnd *getTinySh(void) {return m_tinySh;}
	CTinyWnd *getTinyS(void) {return m_tinyS;}
	CTinyWnd *getTinyC(void) {return m_tinyC;}
	CTinyWnd *getTinyDev(void) {return m_tinyDev;}
	CTinyWnd *getTinyMove(void) {return m_tinyMove;}

// Operations
public:
	//whether we're inited
	BOOL m_bInit;

	//create IL arrays
	void buildIL(void);
	void clearIL(void);

	//get an HBITMAP from an IL image
	HBITMAP getTinyImage(int iImage, HDC hDC);
	HBITMAP getResImage(int iImage, HDC hDC);
	HBITMAP getHistoryImage(int iImage, HDC hDC);
	HBITMAP getControlImage(int iImage, HDC hDC);
	HBITMAP getDiceImage(int iImage, HDC hDC);
	HBITMAP getDeckImage(int iImage, HDC hDC);
	HBITMAP getCardsImage(int iImage, HDC hDC);

	//show/hide tiny windows
	void showRoad(BOOL b, BOOL bAuto);
	void showShip(BOOL b, BOOL bAuto);
	void showSettlement(BOOL b, BOOL bAuto);
	void showCity(BOOL b, BOOL bAuto);
	void showDevCard(BOOL b, BOOL bAuto);
	void showMove(BOOL b);

	//update build possibilities
	void showBuildOptions(BOOL bShow);

	//show initital road/ship options
	void showInitialOptions(BOOL bShow);

	//initialize player buttons
	void initPlayers(void);
	void redrawPlayers(void);

	//keep player counts accurate
	void updateCounts(int nPlayer);
	void updatePlayerDisplay(void);

	//set the game index
	void inline setGameIndex(int i) {m_iIndex = i;}

	//player button width
	int playerWidth(void) {CRect rect; if(m_play[0].m_hWnd) m_play[0].GetClientRect(&rect); return rect.Width();}
	int playerHeight(void) {CRect rect; if(m_play[0].m_hWnd) m_play[0].GetClientRect(&rect); return rect.Height();}
	int bankHeight(void) {CRect rect; if(m_bank.m_hWnd) m_bank.GetClientRect(&rect); return rect.Height();}

	//balloon help function
	void handleBalloon(void);
	void closeBalloon(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettlersView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSettlersView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CSettlersView)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnGlSettings();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnOptionsGameHistory();
	afx_msg void OnOptionsStats();
	afx_msg void OnFileClose();
	afx_msg void OnAppExit();
	afx_msg void HandlePlayerClick(UINT nID);
	afx_msg void OnOptionsDisplay();
	afx_msg void OnUpdateOptionsSysMessages(CCmdUI* pCmdUI);
	afx_msg void OnOptionsSysMessages();
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnOptionsGeneral();
	afx_msg void OnTurnEnd();
	afx_msg void OnTurnPlay();
	afx_msg void OnTurnTrade();
	afx_msg void OnTurnRoll();
	afx_msg void OnTurnBank();
	afx_msg void OnTurnContinue();
	afx_msg void OnOptionsLegend();
	afx_msg void OnTourQuit();
	afx_msg void OnEditScreenshot();
	afx_msg void OnHelp();
	afx_msg void OnFileSaveCamera();
	afx_msg void OnFileClearCamera();
	afx_msg void OnOptionsMapInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:

	//the number of players
	int m_iPlayers;

	//the number of this incomplete game in the game database
	int m_iIndex;

	//player buttons
	CPlayerButton m_play[6];

	//bank button
	CBankButton m_bank;

	//the tiny windows
	CTinyWnd *m_tinyR;
	CTinyWnd *m_tinySh;
	CTinyWnd *m_tinyS;
	CTinyWnd *m_tinyC;
	CTinyWnd *m_tinyDev;
	CTinyWnd *m_tinyMove;

	//the Controls button
	CControlsButton		*m_pControlsButton;

	//the Actions buttonb
	CActionsButton		*m_pActionsButton;

	//the messages button
	CMessageButton		*m_pMessagesButton;

	//the admin button
	CAdminButton		*m_pAdminButton;

	//the panel
	CControlPanel		m_control;

	//dialog windows
	CGLSettingsDialog	*m_pGLSettingsDlg;
	CUndoDialog			*m_pUndoDlg;
	CStatsDialog		*m_pStatsDlg;
	CLegendDialog		*m_pLegendDlg;
	CMapInfoDialog		*m_pMapInfoDlg;

	//the arrays of IL bitmaps
	ILuint m_ilTiny[TINY_SIZE];
	ILuint m_ilRes[IMG_SIZE];
	ILuint m_ilHistory[HISTORY_SIZE];
	ILuint m_ilControl[BUTTON_SIZE];
	ILuint m_ilDice[DICE_SIZE];
	ILuint m_ilDeck[DECK_SIZE << 1];
	ILuint m_ilCards[CARD_SIZE];

	//balloon help
	int m_iBalloon;
	CBalloonHelp m_balloon;

	//are they trying to cheat with keyboard shortcuts in network mode?
	BOOL isCheating(void);

	//euro textures function
	CString getEFile(CString str, BOOL bPossible);
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};

#ifndef _DEBUG  // debug version in SettlersView.cpp
inline CSettlersDoc* CSettlersView::GetDocument()
   { return (CSettlersDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTLERSVIEW_H__1C7EF9D0_78EC_4522_AFF8_C11BD5E46798__INCLUDED_)
