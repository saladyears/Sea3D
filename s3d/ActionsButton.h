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
#include "GoodsButton.h"

/////////////////////////////////////////////////////////////////////////////
// CActionsButton
/////////////////////////////////////////////////////////////////////////////
class CActionsButton : public CAnimateButton
{
	DECLARE_DYNAMIC(CActionsButton)

public:
	CActionsButton();
	virtual ~CActionsButton();

	//button creation
	void createControls(void);

	//return the buttons
	CHoverStatic &Roll(void) {return m_btnRoll;}
	CGoodsButton &Goods(void) {return m_btnGoods;}
	CGoodsButton &Volcano(void) {return m_btnVolcano;}

	//overrides
	void updateAnimate(void);

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	//overrides
	void moveOffset(int nOffset);
	void startShow(BOOL bShow);

public:
	afx_msg void OnRollButton();
 	afx_msg void OnGoodsButton();
 	afx_msg void OnVolcanoButton();

protected:
	//the roll button for multiplayer
	CHoverStatic m_btnRoll;

	//the goods button
	CGoodsButton m_btnGoods;

	//the volcano button
	CGoodsButton m_btnVolcano;
public:
	afx_msg void OnPaint();
};


