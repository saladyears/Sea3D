/* 
 * Settlers3D - Copyright (C) 2001-2003 Jason Fugate (jfugate@settlers3d.net) 
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the Free 
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version. 
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 * for more details. 
 */ 

#include "stdafx.h"
#ifdef MAPS3D
#include "maps3d.h"
#else
#include "settlers.h"
#endif
#include "GLBaseView.h"
#include <math.h>
#include "MapDatabase.h"
#include "defineIni.h"
#include "defineFile.h"
#include "defineMap.h"
#include "defineDisplay.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// palette entries
/////////////////////////////////////////////////////////////////////////////
unsigned char threeto8[8] = {
    0, 0111>>1, 0222>>1, 0333>>1, 0444>>1, 0555>>1, 0666>>1, 0377
};

unsigned char twoto8[4] = {
    0, 0x55, 0xaa, 0xff
};

unsigned char oneto8[2] = {
    0, 255
};

static int defaultOverride[13] = {
    0, 3, 24, 27, 64, 67, 88, 173, 181, 236, 247, 164, 91
};

static PALETTEENTRY defaultPalEntry[20] = {
    { 0,   0,   0,    0 },
    { 0x80,0,   0,    0 },
    { 0,   0x80,0,    0 },
    { 0x80,0x80,0,    0 },
    { 0,   0,   0x80, 0 },
    { 0x80,0,   0x80, 0 },
    { 0,   0x80,0x80, 0 },
    { 0xC0,0xC0,0xC0, 0 },

    { 192, 220, 192,  0 },
    { 166, 202, 240,  0 },
    { 255, 251, 240,  0 },
    { 160, 160, 164,  0 },

    { 0x80,0x80,0x80, 0 },
    { 0xFF,0,   0,    0 },
    { 0,   0xFF,0,    0 },
    { 0xFF,0xFF,0,    0 },
    { 0,   0,   0xFF, 0 },
    { 0xFF,0,   0xFF, 0 },
    { 0,   0xFF,0xFF, 0 },
    { 0xFF,0xFF,0xFF, 0 }
};

/////////////////////////////////////////////////////////////////////////////
// CGLBaseView

IMPLEMENT_DYNCREATE(CGLBaseView, CView)

CGLBaseView::CGLBaseView()
{
	//reset rotation
	m_dxRot = 0.;
	m_dyRot = 0.;

	//no detail yet
	m_iDetail = 0;

	// GL cannot use palettes anyway, so convert early.
	ilEnable(IL_CONV_PAL);

	// Gets rid of dithering on some nVidia-based cards.
	ilutEnable(ILUT_OPENGL_CONV);
}

CGLBaseView::~CGLBaseView()
{
}


BEGIN_MESSAGE_MAP(CGLBaseView, CView)
	//{{AFX_MSG_MAP(CGLBaseView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLBaseView drawing

