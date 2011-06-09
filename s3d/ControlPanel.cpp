/* 
 * Settlers3D - Copyright (C) 2001-2003 Jason Fugate (jfugate@settlers3d.net) 
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the Free 
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version. 
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 * for more details. 
 */

#include "stdafx.h"
#include "settlers.h"
#include "ControlPanel.h"
#include "SettlersView.h"
#include "UndoEnd.h"
#include "NetworkPlayCardDialog.h"
#include "TradeDialog.h"
#include "NetworkTradeDialog.h"
#include "TradeWithBankDialog.h"
#include "rand.h"
#include "ColorDatabase.h"
#include "defineDraw.h"
#include "defineSounds.h"
#include "defineVariants.h"
#include "defineColors.h"
#include "defineMap.h"
#include "defineIni.h"
#include "defineStat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlPanel

CControlPanel::CControlPanel()
{
	m_iPlayers = 0;

	//not in a turn yet
	m_bInTurn = FALSE;
	m_bInGoods = FALSE;
	m_bInTrade = FALSE;
}

CControlPanel::~CControlPanel()
{
}

/////////////////////////////////////////////////////////////////////////////
// initialize the text
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::initControl(void)
{
	int i;
	CString str;

	//load the string here
	if(GAME_NEW == GAME->m_iState)
	{
		//set the color
		updateColor(0);

		str.Format("Place a settlement for %s.", GAME->m_players[0].m_player.getName());

		//toggle controls
		CONTROLS->ShowWindow(SW_HIDE);

		//toggle actions off
		ACTIONS->ShowWindow(SW_HIDE);
	}
	else
	{
		str = "This game has ended.";

		//disable the undo button
		MESSAGES->enableUndo(FALSE);
	}

	//set the number of players
	m_iPlayers = GAME->m_players.size();

	//init the string array
	m_strLines.resize(m_iPlayers);
	m_strVolcano.resize(m_iPlayers);
	
	//set the background colors
	for(i = 0; i < m_iPlayers; i++)
	{
		//set the background color
		ACTIONS->Goods().setBackground(i, COLORS->getTextColor(GAME->m_players[i].m_iColor));
		ACTIONS->Volcano().setBackground(i, COLORS->getTextColor(GAME->m_players[i].m_iColor));
	}

	MESSAGES->Title().SetText(str);

	//reset the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// we're now in a special build phase
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::specialBuildPhase()
{
	int i;
	int iPlayer;
	BOOL bHotseat;
	BOOL bAutohide;
	CString str;

	//set the text
	str = "Special Build Phase for ";

	//get the current player
	iPlayer = VIEW->getPlayer();

	//get the name
	str += GAME->m_players[iPlayer].m_player.getFirstName();
	str += ".";

	//save the current setting
	bHotseat = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE_OTHER, FALSE, INI_FILE);
	bAutohide = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE, FALSE, INI_FILE);

	//write out what we want
	WritePrivateProfileString(INI_DISPLAY, INI_AUTOHIDE_OTHER, "0", INI_FILE);
	WritePrivateProfileString(INI_DISPLAY, INI_AUTOHIDE, "1", INI_FILE);

	//update the players
	for(i = 0; i < (int) GAME->m_players.size(); i++)
	{
		VIEW->updateAutohide();
	}

	MessageBox(NULL, str, "", MB_OK);

	//write it as what it was
	WritePrivateProfileString(INI_DISPLAY, INI_AUTOHIDE_OTHER, bHotseat ? "1" : "0", INI_FILE);
	WritePrivateProfileString(INI_DISPLAY, INI_AUTOHIDE, bAutohide ? "1" : "0", INI_FILE);

	//update the players
	for(i = 0; i < (int) GAME->m_players.size(); i++)
	{
		VIEW->updateAutohide();
	}

	//update player displays
	VIEW->updatePlayerDisplay();

	//handle common
	handleCommonSpecialBuild(str);
}

