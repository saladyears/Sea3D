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
#include "define.h"
#include "defineVariants.h"

/////////////////////////////////////////////////////////////////////////////
// set the network mode, based on what the server says
/////////////////////////////////////////////////////////////////////////////
void CGLView::setNetDrawMode(int iMode)
{
	BOOL bAI;

	//set draw mode
	m_iMode = iMode;

	//determine if current player is an AI
	bAI = m_pGame->m_players[m_iPlayer].m_player.getAI();

	//do some stuff
	switch(m_iMode)
	{
	case GL_PLACE_PLAYERS: 
		THISVIEW->getControlPanel()->updateColor(m_iPlayer);
		if(FALSE == bAI)
		{
			if(IS_VARIANT(VARIANT_TOURNAMENT_START) && (FALSE == m_bIncrease))
			{
				THISVIEW->getControlPanel()->setPanelText("Your turn to place a city."); 
			}
			else
			{
				THISVIEW->getControlPanel()->setPanelText("Your turn to place a settlement."); 
			}
		}

		//selection
		m_bUseSelection = TRUE;

		//reset
		m_iSelection = -1;
		
		//turn on hit tests
		setHitTest(TRUE);

		break;
	case GL_PLACE_PORT:
		//selection
		m_bUseSelection = TRUE;

		//reset
		m_iSelection = -1;

		//reset the port tile
		m_nPortTile = -1;

		//prepare the port placements
		preparePorts();

		//set the panel text
		THISVIEW->getControlPanel()->updateColor(m_iPlayer);
		THISVIEW->getControlPanel()->setPanelText("Your turn to place a port."); 

		//turn on hit tests
		setHitTest(TRUE);

		break;
	case GL_PLACE_THIRD_ROAD:
		//selection
		m_bUseSelection = TRUE;

		//reset
		m_iSelection = -1;

		//prepare roads
		prepareRoads();

		//set the panel text
		THISVIEW->getControlPanel()->updateColor(m_iPlayer);
		THISVIEW->getControlPanel()->setPanelText("Your turn to place a road."); 

		//turn on hit tests
		setHitTest(TRUE);
	}

	//redraw
	Invalidate();
}

//////////////////////////////////////////////////////////////////
// write to a network message
//////////////////////////////////////////////////////////////////
void CGLView::SerializeToMessage(CNDKMessage &msg)
{
	int i;
	int iSize;
	int iStart = 0;

	//current player
	msg.SetAt(iStart++, m_iPlayer);

	//tile size
	msg.SetAt(iStart++, m_nTiles);

	//who played the robber last
	msg.SetAt(iStart++, m_iRobberPlayer);

	//who played the pirate last
	msg.SetAt(iStart++, m_iPiratePlayer);

	//the number of allowed cards in hand
	msg.SetAt(iStart++, m_iAllowedCards);

	//for road building, how many free road placements are left
	msg.SetAt(iStart++, m_iFreeRoads);

	//dev cards
	msg.SetAt(iStart++, m_iDevCardIndex);

	//hidden tiles
	msg.SetAt(iStart++, m_nHiddenIndex);

	//hidden volcano information
	msg.SetAt(iStart++, m_nHiddenVolcanoIndex);

	//hidden roll chits
	msg.SetAt(iStart++, m_nHiddenChitIndex);

	//chitless chits
	msg.SetAt(iStart++, m_nChitlessChitIndex);

	//ports size
	iSize = m_iPorts.GetSize();
	msg.SetAt(iStart++, iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.SetAt(iStart + i, m_iPorts[i]);
	}
	iStart += iSize;

	//last rolled tiles size
	iSize = m_iLastTiles.size();
	msg.SetAt(iStart++, iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.SetAt(iStart + i, m_iLastTiles[i]);
	}
	iStart += iSize;

	//other stuff
	msg.SetAt(iStart++, m_iLastCity);
}


//////////////////////////////////////////////////////////////////
// read froma network message
//////////////////////////////////////////////////////////////////
void CGLView::SerializeFromMessage(CNDKMessage &msg)
{
	int i;
	int iSize;
	int iStart = 0;

	//current player
	msg.GetAt(iStart++, m_iPlayer);

	//tile size
	msg.GetAt(iStart++, m_nTiles);

	//who played the robber last
	msg.GetAt(iStart++, m_iRobberPlayer);

	//who played the pirate last
	msg.GetAt(iStart++, m_iPiratePlayer);

	//the number of allowed cards in hand
	msg.GetAt(iStart++, m_iAllowedCards);

	//for road building, how many free road placements are left
	msg.GetAt(iStart++, m_iFreeRoads);

	//dev cards
	msg.GetAt(iStart++, m_iDevCardIndex);

	//hidden tiles
	msg.GetAt(iStart++, m_nHiddenIndex);

	//hidden volcano index
	msg.GetAt(iStart++, m_nHiddenVolcanoIndex);

	//hidden roll chits
	msg.GetAt(iStart++, m_nHiddenChitIndex);

	//chitless chits
	msg.GetAt(iStart++, m_nChitlessChitIndex);

	//ports size
	msg.GetAt(iStart++, iSize);
	m_iPorts.SetSize(iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.GetAt(iStart + i, m_iPorts[i]);
	}
	iStart += iSize;

	//last rolled tiles size
	msg.GetAt(iStart++, iSize);
	m_iLastTiles.resize(iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.GetAt(iStart + i, m_iLastTiles[i]);
	}
	iStart += iSize;

	//other stuff
	msg.GetAt(iStart++, m_iLastCity);
}
