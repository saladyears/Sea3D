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
#include "MonopolyCardDialog.h"
#include "SettlersView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonopolyCardDialog dialog


CMonopolyCardDialog::CMonopolyCardDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CMonopolyCardDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonopolyCardDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMonopolyCardDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonopolyCardDialog)
	DDX_Control(pDX, IDC_TIMBER_RADIO, m_TimberRadio);
	DDX_Control(pDX, IDC_WHEAT_RADIO, m_WheatRadio);
	DDX_Control(pDX, IDC_ORE_RADIO, m_OreRadio);
	DDX_Control(pDX, IDC_CLAY_RADIO, m_ClayRadio);
	DDX_Control(pDX, IDC_SHEEP_RADIO, m_SheepRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonopolyCardDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CMonopolyCardDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonopolyCardDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// set the type
/////////////////////////////////////////////////////////////////////////////
void CMonopolyCardDialog::OnOK() 
{
	if(m_TimberRadio.GetCheck()) m_iType = RES_TIMBER;
	else if(m_WheatRadio.GetCheck()) m_iType = RES_WHEAT;
	else if(m_OreRadio.GetCheck()) m_iType = RES_ORE;
	else if(m_ClayRadio.GetCheck()) m_iType = RES_CLAY;
	else if(m_SheepRadio.GetCheck()) m_iType = RES_SHEEP;
	
	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// initialization
/////////////////////////////////////////////////////////////////////////////
BOOL CMonopolyCardDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_GAMEPLAY;
	
	m_TimberRadio.SetCheck(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// make sure the turn shows
/////////////////////////////////////////////////////////////////////////////
void CMonopolyCardDialog::OnCancel() 
{
	VIEW->showBuildOptions(TRUE);
	
	CHelpDialog::OnCancel();
}
