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

#include "stdafx.h"
#include "settlers.h"
#include "GameDatabase.h"
#include "Game.h"
#include "UndoDialog.h"
#include "GLView.h"
#include "PlayerDatabase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// static instantiations
/////////////////////////////////////////////////////////////////////////////
CGameDatabase *			CGameDatabase::m_pGameDatabase;
CArray <UINT, UINT>		CGameDatabase::m_uiGames;
CArray <DWORD, DWORD>	CGameDatabase::m_dwOffsets;
CArray <UINT, UINT>		CGameDatabase::m_uiIncompleteGames;
CArray <DWORD, DWORD>	CGameDatabase::m_dwIncompleteOffsets;
float					CGameDatabase::m_fVer;
BOOL					CGameDatabase::m_bReading;
CGame*					CGameDatabase::m_pGame;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGameDatabase::CGameDatabase()
{
	m_fVer = 0.0;
	m_bReading = FALSE;
	m_pGame = NULL;
}

CGameDatabase::~CGameDatabase()
{

}

/////////////////////////////////////////////////////////////////////////////
// instance handling
/////////////////////////////////////////////////////////////////////////////
CGameDatabase *CGameDatabase::instance()
{
	//see if a game database already is open
	if(NULL == m_pGameDatabase)
	{
		//get a new audit handle
		m_pGameDatabase = new CGameDatabase();

		//get preliminary information
		open();
		openIncomplete();
	}

	//return the instance
	return m_pGameDatabase;
}

