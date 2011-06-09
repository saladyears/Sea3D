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
#include "defineStat.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatGrid

CStatGrid::CStatGrid()
{
	//set the base font
	m_font = Verdana12;
	m_titlefont = Verdana13B;
	m_headerfont = Verdana14B;

	//unitialized
	m_bInit = FALSE;
}

CStatGrid::~CStatGrid()
{
}


BEGIN_MESSAGE_MAP(CStatGrid, CGridCtrl)
	//{{AFX_MSG_MAP(CStatGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStatGrid message handlers

/////////////////////////////////////////////////////////////////////////////
// basic initialization
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::Initialize()
{
	//set font
	GetDefaultCell(FALSE, FALSE)->SetFont((LOGFONT *) &m_font);
	GetDefaultCell(FALSE, FALSE)->SetFormat(DT_CENTER | DT_VCENTER);

	//turn off grid lines
	GetDefaultCell(FALSE, FALSE)->SetGridLines(GVL_NONE);

	//set default line color
	SetGridLineColor(COLOR_BLACK);

	//set counts, which will be changed later
	SetRowCount(400);
	SetColumnCount(40);
	
	//use double buffering
	SetDoubleBuffering();

	//disable title tips
	EnableTitleTips(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// handle game data initialization
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::init()
{
	int i;
	int iRow;

	//clear the contents
	SetRowCount(0);
	SetRowCount(400);

	//set the starting row
	iRow = 2;

	//get the number of players
	m_iPlayers = GAME->m_players.size();

	//resize the jump array
	m_iJump.resize(STAT_SIZE);

	//create all the blocks
	for(i = 0; i < STAT_SIZE; i++)
	{
		createBlock(i, iRow);
	}

	//resize column widts
	AutoSizeColumns();

	//we've initialized
	m_bInit = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// handle updating a stat block
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::update(int iBlock)
{
	//update the block
	switch(iBlock)
	{
	case STAT_ROLL_FREQ: updateRollFreq(); break;
	case STAT_TIME_TURN: updateTime(); break;
	case STAT_GOODS_RECV_BOARD:		updateSingleBlock(STAT_GOODS_RECV_BOARD, 3); break;
	case STAT_GOODS_RECV_STEAL:		updateDoubleBlock(STAT_GOODS_RECV_STEAL); break;
	case STAT_GOODS_RECV_DEVCARD:	updateSingleBlock(STAT_GOODS_RECV_DEVCARD, 3); break;
	case STAT_GOODS_RECV_DISCOVER:	updateSingleBlock(STAT_GOODS_RECV_DISCOVER, 3); break;
	case STAT_GOODS_RECV_GOLD:		updateSingleBlock(STAT_GOODS_RECV_GOLD, 3); break;
	case STAT_GOODS_LOST_BOARD:		updateDoubleBlock(STAT_GOODS_LOST_BOARD); break;
	case STAT_GOODS_LOST_STEAL:		updateDoubleBlock(STAT_GOODS_LOST_STEAL); break;
	case STAT_GOODS_LOST_7:			updateSingleBlock(STAT_GOODS_LOST_7, 3); break;
	case STAT_TRADE_NUMBER: 
		updateTradeNo(); 
		updateTradePartners();
		updateTradeRatios();
		updateSingleBlock(STAT_TRADE_TO_BANK, 3); 
		updateSingleBlock(STAT_TRADE_FROM_BANK, 3); 
		updateSingleBlock(STAT_TRADE_TO_OTHERS, 3); 
		updateSingleBlock(STAT_TRADE_FROM_OTHERS, 3); break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// create all the stat blocks
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::createBlock(int iBlock, int &iRow)
{
	//set the jump point for this block
	m_iJump[iBlock] = iRow;

	//create the block
	switch(iBlock)
	{
	case STAT_HEAD_ROLL: createHeader(STAT_HEAD_ROLL, iRow); break;
	case STAT_ROLL_FREQ: createRollFreq(iRow); break;
	case STAT_ROLL_PLAYER: createRollPlayer(iRow); break;
	case STAT_HEAD_TIME: createHeader(STAT_HEAD_TIME, iRow); break;
	case STAT_TIME_TURN: createTimeTurn(iRow); break;
	case STAT_HEAD_GOODS_RECV: createHeader(STAT_HEAD_GOODS_RECV, iRow); break;
	case STAT_GOODS_RECV_TOTAL: createGoodsTable(iRow, STAT_GOODS_RECV_TOTAL); break;
	case STAT_GOODS_RECV_BOARD: createGoodsTable(iRow, STAT_GOODS_RECV_BOARD); break;
	case STAT_GOODS_RECV_STEAL: createGoodsTable(iRow, STAT_GOODS_RECV_STEAL); break;
	case STAT_GOODS_RECV_DEVCARD: createGoodsTable(iRow, STAT_GOODS_RECV_DEVCARD); break;
	case STAT_GOODS_RECV_DISCOVER: createGoodsTable(iRow, STAT_GOODS_RECV_DISCOVER); break;
	case STAT_GOODS_RECV_GOLD: createGoodsTable(iRow, STAT_GOODS_RECV_GOLD); break;
	case STAT_HEAD_GOODS_LOST: createHeader(STAT_HEAD_GOODS_LOST, iRow); break;
	case STAT_GOODS_LOST_TOTAL:	createGoodsTable(iRow, STAT_GOODS_LOST_TOTAL); break;
	case STAT_GOODS_LOST_BOARD:	createGoodsTable(iRow, STAT_GOODS_LOST_BOARD); break;
	case STAT_GOODS_LOST_STEAL:	createGoodsTable(iRow, STAT_GOODS_LOST_STEAL); break;
	case STAT_GOODS_LOST_7:	createGoodsTable(iRow, STAT_GOODS_LOST_7); break;
	case STAT_HEAD_TRADES: createHeader(STAT_HEAD_TRADES, iRow); break;
	case STAT_TRADE_NUMBER: createTradeNo(iRow); break;
	case STAT_TRADE_RATIO: createTradeRatios(iRow); break;
	case STAT_TRADE_TO_BANK: createGoodsTable(iRow, STAT_TRADE_TO_BANK); break;
	case STAT_TRADE_FROM_BANK: createGoodsTable(iRow, STAT_TRADE_FROM_BANK); break;
	case STAT_TRADE_NUM_OTHERS: createTradePartners(iRow); break;
	case STAT_TRADE_TO_OTHERS: createGoodsTable(iRow, STAT_TRADE_TO_OTHERS); break;
	case STAT_TRADE_FROM_OTHERS: createGoodsTable(iRow, STAT_TRADE_FROM_OTHERS); break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// draw an outline for a given range
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::outline(int iT, int iL, int iB, int iR)
{
	int i, j;

	//fill interior title
	fill(iT, iL, iT, iR, COLOR_LTGRAY);

	//draw inner boxes
	for(i = iT; i <= iB; i++)
	{
		for(j = iL; j <= iR + 1; j++)
		{
			SetItemGridLines(i, j, GVL_VERT);
		}
	}

	//draw top frame
	for(i = iT; i <= iT + 1; i++)
	{
		for(j = iL; j <= iR; j++)
		{
			SetItemGridLines(i, j, GVL_BOTH);
		}
	}

	//draw bottom
	for(j = iL; j <= iR; j++)
	{
		SetItemGridLines(iB + 1, j, GVL_HORZ);
	}
}

/////////////////////////////////////////////////////////////////////////////
// color fill
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::fill(int iT, int iL, int iB, int iR, COLORREF color)
{
	int i, j;

	//run through
	for(i = iT; i  <= iB; i++)
	{
		for(j = iL; j <= iR; j++)
		{
			SetItemBkColour(i, j, color);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// create a header
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::createHeader(int iType, int &iRow)
{
	int iCol = 2;
	BOOL bHead;

	//create the header
	SetItemFont(iRow, iCol, (LOGFONT *) &m_headerfont);
	SetItemFormat(iRow, iCol, DT_LEFT);
	SetItemText(iRow, iCol, ((CStatsDialog *) GetParent())->getName(iType, bHead));
	SetItemCovered(iRow, iCol, TRUE);

	//increment the row
	iRow += 2;
}

/////////////////////////////////////////////////////////////////////////////
// create a goods header
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::createGoodsHeader(int iRow, int iCol)
{
	int i;

	//set the column headers
	SetItemFormat(iRow, iCol, DT_LEFT);
	SetItemText(iRow, iCol++, "Player");

	//run through the names
	for(i = 0; i < 5; i++)
	{
		SetItemText(iRow, iCol++, VIEW->getResName(i));
	}

	SetItemText(iRow, iCol++, "Total");
}

/////////////////////////////////////////////////////////////////////////////
// create a title
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::createTitle(int iType, int &iRow)
{
	int iCol = 3;
	BOOL bHead;

	//write out the column headers
	SetItemFont(iRow, iCol, (LOGFONT *) &m_titlefont);
	SetItemFormat(iRow, iCol, DT_LEFT);
	SetItemText(iRow, iCol, ((CStatsDialog *) GetParent())->getName(iType, bHead));
	SetItemCovered(iRow, iCol, TRUE);

	//give a space
	iRow += 2;
}

/////////////////////////////////////////////////////////////////////////////
// jump to a point in the code
/////////////////////////////////////////////////////////////////////////////
void CStatGrid::jumpTo(int iBlock)
{
	//only scroll if we've initialized
	if(m_bInit)
	{
		EnsureVisible(399, 0);
		EnsureVisible(m_iJump[iBlock] - 1, 0);
	}
}