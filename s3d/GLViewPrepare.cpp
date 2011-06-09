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
#include "defineMap.h"
#include "defineCode.h"

/////////////////////////////////////////////////////////////////////////////
// prepare initial road placements
/////////////////////////////////////////////////////////////////////////////
void CGLView::prepareInitialRoads()
{
	int iMinus;
	int iTile1, iTile2, iTile3;
	int iCorner, iCorner2, iCorner3;
	BOOL b12, b13, b23;

	//deconstruct the last built city
	DECODE_SEL(m_iLastCity, iTile1, iCorner);

	//set the trailing value
	iMinus = (iCorner + 5) % 6;

	//get the nearest tiles
	iTile2 = m_pBoard[iTile1].getSide(iCorner);
	iTile3 = m_pBoard[iTile1].getSide(iMinus);

	//get the possible locations
	invalidPlaces(FALSE, iTile1, iCorner, b12, b13, b23);

	//reset the road places
	m_iPossibleRoads.resize(0);
	m_iPossibleRoads.clear();
	m_iPossibleShips.resize(0);
	m_iPossibleShips.clear();

	//set the alternate corners
	iCorner2 = ((iCorner + 3) % 6);
	iCorner3 = ((iMinus + 3) % 6);

	//tile 1 and tile 2
	if(TRUE == b12)
	{
		m_iPossibleRoads.push_back(ENCODE_SEL(iTile1, iCorner));
		m_iPossibleRoads.push_back(ENCODE_SEL(iTile2, iCorner2));
	}
	
	//tile 1 and tile 3
	if(TRUE == b13)
	{
		m_iPossibleRoads.push_back(ENCODE_SEL(iTile1, iMinus));
		m_iPossibleRoads.push_back(ENCODE_SEL(iTile3, iCorner3));
	}

	//tile 2 and tile 3
	if(TRUE == b23)
	{
		//tiles 2 and 3
		m_iPossibleRoads.push_back(ENCODE_SEL(iTile2, ((iCorner2 + 1) % 6)));
		m_iPossibleRoads.push_back(ENCODE_SEL(iTile3, ((iCorner3 + 5) % 6)));
	}
}

/////////////////////////////////////////////////////////////////////////////
// prepare initial ship placements
/////////////////////////////////////////////////////////////////////////////
void CGLView::prepareInitialShips()
{
	int iMinus;
	int iTile1, iTile2, iTile3;
	int iCorner, iCorner2, iCorner3;
	BOOL b12, b13, b23;

	//deconstruct the last built city
	DECODE_SEL(m_iLastCity, iTile1, iCorner);

	//set the trailing value
	iMinus = (iCorner + 5) % 6;

	//get the nearest tiles
	iTile2 = m_pBoard[iTile1].getSide(iCorner);
	iTile3 = m_pBoard[iTile1].getSide(iMinus);

	//determine placement options
	invalidPlaces(TRUE, iTile1, iCorner, b12, b13, b23);

	//reset the road places
	m_iPossibleShips.resize(0);
	m_iPossibleShips.clear();
	m_iPossibleRoads.resize(0);
	m_iPossibleRoads.clear();

	//set the alternate corners
	iCorner2 = ((iCorner + 3) % 6);
	iCorner3 = ((iMinus + 3) % 6);

	//tile 1 and tile 2
	if(TRUE == b12)
	{
		m_iPossibleShips.push_back(ENCODE_SEL(iTile1, iCorner));
		m_iPossibleShips.push_back(ENCODE_SEL(iTile2, iCorner2));
	}
	
	//tile 1 and tile 3
	if(TRUE == b13)
	{
		m_iPossibleShips.push_back(ENCODE_SEL(iTile1, iMinus));
		m_iPossibleShips.push_back(ENCODE_SEL(iTile3, iCorner3));
	}

	//tile 2 and tile 3
	if(TRUE == b23)
	{
		//tiles 2 and 3
		m_iPossibleShips.push_back(ENCODE_SEL(iTile2, ((iCorner2 + 1) % 6)));
		m_iPossibleShips.push_back(ENCODE_SEL(iTile3, ((iCorner3 + 5) % 6)));
	}
}

