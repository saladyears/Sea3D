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
#include "GLSettingsDialog.h"
#include "SettlersView.h"
#include "defineIni.h"
#include "defineColors.h"
#include "defineMap.h"
#include ".\glsettingsdialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLSettingsDialog dialog


CGLSettingsDialog::CGLSettingsDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CGLSettingsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGLSettingsDialog)
	//}}AFX_DATA_INIT
}


void CGLSettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGLSettingsDialog)
	DDX_Control(pDX, IDC_SEATILES_CHECK, m_SeaTilesCheck);
	DDX_Control(pDX, IDC_WHITE_STATIC, m_WhiteStatic);
	DDX_Control(pDX, IDC_WHITE_BUTTON, m_WhiteButton);
	DDX_Control(pDX, IDC_WHITE_CHECK, m_WhiteCheck);
	DDX_Control(pDX, IDC_COLOR_STATIC, m_ColorStatic);
	DDX_Control(pDX, IDC_COLOR_BUTTON, m_ColorButton);
	DDX_Control(pDX, IDC_OUTLINE_BUTTON, m_OutlineButton);
	DDX_Control(pDX, IDC_HIGHLIGHT_CHECK, m_HighlightCheck);
	DDX_Control(pDX, IDC_GOURAUD_CHECK, m_GouraudCheck);
	DDX_Control(pDX, IDC_WIREFRAME_CHECK, m_WireframeCheck);
	DDX_Control(pDX, IDC_DETAIL_INFO_STATIC, m_DetailStatic);
	DDX_Control(pDX, IDC_DETAIL_SLIDER, m_DetailSlider);
	DDX_Control(pDX, IDC_MIPMAP_INFO_STATIC, m_MipmapStatic);
	DDX_Control(pDX, IDC_MIPMAP_CHECK, m_MipmapCheck);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SMALLER_CHECK, m_SmallerCheck);
}


BEGIN_MESSAGE_MAP(CGLSettingsDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CGLSettingsDialog)
	ON_BN_CLICKED(IDC_MIPMAP_CHECK, OnMipmapCheck)
	ON_WM_SHOWWINDOW()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_WIREFRAME_CHECK, OnWireframeCheck)
	ON_BN_CLICKED(IDC_GOURAUD_CHECK, OnGouraudCheck)
	ON_BN_CLICKED(IDC_HIGHLIGHT_CHECK, OnHighlightCheck)
	ON_BN_CLICKED(IDC_WHITE_CHECK, OnWhiteCheck)
	ON_MESSAGE(CPN_SELCHANGE, OnSelChange)
	ON_BN_CLICKED(IDC_SEATILES_CHECK, OnSeatilesCheck)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SMALLER_CHECK, OnBnClickedSmallerCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLSettingsDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// init info boxes