/////////////////////////////////////////////////////////////////////////////
// release data
/////////////////////////////////////////////////////////////////////////////
void CGameDatabase::release()
{
	//delete the static instance if it exists
	if(NULL != m_pGameDatabase)
	{
		delete m_pGameDatabase;
		m_pGameDatabase = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// return the number of games in the database
/////////////////////////////////////////////////////////////////////////////
int CGameDatabase::numGames()
{
	return m_uiGames.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// return the number of incomplete games in the database
/////////////////////////////////////////////////////////////////////////////
int CGameDatabase::numIncompleteGames()
{
	return m_uiIncompleteGames.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// return the current file version
/////////////////////////////////////////////////////////////////////////////
float CGameDatabase::gameVer()
{
	return m_fVer;
}

/////////////////////////////////////////////////////////////////////////////
// return whether we're reading a file or not
/////////////////////////////////////////////////////////////////////////////
BOOL CGameDatabase::isReading()
{
	return m_bReading;
}

/////////////////////////////////////////////////////////////////////////////
// returns a specified game data structure from the game database
/////////////////////////////////////////////////////////////////////////////
void CGameDatabase::getGame(int iGame, CGame *pGame)
{
	CFile file;

	//open the database file
	if(FALSE == file.Open(FILE_GAME, CFile::modeRead)) 
	{
		//set outgoing data to NULL
		pGame = NULL;

		return;
	}

	//seek to this position in the game file
	file.Seek(m_dwOffsets[iGame], CFile::begin);
	CArchive ar(&file, CArchive::load);

	//load the file version
	ar >> m_fVer;
	
	//currently reading
	m_bReading = TRUE;

	//read the game data
	pGame->Serialize(ar);

	//no longer reading
	m_bReading = FALSE;

	//cleanup
	ar.Close();
	file.Close();
}

/////////////////////////////////////////////////////////////////////////////
// returns a specified game data structure from the incomplete game database
/////////////////////////////////////////////////////////////////////////////
void CGameDatabase::getIncompleteGame(int iGame, CGame *pGame)
{
	CFile file;

	//open the database file
	if(FALSE == file.Open(FILE_INGAME, CFile::modeRead)) 
	{
		//set outgoing data to NULL
		pGame = NULL;

		return;
	}

	//seek to this position in the game file
	file.Seek(m_dwIncompleteOffsets[iGame], CFile::begin);
	CArchive ar(&file, CArchive::load);

	//load the file version
	ar >> m_fVer;
	
	//currently reading
	m_bReading = TRUE;

	//read the game data
	pGame->Serialize(ar);

	//no longer reading
	m_bReading = FALSE;

	//cleanup
	ar.Close();
	file.Close();
}

/////////////////////////////////////////////////////////////////////////////
// opens the database and gets information
/////////////////////////////////////////////////////////////////////////////
void CGameDatabase::open(void)
{
	int i, iSize;
	DWORD dwLength;
	CFile file;
	CFile key;
	CGame *pGame;

	//reset the array of game IDs
	m_uiGames.RemoveAll();

	//if they don't exist, it's the first time they've done anything
	if(!file.Open(FILE_GAME, CFile::modeRead)) return;
	if(!key.Open(FILE_KEY, CFile::modeRead)) return;
	
	//create the key archive
	CArchive arKey(&key, CArchive::load);

	//get the length of the file
	dwLength = (DWORD) key.GetLength();

	//calculate the number of games
	iSize = dwLength / sizeof(DWORD);

	//resize the offset array
	m_dwOffsets.SetSize(iSize);

	//keep going until we reach the end of the file
	for(i = 0; i < iSize; i++)
	{
		//read in the offset
		arKey >> m_dwOffsets[i];
	}

	//clean up
	arKey.Close();
	key.Close();

	//now read in all the game IDs
	for(i = 0; i < iSize; i++)
	{
		//create new data structure
		pGame = new CGame;

		//seek to this position in the game file
		file.Seek(m_dwOffsets[i], CFile::begin);

		//read in the buffer
		CArchive ar(&file, CArchive::load);

		//read in the version
		ar >> m_fVer;

		//currently reading
		m_bReading = TRUE;

		//read in the data
		pGame->Serialize(ar);

		//get the game ID
		m_uiGames.Add(pGame->m_uiID);

		//no longer reading
		m_bReading = FALSE;

		//close the buffer
		ar.Close();

		//delete data structure
		delete pGame;
	}

	//close the files
	file.Close();

	//null the pointer
	pGame = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// opens the incomplete game database and gets information
/////////////////////////////////////////////////////////////////////////////
void CGameDatabase::openIncomplete()
{
	int i, iSize;
	DWORD dwLength;
	CFile file;
	CFile key;
	CGame *pGame;

	//reset the array of game IDs
	m_uiIncompleteGames.RemoveAll();

	//if they don't exist, it's the first time they've done anything
	if(FALSE == file.Open(FILE_INGAME, CFile::modeRead))
	{
		return;
	}

	if(FALSE == key.Open(FILE_INKEY, CFile::modeRead)) 
	{
		return;
	}
	
	//create the key archive
	CArchive arKey(&key, CArchive::load);

	//get the length of the file
	dwLength = (DWORD) key.GetLength();

	//calculate the number of games
	iSize = dwLength / sizeof(DWORD);

	//resize the offset array
	m_dwIncompleteOffsets.SetSize(iSize);

	//keep going until we reach the end of the file
	for(i = 0; i < iSize; i++)
	{
		//read in the offset
		arKey >> m_dwIncompleteOffsets[i];
	}

	//clean up
	arKey.Close();
	key.Close();

	//now read in all the game IDs
	for(i = 0; i < iSize; i++)
	{
		//create new data structure
		pGame = new CGame;

		//seek to this position in the game file
		file.Seek(m_dwIncompleteOffsets[i], CFile::begin);

		//read in the buffer
		CArchive ar(&file, CArchive::load);

		//read in the version
		ar >> m_fVer;

		//currently reading
		m_bReading = TRUE;

		//read in the data
		pGame->Serialize(ar);

		//get the game ID
		m_uiIncompleteGames.Add(pGame->m_uiID);

		//no longer reading
		m_bReading = FALSE;

		//close the buffer
		ar.Close();

		//delete data structure
		delete pGame;
	}

	//close the files
	file.Close();

	//null the pointer
	pGame = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// add a game to the database
/////////////////////////////////////////////////////////////////////////////
int CGameDatabase::addGame(CGame *pGame, CUndoDialog *pUndo, CGLView *pGL, CString strGame, CString strKey, BOOL bRefresh)
{
	int iIndex = 0;
	DWORD dwStart;
	DWORD dwLength;

	//open the database file
	CFile file(strGame, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);

	//seek to the end (append)
	file.SeekToEnd();

	//get the starting position
	dwStart = (DWORD) file.GetPosition();

	//create the archive
	CArchive ar(&file, CArchive::store);

	//output the file version
	ar << (float) FILE_VER;

	//write the game data
	pGame->Serialize(ar);

	//write the undo dialog data
	pUndo->Serialize(ar);

	//write the board data
	pGL->Serialize(ar);

	//flush the data
	ar.Close();

	//close the file
	file.Close();

	if(!strKey.IsEmpty())
	{
		//now open the offset file
		file.Open(strKey, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);

		//get to the end
		file.SeekToEnd();

		//open the archive
		CArchive arKey(&file, CArchive::store);

		//dump the offset for this file
		arKey << dwStart;

		//flush it
		arKey.Close();

		//get the length of the file
		dwLength = (DWORD) file.GetLength();

		//calculate the number of games
		iIndex = (dwLength / sizeof(DWORD)) - 1;

		//close the file
		file.Close();
	}

	//recalculate if needed
	if(TRUE == bRefresh)
	{
		open();
		openIncomplete();
	}

	//return the added index
	return iIndex;
}

/////////////////////////////////////////////////////////////////////////////
// update a game in the incomplete database
/////////////////////////////////////////////////////////////////////////////
int CGameDatabase::updateGame(int iIndex, CGame *pGame, CUndoDialog *pUndo, CGLView *pGL)
{
	//if we have an invalid index, return -1, which will cause it to be newly saved next time
	if(iIndex < 0 || iIndex >= m_uiIncompleteGames.GetSize())
	{
		return -1;
	}

	//first delete the game from the file
	deleteIncompleteGame(iIndex);

	//readd the game
	iIndex = addGame(pGame, pUndo, pGL, FILE_INGAME, FILE_INKEY);

	//return the new index
	return iIndex;
}

/////////////////////////////////////////////////////////////////////////////
// check if a game is a ladder game or not
/////////////////////////////////////////////////////////////////////////////
BOOL CGameDatabase::isLadder(int nGame)
{
	BOOL bLadder;
	CFile file;
	CGame *pGame;

	//open the database file
	file.Open(FILE_GAME, CFile::modeRead);

	//create the archive
	CArchive ar(&file, CArchive::load);

	//jump to the right place
	file.Seek(m_dwOffsets[nGame], CFile::begin);

	//load the file version
	ar >> m_fVer;

	//create the new game
	pGame = new CGame;
	
	//currently reading
	m_bReading = TRUE;

	//read the game data
	pGame->Serialize(ar);

	//no longer reading
	m_bReading = FALSE;

	//flush the archive
	ar.Close();

	//close the file
	file.Close();

	//get the value
	bLadder = pGame->m_bLadder;

	//cleanup
	delete pGame;
	pGame = NULL;

	return bLadder;
}

/////////////////////////////////////////////////////////////////////////////
// open a game from the database
/////////////////////////////////////////////////////////////////////////////
void CGameDatabase::loadGame(int iGame, CGame *pGame, CUndoDialog *pUndo, CGLView *pGL)
{
	CFile file;

	//open the database file
	file.Open(FILE_GAME, CFile::modeRead);

	//create the archive
	CArchive ar(&file, CArchive::load);

	//jump to the right place
	file.Seek(m_dwOffsets[iGame], CFile::begin);

	//load the file version
	ar >> m_fVer;
	
	//currently reading
	m_bReading = TRUE;

	//read the game data
	pGame->Serialize(ar);

	//set the currently read GL
	m_pGame = pGame;	

	//read the undo dialog data
	pUndo->Serialize(ar);

	//read the board data
	pGL->Serialize(ar);

	//no longer reading
	m_bReading = FALSE;

	//flush the archive
	ar.Close();

	//close the file
	file.Close();

	//restore
	m_pGame = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// open a game from the incomplete database
/////////////////////////////////////////////////////////////////////////////
void CGameDatabase::loadIncompleteGame(int iGame, CGame *pGame, CUndoDialog *pUndo, CGLView *pGL)
{
	CFile file;

	//open the database file
	file.Open(FILE_INGAME, CFile::modeRead);

	//create the archive
	CArchive ar(&file, CArchive::load);

	//jump to the right place
	file.Seek(m_dwIncompleteOffsets[iGame], CFile::begin);

	//load the file version
	ar >> m_fVer;
	
	//currently reading
	m_bReading = TRUE;

	//read the game data
	pGame->Serialize(ar);

	//set the currently read GL
	m_pGame = pGame;	

	//read the undo dialog data
	pUndo->Serialize(ar);

	//read the board data
	pGL->Serialize(ar);

	//no longer reading
	m_bReading = FALSE;

	//flush the archive
	ar.Close();

	//close the file
	file.Close();

	//restore
	m_pGame = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// delete a game from the database
/////////////////////////////////////////////////////////////////////////////
void CGameDatabase::deleteGame(int iGame)
{
	int i, iSize;
	CGame *pGame;
	CGLView *pGL;
	CUndoDialog *pUndo;

	//make sure the temp files are cleared
	clearTemp();

	//get the number of games
	iSize = numGames();

	//if there's only one game in the file, just delete the files
	if(1 == iSize)
	{
		::DeleteFile(FILE_GAME);
		::DeleteFile(FILE_KEY);
	}

	//run through, loading and saving
	for(i = 0; i < iSize; i++)
	{
		//skip the one we're deleting
		if(i == iGame) continue;

		//create a new data structures
		pGL = new CGLView;
		pGame = new CGame;
		pUndo = new CUndoDialog;

		//instantiate
		pUndo->Create(IDD_UNDO_DLG, NULL);
		pUndo->ShowWindow(SW_HIDE);

		//load the game unitialized
		loadGame(i, pGame, pUndo, pGL);

		//save the game to the temp file
		addGame(pGame, pUndo, pGL, FILE_TEMP, FILE_TEMPKEY);

		//clear data
		delete pGame;
		delete pUndo;
		delete pGL;
	}

	//copy the temp file
	copyTemp();

	//NULL the used data structures
	pGame = NULL;
	pGL = NULL;
	pUndo = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// delete a game from the incomplete game database
/////////////////////////////////////////////////////////////////////////////
void CGameDatabase::deleteIncompleteGame(int iGame)
{
	int i, iSize;
	CGame *pGame;
	CGLView *pGL;
	CUndoDialog *pUndo;

	//make sure the temp files are cleared
	clearTemp();

	//get the number of games
	iSize = numIncompleteGames();

	//if there's only one game in the file, just delete the files
	if(1 == iSize)
	{
		::DeleteFile(FILE_INGAME);
		::DeleteFile(FILE_INKEY);
	}

	//run through, loading and saving
	for(i = 0; i < iSize; i++)
	{
		//skip the one we're deleting
		if(i == iGame) continue;

		//create a new data structures
		pGL = new CGLView;
		pGame = new CGame;
		pUndo = new CUndoDialog;

		//instantiate
		pUndo->Create(IDD_UNDO_DLG, NULL);
		pUndo->ShowWindow(SW_HIDE);

		//load the game unitialized
		loadIncompleteGame(i, pGame, pUndo, pGL);

		//save the game to the temp file
		addGame(pGame, pUndo, pGL, FILE_TEMP, FILE_TEMPKEY, FALSE);

		//clear data
		delete pGame;
		delete pUndo;
		delete pGL;
	}

	//copy the temp file
	copyTemp(TRUE);

	//NULL the used data structures
	pGame = NULL;
	pGL = NULL;
	pUndo = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// copy the temp file over
/////////////////////////////////////////////////////////////////////////////
BOOL CGameDatabase::copyTemp(BOOL bIncomplete)
{
	//copy the temp files to the regular files
	if(TRUE == bIncomplete)
	{
		::CopyFile(FILE_TEMP, FILE_INGAME, FALSE);
		::CopyFile(FILE_TEMPKEY, FILE_INKEY, FALSE);

		//recalculate
		openIncomplete();
	}
	else
	{
		::CopyFile(FILE_TEMP, FILE_GAME, FALSE);
		::CopyFile(FILE_TEMPKEY, FILE_KEY, FALSE);
		
		//recalculate
		open();
	}

	//delete the temp files
	clearTemp();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// delete any temp files
/////////////////////////////////////////////////////////////////////////////
void CGameDatabase::clearTemp()
{
	//delete the temp files
	::DeleteFile(FILE_TEMP);
	::DeleteFile(FILE_TEMPKEY);
}

/////////////////////////////////////////////////////////////////////////////
// start off the export file
/////////////////////////////////////////////////////////////////////////////
void CGameDatabase::createExport(CString strFile, int iGames)
{
	CFile file;

	//open the file
	file.Open(strFile, CFile::modeCreate | CFile::modeWrite);

	//create the archive
	CArchive ar(&file, CArchive::store);

	//send out the identifiers
	ar << EXPORT_FLAG;
	ar << iGames;

	//close all
	ar.Close();
	file.Close();
}

/////////////////////////////////////////////////////////////////////////////
// start reading in files
/////////////////////////////////////////////////////////////////////////////
void CGameDatabase::importGames(CString strFile)
{
	int i, j, iGames;
	int iSize;
	int iImports = 0;
	BOOL bNew;
	CFile import;
	CString str;
	CGame *pGame;
	CGLView *pGL;
	CUndoDialog *pUndo;

	//open the import file
	if(!import.Open(strFile, CFile::modeRead))
	{
		str.Format("%s cannot be found.", strFile);
		MessageBox(NULL, str, "Import error", MB_OK);
		return;
	}

	//create the archive
	CArchive ar(&import, CArchive::load);

	//check for validity
	ar >> str;

	//compare
	if(str != EXPORT_FLAG)
	{
		str.Format("%s is not a valid Sea3D import file.", strFile);
		MessageBox(NULL, str, "Import error", MB_OK);
		return;
	}

	//get the number of current games
	iSize = numGames();

	//read in the number of games stored
	ar >> iGames;

	//run through the list
	for(i = 0; i < iGames; i++)
	{
		//create new data structures
		pGL = new CGLView;
		pGame = new CGame;
		pUndo = new CUndoDialog;

		//instantiate
		pUndo->Create(IDD_UNDO_DLG, NULL);
		pUndo->ShowWindow(SW_HIDE);

		//read in the file version
		ar >> m_fVer;

		//reading
		m_bReading = TRUE;

		//serialize
		pGame->Serialize(ar);
		pUndo->Serialize(ar);
		pGL->Serialize(ar);

		//not reading
		m_bReading = FALSE;

		//start off as if the game is a new one
		bNew = TRUE;

		//now see if it's already in the game database
		for(j = 0; j < iSize; j++)
		{
			//if it's there, this is not a new game
			if(pGame->m_uiID == m_uiGames[j])
			{
				bNew = FALSE;
				break;
			}
		}

		//if it's new, add it in!
		if(TRUE == bNew)
		{
			//no refresh, saves reopening after every game is added
			addGame(pGame, pUndo, pGL, FILE_GAME, FILE_KEY, FALSE);
			iImports++;
		}

		//delete the used data structures
		delete pGame;
		delete pUndo;
		delete pGL;
	}

	//close down all
	ar.Close();
	import.Close();

	if(0 < iImports)
	{
		//recalculate
		open();

		//give a message box
		str.Format("%d %s %s successfully imported from %s.", iImports, (iImports > 1) ? "games" : "game", (iImports > 1) ? "were" : "was", strFile);

		MessageBox(NULL, str, "Import games", MB_OK);
	}
	else
	{
		str.Format("There were no new games in %s.", strFile);

		//give the message box
		MessageBox(NULL, str, "Import games", MB_OK);
	}
}