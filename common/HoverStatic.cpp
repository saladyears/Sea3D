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
#include "HoverStatic.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// constructor
/////////////////////////////////////////////////////////////////////////////
CHoverStatic::CHoverStatic()
{
	//default to left justified, static, starting at the top vertically
	m_iState = HS_STATIC | HS_TOP | HS_LEFT_JUSTIFY;

	//have to figure out the text once
	m_bTextOnce = true;

	//default 
	m_bTracking = FALSE;

	//no border default
	m_bBorder = FALSE;

	// reasonably intelligent defaults
	m_colorDefault	= RGB(0,0,0);
	m_colorHover	= RGB(0,0,255);
	m_colorDisable	= RGB(128, 128, 128);
	m_colorBackground = ::GetSysColor(COLOR_3DFACE);

	//set up the default fonts
	m_fontDefault	= Arial14;
	m_fontHover		= Arial14;

	m_rectClient.SetRect(0, 0, 0, 0);

	//default borders
	m_iLeftBorder = 1;
	m_iTopBorder = 1;

	// dde added these to simplify handling the cursor
	m_hCursor = AfxGetApp()->LoadCursor(IDC_HAND_CURSOR);
}

BEGIN_MESSAGE_MAP(CHoverStatic, CButton)
	//{{AFX_MSG_MAP(CHoverStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// font setup