/////////////////////////////////////////////////////////////////////////////
BOOL CGLSettingsDialog::OnInitDialog() 
{
	CString str;

	CHelpDialog::OnInitDialog();

	//set the help
	m_strHelp = HELP_GLSET;
	
	//set mipmap info
	m_MipmapStatic.SetFont(Tahoma12);
	m_MipmapStatic.SetColors(COLOR_GRAY, COLOR_GRAY);
	m_MipmapStatic.SetText("Mipmapping draws smoother textures at the cost of memory.");

	//set detail info
	m_DetailStatic.SetFont(Tahoma12);
	m_DetailStatic.SetColors(COLOR_GRAY, COLOR_GRAY);
	setDetailText();

	//set graphics detail
	m_DetailSlider.SetRange(0, GL_DETAIL_LEVELS - 1, TRUE);

	//set up color button
	m_ColorButton.SetDefaultColor(COLOR_OUTLINEGREEN);
	m_ColorButton.SetDefaultText("Use Default Color");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// check handler
/////////////////////////////////////////////////////////////////////////////
void CGLSettingsDialog::OnMipmapCheck() 
{
	CString str;
	CWaitCursor cw;
	
	//write it out to the .ini file
	str.Format("%d", m_MipmapCheck.GetCheck());
	WritePrivateProfileString(INI_OPENGL, INI_MIPMAP, str, INI_FILE);

	VIEW->setMipMap();
}

/////////////////////////////////////////////////////////////////////////////
// check handler
/////////////////////////////////////////////////////////////////////////////
void CGLSettingsDialog::OnWireframeCheck() 
{
	BOOL bCheck;
	CString str;
	CWaitCursor cw;

	//get value
	bCheck = m_WireframeCheck.GetCheck();
	
	//write it out to the .ini file
	str.Format("%d", bCheck);
	WritePrivateProfileString(INI_OPENGL, INI_WIREFRAME, str, INI_FILE);

	//refresh
	VIEW->Invalidate();

	//enable/diable the gouraud check
	m_GouraudCheck.EnableWindow(!bCheck);
}

/////////////////////////////////////////////////////////////////////////////
// check handler
/////////////////////////////////////////////////////////////////////////////
void CGLSettingsDialog::OnGouraudCheck() 
{
	BOOL bCheck;
	CString str;
	CWaitCursor cw;

	//get value
	bCheck = m_GouraudCheck.GetCheck();
	
	//write it out to the .ini file
	str.Format("%d", bCheck);
	WritePrivateProfileString(INI_OPENGL, INI_GOURAUD, str, INI_FILE);

	//refresh
	VIEW->setGouraud();
}

/////////////////////////////////////////////////////////////////////////////
// updates
/////////////////////////////////////////////////////////////////////////////
void CGLSettingsDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	BOOL bCheck;
	COLORREF color;

	CHelpDialog::OnShowWindow(bShow, nStatus);
	
	//if we're hiding, return
	if(FALSE == bShow) 
	{
		return;
	}

	//set smaller textures flag
	bCheck = GetPrivateProfileInt(INI_OPENGL, INI_SMALL_TILES, FALSE, INI_FILE);
	m_SmallerCheck.SetCheck(bCheck);

	//set the mipmap check
	m_MipmapCheck.SetCheck(VIEW->getMipMap());

	//set detail
	m_DetailSlider.SetPos(VIEW->getDetail());

	//enable/disable the wireframe check
	m_WireframeCheck.EnableWindow(VIEW->getDetail() >= GL_MED_DETAIL);

	//set wireframe value
	bCheck = GetPrivateProfileInt(INI_OPENGL, INI_WIREFRAME, FALSE, INI_FILE);
	m_WireframeCheck.SetCheck(bCheck);

	//enable/diable the gouraud check
	m_GouraudCheck.EnableWindow(!bCheck);

	//set gouraud value
	bCheck = GetPrivateProfileInt(INI_OPENGL, INI_GOURAUD, FALSE, INI_FILE);
	m_GouraudCheck.SetCheck(bCheck);

	//set highlight value
	bCheck = GetPrivateProfileInt(INI_OPENGL, INI_HIGHLIGHT, TRUE, INI_FILE);
	m_HighlightCheck.SetCheck(bCheck);

	//set highlight color
	color = GetPrivateProfileInt(INI_OPENGL, INI_HIGHLIGHT_COLOR, COLOR_OUTLINEGREEN, INI_FILE);
	m_ColorButton.SetColor(color);

	//enable/disable color buttons
	m_ColorStatic.EnableWindow(bCheck);
	m_ColorButton.EnableWindow(bCheck);

	//set white outline value
	bCheck = GetPrivateProfileInt(INI_OPENGL, INI_WHITE, TRUE, INI_FILE);
	m_WhiteCheck.SetCheck(TRUE);

	//set outline color
	color = GetPrivateProfileInt(INI_OPENGL, INI_WHITE_COLOR, COLOR_OUTLINEPURPLE, INI_FILE);
	m_WhiteButton.SetColor(color);

	//enable/disable outline buttons
	m_WhiteStatic.EnableWindow(bCheck);
	m_WhiteButton.EnableWindow(bCheck);

	//set longest road color
	color = GetPrivateProfileInt(INI_OPENGL, INI_LONGEST_COLOR, COLOR_GOLDOUTLINE, INI_FILE);
	m_OutlineButton.SetColor(color);

	//original ports
	bCheck = GetPrivateProfileInt(INI_OPENGL, INI_SEATILES, TRUE, INI_FILE);
	m_SeaTilesCheck.SetCheck(bCheck);

	//set text
	setDetailText();
}