/////////////////////////////////////////////////////////////////////////////
// set up GL so it knows where valid road spots are
/////////////////////////////////////////////////////////////////////////////
int CGLView::prepareRoads()
{
	int i, j;
	int iSide;
	int iPlayer;
	int iPlus, iMinus;
	int iCount = 0;
	int iOwned = 0;
	int iCity;
	int iBranch;
	BOOL bBuilt[6];
	BOOL bOwned[6];
	
	//clear the old array
	m_iPossibleRoads.resize(0);
	m_iPossibleRoads.clear();

	//run through each of the tiles
	for(i = 0; i < m_nTiles; i++)
	{
		//reset the arrays
		iOwned = 0;
		memset(bBuilt, FALSE, sizeof(bBuilt));
		memset(bOwned, FALSE, sizeof(bOwned));

		//make a note of each road
		for(j = 0; j < 6; j++)
		{
			//get the road ID
			iPlayer = m_pBoard[i].getRoad(j);

			//see if there's even a road here
			if(iPlayer == -1) 
			{
				continue;
			}

			//see if this player owns it
			if(iPlayer == m_iPlayer)
			{
				bOwned[j] = TRUE;
				iOwned++;
			}

			//mark this side as being built on
			bBuilt[j] = TRUE;
		}

		//now go through and see what sides can be built on
		for(j = 0; j < 6; j++)
		{
			//keep going only if the player owns this side
			if(FALSE == bOwned[j]) 
			{
				continue;
			}

			//set the plus and minus
			iMinus = (j + 5) % 6;
			iPlus = (j + 1) % 6;

			//check the sides
			if( (FALSE == bBuilt[iMinus]) && 
				(FALSE == cityBlock(i, j)) && 
				(FALSE == seaBlock(i, iMinus)) &&
				(-1 == m_pBoard[i].getShip(iMinus)))
			{
				m_iPossibleRoads.push_back(ENCODE_SEL(i, iMinus));
				bBuilt[iMinus] = TRUE;
				
				//add the other side, for selection purposes
				iSide = m_pBoard[i].getSide(iMinus);

				//recalc the corner
				iMinus  = (iMinus + 3) % 6;

				//add the other side
				m_iPossibleRoads.push_back(ENCODE_SEL(iSide, iMinus));

				//increment
				iCount++;
			}

			if( (FALSE == bBuilt[iPlus]) && 
				(FALSE == cityBlock(i, iPlus)) && 
				(FALSE == seaBlock(i, iPlus)) &&
				(-1 == m_pBoard[i].getShip(iPlus)))
			{
				m_iPossibleRoads.push_back(ENCODE_SEL(i, iPlus));
				bBuilt[iPlus] = TRUE;

				//add the other side, for selection purposes
				iSide = m_pBoard[i].getSide(iPlus);

				//recalc the corner
				iPlus = (iPlus + 3) % 6;

				//add the other side
				m_iPossibleRoads.push_back(ENCODE_SEL(iSide, iPlus));

				//increment
				iCount++;
			}
		}

		//see if there are any cities/settlements that can be built from
		for(j = 0; j < 6; j++)
		{
			//get the city
			iCity = m_pBoard[i].getCity(j);

			//see if we own the city
			if((0 == iCity) || (iCity % 10) != m_iPlayer)
			{
				continue;
			}

			//since we have a city, we will now check it's two possible branches
			//from this tile to see if one or the other is a road tile and also
			//if those branches are not built on

			//get the second branch
			iBranch = (j + 5) % 6;

			//if there is no road or ship here, it is a valid location

			//check the first branch for existing roads or ships
			if( (-1 == m_pBoard[i].getRoad(j)) && 
				(-1 == m_pBoard[i].getShip(j)) &&
				(FALSE == seaBlock(i, j)))
			{
				m_iPossibleRoads.push_back(ENCODE_SEL(i, j));
				bBuilt[j] = TRUE;

				//add the other side, for selection purposes
				iSide = m_pBoard[i].getSide(j);

				//recalc the corner
				iPlus = (j + 3) % 6;

				//add the other side
				m_iPossibleRoads.push_back(ENCODE_SEL(iSide, iPlus));

				//increment
				iCount++;
			}

			//repeat for the branch side
			if( (-1 == m_pBoard[i].getRoad(iBranch)) && 
				(-1 == m_pBoard[i].getShip(iBranch)) &&
				(FALSE == seaBlock(i, iBranch)))
			{
				m_iPossibleRoads.push_back(ENCODE_SEL(i, iBranch));
				bBuilt[iBranch] = TRUE;

				//add the other side, for selection purposes
				iSide = m_pBoard[i].getSide(iBranch);

				//recalc the corner
				iPlus = (iBranch + 3) % 6;

				//add the other side
				m_iPossibleRoads.push_back(ENCODE_SEL(iSide, iPlus));

				//increment
				iCount++;
			}
		}
	}

	//return the count
	return iCount;
}

