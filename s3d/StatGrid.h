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

#if !defined(AFX_STATGRID_H__84489A00_25C4_4297_956C_E42C89F7483B__INCLUDED_)
#define AFX_STATGRID_H__84489A00_25C4_4297_956C_E42C89F7483B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "grid\GridCtrl.h"
#include "XLogFont.h"

/////////////////////////////////////////////////////////////////////////////
// CStatGrid window

class CStatGrid : public CGridCtrl
{
// Construction
public:
	CStatGrid();

// Attributes
public:

// Operations
public:
	virtual void Initialize(void);

	//init things
	void init(void);

	//update a stat block
	void update(int iBlock);

	//jump to a certain block
	void jumpTo(int iBlock);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStatGrid();

	// Generated message map functions
protected:
	//creation functions
	void createBlock(int iBlock, int &iRow);
	void createHeader(int iType, int &iRow);
	void createGoodsHeader(int iRow, int iCol);
	void createTitle(int iType, int &iRow);
	void createRollFreq(int &iRow);
	void createRollPlayer(int &iRow);
	void createGoodsTable(int &iRow, int iType);
	void createTradeNo(int &iRow);
	void createTradePartners(int &iRow);
	void createTradeRatios(int &iRow);
	void createTimeTurn(int &iRow);

	//update functions
	void updateRollFreq(void);
	void updateTradeNo(void);
	void updateTradePartners(void);
	void updateTradeRatios(void);
	void updateTime(void);

	//meta-update functions
	void updateSingleBlock(int iType, int iCol);
	void updateDoubleBlock(int iType);

	//sums part of a 2d array
	int sumArray(int array[6][5], int j);

	//creates a time string based on number of seconds
	CString createTimeString(int iSeconds);

	//draw an outline
	void outline(int iT, int iL, int iB, int iR);

	//color fill
	void fill(int iT, int iL, int iB, int iR, COLORREF color);

	//jump to locations
	vector <int> m_iJump;

	//the number of players
	int m_iPlayers;

	//whether we've initialized
	BOOL m_bInit;

	//the main font
	XLOGFONT m_font;

	//the large font
	XLOGFONT m_titlefont;

	//the header font
	XLOGFONT m_headerfont;

	//{{AFX_MSG(CStatGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATGRID_H__84489A00_25C4_4297_956C_E42C89F7483B__INCLUDED_)
