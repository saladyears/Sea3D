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
#include "StatGrid.h"
#include "StatsDialog.h"
#include "SettlersView.h"
#include "ColorDatabase.h"
#include "defineStat.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
	
/////////////////////////////////////////////////////////////////////////////
// create the frequency table for roll stats
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::createRollFreq(int &iRow)
{
	int i;
	int iCol = 3;
	CString str;

	//create the title
	createTitle(STAT_ROLL_FREQ, iRow);

	//roll column
	SetItemText(iRow, iCol++, "Die Roll");

	//occurence column
	SetItemText(iRow, iCol++, "# Rolled");

	//% occurrence column
	SetItemText(iRow, iCol++, "% Rolled");

	//% expected column
	SetItemText(iRow, iCol++, "% Expect");

	//add one to the row
	iRow++;

	//now set values for the rest of the table
	for(i = 0; i < 11; i++)
	{
		//reset the column
		iCol = 3;

		//set the roll value
		str.Format("%d", i + 2);

		//set it
		SetItemText(iRow, iCol++, str);

		//set the current value
		SetItemText(iRow, iCol++, "0");

		//set the percentage
		SetItemText(iRow, iCol++, "0.00%");

		//set the expected percentage
		switch(i + 2)
		{
		case 2: case 12: str = "2.77%"; break;
		case 3: case 11: str = "5.56%"; break;
		case 4: case 10: str = "8.33%"; break;
		case 5: case 9: str = "11.11%"; break;
		case 6: case 8: str = "13.89%"; break;
		case 7: str = "16.67%"; break;
		}

		//set it
		SetItemText(iRow, iCol, str);

		//increment the row
		iRow++;
	}

	//draw the outline
	outline(iRow - 12, 3, iRow - 1, 6);

	//add the average row
	SetItemText(iRow, 3, "Average:");
	SetItemText(iRow++, 4, "0.00");

	//set the next row
	iRow++;
}

/////////////////////////////////////////////////////////////////////////////
// create the frequency table for roll stats by player
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::createRollPlayer(int &iRow)
{
	int i, j;
	int iCol = 3;
	CString str;
	COLORREF color;

	//create the title
	createTitle(STAT_ROLL_PLAYER, iRow);

	//roll column
	SetItemText(iRow, iCol++, "Die Roll");

	//add the player names
	for(i = 0; i < m_iPlayers; i++)
	{			
		//get the light color
		color = COLORS->getTextColor(GAME->m_players[i].m_iColor);

		//set the name
		SetItemText(iRow, iCol, GAME->m_players[i].m_player.getFirstName());

		//set the column color
		fill(iRow, iCol, iRow + 11, iCol, color);

		//increment the column
		iCol++;
	}

	//reset the column, increment the row
	iCol = 3; iRow++;

	//now set values for the rest of the table
	for(i = 0; i < 11; i++)
	{
		//set the roll value
		str.Format("%d", i + 2);

		//set it
		SetItemText(iRow, iCol, str);

		//set the startup values
		for(j = 0; j < m_iPlayers; j++)
		{
			SetItemText(iRow, iCol + 1 + j, "0");
		}

		//increment the row
		iRow++;
	}

	//draw the outline
	outline(iRow - 12, 3, iRow - 1, 3 + m_iPlayers);

	//add the average row
	SetItemText(iRow, 3, "Average:");

	//set the values
	for(i = 0; i < m_iPlayers; i++)
	{
		SetItemText(iRow, 4 + i, "0.00");
	}

	//set the next row
	iRow += 2;
}