/////////////////////////////////////////////////////////////////////////////
// set up GL so it knows where valid ship spots are
/////////////////////////////////////////////////////////////////////////////
int CGLView::prepareShips()
{
	int i, j;
	int ID;
	int iSide;
	int iPlayer;
	int iPlus, iMinus;
	int iCount = 0;
	int iOwned = 0;
	int iCity;
	int iBranch;
	BOOL bBuilt[6];
	BOOL bOwned[6];

	m_iPossibleShips.resize(0);
	m_iPossibleShips.clear();

	//run through each of the sea tiles
	for(i = 0; i < m_nTiles; i++)
	{
		//check for land tiles or the pirate tile
		if(i == m_iPirate)
		{
			continue;
		}

		//reset the arrays
		iOwned = 0;
		memset(bBuilt, FALSE, sizeof(bBuilt));
		memset(bOwned, FALSE, sizeof(bOwned));

		//make a note of each existing ship on the tile
		for(j = 0; j < 6; j++)
		{
			//get the ship ID
			iPlayer = m_pBoard[i].getShip(j);

			//see if there's even a ship here
			if(iPlayer == -1)
			{
				continue;
			}

			//see if this player owns it
			if(iPlayer == m_iPlayer)
			{
				bOwned[j] = TRUE;
				iOwned++;
			}

			//mark this side as being built on
			bBuilt[j] = TRUE;
		}

		//now go through and see what sides can be built on
		for(j = 0; j < 6; j++)
		{
			//keep going only if the player owns this side
			if(FALSE == bOwned[j])
			{
				continue;
			}

			//if we're checking a move, we can't build of the spot that's
			//going to be moved
			ID = ENCODE_SEL(i, j);

			//check it
			if( (ID == m_iMovingShip[0]) ||
				(ID == m_iMovingShip[1]))
			{
				continue;
			}
			
			//set the plus and minus
			iMinus = (j + 5) % 6;
			iPlus = (j + 1) % 6;

			//set the minus ID
			ID = ENCODE_SEL(i, iMinus);

			//add the other side, for selection purposes
			iSide = m_pBoard[i].getSide(iMinus);

			//check the sides
			if( (FALSE == bBuilt[iMinus]) && 
				(FALSE == cityBlock(i, j)) && 
				(FALSE == landBlock(i, iMinus)) &&
				(-1 == m_pBoard[i].getRoad(iMinus)) &&
				(m_iPirate != m_pBoard[i].getSide(iMinus)) &&
				(ID != m_iMovingShip[0]) &&
				(ID != m_iMovingShip[1]) &&
				((FALSE == m_pBoard[i].getBorder()) || 
				(FALSE == m_pBoard[iSide].getBorder())))
			{
				m_iPossibleShips.push_back(ENCODE_SEL(i, iMinus));
				bBuilt[iMinus] = TRUE;

				//recalc the corner
				iMinus  = (iMinus + 3) % 6;

				//add the other side
				m_iPossibleShips.push_back(ENCODE_SEL(iSide, iMinus));

				//increment
				iCount++;
			}

			//set the plus ID
			ID = ENCODE_SEL(i, iPlus);

			//add the other side, for selection purposes
			iSide = m_pBoard[i].getSide(iPlus);

			if( (FALSE == bBuilt[iPlus]) && 
				(FALSE == cityBlock(i, iPlus)) && 
				(FALSE == landBlock(i, iPlus)) &&
				(-1 == m_pBoard[i].getRoad(iPlus)) && 
				(m_iPirate != m_pBoard[i].getSide(iPlus)) &&
				(ID != m_iMovingShip[0]) &&
				(ID != m_iMovingShip[1]) &&
				((FALSE == m_pBoard[i].getBorder()) || 
				(FALSE == m_pBoard[iSide].getBorder())))
			{
				m_iPossibleShips.push_back(ENCODE_SEL(i, iPlus));
				bBuilt[iPlus] = TRUE;

				//recalc the corner
				iPlus = (iPlus + 3) % 6;

				//add the other side
				m_iPossibleShips.push_back(ENCODE_SEL(iSide, iPlus));

				//increment
				iCount++;
			}
		}

		//see if there are any cities/settlements that can be built from
		for(j = 0; j < 6; j++)
		{
			//get the city
			iCity = m_pBoard[i].getCity(j);

			//see if we own the city
			if((0 == iCity) || (iCity % 10) != m_iPlayer)
			{
				continue;
			}

			//if we're checking a move, we can't build of the spot that's
			//going to be moved
			ID = ENCODE_SEL(i, j);

			//check it
			if( (ID == m_iMovingShip[0]) ||
				(ID == m_iMovingShip[1]))
			{
				continue;
			}

			//since we have a city, we will now check it's two possible branches
			//from this tile to see if one or the other is a sea tile and also
			//if those branches are not built on

			//get the second branch
			iBranch = (j + 5) % 6;

			//if there is no road or ship here, it is a valid location,
			//but first we check to see if the other side is of a lower tile
			//ID than this side, because we don't want to add the same side
			//twice

			//set the ID
			ID = ENCODE_SEL(i, j);

			//add the other side, for selection purposes
			iSide = m_pBoard[i].getSide(j);

			//check the first branch for existing roads or ships
			if( (-1 == m_pBoard[i].getRoad(j)) && 
				(-1 == m_pBoard[i].getShip(j)) &&
				(FALSE == landBlock(i, j)) &&
				(m_iPirate != m_pBoard[i].getSide(j)) &&
				(ID != m_iMovingShip[0]) &&
				(ID != m_iMovingShip[1]) &&
				((FALSE == m_pBoard[i].getBorder()) || 
				(FALSE == m_pBoard[iSide].getBorder())))
			{
				m_iPossibleShips.push_back(ENCODE_SEL(i, j));
				bBuilt[j] = TRUE;

				//recalc the corner
				iPlus = (j + 3) % 6;

				//add the other side
				m_iPossibleShips.push_back(ENCODE_SEL(iSide, iPlus));

				//increment
				iCount++;
			}

			//set the ID
			ID = ENCODE_SEL(i, iBranch);

			//add the other side, for selection purposes
			iSide = m_pBoard[i].getSide(iBranch);

			//repeat for the branch side
			if( (-1 == m_pBoard[i].getRoad(iBranch)) && 
				(-1 == m_pBoard[i].getShip(iBranch)) &&
				(FALSE == landBlock(i, iBranch)) &&
				(m_iPirate != m_pBoard[i].getSide(iBranch))&&
				(ID != m_iMovingShip[0]) &&
				(ID != m_iMovingShip[1]) &&
				((FALSE == m_pBoard[i].getBorder()) || 
				(FALSE == m_pBoard[iSide].getBorder())))
			{
				m_iPossibleShips.push_back(ENCODE_SEL(i, iBranch));
				bBuilt[iBranch] = TRUE;

				//recalc the corner
				iPlus = (iBranch + 3) % 6;

				//add the other side
				m_iPossibleShips.push_back(ENCODE_SEL(iSide, iPlus));

				//increment
				iCount++;
			}
		}
	}

	//return the count
	return iCount;
}

