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
#include "WinDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinDialog dialog


CWinDialog::CWinDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CWinDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWinDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWinDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWinDialog)
	DDX_Control(pDX, IDC_WIN_STATIC, m_WinStatic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWinDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CWinDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// set the text properly
/////////////////////////////////////////////////////////////////////////////
BOOL CWinDialog::OnInitDialog() 
{
	CString str;

	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_GL;
	
	//initialize the string
	str.Format("%s has won the game!", m_strWinner);

	//set the string
	m_WinStatic.SetWindowText(str);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}