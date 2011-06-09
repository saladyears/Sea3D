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
#include "NetworkPortDialog.h"
#include "define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkPortDialog dialog


CNetworkPortDialog::CNetworkPortDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNetworkPortDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkPortDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//default port
	m_uiPort = PORT;
}


void CNetworkPortDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkPortDialog)
	DDX_Control(pDX, IDC_PORT_EDIT, m_PortEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkPortDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNetworkPortDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkPortDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkPortDialog::OnInitDialog() 
{
	CString str;

	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_NETPORT;
	
	//set the port
	str.Format("%d", m_uiPort);
	m_PortEdit.SetWindowText(str);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// finish up
/////////////////////////////////////////////////////////////////////////////
void CNetworkPortDialog::OnOK() 
{
	CString str;

	//get the port
	m_PortEdit.GetWindowText(str);

	//get the port
	m_uiPort = atoi(str);
	
	CHelpDialog::OnOK();
}
