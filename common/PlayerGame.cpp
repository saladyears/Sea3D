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
#include "PlayerGame.h"

//conditional include
#ifndef AI_DLL
#include "settlers.h"
#include "GameDatabase.h"
#include "ColorDatabase.h"
#include "MapDatabase.h"
#include "SettlersView.h"
#include "math.h"
#include "defineVariants.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CPlayerGame, CObject, 1)

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CPlayerGame::CPlayerGame()
{
	//no color
	m_iColor = 0;

	//has not joined
	m_bJoined = FALSE;

	//resources
	::ZeroMemory(m_iRes, sizeof(m_iRes));

	//jungles
	m_nJungles = 0;

	//trade offers used
	m_nTradeOffers = 0;

	//amounts of resources gained through various means
	::ZeroMemory(m_iResBoard, sizeof(m_iResBoard));
	::ZeroMemory(m_iResStolen, sizeof(m_iResStolen));
	::ZeroMemory(m_iResDev, sizeof(m_iResDev));
	::ZeroMemory(m_iResGold, sizeof(m_iResGold));
	::ZeroMemory(m_iResDiscover, sizeof(m_iResDiscover));

	//set lost values
	::ZeroMemory(m_iLostBoard, sizeof(m_iLostBoard));
	::ZeroMemory(m_iLostStolen, sizeof(m_iLostStolen));
	::ZeroMemory(m_iLostRoll, sizeof(m_iLostRoll));

	//set played values
	::ZeroMemory(m_iCardBought, sizeof(m_iCardBought));
	::ZeroMemory(m_iCardHeld, sizeof(m_iCardHeld));
	::ZeroMemory(m_iCardPlayed, sizeof(m_iCardPlayed));

	//has not played a card yet
	m_bPlayedCard = FALSE;

	//has not build anything yet
	m_bPlacedBuild = FALSE;

	//no ship moved yet
	m_bMovedShip = FALSE;

	//start with zero points
	m_iPoints = 0;

	//does not have largest army
	m_bLargestArmy = FALSE;

	//length of their roads
	m_iRoadLength = 0;

	//does not have longest road
	m_bLongestRoad = FALSE;

	//bonus values
	m_bBonus1 = FALSE;
	m_bBonus2 = FALSE;

	//default home island
	m_nHomeIsland = 0;
	m_nTradePoints = 0;

	//number of trades with the bank
	::ZeroMemory(m_iTradesBank, sizeof(m_iTradesBank));

	//number of trades with other people
	::ZeroMemory(m_iTradesOthers, sizeof(m_iTradesOthers));

	//cards traded to and from the bank
	::ZeroMemory(m_iBankTradeTo, sizeof(m_iBankTradeTo));
	::ZeroMemory(m_iBankTradeFrom, sizeof(m_iBankTradeFrom));

	//cards traded to others
	::ZeroMemory(m_iOtherTradeTo, sizeof(m_iOtherTradeTo));
	::ZeroMemory(m_iOtherTradeFrom, sizeof(m_iOtherTradeFrom));
}