/////////////////////////////////////////////////////////////////////////////
void CHoverStatic::SetFont(LOGFONT lf)
{
	// copy default logfont
	m_fontHover = lf;
	m_fontDefault = lf;
	m_bTextOnce = true;
	if(m_hWnd) Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// hover font setup
/////////////////////////////////////////////////////////////////////////////
void CHoverStatic::SetHoverFont(LOGFONT lf)
{
	m_fontHover = lf;
	m_bTextOnce = true;
	if(m_hWnd) Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// handles mouse movement
/////////////////////////////////////////////////////////////////////////////
void CHoverStatic::OnMouseMove(UINT nFlags, CPoint point) 
{	
	CButton::OnMouseMove(nFlags, point);

	//if this is a static button, don't worry about mouse movement
	if((FALSE == (m_iState & HS_STATIC)) && (FALSE == m_bTracking))
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle hovering
/////////////////////////////////////////////////////////////////////////////
LRESULT CHoverStatic::OnMouseHover(WPARAM /*wparam*/, LPARAM /*lparam*/) 
{
	m_iState |= HS_MOUSE_OVER;
	Invalidate();
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// handle leaving
/////////////////////////////////////////////////////////////////////////////
LRESULT CHoverStatic::OnMouseLeave(WPARAM /*wparam*/, LPARAM /*lparam*/)
{
	m_bTracking = FALSE;
	m_iState &= ~HS_MOUSE_OVER;
	Invalidate();
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// set the cursor properly
/////////////////////////////////////////////////////////////////////////////
BOOL CHoverStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(!(m_iState & HS_STATIC)){
		::SetCursor(m_hCursor);
		return TRUE;
	}
	else
		return CButton::OnSetCursor(pWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
// handles drawing the item
/////////////////////////////////////////////////////////////////////////////
void CHoverStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	int i;
	int iHeight, iWidth;
	BOOL bEnabled;
	CDC dc;
	CSize size;
	CFont fontNew, *fontOld = NULL;
	COLORREF colorOldFG, colorOldBK;
	UINT uiOldAlign;
	vector <CString> *pLines;
	CRect rect;

	//attach the CDC
	dc.Attach(lpDrawItemStruct->hDC);

	//get the client rect
	GetClientRect(&m_rectClient);

	//copy to temp rect
	rect = m_rectClient;

	if(TRUE == m_bBorder)
	{
		dc.FillSolidRect(rect, m_colorBorder);
		rect.DeflateRect(1, 1, 1, 1);
	}

	// erase the background
	dc.FillSolidRect(rect, m_colorBackground);

	// get window enabled state
	bEnabled = IsWindowEnabled();
	
	//figure out the text once
	if(m_bTextOnce)
	{
		CreateWrappedText();
		CreateWrappedText(TRUE);
		m_bTextOnce = false;
	}

	//get old stuff
	colorOldFG		= dc.GetTextColor();
	colorOldBK		= dc.GetBkColor();
	uiOldAlign		= dc.GetTextAlign();

	// setup the DC how we want it, depending on whether hovering or not
	if(m_iState & HS_MOUSE_OVER)
	{
		fontNew.CreateFontIndirect((LOGFONT *) &m_fontHover);
		dc.SetTextColor(bEnabled ? m_colorHover : m_colorDisable);
		pLines = &m_strLinesH;
	}
	else
	{
		fontNew.CreateFontIndirect((LOGFONT *) &m_fontDefault);
		dc.SetTextColor(bEnabled ? m_colorDefault : m_colorDisable);
		pLines = &m_strLinesD;
	}

	//set the new font
	fontOld	= dc.SelectObject(&fontNew);

	//set new settings
	dc.SetBkColor(m_colorBackground);
	dc.SetTextAlign(TA_LEFT | TA_TOP);

	//now output text
	for(i = 0, iHeight = 0; i < (int) pLines->size(); i++)
	{
		//get the text extent
		size = dc.GetTextExtent((*pLines)[i]);
		iWidth = size.cx;

		//output dependent on flags
		if(m_iState & HS_LEFT_JUSTIFY)
		{
			dc.TextOut(m_iLeftBorder, m_iTopBorder + iHeight, (*pLines)[i]);
		}
		else if(m_iState & HS_CENTER)
		{
			dc.TextOut((m_rectClient.Width() - iWidth - m_iLeftBorder) >> 1, m_iTopBorder + iHeight, (*pLines)[i]);
		}
		else if(m_iState & HS_RIGHT_JUSTIFY)
		{
			dc.TextOut((m_rectClient.Width() - iWidth - 1), m_iTopBorder + iHeight, (*pLines)[i]);
		}

		iHeight += size.cy;
	}

	// general cleanup
	dc.SelectObject(fontOld);
	dc.SetTextColor(colorOldFG);
	dc.SetBkColor(colorOldBK);
	dc.SetTextAlign(uiOldAlign);

	//clean up objects
	fontNew.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// create wrapped text out of what we're given
/////////////////////////////////////////////////////////////////////////////
void CHoverStatic::CreateWrappedText(BOOL bHover) // = FALSE
{
	int i, j, iPlace;
	int iNumLines = 1;
	int iLineCount;
	int iSpaceCount;
	CDC *dc;
	CSize size;
	CRect rect;
	CString str, strLine, strTemp, strFinal;
	vector <int> iSpaces;
	vector <CString> strLines;
	vector <CString> *pLines;

	//check to see if we don't care
	if(m_iState & HS_DONT_CARE) return;

	//create some font stuff
	CFont *old = NULL;
	CFont *font = new CFont();

	//assign the str
	str = bHover ? m_strHover : m_strDefault;

	//add one new line to the end
	str += "\n";

	//run through and caluculate how many newline characters are already in the string
	for(i = 0, iLineCount = 0; i < str.GetLength(); i++){
		if(str[i] == '\n') {iSpaces.push_back(i); iLineCount++;}
	}

	//resize the lines array
	strLines.resize(iLineCount, "");

	//fill the lines array
	for(i = 0; i < iLineCount; i++){
		if(i)
			strLines[i] = str.Mid(iSpaces[i - 1] + 1, iSpaces[i] - iSpaces[i - 1] - 1);
		else
			strLines[i] = str.Left(iSpaces[i]);
	}

	//get the client specs
	GetClientRect(&rect);

	//set up the device context
	dc = GetDC();

	//if we get a NULL, do nothing
	if(!dc) return;

	//create the font
	font->CreateFontIndirect(bHover ? (LOGFONT *) &m_fontHover : (LOGFONT *) &m_fontDefault);

	//set the font
	old = dc->SelectObject(font);

	//run through each line and figure it out
	for(j = 0; j < iLineCount; j++){
		
		//assign the new text
		strLine = strLines[j];

		//add a one extra space to the end for the algo to work
		strLines[j] += " ";

		//get the text extent
		size = dc->GetTextExtent(strLines[j]);

		//if we're not shorter than the width, we've got trouble
		if(size.cx >= rect.Width() - m_iLeftBorder - 1){

			//start with the leftmost character
			iPlace = 0;

			//reset the space array
			iSpaces.clear();

			//count and store the number of spaces
			for(i = 0, iSpaceCount = 0; i < strLines[j].GetLength(); i++){
				if(strLines[j][i] == ' ') {iSpaces.push_back(i); iSpaceCount++;}
			}

			//if there's zero spaces, then do nothing (currently)
			if(iSpaceCount == 1){
			}
			//if there's one space, make it a newline and hope for the best (currently)
			else if(iSpaceCount == 2){
				strLine.SetAt(iSpaces[0], '\n');
			}
			//otherwise, scan through and keep adding newlines until we're good
			else{
				while(1){
					//start with the last space and work backwards
					for(i = iSpaceCount - 1; iSpaces[i] > iPlace; i--){
						//create the temporary string
						strTemp = strLines[j].Mid(iPlace, iSpaces[i] - iPlace);

						//get the text extent
						size = dc->GetTextExtent(strTemp);

						//if we fit, use that spot
						if(size.cx < rect.Width() - m_iLeftBorder - 1){
							if(i != iSpaceCount - 1)
								strLine.SetAt(iSpaces[i], '\n');
							iPlace = iSpaces[i];
							break;
						}

						//check to see if we need to stop
						if(i == 0) break;
					}

					//break if we're at the end, or if we can't get to the end
					if(iPlace == iSpaces[iSpaceCount - 1]) break;
					if(i == iSpaceCount - 2) break;
				}
			}
		}

		//add the line in
		if(j)
			strFinal += ("\n" + strLine);
		else
			strFinal += strLine;
	}

	//restore the context
	dc->SelectObject(old);

	//return the context
	ReleaseDC(dc);

	//clean up
	if(font) {delete font; font = NULL;}

	//set up which array gets these lines
	pLines = bHover ? &m_strLinesH : &m_strLinesD;
	
	//reset the array
	pLines->erase(pLines->begin(), pLines->end());
	
	//count how many newline characters there are in the text
	for(i = 0; i < strFinal.GetLength(); i++)
	{
		if(strFinal[i] == '\n') iNumLines++;
	}

	//resize the CString array and fill if needed
	if(iNumLines >= 1){
		for(i = 0, j = 0; i < strFinal.GetLength() + 1; i++){
			if(i == strFinal.GetLength() || strFinal[i] == '\n'){
				pLines->push_back(strFinal.Mid(j, i - j));
				j = i + 1;
			}
		}
	}

	if(bHover) m_strHover = strFinal;
	else m_strDefault = strFinal;
}

/////////////////////////////////////////////////////////////////////////////
// clear the hover flag
/////////////////////////////////////////////////////////////////////////////
void CHoverStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//reset the state
	m_iState &= ~HS_MOUSE_OVER;

	//redraw
	Invalidate();
	
	CButton::OnLButtonUp(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// Gets the minimum size required for the current text and font
/////////////////////////////////////////////////////////////////////////////
CSize CHoverStatic::GetMinimumSize()
{
	int i, j;
	CDC *dc;
	CSize size;
	CFont *old = NULL;
	CFont *font = new CFont();
	vector <CString> strLinesD;
	vector <CString> strLinesH;
	
	//set up the device context
	dc = GetDC();

	//if we get a NULL
	if(!dc) return CSize(0,0);

	//create the font
	font->CreateFontIndirect((LOGFONT *) &m_fontDefault);

	//set the font
	old = dc->SelectObject(font);
	
	//***********************************************************************
	//this assumes that the desired text has already been formatted correctly
	//***********************************************************************
	//create the default lines
	for(i = 0, j = 0; i < m_strDefault.GetLength() + 1; i++){
		if(i == m_strDefault.GetLength() || m_strDefault[i] == '\n'){
			strLinesD.push_back(m_strDefault.Mid(j, i - j));
			j = i + 1;
		}
	}

	//create the hover lines
	for(i = 0, j = 0; i < m_strHover.GetLength() + 1; i++){
		if(i == m_strHover.GetLength() || m_strHover[i] == '\n'){
			strLinesH.push_back(m_strHover.Mid(j, i - j));
			j = i + 1;
		}
	}

	//run through each line and figure it out
	CSize maxSizeDefault(0, 0);
	for(i = 0; i < (int) strLinesD.size(); i++){
		//get the text extent
		size = dc->GetTextExtent(strLinesD[i]);
		if (size.cx > maxSizeDefault.cx) maxSizeDefault.cx = size.cx;
		maxSizeDefault.cy += size.cy;
	}

	CSize maxSizeHover(0, 0);
	for(i = 0; i < (int) strLinesH.size(); i++){
		//get the text extent
		size = dc->GetTextExtent(strLinesH[i]);
		if (size.cx > maxSizeHover.cx) maxSizeHover.cx = size.cx;
		maxSizeHover.cy += size.cy;
	}

	//restore the context
	dc->SelectObject(old);

	//return the context
	ReleaseDC(dc);

	//clean up
	if(font) {delete font; font = NULL;}

	return CSize(max(maxSizeDefault.cx, maxSizeHover.cx) + 6, max(maxSizeDefault.cy, maxSizeHover.cy));
}

/////////////////////////////////////////////////////////////////////////////
// make sure this is set to owner draw
/////////////////////////////////////////////////////////////////////////////
BOOL CHoverStatic::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= BS_OWNERDRAW;
	
	return CButton::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// avoid flicker
/////////////////////////////////////////////////////////////////////////////
BOOL CHoverStatic::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
