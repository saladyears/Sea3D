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
#include "SettlersView.h"
#include "defineStat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// update the roll frequency table
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::updateRollFreq()
{
	int i, j;
	int iTotals[11];
	int iSumTotals[6];
	int iPlayerTotals[6];
	int iRow;
	int iTotal = 0;
	int iSumTotal = 0;
	CString str;

	//lock updates until we're done
	SetRedraw(FALSE);

	//zero memory
	::ZeroMemory(iTotals, sizeof(iTotals));
	::ZeroMemory(iSumTotals, sizeof(iSumTotals));
	::ZeroMemory(iPlayerTotals, sizeof(iPlayerTotals));

	//figure out the starting row
	iRow = m_iJump[STAT_ROLL_FREQ] + 3;

	//go through and get the total
	for(i = 0; i < 11; i++)
	{
		//create the totals array
		for(j = 0; j < 6; j++)
		{
			//add to the totals
			iTotals[i] += GAME->m_iRolls[j][i];

			//add to the sum totals
			iSumTotals[j] += (GAME->m_iRolls[j][i] * (i + 2));

			//add to the player totals
			iPlayerTotals[j] += GAME->m_iRolls[j][i];
		}

		//add to the total
		iTotal += iTotals[i];

		//add to the sum total
		iSumTotal += (iTotals[i] * (i + 2));

		//set the text
		str.Format("%d", iTotals[i]);

		//reset the text
		SetItemText(iRow + i, 4, str);
	}

	//only do percentages if iTotal is not zero
	if(iTotal)
	{
		//now calculate percentages
		for(i = 0; i < 11; i++)
		{
			//set the text
			str.Format("%.2f%%", 100. * (float) iTotals[i] / (float) iTotal);

			//set the text
			SetItemText(iRow + i, 5, str);
		}

		//update the average row
		str.Format("%.2f", (float) iSumTotal / (float) iTotal);
		SetItemText(iRow + i, 4, str);
	}

	//now update player rows
	iRow = m_iJump[STAT_ROLL_PLAYER] + 3;

	//run through the rows
	for(i = 0; i < 11; i++)
	{
		//run through each player
		for(j = 0; j < m_iPlayers; j++)
		{
			//get the text
			str.Format("%d", GAME->m_iRolls[j][i]);

			//set it
			SetItemText(iRow + i, 4 + j, str);
		}
	}

	//now set the averages
	for(j = 0; j < m_iPlayers; j++)
	{
		//set the average
		if(iPlayerTotals[j])
		{
			str.Format("%.2f", (float) iSumTotals[j] / (float) iPlayerTotals[j]);
		}
		else
		{
			str = "0.00";
		}

		//set the value
		SetItemText(iRow + i, 4 + j, str);
	}

	//unlock updates
	SetRedraw(TRUE);
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// update the times of people's turns
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::updateTime()
{
	int i, j;
	int iRow;
	int iTurns;
	int iTotal = 0;
	int iAmount;
	int iShortest, iLongest;
	int iOShort, iOLong;
	int iSumTotal[6];
	int iTotalTime = 0;
	CString str;
	CPlayerGame *pGame;

	//lock updates until we're done
	SetRedraw(FALSE);

	//figure out the starting row 
	iRow = m_iJump[STAT_TIME_TURN] + 3;

	//reset counts
	::ZeroMemory(iSumTotal, sizeof(iSumTotal));

	//reset short/long
	iOShort = INT_MAX;
	iOLong = 0;

	//run through the list of players
	for(i = 0; i < m_iPlayers; i++)
	{
		//get the pointer
		pGame = &(GAME->m_players[i]);

		//get the player's number of turns
		iTurns = pGame->m_iTurnTime.size();

		//reset the row total, shortest and longest
		iShortest = iTurns ? INT_MAX : 0;
		iLongest = 0;

		//add to the total turns so far
		iTotal += iTurns;

		//run through the number of turns
		for(j = 0; j < iTurns; j++)
		{
			//get the number of seconds for this turn
			iAmount = pGame->m_iTurnTime[j];

			//add to the total turn length for this player
			iSumTotal[i] += iAmount;

			//see if it replaces shortest or longest
			if(iAmount < iShortest) 
			{
				iShortest = iAmount;
			}
			if(iAmount > iLongest)
			{
				iLongest = iAmount;
			}

			//see if it replaces overall shortest or longest
			if(iAmount < iOShort)
			{
				iOShort = iAmount;
			}
			if(iAmount > iOLong)
			{
				iOLong = iAmount;
			}
		}

		//set the number of turns
		str.Format("%d", iTurns);

		//write it
		SetItemText(iRow + i, 4, str);
		
		//write the total string
		SetItemText(iRow + i, 5, createTimeString(iSumTotal[i]));

		//set the average
		SetItemText(iRow + i, 6, iTurns ? createTimeString(iSumTotal[i] / iTurns) : "0:00");

		//set the shortest
		SetItemText(iRow + i, 7, createTimeString(iLongest));

		//set the longest
		SetItemText(iRow + i, 8, createTimeString(iShortest));
	}

	//now write the column totals
	for(j = 0; j < 6; j++)
	{
		//add to the grand total
		iTotalTime += iSumTotal[j];
	}

	//set the grand total of turns
	str.Format("%d", iTotal);

	//write it
	SetItemText(iRow + i, 4, str);

	//write the total string
	SetItemText(iRow + i, 5, createTimeString(iTotalTime));

	//set the average
	SetItemText(iRow + i, 6, iTotal ? createTimeString(iTotalTime / iTotal) : "0:00");

	//set the shortest
	SetItemText(iRow + i, 7, createTimeString(iOLong));

	//set the longest
	SetItemText(iRow + i, 8, iOShort == INT_MAX ? "0:00" : createTimeString(iOShort));

	//unlock updates
	SetRedraw(TRUE);
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// create a time string based on the number of seconds sent in
/////////////////////////////////////////////////////////////////////////////
CString CStatGrid::createTimeString(int iSeconds)
{
	int iHours;
	int iMinutes;
	CString str, strTemp;

	//calculate the number of hours in this amount of seconds
	iHours = iSeconds / 3600; iSeconds -= (iHours * 3600);

	//calculate the number of minutes in the remaining amount of seconds
	iMinutes = iSeconds / 60; iSeconds -= (iMinutes * 60);

	//format the string
	if(iHours)
	{
		str.Format("%d:", iHours);
	}

	//set minutes
	if(iHours || iMinutes)
	{
		strTemp.Format(iHours ? "%.2d" : "%d", iMinutes);
		str += strTemp;
	}

	//set seconds
	strTemp.Format(":%.2d", iSeconds);
	str += strTemp;

	//return the string
	return str;
}

/////////////////////////////////////////////////////////////////////////////
// update the number of cards traded
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::updateTradeNo()
{
	int i, j, k;
	int iRow;
	int iTotal = 0;
	int iAmount;
	int iRowTotal = 0;
	int iSumTotal[4];
	CString str;
	CPlayerGame *pGame;

	//lock updates until we're done
	SetRedraw(FALSE);

	//figure out the starting row 
	iRow = m_iJump[STAT_TRADE_NUMBER] + 3;

	//reset counts
	::ZeroMemory(iSumTotal, sizeof(iSumTotal));

	//run through the list of players
	for(i = 0; i < m_iPlayers; i++)
	{
		//get the pointer
		pGame = &(GAME->m_players[i]);

		//reset the row total
		iRowTotal = 0;

		//run through each trade type
		for(j = 0; j < 4; j++)
		{
			//get the right amount of trades with others
			if(!j)
			{
				for(k = 0, iAmount = 0; k < m_iPlayers; iAmount += pGame->m_iTradesOthers[k++]);
			}
			//trades to the bank
			else
			{
				iAmount = pGame->m_iTradesBank[j - 1];
			}

			//add to the total
			iSumTotal[j] += iAmount;

			//add to the row total
			iRowTotal += iAmount;

			//format the string
			str.Format("%d", iAmount);

			//set the value
			SetItemText(iRow + i, 4 + j, str);
		}

		//set the total array
		str.Format("%d", iRowTotal);

		//write it
		SetItemText(iRow + i, 4 + j, str);
	}

	//now write the column totals
	for(j = 0; j < 4; j++)
	{
		//add to the grand total
		iTotal += iSumTotal[j];

		//format the string
		str.Format("%d", iSumTotal[j]);

		//write it
		SetItemText(iRow + i, 4 + j, str);
	}

	//set the grand total
	str.Format("%d", iTotal);

	//write it
	SetItemText(iRow + i, 4 + j, str);

	//unlock updates
	SetRedraw(TRUE);
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// update who has traded with who
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::updateTradePartners()
{
	int i, j;
	int iRow;
	CString str;
	CPlayerGame *pGame;

	//lock updates until we're done
	SetRedraw(FALSE);

	//figure out the starting row 
	iRow = m_iJump[STAT_TRADE_NUM_OTHERS] + 3;

	//run through the list of players
	for(i = 0; i < m_iPlayers; i++)
	{
		//get the pointer
		pGame = &(GAME->m_players[i]);

		//run through the other players
		for(j = 0; j < m_iPlayers; j++)
		{
			//skip self
			if(i == j) continue;

			//post the number
			str.Format("%d", pGame->m_iTradesOthers[j]);
	
			//set the value
			SetItemText(iRow + i, 4 + j, str);
		}
	}

	//unlock updates
	SetRedraw(TRUE);
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// update the trading ratio block
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::updateTradeRatios()
{
	int i, j;
	int iRow;
	int iBankTo, iBankFrom;
	int iOthersTo, iOthersFrom;
	CString str;
	CPlayerGame *pGame;

	//lock updates until we're done
	SetRedraw(FALSE);

	//figure out the starting row 
	iRow = m_iJump[STAT_TRADE_RATIO] + 3;

	//run through the list of players
	for(i = 0; i < m_iPlayers; i++)
	{
		//get the pointer
		pGame = &(GAME->m_players[i]);

		//reset all values
		iBankTo = iBankFrom = iOthersTo = iOthersFrom = 0;

		//sum the array of bank to and from
		for(j = 0; j < 5; j++)
		{
			iBankTo += pGame->m_iBankTradeTo[j];
			iBankFrom += pGame->m_iBankTradeFrom[j];
		}

		//set the values
		str.Format("%d", iBankTo);
		SetItemText(iRow + i, 4, str);
		str.Format("%d", iBankFrom);
		SetItemText(iRow + i, 5, str);
		str.Format("%.2f", iBankTo ? (float) iBankFrom / (float) iBankTo : 0.00);
		SetItemText(iRow + i, 6, str);

		//run through the resources and sum arrays
		for(j = 0; j < 5; j++)
		{
			//add to the totals
			iOthersTo += sumArray(pGame->m_iOtherTradeTo, j);
			iOthersFrom += sumArray(pGame->m_iOtherTradeFrom, j);
		}

		//set the values
		str.Format("%d", iOthersTo);
		SetItemText(iRow + i, 7, str);
		str.Format("%d", iOthersFrom);
		SetItemText(iRow + i, 8, str);
		str.Format("%.2f", iOthersTo ? (float) iOthersFrom / (float) iOthersTo : 0.00);
		SetItemText(iRow + i, 9, str);

		//set the total values
		str.Format("%d", iBankTo + iOthersTo);
		SetItemText(iRow + i, 10, str);
		str.Format("%d", iBankFrom + iOthersFrom);
		SetItemText(iRow + i, 11, str);
		str.Format("%.2f", (iBankTo + iOthersTo) ? (float) (iBankFrom + iOthersFrom) / (float) (iBankTo + iOthersTo) : 0.00);
		SetItemText(iRow + i, 12, str);
	}

	//unlock updates
	SetRedraw(TRUE);
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// sums part of a 2d array
/////////////////////////////////////////////////////////////////////////////
int CStatGrid::sumArray(int array[6][5], int j)
{
	int i;
	int iAmount = 0;

	//run through the number of players
	for(i = 0; i < m_iPlayers; i++)
	{
		//sum it
		iAmount += array[i][j];
	}

	//return the amount
	return iAmount;
}

/////////////////////////////////////////////////////////////////////////////
// update a double stat block
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::updateDoubleBlock(int iType)
{
	int i, j, k;
	int iRow;
	int iAmount = 0;
	int iValue = 0;
	int iTotal = 0;
	int iSumTotal[5];
	int iPlayerTotal[5];
	CString str;
	CString strTemp;
	CString strStolen;
	CPlayerGame *pGame;

	//lock updates until we're done
	SetRedraw(FALSE);

	//figure out the starting row 
	iRow = m_iJump[iType] + 3;

	//reset counts
	::ZeroMemory(iSumTotal, sizeof(iSumTotal));

	//run through the list and calculate
	for(i = 0; i < m_iPlayers; i++)
	{
		//set the pointer
		pGame = &(GAME->m_players[i]);

		//reset player total
		::ZeroMemory(iPlayerTotal, sizeof(iPlayerTotal));

		//run through each resource type
		for(j = 0, iTotal = 0; j < 5; j++)
		{
			//get the sum amount
			switch(iType)
			{
			case STAT_GOODS_RECV_STEAL:	iAmount = sumArray(pGame->m_iResStolen, j); break;
			case STAT_GOODS_LOST_BOARD: iAmount = sumArray(pGame->m_iLostBoard, j); break;
			case STAT_GOODS_LOST_STEAL: iAmount = sumArray(pGame->m_iLostStolen, j); break;
			case STAT_GOODS_LOST_TOTAL: iAmount = sumArray(pGame->m_iLostBoard, j) + sumArray(pGame->m_iLostStolen, j) + pGame->m_iLostRoll[j]; break;
			}

			//add to the sum total (used later)
			iSumTotal[j] += iAmount;
			iPlayerTotal[j] += iAmount;

			//add to the row total
			iTotal += iAmount;

			//create the string
			str.Format("%d", iPlayerTotal[j]);

			//set it
			SetItemText(iRow + i, 4 + j, str);
		}

		//set the total value
		str.Format("%d", iTotal);

		//set it
		SetItemText(iRow + i, 4 + j, str);

		//create the strings
		for(j = 0, strStolen = ""; j < m_iPlayers; j++)
		{
			//reset the temp string
			strTemp = "";

			//run through the resources
			for(k = 0; k < 5; k++)
			{
				switch(iType)
				{
				case STAT_GOODS_RECV_STEAL: iValue = pGame->m_iResStolen[j][k]; break;
				case STAT_GOODS_LOST_BOARD: iValue = pGame->m_iLostBoard[j][k]; break;
				case STAT_GOODS_LOST_STEAL: iValue = pGame->m_iLostStolen[j][k]; break;
				case STAT_GOODS_LOST_TOTAL: iValue = pGame->m_iLostBoard[j][k] + pGame->m_iLostStolen[j][k]; break;
				}

				//see if we stole from this player
				if(iValue)
				{
					str.Format("%d %s, ", iValue, VIEW->getResName(k, TRUE));

					//add it
					strTemp += str;
				}
			}

			//clean up
			if(!strTemp.IsEmpty())
			{
				//get rid of the last comma
				strStolen += strTemp.Left(strTemp.GetLength() - 2);

				//add the from
				switch(iType)
				{
				case STAT_GOODS_RECV_STEAL: strStolen += " from "; break;
				case STAT_GOODS_LOST_BOARD: 
				case STAT_GOODS_LOST_STEAL: 
				case STAT_GOODS_LOST_TOTAL: strStolen +=  " to "; break;
				}

				//add the player name
				strStolen += GAME->m_players[j].m_player.getFirstName();
				strStolen += "; ";
			}
		}

		//get rid of the last ; and place it
		if(!strStolen.IsEmpty())
		{
			//lose the ;
			strStolen = strStolen.Left(strStolen.GetLength() - 2);
		}
		
		//add it
		SetItemText(iRow + i, 11, strStolen);
		SetItemCovered(iRow + i, 11, TRUE);
	}

	//run through and update the totals
	for(j = 0, iTotal = 0; j < 5; j++)
	{
		//format the string
		str.Format("%d", iSumTotal[j]);

		//set it
		SetItemText(iRow + i, 4 + j, str);

		//get the grand total
		iTotal += iSumTotal[j];
	}

	//set the grand total
	str.Format("%d", iTotal);

	//set it
	SetItemText(iRow + i, 4 + j, str);

	//update the total goods received
	if(iType == STAT_GOODS_RECV_STEAL)
	{
		updateSingleBlock(STAT_GOODS_RECV_TOTAL, 3);
	}
	else if(iType != STAT_GOODS_LOST_TOTAL)
	{
		updateDoubleBlock(STAT_GOODS_LOST_TOTAL);
	}

	//unlock updates
	SetRedraw(TRUE);
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// update a single stat block
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::updateSingleBlock(int iType, int iCol)
{
	int i, j;
	int iRow;
	int iAmount = 0;
	int iTotal = 0;
	int iSumTotal[5];
	CString str;
	CPlayerGame *pGame;

	//lock updates until we're done
	SetRedraw(FALSE);

	//figure out the starting row for goods per player
	iRow = m_iJump[iType] + 3;

	//reset counts
	::ZeroMemory(iSumTotal, sizeof(iSumTotal));

	//run through the list and calculate
	for(i = 0; i < m_iPlayers; i++)
	{
		//set the pointer
		pGame = &(GAME->m_players[i]);

		//run through each resource type
		for(j = 0, iTotal = 0; j < 5; j++)
		{
			//get the amount
			switch(iType)
			{
			case STAT_GOODS_RECV_BOARD: iAmount = pGame->m_iResBoard[j]; break;
			case STAT_GOODS_RECV_DEVCARD: iAmount = pGame->m_iResDev[j]; break;
			case STAT_GOODS_RECV_DISCOVER: iAmount = pGame->m_iResDiscover[j]; break;
			case STAT_GOODS_RECV_GOLD: iAmount = pGame->m_iResGold[j]; break;
			case STAT_GOODS_LOST_7: iAmount = pGame->m_iLostRoll[j]; break;
			case STAT_GOODS_RECV_TOTAL:
				iAmount = pGame->m_iResBoard[j] + pGame->m_iResDev[j] + pGame->m_iResGold[j] + pGame->m_iResDiscover[j] + sumArray(pGame->m_iResStolen, j); break;
			case STAT_TRADE_TO_BANK: iAmount = pGame->m_iBankTradeTo[j]; break;
			case STAT_TRADE_FROM_BANK: iAmount = pGame->m_iBankTradeFrom[j]; break;
			case STAT_TRADE_TO_OTHERS: iAmount = sumArray(pGame->m_iOtherTradeTo, j); break;
			case STAT_TRADE_FROM_OTHERS: iAmount = sumArray(pGame->m_iOtherTradeFrom, j); break;
			}

			//add to the sum total (used later)
			iSumTotal[j] += iAmount;

			//add to the row total
			iTotal += iAmount;

			//create the string
			str.Format("%d", iAmount);

			//set it
			SetItemText(iRow + i, iCol + 1 + j, str);
		}

		//set the total value
		str.Format("%d", iTotal);

		//set it
		SetItemText(iRow + i, iCol + 1 + j, str);
	}

	//run through and update
	for(j = 0, iTotal = 0; j < 5; j++)
	{
		//format the string
		str.Format("%d", iSumTotal[j]);

		//set it
		SetItemText(iRow + i, iCol + 1 + j, str);

		//get the grand total
		iTotal += iSumTotal[j];
	}

	//set the grand total
	str.Format("%d", iTotal);

	//set it
	SetItemText(iRow + i, iCol + 1 + j, str);

	//if it's not the total grid, update the total grid
	if(iType == STAT_GOODS_RECV_BOARD || iType == STAT_GOODS_RECV_DEVCARD || iType == STAT_GOODS_RECV_GOLD || iType == STAT_GOODS_RECV_DISCOVER)
	{
		updateSingleBlock(STAT_GOODS_RECV_TOTAL, 3);
	}
	else if(iType == STAT_GOODS_LOST_7)
	{
		updateDoubleBlock(STAT_GOODS_LOST_TOTAL);
	}

	//unlock updates
	SetRedraw(TRUE);
	Invalidate();
}
