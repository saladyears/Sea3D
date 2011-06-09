// TexCreate.h : main header file for the TEXCREATE application
//

#if !defined(AFX_TEXCREATE_H__926FBC2A_3680_49FF_BF60_CE9FBB080FFF__INCLUDED_)
#define AFX_TEXCREATE_H__926FBC2A_3680_49FF_BF60_CE9FBB080FFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTexCreateApp:
// See TexCreate.cpp for the implementation of this class
//

class CTexCreateApp : public CWinApp
{
public:
	CTexCreateApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTexCreateApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTexCreateApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXCREATE_H__926FBC2A_3680_49FF_BF60_CE9FBB080FFF__INCLUDED_)
