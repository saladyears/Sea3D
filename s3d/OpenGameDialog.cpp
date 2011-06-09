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
#include "OpenGameDialog.h"
#include "XLogFont.h"
#include "GLView.h"
#include "UndoDialog.h"
#include "GameDatabase.h"
#include "NetworkConnector.h"
#include "defineConnector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// global function declarations
/////////////////////////////////////////////////////////////////////////////
int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
void OnGameDatabaseClick(DWORD dwCookie);

/////////////////////////////////////////////////////////////////////////////
// COpenGameDialog dialog


COpenGameDialog::COpenGameDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(COpenGameDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenGameDialog)
	//}}AFX_DATA_INIT

	//the current selection (no selection)
	m_iFile = -1;

	//default to normal game database
	m_bIncomplete = FALSE;
}


void COpenGameDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenGameDialog)
	DDX_Control(pDX, IDC_IMPORT_BUTTON, m_ImportButton);
	DDX_Control(pDX, IDC_EXPORT_BUTTON, m_ExportButton);
	DDX_Control(pDX, IDC_OPEN_BUTTON, m_OpenButton);
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_DeleteButton);
	DDX_Control(pDX, IDC_REPOST_BUTTON, m_RepostButton);
	DDX_Control(pDX, IDC_GAME_LIST, m_GameList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenGameDialog, CHelpDialog)
	//{{AFX_MSG_MAP(COpenGameDialog)
	ON_BN_CLICKED(IDC_OPEN_BUTTON, OnOpenButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, OnDeleteButton)
	ON_BN_CLICKED(IDC_EXPORT_BUTTON, OnExportButton)
	ON_BN_CLICKED(IDC_IMPORT_BUTTON, OnImportButton)
	ON_BN_CLICKED(IDC_REPOST_BUTTON, OnRepostButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGameDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// initialize the dialog
/////////////////////////////////////////////////////////////////////////////
BOOL COpenGameDialog::OnInitDialog() 
{
	//set the list control
	m_GameList.setIncomplete(m_bIncomplete);
	
	CHelpDialog::OnInitDialog();

	//set the help system
	if(TRUE == m_bIncomplete)
	{
		m_strHelp = HELP_OPENGAME;
	}
	else
	{
		m_strHelp = HELP_DBGAMES;
	}

	//set the callback function
	m_GameList.setCallBack(OnGameDatabaseClick, (DWORD) this);

	//disable the open and delete buttons until something is selected
	m_OpenButton.EnableWindow(FALSE);
	m_DeleteButton.EnableWindow(FALSE);
	m_ExportButton.EnableWindow(FALSE);
	m_RepostButton.EnableWindow(FALSE);	

	//set window text
	if(TRUE == m_bIncomplete)
	{
		SetWindowText("Select a game to open");
		m_ExportButton.ShowWindow(SW_HIDE);
		m_ImportButton.ShowWindow(SW_HIDE);
		m_RepostButton.ShowWindow(SW_HIDE);
	}
	else
	{
		SetWindowText("Game Database");
		m_ExportButton.ShowWindow(SW_SHOW);
		m_ImportButton.ShowWindow(SW_SHOW);
		m_RepostButton.ShowWindow(SW_SHOW);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// handle a selection change
/////////////////////////////////////////////////////////////////////////////
void COpenGameDialog::handleSelection()
{
	int nSel;

	//get the selection
	nSel = m_GameList.querySelection();

	//enable the windows
	m_OpenButton.EnableWindow(nSel >= 0);
	m_DeleteButton.EnableWindow(nSel >= 0);
	m_ExportButton.EnableWindow(nSel >= 0 || nSel == -2);

	//respost ladder games
	m_RepostButton.EnableWindow(nSel >= 0 && (FALSE == m_bIncomplete) && (TRUE == DATABASE->isLadder(nSel)));
}

/////////////////////////////////////////////////////////////////////////////
// open a file
/////////////////////////////////////////////////////////////////////////////
void COpenGameDialog::OnOpenButton() 
{
	int nSel;

	//get the selection
	nSel = m_GameList.querySelection();

	//sanity check
	if(nSel < 0)
	{
		return;
	}

	//set the file number
	m_iFile = (int) m_GameList.GetItemData(nSel);

	//open the file
	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// delete a game from the database
/////////////////////////////////////////////////////////////////////////////
void COpenGameDialog::OnDeleteButton() 
{
	int nSel;
	int iGame;

	//get the selection
	nSel = m_GameList.querySelection();

	//sanity check
	if(nSel < 0)
	{
		return;
	}

	//see if they really want to
	if(IDNO == MessageBox("Are you sure you want to delete this game from the database?", "Delete game?", MB_YESNO)) 
	{
		return;
	}

	//get the game to delete
	iGame = m_GameList.GetItemData(nSel);

	//delete the game
	if(TRUE == m_bIncomplete)
	{
		DATABASE->deleteIncompleteGame(iGame);
	}
	else
	{
		DATABASE->deleteGame(iGame);
	}

	//remove it from the list
	m_GameList.DeleteItem(nSel);

	//reload
	m_GameList.readGames();

	//reset the selection
	m_DeleteButton.EnableWindow(FALSE);
	m_OpenButton.EnableWindow(FALSE);
	m_ExportButton.EnableWindow(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// export games to a file
/////////////////////////////////////////////////////////////////////////////
void COpenGameDialog::OnExportButton() 
{
	int nSel;
	DWORD dwFlags;
	POSITION pos;
	vector <int> iGames;

	//get the selection
	nSel = m_GameList.querySelection();

	//sanity check
	if(-1 == nSel)
	{
		return;
	}

	//see if there are any items selected
	pos = m_GameList.GetFirstSelectedItemPosition();
	
	while(pos)
	{
		//add the selections to the array
		iGames.push_back(m_GameList.GetNextSelectedItem(pos));
	}
	
	//set the flags
	dwFlags = OFN_NONETWORKBUTTON | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	//create a file open dialog to get the name
	CFileDialog dlg(FALSE, ".s3d", NULL, dwFlags, EXPORT_FILTER, this);

	//run the dialog
	if(IDCANCEL == dlg.DoModal())
	{
		return;
	}

	//export the selected games
	exportGames(dlg.GetPathName(), iGames);
}

/////////////////////////////////////////////////////////////////////////////
// actually export games to a file
/////////////////////////////////////////////////////////////////////////////
void COpenGameDialog::exportGames(CString strFile, vector <int> iGames)
{
	int i, j, iSize;
	CString str;
	CGame *pGame;
	CGLView *pGL;
	CUndoDialog *pUndo;
	CWaitCursor wc;

	//get the number of games being exported
	iSize = (int) iGames.size();

	//start the export file
	DATABASE->createExport(strFile, iSize);

	//now, run through the games to export
	for(i = 0; i < iSize; i++)
	{
		//create new data structures
		pGL = new CGLView;
		pGame = new CGame;
		pUndo = new CUndoDialog;

		//instantiate
		pUndo->Create(IDD_UNDO_DLG, this);
		pUndo->ShowWindow(SW_HIDE);

		//load the game uninitialized
		DATABASE->loadGame(iGames[i], pGame, pUndo, pGL);

		//set all the players to not made here
		for(j = 0; j < (int) pGame->m_players.size(); j++)
		{
			pGame->m_players[j].m_player.setMadeHere(FALSE);
		}

		//save it to the export file
		DATABASE->addGame(pGame, pUndo, pGL, strFile, "");
		
		//delete the used data structures
		delete pGame;
		delete pUndo;
		delete pGL;
	}

	//give the user notification that all went well
	str.Format("%d %s %s successfully exported to %s.", iSize, (iSize > 1) ? "games" : "game", (iSize > 1) ? "were" : "was", strFile);
	MessageBox(str, "Export games");

	//clear pointers
	pGame = NULL;
	pGL = NULL;
	pUndo = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// handle importing games from a file
/////////////////////////////////////////////////////////////////////////////
void COpenGameDialog::OnImportButton() 
{
	DWORD dwFlags;

	//set the flags
	dwFlags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

	//create a file open dialog to get the name
	CFileDialog dlg(TRUE, ".s3d", NULL, dwFlags, EXPORT_FILTER, this);

	//run the dialog
	if(IDCANCEL == dlg.DoModal())
	{
		return;
	}

	//export the selected games
	DATABASE->importGames(dlg.GetPathName());

	//reset the list
	m_GameList.readGames();
}

/////////////////////////////////////////////////////////////////////////////
// handle reposting a game to the ladder
/////////////////////////////////////////////////////////////////////////////
void COpenGameDialog::OnRepostButton()
{
	int nSel;

	//get the selection
	nSel = m_GameList.querySelection();

	//determine if the game was played in the last two weeks
	CGame *pGame;

	//create game data
	pGame = new CGame;

	//get the game data
	DATABASE->getGame(nSel, pGame);

	COleDateTimeSpan span = COleDateTime::GetCurrentTime() - pGame->m_timeEnd;

	delete pGame;
	pGame = NULL;

	if(14 < span.GetTotalDays())
	{
		MessageBox("You cannot repost games older than two weeks.", "Repost");
		return;
	}

	//respost it
	CONNECTOR->repostGame(nSel);
}

/////////////////////////////////////////////////////////////////////////////
// callback handler
/////////////////////////////////////////////////////////////////////////////
void OnGameDatabaseClick(DWORD dwCookie)
{
	if(NULL != dwCookie)
	{
		((COpenGameDialog *) dwCookie)->handleSelection();
	}
}