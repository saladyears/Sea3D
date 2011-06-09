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
#include "SplashDlg.h"
#include "XLogFont.h"
#include <il/ilut.h>
#include "defineFile.h"
#include "defineIni.h"
#include "defineDisplay.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplashDlg dialog


CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CSplashDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSplashDlg)
	//}}AFX_DATA_INIT

	m_strHelp = HELP_SPLASH;
}


void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplashDlg)
	DDX_Control(pDX, IDC_SPLASH_BUTTON, m_SplashButton);
	DDX_Control(pDX, IDC_NETWORK_BUTTON, m_NetworkButton);
	DDX_Control(pDX, IDC_HOTSEAT_BUTTON, m_HotseatButton);
	DDX_Control(pDX, IDC_DISCLAIMER_BUTTON, m_DisclaimerButton);
	DDX_Control(pDX, IDC_SKIP_BUTTON, m_SkipButton);
	DDX_Control(pDX, IDC_TOUR_BUTTON, m_TourButton);
	DDX_Control(pDX, IDC_OPEN_BUTTON, m_OpenButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSplashDlg, CHelpDialog)
	//{{AFX_MSG_MAP(CSplashDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OPEN_BUTTON, OnOpenButton)
	ON_BN_CLICKED(IDC_TOUR_BUTTON, OnTourButton)
	ON_BN_CLICKED(IDC_SKIP_BUTTON, OnSkipButton)
	ON_BN_CLICKED(IDC_DISCLAIMER_BUTTON, OnDisclaimerButton)
	ON_BN_CLICKED(IDC_HOTSEAT_BUTTON, OnHotseatButton)
	ON_BN_CLICKED(IDC_NETWORK_BUTTON, OnNetworkButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplashDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// handle startup
/////////////////////////////////////////////////////////////////////////////
BOOL CSplashDlg::OnInitDialog() 
{
	BOOL bTour;
	XLOGFONT xfont;
	ILuint ilToolbar[TOOLBAR_SIZE];
	CDC *pDC;

	CHelpDialog::OnInitDialog();

	//get current dc
	pDC = GetDC();

	//read the splash .jpg
	ilutRenderer(ILUT_WIN32);

	//read in the toolbar stuff
	APP->readIL(TOOLBAR_SIZE, FILE_APP, &ilToolbar[0]);

	//bind the image
	ilBindImage(ilToolbar[SPLASH_SCREEN]);

	//set the bitmap
	m_SplashButton.SetBitmap(ilutConvertToHBitmap(pDC->GetSafeHdc()));

	//clean up il
	ilDeleteImages(TOOLBAR_SIZE, ilToolbar);

	//clean up
	ReleaseDC(pDC);

	//create the font
	xfont = GetLogFont(13, "Tahoma");
	m_font.CreateFontIndirect((LOGFONT *) &xfont);

	//set up the selections
	m_HotseatButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_WHITE);
	m_HotseatButton.SetColor(CButtonST::BTNST_COLOR_FG_OUT, COLOR_WHITE);
	m_HotseatButton.SetColor(CButtonST::BTNST_COLOR_BK_IN, COLOR_BLACK);
	m_HotseatButton.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COLOR_BLACK);
	m_HotseatButton.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COLOR_BLACK);
	m_HotseatButton.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, COLOR_WHITE);
	m_HotseatButton.SetFont(&m_font);
	m_HotseatButton.SetWindowText("Play hotseat");

	m_NetworkButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_WHITE);
	m_NetworkButton.SetColor(CButtonST::BTNST_COLOR_FG_OUT, COLOR_WHITE);
	m_NetworkButton.SetColor(CButtonST::BTNST_COLOR_BK_IN, COLOR_BLACK);
	m_NetworkButton.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COLOR_BLACK);
	m_NetworkButton.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COLOR_BLACK);
	m_NetworkButton.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, COLOR_WHITE);
	m_NetworkButton.SetFont(&m_font);
	m_NetworkButton.SetWindowText("Play network");

	m_OpenButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_WHITE);
	m_OpenButton.SetColor(CButtonST::BTNST_COLOR_FG_OUT, COLOR_WHITE);
	m_OpenButton.SetColor(CButtonST::BTNST_COLOR_BK_IN, COLOR_BLACK);
	m_OpenButton.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COLOR_BLACK);
	m_OpenButton.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COLOR_BLACK);
	m_OpenButton.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, COLOR_WHITE);
	m_OpenButton.SetFont(&m_font);
	m_OpenButton.SetWindowText("Open a saved game");

	//see if they have a tour prompt
	bTour = GetPrivateProfileInt(INI_GENERAL, INI_TOUR, FALSE, INI_FILE);

	m_TourButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, bTour ? COLOR_RED : COLOR_WHITE);
	m_TourButton.SetColor(CButtonST::BTNST_COLOR_FG_OUT, bTour ? COLOR_RED : COLOR_WHITE);
	m_TourButton.SetColor(CButtonST::BTNST_COLOR_BK_IN, COLOR_BLACK);
	m_TourButton.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COLOR_BLACK);
	m_TourButton.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COLOR_BLACK);
	m_TourButton.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, COLOR_WHITE);
	m_TourButton.SetFont(&m_font);
	m_TourButton.SetWindowText("Take a tour");

	m_SkipButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_WHITE);
	m_SkipButton.SetColor(CButtonST::BTNST_COLOR_FG_OUT, COLOR_WHITE);
	m_SkipButton.SetColor(CButtonST::BTNST_COLOR_BK_IN, COLOR_BLACK);
	m_SkipButton.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COLOR_BLACK);
	m_SkipButton.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COLOR_BLACK);
	m_SkipButton.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, COLOR_WHITE);
	m_SkipButton.SetFont(&m_font);
	m_SkipButton.SetWindowText("Jump to main window");

	m_DisclaimerButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_WHITE);
	m_DisclaimerButton.SetColor(CButtonST::BTNST_COLOR_FG_OUT, COLOR_WHITE);
	m_DisclaimerButton.SetColor(CButtonST::BTNST_COLOR_BK_IN, COLOR_BLACK);
	m_DisclaimerButton.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COLOR_BLACK);
	m_DisclaimerButton.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COLOR_BLACK);
	m_DisclaimerButton.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, COLOR_WHITE);
	m_DisclaimerButton.SetFont(&m_font);
	m_DisclaimerButton.SetWindowText("Read disclaimer");

	//center the window
	CenterWindow();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// handle painting
