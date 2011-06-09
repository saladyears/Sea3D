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
#include "LegendDialog.h"
#include "defineIni.h"
#include "defineDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLegendDialog dialog


CLegendDialog::CLegendDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CLegendDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLegendDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLegendDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLegendDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLegendDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CLegendDialog)
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLegendDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CLegendDialog::OnInitDialog() 
{
	int x, y;
	int iDifX;
	int iDifY;
	int cx, cy;
	int iStyle;
	BOOL bLarge;
	CRect rect;
	CRect rectClient;

	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_LEGEND;

	//get large size
	bLarge = GetPrivateProfileInt(INI_GENERAL, INI_LARGE_FONTS, FALSE, INI_FILE);

	//read offset values
	x = GetPrivateProfileInt(INI_DISPLAY, INI_LEGEND_OFFSET_X, -1, INI_FILE);
	y = GetPrivateProfileInt(INI_DISPLAY, INI_LEGEND_OFFSET_Y, -1, INI_FILE);

	//get screen size
	GetWindowRect(&rect);

	//get client rect
	GetClientRect(&rectClient);

	//calculate height dif
	iDifX = rect.Width() - rectClient.Width();
	iDifY = rect.Height() - rectClient.Height();

	//set values
	if(FALSE == bLarge)
	{
		cx = BANK_BUTTON_WIDTH;
		cy = LEGEND_BUTTON_HEIGHT;
	}
	else
	{
		cx = LARGE_BANK_BUTTON_WIDTH;
		cy = LARGE_LEGEND_BUTTON_HEIGHT;
	}

	//set rect values
	rect = CRect(0, 0, cx, cy);
	
	//set style
	iStyle = SWP_NOZORDER;
	if(-1 == x || -1 == y)
	{
		iStyle |= SWP_NOMOVE;
	}

	//set initial window position
	SetWindowPos(&wndTop, x, y, cx + iDifX, cy + iDifY, iStyle);

	//create legend button
	m_legend.Create("", CHILD, rect, this, 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// remember offsets
/////////////////////////////////////////////////////////////////////////////
void CLegendDialog::OnMove(int x, int y) 
{
	CString str;

	CHelpDialog::OnMove(x, y);
	
	//update ini file
	str.Format("%d", x);
	WritePrivateProfileString(INI_DISPLAY, INI_LEGEND_OFFSET_X, str, INI_FILE);

	str.Format("%d", y);
	WritePrivateProfileString(INI_DISPLAY, INI_LEGEND_OFFSET_Y, str, INI_FILE);
	
}

/////////////////////////////////////////////////////////////////////////////
// update main menu
/////////////////////////////////////////////////////////////////////////////
void CLegendDialog::OnCancel() 
{
	hideMenu();

	//normal handling
	CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// update main menu
/////////////////////////////////////////////////////////////////////////////
void CLegendDialog::OnOK()
{
	hideMenu();

	//normal handling
	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// update main menu
/////////////////////////////////////////////////////////////////////////////
void CLegendDialog::hideMenu()
{
	CMenu *pMenu;
	CMenu *pSubmenu;

	//get the current menu
	pMenu = AfxGetMainWnd()->GetMenu();

	//get the submenu
	pSubmenu = pMenu->GetSubMenu(3);

	//set the checked status
	pSubmenu->CheckMenuItem(2, MF_UNCHECKED | MF_BYPOSITION);
}