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
#include "GLView.h"
#include "SettlersView.h"
#include "defineSounds.h"
#include "defineCode.h"
#include "defineVariants.h"

/////////////////////////////////////////////////////////////////////////////
// look for longest road
/////////////////////////////////////////////////////////////////////////////
int CGLView::longestRoad(int iPlayer)
{
	int i, j;
	int iOwner;
	BOOL bRoad = FALSE;
	BOOL bShip;

	//see if anyone else has longest road
	iOwner = longestPlayer();

	//search each tile and side for a road that belongs to this player
	for(i = 0; i < m_nTiles; i++)
	{
		for(j = 0; j < 6; j++)
		{
			//skip already done sides
			if(m_pBoard[i].getSide(j) < i) continue;

			//set the ship variable
			bShip = (m_pBoard[i].getShip(j) == iPlayer);

			//see if this tile and corner has a road of this player's
			if((m_pBoard[i].getRoad(j) == iPlayer) || (TRUE == bShip))
			{
				if(longestIterateStart(i, j, iPlayer, iOwner, bShip))
				{
					bRoad = TRUE;
				}
			}
		}
	}

	//they don't have it
	if(!bRoad)
	{
		return -3;
	}

	//see if the person already has longest road
	if(iOwner == iPlayer)
	{
		//already has longest road
		return -2;
	}

	//play sound
	SOUND(SOUND_LONGEST_ROAD);

	//see if they're the first one to get it
	if(iOwner == -1)
	{
		return -1;
	}

	//otherwise, they stole it
	return iOwner;
}

/////////////////////////////////////////////////////////////////////////////
// start road iterations
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::longestIterateStart(int iTile, int iSide, int iPlayer, int iOwner, BOOL bShip)
{
	int iLength = 1;
	int iPlus;
	int iMinus;
	int iTile1;
	int iCity;
	int iBranch1, iBranch2;
	BOOL b1, b2, b3, b4;

	//default values
	b1 = b2 = b3 = b4 = FALSE;

	//clear the temp array
	m_iTempRoad.clear();
	m_iTempRoad.resize(0);

	//set the plus and minus side
	iPlus = ((iSide + 1) % 6);
	iMinus = ((iSide + 5) % 6);

	//get the branched sides
	iBranch1 = ((iSide + 2) % 6);
	iBranch2 = ((iSide + 4) % 6);

	//get the connecting side
	iTile1 = m_pBoard[iTile].getSide(iSide);

	//this side is being used
	m_pBoard[iTile].setUsed(iSide, TRUE);
	m_pBoard[iTile1].setUsed((iSide + 3) % 6, TRUE);

	//add this first side, always give it the lower side
	if(iTile < iTile1)
	{
		m_iTempRoad.push_back(ENCODE_SEL(iTile, iSide));
	}
	else
	{
		m_iTempRoad.push_back(ENCODE_SEL(iTile1, iSide));
	}

	//now brute force down each connecting path
	iCity = m_pBoard[iTile].getCity(iPlus);

	//check for a blocking city
	if(iCity == 0 || (iCity % 10) == iPlayer)
	{
		//same tile, plus side
		b1 = longestIterateNext(iLength, iTile1, iTile, iPlus, iPlayer, iOwner, bShip);

		//branched tile from the plus side
		b3 = longestIterateNext(iLength, iTile, iTile1, iBranch1, iPlayer, iOwner, bShip);
	}

	//try minus side
	iCity = m_pBoard[iTile].getCity(iSide);

	//check results
	if(iCity == 0 || (iCity % 10) == iPlayer)
	{
		//same tile, minus side
		b2 = longestIterateNext(iLength, iTile1, iTile, iMinus, iPlayer, iOwner, bShip);

		//branched tile from the minus side
		b4 = longestIterateNext(iLength, iTile, iTile1, iBranch2, iPlayer, iOwner, bShip);
	}

	//this side is not being used
	m_pBoard[iTile].setUsed(iSide, FALSE);
	m_pBoard[iTile1].setUsed((iSide + 3) % 6, FALSE);

	return (b1 || b2 || b3 || b4);
}

