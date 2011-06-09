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
#include "HoverStatic.h"
#include "BtnST.h"
#include "AnimateButton.h"

/////////////////////////////////////////////////////////////////////////////
// CMessageButton
/////////////////////////////////////////////////////////////////////////////
class CMessageButton : public CAnimateButton
{
	DECLARE_DYNAMIC(CMessageButton)

public:
	CMessageButton();
	virtual ~CMessageButton();

	//undo handling
	void enableUndo(BOOL b);
	BOOL undoEnabled(void) {return m_UndoButton.IsWindowEnabled();}

	//return the text box
	CHoverStatic &Title(void) {return m_Title;}

protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUndoButton();

protected:
	//the title text
	CHoverStatic m_Title;

	//the undo button
	CButtonST m_UndoButton;
public:
	afx_msg void OnPaint();
};


