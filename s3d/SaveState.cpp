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
#include "SaveState.h"
#include "Settlers.h"
#include "SettlersView.h"

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CSaveState::CSaveState()
{
	resetState();
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CSaveState::~CSaveState()
{
}

//////////////////////////////////////////////////////////////////////
// reset state variables
//////////////////////////////////////////////////////////////////////
void CSaveState::resetState()
{
	//controls buttons
	m_bControls = FALSE;
	m_bPlay = FALSE;
	m_bBank = FALSE;
	m_bTrade = FALSE;
	m_bEnd = FALSE;

	//message control
	m_strMessage = "";
	m_bUndo = FALSE;

	//actions control
	m_bActions = FALSE;
	m_bRoll = FALSE;
	m_bGoods = FALSE;
	m_bVolcano = FALSE;

	//build options
	m_bRoads = FALSE;
	m_bShips = FALSE;
	m_bSettlements = FALSE;
	m_bCities = FALSE;
	m_bDev = FALSE;
	m_bMove = FALSE;

	//OpenGL settings
	m_nMode = GL_WAIT;

	//save setting
	m_bSaved = FALSE;

	//resynch data
	m_nResynch = 0;
	m_nData1 = 0;
	m_nData2 = 1;
}

//////////////////////////////////////////////////////////////////////
// overloaded assignment operator
//////////////////////////////////////////////////////////////////////
CSaveState &CSaveState::operator =(const CSaveState &save)
{
	//controls buttons
	m_bControls				= save.m_bControls;
	m_bPlay					= save.m_bPlay;
	m_bBank					= save.m_bBank;
	m_bTrade				= save.m_bTrade;
	m_bEnd					= save.m_bEnd;

	//message control
	m_strMessage			= save.m_strMessage;
	m_bUndo					= save.m_bUndo;

	//actions control
	m_bActions				= save.m_bActions;
	m_bRoll					= save.m_bRoll;
	m_bGoods				= save.m_bGoods;
	m_bVolcano				= save.m_bVolcano;

	//build options
	m_bRoads				= save.m_bRoads;
	m_bShips				= save.m_bShips;
	m_bSettlements			= save.m_bSettlements;
	m_bCities				= save.m_bCities;
	m_bDev					= save.m_bDev;
	m_bMove					= save.m_bMove;

	//OpenGL settings
	m_nMode					= save.m_nMode;

	//whether we've saved the state or not
	m_bSaved				= save.m_bSaved;

	//resynch data
	m_nResynch				= save.m_nResynch;
	m_nData1				= save.m_nData1;
	m_nData2				= save.m_nData2;

	return *this;
}

//////////////////////////////////////////////////////////////////////
// save the state
//////////////////////////////////////////////////////////////////////
void CSaveState::saveState(BOOL bNoChanges)
{
	//if we're already saving, no need to resave
	if(TRUE == m_bSaved)
	{
		return;
	}

	//clear everything
	resetState();

	//controls button
	m_bControls = (CONTROLS->getOffset() == 0);
	m_bPlay = CONTROLS->Play().IsWindowEnabled();
	m_bBank = CONTROLS->Bank().IsWindowEnabled();
	m_bTrade = CONTROLS->Trade().IsWindowEnabled();
	m_bEnd = CONTROLS->End().IsWindowEnabled();

	//message window
	m_strMessage = MESSAGES->Title().GetText();
	m_bUndo = MESSAGES->undoEnabled();

	//actions button
	m_bActions = (ACTIONS->getOffset() == 0);
	m_bRoll = ACTIONS->Roll().IsWindowVisible();
	m_bGoods = ACTIONS->Goods().IsWindowVisible();
	m_bVolcano = ACTIONS->Volcano().IsWindowVisible();

	//build options
	m_bRoads = VIEW->getTinyR()->IsWindowVisible();
	m_bShips = VIEW->getTinySh()->IsWindowVisible();
	m_bSettlements = VIEW->getTinyS()->IsWindowVisible();
	m_bCities = VIEW->getTinyC()->IsWindowVisible();
	m_bDev = VIEW->getTinyDev()->IsWindowVisible();
	m_bMove = VIEW->getTinyMove()->IsWindowVisible();

	//OpenGL settings
	m_nMode = VIEW->getMode();

	//hide controls and actions
	if(FALSE == bNoChanges)
	{
		hideAll();
	}

	//we have now saved the state
	m_bSaved = TRUE;
}

//////////////////////////////////////////////////////////////////////
// hide all objects
//////////////////////////////////////////////////////////////////////
void CSaveState::hideAll(void)
{
	CONTROLS->ShowWindow(SW_HIDE);
	CONTROLS->Play().EnableWindow(FALSE);
	CONTROLS->Bank().EnableWindow(FALSE);
	CONTROLS->Trade().EnableWindow(FALSE);
	CONTROLS->End().EnableWindow(FALSE);
	ACTIONS->ShowWindow(SW_HIDE);
	ACTIONS->Roll().ShowWindow(SW_HIDE);
	ACTIONS->Goods().ShowWindow(SW_HIDE);
	ACTIONS->Volcano().ShowWindow(SW_HIDE);

	//hide build options
	VIEW->getTinyR()->ShowWindow(SW_HIDE);
	VIEW->getTinySh()->ShowWindow(SW_HIDE);
	VIEW->getTinyS()->ShowWindow(SW_HIDE);
	VIEW->getTinyC()->ShowWindow(SW_HIDE);
	VIEW->getTinyDev()->ShowWindow(SW_HIDE);
	VIEW->getTinyMove()->ShowWindow(SW_HIDE);

	//set gl mode
	VIEW->setDrawingMode(GL_WAIT);
}

//////////////////////////////////////////////////////////////////////
// handle resynch data
//////////////////////////////////////////////////////////////////////
void CSaveState::saveResynch(int nResynch, int nData1, int nData2)
{
	m_nResynch = nResynch;
	m_nData1 = nData1;
	m_nData2 = nData2;
}

//////////////////////////////////////////////////////////////////////
// restore the state
//////////////////////////////////////////////////////////////////////
void CSaveState::restoreState()
{
	//only restore if we've saved
	if(FALSE == m_bSaved)
	{
		return;
	}

	//do a resynch if needed
	if(0 != m_nResynch)
	{
		VIEW->clientResynch(m_nResynch, m_nData1, m_nData2);
	}
	else
	{
		//controls button
		CONTROLS->ShowWindow(m_bControls ? SW_SHOW : SW_HIDE);
		CONTROLS->Play().EnableWindow(m_bPlay);
		CONTROLS->Bank().EnableWindow(m_bBank);
		CONTROLS->Trade().EnableWindow(m_bTrade);
		CONTROLS->End().EnableWindow(m_bEnd);

		//message window
		MESSAGES->Title().SetText(m_strMessage);
		MESSAGES->enableUndo(m_bUndo);

		//actions button
		ACTIONS->ShowWindow(m_bActions ? SW_SHOW : SW_HIDE);
		ACTIONS->Roll().ShowWindow(m_bRoll ? SW_SHOW : SW_HIDE);
		ACTIONS->Goods().ShowWindow(m_bGoods ? SW_SHOW : SW_HIDE);
		ACTIONS->Volcano().ShowWindow(m_bVolcano ? SW_SHOW : SW_HIDE);

		//build options
		VIEW->getTinyR()->ShowWindow(m_bRoads ? SW_SHOW : SW_HIDE);
		VIEW->getTinySh()->ShowWindow(m_bShips ? SW_SHOW : SW_HIDE);
		VIEW->getTinyS()->ShowWindow(m_bSettlements ? SW_SHOW : SW_HIDE);
		VIEW->getTinyC()->ShowWindow(m_bCities ? SW_SHOW : SW_HIDE);
		VIEW->getTinyDev()->ShowWindow(m_bDev ? SW_SHOW : SW_HIDE);
		VIEW->getTinyMove()->ShowWindow(m_bMove ? SW_SHOW : SW_HIDE);

		//OpenGL settings
		VIEW->setDrawingMode(m_nMode);
	}

	//clear the variables
	resetState();
}