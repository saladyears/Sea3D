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

#if !defined(AFX_PLAYERGAME_H__7FAE43F7_B11C_49DE_9959_85E8DB2D1115__INCLUDED_)
#define AFX_PLAYERGAME_H__7FAE43F7_B11C_49DE_9959_85E8DB2D1115__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "define.h"
#include "Player.h"

//////////////////////////////////////////////////////////////////////
// class which holds information about one player's game
//////////////////////////////////////////////////////////////////////
class CPlayerGame : public CObject  
{
public:
	DECLARE_SERIAL(CPlayerGame)

	//{{AFX_VIRTUAL(CPlayerGame)
	public:
//conditional include
#ifndef AI_DLL
	virtual void Serialize(CArchive& ar);
#endif
	//}}AFX_VIRTUAL

	CPlayerGame();
	CPlayerGame(const CPlayerGame &data);
	virtual ~CPlayerGame();

	//assignment operator
	CPlayerGame &operator =(const CPlayerGame &data);

	//streamline copy function
	void copy(const CPlayerGame &data);

	//can this person trade?
	BOOL canTrade(void);

	//how many cards does this player hold?
	int totalCards(void);

//conditional include
#ifndef AI_DLL
	//total and playable dev cards
	int totalDevCards(void);
	int playDevCards(void);

	//initialize function
	void initPlayer(int iSize);

	//copies bought dev cards to playable dev cards at the end of a turn
	void copyDevCards(void);
	int *getCardsBought(void) {return m_iCardBought;}

	//////////////////////////////////////////////////////////////////
	// network functions
	//////////////////////////////////////////////////////////////////
	void SerializeToMessage(CNDKMessage &msg);
	void SerializeFromMessage(CNDKMessage &msg);
#endif

public:
	//////////////////////////////////////////////////////////////////////
	// all the data
	//////////////////////////////////////////////////////////////////////

	//the player
	CPlayer m_player;

	//player color
	//COLORREF m_color;		//RETIRED IN VERSION 0.8.1
	int m_iColor;

	//temporary variable for use in reloading network games
	BOOL m_bJoined;

	//cities and settlements
	vector <int> m_iSettlements;
	vector <int> m_iCities;

	//the number of points the player has
	int m_iPoints;

	//current amounts of resources
	int m_iRes[5];

	//the amount of jungles
	int m_nJungles;

	//trade offers used this turn
	int m_nTradeOffers;
	
	//amounts of resources gained through various means
	int m_iResBoard[5];
	int m_iResStolen[6][5];
	int m_iResDev[5];
	int m_iResGold[5];
	int m_iResDiscover[5];
	
	//amounts of resources lost through various means
	int m_iLostBoard[6][5];
	int m_iLostStolen[6][5];
	int m_iLostRoll[5];

	//the number of items in stock
	int m_nStockRoads;
	int m_nStockShips;
	int m_nStockCities;
	int m_nStockSettle;

	//whether they've gotten bonuses for building on islands
	BOOL m_bBonus1;
	BOOL m_bBonus2;

	//the player's home island (in Great Crossing maps)
	int m_nHomeIsland;			//ADDED IN 1.0.4
	int m_nTradePoints;			//ADDED IN 1.0.4

	//the number of development cards in hand, playable, and played
	int m_iCardBought[5];		//cards bought on this turn and not playable
	int m_iCardHeld[5];			//cards playable on this turn
	int m_iCardPlayed[5];		//cards played to date

	//has the player played a card on this turn?
	BOOL m_bPlayedCard;

	//has the player placed a building on this turn?
	BOOL m_bPlacedBuild;

	//has the player moved a ship on this turn?
	BOOL m_bMovedShip;

	//does this player have largest army?
	BOOL m_bLargestArmy;

	//the length of their roads
	int m_iRoadLength;

	//does this player have longest road?
	BOOL m_bLongestRoad;

	//number of trades with the bank
	int m_iTradesBank[3];

	//number of trades with other people
	int m_iTradesOthers[6];

	//cards traded to the bank
	int m_iBankTradeTo[5];
	int m_iBankTradeFrom[5];

	//cards traded to others
	int m_iOtherTradeTo[6][5];

	//cads received from others
	int m_iOtherTradeFrom[6][5];

	//the number of seconds this player is taking per turn
	vector <int> m_iTurnTime;
private:
//conditional include
#ifndef AI_DLL
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);
#endif
};

#endif // !defined(AFX_PLAYERGAME_H__7FAE43F7_B11C_49DE_9959_85E8DB2D1115__INCLUDED_)
