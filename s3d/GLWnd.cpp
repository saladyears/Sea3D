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
#include "GLWnd.h"
#include "Mmsystem.h"
#include "defineFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// callback function for timer
/////////////////////////////////////////////////////////////////////////////
void CALLBACK timer(UINT /*uID*/, UINT /*uMsg*/, DWORD dwUser, DWORD /*dw1*/, DWORD /*dw2*/)
{
	((CGLWnd *) dwUser)->OnTimer();
}

/////////////////////////////////////////////////////////////////////////////
// palette entries
/////////////////////////////////////////////////////////////////////////////
extern unsigned char threeto8[8];
extern unsigned char twoto8[4];
extern unsigned char oneto8[2];

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
// CGLWnd

CGLWnd::CGLWnd()
{
	int i;

	//reset rotation
	m_dXRot = 0.;
	m_dYRot = 0.;
	m_dZRot = 0.;

	//delta rotations
	m_dX = 0.;
	m_dY = 0.;
	m_dZ = 0.;

	// GL cannot use palettes anyway, so convert early.
	ilEnable(IL_CONV_PAL);

	// Gets rid of dithering on some nVidia-based cards.
	ilutEnable(ILUT_OPENGL_CONV);

	//current cycle
	m_iCycle = -1;

	//tick counter
	m_iTickCounter = 5000;

	//start in first cycle
	m_bInCycle = TRUE;

	//cycle lengths
	for(i = 0; i < NUM_CYCLES; i++)
	{
		m_iCycleLengths[i] = SLICES_PER_SECOND;
		m_iCyclePauses[i] = 3 * SLICES_PER_SECOND;
	}

	//set first cycle length longer
	m_iCycleLengths[0] = 3 * SLICES_PER_SECOND;

	//set some pauses longer
	m_iCyclePauses[0] = SLICES_PER_SECOND;
	m_iCyclePauses[3] = 5 * SLICES_PER_SECOND;
	m_iCyclePauses[4] = 4 * SLICES_PER_SECOND;
	m_iCyclePauses[5] = 5 * SLICES_PER_SECOND;

	//set initial fade
	m_dFade = 0.0;
}

CGLWnd::~CGLWnd()
{
}


BEGIN_MESSAGE_MAP(CGLWnd, CWnd)
	//{{AFX_MSG_MAP(CGLWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGLWnd message handlers

/////////////////////////////////////////////////////////////////////////////
// drawing
/////////////////////////////////////////////////////////////////////////////
void CGLWnd::OnPaint() 
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
// creation
/////////////////////////////////////////////////////////////////////////////
int CGLWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	TIMECAPS tc;

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//create the GL window
	createGL();

	//see if we can handle it
	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
	{
		return -1;
	}

	m_uiTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);

	//start timer
	timeBeginPeriod(m_uiTimerRes);

	//start the call timer
	m_uiTimer = timeSetEvent(SLICE_LENGTH, 5, timer, (DWORD) this, TIME_PERIODIC);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// create the GL window with appropriate settings
