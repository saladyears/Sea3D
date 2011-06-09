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

#if !defined(AFX_HOVERSTATIC_H__FBE47DC6_932E_11D2_B505_00C0F017B88C__INCLUDED_)
#define AFX_HOVERSTATIC_H__FBE47DC6_932E_11D2_B505_00C0F017B88C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XLogFont.h"
#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// defines
/////////////////////////////////////////////////////////////////////////////
#define HS_STATIC			(1 << 0)
#define HS_LEFT_JUSTIFY		(1 << 1)
#define HS_CENTER			(1 << 2)
#define HS_RIGHT_JUSTIFY	(1 << 3)
#define HS_TOP				(1 << 4)
#define HS_V_CENTER			(1 << 5)
#define HS_BOTTOM			(1 << 6)
#define HS_MOUSE_OVER		(1 << 7)
#define HS_DONT_CARE		(1 << 8)

/////////////////////////////////////////////////////////////////////////////
// class which makes explorer-like text
/////////////////////////////////////////////////////////////////////////////
class CHoverStatic : public CButton
{
public:
	/////////////////////////////////////////////////////////////////////////
	// constructor
	/////////////////////////////////////////////////////////////////////////
	CHoverStatic();

	/////////////////////////////////////////////////////////////////////////
	// inline get functions
	/////////////////////////////////////////////////////////////////////////
	int inline GetState(void) {return m_iState;}
	void inline GetWindowText(CString &text) {text = m_strDefault;}
	CString inline GetText(void) {return m_strDefault;}
	LOGFONT inline GetFont(void) {return (LOGFONT) m_fontDefault;}
	COLORREF inline GetColor(void) {return m_colorDefault;}
	COLORREF inline GetBackground(void) {return m_colorBackground;}

	/////////////////////////////////////////////////////////////////////////
	// inline set functions
	/////////////////////////////////////////////////////////////////////////
	void inline SetState(int i) {m_iState |= i; Invalidate();}
	void inline SetText(CString sz) {m_strDefault = m_strHover = sz; m_bTextOnce = true; if(GetSafeHwnd()) Invalidate();}
	void inline SetHoverText(CString sz) {m_strHover = sz; m_bTextOnce = true; if(GetSafeHwnd()) Invalidate();}
	void inline SetColors(COLORREF c1, COLORREF c2) {m_colorDefault = c1; m_colorHover = c2;}
	void inline Reset(void) {m_iState &= ~HS_MOUSE_OVER; Invalidate();}
	void inline SetNoCare(void) {m_iState |= HS_DONT_CARE;}
	void inline SetBackground(COLORREF c) {m_colorBackground = c;}
	void inline TurnOnBorder(COLORREF c = RGB(0, 0, 0)) {m_colorBorder = c; m_bBorder = TRUE;}
	void inline TurnOffBorder(void) {m_bBorder = FALSE;}

	/////////////////////////////////////////////////////////////////////////
	// inline formatting functions
	/////////////////////////////////////////////////////////////////////////
	void inline SetClick(void) {m_iState &= ~HS_STATIC;}
	void inline SetCenter(void) {m_iState &= ~HS_LEFT_JUSTIFY; m_iState |= HS_CENTER;}
	void inline SetRight(void) {m_iState &= ~HS_LEFT_JUSTIFY; m_iState |= HS_RIGHT_JUSTIFY;}
	void inline SetCenterClick(void) {SetClick(); SetCenter();}
	void inline SetLeftBorder(int i) {m_iLeftBorder = i;};
	void inline SetTopBorder(int i) {m_iTopBorder = i;};

	/////////////////////////////////////////////////////////////////////////
	// set functions
	/////////////////////////////////////////////////////////////////////////
	void SetFont(LOGFONT lf);
	void SetHoverFont(LOGFONT lf);

	/////////////////////////////////////////////////////////////////////////
	// get functions
	/////////////////////////////////////////////////////////////////////////
	CSize GetMinimumSize();
protected:
	/////////////////////////////////////////////////////////////////////////
	// creates the actual text
	/////////////////////////////////////////////////////////////////////////
	void CreateWrappedText(BOOL bHover = FALSE);

	/////////////////////////////////////////////////////////////////////////
	// font variables
	/////////////////////////////////////////////////////////////////////////
	XLOGFONT		m_fontDefault;
	XLOGFONT		m_fontHover;

	/////////////////////////////////////////////////////////////////////////
	// text variables
	/////////////////////////////////////////////////////////////////////////
	CString		m_strDefault;
	CString		m_strHover;
	vector <CString> m_strLinesD;
	vector <CString> m_strLinesH;

	/////////////////////////////////////////////////////////////////////////
	// color variables
	/////////////////////////////////////////////////////////////////////////
	COLORREF	m_colorDefault;
	COLORREF	m_colorHover;
	COLORREF	m_colorDisable;
	COLORREF	m_colorBackground;
	COLORREF	m_colorBorder;

	/////////////////////////////////////////////////////////////////////////
	// text borders
	/////////////////////////////////////////////////////////////////////////
	int m_iLeftBorder;
	int m_iTopBorder;

	/////////////////////////////////////////////////////////////////////////
	// color border
	/////////////////////////////////////////////////////////////////////////
	BOOL m_bBorder;

	/////////////////////////////////////////////////////////////////////////
	// state variable
	/////////////////////////////////////////////////////////////////////////
	int m_iState;

	/////////////////////////////////////////////////////////////////////////
	// other variables
	/////////////////////////////////////////////////////////////////////////
	bool		m_bTextOnce;
	BOOL		m_bTracking;
	CRect		m_rectClient;
	HCURSOR		m_hCursor;

public: // MFC - Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoverStatic)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected: // MFC - Generated message map functions
	//{{AFX_MSG(CHoverStatic)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOVERSTATIC_H__FBE47DC6_932E_11D2_B505_00C0F017B88C__INCLUDED_)
