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
#include "UndoDialog.h"
#include "UndoOne.h"
#include "SettlersView.h"
#include "ColorDatabase.h"
#include "GameDatabase.h"
#include "defineColors.h"
#include "defineCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUndoDialog dialog

/////////////////////////////////////////////////////////////////////////////
// constructor
/////////////////////////////////////////////////////////////////////////////
CUndoDialog::CUndoDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CUndoDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUndoDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//first turn is 1
	m_iTurn = 0;

	//new turn needed at first
	m_bTurn = TRUE;

	//null pointers
	m_itemLast = NULL;
	m_pImages = NULL;

	//null parent items
	m_itemBoard = NULL;
	m_itemChips = NULL;
	m_itemPlayers = NULL;
	m_itemPlacement = NULL;
	m_itemPrevTurn = NULL;
	m_itemTurn = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// denstructor
/////////////////////////////////////////////////////////////////////////////
CUndoDialog::~CUndoDialog()
{
	//delete the image list
	if(m_pImages)
	{
		m_pImages->DeleteImageList();
		delete m_pImages;
		m_pImages = NULL;
	}
}

void CUndoDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUndoDialog)
	DDX_Control(pDX, IDC_UNDO_TREE, m_UndoTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUndoDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CUndoDialog)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUndoDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// create the image list