/////////////////////////////////////////////////////////////////////////////
// update main menu
/////////////////////////////////////////////////////////////////////////////
void CGLSettingsDialog::OnCancel() 
{
	CMenu *pMenu;
	CMenu *pSubmenu;

	//get the current menu
	pMenu = AfxGetMainWnd()->GetMenu();

	//get the submenu
	pSubmenu = pMenu->GetSubMenu(2);

	//set the checked status
	pSubmenu->CheckMenuItem(1, MF_UNCHECKED | MF_BYPOSITION);

	//normal handling
	CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// handle the slider bar
/////////////////////////////////////////////////////////////////////////////
void CGLSettingsDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int iPos;
	CString str;

	//get current pos
	iPos = ((CSliderCtrl *)pScrollBar)->GetPos();

	//write it out to the ini file
	str.Format("%d", iPos);
	WritePrivateProfileString(INI_OPENGL, INI_DETAIL, str, INI_FILE);

	//redraw
	VIEW->setDetail();

	//enable/disable the wireframe check
	m_WireframeCheck.EnableWindow(VIEW->getDetail() >= GL_MED_DETAIL);

	//set text
	setDetailText();
	
	CHelpDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/////////////////////////////////////////////////////////////////////////////
// set the detail slider text
/////////////////////////////////////////////////////////////////////////////
void CGLSettingsDialog::setDetailText()
{
	CString str;

	switch(VIEW->getDetail())
	{
	case GL_NO_DETAIL: str = "no depth to roll chits, no dynamic lighting, no 3D robber, no textures on build items"; break;
	case GL_LOW_DETAIL: str = "roll chits have depth, no dynamic lighting, no 3D robber, no textures on build items"; break;
	case GL_LIGHT_DETAIL: str = "roll chits have depth, dynamic lighting, no 3D robber, no textures on build items"; break;
	case GL_MED_DETAIL: str = "roll chits have depth, dynamic lighting, robber is 3D, no textures on build items"; break;
	case GL_HIGH_DETAIL: str = "roll chits have depth, dynamic lighting, robber is 3D, build items have textures"; break;
	default: break;
	}

	m_DetailStatic.SetText(str);
}

/////////////////////////////////////////////////////////////////////////////
// highlighting objects
/////////////////////////////////////////////////////////////////////////////
void CGLSettingsDialog::OnHighlightCheck() 
{
	BOOL bCheck;
	CString str;
	CWaitCursor cw;

	//get value
	bCheck = m_HighlightCheck.GetCheck();

	//enable/disable color buttons
	m_ColorStatic.EnableWindow(bCheck);
	m_ColorButton.EnableWindow(bCheck);
	
	//write it out to the .ini file
	str.Format("%d", bCheck);
	WritePrivateProfileString(INI_OPENGL, INI_HIGHLIGHT, str, INI_FILE);

	//refresh
	VIEW->setHighlight();
}

/////////////////////////////////////////////////////////////////////////////
// white player outline objects
/////////////////////////////////////////////////////////////////////////////
void CGLSettingsDialog::OnWhiteCheck() 
{
	BOOL bCheck;
	CString str;
	CWaitCursor cw;

	//get value
	bCheck = m_WhiteCheck.GetCheck();

	//enable/disable color buttons
	m_WhiteStatic.EnableWindow(bCheck);
	m_WhiteButton.EnableWindow(bCheck);
	
	//write it out to the .ini file
	str.Format("%d", bCheck);
	WritePrivateProfileString(INI_OPENGL, INI_WHITE, str, INI_FILE);

	//refresh
	VIEW->setWhiteOutline();
}

/////////////////////////////////////////////////////////////////////////////
// sea tiles
/////////////////////////////////////////////////////////////////////////////
void CGLSettingsDialog::OnSeatilesCheck() 
{
	BOOL bCheck;

	//get value
	bCheck = m_SeaTilesCheck.GetCheck();

	//write it to the .ini file
	WritePrivateProfileString(INI_OPENGL, INI_SEATILES, bCheck ? "1" : "0", INI_FILE);

	//set GL
	VIEW->setSeaTiles();
}

/////////////////////////////////////////////////////////////////////////////
// highlight color
/////////////////////////////////////////////////////////////////////////////
LONG CGLSettingsDialog::OnSelChange(UINT lParam, LONG wParam)
{
	CString str;

	//set the color
	if(IDC_COLOR_BUTTON == wParam)
	{
		str.Format("%d", lParam);
		WritePrivateProfileString(INI_OPENGL, INI_HIGHLIGHT_COLOR, str, INI_FILE);

		VIEW->setHighlightColor();
	}
	else if(IDC_OUTLINE_BUTTON == wParam)
	{
		str.Format("%d", lParam);
		WritePrivateProfileString(INI_OPENGL, INI_LONGEST_COLOR, str, INI_FILE);
		
		VIEW->setLongestOutlineColor();
	}
	else
	{
		str.Format("%d", lParam);
		WritePrivateProfileString(INI_OPENGL, INI_WHITE_COLOR, str, INI_FILE);

		VIEW->setWhiteOutlineColor();
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// small textures
/////////////////////////////////////////////////////////////////////////////
void CGLSettingsDialog::OnBnClickedSmallerCheck()
{
	BOOL bCheck;
	CString str;
	CWaitCursor cw;

	//get value
	bCheck = m_SmallerCheck.GetCheck();
	
	//write it out to the .ini file
	str.Format("%d", bCheck);
	WritePrivateProfileString(INI_OPENGL, INI_SMALL_TILES, str, INI_FILE);

	//refresh
	VIEW->rebuildTextures();
	VIEW->rebuildOtherTextures();
}
