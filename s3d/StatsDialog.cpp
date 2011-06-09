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
#include "StatsDialog.h"
#include "defineStat.h"
#include "defineColors.h"
#include "defineIni.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatsDialog dialog


CStatsDialog::CStatsDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CStatsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStatsDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatsDialog)
	DDX_Control(pDX, IDC_STAT_TREE, m_StatTree);
	DDX_GridControl(pDX, IDC_GRID, m_grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatsDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CStatsDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_STAT_TREE, OnSelchangedStatTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatsDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CStatsDialog::OnInitDialog() 
{
	int i;
	BOOL bHead;
	CRect rect;
	HTREEITEM hParent = NULL, hItem;
	CString strName;

	CHelpDialog::OnInitDialog();
	
	//help system
	m_strHelp = HELP_STATS;

	//subclass the window
	m_grid.Initialize();

	//set various colors
	m_StatTree.SetBkColor(COLOR_BLACK);
	m_StatTree.SetTextColor(COLOR_WHITE);

	//add names to the tree
	for(i = 0; i < STAT_SIZE; i++)
	{
		//get the name of the stat
		strName = getName(i, bHead);

		//add it to the tree
		if(bHead)
		{
			//insert the item
			hParent = m_StatTree.InsertItem(strName);

			//set item data
			m_StatTree.SetItemData(hParent, i);
		}
		else
		{
			//insert the item
			hItem = m_StatTree.InsertItem(strName, hParent);

			//set item data
			m_StatTree.SetItemData(hItem, i);
		}
	}

	//get the rect
	m_StatTree.GetClientRect(&rect);

	//get the size of the tree control
	m_iWidth = rect.Width();

	int w, h, x, y;
	w = GetPrivateProfileInt(INI_DISPLAY, INI_STAT_SIZE_X, 500, INI_FILE);
	h = GetPrivateProfileInt(INI_DISPLAY, INI_STAT_SIZE_Y, 100, INI_FILE);
	x = GetPrivateProfileInt(INI_DISPLAY, INI_STAT_OFFSET_X, 0, INI_FILE);
	y = GetPrivateProfileInt(INI_DISPLAY, INI_STAT_OFFSET_Y, 0, INI_FILE);
	SetWindowPos(&wndTop, x, y, w, h, SWP_HIDEWINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// name retrieval
/////////////////////////////////////////////////////////////////////////////
CString CStatsDialog::getName(int iStat, BOOL &bHead)
{
	//default to false
	bHead = FALSE;

	//get the name
	switch(iStat)
	{
	case STAT_HEAD_ROLL: bHead = TRUE; return "Dice rolls";
	case STAT_ROLL_FREQ: return "Frequencies";
	case STAT_ROLL_PLAYER: return "Frequencies by player";
	case STAT_HEAD_TIME: bHead = TRUE; return "Time";
	case STAT_TIME_TURN: return "Time per turn";
	case STAT_HEAD_GOODS_RECV: bHead = TRUE; return "Resources Gained";
	case STAT_GOODS_RECV_TOTAL: return "Total resources gained";
	case STAT_GOODS_RECV_BOARD: return "From board";
	case STAT_GOODS_RECV_STEAL: return "By stealing";
	case STAT_GOODS_RECV_DEVCARD: return "By development cards";
	case STAT_GOODS_RECV_GOLD: return "By gold";
	case STAT_GOODS_RECV_DISCOVER: return "By discovery";
	case STAT_HEAD_GOODS_LOST: bHead = TRUE; return "Resources Lost";
	case STAT_GOODS_LOST_TOTAL: return "Total resources lost";
	case STAT_GOODS_LOST_BOARD: return "From robber placement";
	case STAT_GOODS_LOST_STEAL: return "Stolen by others";
	case STAT_GOODS_LOST_7: return "From a 7 roll";
	case STAT_HEAD_TRADES: bHead = TRUE; return "Trading";
	case STAT_TRADE_NUMBER: return "Number of trades";
	case STAT_TRADE_NUM_OTHERS: return "Trading partners";
	case STAT_TRADE_RATIO: return "Trading ratios";
	case STAT_TRADE_TO_BANK: return "Resources traded to bank";	
	case STAT_TRADE_FROM_BANK: return "Resources received from bank";
	case STAT_TRADE_TO_OTHERS: return "Resources traded to others";
	case STAT_TRADE_FROM_OTHERS: return "Resources received from others";
	}

	//should never get here
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// handle a jump to a new stat block
/////////////////////////////////////////////////////////////////////////////
void CStatsDialog::OnSelchangedStatTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iBlock;
	HTREEITEM hItem;

	//set the pointer
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	//get the item
	hItem = pNMTreeView->itemNew.hItem;
	
	//jump to the new item
	if(hItem)
	{
		//get the block
		iBlock = m_StatTree.GetItemData(hItem);

		//jump
		m_grid.jumpTo(iBlock);
	}

	//set return value
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// resize the controls on a resize
/////////////////////////////////////////////////////////////////////////////
void CStatsDialog::OnSize(UINT nType, int cx, int cy) 
{
	CHelpDialog::OnSize(nType, cx, cy);
	CRect rect;

	//get size
	GetWindowRect(&rect);
	
	//resize the tree control
	if(m_StatTree.m_hWnd)
	{
		//resize it
		m_StatTree.SetWindowPos(&wndTop, 0, 0, m_iWidth, cy, SWP_SHOWWINDOW);
	}
	//resize the grid
	if(m_grid.m_hWnd)
	{
		//lock updates
		//m_grid.LockUpdate(TRUE);

		//resize it
		m_grid.SetWindowPos(&wndTop, m_iWidth + 1, 0, cx - m_iWidth - 1, cy, SWP_SHOWWINDOW);

		//lock updates
		//m_grid.LockUpdate(FALSE);

		//invalidate
		m_grid.Invalidate();
	}
}

/////////////////////////////////////////////////////////////////////////////
// update main menu
/////////////////////////////////////////////////////////////////////////////
void CStatsDialog::OnCancel() 
{
	hideMenu();

	//normal handling
	CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// update main menu
/////////////////////////////////////////////////////////////////////////////
void CStatsDialog::OnOK()
{
	hideMenu();

	//normal handling
	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// update main menu
/////////////////////////////////////////////////////////////////////////////
void CStatsDialog::hideMenu()
{
	// Store the size and location of the screen
	CRect rect;
	CString str;

	//get size
	GetWindowRect(&rect);
	TRACE1("left: %d\t", rect.left);
	TRACE1("top: %d\t", rect.top);
	TRACE1("width: %d\t", rect.Width());
	TRACE1("height: %d\n", rect.Height());

	//create string
	str.Format("%d", rect.Width());
	//set the new size
	WritePrivateProfileString(INI_DISPLAY, INI_STAT_SIZE_X, str, INI_FILE);

	//create string
	str.Format("%d", rect.Height());
	//set the new size
	WritePrivateProfileString(INI_DISPLAY, INI_STAT_SIZE_Y, str, INI_FILE);

	//create string
	str.Format("%d", rect.left);
	//set the new size
	WritePrivateProfileString(INI_DISPLAY, INI_STAT_OFFSET_X, str, INI_FILE);

	//create string
	str.Format("%d", rect.top);
	//set the new size
	WritePrivateProfileString(INI_DISPLAY, INI_STAT_OFFSET_Y, str, INI_FILE);

	// Now hide the menu...
	CMenu *pMenu;
	CMenu *pSubmenu;

	//get the current menu
	pMenu = AfxGetMainWnd()->GetMenu();

	//get the submenu
	pSubmenu = pMenu->GetSubMenu(3);

	//set the checked status
	pSubmenu->CheckMenuItem(1, MF_UNCHECKED | MF_BYPOSITION);
}

CStatsDialog::~CStatsDialog()
{
	hideMenu();
}



