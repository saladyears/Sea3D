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
#include "settlers.h"
#include "Variants.h"
#include "GameDatabase.h"
#include "defineVariants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CVariants, CObject, 1)

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CVariants::CVariants()
{
	//the bit string of variants on/off
	m_iVariants = 0;

	//variant settings
	m_iStockRoads = 0;
	m_iStockCities = 0;
	m_iStockSettlements = 0;
	m_iStockShips = 0;
	m_iAllowedCards = 0;
	m_iPoints = 0;

	//default
	m_nNoSevens = 1;
	m_nTradeOffers = 3;
}

//////////////////////////////////////////////////////////////////////
// copy constructor
//////////////////////////////////////////////////////////////////////
CVariants::CVariants(const CVariants &data)
{
	copy(data);
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CVariants::~CVariants()
{

}

//////////////////////////////////////////////////////////////////////
// assignment operator
//////////////////////////////////////////////////////////////////////
CVariants &CVariants::operator =(const CVariants &data)
{
	copy(data);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// streamline copy function
//////////////////////////////////////////////////////////////////////
void CVariants::copy(const CVariants &data)
{
	//the bit string of variants on/off
	m_iVariants				= data.m_iVariants;

	//variant settings
	m_iStockRoads			= data.m_iStockRoads;
	m_iStockCities			= data.m_iStockCities;
	m_iStockSettlements		= data.m_iStockSettlements;
	m_iStockShips			= data.m_iStockShips;
	m_iAllowedCards			= data.m_iAllowedCards;
	m_iPoints				= data.m_iPoints;

	//no sevens rounds
	m_nNoSevens				= data.m_nNoSevens;

	//trade offer limit
	m_nTradeOffers			= data.m_nTradeOffers;
}

//////////////////////////////////////////////////////////////////////
// save or load this puppy
//////////////////////////////////////////////////////////////////////
void CVariants::Serialize(CArchive &ar)
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
// return variant string
//////////////////////////////////////////////////////////////////////
CString CVariants::getVariantString()
{
	int i;
	int iCount = 0;
	CString str;
	CString strTemp;

	//start off
	str = "\n\nVariants:\n";

	//run through the bit string
	for(i = 0; i < 32; i++)
	{
		//skip those we don't have on
		if(FALSE == ((1 << i) & m_iVariants))
		{
			continue;
		}

		//increment count
		iCount++;

		//add text
		switch(1 << i)
		{
		case VARIANT_TRADE_ZERO: str += "-trades for zero cards allowed\n"; break;
		case VARIANT_EQUAL_ODDS: str += "-all dice rolls have equal odds\n"; break;
		case VARIANT_ALTER_ROADS: strTemp.Format("-stock roads: %d\n", m_iStockRoads); str += strTemp; break;
		case VARIANT_ALTER_CITIES: strTemp.Format("-stock cities: %d\n", m_iStockCities); str += strTemp; break;
		case VARIANT_ALTER_SETTLEMENTS: strTemp.Format("-stock settlements: %d\n", m_iStockSettlements); str += strTemp; break;
		case VARIANT_ALTER_SHIPS: strTemp.Format("-stock ships: %d\n", m_iStockShips); break;
		case VARIANT_ALTER_ALLOWED_CARDS: strTemp.Format("-allowed cards: %d\n", m_iAllowedCards); str += strTemp; break;
		case VARIANT_ALTER_POINTS: strTemp.Format("-points to win: %d\n", m_iPoints); str += strTemp; break;
		case VARIANT_TRADE_AFTER_BUILD: str += "-allow trading after building\n"; break;
		case VARIANT_NO_7_FIRST_ROUND: strTemp.Format("-no sevens for %d round%s\n", m_nNoSevens, (m_nNoSevens == 1) ? "" : "s"); str += strTemp; break;
		case VARIANT_SHOW_ALL_CARDS: str += "-all player's cards are visible\n"; break;
		case VARIANT_ROUND_LOST_UP: str += "-round lost cards up not down\n"; break;
		case VARIANT_HIDE_CHIPS: str += "-hide roll chips until after placements\n"; break;
		case VARIANT_USE_JUNGLE: str += "-replace deserts with jungles\n"; break;
		case VARIANT_USE_VOLCANO: str += "-replace deserts with volcanoes\n"; break;
		case VARIANT_USE_VOLCANO_GOLD: str += "-replace deserts with volcanoes (with gold)\n"; break;
		case VARIANT_SPECIAL_BUILD: str += "-special build phase for 5-6 players\n"; break;
		case VARIANT_TOURNAMENT_START: str += "-use Tournament starting rules\n"; break;
		case VARIANT_NO_TRADING: str += "-no trading allowed"; break;
		case VARIANT_LIMIT_TRADING: strTemp.Format("-trade offers per round: %d\n", m_nTradeOffers); str += strTemp; break;
		}
	}

	//if no count, no string
	if(0 == iCount)
	{
		str.Empty();
	}

	return str;
}

//////////////////////////////////////////////////////////////////////
// load data
//////////////////////////////////////////////////////////////////////
void CVariants::load(CArchive &ar)
{
	//the bit string of variants on/off
	ar >> m_iVariants;

	//variant settings
	ar >> m_iStockRoads;
	ar >> m_iStockCities;
	ar >> m_iStockSettlements;

	//check for seafarers
	if(DATABASE->gameVer() >= VER_SEAFARERS)
	{
		ar >> m_iStockShips;
	}

	ar >> m_iAllowedCards;
	ar >> m_iPoints;

	//check for no sevens
	if(DATABASE->gameVer() >= VER_113)
	{
		ar >> m_nNoSevens;
		ar >> m_nTradeOffers;
	}
}

//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CVariants::save(CArchive &ar)
{
	//the bit string of variants on/off
	ar << m_iVariants;

	//variant settings
	ar << m_iStockRoads;
	ar << m_iStockCities;
	ar << m_iStockSettlements;
	ar << m_iStockShips;
	ar << m_iAllowedCards;
	ar << m_iPoints;

	//no sevens
	ar << m_nNoSevens;
	ar << m_nTradeOffers;
}

//////////////////////////////////////////////////////////////////////
// network write
//////////////////////////////////////////////////////////////////////
void CVariants::SerializeToMessage(int &iStart, CNDKMessage &msg)
{
	//bit string
	msg.SetAt(iStart++, m_iVariants);

	//variant settings
	msg.SetAt(iStart++, m_iStockRoads);
	msg.SetAt(iStart++, m_iStockCities);
	msg.SetAt(iStart++, m_iStockSettlements);
	msg.SetAt(iStart++, m_iStockShips);
	msg.SetAt(iStart++, m_iAllowedCards);
	msg.SetAt(iStart++, m_iPoints);

	//no sevens
	msg.SetAt(iStart++, m_nNoSevens);

	//trade offers
	msg.SetAt(iStart++, m_nTradeOffers);
}

//////////////////////////////////////////////////////////////////////
// network read
//////////////////////////////////////////////////////////////////////
void CVariants::SerializeFromMessage(int &iStart, CNDKMessage &msg)
{
	//bit string
	msg.GetAt(iStart++, m_iVariants);

	//variant settings
	msg.GetAt(iStart++, m_iStockRoads);
	msg.GetAt(iStart++, m_iStockCities);
	msg.GetAt(iStart++, m_iStockSettlements);
	msg.GetAt(iStart++, m_iStockShips);
	msg.GetAt(iStart++, m_iAllowedCards);
	msg.GetAt(iStart++, m_iPoints);

	//no sevens
	msg.GetAt(iStart++, m_nNoSevens);

	//trade offers
	msg.GetAt(iStart++, m_nTradeOffers);
}