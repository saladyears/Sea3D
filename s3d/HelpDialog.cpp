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
#include "HelpDialog.h"
#include "defineFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpDialog dialog


CHelpDialog::CHelpDialog(int ID /*= 0*/, CWnd* pParent /*=NULL*/)
	: CDialog(ID, pParent)
{
	//{{AFX_DATA_INIT(CHelpDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHelpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpDialog, CDialog)
	//{{AFX_MSG_MAP(CHelpDialog)
	ON_COMMAND(ID_HELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpDialog message handlers

BOOL CHelpDialog::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == 0x4d) 
	{
		//call help
		OnHelp();

		//eat the message
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// run the help system
/////////////////////////////////////////////////////////////////////////////
void CHelpDialog::OnHelp() 
{
	if(TRUE == APP->hasHelp())
	{
    #if _MSC_VER >= 1300
		::HtmlHelp(GetDesktopWindow()->GetSafeHwnd(), CString(FILE_HELP) + ">" + m_strHelp, HH_DISPLAY_TOPIC, NULL);
    #else
		HtmlHelp(GetDesktopWindow()->GetSafeHwnd(), CString(FILE_HELP) + ">" + m_strHelp, HH_DISPLAY_TOPIC, NULL);
    #endif // _MSC_VER > 1300
	}
	else
	{
		::ShellExecute(NULL, NULL, URL_HELP + m_strHelp, NULL, NULL, SW_SHOWNORMAL);
	}
}
