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

#if !defined(AFX_VICTORYPOINTDIALOG_H__8775EECC_903C_4F0D_B496_7B071A139CD9__INCLUDED_)
#define AFX_VICTORYPOINTDIALOG_H__8775EECC_903C_4F0D_B496_7B071A139CD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VictoryPointDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CVictoryPointDialog dialog

class CVictoryPointDialog : public CHelpDialog
{
// Construction
public:
	CVictoryPointDialog(CWnd* pParent = NULL);   // standard constructor

	//set methods
	inline void setName(CString str) {m_strName = str;}
	inline void setCards(int i) {m_iCards = i;}

	//get methods
	inline int getPoints(void) {return m_iPoints;}

// Dialog Data
	//{{AFX_DATA(CVictoryPointDialog)
	enum { IDD = IDD_VICTORY_DLG };
	CButton	m_VictoryStatic;
	CButton	m_TwoRadio;
	CButton	m_ThreeRadio;
	CButton	m_OneRadio;
	CButton	m_NoneRadio;
	CButton	m_FourRadio;
	CButton	m_FiveRadio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVictoryPointDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//available cards to turn in
	int m_iCards;

	//selected radio button
	int m_iPoints;

	//player's name
	CString m_strName;

	// Generated message map functions
	//{{AFX_MSG(CVictoryPointDialog)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VICTORYPOINTDIALOG_H__8775EECC_903C_4F0D_B496_7B071A139CD9__INCLUDED_)