/////////////////////////////////////////////////////////////////////////////
// handle common special build phase stuff
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::handleCommonSpecialBuild(CString str)
{
	//set the background color
	updateColor(VIEW->getPlayer());

	//show build options
	VIEW->showBuildOptions(TRUE);

	//set the text
	MESSAGES->Title().SetText(str);

	//enable/disable everything
	EnableDC();
	EnableTrade();
	EnableEndTurn();

	//reset the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// update when called
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::updateText()
{
	//update depending on mode
	switch(VIEW->getMode())
	{
	case GL_PLACE_PLAYERS: updatePlace(); break;
	case GL_PLACE_ROBBER: updateRobber(); break;
	case GL_PLACE_PORT: updatePort(); break;
	case GL_GAME_OVER: MESSAGES->Title().SetText("This game has ended."); break;
	case GL_CHITLESS_TILE: MESSAGES->Title().SetText("Choose a roll chit to take."); break;
	case GL_PLACE_THIRD_ROAD: updateThirdRoad(); break;
	}

	//set the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// set the text correctly
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::updateThirdRoad()
{
	int iPlayer;
	CString str;

	//set initial
	str = "Place a road for ";

	//get the current player
	iPlayer = VIEW->getPlayer();

	//set the background color
	updateColor(iPlayer);

	//get the name
	str += GAME->m_players[iPlayer].m_player.getName();
	str += ".";

	//set the text
	MESSAGES->Title().SetText(str);
}

/////////////////////////////////////////////////////////////////////////////
// set the text correctly
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::updatePlace(void)
{
	int iPlayer;
	CString str;

	//see what text we need to get
	if(TRUE == VIEW->getCityPlace())
	{
		if((IS_VARIANT(VARIANT_TOURNAMENT_START)) && (FALSE == VIEW->getIncrease()))
		{
			str = "Place a city for ";
		}
		else
		{
			str = "Place a settlement for ";
		}
	}
	else
	{
		if(TRUE == VIEW->getShipPlace())
		{
			str = "Place a ship for ";
		}
		else
		{
			str = "Place a road for ";
		}
	}

	//get the current player
	iPlayer = VIEW->getPlayer();

	//set the background color
	updateColor(iPlayer);

	//get the name
	str += GAME->m_players[iPlayer].m_player.getName();
	str += ".";

	//set the text
	MESSAGES->Title().SetText(str);
}

/////////////////////////////////////////////////////////////////////////////
// set text
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::updateColor(int iPlayer)
{
	COLORREF color;

	//set the background color of the info text
	color = COLORS->getTextColor(GAME->m_players[iPlayer].m_iColor);
	MESSAGES->Title().SetColors(color, color);
}

/////////////////////////////////////////////////////////////////////////////
// set text
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::updateRobber()
{
	if(MAP->m_iMode & MAP_SEAFARERS)
	{
		MESSAGES->Title().SetText("Choose a location for the robber or pirate.");
	}
	else
	{
		MESSAGES->Title().SetText("Choose a location for the robber.");
	}
}

/////////////////////////////////////////////////////////////////////////////
// set text
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::updatePort()
{
	int iPlayer;
	CString str;

	//get the current player
	iPlayer = VIEW->getPlayer();

	//set the background color
	updateColor(iPlayer);

	//get the name
	str = GAME->m_players[iPlayer].m_player.getName();
	str += "'s turn to place a port.";

	//set the text
	MESSAGES->Title().SetText(str);
	MESSAGES->Title().Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// set the volcano die value
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::setVolcanoRoll(int nRoll) 
{
	ACTIONS->Volcano().setDice(nRoll, -1); 
	ACTIONS->Volcano().EnableWindow(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// show or hide the roll buttons
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::showRolls(BOOL bShow)
{
	int iShow;
	int iPlayer;
	CString str;

	//get the current player
	iPlayer = VIEW->getPlayer();

	//set the show command
	iShow = (bShow ? SW_SHOW : SW_HIDE);

	//show the roll window
	ACTIONS->Roll().ShowWindow(iShow);

	//if we're showing them, set the text properly
	if(bShow)
	{
		//we cannot be in a turn if the rolls are showing
		m_bInTurn = FALSE;

		//set game status
		GAME->m_iState = GAME_PRE_TURN;

		if(GAME_MODE_NETWORK != GAME->m_iMode)
		{
			updateColor(iPlayer);

			//set the text
			str.Format("Roll for %s.", GAME->m_players[iPlayer].m_player.getName());
			MESSAGES->Title().SetText(str);
		}

		//enable the development card options
		EnableDC();
	}

	//reset the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle keyboard events
/////////////////////////////////////////////////////////////////////////////
BOOL CControlPanel::canRoll(void) 
{
	return ((FALSE == m_bInGoods) && (FALSE == m_bInTurn) &&
			(GL_PLACE_PLAYERS != VIEW->getMode()));
}

/////////////////////////////////////////////////////////////////////////////
// show or hide the roll buttons
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::showGoods(BOOL bShow)
{
	int iShow;
	CString str;

	//set the show command
	iShow = (bShow ? SW_SHOW : SW_HIDE);

	//show/hide the goods button
	ACTIONS->Goods().ShowWindow(iShow);

	//set the show goods variable
	m_bInGoods = bShow;

	//set the text
	if(TRUE == bShow)
	{
		//make sure build options are hidden when this is up
		VIEW->showBuildOptions(FALSE);

		//we're not in a turn
		m_bInTurn = FALSE;

		//set game status
		GAME->m_iState = GAME_PRE_TURN;

		//disable all buttons
		EnableDC();
		EnableTrade();
		EnableEndTurn();

		//format the text
		str.Format("Goods on %s %d.", NEEDS_AN(m_iLastDie1 + m_iLastDie2) ? "an" : "a", m_iLastDie1 + m_iLastDie2);

		//set the text
		if(GAME_MODE_NETWORK != GAME->m_iMode)
		{
			str += " (Click in box or use spacebar to continue)";
		}
		else if((7 != m_iLastDie1 + m_iLastDie2) && 
				(VIEW->getPlayer() == GAME->m_iIndex))
		{
			str = "Waiting for server to continue.";
		}

		MESSAGES->Title().SetText(str);

		///set the goods button bitmaps
		ACTIONS->Goods().setDice(m_iLastDie1 - 1, m_iLastDie2 - 1);

		//if it is network mode, disable the window until we get the continue
		//message from the server
		if(GAME_MODE_NETWORK == GAME->m_iMode)
		{
			ACTIONS->Goods().EnableWindow(FALSE);
		}
	}

	//reset the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// show or hide the action buttons
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::showActions(BOOL bShow)
{
	int iShow;

	//set the right action
	iShow = (bShow ? SW_SHOW : SW_HIDE);

	//show or hide the buttons
	CONTROLS->Trade().ShowWindow(iShow);
	CONTROLS->Play().ShowWindow(iShow);
	CONTROLS->End().ShowWindow(iShow);
	CONTROLS->Bank().ShowWindow(iShow);

	//update actions
	if(bShow)
	{
		EnableDC();

		//enable trade button
		EnableTrade();
	}
}

/////////////////////////////////////////////////////////////////////////////
// show/hide handler
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::updateGoods(int iGoods, int iPlayer, CString strLine)
{
	//if they didn't get any goods, hide their window
	if(0 == iGoods)
	{
		m_strLines[iPlayer].Empty();
		ACTIONS->Goods().showWindow(iPlayer, SW_HIDE);
	}
	else
	{
		//set it
		m_strLines[iPlayer] = strLine;
		ACTIONS->Goods().setText(iPlayer, strLine);
		ACTIONS->Goods().showWindow(iPlayer, SW_SHOW);
	}
}

/////////////////////////////////////////////////////////////////////////////
// show/hide handler
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::updateVolcano(BOOL bShow, int nPlayer, CString strLine)
{
	//if they didn't have a settlement on the volcano, hide them
	if(FALSE == bShow)
	{
		m_strVolcano[nPlayer].Empty();
		ACTIONS->Volcano().showWindow(nPlayer, SW_HIDE);
	}
	else
	{
		//set it
		m_strVolcano[nPlayer] = strLine;
		ACTIONS->Volcano().setText(nPlayer, strLine);
		ACTIONS->Volcano().showWindow(nPlayer, SW_SHOW);
	}
}

/////////////////////////////////////////////////////////////////////////////
// roll the dice, baby!
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::OnRollButton()
{
	int iTurns;
	int iDie1 = 0, iDie2 = 0;
	BOOL bDone = FALSE;
	BOOL bNoSevens;
	BOOL bIsSeven = FALSE;

	//continue the tour
	if(TRUE == APP->inTour())
	{
		VIEW->closeBalloon();
	}

	//play the sound
	SOUND(SOUND_ROLL_DICE);

	//calculate turns
	iTurns = GAME->m_iRollList.size();

	//determine if we're still in the time period for no 7's
	bNoSevens = (iTurns < (m_iPlayers * VARIANTS->m_nNoSevens));

	while(FALSE == bDone)
	{
		//roll the dice!
		if(IS_VARIANT(VARIANT_EQUAL_ODDS))
		{
			generateEqualOdds(iDie1, iDie2);
		}
		else
		{
			generateDiceRoll(iDie1, iDie2);
		}

		//see if we got a seven
		if(7 == iDie1 + iDie2)
		{
			bIsSeven = TRUE;
		}
		else
		{
			bIsSeven = FALSE;
		}

		if(FALSE == IS_VARIANT(VARIANT_NO_7_FIRST_ROUND))
		{
			bDone = TRUE;
		}
		//otherwise, to keep rolling, we must be in the no 7's period and have just
		//rolled a seven
		else
		{
			bDone = !(bNoSevens && bIsSeven);
		}
	}

	//tell the others if we're network
	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		VIEW->clientRoll(iDie1, iDie2);
	}

	//handle it
	handleCommonRoll(iDie1, iDie2);

	//tell the others if we're network and it's not a seven
	if((GAME_MODE_NETWORK == GAME->m_iMode) && (7 != iDie1 + iDie2))
	{
		//if we're in seafarers mode, we need to tell the server if we 
		//got gold or not
		VIEW->clientWaitForGold(VIEW->getsGold());
	}

	//set the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// generate a dice roll
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::generateDiceRoll(int &iDie1, int &iDie2)
{
	//get the die rolls
	iDie1 = int((RAND * 6.0) + 1.0);
	iDie2 = int((RAND * 6.0) + 1.0);

	//clamp high rolls
	if(iDie1 > 6) iDie1 = 6;
	if(iDie2 > 6) iDie2 = 6;
}

/////////////////////////////////////////////////////////////////////////////
// generate equal odds on a roll
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::generateEqualOdds(int &iDie1, int &iDie2)
{
	int iLow = 0, iHigh = 0;
	int iRoll;
	int iTemp;

	//get the die roll
	iRoll = int((RAND * 11.0) + 2.0);

	//clamp roll
	if(iRoll > 12) iRoll = 12;

	//auto set the dice
	switch(iRoll)
	{
	case 12: iLow = 6; iHigh = 6; break;
	case 11: iLow = 5; iHigh = 6; break;
	case 10: iLow = 4; iHigh = 6; break;
	case 9: iLow = 3; iHigh = 6; break;
	case 8: iLow = 2; iHigh = 6; break;
	case 7: iLow = 1; iHigh = 6; break;
	case 6: iLow = 1; iHigh = 5; break;
	case 5: iLow = 1; iHigh = 4; break;
	case 4: iLow = 1; iHigh = 3; break;
	case 3: iLow = 1; iHigh = 2; break;
	case 2: iLow = 1; iHigh = 1; break;
	}

	//determine the first dice
	iDie1 = iLow + (int(RAND * double(iHigh - iLow)));

	//clamp
	if(iDie1 > (iLow + (iHigh - iLow))) iDie1 = iLow + (iHigh - iLow);

	//calculate second die
	iDie2 = iRoll - iDie1;

	//see if we should switch
	if(0 < int(RAND * 2.0))
	{
		iTemp = iDie2;
		iDie2 = iDie1;
		iDie1 = iTemp;
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle what's common in a roll
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::handleCommonRoll(int iDie1, int iDie2, BOOL bFirst)
{
	int iRoll;	

	//calculate roll
	iRoll = iDie1 + iDie2;

	//set the last roll
	m_iLastDie1 = iDie1;
	m_iLastDie2 = iDie2;

	//enable/disable the goods window
	ACTIONS->Goods().EnableWindow(iRoll != 7);

	//hide roll buttons
	showRolls(FALSE);

	//show goods received (including dice)
	showGoods(TRUE);

	//add it to the game's list
	GAME->m_iRollList.push_back(iRoll);
	GAME->m_iRolls[VIEW->getPlayer()][iRoll - 2]++;

	//update the stats
	STAT->update(STAT_ROLL_FREQ);

	//we've started a turn
	if(7 == iRoll)
	{
		//if we're in tracking mode, hide the goods window
		//showGoods(FALSE);
		m_bInGoods = FALSE;

		if(GAME_MODE_NETWORK != GAME->m_iMode)
		{
			m_bInTurn = TRUE;
		}

		//set game status
		GAME->m_iState = GAME_IN_TURN;
	}

	//tell GL
	if(7 == iRoll && TRUE == bFirst)
	{
		VIEW->handleSeven(iDie1, iDie2);
	}
	else
	{
		VIEW->handleRoll(iRoll);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on the goods
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::OnGoodsButton()
{
	int iPlayer;

	//continue the tour
	if(TRUE == APP->inTour())
	{
		VIEW->closeBalloon();
	}

	//get current player
	iPlayer = VIEW->getPlayer();

	//make sure we are this player
	if((GAME_MODE_NETWORK == GAME->m_iMode) && (iPlayer != GAME->m_iIndex))
	{
		return;
	}

	//handle common goods
	handleCommonGoods();

	//set the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on the volcano button
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::OnVolcanoButton()
{
	int iPlayer;

	//get current player
	iPlayer = VIEW->getPlayer();

	//make sure we are this player
	if((GAME_MODE_NETWORK == GAME->m_iMode) && (iPlayer != GAME->m_iIndex))
	{
		return;
	}

	//handle common goods
	handleCommonVolcano();

	//set the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle AI goods
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::OnAIGoods()
{
	//handle common goods
	handleCommonGoods();
}

/////////////////////////////////////////////////////////////////////////////
// handle common goods code
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::handleCommonGoods()
{
	int nRoll;
	int nOwner;
	vector <int> nDestruction;

	//check to see if a volcano has been rolled
	if(FALSE == VIEW->checkForVolcano())
	{
		//add the beginning placeholder
		VIEW->addUndo(UNDO_ACTION_START, new CUndoAction((m_iLastDie1 << 16) + m_iLastDie2, FALSE, m_strLines));

		//hide goods windows
		showGoods(FALSE);

		//toggle actions window
		ACTIONS->ShowWindow(SW_HIDE);

		//move on to the next view mode
		if(GAME_MODE_NETWORK == GAME->m_iMode)
		{
			//set next drawing mode
			VIEW->setNetDrawMode(GL_WAIT);

			//tell others
			VIEW->clientHideGoods();
		}
		else
		{
			VIEW->setNextMode();
		}

		//enter into a turn
		enterTurn();
	}
	else
	{
		//roll the d6
		nRoll = int(RAND * 6.0);

		//sanity
		if(6 <= nRoll)
		{
			nRoll = 6;
		}

		//tell others
		if(GAME_MODE_NETWORK == GAME->m_iMode)
		{
			//tell others
			VIEW->clientVolcano(nRoll);
		}

		//hide the goods
		showGoods(FALSE);

		//set the volcano button
		ACTIONS->Volcano().setDice(nRoll, -1);
		ACTIONS->Volcano().EnableWindow();

		//run the volcano
		nOwner = VIEW->volcanicEruption(nRoll, nDestruction);

		//add the undo
		VIEW->addUndo(UNDO_VOLCANO, new CUndoVolcano((m_iLastDie1 << 16) + m_iLastDie2, nOwner, nDestruction, m_strLines));

		//set last die
		m_nLastVolcano = nRoll;

		//show volcano
		showVolcano(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a volcanic eruption
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::handleCommonVolcano(void)
{
	//add the beginning placeholder
	VIEW->addUndo(UNDO_ACTION_START, new CUndoAction(((m_iLastDie1 + m_iLastDie2) << 16) + m_nLastVolcano, TRUE, m_strVolcano));

	//hide volcano windows
	showVolcano(FALSE);

	//toggle actions window
	ACTIONS->ShowWindow(SW_HIDE);

	//move on to the next view mode
	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		//set next drawing mode
		VIEW->setNetDrawMode(GL_WAIT);

		//tell others
		VIEW->clientHideVolcano();
	}
	else
	{
		VIEW->setNextMode();
	}

	//enter into a turn
	enterTurn();
}

/////////////////////////////////////////////////////////////////////////////
// show/hide the volcano button
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::showVolcano(BOOL bShow)
{
	int iShow;
	CString str;

	//set the show command
	iShow = (bShow ? SW_SHOW : SW_HIDE);

	//show/hide the goods button
	ACTIONS->Volcano().ShowWindow(iShow);

	//set the text
	if(TRUE == bShow)
	{
		//make sure build options are hidden when this is up
		VIEW->showBuildOptions(FALSE);

		//we're not in a turn
		m_bInTurn = FALSE;

		//set game status
		GAME->m_iState = GAME_PRE_TURN;

		//disable all buttons
		EnableDC();
		EnableTrade();
		EnableEndTurn();

		//format the text
		str = "Volcano!";

		//set the text
		if((GAME_MODE_NETWORK != GAME->m_iMode) || 
			(VIEW->getPlayer() == GAME->m_iIndex))
		{
			str += " (Click in box or use spacebar to continue)";
		}

		MESSAGES->Title().SetText(str);
	}

	//reset the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on the title button
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::OnTitleButton()
{
	//only do something if we need to
	if(m_bInGoods)
	{
		OnGoodsButton();
	}
}

/////////////////////////////////////////////////////////////////////////////
// enter a turn
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::enterTurn()
{
	//now we are in a turn
	m_bInTurn = TRUE;

	//set game status
	GAME->m_iState = GAME_IN_TURN;

	//update the turn text
	updateTurn();
}

/////////////////////////////////////////////////////////////////////////////
// set the title text
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::updateTurn()
{
	int iPlayer;
	BOOL bAI;
	CString str;

	//get the current player
	iPlayer = VIEW->getPlayer();

	//determine AIness
	bAI = GAME->m_players[iPlayer].m_player.getAI();

	//show the build options
	if(GAME_MODE_NETWORK != GAME->m_iMode || iPlayer == GAME->m_iIndex)
	{
		VIEW->showBuildOptions(TRUE);
	}

	//set the next text
	if(GAME_MODE_NETWORK != GAME->m_iMode)
	{
		str.Format("%s's turn.", GAME->m_players[iPlayer].m_player.getName());
		MESSAGES->Title().SetText(str);
	}
	else if(FALSE == bAI)
	{
		//set the title text
		MESSAGES->Title().SetText("It is your turn.");
	}

	//enable trading and dev cards
	EnableTrade();
	EnableDC();
	EnableEndTurn();

	//reset the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// return to a turn after undoing
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::returnToTurn()
{
	int iPlayer;
	CString str;

	//now we are in a turn
	m_bInTurn = TRUE;

	//set game status
	GAME->m_iState = GAME_IN_TURN;

	//make sure rolls are hidden
	showRolls(FALSE);

	//toggle actions off
	ACTIONS->ShowWindow(SW_HIDE);

	//show the build options
	VIEW->showBuildOptions(TRUE);

	//get the current player
	iPlayer = VIEW->getPlayer();

	//update the colors
	updateColor(iPlayer);

	//set the next text
	str.Format("%s's turn.", GAME->m_players[iPlayer].m_player.getName());
	MESSAGES->Title().SetText(str);

	//enable trading and dev cards
	EnableTrade();
	EnableDC();
	EnableEndTurn();
	
	//reset the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// continue the turn after receiving the all clear from the server
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::continueTurn()
{
	CString str;
	
	//format the text
	str.Format("Goods on %s %d.", NEEDS_AN(m_iLastDie1 + m_iLastDie2) ? "an" : "a", m_iLastDie1 + m_iLastDie2);

	//add the continue text
	str += " (Click in box or use spacebar to continue)";
	
	//set the title text
	MESSAGES->Title().SetText(str);

	///set the goods button bitmaps
	ACTIONS->Goods().setDice(m_iLastDie1 - 1, m_iLastDie2 - 1);

	//enable the window
	ACTIONS->Goods().EnableWindow();

	//refresh
	ACTIONS->Goods().Invalidate();

	//reset the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// enable/disable dev card buttons
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::EnableDC()
{
	int iCards = 0;
	int iImage;
	int iPlayer;
	BOOL bAI;
	CDC *dc;
	HDC hDC;

	//get the dc
	dc = CONTROLS->GetDC();
	hDC = dc->GetSafeHdc();

	//get the current player
	iPlayer = VIEW->getPlayer();

	//determine AI
	bAI = GAME->m_players[iPlayer].m_player.getAI();

	//only allow current player to play dev cards
	iCards = GAME->m_players[iPlayer].playDevCards();

	//if we're showing goods, or an AI, disable
	if((TRUE == m_bInTrade) || (TRUE == m_bInGoods) || (TRUE == bAI) || 
		((VIEW->getMode() >= GL_PLACE_ROAD) && (VIEW->getMode() <= GL_GAME_OVER)) ||
		(TRUE == VIEW->getSpecialBuild()))
	{
		iCards = 0;
	}

	//get the right image
	iImage = (iCards ? BUTTON_PLAY : BUTTON_PLAY_GRAY);

	//enable the play window
	CONTROLS->Play().EnableWindow((iCards > 0));
	CONTROLS->Play().SetBitmaps(VIEW->getControlImage(iImage, hDC), COLOR_TRANSP, VIEW->getControlImage(iImage, hDC), COLOR_TRANSP);

	//release dc
	CONTROLS->ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// enable/disable the trade button
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::EnableTrade()
{
	int i;
	int iImage;
	int iPlayer;
	int iCards;
	BOOL bAI;
	BOOL bThree;
	BOOL bPort;
	BOOL bTrade = FALSE;
	BOOL bBank = FALSE;
	CDC *dc;
	HDC hDC;

	//get the dc
	dc = CONTROLS->GetDC();
	hDC = dc->GetSafeHdc();

	//get the current player
	iPlayer = VIEW->getPlayer();

	//determine AI
	bAI = GAME->m_players[iPlayer].m_player.getAI();

	//scan through and see if anyone can trade
	for(i = 0; i < m_iPlayers; i++)
	{
		//can't trade with yourself
		if(i == iPlayer) continue;

		//see if this player can trade
		if(GAME->m_players[i].canTrade())
		{
			bTrade = TRUE;
			break;
		}
	}

	//make sure they have cards to trade
	if( (TRUE == bTrade) && 
		((FALSE == GAME->m_players[iPlayer].canTrade()) && (FALSE == GAME->m_bTeam))
		)
	{
		bTrade = FALSE;
	}

	//make sure that we are in turn actions and not AI
	if((TRUE == m_bInTrade) || (FALSE == m_bInTurn) || (TRUE == bAI) || 
		((VIEW->getMode() >= GL_PLACE_ROAD) && (VIEW->getMode() <= GL_GAME_OVER)) ||
		(TRUE == VIEW->getSpecialBuild()))
	{
		bTrade = FALSE;
	}

	//in network or hotseat mode, see if they have already placed a build object
	if ((GAME_MODE_NETWORK == GAME->m_iMode || GAME_MODE_HOTSEAT == GAME->m_iMode) && 
		(TRUE == GAME->m_players[iPlayer].m_bPlacedBuild) &&
		(FALSE == IS_VARIANT(VARIANT_TRADE_AFTER_BUILD)))
	{
		bTrade = FALSE;
	}

	//make sure we don't have the no trade variant on
	if(IS_VARIANT(VARIANT_NO_TRADING))
	{
		bTrade = FALSE;
	}

	//get the right image
	iImage = (bTrade ? BUTTON_TRADE : BUTTON_TRADE_GRAY);

	//enable the buy window
	CONTROLS->Trade().EnableWindow(bTrade);
	CONTROLS->Trade().SetBitmaps(VIEW->getControlImage(iImage, hDC), COLOR_TRANSP, VIEW->getControlImage(iImage, hDC), COLOR_TRANSP);

	//see if this player has a three port
	bThree = VIEW->havePort(iPlayer, RES_PORT3);

	//check for bank
	for(i = 0; i < 5; i++)
	{
		//see if this player has this resource port
		bPort = VIEW->havePort(iPlayer, RES_PORTTIMBER + i);

		//get the number of resource cards this player has
		iCards = GAME->m_players[iPlayer].m_iRes[i];

		//set the tradable value based on the current player's resources
		if(iCards >= 4 || (bThree && iCards >= 3) || (bPort && iCards >= 2))
		{
			bBank = TRUE;
			break;
		}
	}

	//make sure that we are in turn actions and not AI
	if(TRUE == m_bInTrade || FALSE == m_bInTurn || TRUE == bAI || (VIEW->getMode() >= GL_PLACE_ROAD && VIEW->getMode() <= GL_GAME_OVER))
	{
		bBank = FALSE;
	}

	//get correct image
	iImage = (bBank ? BUTTON_BANK : BUTTON_BANK_GRAY);

	//enable the bank window
	CONTROLS->Bank().EnableWindow(bBank);
	CONTROLS->Bank().SetBitmaps(VIEW->getControlImage(iImage, hDC), COLOR_TRANSP, VIEW->getControlImage(iImage, hDC), COLOR_TRANSP);

	//release the DC
	CONTROLS->ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// enable/disable the end turn button
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::EnableEndTurn(void)
{
	int iImage;
	BOOL bShow;
	BOOL bAI;
	CDC *dc;
	HDC hDC;

	//get the dc
	dc = CONTROLS->GetDC();
	hDC = dc->GetSafeHdc();

	//get AI
	bAI = GAME->m_players[VIEW->getPlayer()].m_player.getAI();

	//make sure we're in a turn
	bShow = (FALSE == m_bInTrade) && (TRUE == m_bInTurn) && (FALSE == bAI) && !(VIEW->getMode() >= GL_PLACE_ROAD && VIEW->getMode() <= GL_GAME_OVER);

	//if we're in the special build phase, it must be on
	if(TRUE == VIEW->getSpecialBuild())
	{
		bShow = TRUE;
	}

	//get the right image
	iImage = (bShow ? BUTTON_END : BUTTON_END_GRAY);

	//enable the buy window
	CONTROLS->End().EnableWindow(bShow);
	CONTROLS->End().SetBitmaps(VIEW->getControlImage(iImage, hDC), COLOR_TRANSP, VIEW->getControlImage(iImage, hDC), COLOR_TRANSP);

	//release the DC
	CONTROLS->ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// handle common end of turn activities
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::handleCommonEndTurn()
{
	int i;
	int iPlayer;
	CUndoEnd *pUndoEnd;
	
	//play the sound
	SOUND(SOUND_END_TURN);

	//get the current player
	iPlayer = VIEW->getPlayer();

	//create the undo structure
	pUndoEnd = new CUndoEnd(GAME->m_players[iPlayer].getCardsBought(), 
							GAME->m_players[iPlayer].m_bPlayedCard, 
							GAME->m_players[iPlayer].m_bMovedShip,
							VIEW->getShipsThisTurn(),
							VIEW->getPortsThisTurn());

	//add the undo
	VIEW->addUndo(UNDO_ACTION_END, pUndoEnd);

	//reset the players trade offers this turn
	GAME->m_players[iPlayer].m_nTradeOffers = 0;

	//copy bought dev cards to playable dev cards
	GAME->m_players[iPlayer].copyDevCards();

	//clear any road-building
	VIEW->clearRoadBuild();
	
	//clear any ships built this turn
	VIEW->clearShipsThisTurn();

	//clear any ports built on this turn
	VIEW->clearPortsThisTurn();

	//they can now play a dev card again
	GAME->m_players[iPlayer].m_bPlayedCard = FALSE;

	//they can now move a ship again
	GAME->m_players[iPlayer].m_bMovedShip = FALSE;

	//reset the build on this turn flag
	GAME->m_players[iPlayer].m_bPlacedBuild = FALSE;

	//no longer in a turn
	m_bInTurn = FALSE;

	//set game status
	GAME->m_iState = GAME_PRE_TURN;

	//hide build options
	VIEW->showBuildOptions(FALSE);

	//set all goods received
	for(i = 0; i < m_iPlayers; i++)
	{
		updateGoods(0, i, "");
	}

	//toggle the actions window on
	showRolls(FALSE);
	ACTIONS->ShowWindow(SW_SHOW);
}

/////////////////////////////////////////////////////////////////////////////
// return to rolling
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::OnEndButton() 
{
	COleDateTime time;
	COleDateTimeSpan timespan;

	//intercept special build phase
	if(TRUE == VIEW->getSpecialBuild())
	{
		//hide build options
		VIEW->showBuildOptions(FALSE);

		if(GAME_MODE_HOTSEAT == GAME->m_iMode)
		{
			//go to the next player
			VIEW->setNextPlayer();
		}
		else
		{
			//send in the data
			VIEW->clientSpecialBuildNext();
		}

		return;
	}

	//handle common to network things
	handleCommonEndTurn();

	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		//toggle controls button
		CONTROLS->ShowWindow(SW_HIDE);

		//end their turn
		VIEW->clientEndTurn();
	}
	else
	{
		//get the current time
		time = COleDateTime::GetCurrentTime();
		
		//calculate the difference in time for this turn
		timespan = time - GAME->m_timeRef;

		//handle turn time update
		VIEW->handleCommonTimeUpdate(VIEW->getPlayer(), (int) timespan.GetTotalSeconds());

		//go to the next player
		VIEW->setNextPlayer();

		//check for special build phase
		if(TRUE == VIEW->getSpecialBuild())
		{
			return;
		}

		//run the post build phase
		postBuildPhase();
	}

	//set the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// post build phase setup
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::postBuildPhase()
{
	int i;
	BOOL bHotseat;
	BOOL bAutohide;

	//set the new reference time
	GAME->m_timeRef = COleDateTime::GetCurrentTime();

	//show the rolls
	showRolls(TRUE);

	//toggle actions on
	ACTIONS->ShowWindow(SW_SHOW);

	//enable/disable all buttons
	EnableTrade();
	EnableDC();
	EnableEndTurn();

	//only do more stuff in hotseat
	if(GAME_MODE_HOTSEAT != GAME->m_iMode)
	{
		return;
	}

	//check to see if they want a pause in between players
	if(TRUE == GetPrivateProfileInt(INI_GENERAL, INI_ASK_HOTSEAT, TRUE, INI_FILE))
	{
		//save the current setting
		bHotseat = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE_OTHER, FALSE, INI_FILE);
		bAutohide = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE, FALSE, INI_FILE);

		//write out what we want
		WritePrivateProfileString(INI_DISPLAY, INI_AUTOHIDE_OTHER, "0", INI_FILE);
		WritePrivateProfileString(INI_DISPLAY, INI_AUTOHIDE, "1", INI_FILE);

		//update the players
		for(i = 0; i < (int) GAME->m_players.size(); i++)
		{
			VIEW->updateAutohide();
		}

		MessageBox(NULL, "Click OK when the next person is ready.", "", MB_OK);

		//write it as what it was
		WritePrivateProfileString(INI_DISPLAY, INI_AUTOHIDE_OTHER, bHotseat ? "1" : "0", INI_FILE);
		WritePrivateProfileString(INI_DISPLAY, INI_AUTOHIDE, bAutohide ? "1" : "0", INI_FILE);

		//update the players
		for(i = 0; i < (int) GAME->m_players.size(); i++)
		{
			VIEW->updateAutohide();
		}
	}

	//now see if we automatically roll
	if(TRUE == GetPrivateProfileInt(INI_GENERAL, INI_AUTO_ROLL_H, FALSE, INI_FILE))
	{
		OnRollButton();
	}
}

/////////////////////////////////////////////////////////////////////////////
// show the trade window
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::OnTradeButton() 
{
	CTradeDialog trade;
	CNetworkTradeDialog *pNet;

	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		//create it
		pNet = new CNetworkTradeDialog;
		
		//make app aware
		VIEW->setDialog(pNet, TRUE);

		//create the dialog
		pNet->Create(IDD_NETWORK_TRADE_DLG, NULL);

		//disable other options
		InTrade(TRUE);

		//run the trade window
		pNet->ShowWindow(SW_SHOW);
	}
	else
	{
		//run the trade window
		trade.DoModal();

		//reenable trade
		EnableTrade();
	}

	//set the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle modeless trade dialog
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::InTrade(BOOL b)
{
	m_bInTrade = b;

	//enable/disable all
	EnableDC();
	EnableTrade();
	EnableEndTurn();
}

/////////////////////////////////////////////////////////////////////////////
// show the trade with bank window
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::OnBankButton()
{
	int nRet;
	CTradeWithBankDialog trade;

	//make app aware
	VIEW->setDialog(&trade, TRUE);

	//run the trade window
	nRet = trade.DoModal();

	//handle aborts
	if(IDABORT == nRet)
	{
		return;
	}

	//reenable trade
	EnableTrade();

	//make sure the turn window is showing
	if(FALSE == VIEW->isWaiting())
	{
		VIEW->showBuildOptions(TRUE);
	}

	//make app aware
	VIEW->setDialog(NULL);

	//set the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// run the play development card loop
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::OnPlayButton() 
{
	int iRet;
	CNetworkPlayCardDialog d;

	//set the before flag if we're not in a turn yet
	if(FALSE == m_bInTurn)
	{
		d.setBefore();
	}

	//make app aware
	VIEW->setDialog(&d, TRUE);

	//run the development card window
	iRet = d.DoModal();

	//set the focus
	VIEW->SetFocus();

	//make app aware
	VIEW->setDialog(NULL);

	//handle abot
	if((IDABORT == iRet) || (TRUE == VIEW->isWaiting()))
	{
		return;
	}

	//recheck
	EnableDC();
}

/////////////////////////////////////////////////////////////////////////////
// network message to set the panel text
/////////////////////////////////////////////////////////////////////////////
void CControlPanel::setPanelText(CString str)
{
	MESSAGES->Title().SetText(str);

	//set the focus
	VIEW->SetFocus();
}