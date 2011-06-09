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

#if !defined(AFX_GLWND_H__77F08ADF_731B_4554_A6D1_3F1E3C4BA731__INCLUDED_)
#define AFX_GLWND_H__77F08ADF_731B_4554_A6D1_3F1E3C4BA731__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GLWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "GLBaseView.h"
#include "Game.h"
#include "BoardPiece.h"
#include "UndoNode.h"
#include "UndoRoll.h"
#include "UndoAction.h"
#include "UndoTrade.h"
#include "UndoMonopoly.h"
#include "UndoEnd.h"
#include "UndoGold.h"
#include "UndoShip.h"
#include "UndoVolcano.h"
#include "Game.h"
#include "MultiUseDialog.h"
#include "MapDatabase.h"

/////////////////////////////////////////////////////////////////////////////
// CGLView window

class CGLView : public CGLBaseView
{
// Construction
public:
	CGLView();

// Attributes
public:

// Operations
public:
	/////////////////////////////////////////////////////////////////////////
	// drawing related functions
	/////////////////////////////////////////////////////////////////////////
	void DrawScene();

	//drawing preparation functions
	int prepareRoads(void);
	int prepareShips(void);
	int prepareCities(void);
	int prepareShipMove(void);
	int prepareSettlements(void);
	void preparePorts(void);
	void preparePortSides(void);
	void prepareInitialRoads(void);
	void prepareInitialShips(void);

	//take a screenshot
	BOOL screenShot(CString strFile);

	//////////////////////////////////////////////////////////////////
	// network functions
	//////////////////////////////////////////////////////////////////
	void SerializeToMessage(CNDKMessage &msg);
	void SerializeFromMessage(CNDKMessage &msg);

	/////////////////////////////////////////////////////////////////////////
	// gameflow related functions
	/////////////////////////////////////////////////////////////////////////

	// returns the current game data
 	CGame *getGame(void) {return m_pGame;}

	//reset all variables
	void resetAll(void);

	//randomizes the player order for hotseat or network games
	void randomizePlayers(void);

	//load data from a map file
	void loadMap(BOOL bClient = FALSE, BOOL bSkip = FALSE);
	void findPorts(void);

	void setAllowedCards(int iCards) {m_iAllowedCards = iCards;}
	void setNumPlayers(void) {m_iNumPlayers = m_pGame->m_players.size();}
	void setNextPlayer(void);
	BOOL setPreviousPlayer(BOOL bIgnoreSpecial = FALSE);
	void setDrawingMode(int iMode);
	void setNetDrawMode(int iMode);
	void setInitialMode(int iMode);
	void setMoveShipMode(void);
	BOOL getIncrease(void) {return m_bIncrease;}
	CString getResName(int iResource, BOOL bLower = FALSE);
	CString getDevName(int iCard);
	CString getLangName(int iLang);
	CString getLangString(int iKnown);

	inline int getAllowedCards(void) {return m_iAllowedCards;}
	inline int getMode(void) {return m_iMode;}
	inline int getPlayer(void) {return m_iPlayer;}
	inline int getRoll(int i) {return m_pBoard[i].getRoll();}
	inline BOOL getSpecialBuild(void) {return m_bSpecialBuild;}
	inline BOOL getCityPlace(void) {return m_bCity;}
	inline BOOL getShipPlace(void) {return (m_iPossibleShips.size() != 0);}

	int incResource(int iType, int iPlayer, int iResource, int iAmount = 1, int iOther = -1);
	void decResource(int iType, int iPlayer, int iResource, int iAmount = 1, int iOther = -1);

