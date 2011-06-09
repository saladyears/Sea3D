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

#if !defined(AFX_TILESTABCTRL_H__ED631DC9_D373_4A5E_AC71_D8414903F22E__INCLUDED_)
#define AFX_TILESTABCTRL_H__ED631DC9_D373_4A5E_AC71_D8414903F22E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TilesTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "CustomTabCtrl.h"
#include "CustomListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTilesTabCtrl window
/////////////////////////////////////////////////////////////////////////////
class CTilesTabCtrl : public CCustomTabCtrl
{
// Construction
public:
	CTilesTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTilesTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTilesTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTilesTabCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

//Implementation
protected:
	//creation
	void createListCtrl(int nSize, int nOffset, CCustomListCtrl **pList, CImageList *pImage, CRect &rect, int nSpacing);

	//cleanup
	void deleteListCtrls(void);

//Data members
protected:
	CCustomListCtrl *m_pListLand;
	CCustomListCtrl *m_pListSea;
	CCustomListCtrl *m_pListSpecial;
	CCustomListCtrl *m_pListChits;

	//the currently selected control
	CCustomListCtrl *m_pCurrent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TILESTABCTRL_H__ED631DC9_D373_4A5E_AC71_D8414903F22E__INCLUDED_)
