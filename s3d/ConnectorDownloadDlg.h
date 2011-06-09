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

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "Player.h"
#include "HelpDialog.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CConnectorDownloadDlg dialog
/////////////////////////////////////////////////////////////////////////////
class CConnectorDownloadDlg : public CHelpDialog
{
	DECLARE_DYNAMIC(CConnectorDownloadDlg)

public:
	CConnectorDownloadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConnectorDownloadDlg();

// Dialog Data
	enum { IDD = IDD_CONNECTOR_DOWNLOAD_DLG };

	//set the player we're working with
	void setPlayer(CPlayer player) {m_player = player;}

	//return the player we've worked with
	CPlayer getPlayer(void) {return m_player;}


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	//the current player
	CPlayer m_player;
public:
	CEdit m_HandleEdit;
	afx_msg void OnBnClickedDownloadButton();
};