	//network induced common code
	void handleCommonSettlePlace(int iPlayer, int iT1, int iC1);
	void handleCommonRoadPlace(int iPlayer, int iT1, int iC1);
	void handleCommonShipPlace(int iPlayer, int iT1, int iC1);
	void handleCommonMoveShip(int iPlayer, int iOT1, int iOC1, int iT1, int iC1);
	BOOL handleCommonInitialShipCheck(int nTile, int nC1);
	void handleCommonSteal(int iPlayer, int iRes, BOOL bPirate);
	void handleCommonCityPlace(int iPlayer, int ID);
	void handleCommonTradeToBank(int iPlayer, int iToBank[5], int iFromBank[5], int iBank[3], BOOL bAuto = FALSE);
	void handleCommonTradeToJungle(int nPlayer, int nJungles, int nFromBank[5]);
	void handleCommonPlayVictory(int iPlayer);
	void handleCommonPlayMonopoly(int iPlayer, int iType);
	void handleCommonPlayRoad(int iPlayer);
	void handleCommonPlayYear(int iPlayer, int iCard1, int iCard2);
	void handleCommonPlaySoldier(int iPlayer);
	void handleCommonLoseCards(int iPlayer, int *iRes);
	void handleCommonTrade(int iPlayer, int iPartner, int *iTradeTo, int *iTradeFor);
	void handleCommonWin(int iPlayer);
	void handleCommonTimeUpdate(int iPlayer, int iSeconds);
	void handleCommonBuyDevCard(int iPlayer, int iCard);
	void handleCommonGold(void);
	void handleCommonPortPlace(int nTile, int nSide, int nType);
	void handleCommonChitless(int nTile1, int nTile2);
	void handleCommonInitSteal(BOOL bAI = FALSE, BOOL bPirate = FALSE);

	//settlement/city common code
	void handleCommonAddSettlement(int nPlayer, int nTile, int nSide);
	void handleCommonRemoveSettlement(int nPlayer, int nTile, int nSide);
	void handleCommonAddCity(int nPlayer, int nTile, int nSide);
	void handleCommonPromoteCity(int nPlayer, int nTile, int nSide);
	void handleCommonReduceCity(int nPlayer, int nTile, int nSide);
	void handleCommonRemoveCity(int nPlayer, int nTile, int nSide);

	//longest road common code
	int handleCommonLongestCheck(int nPlayer);
	void handleCommonLongestFindPlayer(void);
	void handleCommonLongestPoints(int nPlayer, int nResult, int nOwner = -1, BOOL bMove = FALSE);

	//trading to bank
	int tradeToBank(int iType, BOOL bCheck = FALSE);
	void tradeToJungle(int nJungles, int *nNeeded);

	//are there more dev cards to be bought?
	BOOL checkForDevCards(void);

	//see if a hidden tile being revealed is a result of placing
	int checkForHiddenTile(int nTile, int nCorner);

	//see if a chitless tile is being revealed as a result of placing
	void checkForChitlessTile(int nTile, int nCorner, int &nChitless);

	//see if a volcano has erupted and might possibly destroy something
	BOOL checkForVolcano(void);
	int volcanicEruption(int nRoll, vector <int> &nDestruction);

	//pick a random card from a player
	int getRandomCard(int iPlayer);

	//get the current dev card index
	int getDevCardIndex(void) {return m_iDevCardIndex;}

	//setting the cards lost on a 7
	void setLostCards(int *i) {memcpy(m_iLostCards, i, sizeof(m_iLostCards));}

	//set the cards gained on a gold roll
	void setGainedCards(int *i) {memcpy(m_iGainedCards, i, sizeof(m_iGainedCards));}

	//gets the length of the longest road
	inline int getLongestLength(void) {return m_iLongestLength;}

	//does a player have a port
	BOOL havePort(int iPlayer, int iPort);
	BOOL checkForCity(int iPlayer, int iTile);

	//handle road building card
	void playRoadBuild(void) {m_iFreeRoads = 2;}
	void clearRoadBuild(void) {m_iFreeRoads = 0;}

	//chitless tile handling
	void initChitless(void);
	void filterRule3(void);
	void filterRule2(void);
	int filterRule1(void);
	void filterRule0(void);

	//handle ships built this turn
	CArray <int, int> *getShipsThisTurn(void) {return &m_nShipsThisTurn;}
	void clearShipsThisTurn(void) {m_nShipsThisTurn.RemoveAll();}

	//handle ports built on this turn
	CArray <int, int> *getPortsThisTurn(void) {return &m_nPortsThisTurn;}
	void clearPortsThisTurn(void) {m_nPortsThisTurn.RemoveAll();}