/////////////////////////////////////////////////////////////////////////////
void CGLWnd::createGL(void)
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
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	//set the clear color
	glClearColor(0., 0., 0., 0.);

	//set up depth testing
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//set up lighting
	GLfloat amb[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat dif[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat pos[] = {0.0f, 1.0f, 4.0f, 1.0f};

	glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT1, GL_POSITION, pos);
	glEnable(GL_LIGHT1);

	//turn on lighting
	glEnable(GL_LIGHTING);

	//set line width
	glLineWidth(1.5);

	//build about textures
	readTextures(GL_TEX_ABOUT, &m_texAbout[0], FILE_ABOUT);

	//build display lists
	buildLists();

	wglMakeCurrent(NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// set up the pixel format
/////////////////////////////////////////////////////////////////////////////
BOOL CGLWnd::setPixelFormat()
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
unsigned char CGLWnd::ComponentFromIndex(int i, UINT nbits, UINT shift)
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
void CGLWnd::CreateRGBPalette()
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
BOOL CGLWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	//modify the style to have the things we need
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// handle a resize event
/////////////////////////////////////////////////////////////////////////////
void CGLWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

	resizeGL(cx, cy);

	wglMakeCurrent(NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// prevent GL from flashing
/////////////////////////////////////////////////////////////////////////////
BOOL CGLWnd::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// resize the GL window
/////////////////////////////////////////////////////////////////////////////
void CGLWnd::resizeGL(int iWidth, int iHeight)
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
	gluPerspective(45.0f, dAspect, 0.1f, 10.);

	//reset the modelview matrix
	glMatrixMode(GL_MODELVIEW);
}

/////////////////////////////////////////////////////////////////////////////
// draw the GL scene
/////////////////////////////////////////////////////////////////////////////
void CGLWnd::DrawScene()
{
	int i;
	float *dFade = NULL;

	//clear things out
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//set the camera point
	gluLookAt(0.0, 0.0, CAMERA_NEAR, 0.0, 0.0, 0.0, 0., 1.0, 0.);

	//set rotation
	glRotatef(m_dXRot, 1.0f, 0.0f, 0.0f);
	glRotatef(m_dYRot, 0.0f, 1.0f, 0.0f);	
	glRotatef(m_dZRot, 0.0f, 0.0f, 1.0f);	

	//turn on textures
	glEnable(GL_TEXTURE_2D);

	//set polygon mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//create fade array
	dFade = new float[4];
	for(i = 0; i < 4; dFade[i++] = m_dFade);

	//set the color
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, dFade);

	//delete
	delete[] dFade;
	dFade = NULL;

	//draw with textures
	for(i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_texAbout[i]);
		glCallList(m_lists[i]);
	}

	//turn off textures
	glDisable(GL_TEXTURE_2D);

	//set polygon mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//draw outline
	for(i = 0; i < 6; i++)
	{
		glCallList(m_lists[i]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle timer events
/////////////////////////////////////////////////////////////////////////////
void CGLWnd::OnTimer()
{
	//increment tick counter
	m_iTickCounter++;

	//see if we trigger the next cycle or pause
	if(TRUE == m_bInCycle)
	{
		if(m_iTickCounter >= m_iCycleLengths[m_iCycle])
		{
			//no longer in cycle
			m_bInCycle = FALSE;

			//reset tick counter
			m_iTickCounter = 0;
		}
	}
	else
	{
		if(m_iTickCounter >= m_iCyclePauses[m_iCycle])
		{
			//now in a cycle
			m_bInCycle = TRUE;

			//increment cycle
			m_iCycle++;

			//see if we're over
			if(m_iCycle > NUM_CYCLES - 1)
			{
				m_iCycle = RESTART_CYCLE;
			}

			//set the new cycle values
			setCycle();

			//reset tick counter
			m_iTickCounter = 0;
		}
	}

	//if we're in a cycle, do it
	if(TRUE == m_bInCycle)
	{
		handleCycle();
	}
}

/////////////////////////////////////////////////////////////////////////////
// start a new cycle
/////////////////////////////////////////////////////////////////////////////
void CGLWnd::setCycle()
{
	float dRotX = 0.;
	float dRotY = 0.;
	float dRotZ = 0.;

	switch(m_iCycle)
	{
	case 0: m_dF = 1.0f / m_iCycleLengths[m_iCycle];
	case 1: dRotX = 90.; break;
	case 2: dRotY = -90.; dRotZ = 90.; break;
	case 3: dRotX = -90.; dRotY = -90.; dRotZ = 90.; break;
	case 4: dRotX = 90.; dRotY = 180.; break;
	case 5: dRotY = 90.; dRotZ = 90.; break;
	case 6: dRotX = -90.; dRotY = -90.; dRotZ = 90.; break;
	}

	//set new delta values
	m_dX = dRotX / (float) m_iCycleLengths[m_iCycle];
	m_dY = dRotY / (float) m_iCycleLengths[m_iCycle];
	m_dZ = dRotZ / (float) m_iCycleLengths[m_iCycle];
}

/////////////////////////////////////////////////////////////////////////////
// cycle handling
/////////////////////////////////////////////////////////////////////////////
void CGLWnd::handleCycle()
{
	//handle first cycle
	if(0 == m_iCycle)
	{
		m_dFade += m_dF;

		return;
	}

	//increment rotation
	m_dXRot += m_dX;
	m_dYRot += m_dY;
	m_dZRot += m_dZ;
}

/////////////////////////////////////////////////////////////////////////////
// handle shutdown
/////////////////////////////////////////////////////////////////////////////
void CGLWnd::OnDestroy() 
{
	CPalette palDefault;

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

	//clear display lists
	glDeleteLists(m_lists[0], 6);

	//clear about textures
	glDeleteTextures(GL_TEX_ABOUT, &m_texAbout[0]);
	
	//stop the timer
	timeKillEvent(m_uiTimer);

	//shut down the timer
	timeEndPeriod(m_uiTimerRes);	

	CWnd::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// helper function to read the textures from the .tex files
/////////////////////////////////////////////////////////////////////////////
void CGLWnd::readTextures(int iTex, GLuint *pList, CString strFile, BOOL bMip)
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
		bLoad = ilLoadL(IL_JPG, (void *) byte, dwLength);
		
		//see if we were successful
		if(FALSE == bLoad)
		{
			strErr.Format("Could not read texture %s from the file %s.", strIn, strFile);
			AfxMessageBox(strErr);
		}

		//create the actual texture
		if(bMip)
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bMip ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

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
// create all the display lists
/////////////////////////////////////////////////////////////////////////////
void CGLWnd::buildLists()
{
	int i;
	
	//create display lists
	for(i = 0; i < 6; i++)
	{
		m_lists[i] = glGenLists(1);
	}
	
	glNewList(m_lists[0], GL_COMPILE);
		glBegin(GL_QUADS);
			// Front Face
			glNormal3f( 0.0f, 0.0f, 1.0f);					// Normal Pointing Towards Viewer
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 1 (Front)
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 2 (Front)
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Front)
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 4 (Front)
		glEnd();
	glEndList();

	glNewList(m_lists[1], GL_COMPILE);
		glBegin(GL_QUADS);
			// Top Face
			glNormal3f( 0.0f, 1.0f, 0.0f);					// Normal Pointing Up
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 1 (Top)
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 2 (Top)
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Top)
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 4 (Top)
		glEnd();
	glEndList();

	glNewList(m_lists[2], GL_COMPILE);
		glBegin(GL_QUADS);
			// Right face
			glNormal3f( 1.0f, 0.0f, 0.0f);					// Normal Pointing Right
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 1 (Right)
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 2 (Right)
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Right)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 4 (Right)
		glEnd();
	glEndList();

	glNewList(m_lists[3], GL_COMPILE);
		glBegin(GL_QUADS);
			// Back Face
			glNormal3f( 0.0f, 0.0f,-1.0f);					// Normal Pointing Away From Viewer
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Back)
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 2 (Back)
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 3 (Back)
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 4 (Back)
		glEnd();
	glEndList();
	
	glNewList(m_lists[4], GL_COMPILE);
		glBegin(GL_QUADS);
			// Bottom Face
			glNormal3f( 0.0f,-1.0f, 0.0f);					// Normal Pointing Down
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Bottom)
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 2 (Bottom)
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 3 (Bottom)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 4 (Bottom)
		glEnd();
	glEndList();

	glNewList(m_lists[5], GL_COMPILE);
		glBegin(GL_QUADS);
			// Left Face
			glNormal3f(-1.0f, 0.0f, 0.0f);					// Normal Pointing Left
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Left)
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 2 (Left)
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 3 (Left)
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 4 (Left)
		glEnd();
	glEndList();
}