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
#include "VictoryPointDialog.h"
#include "SettlersView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVictoryPointDialog dialog


CVictoryPointDialog::CVictoryPointDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CVictoryPointDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVictoryPointDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_iCards = 0;
}


void CVictoryPointDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVictoryPointDialog)
	DDX_Control(pDX, IDC_VICTORY_STATIC, m_VictoryStatic);
	DDX_Control(pDX, IDC_TWO_RADIO, m_TwoRadio);
	DDX_Control(pDX, IDC_THREE_RADIO, m_ThreeRadio);
	DDX_Control(pDX, IDC_ONE_RADIO, m_OneRadio);
	DDX_Control(pDX, IDC_NONE_RADIO, m_NoneRadio);
	DDX_Control(pDX, IDC_FOUR_RADIO, m_FourRadio);
	DDX_Control(pDX, IDC_5_RADIO, m_FiveRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVictoryPointDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CVictoryPointDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVictoryPointDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// initialization
/////////////////////////////////////////////////////////////////////////////
BOOL CVictoryPointDialog::OnInitDialog() 
{
	CString str;

	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_GL;

	//set the static text
	str.Format("How many Victory Points in %s's hand?", m_strName);
	m_VictoryStatic.SetWindowText(str);
	
	//turn on or off the radio buttons
	m_FiveRadio.EnableWindow(m_iCards >= 5);
	m_FourRadio.EnableWindow(m_iCards >= 4);
	m_ThreeRadio.EnableWindow(m_iCards >= 3);
	m_TwoRadio.EnableWindow(m_iCards >= 2);
	m_OneRadio.EnableWindow(m_iCards >= 1);

	//default check to none
	m_NoneRadio.SetCheck(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// no cancelling allowed
/////////////////////////////////////////////////////////////////////////////
void CVictoryPointDialog::OnCancel() 
{
}

/////////////////////////////////////////////////////////////////////////////
// set the outgoing point total
/////////////////////////////////////////////////////////////////////////////
void CVictoryPointDialog::OnOK() 
{
	//default to zero
	m_iPoints = 0;

	//run through the radios
	if(m_OneRadio.GetCheck()) m_iPoints = 1;
	else if(m_TwoRadio.GetCheck()) m_iPoints = 2;
	else if(m_ThreeRadio.GetCheck()) m_iPoints = 3;
	else if(m_FourRadio.GetCheck()) m_iPoints = 4;
	else if(m_FiveRadio.GetCheck()) m_iPoints = 5;
	
	CHelpDialog::OnOK();
}
