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
#include "NetworkPasswordDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkPasswordDialog dialog


CNetworkPasswordDialog::CNetworkPasswordDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNetworkPasswordDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkPasswordDialog)
	m_strPassword = _T("");
	//}}AFX_DATA_INIT

	m_strHelp = HELP_GL;
}


void CNetworkPasswordDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkPasswordDialog)
	DDX_Control(pDX, IDC_PASSWORD_EDIT, m_PasswordEdit);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkPasswordDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNetworkPasswordDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkPasswordDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkPasswordDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();
	
	//set focus
	m_PasswordEdit.SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