//////////////////////////////////////////////////////////////////////
// copy constructor
//////////////////////////////////////////////////////////////////////
CPlayerGame::CPlayerGame(const CPlayerGame &data)
{
	copy(data);
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CPlayerGame::~CPlayerGame()
{

}

//////////////////////////////////////////////////////////////////////
// assignment operator
//////////////////////////////////////////////////////////////////////
CPlayerGame &CPlayerGame::operator =(const CPlayerGame &data)
{
	copy(data);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// streamline copy function
//////////////////////////////////////////////////////////////////////
void CPlayerGame::copy(const CPlayerGame &data)
{
	//player
	m_player				= data.m_player;

	//player color
	//m_color					= data.m_color;
	m_iColor				= data.m_iColor;

	//joined status
	m_bJoined				= data.m_bJoined;

	//cities and settlements
	m_iSettlements			= data.m_iSettlements;
	m_iCities				= data.m_iCities;

	//the number of points the player has
	m_iPoints				= data.m_iPoints;

	//current amounts of resources
	memcpy(m_iRes, data.m_iRes, sizeof(m_iRes));

	//jungles
	m_nJungles				= data.m_nJungles;

	//trade offers used
	m_nTradeOffers			= data.m_nTradeOffers;

	//total amounts of resources
	memcpy(m_iResBoard, data.m_iResBoard, sizeof(m_iResBoard));
	memcpy(m_iResStolen, data.m_iResStolen, sizeof(m_iResStolen));
	memcpy(m_iResDev, data.m_iResDev, sizeof(m_iResDev));
	memcpy(m_iResGold, data.m_iResGold, sizeof(m_iResGold));
	memcpy(m_iResDiscover, data.m_iResDiscover, sizeof(m_iResDiscover));
	
	//amounts of resources lost
	memcpy(m_iLostBoard, data.m_iLostBoard, sizeof(m_iLostBoard));
	memcpy(m_iLostStolen, data.m_iLostStolen, sizeof(m_iLostStolen));
	memcpy(m_iLostRoll, data.m_iLostRoll, sizeof(m_iLostRoll));
	
	//the number of items in stock
	m_nStockRoads			= data.m_nStockRoads;
	m_nStockShips			= data.m_nStockShips;
	m_nStockCities			= data.m_nStockCities;
	m_nStockSettle			= data.m_nStockSettle;

	//bonuses
	m_bBonus1				= data.m_bBonus1;
	m_bBonus2				= data.m_bBonus2;

	//home island
	m_nHomeIsland			= data.m_nHomeIsland;
	m_nTradePoints			= data.m_nTradePoints;

	//development cards
	memcpy(m_iCardBought, data.m_iCardBought, sizeof(m_iCardBought));
	memcpy(m_iCardHeld, data.m_iCardHeld, sizeof(m_iCardHeld));
	memcpy(m_iCardPlayed, data.m_iCardPlayed, sizeof(m_iCardPlayed));

	//whether they've played a dev card
	m_bPlayedCard			= data.m_bPlayedCard;

	//whether they've build this turn
	m_bPlacedBuild			= data.m_bPlacedBuild;

	//whether they've moved a ship this turn
	m_bMovedShip			= data.m_bMovedShip;

	//largest army
	m_bLargestArmy			= data.m_bLargestArmy;

	//length of their roads
	m_iRoadLength			= data.m_iRoadLength;

	//longest road
	m_bLongestRoad			= data.m_bLongestRoad;

	//number of trades with the bank
	memcpy(m_iTradesBank, data.m_iTradesBank, sizeof(m_iTradesBank));

	//number of trades with other people
	memcpy(m_iTradesOthers, data.m_iTradesOthers, sizeof(m_iTradesOthers));

	//cards traded to the bank
	memcpy(m_iBankTradeTo, data.m_iBankTradeTo, sizeof(m_iBankTradeTo));
	memcpy(m_iBankTradeFrom, data.m_iBankTradeFrom, sizeof(m_iBankTradeFrom));

	//cards traded to others
	memcpy(m_iOtherTradeTo, data.m_iOtherTradeTo, sizeof(m_iOtherTradeTo));
	memcpy(m_iOtherTradeFrom, data.m_iOtherTradeFrom, sizeof(m_iOtherTradeFrom));
}

//////////////////////////////////////////////////////////////////////
// can this person trade?
//////////////////////////////////////////////////////////////////////
BOOL CPlayerGame::canTrade()
{
	return (m_iRes[0] || m_iRes[1] || m_iRes[2] || m_iRes[3] || m_iRes[4]);
}

//////////////////////////////////////////////////////////////////////
// how many cards this player holds
//////////////////////////////////////////////////////////////////////
int CPlayerGame::totalCards()
{
	return (m_iRes[0] + m_iRes[1] + m_iRes[2] + m_iRes[3] + m_iRes[4]);
}

//conditional include
#ifndef AI_DLL

//////////////////////////////////////////////////////////////////////
// how many total dev cards does this person have?
//////////////////////////////////////////////////////////////////////
int CPlayerGame::totalDevCards()
{
	int i;
	int iCount = 0;

	//count cards
	for(i = 0; i < 5; i++)
	{
		iCount += m_iCardBought[i];
		iCount += m_iCardHeld[i];
	}

	return iCount;
}

//////////////////////////////////////////////////////////////////////
// how many playable dev cards does this person have?
//////////////////////////////////////////////////////////////////////
int CPlayerGame::playDevCards()
{
	int iCount = 0;

	//get base count
	iCount = m_iCardHeld[0] + m_iCardHeld[1] + m_iCardHeld[2] + m_iCardHeld[3] + m_iCardHeld[4];

	//include the cards they've just bought so they can see
	//what they have to play later.
	iCount += (m_iCardBought[0] + m_iCardBought[1] + m_iCardBought[2] + m_iCardBought[3] + m_iCardBought[4]);

	return iCount;
}

//////////////////////////////////////////////////////////////////////
// initialize settings
//////////////////////////////////////////////////////////////////////
void CPlayerGame::initPlayer(int iSize)
{
	//resize the city and settlement arrays
	m_iSettlements.resize(iSize);
	m_iCities.resize(iSize);

	//starting stocks
	if(IS_VARIANT(VARIANT_ALTER_ROADS))
	{
		m_nStockRoads = VARIANTS->m_iStockRoads;
	}
	else
	{
		m_nStockRoads = MAP->m_iStockRoads;
	}

	if(IS_VARIANT(VARIANT_ALTER_CITIES))
	{
		m_nStockCities = VARIANTS->m_iStockCities;
	}
	else
	{
		m_nStockCities = MAP->m_iStockCities;
	}

	if(IS_VARIANT(VARIANT_ALTER_SETTLEMENTS))
	{
		m_nStockSettle = VARIANTS->m_iStockSettlements;
	}
	else
	{
		m_nStockSettle = MAP->m_iStockSettlements;
	}

	if(IS_VARIANT(VARIANT_ALTER_SHIPS))
	{
		m_nStockShips = VARIANTS->m_iStockShips;
	}
	else
	{
		m_nStockShips = MAP->m_iStockShips;
	}
}

//////////////////////////////////////////////////////////////////////
// copy bought dev cards to playable cards
//////////////////////////////////////////////////////////////////////
void CPlayerGame::copyDevCards()
{
	int i;

	//run through the arrays
	for(i = 0; i < 5; i++)
	{
		m_iCardHeld[i] += m_iCardBought[i];
		m_iCardBought[i] = 0;
	}
}

//////////////////////////////////////////////////////////////////////
// save or load this puppy
//////////////////////////////////////////////////////////////////////
void CPlayerGame::Serialize(CArchive &ar)
{
	//save information
	if(ar.IsStoring())
	{
		save(ar);
	}
	//load information
	else
	{
		load(ar);
	}
}

//////////////////////////////////////////////////////////////////////
// load data
//////////////////////////////////////////////////////////////////////
void CPlayerGame::load(CArchive &ar)
{
	int i;
	int iSize;
	int nDummy;
	COLORREF color;

	//read in the player
	m_player.Serialize(ar);

	//player color
	if(DATABASE->gameVer() < VER_NETWORK)
	{
		ar >> color;
		m_iColor = COLORS->getColorByCREF(color);
	}
	else
	{
		ar >> m_iColor;
	}
	
	//settlements
	ar >> iSize;

	//settlements
	m_iSettlements.resize(iSize);
	for(i = 0; i < iSize; i++)
	{
		ar >> m_iSettlements[i];
	}

	//cities
	ar >> iSize;

	//cities
	m_iCities.resize(iSize);
	for(i = 0; i < iSize; i++)
	{
		ar >> m_iCities[i];
	}

	//the number of points the player has
	ar >> m_iPoints;

	//current amounts of resources
	ar.Read(m_iRes, sizeof(m_iRes));

	//jungles
	if(DATABASE->gameVer() >= VER_JUNGLES)
	{
		ar >> m_nJungles;
	}

	//trade offers used
	if(DATABASE->gameVer() >= VER_113)
	{
		ar >> m_nTradeOffers;
	}
	
	//amounts of resources gained through various means
	ar.Read(m_iResBoard, sizeof(m_iResBoard));
	ar.Read(m_iResStolen, sizeof(m_iResStolen));
	ar.Read(m_iResDev, sizeof(m_iResDev));

	if(DATABASE->gameVer() >= VER_INTO_DESERT)
	{
		ar.Read(m_iResGold, sizeof(m_iResGold));
		ar.Read(m_iResDiscover, sizeof(m_iResDiscover));
	}
	
	//amounts of resources lost through various means
	ar.Read(m_iLostBoard, sizeof(m_iLostBoard));
	ar.Read(m_iLostStolen, sizeof(m_iLostStolen));
	ar.Read(m_iLostRoll, sizeof(m_iLostRoll));

	//the number of items in stock
	ar >> m_nStockRoads;
	ar >> m_nStockCities;
	ar >> m_nStockSettle;

	//ships
	if(DATABASE->gameVer() >= VER_SEAFARERS)
	{
		ar >> m_nStockShips;

		//bonuses
		ar >> m_bBonus1;
		ar >> m_bBonus2;
	}

	//home island
	if(DATABASE->gameVer() >= VER_GREATER_CATAN)
	{
		ar >> m_nHomeIsland;
		ar >> m_nTradePoints;
	}

	//the number of development cards in hand, playable, and played
	if(DATABASE->gameVer() < VER_SEAFARERS)
	{
		ar >> nDummy;		//RETIRED IN 1.0.1
		ar >> nDummy;		//RETIRED IN 1.0.1
	}
	
	//check file version
	if(DATABASE->gameVer() >= VER_GAMEMODE)
	{
		ar.Read(m_iCardBought, sizeof(m_iCardBought));
		ar.Read(m_iCardHeld, sizeof(m_iCardHeld));
	}

	ar.Read(m_iCardPlayed, sizeof(m_iCardPlayed));

	//has the player played a card on this turn?
	ar >> m_bPlayedCard;

	//check file version
	if(DATABASE->gameVer() >= VER_NETWORKb)
	{
		//has the player built this turn, yet?
		ar >> m_bPlacedBuild;
	}

	//whether they've moved a ship
	if(DATABASE->gameVer() >= VER_SEAFARERS)
	{
		ar >> m_bMovedShip;
	}

	//does this player have largest army?
	ar >> m_bLargestArmy;

	//road length
	if(DATABASE->gameVer() >= VER_ROADLENGTH)
	{
		ar >> m_iRoadLength;
	}

	//does this player have longest road?
	ar >> m_bLongestRoad;

	//number of trades with the bank
	ar.Read(m_iTradesBank, sizeof(m_iTradesBank));

	//number of trades with other people
	ar.Read(m_iTradesOthers, sizeof(m_iTradesOthers));

	//cards traded to the bank
	ar.Read(m_iBankTradeTo, sizeof(m_iBankTradeTo));
	ar.Read(m_iBankTradeFrom, sizeof(m_iBankTradeFrom));

	//cards traded to others
	ar.Read(m_iOtherTradeTo, sizeof(m_iOtherTradeTo));

	//cads received from others
	ar.Read(m_iOtherTradeFrom, sizeof(m_iOtherTradeFrom));

	//turn time
	ar >> iSize;

	//turn time
	m_iTurnTime.resize(iSize);
	for(i = 0; i < iSize; i++)
	{
		ar >> m_iTurnTime[i];
	}
}

//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CPlayerGame::save(CArchive &ar)
{
	int i;
	int iSize;

	//save the player
	m_player.Serialize(ar);

	//player color
	//ar << m_color;
	ar << m_iColor;

	//cities and settlements
	iSize = m_iSettlements.size();
	
	//settlements
	ar << iSize;
	for(i = 0; i < iSize; i++)
	{
		ar << m_iSettlements[i];
	}

	//cities and settlements
	iSize = m_iCities.size();

	//cities
	ar << iSize;
	for(i = 0; i < iSize; i++)
	{
		ar << m_iCities[i];
	}

	//the number of points the player has
	ar << m_iPoints;

	//current amounts of resources
	ar.Write(m_iRes, sizeof(m_iRes));

	//jungles
	ar << m_nJungles;

	//trade offers used
	ar << m_nTradeOffers;
	
	//amounts of resources gained through various means
	ar.Write(m_iResBoard, sizeof(m_iResBoard));
	ar.Write(m_iResStolen, sizeof(m_iResStolen));
	ar.Write(m_iResDev, sizeof(m_iResDev));
	ar.Write(m_iResGold, sizeof(m_iResGold));
	ar.Write(m_iResDiscover, sizeof(m_iResDiscover));
	
	//amounts of resources lost through various means
	ar.Write(m_iLostBoard, sizeof(m_iLostBoard));
	ar.Write(m_iLostStolen, sizeof(m_iLostStolen));
	ar.Write(m_iLostRoll, sizeof(m_iLostRoll));

	//the number of items in stock
	ar << m_nStockRoads;
	ar << m_nStockCities;
	ar << m_nStockSettle;
	ar << m_nStockShips;

	//bonuses
	ar << m_bBonus1;
	ar << m_bBonus2;

	//home island
	ar << m_nHomeIsland;
	ar << m_nTradePoints;

	//the number of development cards in hand, playable, and played
	ar.Write(m_iCardBought, sizeof(m_iCardBought));
	ar.Write(m_iCardHeld, sizeof(m_iCardHeld));
	ar.Write(m_iCardPlayed, sizeof(m_iCardPlayed));

	//has the player played a card on this turn?
	ar << m_bPlayedCard;

	//has the player build this turn yet?
	ar << m_bPlacedBuild;

	//whether they've moved a ship this turn
	ar << m_bMovedShip;

	//does this player have largest army?
	ar << m_bLargestArmy;

	//road length
	ar << m_iRoadLength;

	//does this player have longest road?
	ar << m_bLongestRoad;

	//number of trades with the bank
	ar.Write(m_iTradesBank, sizeof(m_iTradesBank));

	//number of trades with other people
	ar.Write(m_iTradesOthers, sizeof(m_iTradesOthers));

	//cards traded to the bank
	ar.Write(m_iBankTradeTo, sizeof(m_iBankTradeTo));
	ar.Write(m_iBankTradeFrom, sizeof(m_iBankTradeFrom));

	//cards traded to others
	ar.Write(m_iOtherTradeTo, sizeof(m_iOtherTradeTo));

	//cads received from others
	ar.Write(m_iOtherTradeFrom, sizeof(m_iOtherTradeFrom));

	//the number of seconds this player is taking per turn
	iSize = m_iTurnTime.size();

	//turn time
	ar << iSize;
	for(i = 0; i < iSize; i++)
	{
		ar << m_iTurnTime[i];
	}
}

//////////////////////////////////////////////////////////////////////
// write to a network message
//////////////////////////////////////////////////////////////////////
void CPlayerGame::SerializeToMessage(CNDKMessage &msg)
{
	int i;
	int iStart = 11;
	int iSize;

	//player
	m_player.SerializeToMessage(msg);

	//color
	msg.SetAt(iStart++, m_iColor);

	//settlements
	iSize = m_iSettlements.size();
	msg.SetAt(iStart++, iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.SetAt(iStart + i, m_iSettlements[i]);
	}
	iStart += iSize;

	//cities
	iSize = m_iCities.size();
	msg.SetAt(iStart++, iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.SetAt(iStart + i, m_iCities[i]);
	}
	iStart += iSize;

	//points
	msg.SetAt(iStart++, m_iPoints);

	//resources
	msg.SetAt(iStart++, m_iRes, sizeof(m_iRes));

	//jungles
	msg.SetAt(iStart++, m_nJungles);

	//trade offers used
	msg.SetAt(iStart++, m_nTradeOffers);

	//amounts of resources gained through various means
	msg.SetAt(iStart++, m_iResBoard, sizeof(m_iResBoard));
	msg.SetAt(iStart++, m_iResStolen, sizeof(m_iResStolen));
	msg.SetAt(iStart++, m_iResDev, sizeof(m_iResDev));
	msg.SetAt(iStart++, m_iResGold, sizeof(m_iResGold));
	msg.SetAt(iStart++, m_iResDiscover, sizeof(m_iResDiscover));

	//amounts of resources lost through various means
	msg.SetAt(iStart++, m_iLostBoard, sizeof(m_iLostBoard));
	msg.SetAt(iStart++, m_iLostStolen, sizeof(m_iLostStolen));
	msg.SetAt(iStart++, m_iLostRoll, sizeof(m_iLostRoll));

	//the number of items in stock
	msg.SetAt(iStart++, m_nStockRoads);
	msg.SetAt(iStart++, m_nStockCities);
	msg.SetAt(iStart++, m_nStockSettle);
	msg.SetAt(iStart++, m_nStockShips);

	//bonuses
	msg.SetAt(iStart++, m_bBonus1);
	msg.SetAt(iStart++, m_bBonus2);

	//home island
	msg.SetAt(iStart++, m_nHomeIsland);
	msg.SetAt(iStart++, m_nTradePoints);

	//the number of development cards in hand, playable, and played
	msg.SetAt(iStart++, m_iCardBought, sizeof(m_iCardBought));
	msg.SetAt(iStart++, m_iCardHeld, sizeof(m_iCardHeld));
	msg.SetAt(iStart++, m_iCardPlayed, sizeof(m_iCardPlayed));

	//has the player played a card on this turn?
	msg.SetAt(iStart++, m_bPlayedCard);

	//has the player placed a building on this turn?
	msg.SetAt(iStart++, m_bPlacedBuild);

	//has the player moved a ship this turn
	msg.SetAt(iStart++, m_bMovedShip);

	//does this player have largest army?
	msg.SetAt(iStart++, m_bLargestArmy);

	//road length
	msg.SetAt(iStart++, m_iRoadLength);

	//does this player have longest road?
	msg.SetAt(iStart++, m_bLongestRoad);

	//number of trades with the bank
	msg.SetAt(iStart++, m_iTradesBank, sizeof(m_iTradesBank));

	//number of trades with other people
	msg.SetAt(iStart++, m_iTradesOthers, sizeof(m_iTradesOthers));

	//cards traded to the bank
	msg.SetAt(iStart++, m_iBankTradeTo, sizeof(m_iBankTradeTo));
	msg.SetAt(iStart++, m_iBankTradeFrom, sizeof(m_iBankTradeFrom));

	//cards traded to others
	msg.SetAt(iStart++, m_iOtherTradeTo, sizeof(m_iOtherTradeTo));

	//cads received from others
	msg.SetAt(iStart++, m_iOtherTradeFrom, sizeof(m_iOtherTradeFrom));

	//the number of seconds this player is taking per turn
	iSize = m_iTurnTime.size();
	msg.SetAt(iStart++, iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.SetAt(iStart + i, m_iTurnTime[i]);
	}
}

//////////////////////////////////////////////////////////////////////
// read from a network message
//////////////////////////////////////////////////////////////////////
void CPlayerGame::SerializeFromMessage(CNDKMessage &msg)
{
	int i;
	int iSize;
	int iStart = 11;

	//player
	m_player.SerializeFromMessage(msg);

	//color
	msg.GetAt(iStart++, m_iColor);

	//settlements
	msg.GetAt(iStart++, iSize);
	m_iSettlements.resize(iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.GetAt(iStart + i, m_iSettlements[i]);
	}
	iStart += iSize;

	//cities
	msg.GetAt(iStart++, iSize);
	m_iCities.resize(iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.GetAt(iStart + i, m_iCities[i]);
	}
	iStart += iSize;

	//points
	msg.GetAt(iStart++, m_iPoints);

	//resources
	msg.GetAt(iStart++, m_iRes, sizeof(m_iRes));

	//jungles
	msg.GetAt(iStart++, m_nJungles);

	//trade offers used
	msg.GetAt(iStart++, m_nTradeOffers);

	//amounts of resources gained through various means
	msg.GetAt(iStart++, m_iResBoard, sizeof(m_iResBoard));
	msg.GetAt(iStart++, m_iResStolen, sizeof(m_iResStolen));
	msg.GetAt(iStart++, m_iResDev, sizeof(m_iResDev));
	msg.GetAt(iStart++, m_iResGold, sizeof(m_iResGold));
	msg.GetAt(iStart++, m_iResDiscover, sizeof(m_iResDiscover));

	//amounts of resources lost through various means
	msg.GetAt(iStart++, m_iLostBoard, sizeof(m_iLostBoard));
	msg.GetAt(iStart++, m_iLostStolen, sizeof(m_iLostStolen));
	msg.GetAt(iStart++, m_iLostRoll, sizeof(m_iLostRoll));

	//the number of items in stock
	msg.GetAt(iStart++, m_nStockRoads);
	msg.GetAt(iStart++, m_nStockCities);
	msg.GetAt(iStart++, m_nStockSettle);
	msg.GetAt(iStart++, m_nStockShips);

	//bonuses
	msg.GetAt(iStart++, m_bBonus1);
	msg.GetAt(iStart++, m_bBonus2);

	//home island
	msg.GetAt(iStart++, m_nHomeIsland);
	msg.GetAt(iStart++, m_nTradePoints);

	//the number of development cards in hand, playable, and played
	msg.GetAt(iStart++, m_iCardBought, sizeof(m_iCardBought));
	msg.GetAt(iStart++, m_iCardHeld, sizeof(m_iCardHeld));
	msg.GetAt(iStart++, m_iCardPlayed, sizeof(m_iCardPlayed));

	//has the player played a card on this turn?
	msg.GetAt(iStart++, m_bPlayedCard);

	//has the player placed a building on this turn?
	msg.GetAt(iStart++, m_bPlacedBuild);

	//has the player moved a ship this turn
	msg.GetAt(iStart++, m_bMovedShip);

	//does this player have largest army?
	msg.GetAt(iStart++, m_bLargestArmy);

	//road length
	msg.GetAt(iStart++, m_iRoadLength);

	//does this player have longest road?
	msg.GetAt(iStart++, m_bLongestRoad);

	//number of trades with the bank
	msg.GetAt(iStart++, m_iTradesBank, sizeof(m_iTradesBank));

	//number of trades with other people
	msg.GetAt(iStart++, m_iTradesOthers, sizeof(m_iTradesOthers));

	//cards traded to the bank
	msg.GetAt(iStart++, m_iBankTradeTo, sizeof(m_iBankTradeTo));
	msg.GetAt(iStart++, m_iBankTradeFrom, sizeof(m_iBankTradeFrom));

	//cards traded to others
	msg.GetAt(iStart++, m_iOtherTradeTo, sizeof(m_iOtherTradeTo));

	//cads received from others
	msg.GetAt(iStart++, m_iOtherTradeFrom, sizeof(m_iOtherTradeFrom));

	//the number of seconds this player is taking per turn
	msg.GetAt(iStart++, iSize);
	m_iTurnTime.resize(iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.GetAt(iStart + i, m_iTurnTime[i]);
	}
}

#endif //AI_DLL