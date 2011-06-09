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

/////////////////////////////////////////////////////////////////////////////
// GridCellBase.h : header file
//
// MFC Grid Control - Grid cell base class header file
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2001. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.22+
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCELLBASE_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_GRIDCELLBASE_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CGridCtrl;

// Cell data mask
#define GVIF_TEXT               LVIF_TEXT
#define GVIF_IMAGE              LVIF_IMAGE
#define GVIF_PARAM              LVIF_PARAM
#define GVIF_STATE              LVIF_STATE
#define GVIF_BKCLR              (GVIF_STATE<<1)
#define GVIF_FGCLR              (GVIF_STATE<<2)
#define GVIF_FORMAT             (GVIF_STATE<<3)
#define GVIF_FONT               (GVIF_STATE<<4)
#define GVIF_MARGIN             (GVIF_STATE<<5)
#define GVIF_ALL                (GVIF_TEXT|GVIF_IMAGE|GVIF_PARAM|GVIF_STATE|GVIF_BKCLR|GVIF_FGCLR| \
                                 GVIF_FORMAT|GVIF_FONT|GVIF_MARGIN)

// Used for Get/SetItem calls.
typedef struct _GV_ITEM {
    int      row,col;     // Row and Column of item
    UINT     mask;        // Mask for use in getting/setting cell data
    DWORD    nFormat;     // Format of cell
    int      iImage;      // index of the list view item’s icon
    COLORREF crBkClr;     // Background colour (or CLR_DEFAULT)
    COLORREF crFgClr;     // Forground colour (or CLR_DEFAULT)
    LPARAM   lParam;      // 32-bit value to associate with item
    LOGFONT  lfFont;      // Cell font
    UINT     nMargin;     // Internal cell margin
    CString  strText;     // Text in cell
} GV_ITEM;


// Each cell contains one of these. Fields "row" and "column" are not stored since we
// will usually have acces to them in other ways, and they are an extra 8 bytes per
// cell that is probably unnecessary.

class CGridCellBase : public CObject
{
    friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCellBase)

// Construction/Destruction
public:
    CGridCellBase();
    virtual ~CGridCellBase();

// Attributes
public:
    // can't do pure virtual because of DECLARE_DYNCREATE requirement
    //  use ASSERT() to require that programmer overrides all that should
    //  be pure virtuals

    virtual void SetText(LPCTSTR /* szText */)              { ASSERT( FALSE);    }
    virtual void SetImage(int /* nImage */)                 { ASSERT( FALSE);    }
    virtual void SetData(LPARAM /* lParam */)               { ASSERT( FALSE);    }
    virtual void SetFormat(DWORD /* nFormat */)             { ASSERT( FALSE);    }
    virtual void SetTextClr(COLORREF /* clr */)             { ASSERT( FALSE);    }
    virtual void SetBackClr(COLORREF /* clr */)             { ASSERT( FALSE);    }
    virtual void SetFont(const LOGFONT* /* plf */)          { ASSERT( FALSE);    }
    virtual void SetMargin( UINT /* nMargin */)             { ASSERT( FALSE);    }
    virtual void SetGrid(CGridCtrl* /* pGrid */)            { ASSERT( FALSE);    }
    virtual void SetCoords( int /* nRow */, int /* nCol */) { ASSERT( FALSE);    }
	virtual void SetCovered(BOOL /* b */)					{ ASSERT( FALSE);	 }
	virtual void SetGridLines(int /* i */)					{ ASSERT( FALSE);    }

    virtual LPCTSTR  GetText() const                        { ASSERT( FALSE); return NULL;  }
    virtual LPCTSTR  GetTipText() const                     { return GetText();             } // may override TitleTip return
    virtual int      GetImage() const                       { ASSERT( FALSE); return -1;    }
    virtual LPARAM   GetData() const                        { ASSERT( FALSE); return 0;     }
    virtual DWORD    GetFormat() const                      { ASSERT( FALSE); return 0;     }
    virtual COLORREF GetTextClr() const                     { ASSERT( FALSE); return 0;     }
    virtual COLORREF GetBackClr() const                     { ASSERT( FALSE); return 0;     }
    virtual LOGFONT* GetFont() const                        { ASSERT( FALSE); return NULL;  }
    virtual CFont*   GetFontObject() const                  { ASSERT( FALSE); return NULL;  }
    virtual CGridCtrl* GetGrid() const                      { ASSERT( FALSE); return NULL;  }
    virtual CWnd*    GetEditWnd() const                     { ASSERT( FALSE); return NULL;  }
    virtual UINT     GetMargin() const                      { ASSERT( FALSE); return 0;     }
	virtual BOOL	 GetCovered() const						{ ASSERT( FALSE); return FALSE; }
	virtual int		 GetGridLines() const					{ ASSERT( FALSE); return 0;     }

    virtual CGridCellBase* GetDefaultCell() const;

    virtual BOOL IsDefaultFont() const                      { ASSERT( FALSE); return FALSE; }
    
// Operators
public:
    virtual void operator=(const CGridCellBase& cell);

// Operations
public:
    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
    virtual BOOL GetTextRect( LPRECT pRect);    // i/o:  i=dims of cell rect; o=dims of text rect
    virtual BOOL GetTipTextRect( LPRECT pRect) { return GetTextRect( pRect); }  // may override for btns, etc.
    virtual CSize GetTextExtent(LPCTSTR str, CDC* pDC = NULL);
    virtual CSize GetCellExtent(CDC* pDC);
protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCELLBASE_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
