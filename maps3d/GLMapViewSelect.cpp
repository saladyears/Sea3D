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
#include "Maps3DView.h"
#include "define.h"
#include "defineMap.h"
#include "defineMaps3D.h"

/////////////////////////////////////////////////////////////////////////////
// turn selection on/off
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::setSelection(BOOL b)
{
	//turn hit testing on
	setHitTest(b);

	//turn on selection
	m_bUseSelection = b;

	//reset selection
	m_iSelection = -1;
}

/////////////////////////////////////////////////////////////////////////////
// catch a selection
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::SelectObject(int id)
{
	//set the selection id
	m_iSelection = id - 1;

	//check for a good selection
	if(m_iSelection < 0) return;

	//call the selection
	if(FALSE == m_bHitTest)
	{
		//handle it
		handleSelect();

		//reset the mode
		setDrawMode(m_nMode);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a selection
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::handleSelect()
{
	switch(m_nMode)
	{
	case GL_DRAW_NORMAL: break;
	case GL_DRAW_RES_PAINT: selectResPaint(); break;
	case GL_DRAW_PORT_PAINT: selectPortPaint(); break;
	case GL_DRAW_CHIT_PAINT: selectChitPaint(); break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// resource painting
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::selectResPaint()
{
	int nRes;
	CMapPiece *pPiece;

	//get the res we're painting
	nRes = VIEW->getResource();

	//get the map piece
	pPiece = &(m_pMap->m_mapPieces[m_iSelection]);

	//set the tile type
	pPiece->m_iPermaType = nRes;

	//reset chits
	pPiece->m_iPermaRoll = MAP_NOT_PERMANENT;
	pPiece->m_iPermaRollTex = MAP_NOT_PERMANENT;
	pPiece->m_iPermaRollTexType = MAP_NOT_PERMANENT;
	
	//set the general type
	if(TRUE == VALID_LAND(nRes))
	{
		pPiece->m_iPieceType = MAP_LAND;
	}
	else if(TRUE == VALID_OCEAN(nRes))
	{
		pPiece->m_iPieceType = MAP_SEA;
	}
	else if(RES_RANDOM == nRes)
	{
		pPiece->m_iPieceType = MAP_RANDOM;
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;
	}
	else if(RES_HIDDEN == nRes)
	{
		pPiece->m_iPieceType = MAP_HIDDEN;
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;
	}

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// port painting
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::selectPortPaint(void)
{
	int i;
	int nRes;

	//see what stage we're at
	if(-1 == m_nLastTile)
	{
		//set it
		m_nLastTile = m_iSelection;
	}
	else
	{
		//get the port we're painting
		nRes = VIEW->getResource();

		//set the tile type
		m_pMap->m_mapPieces[m_nLastTile].m_iPermaType = nRes;

		//set the general type
		m_pMap->m_mapPieces[m_nLastTile].m_iPieceType = MAP_PORT;

		//find the permanent rotation side
		for(i = 0; i < 6; i++)
		{
			if(m_pMap->m_mapPieces[m_nLastTile].m_iSides[i] == m_iSelection)
			{
				m_pMap->m_mapPieces[m_nLastTile].m_iPermaPortSide = i;
				break;
			}
		}

		//reset chits
		m_pMap->m_mapPieces[m_nLastTile].m_iPermaRoll = MAP_NOT_PERMANENT;
		m_pMap->m_mapPieces[m_nLastTile].m_iPermaRollTex = MAP_NOT_PERMANENT;
		m_pMap->m_mapPieces[m_nLastTile].m_iPermaRollTexType = MAP_NOT_PERMANENT;

		//reset selection
		m_nLastTile = -1;
	}

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// chit painting
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::selectChitPaint()
{
	int nRes;
	int nRoll;

	//get the resource
	nRes = VIEW->getResource();

	//get it's actual value
	nRes -= RES_CHIT_2;

	//get the roll value
	nRoll = nRes + (nRes > 5) ? 3 : 2;

	//set the values
	m_pMap->m_mapPieces[m_iSelection].m_iPermaRoll = nRoll;
	m_pMap->m_mapPieces[m_iSelection].m_iPermaRollTex = nRes;
	m_pMap->m_mapPieces[m_iSelection].m_iPermaRollTexType = MAP_USES_SEA_CHIPS;

	//redraw
	Invalidate();
}