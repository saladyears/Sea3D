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

#if !defined(AFX_NETWORKVIEW_H__17FC2DB1_D1E3_495C_82C8_B9DAA4BAB124__INCLUDED_)
#define AFX_NETWORKVIEW_H__17FC2DB1_D1E3_495C_82C8_B9DAA4BAB124__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "GLView.h"
#include "NDKServer.h"
#include "NDKClient.h"
#include "NetworkListCtrl.h"
#include "ChatControl.h"
#include "TradeOfferDialog.h"
#include "SaveState.h"
#include "NetworkChatDialog.h"
#include "NetworkClientDialog.h"
#include "NetworkServerDialog.h"
#include "NetworkStatusDialog.h"
#include "NetworkTradeDialog.h"
#include "NetworkConnector.h"

/////////////////////////////////////////////////////////////////////////////
// typedefs
/////////////////////////////////////////////////////////////////////////////
typedef CMap <long, long, UINT, UINT> CMapLongToUINT;
typedef CMap <UINT, UINT, long, long> CMapUINTToLong;

/////////////////////////////////////////////////////////////////////////////
// CNetworkView view

class CNetworkView : public CGLView, public CNDKServer, public CNDKClient
{
protected:
	CNetworkView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CNetworkView)

// Attributes
public:
	void setDialog(CDialog *p, BOOL bCanClose = FALSE);
	void setDialog2(CDialog *p) {m_pDialog2 = p;}
	void setListCtrl(CNetworkListCtrl *p) {m_pList = p;}
	void setChatCtrl(CChatControl *p) {m_pChat = p;}
	void setMessageCtrl(CChatControl *p) {m_pMessage = p;}
	void setInfoCtrl(CChatControl *p) {m_pInfo = p;}
	void setSendCtrl(CChatControl *p) {m_pSend = p;}
	void setTradeDialog(CNetworkTradeDialog *p) {m_pTrade = p;}
	void setClientDialog(CNetworkClientDialog *p) {m_pClientDialog = p;}
	void setServerDialog(CNetworkServerDialog *p) {m_pServerDialog = p;}

	CDialog *getDialog(void) {return m_pDialog;}

	//map info string
	void setInfoString(CString str);

	//client message to reset game to original after disconnect
	void resetGame(void);

	//send data to a rejoin player
	void sendRejoinData(long lUserID, int nPlayer);

	//send game data to all clients
	void sendGameData(void);

	//send saved data to all clients
	void sendSaveData(void);

	//common send board function
	void sendBoardData(long lUserID = -1);

	//boot a player by ID
	void bootPlayer(UINT uiID);

	//get a user ID by player ID
	long getSocketID(UINT uiID);

	//are we waiting for players to rejoin
	BOOL isWaiting(void) {return m_saveState.isSaved();}
	CSaveState &getSaveState(void) {return m_saveState;}

// Operations
public:

// Overrides
	 // Overrides from CNDKServer
    BOOL OnIsConnectionAccepted();
    void OnConnect(long lUserId);
    void OnDisconnect(long lUserId, NDKServerDisconnection disconnectionType);
    void OnMessage(long lUserId, CNDKMessage& message);
    void OnPing(long lUserId, long lNbMilliseconds);
    
    // Overrides from CNDKClient
    void OnMessage(CNDKMessage& message);
    void OnDisconnect(NDKClientDisconnection disconnectionType);

	//server methods
	void serverPortPlace(BOOL bStart = FALSE);
	void serverPlayerPlace(BOOL bStart = FALSE);
	void serverStartTurn(BOOL bStart = FALSE);
	void serverThirdRoadPlace(BOOL bStart = FALSE);
	void serverContinueTurn(void);
	void serverSpecialBuild(void);
	void serverPanelText(long lMessage, int iPlayer, long lUserID);
	void serverDrawMode(long lUserID, int iMode, BOOL bUseSelection);
	void serverWaitForLose(long lUserID = -1);
	void serverWaitForGold(long lUserID = -1);
	void serverSysMessage(UINT uiMessage, int iPlayer, CString str = "");
	void serverTimeUpdate(void);
	void serverStartPing(void);
	int serverAI(int iState, int iPlayer = -1, int *iTrade = NULL);
	int serverAIloop(void);
	void serverAITurn(BOOL bRoll = TRUE);
	void serverAISteal(int iTile, int iPlayer, BOOL bPirate);
	void serverAILoseCards(int &iResult, int iPlayer, int *iCards);
	void serverAITradeOffer(int iPlayer, int iReply, int *iOffer);
	void serverAIBuyCard(int iPlayer);
	void serverAIPlayCard(int iPlayer, int iCard, int iData1 = -1, int iData2 = -1);
	void serverSaveState(void);
	void serverRestoreState(void);
	void serverResynchRejoin(long lUserID, int nPlayer);

	//client methods (though server uses them too)
	void clientDispatch(CNDKMessage &msg);
	void clientPortPlace(int nTile, int nSide, int nType);
	void clientSettlePlace(int ID);
	void clientRoadPlace(int ID);
	void clientShipPlace(int ID);
	void clientShipMove(int ID);
	void clientCityPlace(int ID);
	void clientRoll(int iDie1, int iDie2);
	void clientHideGoods(void);
	void clientHideVolcano(void);
	void clientEndTurn(void);
	void clientPlaceRobber(void);
	void clientPlacePirate(void);
	void clientPanelText(long lMessage);
	void clientSteal(int iPlayer, int iRes, BOOL bPirate);
	void clientTradeToBank(int iToBank[5], int iFromBank[5], int iBank[3]);
	void clientTradeJungle(int nJungles, int nFromBank[5]);
	void clientBuyDevCard(int iCard);
	void clientPlayDevCard(int iType, int iCard1 = -1, int iCard2 = -1);
	void clientWaitForLose(BOOL bWait);
	void clientWaitForGold(int nGold);
	void clientSelectGold(void);
	void clientLoseCards(int *iRes);
	void clientTradeOffer(int *iTo, int *iFor, CArray <UINT, UINT> &uiPlayers);
	void clientReplyOffer(UINT uiID, short shStatus);
	void clientTradeCounter(UINT uiID, int *iTo, int *iFor);
	void clientTrade(int iPartner, int *iTo, int *iFrom);
	void clientChitless(int nTile1, int nTile2);
	void clientWin(int nPlayer);
	void clientUndoBuild(int iType, int iUndo, int iOther = -1, int iOther2 = -1, int iOther3 = -1);
	void clientUndoAutoTrade(int *iTo, int *iFrom, int *iBank);
	void clientVolcano(int nRoll);
	void clientSpecialBuildNext(void);
	void clientResynch(int nResynch, int nData1, int nData2);
	void clientReady(UINT uiID, BOOL bReady);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CNetworkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	long m_lWaitID;
	vector <BOOL> m_bWaitForLose;

	//gold handling
	int m_nGoldIndex;
	int m_nGoldCount;
	vector <int> m_nWaitForGold;

	//currently open dialog boxes
	CDialog *m_pDialog;
	CDialog *m_pDialog2;

	//can we close the currently open dialog if someone drops?
	BOOL m_bCanClose;

	//should we add an UNDO_ACTION_START after the next steal message?
	BOOL m_bActionSteal;

	//map of socket ID to player ID
	CMapLongToUINT m_mapPlayerID;

	//map of player ID to socket ID
	CMapUINTToLong m_mapSocketID;
	
	//list control of players
	CNetworkListCtrl *m_pList;

	//open trade dialog
	CNetworkTradeDialog *m_pTrade;

	//current trade ID
	long m_lTradeID;
	UINT m_uiTradeID;

	//ping variables
	UINT m_uiPing;

	//chat control
	CChatControl *m_pChat;
	CChatControl *m_pInfo;
	CChatControl *m_pMessage;
	CChatControl *m_pSend;
	CNetworkChatDialog *m_pChatWindow;
	CString m_strMapInfo;

	//client/server dialogs
	CNetworkClientDialog *m_pClientDialog;
	CNetworkServerDialog *m_pServerDialog;
	CNetworkStatusDialog *m_pStatusDialog;

	//the trade offer dialog
	CTradeOfferDialog *m_pTradeOfferDialog;

	//save states
	CSaveState m_saveState;
	CSaveState m_saveDialog;