/////////////////////////////////////////////////////////////////////////////
void CSplashDlg::OnPaint() 
{
	CRect rect;

	CPaintDC dc(this); // device context for painting
	
	//get client rect
	GetClientRect(&rect);

	//paint
	dc.FillSolidRect(&rect, COLOR_BLACK);
}

/////////////////////////////////////////////////////////////////////////////
// handle shutdown
/////////////////////////////////////////////////////////////////////////////
void CSplashDlg::OnDestroy() 
{
	HBITMAP hBitmap;

	CHelpDialog::OnDestroy();
	
	m_font.DeleteObject();

	//clean up splash
	hBitmap = m_SplashButton.GetBitmap();

	if(NULL != hBitmap)
	{
		DeleteObject(hBitmap);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle button click
/////////////////////////////////////////////////////////////////////////////
void CSplashDlg::OnOpenButton() 
{
	APP->setSplash(SPLASH_OPEN);

	OnOK();	
}

/////////////////////////////////////////////////////////////////////////////
// handle button click
/////////////////////////////////////////////////////////////////////////////
void CSplashDlg::OnTourButton() 
{
	APP->setSplash(SPLASH_TOUR);

	OnOK();	
}

/////////////////////////////////////////////////////////////////////////////
// handle button click
/////////////////////////////////////////////////////////////////////////////
void CSplashDlg::OnSkipButton() 
{
	APP->setSplash(SPLASH_SKIP);

	OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// handle button click
/////////////////////////////////////////////////////////////////////////////
void CSplashDlg::OnDisclaimerButton() 
{
	CFile file;

	//first see if the notepad file is there
	if(FALSE == file.Open("disclaimer.txt", CFile::modeRead))
	{
		::ShellExecute(NULL, NULL, URL_HELP, NULL, NULL, SW_SHOWNORMAL);
	}
	else
	{
		file.Close();
		::ShellExecute(NULL, NULL, "disclaimer.txt", NULL, NULL, SW_SHOWNORMAL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle button click
/////////////////////////////////////////////////////////////////////////////
void CSplashDlg::OnHotseatButton() 
{
	APP->setSplash(SPLASH_HOTSEAT);

	OnOK();	
}

/////////////////////////////////////////////////////////////////////////////
// handle button click
/////////////////////////////////////////////////////////////////////////////
void CSplashDlg::OnNetworkButton() 
{
	APP->setSplash(SPLASH_NETWORK);

	OnOK();	
}
