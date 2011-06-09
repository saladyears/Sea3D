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

#if !defined(AFX_MAPS3D_H__9E281CE2_8275_4A83_B94C_7F2A20039F02__INCLUDED_)
#define AFX_MAPS3D_H__9E281CE2_8275_4A83_B94C_7F2A20039F02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// defines
/////////////////////////////////////////////////////////////////////////////
#define ILUT_USE_OPENGL
#define ILUT_USE_WIN32

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include <il\ilut.h>

/////////////////////////////////////////////////////////////////////////////
// forward class declarations
/////////////////////////////////////////////////////////////////////////////
class CMaps3DView;

/////////////////////////////////////////////////////////////////////////////
// CMaps3DApp:
/////////////////////////////////////////////////////////////////////////////
class CMaps3DApp : public CWinApp
{
public:
	CMaps3DApp();

	//view handling
	void setView(CMaps3DView *v) {m_pView = v;}
	CMaps3DView *getView(void) {return m_pView;}

	//get module directory
	CString getDir(void) {return m_strDirectory;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaps3DApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMaps3DApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	//init functions
	void initDirectory(void);
	void initDatabases(void);
	BOOL initCommandLine(void);

	//creates the GL window
	CFrameWnd *createView();

	//splash related functions
	void runNew(void);
	void runOpen(void);

protected:
	//our directory
	CString m_strDirectory;

	//the view
	CMaps3DView *m_pView;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPS3D_H__9E281CE2_8275_4A83_B94C_7F2A20039F02__INCLUDED_)
