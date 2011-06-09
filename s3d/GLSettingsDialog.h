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

#if !defined(AFX_GLSETTINGSDIALOG_H__0E66B958_6675_4898_AFAD_1C7020E77592__INCLUDED_)
#define AFX_GLSETTINGSDIALOG_H__0E66B958_6675_4898_AFAD_1C7020E77592__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GLSettingsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HoverStatic.h"
#include "HelpDialog.h"
#include "ColorButton.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CGLSettingsDialog dialog

class CGLSettingsDialog : public CHelpDialog
{
// Construction
public:
	CGLSettingsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGLSettingsDialog)
	enum { IDD = IDD_GL_SETTINGS_DLG };
	CButton			m_SeaTilesCheck;
	CStatic			m_WhiteStatic;
	CColorButton	m_WhiteButton;
	CButton			m_WhiteCheck;
	CStatic			m_ColorStatic;
	CColorButton	m_ColorButton;
	CColorButton	m_OutlineButton;
	CButton			m_HighlightCheck;
	CButton			m_GouraudCheck;
	CButton			m_WireframeCheck;
	CHoverStatic	m_DetailStatic;
	CSliderCtrl		m_DetailSlider;
	CHoverStatic	m_MipmapStatic;
	CButton	m_MipmapCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLSettingsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	//sett text for detail slider
	void setDetailText(void);

	// Generated message map functions
	//{{AFX_MSG(CGLSettingsDialog)
	virtual void OnCancel();
	afx_msg void OnMipmapCheck();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnWireframeCheck();
	afx_msg void OnGouraudCheck();
	afx_msg void OnHighlightCheck();
	afx_msg void OnWhiteCheck();
	afx_msg void OnSeatilesCheck();
	//}}AFX_MSG
	afx_msg LONG OnSelChange(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSmallerCheck();
	CButton m_SmallerCheck;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLSETTINGSDIALOG_H__0E66B958_6675_4898_AFAD_1C7020E77592__INCLUDED_)
