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

#if !defined(AFX_GAMEDATABASE_H__7CB7DABA_CAF4_41DE_9974_3A1EC52C6162__INCLUDED_)
#define AFX_GAMEDATABASE_H__7CB7DABA_CAF4_41DE_9974_3A1EC52C6162__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "defineFile.h"

//////////////////////////////////////////////////////////////////////
// instance definition
//////////////////////////////////////////////////////////////////////
#define DATABASE		CGameDatabase::instance()

//////////////////////////////////////////////////////////////////////
// class declarations
//////////////////////////////////////////////////////////////////////
class CGame;
class CGLView;
class CUndoDialog;

//////////////////////////////////////////////////////////////////////
// static class for writing to and from the game database
//////////////////////////////////////////////////////////////////////
class CGameDatabase  
{
public:
	//////////////////////////////////////////////////////////////////
	// constructor / destructor
	//////////////////////////////////////////////////////////////////
	CGameDatabase();
	~CGameDatabase();

protected:
	/////////////////////////////////////////////////////////////////////////
	// a pointer to a static instance of this class
	/////////////////////////////////////////////////////////////////////////
	static CGameDatabase *m_pGameDatabase;

	//an array of game IDs
	static CArray <UINT, UINT> m_uiGames;

	//an array of offsets into the game file
	static CArray <DWORD, DWORD> m_dwOffsets;

	//arra of incomplete game IDs
	static CArray <UINT, UINT> m_uiIncompleteGames;

	//an array of offsets into the incomplete game file
	static CArray <DWORD, DWORD> m_dwIncompleteOffsets;

	//the current file version
	static float m_fVer;

	//are we reading a file in?
	static BOOL m_bReading;

	//the currently read game file
	static CGame *m_pGame;

	//opens the database and gets information
	static void open(void);
	static void openIncomplete(void);

public:
	/////////////////////////////////////////////////////////////////////
	// instanstiation/destruction methods
	/////////////////////////////////////////////////////////////////////
	static CGameDatabase *instance(void);
	static void release(void);

	//////////////////////////////////////////////////////////////////
	// information methods
	//////////////////////////////////////////////////////////////////
	static int numGames(void);
	static int numIncompleteGames(void);
	static float gameVer(void);
	static BOOL isReading(void);
	static CGame *getGame(void) {return m_pGame;}

	//////////////////////////////////////////////////////////////////
	// database methods
	//////////////////////////////////////////////////////////////////
	static void clearTemp(void);
	static BOOL copyTemp(BOOL bIncomplete = FALSE);
	static void getGame(int iGame, CGame *pGame);
	static void getIncompleteGame(int iGame, CGame *pGame);
	static int addGame(CGame *pGame, CUndoDialog *pUndo, CGLView *pGL, CString strGame = FILE_GAME, CString strKey = FILE_KEY, BOOL bRefresh = TRUE);
	static int updateGame(int iIndex, CGame *pGame, CUndoDialog *pUndo, CGLView *pGL);
	static void loadGame(int iGame, CGame *pGame, CUndoDialog *pUndo, CGLView *pGL);
	static void loadIncompleteGame(int iGame, CGame *pGame, CUndoDialog *pUndo, CGLView *pGL);
	static void deleteGame(int iGame);
	static void deleteIncompleteGame(int iGame);
	static void createExport(CString strFile, int iGames);
	static void importGames(CString strFile);
	static BOOL isLadder(int nGame);
};

#endif // !defined(AFX_GAMEDATABASE_H__7CB7DABA_CAF4_41DE_9974_3A1EC52C6162__INCLUDED_)
