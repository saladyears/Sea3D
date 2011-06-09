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
#include "Settlers.h"
#include "AboutDlg.h"
#include "defineAI.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_ESC_STATIC, m_EscStatic);
	DDX_Control(pDX, IDC_VER_STATIC, m_VerStatic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CHelpDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CAboutDlg::OnInitDialog() 
{
	CRect rect;
	CString str, strTemp;
	CWaitCursor wait;

	CHelpDialog::OnInitDialog();

	//set the help
	m_strHelp = HELP_ABOUT;
	
	//get client rect
	GetClientRect(&rect);

	//create GL
	m_wnd.Create(NULL, NULL, WS_CHILD | WS_TABSTOP | WS_VISIBLE, rect, this, 0);

	//set the font
	m_VerStatic.SetFont(Tahoma12);
	m_VerStatic.SetColors(COLOR_WHITE, COLOR_WHITE);
	m_VerStatic.SetRight();
	m_VerStatic.SetBackground(COLOR_BLACK);

	//set the version number
	str.Format("Sea3D version %s", GAME_VER);

	//add the AI string
	if(TRUE == APP->hasAI())
	{
		strTemp.Format(", AI version %s", AI_VER);
		str += strTemp;
	}

	//set text
	m_VerStatic.SetText(str);

	//set the font
	m_EscStatic.SetFont(Tahoma12);
	m_EscStatic.SetColors(COLOR_WHITE, COLOR_WHITE);
	m_EscStatic.SetBackground(COLOR_BLACK);

	//set escape text
	m_EscStatic.SetText("ESC to close");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// sizing
/////////////////////////////////////////////////////////////////////////////
void CAboutDlg::OnSize(UINT nType, int cx, int cy) 
{
	CRect rect;

	CHelpDialog::OnSize(nType, cx, cy);
	
	if(NULL != m_wnd.m_hWnd)
	{
		m_wnd.SetWindowPos(&wndTop, 0, 0, cx, cy, SWP_NOZORDER);
		m_wnd.resizeGL(cx, cy);
	}

	//set other statics
	if(NULL != m_VerStatic.m_hWnd)
	{
		//get ver rect
		m_VerStatic.GetClientRect(&rect);
		m_VerStatic.SetWindowPos(&wndTop, cx - rect.Width(), cy - rect.Height(), 0, 0, SWP_NOSIZE);
	}
	
	if(NULL != m_EscStatic.m_hWnd)
	{
		//get ver rect
		m_EscStatic.GetClientRect(&rect);
		m_EscStatic.SetWindowPos(&wndTop, 0, cy - rect.Height(), 0, 0, SWP_NOSIZE);
	}
}
