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

#if !defined(AFX_CONTROLPANEL_H__88200F43_2A24_11D6_A65D_0010A4C2C731__INCLUDED_)
#define AFX_CONTROLPANEL_H__88200F43_2A24_11D6_A65D_0010A4C2C731__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlPanel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// the control panel
/////////////////////////////////////////////////////////////////////////////
class CControlPanel
{
// Construction
public:
	CControlPanel();

// Attributes
public:

// Operations
public:
	void initControl(void);
	void updateText(void);
	void specialBuildPhase(void);
	void postBuildPhase(void);

	//are win a turn yet?
	BOOL inTurn(void) {return m_bInTurn;}
	void setTurn(void) {m_bInTurn = TRUE;}

	//show or hide the rolls and goods and actions
	void showRolls(BOOL bShow);
	void showGoods(BOOL bShow);
	void showVolcano(BOOL bShow);
	void showActions(BOOL bShow);
	void updateGoods(int iGoods, int iPlayer, CString strLine);
	void updateVolcano(BOOL bShow, int nPlayer, CString strLine);
	void OnAIGoods();
	
	//message handling
	void OnEndButton();
	void OnBankButton();
	void OnPlayButton();
	void OnTradeButton();
	void OnTitleButton();
	void OnRollButton();
	void OnGoodsButton();
	void OnVolcanoButton();

	//see if we can do any of these actions
	BOOL canRoll(void);
	BOOL canContinue(void) {return m_bInGoods;}

	//handle dev card, trade, and end turn updating
	void EnableDC(void);
	void EnableTrade(void);
	void EnableEndTurn(void);

	//handle modeless trade box
	void InTrade(BOOL b);

	//network common functions
	void handleCommonRoll(int iDie1, int iDie2, BOOL bFirst = TRUE);
	void handleCommonEndTurn(void);
	void handleCommonGoods(void);
	void handleCommonVolcano(void);
	void handleCommonSpecialBuild(CString str);

	//enter into a turn
	void enterTurn(void);
	void returnToTurn(void);
	void updateTurn(void);
	void continueTurn(void);

	//sets the last roll
	void setLastRoll(int iDie1, int iDie2) {m_iLastDie1 = iDie1; m_iLastDie2 = iDie2;}
	void getLastRoll(int &iDie1, int &iDie2) {iDie1 = m_iLastDie1; iDie2 = m_iLastDie2;}

	//sets the last volcano roll
	void setLastVolcano(int n) {m_nLastVolcano = n;}
	void setVolcanoRoll(int nRoll);

	//network function to manually set the panel text
	void setPanelText(CString str);

	//sets the color for the display text
	void updateColor(int iPlayer);

// Implementation
public:
	virtual ~CControlPanel();

protected:
	//update functions
	void updatePlace(void);
	void updateRobber(void);
	void updatePort(void);
	void updateThirdRoad(void);

	//generate the dice roll
	void generateDiceRoll(int &iDie1, int &iDie2);

	//generate equal odds dice
	void generateEqualOdds(int &iDie1, int &iDie2);

protected:
	//the number of players
	int m_iPlayers;

	//the last roll dice
	int m_iLastDie1;
	int m_iLastDie2;

	//the last volcano die
	int m_nLastVolcano;

	//started a turn yet?
	BOOL m_bInTurn;
	BOOL m_bInGoods;
	BOOL m_bInTrade;

	//array of strings of goods
	vector <CString> m_strLines;
	vector <CString> m_strVolcano;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLPANEL_H__88200F43_2A24_11D6_A65D_0010A4C2C731__INCLUDED_)
