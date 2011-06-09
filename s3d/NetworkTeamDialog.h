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

#pragma once
#include "afxwin.h"


// CNetworkTeamDialog dialog

class CNetworkTeamDialog : public CDialog
{
	DECLARE_DYNAMIC(CNetworkTeamDialog)

public:
	CNetworkTeamDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNetworkTeamDialog();

// Dialog Data
	enum { IDD = IDD_NETWORK_TEAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	CComboBox m_TeamCombo;
	int m_nTeamCombo;
};
