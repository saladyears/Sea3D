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

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CAnimateButton
/////////////////////////////////////////////////////////////////////////////
class CAnimateButton : public CWnd
{
	DECLARE_DYNAMIC(CAnimateButton)

public:
	CAnimateButton();
	virtual ~CAnimateButton();

	//timer handling
	void OnTimer(void);
	void OnOneshot(void);

	//are we animating
	void setAnimate(BOOL b) {m_bAnimate = b;}
	BOOL isAnimating(void) {return m_bIsAnimating;}
	void setBuildOptions(void) {m_bShowBuildOptions = TRUE;}

	//offest
	int getOffset(void) {return m_bAnimate ? m_nOffset : (IsWindowVisible() != TRUE);}

	//autohiding
	void updateAutohide(void);

	//overrides
	virtual void handleAutohide(void);
	virtual void updateAnimate(void);

protected:
	DECLARE_MESSAGE_MAP()

	//mouse handling
	void setMouseEvents(void);
	void drawOffset(int nOffset = 0);

	//animation
	void startAnimation(void);
	void stopAnimation(void);

	//oneshot
	void startOneshot(void);

	//overrides
	virtual void showItems(BOOL bShow);
	virtual void moveOffset(int nOffset);
	virtual void startShow(BOOL bShow);

protected:
	//timer variables
	UINT m_uiTimer;
	UINT m_uiTimerOneshot;

	//are we tracking the mouse?
	BOOL m_bTracking;

	//are we mouseable with this button?
	BOOL m_bMouseable;

	//auto-hide offsets and variables
	int m_nOffset;
	int m_nOffsetDraw;
	int m_nOffsetAmount;
	int m_nDX;
	BOOL m_bAutohide;
	BOOL m_bAnimate;

	//are we animating right now?
	BOOL m_bIsAnimating;

	//show build options when done?
	BOOL m_bShowBuildOptions;

	//animate speeds
	int m_nSpeed1;
	int m_nSpeed2;
	int m_nSpeed3;
	int m_nSpeed4;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	BOOL ShowWindow(int nCmdShow);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


