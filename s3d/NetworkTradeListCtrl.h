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

#if !defined(AFX_NETWORKTRADELISTCTRL_H__DE8555A6_E99A_401F_AC8E_7C66D4A8FE29__INCLUDED_)
#define AFX_NETWORKTRADELISTCTRL_H__DE8555A6_E99A_401F_AC8E_7C66D4A8FE29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkTradeListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "SortListCtrl.h"
#include "Player.h"

/////////////////////////////////////////////////////////////////////////////
// CNetworkTradeListCtrl window
/////////////////////////////////////////////////////////////////////////////
class CNetworkTradeListCtrl : public CSortListCtrl
{
// Construction
public:
	CNetworkTradeListCtrl();

// Attributes
public:

	//add a player to the list of those who can trade
	void addPlayer(CPlayer *player, int iColor);

	//return whether a player is checked
	BOOL getCheck(int i) {return m_bChecks[i];}

	//handle a trade reply
	void handleReply(int iPlayer, short shStatus);

	//updates status text when status changes
	void updateStatus(int nItem, short shStatus);

	//return the status of someone
	short getStatus(int iPlayer);

	//reset trade statuses
	void resetStatus(void);

	//determine if all responses have returned
	int isComplete(void);

	//determine whether there are people to trade with
	BOOL isValid(void);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkTradeListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNetworkTradeListCtrl();

	// Generated message map functions
protected:
	//map of player ID to BOOL value for checkmarks
	BOOL m_bChecks[5];

	//map of player ID to trade status
	short m_shStatus[5];

	//fonts for use
	CFont m_fontBold;

	//{{AFX_MSG(CNetworkTradeListCtrl)
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	//returns a colorref for a status message
	COLORREF getStatusColor(int nItem);

	//draws a checkbox in the row
	void DrawCheckbox(int nItem, CDC *pDC, CRect& rect);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKTRADELISTCTRL_H__DE8555A6_E99A_401F_AC8E_7C66D4A8FE29__INCLUDED_)