/////////////////////////////////////////////////////////////////////////////
// set up GL so it knows where valid city spots are
/////////////////////////////////////////////////////////////////////////////
int CGLView::prepareCities()
{
	int i, j;
	int ID;
	int iSettle, iPlayer;
	int iSide1, iSide2;
	int iCount = 0;

	//clear the old array
	m_iPossibleCities.resize(0);
	m_iPossibleCities.clear();

	//run through each of the land tiles
	for(i = 0; i < m_nTiles; i++)
	{
		//check for ocean tiles
		if(TRUE == VALID_OCEAN(m_pBoard[i].getType()))
		{
			continue;
		}

		//run through and look for settlements that belong to this player
		for(j = 0; j < 6; j++)
		{
			//get the ID
			ID = m_pBoard[i].getCity(j);

			//get the ID
			if(0 == ID) continue;

			//deconstruct the code
			DECODE_SEL(ID, iSettle, iPlayer);

			//see if it's a settlement
			if(iSettle != 1) continue;

			//see if it belongs to this player
			if(iPlayer != m_iPlayer) continue;

			//get the rest of the info
			iSide1 = m_pBoard[i].getSide(j);
			iSide2 = m_pBoard[i].getSide((j + 5) % 6);

			//add these tiles to the possible city array
			m_iPossibleCities.push_back(i);
			m_iPossibleCities.push_back(iSide1);
			m_iPossibleCities.push_back(iSide2);

			//increment the counter
			iCount++;
		}
	}

	//return the count
	return iCount;
}

