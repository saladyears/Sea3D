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

//////////////////////////////////////////////////////////////////////
// class to hold information when someone drops
//////////////////////////////////////////////////////////////////////
class CSaveState
{
public:
	CSaveState(void);
	~CSaveState(void);

	//clear all
	void resetState(void);

	//save and restore the state
	void saveState(BOOL bNoChanges = FALSE);
	void restoreState(void);

	//resynch data
	void saveResynch(int nResynch, int nData1, int nData2);

	//return the saved state
	BOOL isSaved(void) {return m_bSaved;}

	//hide things
	void hideAll(void);

	//overloaded assignment operator
	CSaveState &operator =(const CSaveState &save);

protected:
	//data members

	//controls buttons
	BOOL m_bControls;
	BOOL m_bPlay;
	BOOL m_bBank;
	BOOL m_bTrade;
	BOOL m_bEnd;

	//message control
	CString m_strMessage;
	BOOL m_bUndo;

	//actions control
	BOOL m_bActions;
	BOOL m_bRoll;
	BOOL m_bGoods;
	BOOL m_bVolcano;

	//build options
	BOOL m_bRoads;
	BOOL m_bShips;
	BOOL m_bSettlements;
	BOOL m_bCities;
	BOOL m_bDev;
	BOOL m_bMove;

	//OpenGL settings
	int m_nMode;

	//whether we've saved the state or not
	BOOL m_bSaved;

	//resynch data
	int m_nResynch;
	int m_nData1;
	int m_nData2;
};