/////////////////////////////////////////////////////////////////////////////
BOOL CUndoDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_HISTORY;

	//create the image list
	m_pImages = new CImageList;
	m_pImages->Create(16, 16, ILC_COLOR24 | ILC_MASK, HISTORY_SIZE, 1);

	//generate the image list
	generateIL();

	//add the image list to the tree control
	m_UndoTree.SetImageList(m_pImages, TVSIL_NORMAL);

	//set various colors
	m_UndoTree.SetBkColor(COLOR_BLACK);
	m_UndoTree.SetTextColor(COLOR_WHITE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// add a new item to the tree
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addItem(int iAction, CUndo *pUndo)
{
	switch(iAction)
	{
	case UNDO_SETTLE_PLACE: addPlacement(TRUE);	break;
	case UNDO_THIRD_ROAD_PLACE:
	case UNDO_ROAD_PLACE: addPlacement(FALSE); break;
	case UNDO_SHIP_PLACE: addPlacement(FALSE, TRUE); break;
	case UNDO_FIRST_ROLL: addRoll(TRUE, ((CUndoOne *) pUndo)->m_iOne); break;
	case UNDO_ROLL:	addRoll(FALSE, ((CUndoOne *) pUndo)->m_iOne); break;
	case UNDO_ACTION_START:	addActionStart(); break;
	case UNDO_ACTION: addAction(); break;
	case UNDO_ROAD:	addRoad(((CUndoOne *) pUndo)->m_iOne); break;
	case UNDO_SHIP: addShip(((CUndoOne *) pUndo)->m_iOne); break;
	case UNDO_CITY:	addCity(); break;
	case UNDO_MOVE_SHIP: addMoveShip(((CUndoOne *) pUndo)->m_iOne); break;
	case UNDO_SETTLEMENT: addSettle(); break;
	case UNDO_BUY_DEVCARD: addDevcard(); break;
	case UNDO_ROBBER_PLACE:	addRobber(); break;
	case UNDO_PIRATE_PLACE: addPirate(); break;
	case UNDO_PLAY_VICTORY:	addVictory(((CUndoOne *) pUndo)->m_iOne); break;
	case UNDO_PLAY_MONOPOLY: addMonopoly((CUndoMonopoly *) pUndo); break;
	case UNDO_PLAY_ROAD: addRoadBuild(); break;
	case UNDO_PLAY_YEAR: addYearPlenty(((CUndoOne *) pUndo)->m_iOne); break;
	case UNDO_PLAY_SOLDIER:	addSoldier(((CUndoOne *) pUndo)->m_iOne); break;
	case UNDO_TRADE: addTrade((CUndoTrade *) pUndo); break;
	case UNDO_STEAL: addSteal(((CUndoOne *) pUndo)->m_iOne); break;
	case UNDO_LOSECARDS: addLoseCards((CUndoTrade *) pUndo); break;
	case UNDO_ACTION_END: endTurn(); break;
	case UNDO_WIN: addWin(((CUndoOne *) pUndo)->m_iOne); break;
	case UNDO_WASTE: addWaste(((CUndoOne *) pUndo)->m_iOne); break;
	case UNDO_GOLD: addGold((CUndoGold *) pUndo); break;
	case UNDO_CHITLESS: addChitless(((CUndoOne *) pUndo)->m_iOne); break;
	case UNDO_VOLCANO: addVolcano(); break;
	case UNDO_TRADE_JUNGLE: addJungle((CUndoTrade *) pUndo); break;
	case UNDO_SPECIAL_BUILD: addSpecialBuild(((CUndoOne *) pUndo)->m_iOne); break;
	case UNDO_SPECIAL_BUILD_END: addSpecialBuildEnd(); break;
	default: break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// remove the last item from the tree\
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::removeItem(void)
{
	HTREEITEM hPrevItem;
	HTREEITEM hParent;

	//get the parent item
	hParent = m_UndoTree.GetParentItem(m_itemLast);

	//get the previous item
	hPrevItem = (HTREEITEM) m_UndoTree.GetItemData(m_itemLast);

	//remove the last item
	m_UndoTree.DeleteItem(m_itemLast);

	//default to not needing a new turn
	m_bTurn = FALSE;

	//see if the parent is empty
	if(!m_UndoTree.ItemHasChildren(hParent))
	{
		//compare it to others
		if(hParent == m_itemBoard) m_itemBoard = NULL;
		else if(hParent == m_itemChips) m_itemChips = NULL;
		else if(hParent == m_itemPlayers) m_itemPlayers = NULL;
		else if(hParent == m_itemPlacement) m_itemPlacement = NULL;
		else if(hParent == m_itemTurn)
		{
			m_itemTurn = m_itemPrevTurn;

			//subtract one from the turn counter
			m_iTurn--;

			if(m_itemTurn)
			{
				m_itemPrevTurn = m_UndoTree.GetPrevSiblingItem(m_itemTurn);

				//make sure it's not the placement one
				if(m_itemPrevTurn == m_itemPlacement) 
				{
					m_itemPrevTurn = NULL;
				}
			}
			//will need a new turn
			m_bTurn = TRUE;
		}

		//remove it
		m_UndoTree.DeleteItem(hParent);
	}

	//set the previous item
	m_itemLast = hPrevItem;

	//ensure its visibility
	m_UndoTree.EnsureVisible(m_itemLast);
}

/////////////////////////////////////////////////////////////////////////////
// inserts an item into the tree
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::insertItem(CString str, int iImage, HTREEITEM hParent)
{
	HTREEITEM hPrevItem;

	//save the last item
	hPrevItem = m_itemLast;

	//add it to the tree
	m_itemLast = m_UndoTree.InsertItem(str, iImage, iImage, hParent);

	//set the previous item data
	m_UndoTree.SetItemData(m_itemLast, (DWORD) hPrevItem);
		
	//make sure it's visible
	m_UndoTree.EnsureVisible(m_itemLast);
}

/////////////////////////////////////////////////////////////////////////////
// inserts a turn into the tree
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::insertTurn(BOOL bFirst)
{
	int iPlayer;
	CString str;
	int iColor;

	//get the current player
	iPlayer = VIEW->getPlayer();

	//see if it needs to be added
	if(m_bTurn)
	{
		//close the board setup
		m_UndoTree.Expand(bFirst ? m_itemPlacement : m_itemTurn, TVE_COLLAPSE);

		//increment the turn counter
		m_iTurn++;

		//create the string
		str.Format("Turn %d - %s", m_iTurn, GAME->m_players[iPlayer].m_player.getName());

		//get the color
		iColor = getTurnImage(GAME->m_players[iPlayer].m_iColor);

		//set the previous turn
		m_itemPrevTurn = m_itemTurn;

		//add this new one
		m_itemTurn = m_UndoTree.InsertItem(str, iColor, iColor);

		//no longer needed
		m_bTurn = FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// add placement information
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addPlacement(BOOL bSettle, BOOL bShip)
{
	int iImage;
	int iPlayer;
	CString str;
	CString strText;

	//see if it's been added
	if(!m_itemPlacement)
	{
		//close the board setup
		m_UndoTree.Expand(m_itemPlayers, TVE_COLLAPSE);

		//add this new one
		m_itemPlacement = m_UndoTree.InsertItem("Initial Placement", HISTORY_INITPLACE, HISTORY_INITPLACE);
	}

	//get the current player
	iPlayer = VIEW->getPlayer();

	//get the image and text
	if(TRUE == bSettle)
	{
		iImage = GL_SETTLEMENT;
		strText = "settlement";
	}
	else
	{
		iImage = bShip ? GL_SHIP : GL_ROAD;
		strText = bShip ? "ship" : "road";
	}

	//create the text
	str.Format("Placed a %s.", strText);

	//new turn needed at first
	m_bTurn = TRUE;

	//insert it
	insertItem(str, getImage(GAME->m_players[iPlayer].m_iColor, iImage), m_itemPlacement);
}

/////////////////////////////////////////////////////////////////////////////
// add a roll to the turn
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addRoll(BOOL bFirst, int iRoll)
{
	CString str;

	//insert the turn if needed
	insertTurn(bFirst);

	//add the roll
	if(NEEDS_AN(iRoll))
	{
		str.Format("Rolled an %d.", iRoll);
	}
	else
	{
		str.Format("Rolled a %d.", iRoll);	
	}

	//insert the image
	insertItem(str, HISTORY_DICE, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle the start of a turn
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addActionStart()
{
	//add the item
	insertItem("Started actions.", HISTORY_TURNSTART, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle an action start
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addAction()
{
	insertItem("Showed placement options.", HISTORY_ACTION, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle road adding
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addRoad(int ID)
{
	int iLongest;
	BOOL bNeg;
	CString str;

	//see if we got longest
	bNeg = (ID < 0);

	//reverse it
	if(bNeg) ID *= -1;

	//get the longest value
	iLongest = ID % 10;

	//reverse it
	if(bNeg) iLongest *= -1;

	//compare
	if(iLongest >= -1)
	{
		str = "Placed a road (Longest Road).";
	}
	else
	{
		str = "Placed a road.";
	}

	insertItem(str, getImage(GAME->m_players[VIEW->getPlayer()].m_iColor, GL_ROAD), m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle ship adding
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addShip(int ID)
{
	int iLongest;
	BOOL bNeg;
	CString str;

	//see if we got longest
	bNeg = (ID < 0);

	//reverse it
	if(bNeg) ID *= -1;

	//get the longest value
	iLongest = ID % 10;

	//reverse it
	if(bNeg) iLongest *= -1;

	//compare
	if(iLongest >= -1)
	{
		str = "Placed a ship (Longest Road).";
	}
	else
	{
		str = "Placed a ship.";
	}

	insertItem(str, getImage(GAME->m_players[VIEW->getPlayer()].m_iColor, GL_SHIP), m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle city adding
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addCity()
{
	insertItem("Placed a city.", getImage(GAME->m_players[VIEW->getPlayer()].m_iColor, GL_CITY), m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle ship moving
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addMoveShip(int ID)
{
	int iLongest;
	BOOL bNeg;
	CString str;

	//see if we got longest
	bNeg = (ID < 0);

	//reverse it
	if(bNeg) ID *= -1;

	//get the longest value
	iLongest = ID % 10;

	//reverse it
	if(bNeg) iLongest *= -1;

	//compare
	if(iLongest >= -1)
	{
		str = "Moved a ship (Longest Road).";
	}
	else
	{
		str = "Moved a ship.";
	}

	insertItem(str, HISTORY_MOVE, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle setlement adding
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addSettle()
{
	insertItem("Placed a settlement.", getImage(GAME->m_players[VIEW->getPlayer()].m_iColor, GL_SETTLEMENT), m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle buying a dev card
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addDevcard()
{
	insertItem("Bought a development card.", HISTORY_BUYCARD, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle a robber placement
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addRobber()
{
	insertItem("Placed the robber.", HISTORY_ROBBER, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle a pirate placement
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addPirate(void)
{
	insertItem("Placed the pirate.", HISTORY_PIRATE, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle playing a victory point
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addVictory(int iPlayer)
{
	int iPlay;
	CString str;

	//insert turn if needed
	insertTurn();

	//get the current player
	iPlay = VIEW->getPlayer();

	//see if they're the same
	if(iPlay == iPlayer)
	{
		//insert the victory point
		insertItem("Played a Victory Point.", HISTORY_VICTORY, m_itemTurn);	
	}
	//otherwise, a different player played a point
	else
	{
		str.Format("%s played a Victory Point.", GAME->m_players[iPlayer].m_player.getName());

		//set the text
		insertItem(str, HISTORY_VICTORY, m_itemTurn);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle playing a monopoly
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addMonopoly(CUndoMonopoly *pMonopoly)
{
	CString str;

	//insert turn if needed
	insertTurn();

	//format it
	str.Format("Played a Monopoly card on %s.", VIEW->getResName(pMonopoly->m_iType, TRUE));

	//insert it
	insertItem(str, HISTORY_MONOPOLY, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle playing a road building card
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addRoadBuild()
{
	//insert turn if needed
	insertTurn();

	insertItem("Played a Road Building card.", HISTORY_ROADBUILD, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle playing a year of plenty card
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addYearPlenty(int ID)
{
	int i;
	int iAmount;
	int iMult = 10000;
	BOOL bFirst = TRUE;
	CString str, strTemp;

	//insert turn if needed
	insertTurn();

	//record what they got
	str = "Played a Year of Plenty Card to get ";

	//run through
	for(i = 0; i < 5; i++)
	{
		//get the amount
		iAmount = ID / iMult; ID -= (iAmount * iMult);

		if(iAmount)
		{
			//format the string
			strTemp.Format("%d %s", iAmount, VIEW->getResName(i, TRUE));
			
			//see if we need an and
			if(iAmount == 1 && bFirst)
			{
				strTemp += " and ";
				bFirst = FALSE;
			}

			//add it
			str += strTemp;
		}

		//change the multiplier
		iMult /= 10;
	}

	//finish up
	str += ".";

	//insert it
	insertItem(str, HISTORY_YEARPLENTY, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle playing a soldier card
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addSoldier(int iPlayer)
{
	//insert turn if needed
	insertTurn();

	//check for largest army
	if(iPlayer > -2)
	{
		insertItem("Played a Soldier card (Largest Army).", HISTORY_SOLDIER, m_itemTurn);
	}
	else
	{
		insertItem("Played a Soldier card.", HISTORY_SOLDIER, m_itemTurn);
	}
}

/////////////////////////////////////////////////////////////////////////////
// add a trade into the mix
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addTrade(CUndoTrade *pTrade)
{
	int i;
	int iTo = 0;
	int iFor = 0;
	CString str;
	CString strTemp;
	CString strTo, strFor;

	//create the formatted string
	str = "Traded";

	//run through the list of goods they traded
	for(i = 0; i < 5; i++)
	{
		//see if they traded this item
		if(0 < pTrade->m_iTo[i])
		{
			//increment counter
			iTo++;

			//add it
			strTemp.Format(" %d %s,", pTrade->m_iTo[i], VIEW->getResName(i, TRUE));
			strTo += strTemp;
		}

		//see if they got this item
		if(0 < pTrade->m_iFrom[i])
		{
			//increment counter
			iFor++;

			//add it
			strTemp.Format(" %d %s,", pTrade->m_iFrom[i], VIEW->getResName(i, TRUE));
			strFor += strTemp;
		}
	}

	//get rid of the last commas
	strTo = strTo.Left(strTo.GetLength() - 1);
	strFor = strFor.Left(strFor.GetLength() - 1);

	//add the to clause
	if(0 < iTo)
	{
		str += strTo;
	}
	else
	{
		str += " nothing";
	}

	//add the other player
	str += " to ";
	if(pTrade->m_iPartner >= 0)
	{
		str += GAME->m_players[pTrade->m_iPartner].m_player.getName();
	}
	else
	{
		str += "the bank";
	}
	str += " for";

	//add the from clause
	if(0 < iFor)
	{
		str += strFor;
	}
	else
	{
		str += " nothing";
	}

	//add a period
	str += ".";

	//insert the item
	insertItem(str, HISTORY_TRADE, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// a stolen card
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addSteal(int ID)
{
	int iPlayer;
	int iRes;
	BOOL bPirate;
	CString str;

	//decipher the ID
	DECODE_STEAL(ID, bPirate, iPlayer, iRes);

	//see if they did not steal
	if(iRes == 5)
	{
		insertItem("Chose to not steal a card.", HISTORY_NOSTEAL, m_itemTurn);
	}
	else
	{		
		//format the string
		str.Format("Stole 1 card from %s.", GAME->m_players[iPlayer].m_player.getName());

		//insert 
		insertItem(str, HISTORY_STEAL, m_itemTurn);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a player losing cards
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addLoseCards(CUndoTrade *pLose)
{
	int i;
	CString str;
	CString strTemp;

	//start the string
	str = GAME->m_players[pLose->m_iPartner].m_player.getName();
	str += " lost";

	//add the resources
	for(i = 0; i < 5; i++)
	{
		//see if they lost this card
		if(!pLose->m_iTo[i]) continue;

		//format
		strTemp.Format(" %d %s,", pLose->m_iTo[i], VIEW->getResName(i, TRUE));

		//add it
		str += strTemp;
	}

	//get rid of the last comma
	str = str.Left(str.GetLength() - 1);
	str += ".";

	//insert it
	insertItem(str, HISTORY_LOSECARDS, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle the end of a turn
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::endTurn()
{
	//add the item
	insertItem("Ended turn.", HISTORY_TURNEND, m_itemTurn);

	//need a new turn next time
	m_bTurn = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// handle a player winning the game
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addWin(int iPlayer)
{
	CString str;

	//insert turn if needed
	insertTurn();

	//start the string
	str.Format("%s won the game!", GAME->m_players[iPlayer].m_player.getName());

	//insert it
	insertItem(str, HISTORY_WIN, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle a player wasting victory points
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addWaste(int ID)
{
	int iPlayer;
	int iPoints;
	CString str;

	//deconstruct the ID
	DECODE_SEL(ID, iPlayer, iPoints);

	//create the string
	str.Format("%s had %d unplayed victory point%s.", GAME->m_players[iPlayer].m_player.getName(), iPoints, iPoints == 1 ? "" : "s");
	
	//insert it
	insertItem(str, HISTORY_VICTORY, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle a player getting resources from gold
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addGold(CUndoGold *pGold)
{
	int i, j;
	int iCount;
	CString str, strTemp;

	for(i = 0; i < (int) GAME->m_players.size(); i++)
	{
		//reset the count
		iCount = 0;

		//start off the string
		str.Format("%s gained ", GAME->m_players[i].m_player.getName());

		//go through the resources
		for(j = 0; j < 5; j++)
		{
			if(0 < pGold->m_iResources[i][j])
			{
				strTemp.Format("%d %s, ", pGold->m_iResources[i][j], VIEW->getResName(j));
				iCount += pGold->m_iResources[i][j];
				str += strTemp;
			}
		}

		//see if it should be added
		if(0 < iCount)
		{
			//remove the last comma
			str = str.Left(str.GetLength() - 2);
			
			//add the final text
			str += " from gold.";

			//insert it
			insertItem(str, HISTORY_GOLD, m_itemTurn);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle chitless tile moving
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addChitless(int ID)
{
	int nTile1, nTile2;
	int nRoll;
	CString str;

	//decode the tiles
	DECODE_CHITLESS(ID, nTile1, nTile2);

	//get the roll
	nRoll = VIEW->getRoll(nTile1);

	//set it
	str.Format("Switched a%s %d chit from the mainland to an island.", NEEDS_AN(nRoll) ? "n" : "", nRoll);

	//insert it
	insertItem(str, HISTORY_CHITLESS, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle volcanic eruptions
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addVolcano()
{
	insertItem("Volcanic Eruption!", HISTORY_VOLCANO, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle jungle trades
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addJungle(CUndoTrade *pJungle)
{
	int i;
	int nJungles;
	CString str;
	CString strTemp;

	//get jungle amount
	nJungles = pJungle->m_iPartner;

	//create the formatted string
	str.Format("Traded %d Discover%s for", nJungles, (nJungles == 1) ? "y" : "ies");

	//run through the list of goods they traded
	for(i = 0; i < 5; i++)
	{
		//see if they got this item
		if(0 < pJungle->m_iFrom[i])
		{
			//add it
			strTemp.Format(" %d %s,", pJungle->m_iFrom[i], VIEW->getResName(i, TRUE));
			str += strTemp;
		}
	}

	//get rid of the last comma
	str = str.Left(str.GetLength() - 1);
	str += ".";

	//insert it
	insertItem(str, HISTORY_JUNGLE, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// add the special build icon
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addSpecialBuild(int nPlayer)
{
	int nImage;
	CString str;

	//create the string
	str.Format("Special Build Phase for %s.", GAME->m_players[nPlayer].m_player.getFirstName());

	//get the turn image
	nImage = getTurnImage(GAME->m_players[nPlayer].m_iColor);

	//insert it
	insertItem(str, nImage, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// add the end special build icon
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::addSpecialBuildEnd()
{
	insertItem("Special Build Phase ended.", HISTORY_TURNEND, m_itemTurn);
}

/////////////////////////////////////////////////////////////////////////////
// handle resize information
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::OnSize(UINT nType, int cx, int cy) 
{
	CHelpDialog::OnSize(nType, cx, cy);
	
	//resize the tree
	if(m_UndoTree.GetSafeHwnd())
	{
		m_UndoTree.SetWindowPos(&wndTop, 0, 0, cx, cy, SWP_SHOWWINDOW);
	}
}

/////////////////////////////////////////////////////////////////////////////
// return a certain image
/////////////////////////////////////////////////////////////////////////////
int CUndoDialog::getImage(int iColor, int iType)
{
	int ui = 0;

	switch(iColor)
	{
	case CR_WHITE: switch(iType)	{case GL_CITY: ui = HISTORY_CITYW; break; case GL_ROAD: ui = HISTORY_ROADW; break; case GL_SETTLEMENT: ui = HISTORY_SETTLEW; break; case GL_SHIP: ui = HISTORY_SHIPW;} break;
	case CR_RED: switch(iType)		{case GL_CITY: ui = HISTORY_CITYR; break; case GL_ROAD: ui = HISTORY_ROADR; break; case GL_SETTLEMENT: ui = HISTORY_SETTLER; break; case GL_SHIP: ui = HISTORY_SHIPR;} break;
	case CR_GREEN: switch(iType)	{case GL_CITY: ui = HISTORY_CITYG; break; case GL_ROAD: ui = HISTORY_ROADG; break; case GL_SETTLEMENT: ui = HISTORY_SETTLEG; break; case GL_SHIP: ui = HISTORY_SHIPG;} break;
	case CR_ORANGE: switch(iType)	{case GL_CITY: ui = HISTORY_CITYO; break; case GL_ROAD: ui = HISTORY_ROADO; break; case GL_SETTLEMENT: ui = HISTORY_SETTLEO; break; case GL_SHIP: ui = HISTORY_SHIPO;} break;
	case CR_BLUE: switch(iType)		{case GL_CITY: ui = HISTORY_CITYB; break; case GL_ROAD: ui = HISTORY_ROADB; break; case GL_SETTLEMENT: ui = HISTORY_SETTLEB; break; case GL_SHIP: ui = HISTORY_SHIPB;} break;
	case CR_BROWN: switch(iType)	{case GL_CITY: ui = HISTORY_CITYBN; break; case GL_ROAD: ui = HISTORY_ROADBN; break; case GL_SETTLEMENT: ui = HISTORY_SETTLEBN; break; case GL_SHIP: ui = HISTORY_SHIPBN;} break;
	}

	//return it
	return ui;
}

/////////////////////////////////////////////////////////////////////////////
// return a certain image
/////////////////////////////////////////////////////////////////////////////
int CUndoDialog::getTurnImage(int iColor)
{
	int ui = 0;

	switch(iColor)
	{
	case CR_WHITE:	ui = HISTORY_TURN_WHITE; break;
	case CR_RED:	ui = HISTORY_TURN_RED; break;
	case CR_GREEN:	ui = HISTORY_TURN_GREEN; break;
	case CR_ORANGE: ui = HISTORY_TURN_ORANGE; break;
	case CR_BLUE:	ui = HISTORY_TURN_BLUE; break;
	case CR_BROWN:	ui = HISTORY_TURN_BROWN; break;
	}

	return ui;
}

/////////////////////////////////////////////////////////////////////////////
// update main menu
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::OnCancel() 
{
	hideMenu();

	//normal handling
	CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// update main menu
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::OnOK()
{
	hideMenu();

	//normal handling
	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// update main menu
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::hideMenu(void)
{
	CMenu *pMenu;
	CMenu *pSubmenu;

	//get the current menu
	pMenu = AfxGetMainWnd()->GetMenu();

	//get the submenu
	pSubmenu = pMenu->GetSubMenu(3);

	//set the checked status
	pSubmenu->CheckMenuItem(0, MF_UNCHECKED | MF_BYPOSITION);
}

/////////////////////////////////////////////////////////////////////////////
// save and load this dialog box
/////////////////////////////////////////////////////////////////////////////
void CUndoDialog::Serialize(CArchive& ar) 
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
void CUndoDialog::load(CArchive &ar)
{
	int i;
	int iNodes;

	//the turn counter and item flag
	if(DATABASE->gameVer() >= VER_NETWORKc)
	{
		//turn counter
		ar >> m_iTurn;

		//item flag
		ar >> m_bTurn;
	}

	//read in the number of nodes
	ar >> iNodes;

	//read in this many nodes and add them to the tree
	for(i = 0; i < iNodes; i++)
	{
		//read them in
		input(ar);
	}
}

//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CUndoDialog::save(CArchive &ar)
{
	HTREEITEM hChild;
	HTREEITEM hParent;

	//turn counter
	ar << m_iTurn;

	//turn flag
	ar << m_bTurn;

	//write out how many things we're going to save
	ar << m_UndoTree.GetCount();
	
	//get the root item
	hParent = m_UndoTree.GetRootItem();

	//run through each node
	while(hParent)
	{
		//write the node
		output(ar, hParent);

		//sort through the children
		hChild = m_UndoTree.GetChildItem(hParent);

		//run through each node
		while(hChild)
		{
			//write the node
			output(ar, hChild, FALSE);

			//get the next child
			hChild = m_UndoTree.GetNextSiblingItem(hChild);
		}

		//get the next header
		hParent = m_UndoTree.GetNextSiblingItem(hParent);
	}
}

//////////////////////////////////////////////////////////////////////
// outputs one node
//////////////////////////////////////////////////////////////////////
void CUndoDialog::output(CArchive &ar, HTREEITEM hItem, BOOL bHead)
{
	int iImage, iSelImage;
	CString str;

	//get the image used for this item
	m_UndoTree.GetItemImage(hItem, iImage, iSelImage);

	//get the text for this item
	str = m_UndoTree.GetItemText(hItem);

	//output if this is a header
	ar << bHead;

	//output the image used
	ar << iImage;
		
	//output the text string
	ar << str;
}

//////////////////////////////////////////////////////////////////////
// outputs one node
//////////////////////////////////////////////////////////////////////
void CUndoDialog::output(int &iStart, CNDKMessage &msg, HTREEITEM hItem, BOOL bHead)
{
	int iImage, iSelImage;
	CString str;

	//get the image used for this item
	m_UndoTree.GetItemImage(hItem, iImage, iSelImage);

	//get the text for this item
	str = m_UndoTree.GetItemText(hItem);

	//output if this is a header
	msg.SetAt(iStart++, bHead);

	//output the image used
	msg.SetAt(iStart++, iImage);
		
	//output the text string
	msg.SetAt(iStart++, str);
}

//////////////////////////////////////////////////////////////////////
// inputs one node
//////////////////////////////////////////////////////////////////////
void CUndoDialog::input(CArchive &ar)
{
	int iImage;
	BOOL bHead;
	CString str;
	HTREEITEM hItem;

	//input if this is a header
	ar >> bHead;

	//input the image used
	ar >> iImage;

	//set the image offset if needed
	if(DATABASE->gameVer() < VER_SEAFARERS)
	{
		iImage -= HISTORY_OFFSET;
	}
		
	//input the text string
	ar >> str;

	//add the string to the tree
	hItem = m_UndoTree.InsertItem(str, iImage, iImage, bHead ? TVI_ROOT : m_itemLast);

	//if it's a head item, set it as the item to insert under
	if(TRUE == bHead)
	{
		m_itemLast = hItem;
		m_itemTurn = hItem;
	}
}

//////////////////////////////////////////////////////////////////////
// inputs one node
//////////////////////////////////////////////////////////////////////
void CUndoDialog::input(int &iStart, CNDKMessage &msg)
{
	int iImage;
	BOOL bHead;
	CString str;
	HTREEITEM hItem;

	//input if this is a header
	msg.GetAt(iStart++, bHead);

	//input the image used
	msg.GetAt(iStart++, iImage);
		
	//input the text string
	msg.GetAt(iStart++, str);

	//add the string to the tree
	hItem = m_UndoTree.InsertItem(str, iImage, iImage, bHead ? TVI_ROOT : m_itemLast);

	//if it's a head item, set it as the item to insert under
	if(TRUE == bHead)
	{
		m_itemLast = hItem;
		m_itemTurn = hItem;
	}
}

//////////////////////////////////////////////////////////////////////
// generate IL images
//////////////////////////////////////////////////////////////////////
void CUndoDialog::generateIL()
{
	int i;
	CDC *dc;
	CBitmap *pBitmap;
	HDC hDC;

	//sanity check
	if(NULL == VIEW)
	{
		return;
	}

	//get the current device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//run through
	for(i = 0; i < HISTORY_SIZE; i++)
	{
		//get a pointer
		pBitmap = CBitmap::FromHandle(VIEW->getHistoryImage(i, hDC));

		//copy it into the image list
		m_pImages->Add(pBitmap, COLOR_TRANSP);

		//clean up resource
		pBitmap->DeleteObject();
	}

	//return the context
	ReleaseDC(dc);
}

//////////////////////////////////////////////////////////////////
// write to a network message
//////////////////////////////////////////////////////////////////
void CUndoDialog::SerializeToMessage(CNDKMessage &msg)
{
	int iStart = 0;
	HTREEITEM hChild;
	HTREEITEM hParent;

	//turn counter
	msg.SetAt(iStart++, m_iTurn);

	//turn flag
	msg.SetAt(iStart++, m_bTurn);

	//write out how many things we're going to save
	msg.SetAt(iStart++, (int) m_UndoTree.GetCount());
	
	//get the root item
	hParent = m_UndoTree.GetRootItem();

	//run through each node
	while(hParent)
	{
		//write the node
		output(iStart, msg, hParent);

		//sort through the children
		hChild = m_UndoTree.GetChildItem(hParent);

		//run through each node
		while(hChild)
		{
			//write the node
			output(iStart, msg, hChild, FALSE);

			//get the next child
			hChild = m_UndoTree.GetNextSiblingItem(hChild);
		}

		//get the next header
		hParent = m_UndoTree.GetNextSiblingItem(hParent);
	}
}

//////////////////////////////////////////////////////////////////
// read from a network message
//////////////////////////////////////////////////////////////////
void CUndoDialog::SerializeFromMessage(CNDKMessage &msg)
{
	int i;
	int iNodes;
	int iStart = 0;

	//turn counter
	msg.GetAt(iStart++, m_iTurn);

	//item flag
	msg.GetAt(iStart++, m_bTurn);

	//read in the number of nodes
	msg.GetAt(iStart++, iNodes);

	//read in this many nodes and add them to the tree
	for(i = 0; i < iNodes; i++)
	{
		//read them in
		input(iStart, msg);
	}
}