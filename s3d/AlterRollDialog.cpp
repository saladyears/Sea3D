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
#include "AlterRollDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlterRollDialog dialog


CAlterRollDialog::CAlterRollDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAlterRollDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlterRollDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAlterRollDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlterRollDialog)
	DDX_Control(pDX, IDC_ROLL_EDIT, m_RollEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlterRollDialog, CDialog)
	//{{AFX_MSG_MAP(CAlterRollDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlterRollDialog message handlers

BOOL CAlterRollDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_RollEdit.SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CAlterRollDialog::OnOK() 
{
	int nRoll;
	CString str;
	
	//get the value
	m_RollEdit.GetWindowText(str);

	//sanity
	if(TRUE == str.IsEmpty())
	{
		return;
	}

	//get the value
	nRoll = atoi(str);

	//sanity
	if((2 > nRoll) || (12 < nRoll))
	{
		return;
	}

	//set it
	APP->setNextRoll(nRoll);
	
	CDialog::OnOK();
}