	//handle a dice roll
	void handleSeven(int iDie1 = 0, int iDie2 = 0);
	void handleRoll(int iRoll);
	int handleLoseCards(void);
	void runLoseCards(void);
	void initSeven(void);
	void initGold(int iGoods[6][6]);
	void runGold(int *iPlayers);
	int getsGold(void) {return m_nGetsGold;}

	//point functions
	void incPoints(int iPlayer, int iPoints = 1, BOOL bNoUpdate = FALSE);
	void decPoints(int iPlayer, int iPoints = 1, BOOL bNoUpdate = FALSE);
	void incLongestRoad(int nPlayer);
	void decLongestRoad(int nPlayer);
	void checkForWin(void);

	//someone buys a development card in network/hotseat
	int buyDevCard(void);

	//someone plays a soldier card
	void playSoldier(void);

	//mode functions
	void setNextMode(void);
	void setPreviousMode(void);

	/////////////////////////////////////////////////////////////////////////
	// undo related functions
	/////////////////////////////////////////////////////////////////////////
	void addUndo(int i, CUndo *p);
	void undo(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLView)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGLView();

	// Generated message map functions

	/////////////////////////////////////////////////////////////////////////
	// I/O functions
	/////////////////////////////////////////////////////////////////////////
	void saveGame(void);
	void loadGame(int iGame, BOOL bIncomplete = FALSE);
	void updateGame(void);

protected:
	void save(CArchive &ar);
	void load(CArchive &ar);

	/////////////////////////////////////////////////////////////////////////
	// game data functions and variables
	/////////////////////////////////////////////////////////////////////////
	void generateBoard(void);
	void generateRandomTiles(void);
	void generateHiddenTiles(void);
	void generateChitlessTiles(void);
	void generatePorts(void);
	void generatePortPlace(void);
	void generateDevCards(void);
	void generateVolcanoes(void);
	void generateJungles(void);
	BOOL validateChits(void);
	void buildBoard(void);
	void setChips(void);
	void randomizeArray(vector <int> *pArray);
	BOOL roadCheck(int iTile, int iCorner);
	BOOL cityCheck(int iTile, int iCorner);
	BOOL cityBlock(int iTile, int iCorner);
	BOOL seaBlock(int iTile, int iSide);
	BOOL landBlock(int iTile, int iSide);
	int deRotate(int &iTile, int &iCorner);
	void invalidPlaces(BOOL bShip, int iTile, int iCorner, BOOL &b12, BOOL &b13, BOOL &b23);

	//deletes the current game
	void deleteGame(void);

	//port placement
	void initPortPlacement(void);

	//init the steal dialog
	BOOL initSteal(CMultiUseDialog *pDlg);
	BOOL initPirateSteal(CMultiUseDialog *pDlg);

	//look for largest army
	int largestArmy(void);

	//look for longest road
	int longestPlayer(void);
	int longestRoad(int iPlayer);
	BOOL longestIterateStart(int iTile, int iSide, int iPlayer, int iOwner, BOOL bShip);
	BOOL longestIterateNext(int &iLength, int iAvoid, int iTile, int iSide, int iPlayer, int iOwner, BOOL bShip);
	void calculateRoadLengths(void);

	//extra points handling
	int calculateExtraPoints(int iTile, int iCorner, int iPlayer);
	BOOL extraIterate(int iTile, int iPlayer, int iID1, int iID2, int iID3);

	//port calculation
	int portIterateStart(int iTile);
	int portIterateNext(int iTile, int iSide, int &iLength);

	//trade route calculation
	void tradeRoutes(void);
	int tradeIterateStart(int nTile, int nSide);
	int tradeIterateNext(int nTile, int nSide, int nTileAvoid, int nPlayerCount, int nOpponentCount, 
					int nTradeOpponent, BOOL bPlayerClosed, int nLastTurn);

	//ship loop calculation
	BOOL shipIterate(int nCity, int nTile, int nSide, int nAvoid);

	//ship movment function
	BOOL moveShip(int iTile, int iSide);

	//screen shot function
	void screen(void);

	//the game
	CGame *m_pGame;

	//board pieces
	CArray <CBoardPiece, CBoardPiece> m_pBoard;

	//ports
	CArray <int, int> m_iPorts;

	//the drawing commands
	int m_nGLDraw;
	
