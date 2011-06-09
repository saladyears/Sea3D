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
#include "ConnectorRegisterDlg.h"
#include "SettlersView.h"
#include "defineConnector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectorRegisterDlg dialog


CConnectorRegisterDlg::CConnectorRegisterDlg(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CConnectorRegisterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectorRegisterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strHelp = HELP_CONNECTOR_REGISTER;
}


void CConnectorRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectorRegisterDlg)
	DDX_Control(pDX, IDC_VALIDATE_BUTTON, m_ValidateButton);
	DDX_Control(pDX, IDC_STEP2_STATIC, m_Step2Static);
	DDX_Control(pDX, IDC_STEP1_STATIC, m_Step1Static);
	DDX_Control(pDX, IDC_REGISTER_STATUS_STATIC, m_RegisterStatusStatic);
	DDX_Control(pDX, IDC_REGISTER_BUTTON, m_RegisterButton);
	DDX_Control(pDX, IDC_PASSWORD_STATIC, m_PasswordStatic);
	DDX_Control(pDX, IDC_PASSWORD_EDIT, m_PasswordEdit);
	DDX_Control(pDX, IDC_ONENAME_CHECK, m_OneNameCheck);
	DDX_Control(pDX, IDC_NAME_STATUS_STATIC, m_NameStatusStatic);
	DDX_Control(pDX, IDC_LASTNAME_STATIC, m_LastNameStatic);
	DDX_Control(pDX, IDC_LASTNAME_EDIT, m_LastNameEdit);
	DDX_Control(pDX, IDC_HANDLE_STATIC, m_HandleStatic);
	DDX_Control(pDX, IDC_FIRSTNAME_EDIT, m_FirstNameEdit);
	DDX_Control(pDX, IDC_CONFIRM_STATIC, m_ConfirmStatic);
	DDX_Control(pDX, IDC_CONFIRM_EDIT, m_ConfirmEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectorRegisterDlg, CHelpDialog)
	//{{AFX_MSG_MAP(CConnectorRegisterDlg)
	ON_BN_CLICKED(IDC_ONENAME_CHECK, OnOnenameCheck)
	ON_BN_CLICKED(IDC_VALIDATE_BUTTON, OnValidateButton)
	ON_BN_CLICKED(IDC_REGISTER_BUTTON, OnRegisterButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectorRegisterDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CConnectorRegisterDlg::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();

	//clear status messages
	m_NameStatusStatic.SetWindowText("");
	m_RegisterStatusStatic.SetWindowText("");

	//set values
	m_OneNameCheck.SetCheck(m_player.getHandle());

	//hide others if needed
	if(TRUE == m_player.getHandle())
	{
		m_LastNameStatic.ShowWindow(SW_HIDE);
		m_LastNameEdit.ShowWindow(SW_HIDE);
		m_HandleStatic.SetWindowText("Handle:");
	}

	//set values
	m_FirstNameEdit.SetWindowText(m_player.getFirstName());
	m_LastNameEdit.SetWindowText(m_player.getLastName());
	
	//run the name through
	validateName();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// check for a valid name
/////////////////////////////////////////////////////////////////////////////
void CConnectorRegisterDlg::validateName()
{
	int nResult;
	CString str;

	//get the name of the player
	str = m_player.getName();

	//check for a valid length
	if(CONNECTOR_NAME_MAX_LENGTH < str.GetLength())
	{
		MessageBox("The name you selected, " + str + ", is too long.  Please shorten it and try again.", "Sea3D Connector");
		return;
	}
	else if(-1 != str.FindOneOf("|'\""))
	{
		MessageBox("Your name may not contain any of the following characters: |, ', or \"", "Sea3D Connector");
		return;
	}

	//disable all except the frame and the status text
	enableStep1(FALSE);
	enableStep2(FALSE);

	//reneable frame and status text
	m_Step1Static.EnableWindow();
	m_NameStatusStatic.EnableWindow();

	//set the initial text
	m_NameStatusStatic.SetWindowText("Validating name...");
	
	//check it
	nResult = CONNECTOR->validateName(str);
	//see if it works
	if(TRUE == nResult)
	{
		//set the text
		m_NameStatusStatic.SetWindowText("Your name is available.");

		//move on
		enableStep1(FALSE);
		enableStep2(TRUE);
	}
	else if(-1 == nResult)
	{
		MessageBox("This player has been banned from ladder games.", "Sea3D Connector");
		EndDialog(IDCANCEL);
	}
	else
	{
		//set the text
		m_NameStatusStatic.SetWindowText("Your name is not available!");

		//they have to pick a new name
		enableStep1(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// enable/disable the sections
/////////////////////////////////////////////////////////////////////////////
void CConnectorRegisterDlg::enableStep1(BOOL bEnable)
{
	m_Step1Static.EnableWindow(bEnable);
	m_HandleStatic.EnableWindow(bEnable);
	m_FirstNameEdit.EnableWindow(bEnable);
	m_LastNameStatic.EnableWindow(bEnable);
	m_LastNameEdit.EnableWindow(bEnable);
	m_OneNameCheck.EnableWindow(bEnable);
	m_ValidateButton.EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// enable/disable the sections
/////////////////////////////////////////////////////////////////////////////
void CConnectorRegisterDlg::enableStep2(BOOL bEnable)
{
	m_Step2Static.EnableWindow(bEnable);
	m_PasswordStatic.EnableWindow(bEnable);
	m_PasswordEdit.EnableWindow(bEnable);
	m_ConfirmStatic.EnableWindow(bEnable);
	m_ConfirmEdit.EnableWindow(bEnable);
	m_RegisterButton.EnableWindow(bEnable);
	m_RegisterStatusStatic.EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// when they switch to using a handle, update the screen
/////////////////////////////////////////////////////////////////////////////
void CConnectorRegisterDlg::OnOnenameCheck() 
{
	BOOL bHandle;

	//get the value
	bHandle = m_OneNameCheck.GetCheck();

	//do stuff
	if(TRUE == bHandle)
	{
		m_LastNameStatic.ShowWindow(SW_HIDE);
		m_LastNameEdit.ShowWindow(SW_HIDE);
		m_HandleStatic.SetWindowText("Handle:");
	}
	else
	{
		m_LastNameStatic.ShowWindow(SW_SHOW);
		m_LastNameEdit.ShowWindow(SW_SHOW);
		m_HandleStatic.SetWindowText("First Name:");
		m_LastNameEdit.SetWindowText("");
	}
}

/////////////////////////////////////////////////////////////////////////////
// recheck a name
/////////////////////////////////////////////////////////////////////////////
void CConnectorRegisterDlg::OnValidateButton() 
{
	CString str;

	//set the handle value
	m_player.setHandle(m_OneNameCheck.GetCheck());

	//get first name
	m_FirstNameEdit.GetWindowText(str);

	//validate that they have a first name
	if(TRUE == str.IsEmpty())
	{
		MessageBox("The new player must at least have a first name or handle.", "New Player");
		return;
	}

	m_player.setFirstName(str);

	//last name
	m_LastNameEdit.GetWindowText(str);
	m_player.setLastName(str);

	//run it
	validateName();
}

/////////////////////////////////////////////////////////////////////////////
// register the player
/////////////////////////////////////////////////////////////////////////////
void CConnectorRegisterDlg::OnRegisterButton() 
{
	CString strName;
	CString strID;
	CString strPassword, strConfirm;

	//get the password strings
	m_PasswordEdit.GetWindowText(strPassword);
	m_ConfirmEdit.GetWindowText(strConfirm);

	//ensure they don't have a blank password
	if(TRUE == strPassword.IsEmpty())
	{
		MessageBox("Your password cannot be blank.", "Sea3D Connector");
		return;
	}

	//compare confirm
	if(strConfirm != strPassword)
	{
		MessageBox("Your password and confirmation do not match.", "Sea3D Connector");
		return;
	}

	//we're ready to rumble, so first disable everything
	enableStep2(FALSE);

	//get the player's name
	strName = m_player.getName();

	//renable the frame and the message text
	m_Step2Static.EnableWindow();
	m_RegisterStatusStatic.EnableWindow();

	//now revalidate
	m_RegisterStatusStatic.SetWindowText("Validating name...");

	//run it
	if(FALSE == CONNECTOR->validateName(strName))
	{
		//have to start over
		validateName();
		return;
	}

	//now register
	m_RegisterStatusStatic.SetWindowText("Registering player...");

	//run it
	if(FALSE == CONNECTOR->registerPlayer(strName, strPassword, strID))
	{
		//set text
		m_RegisterStatusStatic.SetWindowText("Registration failed!");

		return;
	}

	//tell them
	MessageBox("You have successfully registered this player at s3dconnector.net.", "Sea3D Connector");

	//we made it, so set the value
	m_player.setLadderID(strID);
	m_player.setLadder(TRUE);

	//we're done
	EndDialog(IDOK);	
}
