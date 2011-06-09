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
#include "Settlers.h"
#include "ColorComboBox.h"
#include "ColorDatabase.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorComboBox

CColorComboBox::CColorComboBox()
{
}

CColorComboBox::~CColorComboBox()
{
}


BEGIN_MESSAGE_MAP(CColorComboBox, CComboBox)
	//{{AFX_MSG_MAP(CColorComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorComboBox message handlers

void CColorComboBox::PreSubclassWindow() 
{
	int i;

	//create the font
	m_font.CreateFont(
				13,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				_T("Tahoma"));                 // lpszFacename

	CComboBox::PreSubclassWindow();

	//add all the color database strings here
	for(i = 0; i < COLORS->numColors(); i++)
	{
		AddString(COLORS->getColorName(i));
	}
}

void CColorComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC;
	CRect rect, colorRect;
	CFont *pFont;
	CBrush *pBrush, brush;
	CPen *pPen, pen;
	COLORREF crColor, crBack;
	CString strText;
	int iItem = lpDrawItemStruct->itemID;
	int iState = lpDrawItemStruct->itemState;
	COLORREF crNormal = COLOR_BLACK;
	COLORREF crSelected = GetSysColor( COLOR_HIGHLIGHT );
	COLORREF crText = COLOR_WHITE;
	
	//get the DC
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	//get drawing rect
	rect = lpDrawItemStruct->rcItem;

	//sanity
	if(-1 == iItem)
	{
		pDC->FillSolidRect( &rect, crNormal );
		return;
	}

	//get the text
	GetLBText(iItem, strText);

	//create the pen and brush
	pen.CreatePen(PS_SOLID, 1, COLOR_WHITE);
	brush.CreateSolidBrush(COLORS->getTextColor(iItem));

	//select them in
	pPen = pDC->SelectObject(&pen);
	pBrush = pDC->SelectObject(&brush);
	pFont = pDC->SelectObject(&m_font);

	if( iState & ODS_SELECTED )
	{
		crColor = pDC->SetTextColor(crText);
		crBack = pDC->SetBkColor( crSelected );
		pDC->FillSolidRect( &rect, crSelected );
	}
	else
	{
		crColor = pDC->SetTextColor( COLORS->getTextColor(iItem) );
		crBack = pDC->SetBkColor( crNormal );
		pDC->FillSolidRect( &rect, crNormal );
	}

	//create the color rect
	colorRect = rect;
	colorRect.right = 38;
	colorRect.DeflateRect(2, 2);
	pDC->Rectangle(colorRect);

	//offset the rect
	rect.OffsetRect(40, 0);

	//output the text
	pDC->DrawText(strText, strText.GetLength(), &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	//select back old
	pDC->SelectObject(pPen);
	pDC->SelectObject(pBrush);
	pDC->SelectObject(pFont);
	pDC->SetTextColor(crColor);
	pDC->SetBkColor(crBack);
}