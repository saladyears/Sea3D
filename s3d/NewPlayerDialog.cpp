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
#include "NewPlayerDialog.h"
#include "SettlersView.h"
#include "ConnectorRegisterDlg.h"
#include "ConnectorDownloadDlg.h"
#include "defineNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewPlayerDialog dialog


CNewPlayerDialog::CNewPlayerDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNewPlayerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewPlayerDialog)
	//}}AFX_DATA_INIT

	//default to non edit mode
	m_bEdit = FALSE;

	m_bFirst = FALSE;

	m_iSel = -1;

	m_bLadderNow = FALSE;

	//set the player as being made here
	m_Player.setMadeHere(TRUE);
}


void CNewPlayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewPlayerDialog)
	DDX_Control(pDX, IDC_UPDATE_BUTTON, m_UpdateButton);
	DDX_Control(pDX, IDC_DOWNLOAD_BUTTON, m_DownloadButton);
	DDX_Control(pDX, IDC_UNREGISTER_BUTTON, m_UnregisterButton);
	DDX_Control(pDX, IDC_REGISTER_BUTTON, m_RegisterButton);
	DDX_Control(pDX, IDC_LADDER_CHECK, m_LadderCheck);
	DDX_Control(pDX, IDC_LASTNAME_STATIC, m_LastNameStatic);
	DDX_Control(pDX, IDC_HANDLE_STATIC, m_HandleStatic);
	DDX_Control(pDX, IDC_ONENAME_CHECK, m_OneNameCheck);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDC_LANG_COMBO, m_LangCombo);
	DDX_Control(pDX, IDC_LANG1, m_Lang1Check);
	DDX_Control(pDX, IDC_LANG2, m_Lang2Check);
	DDX_Control(pDX, IDC_LANG3, m_Lang3Check);
	DDX_Control(pDX, IDC_LANG4, m_Lang4Check);
	DDX_Control(pDX, IDC_LANG5, m_Lang5Check);
	DDX_Control(pDX, IDC_LANG6, m_Lang6Check);
	DDX_Control(pDX, IDC_LANG7, m_Lang7Check);
	DDX_Control(pDX, IDC_LANG8, m_Lang8Check);
	DDX_Control(pDX, IDC_LANG9, m_Lang9Check);
	DDX_Control(pDX, IDC_LANG10, m_Lang10Check);
	DDX_Control(pDX, IDC_LANG11, m_Lang11Check);
	DDX_Control(pDX, IDC_LANG12, m_Lang12Check);
	DDX_Control(pDX, IDC_LANG13, m_Lang13Check);
	DDX_Control(pDX, IDC_LANG14, m_Lang14Check);
	DDX_Control(pDX, IDC_LANG15, m_Lang15Check);
	DDX_Control(pDX, IDC_LANG16, m_Lang16Check);
	DDX_Control(pDX, IDC_COLOR2_COMBO, m_Color2Combo);
	DDX_Control(pDX, IDC_COLOR1_COMBO, m_Color1Combo);
	DDX_Control(pDX, IDC_LASTNAME_EDIT, m_LastNameCtrl);
	DDX_Control(pDX, IDC_FIRSTNAME_EDIT, m_FirstNameCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewPlayerDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNewPlayerDialog)
	ON_CBN_SELCHANGE(IDC_LANG_COMBO, OnSelchangeLangCombo)
	ON_BN_CLICKED(IDC_ONENAME_CHECK, OnOnenameCheck)
	ON_BN_CLICKED(IDC_LADDER_CHECK, OnLadderCheck)
	ON_BN_CLICKED(IDC_REGISTER_BUTTON, OnRegisterButton)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DOWNLOAD_BUTTON, OnBnClickedDownloadButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPlayerDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CNewPlayerDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_ADDPLAYER;

	//init
	initDialog();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// setup the dialog