// Implementation
protected:
	//handles color assignments for the server
	int handleColors(long lUserID, CPlayer *pPlayer, BOOL bReload);

	//handles verification of reloading a player
	BOOL handleReload(long lUserID, CPlayer *pPlayer);

	//handles a player rejoining a game ingame
	BOOL handleInGameRejoin(long lUserID, CPlayer *pPlayer);

	//handle a trade offer
	void handleTradeOffer(int *iTo, int *iFor, UINT uiID);
	void handleTradeReply(int iPlayer, UINT uiID, short shStatus, int *iTo = NULL, int *iFor = NULL);

	//returns the info string for a trade to bank
	CString getTradeJungleString(int nJungles, int *nFromBank);
	CString getTradeToBankString(int *iToBank, int *iFromBank);
	CString getLoseString(int *iLose);
	CString getTradeString(int iPartner, int *iTo, int *iFor);

	//gold handling functions
	void incrementGold(void);

	//create the status dialog
	void createStatus(void);
	void deleteStatus(void);

	//create the chat window
	void createChatWindow(void);
	void deleteChatWindow(void);

	//all the message handlers
	void OnAddPlayer(CNDKMessage &message, long lUserID = 0);
	void OnRemovePlayer(CNDKMessage &message);
	void OnColorAssign(CNDKMessage &message);
	void OnChatMessage(CNDKMessage &message, long lUserID = 0);
	void OnSysMessage(CNDKMessage &message);
	void OnPingUpdate(CNDKMessage &message);
	void OnStartSend(CNDKMessage &message);
	void OnMap(CNDKMessage &message);
	void OnGame(CNDKMessage &message);
	void OnUndo(CNDKMessage &message);
	void OnStartBoard(CNDKMessage &message);
	void OnBoardPiece(CNDKMessage &message);
	void OnStartPlayer(CNDKMessage &message);
	void OnPlayerPiece(CNDKMessage &message);
	void OnPlayerGame(CNDKMessage &message);
	void OnSavedData(CNDKMessage &message);
	void OnEndSend(CNDKMessage &message);
	void OnEndSave(CNDKMessage &message);
	void OnMapInfo(CNDKMessage &message);
	void OnDrawMode(CNDKMessage &message);
	void OnPanelText(CNDKMessage &message, long lUserID = 0, BOOL bIgnore = FALSE);
	void OnPortPlace(CNDKMessage &message);
	void OnPort(CNDKMessage &message, long lUserID = 0);
	void OnSettlePlace(CNDKMessage &message, long lUserID = 0);
	void OnRoadPlace(CNDKMessage &message, long lUserID = 0);
	void OnShipPlace(CNDKMessage &message, long lUserID = 0);
	void OnCityPlace(CNDKMessage &message, long lUserID = 0);
	void OnShipMove(CNDKMessage &message, long lUserID = 0);
	void OnStartTurn(CNDKMessage &message);
	void OnRoll(CNDKMessage &message, long lUserID = 0, BOOL bIgnore = FALSE);
	void OnHideGoods(CNDKMessage &message, long lUserID = 0);
	void OnHideVolcano(CNDKMessage &message, long lUserID = 0);
	void OnEndTurn(CNDKMessage &message, long lUserID = 0);
	void OnPlaceRobber(CNDKMessage &message, long lUserID = 0);
	void OnPlacePirate(CNDKMessage &message, long lUserID = 0);
	void OnSteal(CNDKMessage &message, long lUserID = 0);
	void OnTradeToBank(CNDKMessage &message, long lUserID = 0);
	void OnTradeJungle(CNDKMessage &message, long lUserID = 0);
	void OnBuyDevCard(CNDKMessage &message, long lUserID = 0);
	void OnPlayDevCard(CNDKMessage &message, long lUserID = 0);
	void OnWaitForLose(CNDKMessage &message);
	void OnWaitForGold(CNDKMessage &message);
	void OnContinue7(CNDKMessage &message);
	void OnContinueRoll(CNDKMessage &message);
	void OnInitGold(CNDKMessage &message);
	void OnSelectGold(CNDKMessage &message, long lUserID = 0);
	void OnLoseCards(CNDKMessage &message, long lUserID = 0);
	void OnTradeOffer(CNDKMessage &message, long lUserID = 0);
	void OnTradeCounter(CNDKMessage &message);
	void OnTradeReply(CNDKMessage &message);
	void OnTrade(CNDKMessage &message, long lUserID = 0);
	void OnChitless(CNDKMessage &message, long lUserID = 0);
	void OnWin(CNDKMessage &message, long lUserID = 0);
	void OnTimeUpdate(CNDKMessage &message);
	void OnVersionCheck(CNDKMessage &message, long lUserID = 0);
	void OnTerminate(void);
	void OnUndoBuild(CNDKMessage &message, long lUserID = 0);
	void OnUndoAutoTrade(CNDKMessage &message, long lUserID = 0);
	void OnVolcano(CNDKMessage &message, long lUserID = 0);
	void OnSpecialStart(CNDKMessage &message);
	void OnSpecialNext(CNDKMessage &message);
	void OnSpecialEnd(CNDKMessage &message);
	void OnSaveState(CNDKMessage &message);
	void OnRestoreState(CNDKMessage &message);
	void OnRejoinSynch(CNDKMessage &message);
	void OnReadyState(CNDKMessage &message, long lUserID = 0);

	//{{AFX_MSG(CNetworkView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKVIEW_H__17FC2DB1_D1E3_495C_82C8_B9DAA4BAB124__INCLUDED_)
