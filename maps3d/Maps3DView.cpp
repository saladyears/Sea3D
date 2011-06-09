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
#include "Maps3D.h"

#include "Maps3DDoc.h"
#include "Maps3DView.h"
#include <il\ilut.h>
#include "define.h"
#include "defineTab.h"
#include "defineMaps3D.h"
#include "defineFile.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaps3DView

IMPLEMENT_DYNCREATE(CMaps3DView, CGLMapView)

BEGIN_MESSAGE_MAP(CMaps3DView, CGLMapView)
	//{{AFX_MSG_MAP(CMaps3DView)
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CGLMapView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CGLMapView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CGLMapView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaps3DView construction/destruction

/////////////////////////////////////////////////////////////////////////////
// constructor
/////////////////////////////////////////////////////////////////////////////
CMaps3DView::CMaps3DView()
{
	m_pControl = NULL;

	//image lists
	m_pImageLand16 = NULL;
	m_pImageSea16 = NULL;
	m_pImageSpecial16 = NULL;
	m_pImageChits16 = NULL;

	m_pImageLand32 = NULL;
	m_pImageSea32 = NULL;
	m_pImageSpecial32 = NULL;
	m_pImageChits32 = NULL;

	m_pImageLand64 = NULL;
	m_pImageSea64 = NULL;
	m_pImageSpecial64 = NULL;
	m_pImageChits64 = NULL;

	//cursors
	::ZeroMemory(m_hCursors, sizeof(m_hCursors));

	//no res yet
	m_nRes = RES_BLANK;
}

/////////////////////////////////////////////////////////////////////////////
// destructor
/////////////////////////////////////////////////////////////////////////////
CMaps3DView::~CMaps3DView()
{
	//tell the app we do not exist
	APP->setView(NULL);

	//clean up image lists
	deleteImageLists(); 

	//clean up cursors
	deleteCursors();

	if(NULL != m_pControl)
	{
		delete m_pControl;
		m_pControl = NULL;
	}
}

BOOL CMaps3DView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CGLMapView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMaps3DView drawing

