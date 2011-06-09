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
#include "maps3d.h"
#include "GLMapView.h"
#include "MapDatabase.h"
#include "defineIni.h"
#include "defineMap.h"
#include "defineMaps3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLMapView

IMPLEMENT_DYNCREATE(CGLMapView, CGLBaseView)

CGLMapView::CGLMapView()
{
	//no tiles yet
	m_nTiles = 0;

	//create the map
	m_pMap = new CS3DMap;

	//TEMP MAP CREATION
	MAPS->createMap(MAP_BORDERS_NO_X, m_pMap, NULL);
	m_nTiles = m_pMap->m_mapPieces.size();
	m_pMap->m_dInitialDepth = 80.0;
	m_pMap->m_iMode = MAP_SEAFARERS;
	m_pMap->m_iChipsUsed = MAP_USES_SEA_CHIPS;
	m_pMap->m_dInitialRot = 60.;

	//set current
	MAPS->setCurrent(m_pMap);

	resetEye();
	resetMapState();
	resetTileState();
}

CGLMapView::~CGLMapView()
{
	//destroy the map
	if(NULL != m_pMap)
	{
		delete m_pMap;
		m_pMap = NULL;
	}
}


BEGIN_MESSAGE_MAP(CGLMapView, CGLBaseView)
	//{{AFX_MSG_MAP(CGLMapView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLMapView diagnostics

#ifdef _DEBUG
void CGLMapView::AssertValid() const
{
	CGLBaseView::AssertValid();
}

void CGLMapView::Dump(CDumpContext& dc) const
{
	CGLBaseView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGLMapView message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
int CGLMapView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	BOOL bNoLoad;

	if (CGLBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//save it
	bNoLoad = GetPrivateProfileInt(INI_OPENGL, INI_NOLOAD, FALSE, INI_FILE);
	
	//override border setting
	WritePrivateProfileString(INI_OPENGL, INI_NOLOAD, "1", INI_FILE);

	//build textures
	buildOtherTextures();

	//restore
	WritePrivateProfileString(INI_OPENGL, INI_NOLOAD, bNoLoad ? "1" : "0", INI_FILE);

	//borders are disabled
	m_bBorders = FALSE;

	//set initial rotation
	m_dyRot = MAP->m_dInitialRot;

	//make sure textures are enabled
	glEnable(GL_TEXTURE_2D);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// set a new drawing mode
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::setDrawMode(int nMode)
{
	//set the mode
	m_nMode = nMode;

	//other stuff
	switch(nMode)
	{
	case GL_DRAW_NORMAL:
		//turn selection off
		setSelection(FALSE);

		//reset variables
		resetMapState();
		break;
	case GL_DRAW_PORT_PAINT:
		//set selection on
		setSelection(TRUE);
		break;
	case GL_DRAW_RES_PAINT:
		//set the selection on
		setSelection(TRUE);

		//reset variables
		resetMapState();
		break;
	case GL_DRAW_CHIT_PAINT:
		//set the selection on
		setSelection(TRUE);

		//reset variables
		resetMapState();
		break;
	default:
		break;
	}

	//refresh
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// reset camera settings
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::resetEye()
{
	//reset eyepoint
	m_dCamera[0] = 0.;
	m_dCamera[1] = m_pMap->m_dInitialDepth;
	m_dCamera[2] = 1.0;	

	//reset rotation
	m_dxRot = 0.;
	m_dyRot = 0.;

	//reset look point
	m_dLook[0] = 0.;
	m_dLook[1] = 0.;
	m_dLook[2] = 0.;
}

/////////////////////////////////////////////////////////////////////////////
// reset the map state
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::resetMapState()
{
	//no tile selection yet
	m_nLastTile = -1;
}

/////////////////////////////////////////////////////////////////////////////
// tile drawing reset
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::resetTileState(int nInit, BOOL bNoSelectBorders)
{
	int i;

	m_vnTiles.clear();
	m_vnTiles.resize(m_nTiles, nInit);

	//turn off border selection if wanted
	if(TRUE == bNoSelectBorders)
	{
		for(i = 0; i < m_nTiles; i++)
		{
			if(TRUE == m_pMap->m_mapPieces[i].m_bBorder)
			{
				m_vnTiles[i] &= ~GL_TILE_BIT_SELECT;
			}
		}
	}
}