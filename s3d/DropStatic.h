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

#if !defined(AFX_DROPSTATIC_H__3892F2AF_E005_4148_8403_6D86E3F851C1__INCLUDED_)
#define AFX_DROPSTATIC_H__3892F2AF_E005_4148_8403_6D86E3F851C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "HoverStatic.h"
#include "TBTextTarget.h"
#include "define.h"

//////////////////////////////////////////////////////////////////////
// class declarations
//////////////////////////////////////////////////////////////////////
class CDropListCtrl;

//////////////////////////////////////////////////////////////////////
// hoverstatic which includes drag/drop
//////////////////////////////////////////////////////////////////////
class CDropStatic : public CHoverStatic, public TBTextTarget  
{
public:
	CDropStatic();
	virtual ~CDropStatic();

	//data processing
	void ProcessData(CString strData);

	//list control
	inline void setListCtrl(CDropListCtrl *p) {m_pList = p;}

	//id retrieval
	inline UINT getIndex(void) {return m_iIndex;}

	//drag settings
	inline void resetDrag(void) {m_bDrag = FALSE;}

	//drag/drop handlers
	HRESULT dragOver(void);
	HRESULT dragLeave(void);
	HRESULT dragEnter(void);
	void drop(void);

	//dropped elsewhere
	void dropItem(void);

public: // MFC - Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDropStatic)
	//}}AFX_VIRTUAL

protected: 

	//drag and drop
	BOOL m_bDrag;

	//player index
	int m_iIndex;

	//list control this is tied to
	CDropListCtrl *m_pList;
	
	// MFC - Generated message map functions
	//{{AFX_MSG(CDropStatic)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DROPSTATIC_H__3892F2AF_E005_4148_8403_6D86E3F851C1__INCLUDED_)
