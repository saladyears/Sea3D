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
#include "Settlers.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "SettlersDoc.h"
#include "SettlersView.h"
#include "GameTypeDialog.h"
#include "OpenGameDialog.h"
#include "PlayerDatabaseDialog.h"
#include "MapDatabaseDialog.h"
#include "GameDatabase.h"
#include "PlayerDatabase.h"
#include "ColorDatabase.h"
#include "MapDatabase.h"
#include "AIDatabase.h"
#include "NetworkPlayerDialog.h"
#include "NetworkServerDialog.h"
#include "NetworkClientDialog.h"
#include "rand.h"
#include <bass.h>
#include <direct.h>
#include "SettingsGeneralDialog.h"
#include "AboutDlg.h"
#include "SplashDlg.h"
#include "defineAI.h"
#include "defineNetwork.h"
#include "defineIni.h"
#include "defineFile.h"
#include "defineMap.h"
#include "defineDebug.h"
#include "defineColors.h"
#include ".\settlers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettlersApp

BEGIN_MESSAGE_MAP(CSettlersApp, CWinApp)
	//{{AFX_MSG_MAP(CSettlersApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_DATABASE_GAME, OnDatabaseGame)
	ON_COMMAND(ID_DATABASE_PLAYER, OnDatabasePlayer)
	ON_COMMAND(ID_DATABASE_MAPS, OnDatabaseMaps)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPTIONS_GENERAL, OnOptionsGeneral)
	ON_COMMAND(ID_TOUR_QUIT, OnTourQuit)
	ON_COMMAND(ID_FILE_TOUR, OnFileTour)
	ON_COMMAND(ID_HELP, OnHelp)
	//}}AFX_MSG_MAP
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettlersApp construction

