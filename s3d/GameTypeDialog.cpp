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
#include "SettlersView.h"
#include "GameTypeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameTypeDialog dialog
/////////////////////////////////////////////////////////////////////////////
CGameTypeDialog::CGameTypeDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CGameTypeDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGameTypeDialog)
	//}}AFX_DATA_INIT

	m_iMode = 0;
}


void CGameTypeDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameTypeDialog)
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDC_HOTSEAT_RADIO, m_HotseatRadio);
	DDX_Control(pDX, IDC_NETWORK_RADIO, m_NetworkRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGameTypeDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CGameTypeDialog)
	ON_BN_CLICKED(IDC_HOTSEAT_RADIO, OnHotseatRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameTypeDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// initialization
/////////////////////////////////////////////////////////////////////////////
BOOL CGameTypeDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_GAMETYPE;
	
	//set default
	m_NetworkRadio.SetCheck(TRUE);

	//make sure we're centered
	CenterWindow();

	//if we're in tour mode, start up the balloon stuff
	if(TRUE == APP->inTour())
	{
		//set up the hotseat balloon
		m_balloonHotseat.Create("Sea3D Tour",
		"For the tour, we'll start off playing a hotseat game of Sea3D, where everyone\nis seated at the same computer.  You also have the option of tracking\na tabletop game or playing on a network, but for now, we'll stick with hotseat.\n\nSelect the 'Play a hotseat Sea3D game' option.\n\nPress Esc to cancel the tour.", 
		CPoint(5, 3),
		CBalloonHelp::unSHOW_INNER_SHADOW,
		&m_HotseatRadio);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// set the game type
/////////////////////////////////////////////////////////////////////////////
void CGameTypeDialog::OnOK() 
{
	//set the game mode
	if(TRUE == m_HotseatRadio.GetCheck())
	{
		m_iMode = GAME_MODE_HOTSEAT;
	}
	else if(TRUE == m_NetworkRadio.GetCheck())
	{
		m_iMode = GAME_MODE_NETWORK;
	}
	
	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// continue the tour
/////////////////////////////////////////////////////////////////////////////
void CGameTypeDialog::OnHotseatRadio() 
{
	if(FALSE == APP->inTour())
	{
		return;
	}

	//close the hotseat balloon
	if(NULL != m_balloonHotseat.m_hWnd)
	{
		m_balloonHotseat.SendMessage(WM_CLOSE);
	}
	
	if(NULL == m_balloonOK.m_hWnd)
	{
		//set up the hotseat balloon
		m_balloonOK.Create("Sea3D Tour",
		"Now click OK to continue on.\n\nPress Esc to cancel the tour.", 
		CPoint(25, 5),
		CBalloonHelp::unSHOW_INNER_SHADOW,
		&m_OKButton);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a cancel
/////////////////////////////////////////////////////////////////////////////
void CGameTypeDialog::OnCancel() 
{
	APP->setTour(FALSE);
	
	CHelpDialog::OnCancel();
}
