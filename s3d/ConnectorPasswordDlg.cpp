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
#include "Settlers.h"
#include "ConnectorPasswordDlg.h"


// CConnectorPasswordDlg dialog

IMPLEMENT_DYNAMIC(CConnectorPasswordDlg, CHelpDialog)
CConnectorPasswordDlg::CConnectorPasswordDlg(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CConnectorPasswordDlg::IDD, pParent)
	, m_strPassword(_T(""))
{
	m_strHelp = HELP_CONNECTOR_PASSWORD;
}

CConnectorPasswordDlg::~CConnectorPasswordDlg()
{
}

void CConnectorPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PASSWORD_EDIT, m_PasswordEdit);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_strPassword);
}


BEGIN_MESSAGE_MAP(CConnectorPasswordDlg, CHelpDialog)
END_MESSAGE_MAP()


// CConnectorPasswordDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CConnectorPasswordDlg::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();
	
	//set focus
	m_PasswordEdit.SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

