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
#include "TradeBankDialog.h"
#include "SettlersView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTradeBankDialog dialog


CTradeBankDialog::CTradeBankDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CTradeBankDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTradeBankDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTradeBankDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTradeBankDialog)
	DDX_Control(pDX, IDC_QUESTION_STATIC, m_QuestionStatic);
	DDX_Control(pDX, IDC_TIMBER_RADIO, m_TimberRadio);
	DDX_Control(pDX, IDC_WHEAT_RADIO, m_WheatRadio);
	DDX_Control(pDX, IDC_ORE_RADIO,	m_OreRadio);
	DDX_Control(pDX, IDC_CLAY_RADIO, m_ClayRadio);
	DDX_Control(pDX, IDC_SHEEP_RADIO, m_SheepRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTradeBankDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CTradeBankDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTradeBankDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// handle start up
/////////////////////////////////////////////////////////////////////////////
BOOL CTradeBankDialog::OnInitDialog() 
{
	int i;
	int iCheck = -1;
	CString str;
	CButton *pButton;

	//normal initialization
	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_GAMEPLAY;
	
	//run through the list and enable radios
	for(i = 4; i >= 0; i--)
	{
		//set the button
		pButton = &m_TimberRadio + i;

		//enable or disable
		pButton->EnableWindow(m_iRes[i]);

		//set the check
		if(m_iRes[i])
		{
			iCheck = i;
		}
	}

	//set one check
	if(-1 != iCheck)
	{
		pButton = &m_TimberRadio + iCheck;
		pButton->SetCheck(TRUE);
	}

	//set the static text
	str.Format("Which resource will you trade to get a %s?", m_strRes);
	m_QuestionStatic.SetWindowText(str);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// disable cancel
/////////////////////////////////////////////////////////////////////////////
void CTradeBankDialog::OnCancel() 
{
	if(TRUE == VIEW->isWaiting())
	{
		CHelpDialog::OnCancel();
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle an ok click
/////////////////////////////////////////////////////////////////////////////
void CTradeBankDialog::OnOK() 
{
	int i;
	CButton *pButton;
	
	//run through the list and enable radios
	for(i = 0; i < 5; i++)
	{
		//set the button
		pButton = &m_TimberRadio + i;

		//see if it's checked
		if(pButton->GetCheck())
		{
			//set the type and break
			m_iType = i;
			break;
		}
	}

	CHelpDialog::OnOK();
}