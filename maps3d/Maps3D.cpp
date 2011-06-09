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
#include "Maps3D.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "Maps3DDoc.h"
#include "Maps3DView.h"
#include "MapDatabase.h"
#include <direct.h>
#include "defineDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaps3DApp

BEGIN_MESSAGE_MAP(CMaps3DApp, CWinApp)
	//{{AFX_MSG_MAP(CMaps3DApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaps3DApp construction

CMaps3DApp::CMaps3DApp()
{
	m_pView = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMaps3DApp object

CMaps3DApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMaps3DApp initialization
/////////////////////////////////////////////////////////////////////////////
BOOL CMaps3DApp::InitInstance()
{
	int nSplash = SPLASH_NEW;

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Maps3D"));

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	//initialization
	initDirectory();
	initDatabases();
	
	//command line
	if(FALSE == initCommandLine())
	{
		return FALSE;
	}

	//initialize image library
	ilInit();

	//RUN THE SPLASH SCREEN HERE

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_MAPS3DTYPE,
		RUNTIME_CLASS(CMaps3DDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMaps3DView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	// The main window has been initialized, so show and update it.
	m_pMainWnd = pMainFrame;
	pMainFrame->ShowWindow(SW_HIDE | SW_SHOWMINNOACTIVE);
	pMainFrame->UpdateWindow();

	//handle the splash screen
	switch(nSplash)
	{
	case SPLASH_NEW: runNew(); break;
	case SPLASH_OPEN: runOpen(); break;
	}

	//show the window
	m_pMainWnd->ShowWindow(SW_RESTORE);
	m_pMainWnd->SetForegroundWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMaps3DApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMaps3DApp message handlers

/////////////////////////////////////////////////////////////////////////////
// shutdown
/////////////////////////////////////////////////////////////////////////////
int CMaps3DApp::ExitInstance() 
{
	//close the databases down
	MAPS->release();

	//clean up IL
	ilShutDown();
	
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// init current directory
/////////////////////////////////////////////////////////////////////////////
void CMaps3DApp::initDirectory(void)
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
// init databases
/////////////////////////////////////////////////////////////////////////////
void CMaps3DApp::initDatabases(void)
{
	MAPS;
}

/////////////////////////////////////////////////////////////////////////////
// process command line
/////////////////////////////////////////////////////////////////////////////
BOOL CMaps3DApp::initCommandLine()
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
// actually creates the new window
/////////////////////////////////////////////////////////////////////////////
CFrameWnd *CMaps3DApp::createView()
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
// run a new map
/////////////////////////////////////////////////////////////////////////////
void CMaps3DApp::runNew()
{
	CFrameWnd *pFrame;

	//create the view
	pFrame = createView();

	if(NULL != pFrame)
	{
		pFrame->ShowWindow(SW_MAXIMIZE);
		pFrame->ShowWindow(SW_SHOW);
	}
}

/////////////////////////////////////////////////////////////////////////////
// open an existing map
/////////////////////////////////////////////////////////////////////////////
void CMaps3DApp::runOpen()
{

}