/////////////////////////////////////////////////////////////////////////////
// create the table of goods received per player
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::createGoodsTable(int &iRow, int iType)
{
	int i, j;
	int iCol = 3;
	int iSave;
	CString str;
	COLORREF color;

	//save the row
	iSave = iRow;

	//create the title
	createTitle(iType, iRow);

	//create the goods header
	createGoodsHeader(iRow, iCol);

	//increment the row
	iRow++;

	//set the player names
	for(i = 0; i <= m_iPlayers; i++)
	{
		//reset starting column
		iCol = 3;

		//get the name
		if(i < m_iPlayers)
		{
			str = GAME->m_players[i].m_player.getFirstName();
			
			//get the light color
			color = COLORS->getTextColor(GAME->m_players[i].m_iColor);
		}
		else
		{
			str = "Total";
			color = COLOR_WHITE;
		}

		//add it
		fill(iRow, iCol, iRow, iCol + 6, color);
		SetItemFormat(iRow, iCol, DT_LEFT);
		SetItemText(iRow, iCol++, str);

		//add the values
		for(j = 0; j < 6; j++)
		{
			SetItemText(iRow, iCol++, "0");
		}
		
		//incrememt the row
		iRow++;
	}
	
	//draw the outline
	outline(iRow - m_iPlayers - 2, 3, iRow - 1, 9);

	//add the total line
	for(i = 3; i < 10; i++)
	{
		SetItemGridLines(iRow - 1, i, GVL_BOTH);
	}

	//add one to the row
	iRow++;
}

/////////////////////////////////////////////////////////////////////////////
// create the time per turn table
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::createTimeTurn(int &iRow)
{
	int i, j;
	int iCol = 3;
	CString str;
	COLORREF color;

	//create the title
	createTitle(STAT_TIME_TURN, iRow);

	//create the column headers
	SetItemText(iRow, iCol++, "Player");
	SetItemText(iRow, iCol++, "Turns");
	SetItemText(iRow, iCol++, "Total Time");
	SetItemText(iRow, iCol++, "Average");
	SetItemText(iRow, iCol++, "Longest");
	SetItemText(iRow, iCol++, "Shortest");

	//increment the row
	iRow++;

	//run through the player list
	for(i = 0; i <= m_iPlayers; i++)
	{
		//reset starting column
		iCol = 3;
			
		//get the name
		if(i < m_iPlayers)
		{
			str = GAME->m_players[i].m_player.getFirstName();
			
			//get the light color
			color = COLORS->getTextColor(GAME->m_players[i].m_iColor);
		}
		else
		{
			str = "Total";
			color = COLOR_WHITE;
		}

		//add it
		fill(iRow, iCol, iRow, iCol + 5, color);
		SetItemFormat(iRow, iCol, DT_LEFT);
		SetItemText(iRow, iCol++, str);

		//add the values
		for(j = 0; j < 5; j++)
		{
			SetItemText(iRow, iCol++, j ? "0:00" : "0");
		}
		
		//incrememt the row
		iRow++;
	}

	//draw the outline
	outline(iRow - m_iPlayers - 2, 3, iRow - 1, 8);

	//add the total line
	for(i = 3; i < 9; i++)
	{
		SetItemGridLines(iRow - 1, i, GVL_BOTH);
	}

	//add one more row
	iRow++;
}

/////////////////////////////////////////////////////////////////////////////
// create the trade numbers table
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::createTradeNo(int &iRow)
{
	int i, j;
	int iCol = 3;
	CString str;
	COLORREF color;

	//create the title
	createTitle(STAT_TRADE_NUMBER, iRow);

	//create the column headers
	SetItemText(iRow, iCol++, "Player");
	SetItemText(iRow, iCol++, "To others");
	SetItemText(iRow, iCol++, "Bank: 2-1");
	SetItemText(iRow, iCol++, "Bank: 3-1");
	SetItemText(iRow, iCol++, "Bank: 4-1");
	SetItemText(iRow, iCol++, "Total");

	//increment the row
	iRow++;

	//run through the player list
	for(i = 0; i <= m_iPlayers; i++)
	{
		//reset starting column
		iCol = 3;
			
		//get the name
		if(i < m_iPlayers)
		{
			str = GAME->m_players[i].m_player.getFirstName();
			
			//get the light color
			color = COLORS->getTextColor(GAME->m_players[i].m_iColor);
		}
		else
		{
			str = "Total";
			color = COLOR_WHITE;
		}

		//add it
		fill(iRow, iCol, iRow, iCol + 5, color);
		SetItemFormat(iRow, iCol, DT_LEFT);
		SetItemText(iRow, iCol++, str);

		//add the values
		for(j = 0; j < 5; j++)
		{
			SetItemText(iRow, iCol++, "0");
		}
		
		//incrememt the row
		iRow++;
	}

	//draw the outline
	outline(iRow - m_iPlayers - 2, 3, iRow - 1, 8);

	//add the total line
	for(i = 3; i < 9; i++)
	{
		SetItemGridLines(iRow - 1, i, GVL_BOTH);
	}

	//add one more row
	iRow++;
}