void CMaps3DView::OnDraw(CDC* pDC)
{	
	CGLMapView::OnDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CMaps3DView printing

BOOL CMaps3DView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMaps3DView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMaps3DView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMaps3DView diagnostics

#ifdef _DEBUG
void CMaps3DView::AssertValid() const
{
	CGLMapView::AssertValid();
}

void CMaps3DView::Dump(CDumpContext& dc) const
{
	CGLMapView::Dump(dc);
}

CMaps3DDoc* CMaps3DView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMaps3DDoc)));
	return (CMaps3DDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMaps3DView message handlers

/////////////////////////////////////////////////////////////////////////////
// initial update
/////////////////////////////////////////////////////////////////////////////
void CMaps3DView::OnInitialUpdate() 
{
	CGLMapView::OnInitialUpdate();

	//tell the app we exist
	APP->setView(this);

	//create the image lists
	createImageLists();

	//load the cursors
	loadCursors();

	//create the control panel window
	m_pControl = new CControlPanelWnd;
	m_pControl->Create(NULL, "", CHILD, CRect(0, 0, 0, 0), this, 0, NULL);
	m_pControl->ShowWindow(SW_SHOW);
}

/////////////////////////////////////////////////////////////////////////////
// set the current resource
/////////////////////////////////////////////////////////////////////////////
void CMaps3DView::setResource(int n)
{
	int nMode;

	//set the resource
	m_nRes = n;

	//set the draw mode
	if(RES_BLANK == n)
	{
		nMode = GL_DRAW_NORMAL;
	}
	else if(RES_PORT3 <= n && RES_PORTSHEEP >= n)
	{
		nMode = GL_DRAW_PORT_PAINT;
	}
	else if(RES_RANDOM <= n && RES_CHIT_2 >= n)
	{
		nMode = GL_DRAW_RES_PAINT;
	}
	else if(RES_CHIT_2 <= n)
	{
		nMode = GL_DRAW_CHIT_PAINT;
	}
	else
	{
		nMode = GL_DRAW_RES_PAINT;
	}

	//set the new drawing mode
	setDrawMode(nMode);
}

/////////////////////////////////////////////////////////////////////////////
// sizing
/////////////////////////////////////////////////////////////////////////////
void CMaps3DView::OnSize(UINT nType, int cx, int cy) 
{
	CGLMapView::OnSize(nType, cx, cy);

	if(m_pControl && m_pControl->m_hWnd)
	{
		m_pControl->SetWindowPos(&wndTop, 0, 0, cx, PANEL_HEIGHT, SWP_NOZORDER);
	}
}

/////////////////////////////////////////////////////////////////////////////
// retrieve an image list
/////////////////////////////////////////////////////////////////////////////
CImageList *CMaps3DView::imageList(int nList)
{
	CImageList *pList;

	switch(nList)
	{
	case IMAGE_LAND_16: pList = m_pImageLand16; break;
	case IMAGE_LAND_32: pList = m_pImageLand32; break;
	case IMAGE_LAND_64: pList = m_pImageLand64; break;
	case IMAGE_SEA_16: pList = m_pImageSea16; break;
	case IMAGE_SEA_32: pList = m_pImageSea32; break;
	case IMAGE_SEA_64: pList = m_pImageSea64; break;
	case IMAGE_SPECIAL_16: pList = m_pImageSpecial16; break;
	case IMAGE_SPECIAL_32: pList = m_pImageSpecial32; break;
	case IMAGE_SPECIAL_64: pList = m_pImageSpecial64; break;
	case IMAGE_CHITS_16: pList = m_pImageChits16; break;
	case IMAGE_CHITS_32: pList = m_pImageChits32; break;
	case IMAGE_CHITS_64: pList = m_pImageChits64; break;
	}

	return pList;
}

/////////////////////////////////////////////////////////////////////////////
// image list creation
/////////////////////////////////////////////////////////////////////////////
void CMaps3DView::createImageLists()
{
	//land tiles
	createImageGroup(TEX_LAND_SIZE, FILE_MAPS3D_LAND, &m_pImageLand16, &m_pImageLand32, &m_pImageLand64);

	//sea tiles
	createImageGroup(TEX_SEA_SIZE, FILE_MAPS3D_SEA, &m_pImageSea16, &m_pImageSea32, &m_pImageSea64);

	//special tiles
	createImageGroup(TEX_SPECIAL_SIZE, FILE_MAPS3D_SPEC, &m_pImageSpecial16, &m_pImageSpecial32, &m_pImageSpecial64);

	//chits
	createImageGroup(TEX_CHITS_SIZE, FILE_MAPS3D_CHITS, &m_pImageChits16, &m_pImageChits32, &m_pImageChits64);
}

/////////////////////////////////////////////////////////////////////////////
// create a certain group of image lists
/////////////////////////////////////////////////////////////////////////////
void CMaps3DView::createImageGroup(int nSize, CString strFile, CImageList **p16, CImageList **p32, CImageList **p64)
{
	int i;
	int nTotal;
	ILuint *pIL;
	CBitmap* pBitmap;
	CDC *dc;

	//get the dc
	dc = GetDC();

	//get size
	nTotal = nSize * 3;

	//read land textures
	pIL = new ILuint[nTotal];

	//read values
	readIL(nTotal, strFile, pIL);

	//create the image lists
	*p16 = new CImageList();
	*p32 = new CImageList();
	*p64 = new CImageList();

	//create
	(*p16)->Create(16, 16, ILC_COLOR24 | ILC_MASK, 0, 0);
	(*p32)->Create(32, 32, ILC_COLOR24 | ILC_MASK, 0, 0);
	(*p64)->Create(64, 64, ILC_COLOR24 | ILC_MASK, 0, 0);

	//set the render mode
	ilutRenderer(ILUT_WIN32);

	//parse
	for(i = 0; i < nTotal; i++)
	{
		//bind the image
		ilBindImage(pIL[i]);

		//create the bitmap
		pBitmap = CBitmap::FromHandle(ilutConvertToHBitmap(dc->GetSafeHdc()));

		//add to the list
		switch(i % 3)
		{
		case 0: (*p16)->Add(pBitmap, COLOR_BLACK); break;
		case 1: (*p32)->Add(pBitmap, COLOR_BLACK); break;
		case 2: (*p64)->Add(pBitmap, COLOR_BLACK); break;
		default: break;
		}
	}

	//return the context
	ReleaseDC(dc);

	//cleanup
	delete pIL;
	pIL = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// read IL data from a file
/////////////////////////////////////////////////////////////////////////////
void CMaps3DView::readIL(int iSize, CString strFile, ILuint *pIL)
{
	int i;
	BOOL bLoad;
	CFile fileIn;
	DWORD dwLength;
	BYTE *byte;
	CString strIn;
	CString strErr;
	CString strExt;
	ILenum ilType;

	//build the image array
	ilGenImages(iSize, pIL);

	//open the file
	if(FALSE == fileIn.Open(strFile, CFile::modeRead))
	{
		strErr.Format("Could not open %s.  Check to see if this file exists.", strFile);
		AfxMessageBox(strErr);
		return;
	}

	//attach the archive
	CArchive ar(&fileIn, CArchive::load);

	//run through
	for(i = 0; i < iSize; i++)
	{
		//read the file name and length
		ar >> strIn;
		ar >> dwLength;

		//check file
		strExt = strIn.Right(3);

		//prepare the buffer
		byte = new BYTE[dwLength];

		//read the data
		ar.Read(byte, dwLength);

		//bind the name
		ilBindImage(pIL[i]);

		//set the load type
		if(0 == strExt.CompareNoCase("jpg")) ilType = IL_JPG;
		else if(0 == strExt.CompareNoCase("png")) ilType = IL_PNG;
		else ilType = IL_BMP;

		//load it
		bLoad = ilLoadL(ilType, (void *) byte, dwLength);

		//see if we were successful
		if(!bLoad)
		{
			strErr.Format("Could not read file %s from the file %s.", strIn, strFile);
			AfxMessageBox(strErr);
		}

		//clear the buffer
		delete byte;
		byte = NULL;
	}

	//cleanup
	ar.Close();
	fileIn.Close();
}

/////////////////////////////////////////////////////////////////////////////
// cleanup
/////////////////////////////////////////////////////////////////////////////
void CMaps3DView::deleteImageLists()
{
	//cleanup
	if(NULL != m_pImageLand16)
	{
		delete m_pImageLand16;
		m_pImageLand16 = NULL;
	}

	if(NULL != m_pImageLand32)
	{
		delete m_pImageLand32;
		m_pImageLand32 = NULL;
	}

	if(NULL != m_pImageLand64)
	{
		delete m_pImageLand64;
		m_pImageLand64 = NULL;
	}

	if(NULL != m_pImageSea16)
	{
		delete m_pImageSea16;
		m_pImageSea16 = NULL;
	}

	if(NULL != m_pImageSea32)
	{
		delete m_pImageSea32;
		m_pImageSea32 = NULL;
	}

	if(NULL != m_pImageSea64)
	{
		delete m_pImageSea64;
		m_pImageSea64 = NULL;
	}

	if(NULL != m_pImageSpecial16)
	{
		delete m_pImageSpecial16;
		m_pImageSpecial16 = NULL;
	}

	if(NULL != m_pImageSpecial32)
	{
		delete m_pImageSpecial32;
		m_pImageSpecial32 = NULL;
	}

	if(NULL != m_pImageSpecial64)
	{
		delete m_pImageSpecial64;
		m_pImageSpecial64 = NULL;
	}

	if(NULL != m_pImageChits16)
	{
		delete m_pImageChits16;
		m_pImageChits16 = NULL;
	}

	if(NULL != m_pImageChits32)
	{
		delete m_pImageChits32;
		m_pImageChits32 = NULL;
	}

	if(NULL != m_pImageChits64)
	{
		delete m_pImageChits64;
		m_pImageChits64 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// load cursors
/////////////////////////////////////////////////////////////////////////////
void CMaps3DView::loadCursors(void)
{
	int i;

	//load
	for(i = IDC_TIMBER_CURSOR; i <= IDC_CHIT12_CURSOR; i++)
	{
		m_hCursors[i - IDC_TIMBER_CURSOR] = AfxGetApp()->LoadCursor(i);
	}

	//set the blank cursor
	m_hCursors[RES_BLANK] = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
}

/////////////////////////////////////////////////////////////////////////////
// delete cursors
/////////////////////////////////////////////////////////////////////////////
void CMaps3DView::deleteCursors(void)
{
	int i;

	//delete
	for(i = 0; i < RES_SIZE; i++)
	{
		if(NULL != m_hCursors[i])
		{
			DeleteObject(m_hCursors[i]);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// set the proper cursor
/////////////////////////////////////////////////////////////////////////////
BOOL CMaps3DView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	//set the cursor
	::SetCursor(m_hCursors[m_nRes]);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// clear any resource cursor, or else bring up the popup menu
/////////////////////////////////////////////////////////////////////////////
void CMaps3DView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//clear the resource cursor
	if(RES_BLANK != m_nRes)
	{
		setResource(RES_BLANK);
		return;
	}
	
	CGLMapView::OnRButtonDown(nFlags, point);
}