/////////////////////////////////////////////////////////////////////////////
// mmm... tasty recursion...
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::longestIterateNext(int &iLength, int iAvoid, int iTile, int iSide, int iPlayer, int iOwner, BOOL bShip)
{
	int iCity;
	int iPlus, iMinus;
	int iTileAvoid;
	int iTilePlus, iTileMinus;
	int iBranch;
	int iOppSide;
	BOOL bRet = FALSE;
	BOOL bIsShip;
	BOOL bIsRoad;

	//see if there is a ship or a road here
	bIsShip = (m_pBoard[iTile].getShip(iSide) == iPlayer);
	bIsRoad = (m_pBoard[iTile].getRoad(iSide) == iPlayer);

	//immediate check to see if this road or ship belongs to this player
	if( (FALSE == bIsRoad) && (FALSE == bIsShip))
	{
		return FALSE;
	}

	//if we are a road, and what came in was a ship, or vice versa, then
	//we can only continue if one of our settlements/cities is between us
	if(((TRUE == bIsRoad) && (TRUE == bShip)) ||
		((TRUE == bIsShip) && (FALSE == bShip)))
	{
		//calculate which side has the avoid tile that we came from
		if(iAvoid == m_pBoard[iTile].getSide((iSide + 1) % 6))
		{
			iOppSide = (iSide + 1) % 6;
		}
		else
		{
			iOppSide = iSide;
		}

		//check for blocking city
		iCity = m_pBoard[iTile].getCity(iOppSide);

		if(iCity == 0 || (iCity % 10) != iPlayer)
		{
			return FALSE;
		}
	}

	//see if this side is in use by the algorithm
	if(m_pBoard[iTile].getUsed(iSide)) return FALSE;

	//set the plus and minus side
	iPlus = ((iSide + 1) % 6);
	iMinus = ((iSide + 5) % 6);
	iOppSide = ((iSide + 3) % 6);

	//get the plus and minus tiles
	iTileAvoid = m_pBoard[iTile].getSide(iSide);
	iTilePlus = m_pBoard[iTile].getSide(iPlus);
	iTileMinus = m_pBoard[iTile].getSide(iMinus);

	//this side is being used
	m_pBoard[iTile].setUsed(iSide, TRUE);
	m_pBoard[iTileAvoid].setUsed(iOppSide, TRUE);

	//add this position to the temp array, always give it the lower tile
	if(iTile < iTileAvoid)
	{
		m_iTempRoad.push_back(ENCODE_SEL(iTile, iSide));
	}
	else
	{
		m_iTempRoad.push_back(ENCODE_SEL(iTileAvoid, iOppSide));
	}

	//increment the length
	iLength++;

	//now traverse down the side that we are not trying to avoid
	if(iTileMinus != iAvoid)
	{
		//check for blocking city
		iCity = m_pBoard[iTile].getCity(iSide);

		//see if we a city is blocking
		if(iCity == 0 || (iCity % 10) == iPlayer)
		{
			//set the branch
			iBranch = ((iSide + 1) % 6);

			//iterate same tile
			if(longestIterateNext(iLength, iTileAvoid, iTile, iMinus, iPlayer, iOwner, bIsShip)) 
			{
				bRet = TRUE;
			}

			//iterate branch tile
			if(longestIterateNext(iLength, iTile, iTileMinus, iBranch, iPlayer, iOwner, bIsShip))
			{
				bRet = TRUE;
			}
		}
	}
	else
	{
		//check for blocking city
		iCity = m_pBoard[iTile].getCity(iPlus);

		//see if we a city is blocking
		if(iCity == 0 || (iCity % 10) == iPlayer)
		{
			//set the branch
			iBranch = ((iSide + 5) % 6);

			//iterate same tile
			if(longestIterateNext(iLength, iTileAvoid, iTile, iPlus, iPlayer, iOwner, bIsShip))
			{
				bRet = TRUE;
			}

			//iterate branch tile
			if(longestIterateNext(iLength, iTile, iTilePlus, iBranch, iPlayer, iOwner, bIsShip))
			{
				bRet = TRUE;
			}
		}
	}

	//determine how long their road is
	if(iLength > m_pGame->m_players[iPlayer].m_iRoadLength)
	{
		m_pGame->m_players[iPlayer].m_iRoadLength = iLength;
	}

	//increment the length and check
	if( ((iLength > m_iLongestLength) && (iLength >= 5)) ||
		((iLength >= m_iLongestLength) && (iLength >= 5) && (iPlayer == iOwner)))
	{
		//copy the temp array to the real array
		m_iLongestRoad = m_iTempRoad;

		//set the longest length
		m_iLongestLength = iLength;

		//we have longest road
		bRet = TRUE;
	}

	//this side is not being used anymore
	m_pBoard[iTile].setUsed(iSide, FALSE);
	m_pBoard[iTileAvoid].setUsed(iOppSide, FALSE);

	//pop this position from the temp array
	m_iTempRoad.pop_back();

	//subtract one from the length
	iLength--;

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// calculate any extra points gained for this player by building on this tile
/////////////////////////////////////////////////////////////////////////////
int CGLView::calculateExtraPoints(int iTile, int iCorner, int iPlayer)
{
	int iT2, iT3;
	int iC2, iC3;
	int iID1, iID2, iID3;
	BOOL bHasCities;

	//first see if this tile is even extrable
	if( (FALSE == m_pBoard[iTile].getSecondary()) || 
		(0 == MAP->m_nBonus1) ||
		(TRUE == m_pGame->m_players[iPlayer].m_bBonus2))
	{
		return 0;
	}

	//see if they already have the bonus and if so, is there a second bonus?
	if( (TRUE == m_pGame->m_players[iPlayer].m_bBonus1) &&
		(0 == MAP->m_nBonus2))
	{
		return 0;
	}

	//calculate the 3 IDs that are safe
	iT2 = m_pBoard[iTile].getSide(iCorner);
	iT3 = m_pBoard[iTile].getSide((iCorner + 5) % 6);
	iC2 = (iCorner + 4) % 6;
	iC3 = (iCorner + 2) % 6;
	iID1 = ENCODE_SEL(iTile, iCorner);
	iID2 = ENCODE_SEL(iT2, iC2);
	iID3 = ENCODE_SEL(iT3, iC3);

	//if they've made it here, it means that this tile is potentially an extra,
	//and they haven't gotten the bonus yet (either the first or second), therefore
	//we iterate through the tiles that touch this tile to see if this player
	//has already built any settlements/cities on... if we get all the way through
	//without finding any, they will receive the bonus points
	bHasCities = extraIterate(iTile, iPlayer, iID1, iID2, iID3);

	//if valid, calculate points
	if(FALSE == bHasCities)
	{
		//check for the first bonus
		if(FALSE == m_pGame->m_players[iPlayer].m_bBonus1)
		{
			return MAP->m_nBonus1;
		}
		//check for the second bonus
		else if(FALSE == m_pGame->m_players[iPlayer].m_bBonus2)
		{
			return MAP->m_nBonus2;
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// iterate through tiles to check for existing settlements
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::extraIterate(int iTile, int iPlayer, int iID1, int iID2, int iID3)
{
	int i;
	int iCity;
	int ID;
	int nType;
	BOOL bVolcano, bJungle;
	BOOL bDesertBuffer;

	//see if this side is already in use by the algorithm
	if(m_pBoard[iTile].getUsed(0)) 
	{
		return FALSE;
	}

	//see if the volcano variant is turned on
	bVolcano = (IS_VARIANT(VARIANT_USE_VOLCANO) || IS_VARIANT(VARIANT_USE_VOLCANO_GOLD)) ? TRUE : FALSE;

	//see if the jungle variant is on
	bJungle = (IS_VARIANT(VARIANT_USE_JUNGLE)) ? TRUE : FALSE;

	//get tile type
	nType = m_pBoard[iTile].getType();

	//determine if this is a desert buffer tile
	bDesertBuffer = ((TRUE == MAP->m_bDesertBuffer) && 
					//deserts work as buffers with this on
					((RES_DESERT == nType) || 
					//check for volcanoes
					((RES_VOLCANO == nType) && (TRUE == bVolcano)) ||
					//check for jungles
					((RES_JUNGLE == nType) && (TRUE == bJungle)))
					);

	//check for water tile, or a desert tile if the map uses the deserts as buffers
	if((TRUE == VALID_OCEAN(nType)) || (TRUE == bDesertBuffer))
	{
		return FALSE;
	}

	//set this tile to being used
	m_pBoard[iTile].setUsed(0, TRUE);

	//look at each side
	for(i = 0; i < 6; i++)
	{
		//check for a city
		iCity = m_pBoard[iTile].getCity(i);

		//calculate the ID
		ID = ENCODE_SEL(iTile, i);
		
		//examine for a city
		if( (0 != iCity) && 
			((iCity % 10) == iPlayer) &&
			((iID1 != ID) && (iID2 != ID) && (iID3 != ID)))
		{
			//clear this tile from being used
			m_pBoard[iTile].setUsed(0, FALSE);

			return TRUE;
		}
	}

	//iterate each existing side
	for(i = 0; i < 6; i++)
	{
		if(TRUE == extraIterate(m_pBoard[iTile].getSide(i), iPlayer, iID1, iID2, iID3))
		{
			//clear this tile from being used
			m_pBoard[iTile].setUsed(0, FALSE);

			return TRUE;
		}
	}

	//clear this tile from being used
	m_pBoard[iTile].setUsed(0, FALSE);

	//return what we got
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// find the ports and store them
/////////////////////////////////////////////////////////////////////////////
void CGLView::findPorts()
{
	int i;
	int iTile, iType;
	int iSide = 0;

	//clear current port data
	m_iPorts.RemoveAll();

	//run through the list
	for(i = 0; i < m_nTiles; i++)
	{
		//set the ports
		switch(m_pBoard[i].getType())
		{
		case RES_PORT3:			m_iPorts.Add((RES_PORT3 << 16) + i); break;
		case RES_PORTTIMBER:	m_iPorts.Add((RES_PORTTIMBER << 16) + i); break;
		case RES_PORTWHEAT:		m_iPorts.Add((RES_PORTWHEAT << 16) + i); break;
		case RES_PORTORE:		m_iPorts.Add((RES_PORTORE << 16) + i); break;
		case RES_PORTCLAY:		m_iPorts.Add((RES_PORTCLAY << 16) + i); break;
		case RES_PORTSHEEP:		m_iPorts.Add((RES_PORTSHEEP << 16) + i); break;
		default: break;
		}
	}

	//now that we've got ports, we need to rotate them and set the board port
	//corners correctly
	for(i = 0; i < m_iPorts.GetSize(); i++)
	{
		//get this port tile
		iType = m_iPorts[i] >> 16;
		iTile = m_iPorts[i] - (iType << 16);

		//now calculate which side the tile should face, if it does not have
		//a permanent setting
		if(-1 != m_pBoard[iTile].getPortSide())
		{
			iSide = m_pBoard[iTile].getPortSide();
		}
		else
		{
			iSide = portIterateStart(iTile);
		}

		//set the port values
		m_pBoard[iTile].setPort1(iSide);
		m_pBoard[iTile].setPort2((iSide + 1) % 6);

		//set the rotation
		m_pBoard[iTile].setRotation(((iSide + 4) % 6) * 60.);
	}
}

/////////////////////////////////////////////////////////////////////////////
// determine which way a port should face
/////////////////////////////////////////////////////////////////////////////
int CGLView::portIterateStart(int iTile)
{
	int i;
	int iTemp;
	int iLength = 0;
	int iCurLength = 0;
	int iSide = 0;
	int iNextTile;

	//run through each side and determine if we can iterate down it
	for(i = 0; i < 6; i++)
	{
		//get the next tile
		iNextTile = m_pBoard[iTile].getSide(i);

		//sanity
		if(-1 == iNextTile)
		{
			continue;
		}

		iTemp = portIterateNext(iNextTile, i, iLength);

		//set the side if needed
		if(iTemp > iCurLength)
		{
			iSide = i;
			iCurLength = iTemp;
		}
	}

	return iSide;
}

/////////////////////////////////////////////////////////////////////////////
// keep going
/////////////////////////////////////////////////////////////////////////////
int CGLView::portIterateNext(int iTile, int iSide, int &iLength)
{
	int iCurLength;
	int iNextTile;

	//if this is an ocean, stop counting
	if(TRUE == VALID_OCEAN(m_pBoard[iTile].getType()))
	{
		return iLength;
	}

	//get the next tile
	iNextTile = m_pBoard[iTile].getSide(iSide);

	//sanity
	if(-1 == iNextTile)
	{
		return iLength;
	}

	//increment the length for getting to a new non-ocean tile
	iLength++;

	//keep going down this path
	iCurLength = portIterateNext(iNextTile, iSide, iLength);

	//move back off this tile
	iLength--;

	//return the longest we got
	return iCurLength;
}

/////////////////////////////////////////////////////////////////////////////
// see if a ship loop closes in on the starting city
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::shipIterate(int nCity, int nTile, int nSide, int nAvoid)
{
	int nBranch;
	int nTileAvoid;
	int nTilePlus, nTileMinus;
	int nSidePlus, nSideMinus, nSideOpp;
	BOOL bRet = FALSE;

	//first see if there's even a ship for this player here
	if(m_pBoard[nTile].getShip(nSide) != m_iPlayer)
	{
		return FALSE;
	}

	//see if this side is in use by the algorithm
	if(TRUE == m_pBoard[nTile].getUsed(nSide)) 
	{
		return FALSE;
	}

	//get plus and minus sides for this tile
	nSidePlus = ((nSide + 1) % 6);
	nSideMinus = ((nSide + 5) % 6);
	nSideOpp = ((nSide + 3) % 6);

	//get plus and minus facing tiles
	nTilePlus = m_pBoard[nTile].getSide(nSidePlus);
	nTileMinus = m_pBoard[nTile].getSide(nSideMinus);
	nTileAvoid = m_pBoard[nTile].getSide(nSide);

	//now see if we've returned to the city
	if(nAvoid == nTileMinus)
	{
		if(ENCODE_SEL(nTile, ((nSide + 1) % 6)) == nCity)
		{
			return TRUE;
		}
		else if(ENCODE_SEL(nTileAvoid, ((nSide + 3) % 6)) == nCity)
		{
			return TRUE;
		}
		else if(ENCODE_SEL(nTilePlus, ((nSide + 5) % 6)) == nCity)
		{
			return TRUE;
		}
	}
	else
	{
		if(ENCODE_SEL(nTile, nSide) == nCity)
		{
			return TRUE;
		}
		else if(ENCODE_SEL(nTileAvoid, ((nSide + 4) % 6)) == nCity)
		{
			return TRUE;
		}
		else if(ENCODE_SEL(nTileMinus, ((nSide + 2) % 6)) == nCity)
		{
			return TRUE;
		}
	}

	//this side is being used
	m_pBoard[nTile].setUsed(nSide, TRUE);
	m_pBoard[nTileAvoid].setUsed(nSideOpp, TRUE);

	//iterate to the next tile
	//now traverse down the side that we are not trying to avoid
	if(nTileMinus != nAvoid)
	{
		//set the branch
		nBranch = ((nSide + 1) % 6);

		//iterate same tile
		if(TRUE == shipIterate(nCity, nTile, nSideMinus, nTileAvoid))
		{
			bRet = TRUE;
		}

		//iterate branch tile
		if(TRUE == shipIterate(nCity, nTileMinus, nBranch, nTile))
		{
			bRet = TRUE;
		}
	}
	else
	{
		//set the branch
		nBranch = ((nSide + 5) % 6);

		//iterate same tile
		if(TRUE == shipIterate(nCity, nTile, nSidePlus, nTileAvoid))
		{
			bRet = TRUE;
		}

		//iterate branch tile
		if(TRUE == shipIterate(nCity, nTilePlus, nBranch, nTile))
		{
			bRet = TRUE;
		}
	}

	//this side is not being used anymore
	m_pBoard[nTile].setUsed(nSide, FALSE);
	m_pBoard[nTileAvoid].setUsed(nSideOpp, FALSE);

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// calculate trade routes
/////////////////////////////////////////////////////////////////////////////
void CGLView::tradeRoutes()
{
	int i, j;
	int nTile2, nTile3;
	int nC2, nC3;
	int nCity;
	int nPoints;

	//reset all players trade route points counts
	for(i = 0; i < m_iNumPlayers; i++)
	{
		decPoints(i, m_pGame->m_players[i].m_nTradePoints, TRUE);
		m_pGame->m_players[i].m_nTradePoints = 0;
	}

	//look for trade routes from each city
	for(i = 0; i < m_nTiles; i++)
	{
		//look at each side
		for(j = 0; j < 6; j++)
		{
			//get the bordering tiles
			nTile2 = m_pBoard[i].getSide(j);
			nTile3 = m_pBoard[i].getSide((j + 5) % 6);
			nC2 = (j + 4) % 6;
			nC3 = (j + 2) % 6;

			//avoid multiple runs through the list by only using the lowest
			//tile of any combination of three
			if((nTile2 < i) || (nTile3 < i))
			{
				continue;
			}

			//see if there is a city/settlement here
			nCity = m_pBoard[i].getCity(j);

			//if zero, continue
			if(0 == nCity)
			{
				continue;
			}

			//determine the starting island
			if(TRUE == VALID_LAND(m_pBoard[i].getType()))
			{
				m_nTradeIsland = m_pBoard[i].getIsland();
			}
			else if((-1 != nTile2) && (TRUE == VALID_LAND(m_pBoard[nTile2].getType())))
			{
				m_nTradeIsland = m_pBoard[nTile2].getIsland();
			}
			else if(-1 != nTile3)
			{
				m_nTradeIsland = m_pBoard[nTile3].getIsland();
			}

			//set values for traversing
			m_nTradePlayer = nCity % 10;

			//the originating city must be on the starting island of the player
			if(m_nTradeIsland != m_pGame->m_players[m_nTradePlayer].m_nHomeIsland)
			{
				continue;
			}

			//run the algorithm
			nPoints = tradeIterateStart(i, j);

			//set the extra chit markers
			m_pBoard[i].setExtra(j, nPoints);
			m_pBoard[nTile2].setExtra(nC2, nPoints);
			m_pBoard[nTile3].setExtra(nC3, nPoints);

			//set the players point total
			m_pGame->m_players[m_nTradePlayer].m_nTradePoints += nPoints;
		}
	}

	//finally, reincrement all the trade route points
	for(i = 0; i < m_iNumPlayers; i++)
	{
		incPoints(i, m_pGame->m_players[i].m_nTradePoints);
	}
}

/////////////////////////////////////////////////////////////////////////////
// recursion to find trade routes
/////////////////////////////////////////////////////////////////////////////
int CGLView::tradeIterateStart(int nTile, int nSide)
{
	int nPoints = 0;
	int nTile2, nTile3;
	int nOpponent = -1;
	int nPlayerCount = 0;
	int nOpponentCount = 0;
	int nTurn;
	BOOL bClosed = FALSE;

	//get the opposite tile
	nTile2 = m_pBoard[nTile].getSide(nSide);
	nTile3 = m_pBoard[nTile].getSide((nSide + 5) % 6);

	//try each of the three sides, we can only start from a ship

	//same tile, city side
	if(m_pBoard[nTile].getShip(nSide) == m_nTradePlayer)
	{
		nTurn = m_pBoard[nTile].getTurnPlacement(nSide);
		nPoints += tradeIterateNext(nTile, nSide, nTile3, nPlayerCount, nOpponentCount, nOpponent, bClosed, nTurn);
	}

	//same tile, trailing side
	if(m_pBoard[nTile].getShip((nSide + 5) % 6) == m_nTradePlayer)
	{
		nTurn = m_pBoard[nTile].getTurnPlacement((nSide + 5) % 6);
		nPoints += tradeIterateNext(nTile, (nSide + 5) % 6, nTile2, nPlayerCount, nOpponentCount, nOpponent, bClosed, nTurn);
	}

	//opposite tile
	if((-1 != nTile2) && (m_pBoard[nTile2].getShip((nSide + 4) % 6) == m_nTradePlayer))
	{
		nTurn = m_pBoard[nTile2].getTurnPlacement((nSide + 4) % 6);
		nPoints += tradeIterateNext(nTile2, (nSide + 4) % 6, nTile, nPlayerCount, nOpponentCount, nOpponent, bClosed, nTurn);
	}

	return nPoints;
}

/////////////////////////////////////////////////////////////////////////////
// recursion to find trade routes
/////////////////////////////////////////////////////////////////////////////
int CGLView::tradeIterateNext(int nTile, int nSide, int nTileAvoid,	int nPlayerCount, 
						  int nOpponentCount, int nTradeOpponent, BOOL bPlayerClosed, int nLastTurn)
{
	int nShip;
	int nRoad;
	int nCity;
	int nThisPiece = -1;
	int nThisTurn;
	int nOppSide;
	int nOppTile;
	int nTile2;
	int nTile3;
	int nCitySide;
	int nTileMinus;
	int nBranch;
	int nIsland = 0;
	int nMinus, nPlus;
	int nPoints = 0;
	BOOL bShip = FALSE;

	//sanity
	if(-1 == nTile)
	{
		return nPoints;
	}

	//get the possible ship and road at this location
	nShip = m_pBoard[nTile].getShip(nSide);
	nRoad = m_pBoard[nTile].getRoad(nSide);

	//set the piece variable
	if(-1 != nShip)
	{
		nThisPiece = nShip;
		bShip = TRUE;
	}
	else if(-1 != nRoad)
	{
		nThisPiece = nRoad;
	}

	//if no road or ship, no need to continue
	if(-1 == nThisPiece)
	{
		return nPoints;
	}

	//if it is a road, it must not belong to the player
	if((FALSE == bShip) && (nThisPiece == m_nTradePlayer))
	{
		return nPoints;
	}

	//see if this side is in use by the algorithm
	if(m_pBoard[nTile].getUsed(nSide)) 
	{
		return nPoints;
	}

	//now ensure that if we are traversing opponent pieces, this piece is an 
	//opponents
	if((-1 != nTradeOpponent) && (nTradeOpponent != nThisPiece))
	{
		return nPoints;
	}

	//determine when this piece was built
	nThisTurn = m_pBoard[nTile].getTurnPlacement(nSide);

	//see if we are now an opponent's piece
	if(nThisPiece != m_nTradePlayer)
	{
		//increment the opponent's count
		nOpponentCount++;

		if(-1 == nTradeOpponent)
		{
			//set the new opponent
			nTradeOpponent = nThisPiece;

			//see if the player closed it
			if(nLastTurn > nThisTurn)
			{
				bPlayerClosed = TRUE;
			}
		}
	}
	else
	{
		//increment the player's count of pieces
		nPlayerCount++;
	}

	//set minus and plus sides
	nMinus = (nSide + 5) % 6;
	nPlus = (nSide + 1) % 6;

	//get the opposite side and tile
	nOppTile = m_pBoard[nTile].getSide(nSide);
	nOppSide = (nSide + 3) % 6;

	//calculate which side has the avoid tile that we came from
	if(nTileAvoid == m_pBoard[nTile].getSide((nSide + 1) % 6))
	{
		nCitySide = nSide;
	}
	else
	{
		nCitySide = (nSide + 1) % 6;
	}

	//get tiles around city
	nTile2 = m_pBoard[nTile].getSide(nCitySide);
	nTile3 = m_pBoard[nTile].getSide((nCitySide + 5) % 6);

	//check for an ending city
	nCity = m_pBoard[nTile].getCity(nCitySide);

	//get the island
	if(TRUE == VALID_LAND(m_pBoard[nTile].getType()))
	{
		nIsland = m_pBoard[nTile].getIsland();
	}
	else if((-1 != nTile2) && (TRUE == VALID_LAND(m_pBoard[nTile2].getType())))
	{
		nIsland = m_pBoard[nTile2].getIsland();
	}
	else if(-1 != nTile3)
	{
		nIsland = m_pBoard[nTile3].getIsland();
	}

	//see if a city exists here and it is on the opposite island
	if(0 != nCity)
	{
		if(nIsland != m_nTradeIsland)
		{
			//we are not branching down an opponent's path
			if(-1 == nTradeOpponent)
			{
				//if the city exists, it is a valid end point, regardless of
				//whether the player owns it or anyone else, as long as it is on
				//another island
				nPoints++;
			}
			else if((nCity % 10) == nTradeOpponent)
			{
				//points will only be awarded if the city is the opponent's and
				//the path to the city has more of the player's pieces than the
				//opponent's... if they are tied, it goes to whoever played
				//the closing piece
				if(nPlayerCount > nOpponentCount)
				{
					nPoints++;
				}
				else if((nPlayerCount == nOpponentCount) && 
						(TRUE == bPlayerClosed))
				{
					nPoints++;
				}
			}
		}
		
		return nPoints;
	}

	//since there's no city, continue recursion

	//this side is being used
	m_pBoard[nTile].setUsed(nSide, TRUE);
	if(-1 != nOppTile)
	{
		m_pBoard[nOppTile].setUsed(nOppSide, TRUE);
	}

	//get the minus side
	nTileMinus = m_pBoard[nTile].getSide((nSide + 5) % 6);

	//now traverse down the side that we are not trying to avoid
	if(nTileMinus == nTileAvoid)
	{
		//set the branch
		nBranch = (nSide + 2) % 6;

		//same tile, plus side
		nPoints += tradeIterateNext(nTile, nPlus, nOppTile, nPlayerCount, nOpponentCount, nTradeOpponent, bPlayerClosed, nThisTurn);

		//opposite tile, branched side
		nPoints += tradeIterateNext(nOppTile, nBranch, nTile, nPlayerCount, nOpponentCount, nTradeOpponent, bPlayerClosed, nThisTurn);
	}
	else
	{
		//set the branch
		nBranch = (nSide + 4) % 6;

		//same tile, minus side
		nPoints += tradeIterateNext(nTile, nMinus, nOppTile, nPlayerCount, nOpponentCount, nTradeOpponent, bPlayerClosed, nThisTurn);

		//opposite tile, branched side
		nPoints += tradeIterateNext(nOppTile, nBranch, nTile, nPlayerCount, nOpponentCount, nTradeOpponent, bPlayerClosed, nThisTurn);
	}

	//this side is no longer being used
	m_pBoard[nTile].setUsed(nSide, FALSE);
	if(-1 != nOppTile)
	{
		m_pBoard[nOppTile].setUsed(nOppSide, FALSE);
	}

	return nPoints;
}