/////////////////////////////////////////////////////////////////////////////
// create the trade numbers table
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::createTradePartners(int &iRow)
{
	int i, j;
	int iCol = 3;
	CString str;
	COLORREF color;

	//create the title
	createTitle(STAT_TRADE_NUM_OTHERS, iRow);

	//create the column headers
	SetItemText(iRow, iCol, "Player");

	//run through the player list
	for(i = 0; i < m_iPlayers; i++)
	{		
		//get the name
		str = GAME->m_players[i].m_player.getFirstName();
			
		//get the light color
		color = COLORS->getTextColor(GAME->m_players[i].m_iColor);

		//add it
		fill(iRow + i + 1, iCol, iRow + i + 1, iCol + m_iPlayers, color);
		SetItemFormat(iRow, iCol, DT_LEFT);
		SetItemText(iRow + i + 1, iCol, str);

		//add the column title
		SetItemText(iRow, iCol + i + 1, str);

		//add the values
		for(j = 0; j < m_iPlayers; j++)
		{
			SetItemText(iRow + i + 1, iCol + j + 1, (j == i) ? "--" : "0");
		}
	}

	//draw the outline
	outline(iRow, 3, iRow + m_iPlayers, 3 + m_iPlayers);

	//add to the rows
	iRow += m_iPlayers + 2;
}

/////////////////////////////////////////////////////////////////////////////
// create the trading ratios table
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::createTradeRatios(int &iRow)
{
	int i, j;
	int iCol = 3;
	CString str;
	COLORREF color;

	//create the title
	createTitle(STAT_TRADE_RATIO, iRow);

	//create the column headers
	SetItemText(iRow, iCol++, "Player");
	SetItemText(iRow, iCol++, "To Bank");
	SetItemText(iRow, iCol++, "From Bank");
	SetItemText(iRow, iCol++, "Bank Ratio");
	SetItemText(iRow, iCol++, "To Others");
	SetItemText(iRow, iCol++, "From Others");
	SetItemText(iRow, iCol++, "Others Ratio");
	SetItemText(iRow, iCol++, "To Total");
	SetItemText(iRow, iCol++, "From Total");
	SetItemText(iRow, iCol++, "Total Ratio");

	//increment the row
	iRow++;

	//run through the player list
	for(i = 0; i < m_iPlayers; i++)
	{
		//reset starting column
		iCol = 3;
			
		//get the name
		str = GAME->m_players[i].m_player.getFirstName();
			
		//get the light color
		color = COLORS->getTextColor(GAME->m_players[i].m_iColor);

		//add it
		fill(iRow, iCol, iRow, iCol + 9, color);
		SetItemFormat(iRow, iCol, DT_LEFT);
		SetItemText(iRow, iCol++, str);

		//add the values
		for(j = 0; j < 9; j++)
		{
			SetItemText(iRow, iCol++, ((j + 1) % 3) ? "0" : "0.00");
		}
		
		//incrememt the row
		iRow++;
	}

	//draw the outline
	outline(iRow - m_iPlayers - 1, 3, iRow - 1, 12);

	//add one more row
	iRow++;
}