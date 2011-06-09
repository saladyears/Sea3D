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

#if !defined(AFX_UNDODIALOG_H__A0986264_764B_464A_8B16_BC9AAEB9A8A0__INCLUDED_)
#define AFX_UNDODIALOG_H__A0986264_764B_464A_8B16_BC9AAEB9A8A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UndoDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "Undo.h"
#include "UndoTrade.h"
#include "UndoMonopoly.h"
#include "UndoGold.h"
#include "NDKMessage.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CUndoDialog dialog

class CUndoDialog : public CHelpDialog
{
// Construction
public:
	CUndoDialog(CWnd* pParent = NULL);   // standard constructor
	~CUndoDialog();

	//add a new item to the tree
	void addItem(int iAction, CUndo *pUndo);

	//remove the last item from the tree
	void removeItem(void);

	//////////////////////////////////////////////////////////////////
	// network functions
	//////////////////////////////////////////////////////////////////
	void SerializeToMessage(CNDKMessage &msg);
	void SerializeFromMessage(CNDKMessage &msg);


// Dialog Data
	//{{AFX_DATA(CUndoDialog)
	enum { IDD = IDD_UNDO_DLG };
	CTreeCtrl	m_UndoTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUndoDialog)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//hide the menu check
	void hideMenu(void);

	//generates IL images
	void generateIL(void);

	//the current turn
	int m_iTurn;

	//whether we need a new turn or not
	BOOL m_bTurn;

	//the image list
	CImageList *m_pImages;

	//parent items
	HTREEITEM m_itemBoard;
	HTREEITEM m_itemChips;
	HTREEITEM m_itemPlayers;
	HTREEITEM m_itemPlacement;
	HTREEITEM m_itemPrevTurn;
	HTREEITEM m_itemTurn;

	//the last added item
	HTREEITEM m_itemLast;

	//inserts a turn into the tree
	void insertTurn(BOOL bFirst = FALSE);

	//inserts an item into the tree
	void insertItem(CString str, int iImage, HTREEITEM hParent);

	//add functions
	void addPlacement(BOOL bSettle, BOOL bShip = FALSE);
	void addRoll(BOOL bFirst, int iRoll);
	void addActionStart(void);
	void addAction(void);
	void addRoad(int ID);
	void addShip(int ID);
	void addMoveShip(int ID);
	void addCity(void);
	void addSettle(void);
	void addRobber(void);
	void addPirate(void);
	void addDevcard(void);
	void addVictory(int iPlayer);
	void addMonopoly(CUndoMonopoly *pMonopoly);
	void addRoadBuild(void);
	void addYearPlenty(int ID);
	void addSoldier(int iPlayer);
	void addTrade(CUndoTrade *pTrade);
	void addSteal(int ID);
	void addLoseCards(CUndoTrade *pLose);
	void endTurn(void);
	void addWin(int iPlayer);
	void addWaste(int ID);
	void addGold(CUndoGold *pGold);
	void addChitless(int ID);
	void addVolcano(void);
	void addJungle(CUndoTrade *pJungle);
	void addSpecialBuild(int nPlayer);
	void addSpecialBuildEnd(void);

	//get a certain image
	int getImage(int iColor, int iType);
	int getTurnImage(int iColor);

	// Generated message map functions
	//{{AFX_MSG(CUndoDialog)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);

	//outputs one node
	void output(CArchive &ar, HTREEITEM hItem, BOOL bHead = TRUE);

	//inputs one node
	void input(CArchive &ar);

	//write one node to the message
	void output(int &iStart, CNDKMessage &msg, HTREEITEM hItem, BOOL bHead = TRUE);
	void input(int &iStart, CNDKMessage &msg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNDODIALOG_H__A0986264_764B_464A_8B16_BC9AAEB9A8A0__INCLUDED_)