/////////////////////////////////////////////////////////////////////////////
// set up GL so it knows where valid settlement spots are
/////////////////////////////////////////////////////////////////////////////
int CGLView::prepareSettlements()
{
	int i, j;
	int iCount = 0;
	int iType;

	//clear the old array
	m_iPossibleSettlements.resize(0);
	m_iPossibleSettlements.clear();

	//search for legitimate spots
	for(i = 0; i < m_nTiles; i++)
	{
		//check for ocean tiles
		if(TRUE == VALID_OCEAN(m_pBoard[i].getType()))
		{
			continue;
		}

		for(j = 0; j < 6; j++)
		{
			iType = m_pBoard[m_pBoard[i].getSide(j)].getType();

			//only draw circles on hexes with a higher number
			if((FALSE == cityCheck(i, j)) && (TRUE == roadCheck(i, j)))
			{
				m_iPossibleSettlements.push_back(ENCODE_SEL(i, j));
				iCount++;
			}
		}
	}

	return iCount;
}

/////////////////////////////////////////////////////////////////////////////
// prepare possible port tiles
/////////////////////////////////////////////////////////////////////////////
void CGLView::preparePorts()
{
	int i, j;
	int nSide;
	BOOL bLand;

	//sanity
	if(FALSE == (MAP->m_iMode & MAP_SEAFARERS))
	{
		return;
	}

	//clear the array
	m_iPossiblePorts.resize(0);
	m_iPossiblePorts.clear();

	//traverse the tiles, looking for sea tiles that touch at least one land tile
	for(i = 0; i < m_nTiles; i++)
	{
		//skip land tiles and existing port tiles
		if( (TRUE == VALID_LAND(m_pBoard[i].getType())) ||
			(TRUE == VALID_PORT(m_pBoard[i].getType())))
		{
			continue;
		}

		//reset the land value
		bLand = FALSE;

		//see if it touches land
		for(j = 0; j < 6; j++)
		{
			//get the side
			nSide = m_pBoard[i].getSide(j);

			//if -1, continue
			if(-1 == nSide)
			{
				continue;
			}

			//check for land
			if(TRUE == VALID_LAND(m_pBoard[nSide].getType()))
			{
				bLand = TRUE;
				break;
			}
		}

		//see if we can add it
		if(TRUE == bLand)
		{
			m_iPossiblePorts.push_back(i);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// prepare possible port sides for selection
/////////////////////////////////////////////////////////////////////////////
void CGLView::preparePortSides()
{
	int i;
	int nSide;

	//sanity
	if(-1 == m_nPortTile)
	{
		return;
	}

	//clear the array
	m_iPossiblePortSides.resize(0);
	m_iPossiblePortSides.clear();

	//now search the tile for land tiles around it
	for(i = 0; i < 6; i++)
	{
		//get the side
		nSide = m_pBoard[m_nPortTile].getSide(i);

		//check it
		if((-1 != nSide) && (TRUE == VALID_LAND(m_pBoard[nSide].getType())))
		{
			m_iPossiblePortSides.push_back(nSide);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// see if the current player can move any ships
/////////////////////////////////////////////////////////////////////////////
int CGLView::prepareShipMove()
{
	int i, j, k;
	int ID;
	int iCount = 0;
	int iT2, iC2;
	int nShips;
	BOOL bThisTurn;
	vector <int> nSave;

	//clear the array
	m_iPossibleShipMoves.resize(0);
	m_iPossibleShipMoves.clear();

	//sanity
	if(FALSE == (MAP->m_iMode & MAP_SEAFARERS))
	{
		return 0;
	}

	if(TRUE == m_pGame->m_players[m_iPlayer].m_bMovedShip)
	{
		return 0;
	}

	//run through each tile looking for ships
	for(i = 0; i < m_nTiles; i++)
	{
		//check each side
		for(j = 0; j < 6; j++)
		{
			//only visit sides on lower tiles, thus looking at each possibility
			//only once
			if(m_pBoard[i].getSide(j) < i)
			{
				continue;
			}

			//get the value
			if(m_pBoard[i].getShip(j) == m_iPlayer)
			{
				//see if this combination is in the list of ships built this turn,
				//in which case we skip it
				ID = ENCODE_SEL(i, j);

				//set the flag
				bThisTurn = FALSE;

				//search the array
				for(k = 0; k < m_nShipsThisTurn.GetSize(); k++)
				{
					if(m_nShipsThisTurn[k] == ID)
					{
						bThisTurn = TRUE;
						break;
					}
				}
				
				//see if we can continue
				if(TRUE == bThisTurn)
				{
					continue;
				}

				//get other side
				iT2 = m_pBoard[i].getSide(j);
				iC2 = (j + 3) % 6;

				//ensure that we would have a place to move it to if this ship
				//was valid
				m_iMovingShip[0] = ID;
				m_iMovingShip[1] = ENCODE_SEL(iT2, iC2);

				//save possible ships
				nSave = m_iPossibleShips;

				//check
				nShips = prepareShips();

				//restore possible ships
				m_iPossibleShips = nSave;

				//reset
				::ZeroMemory(m_iMovingShip, sizeof(m_iMovingShip));

				if(0 >= nShips)
				{
					continue;
				}

				//run the algorithm
				if(TRUE == moveShip(i, j))
				{
					//add it to the possibility list
					m_iPossibleShipMoves.push_back(ID);

					//add to the other side
					m_iPossibleShipMoves.push_back(ENCODE_SEL(iT2, iC2));
					
					iCount++;
				}
			}
		}
	}

	return iCount;
}
