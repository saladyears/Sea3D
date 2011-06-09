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

#if !defined(AFX_SETTLERS_H__1EC819D4_7065_4715_B76E_2D621955E765__INCLUDED_)
#define AFX_SETTLERS_H__1EC819D4_7065_4715_B76E_2D621955E765__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"       // main symbols
#include "BoardPiece.h"
#include "BalloonHelp.h"
#include <il\il.h>

/////////////////////////////////////////////////////////////////////////////
// typedefs
/////////////////////////////////////////////////////////////////////////////
typedef CArray <CBoardPiece, CBoardPiece> CBoardPieceArray;

/////////////////////////////////////////////////////////////////////////////
// forward declarations
/////////////////////////////////////////////////////////////////////////////
class CGame;
class CNetworkConnector;

/////////////////////////////////////////////////////////////////////////////
// .dll typedefs
/////////////////////////////////////////////////////////////////////////////
//AI
typedef int (EXPORT* LPNQUERYACTION)(int, int, CGame *, CBoardPieceArray *, int *);
typedef void (EXPORT *LPNSHUTDOWN)(void);

//BASS
typedef BOOL (WINAPI *LPNBASS)(void);
typedef BOOL (WINAPI *LPNBASSINIT) (int, DWORD, DWORD, HWND);
typedef DWORD (WINAPI *LPNBASSSAMPLELOAD) (BOOL, void *, DWORD, DWORD, DWORD, DWORD);
typedef DWORD (WINAPI *LPNBASSSAMPLEPLAY) (DWORD);

/////////////////////////////////////////////////////////////////////////////
// forward class declarations
/////////////////////////////////////////////////////////////////////////////
class CSettlersView;

/////////////////////////////////////////////////////////////////////////////
// CSettlersApp:
// See Settlers.cpp for the implementation of this class
//

class CSettlersApp : public CWinApp
{
public:
	CSettlersApp();

	//return what mode we're in
	inline int getMode(void) {return m_iMode;}

	//set the view pointer
	inline void setView(CSettlersView *v) {m_pView = v;}

	//do we have AI?
	inline BOOL hasAI(void) {return (m_hAI != NULL);}

	//do we have help?
	inline BOOL hasHelp(void) {return m_bHelp;}

	//are we in a tour?
	inline BOOL inTour(void) {return m_bTour;}
	inline void setTour(BOOL b) {m_bTour = b;}

	//splash screen values
	inline void setSplash(int i) {m_iSplash = i;}

	// returns the current view
	CSettlersView *getView(void);

	//creates the GL window
	CFrameWnd *createView();
	void showView(CFrameWnd *pFrame);

	//get module directory
	CString getDir(void) {return m_strDirectory;}

	//AI
	int QueryAction(int iState, int iPlayer, CGame *pGame, CBoardPieceArray *pBoard, int *ID);
	void ShutdownAI(void);

	//sounds
	void playSound(CString strSound);

	//tour
	void startTour(void);
	void stopTour(void);

	//texture reading function
	void readIL(int iSize, CString strFile, ILuint *pIL);

	//image lists
	CImageList *getImageConnector(void) {return m_pImageConnector;}

	//network connector
	CNetworkConnector *getConnector(void) {return m_pConnector;}

	//returns a string of the IP address
	CString getIPAddress(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettlersApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSettlersApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnDatabaseGame();
	afx_msg void OnDatabasePlayer();
	afx_msg void OnDatabaseMaps();
	afx_msg void OnFileOpen();
	afx_msg void OnOptionsGeneral();
	afx_msg void OnTourQuit();
	afx_msg void OnFileTour();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//init functions
	BOOL checkTime(void);
	void initAI(void);
	void initSound(void);
	void initHelp(void);
	void initDatabases(void);
	void initDirectory(void);
	BOOL initCommandLine(void);
	BOOL initLibraries(void);

	//image lists
	void buildImageLists(void);
	void clearImageLists(void);

	//the game mode selected
	int m_iMode;

	//are we in a tour?
	BOOL m_bTour;

	//new game handlers
	void runHotseat(void);
	void runNetwork(void);

	//reload a network game
	BOOL reloadNetwork(void);

	//the view
	CSettlersView *m_pView;

	//whether the sound library is initialized
	BOOL m_bSoundInit;

	//our directory
	CString m_strDirectory;

	//help?
	BOOL m_bHelp;

	//splash screen value
	int m_iSplash;

	//AI DLL
	HINSTANCE m_hAI;

	//AI query functions
	LPNQUERYACTION m_lpnQuery;

	//AI shutdown function
	LPNSHUTDOWN m_lpnShutdown;

	//BASS DLL
	HINSTANCE m_hBass;

	//init functions
	LPNBASS m_lpnBassStart;
	LPNBASS m_lpnBassFree;
	LPNBASS m_lpnBassStop;

	//play functions
	LPNBASSINIT m_lpnBassInit;
	LPNBASSSAMPLELOAD m_lpnBassSampleLoad;
	LPNBASSSAMPLEPLAY m_lpnBassSamplePlay;

	//balloon help for tour
	CBalloonHelp m_balloon;

	//image lists
	CImageList *m_pImageConnector;

	//the network connector
	CNetworkConnector *m_pConnector;
public:
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTLERS_H__1EC819D4_7065_4715_B76E_2D621955E765__INCLUDED_)