/////////////////////////////////////////////////////////////////////////////
void CNewPlayerDialog::initDialog()
{
	int i;
	BOOL bLadder;
	CString strLadder;
	CString strLang;
	CButton *pButton;

	//set values
	m_OneNameCheck.SetCheck(m_Player.getHandle());

	//hide others if needed
	if(TRUE == m_Player.getHandle())
	{
		m_LastNameStatic.ShowWindow(SW_HIDE);
		m_LastNameCtrl.ShowWindow(SW_HIDE);
		m_HandleStatic.SetWindowText("Handle:");
	}

	//set values
	m_FirstNameCtrl.SetWindowText(m_Player.getFirstName());
	m_LastNameCtrl.SetWindowText(m_Player.getLastName());

	//set colors
	m_Color1Combo.SetCurSel(m_Player.getColor1());
	m_Color2Combo.SetCurSel(m_Player.getColor2());
	
	//set window text appropriately
	SetWindowText(m_bEdit ? "Edit a player" : "Add a player");

	//set the focus correctly
	m_FirstNameCtrl.SetFocus();
	
	//add the languages
	for(i = 0; i < LANGUAGES_SIZE; i++)
	{
		//get the check box
		pButton = &m_Lang1Check + i;

		//get the text
		strLang = VIEW->getLangName(1 << i);
		
		//add it
		m_LangCombo.AddString(strLang);
		pButton->SetWindowText(strLang);

		//set languages
		if(m_Player.getLanguage() == (1 << i))
		{
			m_LangCombo.SetCurSel(i);
			pButton->EnableWindow(FALSE);
			m_iSel = i;
		}

		//set checks
		if(m_Player.getKnown() & (1 << i))
		{
			pButton->SetCheck(TRUE);
		}
	}

	//make sure we're centered
	CenterWindow();

	//set up the balloon help
	if(TRUE == APP->inTour())
	{
		if(TRUE == m_bFirst)
		{
			//set up the balloon
			m_balloonOK.Create("Sea3D Tour",
			"Creating a player is easy.  Just fill in the first and last names of the player and you're done.\nThe other settings are for playing in a network game.  You can always come back and edit\nyour player later if you want to play in a network game.\n\nFill out the player information and click OK to continue.\n\nPress Esc to cancel the tour.", 
			CPoint(5, 20),
			CBalloonHelp::unSHOW_INNER_SHADOW,
			&m_LastNameCtrl);
		}
		else
		{
			//set up the balloon
			m_balloonOK.Create("Sea3D Tour",
			"It's the same process every time... just do what you did last time.\n\nFill out the player information and click OK to continue.\n\nPress Esc to cancel the tour.", 
			CPoint(5, 20),
			CBalloonHelp::unSHOW_INNER_SHADOW,
			&m_LastNameCtrl);
		}
	}

	//get the player's ladder settings
	bLadder = m_Player.getLadder();
	strLadder = m_Player.getLadderID();

	//if it's a ladder player, they can only update the with button below
	if(TRUE == bLadder)
	{
		m_HandleStatic.EnableWindow(FALSE);
		m_FirstNameCtrl.EnableWindow(FALSE);
		m_LastNameStatic.EnableWindow(FALSE);
		m_LastNameCtrl.EnableWindow(FALSE);
		m_OneNameCheck.EnableWindow(FALSE);
	}

	//set the ladder checks and buttons
	m_LadderCheck.SetCheck(bLadder);
	m_LadderCheck.EnableWindow(!bLadder);
	m_RegisterButton.EnableWindow(FALSE);
	m_UpdateButton.EnableWindow(bLadder);
	m_DownloadButton.EnableWindow(!bLadder);
	m_UnregisterButton.EnableWindow(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// handle them clicking on the ladder check
/////////////////////////////////////////////////////////////////////////////
void CNewPlayerDialog::OnLadderCheck() 
{
	m_RegisterButton.EnableWindow(m_LadderCheck.GetCheck());	
}

/////////////////////////////////////////////////////////////////////////////
// handle registering the player
/////////////////////////////////////////////////////////////////////////////
void CNewPlayerDialog::OnRegisterButton() 
{
	CString str;
	CConnectorRegisterDlg dlg;

	//check name
	if(FALSE == validateName())
	{
		return;
	}

	//set the player info
	dlg.setPlayer(m_Player);

	//run the dialog
	if(IDOK == dlg.DoModal())
	{
		m_Player = dlg.getPlayer();

		//reinit
		initDialog();

		//they created a ladder player just now
		m_bLadderNow = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle download
/////////////////////////////////////////////////////////////////////////////
void CNewPlayerDialog::OnBnClickedDownloadButton()
{
	CConnectorDownloadDlg dlg;

	//get the name into the data structure
	validateName(FALSE);

	//set the player info
	dlg.setPlayer(m_Player);

	//run the dialog
	if(IDOK == dlg.DoModal())
	{
		m_Player = dlg.getPlayer();

		//reinit
		initDialog();

		//they created a ladder player just now
		m_bLadderNow = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// when they switch to using a handle, update the screen
/////////////////////////////////////////////////////////////////////////////
void CNewPlayerDialog::OnOnenameCheck() 
{
	BOOL bHandle;

	//get the value
	bHandle = m_OneNameCheck.GetCheck();

	//do stuff
	if(TRUE == bHandle)
	{
		m_LastNameStatic.ShowWindow(SW_HIDE);
		m_LastNameCtrl.ShowWindow(SW_HIDE);
		m_HandleStatic.SetWindowText("Handle:");
	}
	else
	{
		m_LastNameStatic.ShowWindow(SW_SHOW);
		m_LastNameCtrl.ShowWindow(SW_SHOW);
		m_HandleStatic.SetWindowText("First Name:");
		m_LastNameCtrl.SetWindowText("");
	}
}

/////////////////////////////////////////////////////////////////////////////
// when they switch primary languages, automatically turn on the check
/////////////////////////////////////////////////////////////////////////////
void CNewPlayerDialog::OnSelchangeLangCombo() 
{
	int iSel;
	CButton *pButton;

	//get the current selection
	iSel = m_LangCombo.GetCurSel();

	//reenable the old
	if(0 <= m_iSel)
	{
		pButton = &m_Lang1Check + m_iSel;
		pButton->EnableWindow();
	}
	
	//set the check
	pButton = &m_Lang1Check + iSel;
	pButton->SetCheck(TRUE);
	pButton->EnableWindow(FALSE);
	m_iSel = iSel;
}

/////////////////////////////////////////////////////////////////////////////
// see if we've got a valid name
/////////////////////////////////////////////////////////////////////////////
BOOL CNewPlayerDialog::validateName(BOOL bEmpty)
{
	CString str;

	//set the handle value
	m_Player.setHandle(m_OneNameCheck.GetCheck());

	//get first name
	m_FirstNameCtrl.GetWindowText(str);

	//validate that they have a first name
	if((TRUE == str.IsEmpty()) && (TRUE == bEmpty))
	{
		MessageBox("The new player must at least have a first name or handle.", "New Player");
		return FALSE;
	}

	m_Player.setFirstName(str);

	//last name
	m_LastNameCtrl.GetWindowText(str);
	m_Player.setLastName(str);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// set new stuff
/////////////////////////////////////////////////////////////////////////////
void CNewPlayerDialog::OnOK() 
{
	int i;
	int iSel;
	CString str;
	CButton *pButton;

	//check name
	if(FALSE == validateName())
	{
		return;
	}
	
	//get colors
	m_Player.setColor1(m_Color1Combo.GetCurSel());
	m_Player.setColor2(m_Color2Combo.GetCurSel());

	//get language
	iSel = m_LangCombo.GetCurSel();
	m_Player.setLanguage(1 << iSel);

	//get known
	for(i = 0, iSel = 0; i < LANGUAGES_SIZE; i++)
	{
		pButton = &m_Lang1Check + i;

		//check for check
		if(TRUE == pButton->GetCheck())
		{
			iSel |= (1 << i);	
		}
	}

	//set the known
	m_Player.setKnown(iSel);

	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// handle cancel
/////////////////////////////////////////////////////////////////////////////
void CNewPlayerDialog::OnCancel() 
{
	APP->setTour(FALSE);

	//see if they really want to
	if(TRUE == m_bLadderNow)
	{
		if(IDNO == MessageBox("You just added this player to s3dconnector.net!  If you cancel now, you will have to download this player\nfrom s3dconnector.net to use them.  Are you sure you want to do this?", "Sea3D Connector", MB_YESNO))
		{
			return;
		}
	}
	
	CHelpDialog::OnCancel();
}
