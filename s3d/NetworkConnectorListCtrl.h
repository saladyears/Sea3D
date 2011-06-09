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

#if !defined(AFX_NETWORKCONNECTORLISTCTRL_H__5D984253_3DEB_40ED_AF8E_034379BF3A81__INCLUDED_)
#define AFX_NETWORKCONNECTORLISTCTRL_H__5D984253_3DEB_40ED_AF8E_034379BF3A81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkConnectorListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "SortListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CNetworkConnectorListCtrl window
/////////////////////////////////////////////////////////////////////////////
class CNetworkConnectorListCtrl : public CSortListCtrl
{
// Construction
public:
	CNetworkConnectorListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkConnectorListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNetworkConnectorListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNetworkConnectorListCtrl)
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKCONNECTORLISTCTRL_H__5D984253_3DEB_40ED_AF8E_034379BF3A81__INCLUDED_)
