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
#include "MapDatabaseDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// global function declaration
/////////////////////////////////////////////////////////////////////////////
void OnMapDatabaseClick(DWORD dwCookie);

/////////////////////////////////////////////////////////////////////////////
// CMapDatabaseDialog dialog


CMapDatabaseDialog::CMapDatabaseDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CMapDatabaseDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMapDatabaseDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//set selection default to true
	m_bSelect = TRUE;
}


void CMapDatabaseDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapDatabaseDialog)
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_IMPORT_BUTTON, m_ImportButton);
	DDX_Control(pDX, IDC_EXPORT_BUTTON, m_ExportButton);
	DDX_Control(pDX, IDC_MAP_LIST, m_MapList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapDatabaseDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CMapDatabaseDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapDatabaseDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// start up
/////////////////////////////////////////////////////////////////////////////
BOOL CMapDatabaseDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_DBMAPS;
	
	//center ourselves
	CenterWindow();

	//set various thing based on mode
	if(TRUE == m_bSelect)
	{
		SetWindowText("Select a map to play");
		m_ImportButton.ShowWindow(SW_HIDE);
		m_ExportButton.ShowWindow(SW_HIDE);
		m_OKButton.EnableWindow(FALSE);
	}
	else
	{
		m_OKButton.SetWindowText("Close");
		m_CancelButton.ShowWindow(SW_HIDE);
	}

	//set the callback function
	m_MapList.setCallBack(OnMapDatabaseClick, (DWORD) this);

	//handle tour
	if(TRUE == APP->inTour())
	{
		//set up the hotseat balloon
		m_balloonList.Create("Sea3D Tour",
		"In order to play Sea3D, you need a map to play on!  In this screen, Sea3D\nallows you to select which map you would like to use for the current game.\n\nClick on the Standard Settlers 3-4 player map.\n\nPress Esc to cancel the tour.", 
		CPoint(200, 25),
		CBalloonHelp::unSHOW_INNER_SHADOW,
		&m_MapList);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// handle map selection
/////////////////////////////////////////////////////////////////////////////
void CMapDatabaseDialog::handleSelection()
{
	int nSel;

	//get the selection
	nSel = m_MapList.querySelection();
	
	//enable the windows
	if(TRUE == m_bSelect)
	{
		m_OKButton.EnableWindow(nSel != -1);
	}

	//check for tour
	if((TRUE == APP->inTour()) && (0 == nSel))
	{
		//close the old one
		if(NULL != m_balloonList.m_hWnd)
		{
			m_balloonList.SendMessage(WM_CLOSE);
		}

		//set up the hotseat balloon
		if(NULL == m_balloonOK.m_hWnd)
		{
			m_balloonOK.Create("Sea3D Tour",
			"Now click OK to continue on.\n\nPress Esc to cancel the tour.", 
			CPoint(25, 5),
			CBalloonHelp::unSHOW_INNER_SHADOW,
			&m_OKButton);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// set the selecte value
/////////////////////////////////////////////////////////////////////////////
void CMapDatabaseDialog::OnOK() 
{
	int nSel;

	//get selection
	nSel = m_MapList.querySelection();

	if((TRUE == m_bSelect) && (-1 == nSel))
	{
		AfxMessageBox("You must select a map to continue.");
		return;
	}

	//get the selected map
	if(TRUE == m_bSelect)
	{
		m_iMap = m_MapList.GetItemData(nSel);
	}
	
	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// turn off the tour
/////////////////////////////////////////////////////////////////////////////
void CMapDatabaseDialog::OnCancel() 
{
	APP->setTour(FALSE);
	
	CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// callback handler
/////////////////////////////////////////////////////////////////////////////
void OnMapDatabaseClick(DWORD dwCookie)
{
	if(NULL != dwCookie)
	{
		((CMapDatabaseDialog *) dwCookie)->handleSelection();
	}
}