	//current setup place
	int m_iSetup;

	//the number of allowed cards in hand
	int m_iAllowedCards;

	//the number of players who lost cards
	int m_iLostTotal;

	//player placement
	int m_iLastCity;
	BOOL m_bIncrease;
	BOOL m_bCity;

	//special build phase flags
	int m_nSpecialStart;
	BOOL m_bSpecialBuild;

	//ship placements this turn
	CArray <int, int> m_nShipsThisTurn;

	//port builds this turn
	CArray <int, int> m_nPortsThisTurn;

	//number of land and sea tiles
	//int m_iLandTiles;			//RETIRED IN 1.0.1
	//int m_iSeaTiles;			//RETIRED IN 1.0.1
	int m_nTiles;

	//number of players in the game
	int m_iNumPlayers;

	//the current location of the robber
	int m_iRobber;
	int m_iRobberPlayer;

	//the current location of the pirate
	int m_iPirate;
	int m_iPiratePlayer;

	//dev cards
	int m_iDevCardIndex;
	vector <int> m_iDevCardArray;
	vector <int> m_iDevCardSet;

	//hidden tiles				//ADDED IN 1.0.2
	int m_nHiddenIndex;
	vector <int> m_nHiddenArray;

	int m_nHiddenVolcanoIndex;
	vector <int> m_nHiddenVolcanoArray;		//ADDED IN 1.1.2

	//hidden roll chits			//ADDED IN 1.0.2
	int m_nHiddenChitIndex;
	vector <int> m_nHiddenChitArray;

	//chitless roll chits		//ADDED IN 1.0.4
	int m_nChitlessTile;
	int m_nChitlessChitIndex;
	vector <int> m_nChitlessChitArray;

	//port placement
	int m_nPortTile;
	int m_nPortIndex;
	int m_nPortRes;
	vector <int> m_nPortArray;
	BOOL m_bPortPlacement;

	//cards lost on a 7
	int m_iLostCards[6][5];

	//cards gained on a gold roll
	int m_iGainedCards[6][5];
	int m_nGetsGold;
	int m_nGoldPlayers[6];

	//the length of the longest road
	int m_iLongestLength;

	//helper for undo
	//BOOL m_bInGame;		//MOVED TO GAME.H IN 0.9.2

	//whether a soldier is being played
	BOOL m_bSoldier;

	//for road building, how many free road placements are left
	int m_iFreeRoads;

	//last rolled tiles
	vector <int> m_iLastTiles;

	//volcano stuff
	int m_nVolcanoRoll;
	vector <int> m_nLastVolcano;

	//prepared drawing places
	vector <int> m_iPossibleRoads;
	vector <int> m_iPossibleShips;
	vector <int> m_iPossibleCities;
	vector <int> m_iPossibleSettlements;
	vector <int> m_iPossibleShipMoves;
	vector <int> m_iPossiblePorts;
	vector <int> m_iPossiblePortSides;
	int m_iMovingShip[2];
	int m_iLastMoveTurn;

	//longest road storage
	vector <int> m_iTempRoad;
	vector <int> m_iLongestRoad;

	//trade route variables
	int m_nTradeIsland;
	int m_nTradePlayer;

	//roll placement hexes
	//vector <int> m_iRollHexes;		//RETIRED IN VER_GREATER_CATAN (1.0.4)

