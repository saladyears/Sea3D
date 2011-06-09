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

#if !defined(AFX_NETWORKLISTCTRL_H__2609BF15_AC0F_4D3D_ACE3_F51E51327BD2__INCLUDED_)
#define AFX_NETWORKLISTCTRL_H__2609BF15_AC0F_4D3D_ACE3_F51E51327BD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "SortListCtrl.h"
#include "Player.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNetworkListCtrl window

class CNetworkListCtrl : public CSortListCtrl
{
// Construction
public:
	CNetworkListCtrl();

// Attributes
public:

// Operations
public:

	void addPlayer(CPlayer *pPlayer, int iColor);
	void removePlayer(UINT uiID);

	void updatePlayerColor(UINT uiID, int iColor);
	void updatePing(UINT uiID, long lPing);
	void updateJoin(UINT uiID, BOOL bIn);
	void updateReady(UINT uiID, BOOL bReady);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNetworkListCtrl();

	// Generated message map functions
protected:
	//the bold font of the list control
	CFont m_fontBold;

	//find a player by ID
	int findPlayer(UINT uiID);

	//{{AFX_MSG(CNetworkListCtrl)
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;


	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKLISTCTRL_H__2609BF15_AC0F_4D3D_ACE3_F51E51327BD2__INCLUDED_)