/////////////////////////////////////////////////////////////////////////////
// draw stuff
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::OnDraw(CDC* /*pDC*/) 
{
	static BOOL bBusy = FALSE;

	//fake a semaphore
	if(bBusy)
	{
		return;
	}

	//we're busy
	bBusy = TRUE;

	//make current
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

	//draw the scene
	DrawScene();

	//double-buffering
    SwapBuffers(wglGetCurrentDC());

	//make uncurrent
	wglMakeCurrent(NULL, NULL);

	//no longer busy
	bBusy = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CGLBaseView diagnostics

#ifdef _DEBUG
void CGLBaseView::AssertValid() const
{
	CView::AssertValid();
}

void CGLBaseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGLBaseView message handlers

/////////////////////////////////////////////////////////////////////////////
// handle window creation
/////////////////////////////////////////////////////////////////////////////
int CGLBaseView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//create the GL window
	createGL();

	//read graphics deatil
	m_iDetail = GetPrivateProfileInt(INI_OPENGL, INI_DETAIL, GL_HIGH_DETAIL, INI_FILE);

	//clamp
	if(m_iDetail < GL_NO_DETAIL) m_iDetail = GL_NO_DETAIL;
	if(m_iDetail > GL_HIGH_DETAIL) m_iDetail = GL_HIGH_DETAIL;

	//read mipmapping from .ini file
	m_bMip = GetPrivateProfileInt(INI_OPENGL, INI_MIPMAP, TRUE, INI_FILE);

	//check euro value
	setEuro();

	//check borders value
	setBorders();

	//check sea ports value
	setSeaTiles();

	//read gouraud
	m_bGouraud = GetPrivateProfileInt(INI_OPENGL, INI_GOURAUD, FALSE, INI_FILE);

	//read highlight
	m_bHighlight = GetPrivateProfileInt(INI_OPENGL, INI_HIGHLIGHT, TRUE, INI_FILE);

	//read white outline
	m_bWhiteOutline = GetPrivateProfileInt(INI_OPENGL, INI_WHITE, TRUE, INI_FILE);

	//build the display lists
	buildLists();

	//load standard texture maps
	buildTileTextures();

	//set rendering context off
	wglMakeCurrent(NULL, NULL);

	//set up material colors
	glGray.SetMaterial(COLOR_BLACK);
	glGray.SetEmission(COLOR_DKGRAY);
	glWhite.SetMaterial(COLOR_WHITE);
	glGold.SetMaterial(COLOR_BLACK);
	glGold.SetEmission(COLOR_GOLDOUTLINE);
	glRed.SetMaterial(COLOR_BLACK);
	glRed.SetEmission(COLOR_RED);
	glEdge.SetMaterial(COLOR_BLACK);
	glEdge.SetEmission(COLOR_EDGE);
	glBlack.SetMaterial(COLOR_BLACK);
	glHighlight.SetMaterial(COLOR_BLACK);
	glHighlight.SetEmission(GetPrivateProfileInt(INI_OPENGL, INI_HIGHLIGHT_COLOR, COLOR_OUTLINEGREEN, INI_FILE));
	glWhiteOutline.SetMaterial(COLOR_BLACK);
	glWhiteOutline.SetEmission(GetPrivateProfileInt(INI_OPENGL, INI_WHITE_COLOR, COLOR_OUTLINEPURPLE, INI_FILE));
	glLongestOutline.SetMaterial(COLOR_BLACK);
	glLongestOutline.SetEmission(GetPrivateProfileInt(INI_OPENGL, INI_LONGEST_COLOR, COLOR_GOLDOUTLINE, INI_FILE));
	glLava.SetMaterial(COLOR_BLACK);
	glLava.SetEmission(COLOR_LAVA);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// reset the euro value
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::setEuro(void)
{
	//read euro flag from .ini file
	m_bEuro = GetPrivateProfileInt(INI_OPENGL, INI_EURO, FALSE, INI_FILE);
}

/////////////////////////////////////////////////////////////////////////////
// reset the borders value
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::setBorders()
{
	m_bBorders = GetPrivateProfileInt(INI_OPENGL, INI_BORDERS, TRUE, INI_FILE);

	//validate that borders exist
	if(FALSE == checkForSeaBorders())
	{
		m_bBorders = FALSE;
	}

	//redraw the current screen
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// reset the sea tiles value
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::setSeaTiles()
{
	m_bSeaTiles = GetPrivateProfileInt(INI_OPENGL, INI_SEATILES, TRUE, INI_FILE);

	//redraw the current screen
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// set the higlight color
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::setHighlightColor(void)
{
	glHighlight.SetEmission(GetPrivateProfileInt(INI_OPENGL, INI_HIGHLIGHT_COLOR, COLOR_OUTLINEGREEN, INI_FILE));
}

/////////////////////////////////////////////////////////////////////////////
// set the white outline color
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::setWhiteOutlineColor()
{
	glWhiteOutline.SetEmission(GetPrivateProfileInt(INI_OPENGL, INI_WHITE_COLOR, COLOR_OUTLINEPURPLE, INI_FILE));
}

/////////////////////////////////////////////////////////////////////////////
// set the longest road outline color
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::setLongestOutlineColor()
{
	glLongestOutline.SetEmission(GetPrivateProfileInt(INI_OPENGL, INI_LONGEST_COLOR, COLOR_GOLDOUTLINE, INI_FILE));
}

/////////////////////////////////////////////////////////////////////////////
// create the GL window with appropriate settings
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::createGL(void)
{
    int         n;
	CRect		rect;
	PIXELFORMATDESCRIPTOR pfd;

	//create a new drawing DC
    m_pDC = new CClientDC(this);

	//set up the pixel format
    if(FALSE == setPixelFormat())
        return;

    n = ::GetPixelFormat(m_pDC->GetSafeHdc());
    ::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

    CreateRGBPalette();

    // create a rendering context and make it current
        
    m_hrc = wglCreateContext(m_pDC->GetSafeHdc());
    wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

	//get the client rectangle
	GetClientRect(&rect);

	//resize the window
	resizeGL(rect.right, rect.bottom);

	//set shading model to smooth
	glShadeModel(GL_SMOOTH);

	//adjust the perspective hint
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//set the clear color
	glClearColor(0., 0., 0., 0.);

	//set up depth testing
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//set up lighting
	GLfloat amb[] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat dif[] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat pos[] = {0.0f, 5.0f, 1.0f, 1.0f};
	GLfloat pos2[] = {0.0f, 5.0f, -1.0f, 1.0f};

	glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT1, GL_POSITION, pos);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT2, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT2, GL_POSITION, pos2);
	glEnable(GL_LIGHT2);

	glEnable(GL_LIGHTING);

	//create the object
	m_pQuad = gluNewQuadric();

	//turn on textures
	gluQuadricTexture(m_pQuad, GL_TRUE);
	gluQuadricOrientation(m_pQuad, GLU_OUTSIDE);
}

/////////////////////////////////////////////////////////////////////////////
// set up the pixel format
/////////////////////////////////////////////////////////////////////////////
BOOL CGLBaseView::setPixelFormat()
{
	int iFormat;
	DWORD dwError;
	CString strError;

	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 16-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		24,                             // 16-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};

	if ((iFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
	{
		//get the last error
		dwError = GetLastError();

		//format the message
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
				strError.GetBuffer(512), 512, NULL);
		strError.ReleaseBuffer();
 
		MessageBox(strError, "ChoosePixelFormat failed");
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), iFormat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat failed");
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// get a palette component
/////////////////////////////////////////////////////////////////////////////
unsigned char CGLBaseView::ComponentFromIndex(int i, UINT nbits, UINT shift)
{
    unsigned char val;

    val = (unsigned char) (i >> shift);

    switch (nbits)
    {
        case 1:
            val &= 0x1;
            return oneto8[val];

        case 2:
            val &= 0x3;
            return twoto8[val];

        case 3:
            val &= 0x7;
            return threeto8[val];

        default:
            return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////
// create the RGB palette
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::CreateRGBPalette()
{
    PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE  *pPal;
    WORD        n, i;

    n = (WORD) ::GetPixelFormat(m_pDC->GetSafeHdc());
    ::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

    if (pfd.dwFlags & PFD_NEED_PALETTE)
    {
        n = 1 << pfd.cColorBits;

                                               
        pPal = (PLOGPALETTE) new BYTE[sizeof(LOGPALETTE) + n * sizeof(PALETTEENTRY)];

        ASSERT(pPal != NULL);

        pPal->palVersion = 0x300;
        pPal->palNumEntries = n;

        for (i=0; i<n; i++)
        {
            pPal->palPalEntry[i].peRed =
                    ComponentFromIndex(i, pfd.cRedBits, pfd.cRedShift);
            pPal->palPalEntry[i].peGreen =
                    ComponentFromIndex(i, pfd.cGreenBits, pfd.cGreenShift);
            pPal->palPalEntry[i].peBlue =
                    ComponentFromIndex(i, pfd.cBlueBits, pfd.cBlueShift);
            pPal->palPalEntry[i].peFlags = 0;
        }

        // fix up the palette to include the default GDI palette

        if ((pfd.cColorBits == 8)                           &&
            (pfd.cRedBits   == 3) && (pfd.cRedShift   == 0) &&
            (pfd.cGreenBits == 3) && (pfd.cGreenShift == 3) &&
            (pfd.cBlueBits  == 2) && (pfd.cBlueShift  == 6)
           )
        {
			for (i = 1 ; i <= 12 ; i++)
                pPal->palPalEntry[defaultOverride[i]] = defaultPalEntry[i];
        }
        

        m_cPalette.CreatePalette(pPal);
        delete pPal;

        m_pDC->SelectPalette(&m_cPalette, FALSE);
        m_pDC->RealizePalette();
    }
}

/////////////////////////////////////////////////////////////////////////////
// set up some things
/////////////////////////////////////////////////////////////////////////////
BOOL CGLBaseView::PreCreateWindow(CREATESTRUCT& cs) 
{
	//modify the style to have the things we need
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// handle a resize event
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

	resizeGL(cx, cy);

	wglMakeCurrent(NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// prevent GL from flashing
/////////////////////////////////////////////////////////////////////////////
BOOL CGLBaseView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// resize the GL window
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::resizeGL(int iWidth, int iHeight)
{
	double dAspect = 0.;

	//prevent a divide by zero
	if(0 == iHeight)
	{
		iHeight = 1;
	}

	//resize the gl vieport
	glViewport(0, 0, iWidth, iHeight);
	
	//select the projection matrix and reset it
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//calculate the aspect ratio
	dAspect = (GLfloat) iWidth / (GLfloat) iHeight;
	gluPerspective(45.0f, dAspect, 0.1f, 5000.0f);

	//reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
}

/////////////////////////////////////////////////////////////////////////////
// handle a click into the screen
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//turn off hit tests
	setHitTest(FALSE);

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

	//try selection
	selectGL(point.x, point.y);	

	wglMakeCurrent(NULL, NULL);
	
	CView::OnLButtonDown(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// handle a right mouse button double-click
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	//reset the eye direction
	resetEye();

	//redraw
	Invalidate();
	
	CView::OnRButtonDblClk(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// handle selection
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::selectGL(int x, int y)
{
	int i;
	int choose;
	int newchoose;
	int depth;
	GLuint	buffer[512];
	GLint	hits;

	// The Size Of The Viewport. [0] Is <x>, [1] Is <y>, [2] Is <length>, [3] Is <width>
	GLint	viewport[4];

	//see if selection is even turned on
	if(FALSE == m_bUseSelection)
	{
		return;
	}

	// This Sets The Array <viewport> To The Size And Location Of The Screen Relative To The Window
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(512, buffer);

	// Puts OpenGL In Selection Mode. Nothing Will Be Drawn.  Object ID's and Extents Are Stored In The Buffer.
	glRenderMode(GL_SELECT);

	// Initializes The Name Stack
	glInitNames();

	// Push 0 (At Least One Entry) Onto The Stack
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// This Creates A Matrix That Will Zoom Up To A Small Portion Of The Screen, Where The Mouse Is.
	gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y), 1.0f, 1.0f, viewport);

	// Apply The Perspective Matrix
	gluPerspective(45.0f, (GLfloat) (viewport[2]-viewport[0]) / (GLfloat) (viewport[3]-viewport[1]), 0.1f, 5000.0f);
	glMatrixMode(GL_MODELVIEW);
	DrawScene();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	//count the hits
	hits = glRenderMode(GL_RENDER);

	if (hits > 0)
	{
		// Make Our Selection The First Object
		choose = buffer[3];

		// Store How Far Away It Is
		depth = buffer[1];

		for(i = 1; i < hits; i++)
		{
			newchoose = buffer[(i << 2)+3];

			if(0 == newchoose)
			{
				continue;
			}

			//depth test
			if (buffer[(i << 2) + 1] < GLuint(depth))
			{
				// Select The Closer Object
				choose = buffer[(i << 2)+3];
				
				// Store How Far Away It Is
				depth = buffer[(i << 2)+1];
			}       
		}

		SelectObject(choose);
	}
}

/////////////////////////////////////////////////////////////////////////////
// override
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::DrawScene()
{

}

/////////////////////////////////////////////////////////////////////////////
// override
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::SelectObject(int /*id*/)
{

}

/////////////////////////////////////////////////////////////////////////////
// override
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::resetEye()
{

}

/////////////////////////////////////////////////////////////////////////////
// rotation code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect client;
	float dA, dB;
	double dX, dY;
	double magnitude;
	double theta;

	//perform hit testing if we need it
	static int iCount = 0;

	if(nFlags & MK_RBUTTON)
	{
		GetClientRect(&client);

		//capture the mouse
		SetCapture();

		//get the delta values
		dX = (double) m_ptOld.x - point.x;
		dY = (double) (client.bottom - m_ptOld.y) - (client.bottom - point.y); // invert y-coords
		magnitude = pow(dX * dX + dY * dY, 0.5);
		theta = atan2(dY, dX);

		//see if they've got the left button, too
		if(nFlags & MK_LBUTTON)
		{
			//alter the look point and eye points
			m_dLook[0] += (float) (dX / 7.);
			m_dLook[2] -= (float) (dY / 7.);
			m_dCamera[0] += (float) (dX / 7.);
			m_dCamera[2] -= (float) (dY / 7.);
		}
		// if holding down control button, zoom
		else if(nFlags & MK_CONTROL)
		{
			dA = float(m_dCamera[1] + dY / 10.0);
			dB = float(m_dCamera[2] + dY / 800.0);

			//clamp
			if(dA > 10.0 && dA < 150.0)
			{
				m_dCamera[1] = dA;
				m_dCamera[2] = dB;
			}
		}
		else
		{
			//calculate rotations
			m_dxRot += magnitude * R2D * sin(theta) / 120.0;
			m_dyRot -= magnitude * R2D * cos(theta) / 120.0; // inverted y-coords

			//clamp
			if(m_dxRot < -86.) m_dxRot = -86.;
			if(m_dxRot > 0) m_dxRot = 0.;
		}

		Invalidate();
	}
	//perform hit testing if we need it
	else if(TRUE == m_bHitTest)
	{
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

		//try selection
		selectGL(point.x, point.y);	

		wglMakeCurrent(NULL, NULL);

		Invalidate();
	}

	
	CView::OnMouseMove(nFlags, point);

	m_ptOld = point; // save off this point
}

/////////////////////////////////////////////////////////////////////////////
// use the mouse wheel to zoom as well
/////////////////////////////////////////////////////////////////////////////
BOOL CGLBaseView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	float dA, dB;
	double dY;

	dY = -(zDelta / 120.);
	
	dA = float(m_dCamera[1] + dY * 5.0);
	dB = float(m_dCamera[2] + dY / 160.0);

	//clamp
	if(dA > 10.0 && dA < 150.0)
	{
		m_dCamera[1] = dA;
		m_dCamera[2] = dB;
	}

	//redraw
	Invalidate();
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

/////////////////////////////////////////////////////////////////////////////
// handle a button up
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	//release the mouse capture
	ReleaseCapture();
	
	CView::OnRButtonUp(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// handle cleanup
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::OnDestroy() 
{	
	CPalette palDefault;

	//need a rendering context
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

	//clear previous textures
	glDeleteTextures(GL_BASE_TEXTURES, &m_texBase[0]);
	
	//clear other textures
	clearOtherTextures();

	//clear display lists
	glDeleteLists(m_lists[0], GL_DISPLAY_LISTS);

	//need a rendering context
	wglMakeCurrent(NULL, NULL);

	//delete it
    if (NULL != m_hrc)
	{
        ::wglDeleteContext(m_hrc);
	}

    //select our palette out of the dc
    palDefault.CreateStockObject(DEFAULT_PALETTE);
    m_pDC->SelectPalette(&palDefault, FALSE);

    if (NULL != m_pDC)
	{
        delete m_pDC;
	}

    CView::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// turn on/off hit testing
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::setHitTest(BOOL b)
{
	//set the value
	m_bHitTest = b && m_bHighlight;

	//reset selection
	m_iSelection = -1;

}

/////////////////////////////////////////////////////////////////////////////
// rebuild textures on a mode change
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::rebuildTextures()
{
	//need a rendering context
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

	//set no texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//clear out the old textures
	glDeleteTextures(GL_BASE_TEXTURES, &m_texBase[0]);

	//need a rendering context
	wglMakeCurrent(NULL, NULL);

	//rebuild new textures
	buildTileTextures(); 

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// rebuild other textures on a mode change
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::rebuildOtherTextures()
{
	//need a rendering context
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

	//set no texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//clear old
	clearOtherTextures();

	//need a rendering context
	wglMakeCurrent(NULL, NULL);

	//rebuild
	buildOtherTextures();

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// turn mipmapping on or off
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::setMipMap()
{
	//set the flag
	m_bMip = GetPrivateProfileInt(INI_OPENGL, INI_MIPMAP, TRUE, INI_FILE);
	
	//need a rendering context
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

	//set no texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//clear out the old textures
	glDeleteTextures(GL_BASE_TEXTURES, &m_texBase[0]);

	//clear other textures
	clearOtherTextures();

	//need a rendering context
	wglMakeCurrent(NULL, NULL);

	//rebuild new textures
	buildTileTextures();
	buildOtherTextures();

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// build the tile texture maps
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::buildTileTextures()
{
	CString strFile;

	//need a rendering context
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

	//get the right texture file
	strFile = FILE_TILES;

	//set euro file (and sanity check)
	if(TRUE == m_bEuro && TRUE == checkForEuro())
	{
		strFile.Insert(strFile.GetLength() - 4, 'e');
	}

	//set small texture size
	if(TRUE == GetPrivateProfileInt(INI_OPENGL, INI_SMALL_TILES, FALSE, INI_FILE))
	{
		strFile.Insert(strFile.GetLength() - 4, "64");
	}

	//read in the textures
	readTextures(GL_BASE_TEXTURES, &m_texBase[0], strFile);

	//release context
	wglMakeCurrent(NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// build the other texture maps which are never reloaded
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::buildOtherTextures(void)
{
	BOOL bNoLoad;
	CString strFile;

	//need a rendering context
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

	//get noload
	bNoLoad = GetPrivateProfileInt(INI_OPENGL, INI_NOLOAD, FALSE, INI_FILE);

	//chip textures
	if(MAP->m_iChipsUsed & MAP_USES_34_CHIPS)
	{
		readTextures(GL_TEX_CHIP34, &m_texChip34[0], FILE_CHIP34);
	}
	
	if(MAP->m_iChipsUsed & MAP_USES_56_CHIPS)
	{
		readTextures(GL_TEX_CHIP56, &m_texChip56[0], FILE_CHIP56);
	}

	if(MAP->m_iChipsUsed & MAP_USES_SEA_CHIPS)
	{
		readTextures(GL_TEX_CHIPSEA, &m_texChipSea[0], FILE_CHIPSEA);
	}

	if(MAP->m_iMode & MAP_SEAFARERS)
	{
		//set seaports file
		strFile = FILE_SEAPORTS;

		//set euro file (and sanity check)
		if(TRUE == m_bEuro && TRUE == checkForEuro())
		{
			strFile.Insert(strFile.GetLength() - 4, 'e');
		}

		readTextures(GL_TEX_SEAPORTS, &m_texSeaPorts[0], strFile);

		if((FALSE == bNoLoad) && (TRUE == checkForSeaBorders()))
		{
			//set string
			strFile = FILE_SEABORDERS;

			//set small texture size
			if(TRUE == GetPrivateProfileInt(INI_OPENGL, INI_SMALL_TILES, FALSE, INI_FILE))
			{
				strFile.Insert(strFile.GetLength() - 4, "64");
			}

			readTextures(GL_TEX_SEABORDERS, &m_texSeaBorders[0], strFile);
		}
	}

	//build piece textures
	readTextures(GL_TEX_PIECES, &m_texPieces[0], FILE_PIECES);

	//release context
	wglMakeCurrent(NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// clear other textures
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::clearOtherTextures()
{
	BOOL bNoLoad;

	//get borders
	bNoLoad = GetPrivateProfileInt(INI_OPENGL, INI_NOLOAD, FALSE, INI_FILE);

	if(MAP->m_iChipsUsed & MAP_USES_34_CHIPS)
	{
		glDeleteTextures(GL_TEX_CHIP34, &m_texChip34[0]);
	}
	
	if(MAP->m_iChipsUsed & MAP_USES_56_CHIPS)
	{
		glDeleteTextures(GL_TEX_CHIP56, &m_texChip56[0]);
	}

	if(MAP->m_iChipsUsed & MAP_USES_SEA_CHIPS)
	{
		glDeleteTextures(GL_TEX_CHIPSEA, &m_texChipSea[0]);
	}

	if(MAP->m_iMode & MAP_SEAFARERS)
	{
		//clear sea port textures
		glDeleteTextures(GL_TEX_SEAPORTS, &m_texSeaPorts[0]);
		glDeleteTextures(GL_TEX_SEABORDERS, &m_texSeaBorders[0]);
	}

	//clear piece textures
	glDeleteTextures(GL_TEX_PIECES, &m_texPieces[0]);
}

/////////////////////////////////////////////////////////////////////////////
// determine if they have euro textures installed
/////////////////////////////////////////////////////////////////////////////
BOOL CGLBaseView::checkForEuro()
{
	int i;
	CFile file;
	CString strFile;
	BOOL bExists = TRUE;

	//check texture
	strFile = FILE_TILES;

	//set euro file
	strFile.Insert(strFile.GetLength() - 4, 'e');

	//try and open the file
	if(FALSE == file.Open(strFile, CFile::modeRead))
	{
		bExists = FALSE;
	}
	else
	{
		//close the file we just opened
		file.Close();
	}

	//now check res files
	for(i = 0; i < 5; i++)
	{
		switch(i)
		{
		case 0: strFile = FILE_RES; break;
		case 1: strFile = FILE_HISTORY; break;
		case 2: strFile = FILE_DECK; break;
		case 3: strFile = FILE_CARDS; break;
		case 4: strFile = FILE_SEAPORTS; break;
		}

		//set euro file
		strFile.Insert(strFile.GetLength() - 4, 'e');

		//try and open the file
		if(FALSE == file.Open(strFile, CFile::modeRead))
		{
			bExists = FALSE;
			break;
		}

		//close the file we just opened
		file.Close();
	}

	return bExists;
}

/////////////////////////////////////////////////////////////////////////////
// determine if they have sea borders installed
/////////////////////////////////////////////////////////////////////////////
BOOL CGLBaseView::checkForSeaBorders()
{
	BOOL bExists = TRUE;
	CFile file;
	CString strFile;

	//get the borders file name
	strFile = FILE_SEABORDERS;

	//set euro if needed
	if(TRUE == m_bEuro)
	{
		//NOT CREATED YET
		//strFile.Insert(strFile.GetLength() - 4, 'e');
	}
	
	//try and open the file
	if(FALSE == file.Open(strFile, CFile::modeRead))
	{
		bExists = FALSE;
	}
	else
	{
		//close the file we just opened
		file.Close();
	}

	return bExists;
}

/////////////////////////////////////////////////////////////////////////////
// helper function to read the textures from the .tex files
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::readTextures(int iTex, GLuint *pList, CString strFile, BOOL bMip)
{
	int i;
	BOOL bLoad;
	CFile fileIn;
	CString strIn;
	CString strErr;
	DWORD dwLength;
	BYTE *byte;
	ILuint ilImage;

	//generate textures
	glGenTextures(iTex, pList);

	// Lets OpenILUT know to use its OpenGL functions.
	ilutRenderer(ILUT_OPENGL);

	//open the right texture set
	if(FALSE == fileIn.Open(strFile, CFile::modeRead))
	{
		strErr.Format("Could not open %s.  Check to see if this file exists.", strFile);
		AfxMessageBox(strErr);
		return;
	}

	//attach the archive
	CArchive ar(&fileIn, CArchive::load);

	//read through the base textures
	for(i = 0; i < iTex; i++)
	{
		//read the file name and length
		ar >> strIn;
		ar >> dwLength;

		//prepare the buffer
		byte = new BYTE[dwLength];

		//read the data
		ar.Read(byte, dwLength);

		//create the IL image
		ilGenImages(1, &ilImage);

		//read into IL
		ilBindImage(ilImage);

		if(iTex == IMG_SIZE)
		{
			bLoad = ilLoadL(IL_BMP, (void *) byte, dwLength);
		}
		else
		{
			bLoad = ilLoadL(IL_JPG, (void *) byte, dwLength);
		}
		
		//see if we were successful
		if(FALSE == bLoad)
		{
			strErr.Format("Could not read texture %s from the file %s.", strIn, strFile);
			AfxMessageBox(strErr);
		}

		//create the actual texture
		if(bMip && m_bMip)
		{
			//mipmapping
			pList[i] = ilutGLBindMipmaps();
		}
		else
		{
			pList[i] = ilutGLBindTexImage();
		}

		//set the tex parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (bMip && m_bMip) ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR);

		//delete the IL image
		ilDeleteImages(1, &ilImage);

		//clear the buffer
		delete[] byte;
		byte = NULL;
	}

	//close the archive
	ar.Close();

	//close the in file
	fileIn.Close();
}

/////////////////////////////////////////////////////////////////////////////
// turn gouraud on/off
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::setGouraud()
{
	//read value
	m_bGouraud = GetPrivateProfileInt(INI_OPENGL, INI_GOURAUD, TRUE, INI_FILE);

	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// turn highlighting
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::setHighlight()
{
	//read value
	m_bHighlight = GetPrivateProfileInt(INI_OPENGL, INI_HIGHLIGHT, TRUE, INI_FILE);

	//turn on/off hit testing
	setHitTest(m_bHighlight);

	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// turn on white outline
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::setWhiteOutline()
{
	//read value
	m_bWhiteOutline = GetPrivateProfileInt(INI_OPENGL, INI_WHITE, TRUE, INI_FILE);

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// set graphics detail
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::setDetail()
{
	//read graphics deatil
	m_iDetail = GetPrivateProfileInt(INI_OPENGL, INI_DETAIL, GL_HIGH_DETAIL, INI_FILE);

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// handle keyboard messages
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	BOOL bShift;
	BOOL bControl;
	CString str;
	short key;
	double dAdj = 0.0;
	double dA = 0., dB = 0.;
	double dX = 0., dY = 0.;

	//get control key state
	key = GetKeyState(VK_CONTROL);
	bControl = (key < 0);

	//get shift key state
	key = GetKeyState(VK_SHIFT);
	bShift = (key < 0);

	//control key down
	if(TRUE == bControl && FALSE == bShift)
	{
		switch(nChar)
		{
			case VK_UP: dAdj = -5.0; break;
			case VK_DOWN: dAdj = 5.0; break;
			default: break;
		}
	}

	//shift key down
	if(FALSE == bControl && TRUE == bShift)
	{
		//alter the look point and eye points for panning
		switch(nChar)
		{
		case VK_UP: dY = -0.25; break;
		case VK_DOWN: dY = 0.25; break;
		case VK_RIGHT: dX = -0.25; break;
		case VK_LEFT: dX = 0.25; break;			
		}
	}

	//just normal keys
	if(FALSE == bControl && FALSE == bShift)
	{
		switch(nChar)
		{
		case VK_LEFT: m_dyRot -= 1.0; break;
		case VK_RIGHT: m_dyRot += 1.0; break;
		case VK_UP: m_dxRot -= 1.0; break;
		case VK_DOWN: m_dxRot += 1.0; break;
		}

	}

	//panning adjustments
	m_dLook[0] += (float) dX;
	m_dLook[2] -= (float) dY;
	m_dCamera[0] += (float) dX;
	m_dCamera[2] -= (float) dY;

	//zoom adjustments
	dA = float(m_dCamera[1] + dAdj / 10.0);
	dB = float(m_dCamera[2] + dAdj / 800.0);

	//clamp
	if(dA > 10.0 && dA < 150.0)
	{
		m_dCamera[1] = (float) dA;
		m_dCamera[2] = (float) dB;
	}

	//clamp
	if(m_dxRot < -86.) m_dxRot = -86.;
	if(m_dxRot > 0) m_dxRot = 0.;

	Invalidate(); 
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