	/////////////////////////////////////////////////////////////////////////
	// timer functions
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	// undo functions and variables
	/////////////////////////////////////////////////////////////////////////
	void undoSettlementPlace(int ID);
	void undoRoadPlace(CUndoShip *pShip);
	void undoThirdRoadPlace(CUndoShip *pShip);
	void undoShipPlace(CUndoShip *pShip);
	void undoRoll(CUndoRoll *pRoll);
	void undoVolcano(CUndoVolcano *pVolcano);
	void undoJungle(CUndoTrade *pJungle);
	void undoActionStart(CUndoAction *pAction);
	void undoActionEnd(CUndoEnd *pEnd);
	void undoAction(void);
	void undoRoad(CUndoShip *pShip);
	void undoShip(CUndoShip *pShip);
	void undoCity(int ID);
	void undoSettlement(int ID);
	void undoRobber(int iTile);
	void undoPirate(int iTile);
	void undoBuyCard(int iPlayer);
	void undoPlayVictory(int ID);
	void undoPlayMonopoly(CUndoMonopoly *pMonopoly);
	void undoPlayRoadBuild(void);
	void undoPlayYearPlenty(int ID);
	void undoPlaySoldier(int iStat);
	void undoTrade(CUndoTrade *pTrade);
	void undoSteal(int ID);
	void undoMoveShip(CUndoShip *pShip);
	BOOL undoSpecialBuild(void);
	void undoSpecialBuildEnd(void);
	BOOL handleSteal(BOOL bPirate);
	void undoLoseCards(CUndoTrade *pLose);
	int *undoGold(CUndoGold *pGold, int *iPlayers);
	void undoChitless(int ID);
	BOOL handleGold(int *iPlayers);
	void undoHiddenTile(int nTile);
	void undoChitlessTile(int nTile);

	/////////////////////////////////////////////////////////////////////////
	// undo functions and variables
	/////////////////////////////////////////////////////////////////////////
	void handleCommonUndoRoad(int ID, int nHidden, int nChitless);
	void handleCommonUndoShip(int ID, int nHidden, int nChitless);
	void handleCommonUndoShipMove(int ID, int nHidden, int nChitless, int nTurn);
	void handleCommonUndoCity(int ID);
	void handleCommonUndoSettlement(int ID);
	void handleCommonUndoTrade(int iPartner, int *iTo, int *iFrom, int *iBank);
	void handleCommonUndoJungle(int nJungles, int *nFrom);

	void drawVolcano(void);
	void drawShipChoose(void);
	void drawRobberPlace(void);
	void drawSettlePlace(void);
	void drawCityPlace(void);
	void drawRoadPlace(void);
	void drawShipPlace(void);
	void drawPortPlace(void);
	void drawChitlessTile(void);
	void drawGoods(void);
	void drawPlayerPlace(void);
	void drawInitialSettlePlace(GLMaterial *pMat);
	void drawInitialRoadPlace(GLMaterial *pMat);
	void drawNormal(void);
	void drawBase(void);
	void drawStart(void);
	void drawEnd(void);

	//helper draw
	void drawTile(int iTile);
	void drawTileVolcano(int nTile);
	void drawTileOutline(int iTile, GLMaterial *mat, int iWidth = 3.0);
	void drawTileEdge(int iTile, int iSide);
	void drawChip(int iTile);
	void drawCities(int iTile);
	void drawRoads(int iTile);
	void drawRobber(void);
	
	//player color
	void playerColor(CPlayerGame *pPlayer, GLMaterial *pMat);

	//rotation/zoom functions
	void resetEye(void);

	//select functions
	void SelectObject(int id);
	void handleSelect(void);
	void handleSelectPlace(void);
	BOOL handleSelectRoad(void);
	BOOL handleSelectShip(void);
	void handleSelectCity(void);
	void handleSelectSettle(void);
	void handleSelectRobber(void);
	void handleSelectPirate(void);
	void handleSelectChooseShip(void);
	void handleSelectMoveShip(void);
	void handleSelectPort(void);
	void handleSelectChitless(void);
	void handleSelectThirdRoad(void);

	//lighting reset
	void resetLight();

	//tile selection reset
	void resetTileSelect();

	//drawing mode
	int m_iMode;

	//current player
	int m_iPlayer;

	//are we in tournament additional roads mode
	BOOL m_bTournamentRoads;

	//roll mode
	//int m_iChip;				//RETIRED IN VER_GREATER_CATAN (1.0.4)
	//int m_iChipNext;			//RETIRED IN VER_GREATER_CATAN (1.0.4)

	//timer resolution
	UINT m_uiTimerRes;
	UINT m_uiTimer;

	//light array
	vector <BOOL> m_bLight;

	//tile selection array
	vector <BOOL> m_bTileSelect;

	//screenshot variables
	BOOL m_bScreen;
	CString m_strScreenPath;

	//{{AFX_MSG(CGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLWND_H__77F08ADF_731B_4554_A6D1_3F1E3C4BA731__INCLUDED_)
