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

#if !defined(AFX_VARIANTSDIALOG_H__A94DCAD5_31F5_43F8_9B30_06839403A9EA__INCLUDED_)
#define AFX_VARIANTSDIALOG_H__A94DCAD5_31F5_43F8_9B30_06839403A9EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VariantsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HoverStatic.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CVariantsDialog dialog
/////////////////////////////////////////////////////////////////////////////
class CVariantsDialog : public CHelpDialog
{
// Construction
public:
	CVariantsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVariantsDialog)
	enum { IDD = IDD_VARIANTS_DLG };
	CButton	m_SpecialCheck;
	CStatic	m_ParenStatic;
	CButton	m_DesertRadio;
	CButton	m_VolcanoRadio;
	CButton	m_JungleRadio;
	CButton	m_GoldCheck;
	CStatic	m_ShipsStatic;
	CEdit	m_ShipsEdit;
	CButton	m_HideChipsCheck;
	CButton	m_RoundUpCheck;
	CButton	m_ShowCardsCheck;
	CButton	m_TradeAfterCheck;
	CButton	m_SevensCheck;
	CEdit	m_PointsEdit;
	CEdit	m_AllowedEdit;
	CEdit	m_CitiesEdit;
	CEdit	m_SettleEdit;
	CEdit	m_RoadsEdit;
	CButton	m_EqualOddsCheck;
	CButton	m_AllowZeroCheck;
	CButton m_TournamentCheck;
	CHoverStatic	m_RollStatic;
	CHoverStatic	m_AllowedStatic;
	CButton m_NoTradeCheck;
	CEdit	m_NoSevensEdit;
	CButton	m_TradeLimitCheck;
	CEdit	m_TradeLimitEdit;
	CStatic	m_TradeLimitStatic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVariantsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//disable things in ladder games
	void disableLadder(void);

	// Generated message map functions
	//{{AFX_MSG(CVariantsDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnJungleRadio();
	afx_msg void OnDesertRadio();
	afx_msg void OnVolcanoRadio();
	afx_msg void OnSpecialCheck();
	afx_msg void OnNoTradeCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VARIANTSDIALOG_H__A94DCAD5_31F5_43F8_9B30_06839403A9EA__INCLUDED_)