CSettlersApp::CSettlersApp()
{
	//game mode
	m_iMode = 0;

	//null pointer
	m_pView = NULL;

	//no sounds yet
	m_bSoundInit = FALSE;

	//no AI yet
	m_hAI = NULL;
	m_lpnQuery = NULL;
	m_lpnShutdown = NULL;

	//no BASS yet
	m_hBass = NULL;

	m_lpnBassStart = NULL;
	m_lpnBassFree = NULL;
	m_lpnBassStop = NULL;
	m_lpnBassInit = NULL;
	m_lpnBassSampleLoad = NULL;
	m_lpnBassSamplePlay = NULL;

	//default to no tour
	m_bTour = FALSE;

	//no help yet
	m_bHelp = FALSE;

	//splash screen value
	m_iSplash = -1;

	//image lists
	m_pImageConnector = NULL;

	//create the connector
	m_pConnector = new CNetworkConnector;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSettlersApp object

CSettlersApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSettlersApp initialization

BOOL CSettlersApp::InitInstance()
{
	BOOL bValue;
	CSplashDlg dlg;
	BOOL bShowSplash;

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("S3D"));

#ifdef _TIMELIMIT
	if(FALSE == checkTime())
	{
		return FALSE;
	}
#endif //_TIMELIMIT

//build maps if needed
#ifdef DEBUG_BUILD_MAPS
	//remove the current maps
	remove(FILE_MAP);
	remove(FILE_MAPKEY);

	//add the base maps
	MAPS->addMap(MAP_SETTLERS_34);
	MAPS->addMap(MAP_SETTLERS_56);
	MAPS->addMap(MAP_SEA_NEW_SHORES);
	MAPS->addMap(MAP_SEA_FOUR_ISLANDS_III);
	MAPS->addMap(MAP_SEA_FOUR_ISLANDS_IV);
	MAPS->addMap(MAP_SEA_FIVE_ISLANDS_V);
	MAPS->addMap(MAP_SEA_SIX_ISLANDS_VI);
	MAPS->addMap(MAP_SEA_OCEANS_III);
	MAPS->addMap(MAP_SEA_OCEANS_IV);
	MAPS->addMap(MAP_SEA_OCEANS_V);
	MAPS->addMap(MAP_SEA_OCEANS_VI);
	MAPS->addMap(MAP_SEA_INTO_DESERT_III);
	MAPS->addMap(MAP_SEA_INTO_DESERT_IV);
	MAPS->addMap(MAP_SEA_INTO_DESERT_V);
	MAPS->addMap(MAP_SEA_INTO_DESERT_VI);
	MAPS->addMap(MAP_SEA_NEW_WORLD_III);
	MAPS->addMap(MAP_SEA_NEW_WORLD_IV);
	MAPS->addMap(MAP_SEA_NEW_WORLD_V);
	MAPS->addMap(MAP_SEA_NEW_WORLD_VI);
	MAPS->addMap(MAP_SEA_GREAT_CROSSING_IV);
	MAPS->addMap(MAP_SEA_GREAT_CROSSING_VI);
	MAPS->addMap(MAP_SEA_GREATER_CATAN_IV);
	MAPS->addMap(MAP_SEA_GREATER_CATAN_VI);

#endif //DEBUG_BUILD_MAPS

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER < 1300
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	// Check to see if the splash screen option is set in the INI file.
	bShowSplash = GetPrivateProfileInt(INI_GENERAL, INI_SPLASH, 1, INI_FILE); 

	//initialize AI
	initAI();

	//initialize sound library
	initSound();

	//initialize the databases
	initDatabases();

	//initialize image library
	ilInit();

	//build the image lists
	buildImageLists();

	//initialize help system
	initHelp();

	//run splash screen
	if( bShowSplash )
	{
		if(IDOK != dlg.DoModal())
		{
			return FALSE;
		}
	}
	else
	{
		m_iSplash = SPLASH_SKIP;
	}

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_SETTLETYPE,
		RUNTIME_CLASS(CSettlersDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSettlersView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	// Parse command line for standard shell commands, DDE, file open
	if(FALSE == initCommandLine())
	{
		return FALSE;
	}
	
	//initialize libraries
	if(FALSE == initLibraries())
	{
		return FALSE;
	}

	//mark us as having run the tour
	WritePrivateProfileString(INI_GENERAL, INI_TOUR, "0", INI_FILE);

	//save state
	if(SPLASH_HOTSEAT == m_iSplash || m_iSplash == SPLASH_NETWORK)
	{
		//hide the toolbar
		pMainFrame->saveToolbarState(FALSE);
	}
	else
	{
		//reload toolbar
		pMainFrame->saveToolbarState(TRUE);
	}

	// The main window has been initialized, hide it until done.
	m_pMainWnd = pMainFrame;
	pMainFrame->ShowWindow(SW_HIDE | SW_SHOWMINNOACTIVE);
	pMainFrame->UpdateWindow();

	//now handle splash
	switch(m_iSplash)
	{
	case SPLASH_HOTSEAT: m_iMode = GAME_MODE_HOTSEAT; runHotseat(); break;
	case SPLASH_NETWORK: m_iMode = GAME_MODE_NETWORK; runNetwork(); break;
	case SPLASH_OPEN: OnFileOpen(); break;
	case SPLASH_TOUR: m_bTour = TRUE; startTour(); break;
	}

	//get maximize value from .INI file
	bValue = GetPrivateProfileInt(INI_DISPLAY, INI_MAXIMIZED, FALSE, INI_FILE);

	//show the window
	m_pMainWnd->ShowWindow(bValue ? SW_MAXIMIZE : SW_RESTORE);
	m_pMainWnd->SetForegroundWindow();

	return TRUE;
}

// App command to run the dialog
void CSettlersApp::OnAppAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// initialization function
/////////////////////////////////////////////////////////////////////////////
BOOL CSettlersApp::checkTime(void)
{
	int iLength;
	CString strVer;
	CString strTime;
	COleDateTimeSpan span;

	//check for version
	strVer = GetProfileString("Beta", "Version");

	//if it's empty, wrong, or old add it
	if(strVer.IsEmpty() || strVer != CString(GAME_VER))
	{
		//write the current version
		WriteProfileString("Beta", "Version", GAME_VER);

		//write the current time
		WriteProfileInt("Beta", "Listen", time(NULL));
	}
	//they are the same, so see if we've expired
	else
	{
		//get the start time from the profile
		iLength = GetProfileInt("Beta", "Listen", 0);

		//create the time span
		span = COleDateTimeSpan(0, 0, 0, time(NULL) - iLength);

		//compare to alloted time
		if(span > TIME_LIMIT)
		{
			//tell them about
			MessageBox(NULL, "This version of Sea3D has expired.  Visit www.settlers3d.net to get the latest version.", "Sea3D Expiration", MB_OK);
			return FALSE;
		}
		//check for less than a day remaining
		else if(TIME_LIMIT - span <= COleDateTimeSpan(1, 0, 0, 0))
		{
			//tell them
			MessageBox(NULL, "There is less then one day until this version of Sea3D expires.", "Sea3D Expiration", MB_OK);
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// process command line
/////////////////////////////////////////////////////////////////////////////
BOOL CSettlersApp::initCommandLine()
{
	CCommandLineInfo cmdInfo;

	//prevent it from trying to start a new document
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if(FALSE == ProcessShellCommand(cmdInfo))
	{
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// initialize libraries
/////////////////////////////////////////////////////////////////////////////
BOOL CSettlersApp::initLibraries()
{
	//initialize OLE stuff
	if(FALSE == AfxOleInit())
	{
		AfxMessageBox("OLE initialization failed.");
		return FALSE;
	}

	//initalize socket stuff
	if(FALSE == AfxSocketInit())
	{
		AfxMessageBox("Socket initialization failed.");
		return FALSE;
	}

	//initalize rich edit
	if(FALSE == AfxInitRichEdit())
	{
		AfxMessageBox("Control initialization failed.");
		return FALSE;
	}

	//register stuff
	EnableShellOpen();
	RegisterShellFileTypes();
	COleObjectFactory::UpdateRegistryAll();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// start up AI
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::initAI()
{
	//try and load the AI .dll
	m_hAI = AfxLoadLibrary("AI.dll");

	//try and find it the proc addresses
	if(NULL != m_hAI)
	{
		//find function pointers
		m_lpnQuery = (LPNQUERYACTION) GetProcAddress(m_hAI, "QueryAction");
		m_lpnShutdown = (LPNSHUTDOWN) GetProcAddress(m_hAI, "Shutdown");

		//see if we fail
		if(NULL == m_lpnQuery || NULL == m_lpnShutdown)
		{
			AfxFreeLibrary(m_hAI);
			m_hAI = NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// init databases
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::initDatabases(void)
{
	DATABASE;
	PLAYERS;
	COLORS;
	MAPS;
	AI_DATABASE;
}

/////////////////////////////////////////////////////////////////////////////
// init current directory
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::initDirectory(void)
{
	//set the current directory
	char buffer[_MAX_PATH];

	if(NULL != _getcwd(buffer, _MAX_PATH))
	{
		m_strDirectory = buffer;
		m_strDirectory += '\\';
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSettlersApp message handlers

/////////////////////////////////////////////////////////////////////////////
// handle a new view
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::OnFileNew() 
{
	CGameTypeDialog dlg;
	
	//stop the tour
	if(TRUE == m_bTour)
	{
		stopTour();
	}

	//make sure this is the only document
	CloseAllDocuments(FALSE);

	//see what kind of game they want
	if(dlg.DoModal() != IDOK)
	{
		return;
	}

	//get the mode
	m_iMode = dlg.getMode();

	//hide the toolbar
	((CMainFrame *) m_pMainWnd)->saveToolbarState(FALSE);

	//run different things depending on what game mode we're in
	switch(m_iMode)
	{
	case GAME_MODE_HOTSEAT: runHotseat(); break;
	case GAME_MODE_NETWORK: runNetwork(); break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// actually creates the new window
/////////////////////////////////////////////////////////////////////////////
CFrameWnd * CSettlersApp::createView()
{
	POSITION pos;
	CDocTemplate *pTemplate;
	CDocument *pDoc;
	CFrameWnd *pFrame;
	
	//set a wait cursor
	CWaitCursor wc;

	//create a new child window
	pos = GetFirstDocTemplatePosition();
	pTemplate = GetNextDocTemplate(pos);

	//create the new document
	pDoc = pTemplate->CreateNewDocument();
	pFrame = pTemplate->CreateNewFrame(pDoc, NULL);
	pTemplate->InitialUpdateFrame(pFrame, pDoc, FALSE);

	//send initial update to all views (and other controls) in the frame
	//cheap override because WM_INITIALUPDATE is not defined (??)
	pFrame->SendMessageToDescendants(868, 0, 0, TRUE, TRUE);

	//return the frame
	return pFrame;
}

/////////////////////////////////////////////////////////////////////////////
// show a view
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::showView(CFrameWnd *pFrame)
{
	pFrame->ShowWindow(SW_MAXIMIZE);
	pFrame->ShowWindow(SW_SHOW);
}

/////////////////////////////////////////////////////////////////////////////
// run a hotseat game
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::runHotseat()
{
	CFrameWnd *pFrame;
	CNewGameDialog dlg;
	CMapDatabaseDialog map;

	//create the view
	pFrame = createView();

	//create control panel
	CONTROLS->createControls();
	ACTIONS->createControls();

	//seed the generator - one time
	sgenrand(time(NULL));

	//make sure we have a good pointer
	if(NULL == m_pView || NULL == pFrame)
	{
		AfxMessageBox("Error creating view.");
		return;
	}

	//GL must generate a board for us
	GAME->m_bGenerate = TRUE;

	if(IDOK != map.DoModal())
	{
		pFrame->SendMessage(WM_CLOSE);

		//reload toolbar
		((CMainFrame *) m_pMainWnd)->saveToolbarState(TRUE);

		return;
	}

	//load the map they chose
	MAPS->loadMap(map.getMap());

	//set variant settings
	VARIANTS->m_iStockRoads = MAP->m_iStockRoads;
	VARIANTS->m_iStockShips = MAP->m_iStockShips;
	VARIANTS->m_iStockCities = MAP->m_iStockCities;
	VARIANTS->m_iStockSettlements = MAP->m_iStockSettlements;
	VARIANTS->m_iAllowedCards = MAP->m_iAllowedCards;
	VARIANTS->m_iPoints = MAP->m_iPoints;

	//set the map ID
	GAME->m_uiMapID = MAPS->getMapID(map.getMap());

	//determine what players they need
	if(IDOK != dlg.DoModal())
	{
		pFrame->SendMessage(WM_CLOSE);

		//reload toolbar
		((CMainFrame *) m_pMainWnd)->saveToolbarState(TRUE);

		return;
	}

	//load the map
	m_pView->loadMap();

	//randomize the player order
	m_pView->randomizePlayers();
	m_pView->initNetwork();

	//set mode to player place
	m_pView->setNextMode();
	
	//show stuff
	showView(pFrame);

	//run the tour
	if(TRUE == m_bTour)
	{
		m_pView->handleBalloon();
	}
}

/////////////////////////////////////////////////////////////////////////////
// run a network game
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::runNetwork()
{
	int iMap;
	int iC1, iC2;
	CNetworkPlayerDialog player;
	CNetworkServerDialog server;
	CNetworkClientDialog client;
	CFrameWnd *pFrame;

	//get a player and connect type
	if(IDOK != player.DoModal())
	{
		//reload toolbar
		((CMainFrame *) m_pMainWnd)->saveToolbarState(TRUE);

		return;
	}
	
	//create the view
	pFrame = createView();

	//create control panel
	CONTROLS->createControls();
	ACTIONS->createControls();

	//seed the generator - one time
	sgenrand(time(NULL));

	//make sure we have a good pointer
	if(NULL == m_pView || NULL == pFrame)
	{
		AfxMessageBox("Error creating view.");
		return;
	}

	//get information
	GAME->m_bServer = player.hostGame();
	GAME->m_uiPlayerID = PLAYERS->getPlayer(player.playerSel()).getID();
	player.getColors(iC1, iC2);

	//GL must generate a board for us if we're the server
	GAME->m_bGenerate = GAME->m_bServer;

	//add ourselves to the current player list
	GAME->m_players.push_back(CPlayerGame());
	GAME->m_players[0].m_player = PLAYERS->getPlayer(player.playerSel());

	//if this is a ladder player, get the stats
	if(TRUE == GAME->m_players[0].m_player.getLadder())
	{
		GAME->m_players[0].m_player.setReport(CONNECTOR->reportPlayer(GAME->m_players[0].m_player.getName()));
		GAME->m_players[0].m_player.setIP(CONNECTOR->playerIP());
	}

	//default to desired color, but may change later
	GAME->m_players[0].m_iColor = iC1;

	//since this player is only in the game database, use the color values
	//to store their desired colors
	GAME->m_players[0].m_player.setGameColor(iC1);
	GAME->m_players[0].m_player.setColor1(iC1);
	GAME->m_players[0].m_player.setColor2(iC2);

	//run the right kind of dialog
	if(TRUE == GAME->m_bServer)
	{
		//set the server's ready flag to true
		GAME->m_players[0].m_player.setReady(TRUE);

		//server automatically gets their color
		GAME->m_bColors[iC1] = TRUE;

		if(IDOK != server.DoModal())
		{
			pFrame->SendMessage(WM_CLOSE);

			//reload toolbar
			((CMainFrame *) m_pMainWnd)->saveToolbarState(TRUE);

			return;
		}
	}
	else
	{
		if(IDOK != client.DoModal())
		{
			pFrame->SendMessage(WM_CLOSE);
		}

		//reload toolbar
		((CMainFrame *) m_pMainWnd)->saveToolbarState(TRUE);
		
		//return and let the server tell us what is happening
		return;
	}

	//get the map we're using
	iMap = server.getMap();
	
	//set the game map ID
	GAME->m_uiMapID = MAPS->getMapID(iMap);

	//map is already loaded into database when selected, so only GL needs to know
	m_pView->loadMap();

	//randomize the player order
	m_pView->randomizePlayers();

	//send all data to players
	m_pView->sendGameData();
	m_pView->initNetwork();
	m_pView->setNextMode();
	m_pView->setNetDrawMode(GL_WAIT);

	//set the game init
	GAME->m_bInit = TRUE;

	//show the view
	showView(pFrame);

	//start port placement if the map requires it
	if(TRUE == MAP->m_bAssignsPorts)
	{
		m_pView->serverPortPlace(TRUE);
	}
	//start player placement
	else
	{
		m_pView->serverPlayerPlace(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// returns the current view
/////////////////////////////////////////////////////////////////////////////
CSettlersView *CSettlersApp::getView()
{
	return m_pView;
}

/////////////////////////////////////////////////////////////////////////////
// cleanup
/////////////////////////////////////////////////////////////////////////////
int CSettlersApp::ExitInstance() 
{
	//close the databases down
	DATABASE->release();
	PLAYERS->release();
	COLORS->release();
	MAPS->release();
	AI_DATABASE->release();

	//clean up IL
	ilShutDown();

	if(NULL != m_hBass)
	{
		//clean up sound
		m_lpnBassStop();
		m_lpnBassFree();
	}

	//shut down AI
	if(NULL != m_hAI)
	{
		AfxFreeLibrary(m_hAI);
	}

	//shut down bass
	if(NULL != m_hBass)
	{
		AfxFreeLibrary(m_hBass);
	}

	//clean up connector
	if(NULL != m_pConnector)
	{
		delete m_pConnector;
		m_pConnector = NULL;
	}

	//cleanup image lists
	clearImageLists();

	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// run the game database manager
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::OnDatabaseGame() 
{
	int iFile = 0;
	CFrameWnd *pFrame;
	COpenGameDialog dlg;

	//run the open game dialog
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	//get the file wanted
	iFile = dlg.getFile();

	//make sure this is the only document
	CloseAllDocuments(FALSE);

	//hide the toolbar
	((CMainFrame *) m_pMainWnd)->saveToolbarState(FALSE);

	//create the view
	pFrame = createView();

	//create control panel
	CONTROLS->createControls();
	ACTIONS->createControls();

	//load the game
	VIEW->loadGame(iFile);

	//show the view
	showView(pFrame);
}

/////////////////////////////////////////////////////////////////////////////
// open the player database manager
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::OnDatabasePlayer() 
{
	CPlayerDatabaseDialog dlg;

	dlg.DoModal();	
}

/////////////////////////////////////////////////////////////////////////////
// open the maps manager
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::OnDatabaseMaps() 
{
	CMapDatabaseDialog dlg;

	//set the flag
	dlg.setSelection(FALSE);

	//run it
	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// see if it's a legitimate time top open a file
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((NULL == m_pView));
}

/////////////////////////////////////////////////////////////////////////////
// handle opening a game from the incomplete game database
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::OnFileOpen() 
{
	int iFile = 0;
	int iPlayer;
	BOOL bAI;
	long lUserID;
	CString str;
	CFrameWnd *pFrame;
	COpenGameDialog dlg;

	//set incomplete mode
	dlg.setIncomplete(TRUE);

	//run the open game dialog
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	//get the file wanted
	iFile = dlg.getFile();

	//make sure this is the only document
	CloseAllDocuments(FALSE);

	//seed the generator - one time
	sgenrand(time(NULL));

	//hide the toolbar
	((CMainFrame *) m_pMainWnd)->saveToolbarState(FALSE);

	//create the view
	pFrame = createView();

	//load the game
	VIEW->loadGame(iFile, TRUE);

	//create control panel
	CONTROLS->createControls();
	ACTIONS->createControls();

	if(GAME_MODE_NETWORK != GAME->m_iMode)
	{
		//show the view
		showView(pFrame);

		//update game content
		VIEW->updateGame();
	}
	//otherwise, run the connection process
	else
	{
		if(FALSE == reloadNetwork())
		{
			pFrame->SendMessage(WM_CLOSE);
			return;
		}

		//show the view
		showView(pFrame);

		//update game content
		VIEW->updateGame();

		//toggle actions off
		ACTIONS->ShowWindow(SW_HIDE);

		//get current player
		iPlayer = VIEW->getPlayer();

		//determine if AI
		bAI = GAME->m_players[iPlayer].m_player.getAI();

		//get the player we're not sending to
		if(FALSE == bAI)
		{
			lUserID = VIEW->getSocketID(GAME->m_players[iPlayer].m_player.getID());
		}
		else
		{
			lUserID = -10;
		}

		//update panel text
		VIEW->serverPanelText(S3D_RESTART_GAME, iPlayer, lUserID);

		//continue stuff
		if(iPlayer == GAME->m_iIndex)
		{
			//toggle controls
			CONTROLS->ShowWindow(SW_SHOW);

			//enter the turn
			CONTROLPANEL->enterTurn();
		}
		else if(TRUE == bAI)
		{
			//call turn actions but skip the roll
			VIEW->serverAITurn(TRUE);
		}
		else
		{
			//toggle controls
			CONTROLS->ShowWindow(SW_HIDE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// if we're in network mode, we need everyone to come back into the game
/////////////////////////////////////////////////////////////////////////////
BOOL CSettlersApp::reloadNetwork()
{
	int i;
	int iPlayer = -1;
	CNetworkServerDialog server;

	//search through the players for the player who was created here
	for(i = 0; i < (int) GAME->m_players.size(); i++)
	{
		//check
		if(TRUE == GAME->m_players[i].m_player.getMadeHere())
		{
			iPlayer = i;
			break;
		}
	}

	//this should not happen, but if the player is -1, we have a problem
	if(-1 == iPlayer)
	{
		AfxMessageBox("Sea3D could not find a server player in this save game.");
		return FALSE;
	}

	//set player and index
	GAME->m_uiPlayerID = GAME->m_players[i].m_player.getID();
	GAME->m_players[i].m_player.setReady(TRUE);
	GAME->m_iIndex = iPlayer;

	//set up the server dialog
	server.setReload(TRUE);

	//we're not inited yet
	GAME->m_bInit = FALSE;

	//run it
	if(IDOK != server.DoModal())
	{
		return FALSE;
	}

	//update data
	m_pView->sendSaveData();

	//we're good
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// initialize sound
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::initSound()
{
	BOOL bFail = FALSE;

	//try and load the bass .dll
	m_hBass = AfxLoadLibrary("bass.dll");

	//try and find proc addresses
	if(NULL != m_hBass)
	{
		//find function pointers
		m_lpnBassStart = (LPNBASS) GetProcAddress(m_hBass, "BASS_Start");
		m_lpnBassFree = (LPNBASS) GetProcAddress(m_hBass, "BASS_Free");
		m_lpnBassStop = (LPNBASS) GetProcAddress(m_hBass, "BASS_Stop");
		m_lpnBassInit = (LPNBASSINIT) GetProcAddress(m_hBass, "BASS_Init");
		m_lpnBassSampleLoad = (LPNBASSSAMPLELOAD) GetProcAddress(m_hBass, "BASS_SampleLoad");
		m_lpnBassSamplePlay = (LPNBASSSAMPLEPLAY) GetProcAddress(m_hBass, "BASS_SamplePlay");

		//check
		if(NULL == m_lpnBassStart || NULL == m_lpnBassFree || NULL == m_lpnBassStop ||
			NULL == m_lpnBassInit || NULL == m_lpnBassSampleLoad || NULL == m_lpnBassSamplePlay)
		{
			AfxFreeLibrary(m_hBass);
			m_hBass = NULL;
		}
	}

	//sanity
	if(NULL == m_hBass)
	{
		return;
	}

	//try initialization
	if(FALSE == m_lpnBassInit(-1, 44100, BASS_DEVICE_MONO | BASS_DEVICE_LEAVEVOL, m_pMainWnd->GetSafeHwnd()))
	{
		bFail = TRUE;
	}

	//start things up
	if(FALSE == m_lpnBassStart())
	{
		bFail = TRUE;
	}

	if(TRUE == bFail)
	{
		AfxFreeLibrary(m_hBass);
		m_hBass = NULL;
	}

	//set successful init sound
	m_bSoundInit = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// initializae help system
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::initHelp()
{
	//see if we have help
	CFile file;

	//try opening it
	m_bHelp = file.Open(FILE_HELP, CFile::modeRead);

	//close the file
	if(TRUE == m_bHelp)
	{
		file.Close();	
	}
}

/////////////////////////////////////////////////////////////////////////////
// play a selected sound
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::playSound(CString strSound)
{
	int iSound;
	HSAMPLE hSample = NULL;

	//sanity check
	if(NULL == m_hBass)
	{
		return;
	}

	//check .ini file to see if we've got sounds on
	iSound = GetPrivateProfileInt(INI_GENERAL, INI_SOUNDS, TRUE, INI_FILE);

	//see if they want it
	if(0 == iSound)
	{
		return;
	}

	//try and load the file
	hSample = (HSAMPLE) m_lpnBassSampleLoad(FALSE, (void *) (LPCTSTR) strSound, 0, 0, 5, 0);

	//make sure we got it
	if(NULL == hSample)
	{
		return;
	}

	//play the sound
	m_lpnBassSamplePlay(hSample);
}

/////////////////////////////////////////////////////////////////////////////
// run general options
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::OnOptionsGeneral() 
{
	CSettingsGeneralDialog dlg;

	dlg.DoModal();	
}

/////////////////////////////////////////////////////////////////////////////
// AI turn action
/////////////////////////////////////////////////////////////////////////////
int CSettlersApp::QueryAction(int iState, int iPlayer, CGame *pGame, CBoardPieceArray *pBoard, int *ID)
{
	int iRet;

	//sanity
	if(FALSE == hasAI())
	{
		return AI_ERR_NO_DLL;
	}

	//second sanity
	if(NULL == m_lpnQuery)
	{
		return AI_ERR_NO_FUNC;
	}

	//check params
	if(NULL == pGame || NULL == pBoard)
	{
		return AI_ERR_INVALID_PARAMS;
	}

	//call the function
	iRet = m_lpnQuery(iState, iPlayer, pGame, pBoard, ID);

	return iRet;
}

/////////////////////////////////////////////////////////////////////////////
// AI emergency shutdown
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::ShutdownAI()
{
	//sanity
	if(FALSE == hasAI())
	{
		return;
	}

	//second sanity
	if(NULL == m_lpnShutdown)
	{
		return;
	}

	//call the function
	m_lpnShutdown();
}

/////////////////////////////////////////////////////////////////////////////
// start the tour window
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::startTour()
{
	CRect rectWindow;
	CRect rectClient;

	//get rects
	GetMainWnd()->GetWindowRect(&rectWindow);
	GetMainWnd()->GetClientRect(&rectClient);

	//create point
	CPoint point = CPoint(rectWindow.left + (rectWindow.Width() - rectClient.Width() + 4),
						rectWindow.top + (rectWindow.Height() - rectClient.Height()) - 6);

	//balloon help test
	m_balloon.Create("Sea3D Tour",
		"Click on the File menu and select New\nto start a new Sea3D game.\n\nPress Esc to cancel the tour.", 
		point, 
		CBalloonHelp::unSHOW_INNER_SHADOW,
		NULL,
		"",
		0);
}

/////////////////////////////////////////////////////////////////////////////
// hide the tour window
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::stopTour()
{
	//sanity
	if(NULL != m_balloon.m_hWnd)
	{
		m_balloon.SendMessage(WM_CLOSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a tour quit
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::OnTourQuit() 
{
	if(TRUE == m_bTour)
	{
		m_bTour = FALSE;
		stopTour();
	}
}

/////////////////////////////////////////////////////////////////////////////
// start the tour manually
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::OnFileTour() 
{
	if(FALSE == m_bTour)
	{
		m_bTour = TRUE;
		startTour();
	}
}

/////////////////////////////////////////////////////////////////////////////
// run the help system
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::OnHelp() 
{
	if(TRUE == m_bHelp)
	{
    #if _MSC_VER >= 1300
		/* ### AEGGENBE : has to be found out how to call under vc7 */
		::HtmlHelp(GetDesktopWindow(), CString(FILE_HELP) + ">"  + HELP_MAIN, HH_DISPLAY_TOPIC, NULL);
    #else
		HtmlHelp(GetDesktopWindow(), CString(FILE_HELP) + ">"  + HELP_MAIN, HH_DISPLAY_TOPIC, NULL);
    #endif // _MSC_VER > 1300
	}
	else
	{
		::ShellExecute(NULL, NULL, CString(URL_HELP) + HELP_MAIN, NULL, NULL, SW_SHOWNORMAL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// read IL data from a file
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::readIL(int iSize, CString strFile, ILuint *pIL)
{
	int i;
	BOOL bLoad;
	CFile fileIn;
	DWORD dwLength;
	BYTE *byte;
	CString strIn;
	CString strErr;
	CString strExt;
	ILenum ilType;

	//build the image array
	ilGenImages(iSize, pIL);

	//open the file
	if(FALSE == fileIn.Open(strFile, CFile::modeRead))
	{
		strErr.Format("Could not open %s.  Check to see if this file exists.", strFile);
		AfxMessageBox(strErr);
		return;
	}

	//attach the archive
	CArchive ar(&fileIn, CArchive::load);

	//run through
	for(i = 0; i < iSize; i++)
	{
		//read the file name and length
		ar >> strIn;
		ar >> dwLength;

		//check file
		strExt = strIn.Right(3);

		//prepare the buffer
		byte = new BYTE[dwLength];

		//read the data
		ar.Read(byte, dwLength);

		//bind the name
		ilBindImage(pIL[i]);

		//set the load type
		if(0 == strExt.CompareNoCase("jpg")) ilType = IL_JPG;
		else if(0 == strExt.CompareNoCase("png")) ilType = IL_PNG;
		else ilType = IL_BMP;

		//load it
		bLoad = ilLoadL(ilType, (void *) byte, dwLength);

		//see if we were successful
		if(!bLoad)
		{
			strErr.Format("Could not read file %s from the file %s.", strIn, strFile);
			AfxMessageBox(strErr);
		}

		//clear the buffer
		delete[] byte;
		byte = NULL;
	}

	//cleanup
	ar.Close();
	fileIn.Close();
}

/////////////////////////////////////////////////////////////////////////////
// create image lists data
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::buildImageLists()
{
	int i;
	CBitmap *pBitmap;
	CDC *pDC;
	ILuint *pILTemp;

	//set the render mode
	ilutRenderer(ILUT_WIN32);

	//get the dc
	pDC = CDC::FromHandle(GetDC(GetActiveWindow()));

	//read in connector images
	pILTemp = new ILuint[CONNECT_SIZE];

	//read in the images
	APP->readIL(CONNECT_SIZE, FILE_CONNECTOR, pILTemp);

	//create the image list
	m_pImageConnector = new CImageList;
	m_pImageConnector->Create(11, 11, ILC_COLOR24 | ILC_MASK, 0, 0);

	//now fill
	for(i = 0; i < CONNECT_SIZE; i++)
	{
		//bind the image
		ilBindImage(pILTemp[i]);

		//create the bitmap
		pBitmap = CBitmap::FromHandle(ilutConvertToHBitmap(pDC->GetSafeHdc()));
		m_pImageConnector->Add(pBitmap, COLOR_BLACK);
	}

	//clear
	ReleaseDC(GetActiveWindow(), pDC->GetSafeHdc());

	//cleanup
	delete[] pILTemp;
}

/////////////////////////////////////////////////////////////////////////////
// clear image list data
/////////////////////////////////////////////////////////////////////////////
void CSettlersApp::clearImageLists()
{
	//image lists
	if(NULL != m_pImageConnector)
	{
		m_pImageConnector->DeleteImageList();
		delete m_pImageConnector;
		m_pImageConnector = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// return the IP address of this computer
/////////////////////////////////////////////////////////////////////////////
CString CSettlersApp::getIPAddress()
{
	char oip[50];
	char name[256];
	struct in_addr addr;

	//get the host name
	::gethostname(name, 256);

	struct hostent *phost = gethostbyname(name);

	//sanity check
	if(NULL == phost)
	{
		return "";
	}

	//copy the address
	memcpy(&addr, phost->h_addr_list[0], sizeof(struct in_addr));

	//ip address is inet_ntoa(addr)
	strncpy(oip, inet_ntoa(addr), sizeof(oip));

	return CString(oip);
}
