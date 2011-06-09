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
#include "AnimateButton.h"
#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CControlsButton
/////////////////////////////////////////////////////////////////////////////
class CControlsButton : public CAnimateButton
{
	DECLARE_DYNAMIC(CControlsButton)

public:
	CControlsButton();
	virtual ~CControlsButton();

	//button creation
	void createControls(void);

	//return the buttons
	CButtonST &Play(void) {return m_btnPlay;}
	CButtonST &Bank(void) {return m_btnBank;}
	CButtonST &Trade(void) {return m_btnTrade;}
	CButtonST &End(void) {return m_btnEnd;}

	//overrides
	void updateAnimate(void);

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg void OnEndButton();
 	afx_msg void OnBankButton();
 	afx_msg void OnPlayButton();
 	afx_msg void OnTradeButton();

	//overrides
	void moveOffset(int nOffset);
	void startShow(BOOL bShow);

protected:
	//action buttons
	CButtonST m_btnPlay;
	CButtonST m_btnBank;
	CButtonST m_btnTrade;
	CButtonST m_btnEnd;
public:
	afx_msg void OnPaint();
};


