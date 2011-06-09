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
#include "define.h"
#include "defineMap.h"
#include "defineMaps3D.h"

/////////////////////////////////////////////////////////////////////////////
// draw the GL scene
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::DrawScene()
{
	//clear things out
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();

	//set the camera point
	gluLookAt(m_dCamera[0], m_dCamera[1], m_dCamera[2], m_dLook[0], m_dLook[1], m_dLook[2], 0., 1.0, 0.);

	//draw
	switch(m_nMode)
	{
	case GL_DRAW_NORMAL: drawNormal(); break;
	case GL_DRAW_RES_PAINT: drawResPaint(); break;
	case GL_DRAW_PORT_PAINT: drawPortPaint(); break;
	case GL_DRAW_CHIT_PAINT: drawChitPaint(); break;
	default: drawNormal(); break;
	}

	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// base drawing functionality
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::drawBase()
{
	int i;

	//push the matrix
	glPushMatrix();

	//rotation
	glRotated(m_dxRot, 1.0f, 0.0f, 0.0f);
	glRotated(m_dyRot, 0.0f, 1.0f, 0.0f);

	//draw each tile and chit
	for(i = 0; i < m_nTiles; i++)
	{
		//tiles
		drawTile(i);

		//chits
		drawChit(i);
	}

	//pop the matrix
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// draw normal
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::drawNormal()
{
	//reset tile states
	resetTileState();

	//draw it
	drawBase();
}

/////////////////////////////////////////////////////////////////////////////
// draw resource selection
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::drawResPaint()
{
	//reset tile states so all are selectable
	resetTileState(GL_TILE_BIT_SELECT | GL_TILE_BIT_LIGHT);

	//draw it
	drawBase();
}

/////////////////////////////////////////////////////////////////////////////
// draw port selection
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::drawPortPaint()
{
	int i;
	int nSide;

	//check to see if we have the first tile selected
	if(-1 == m_nLastTile)
	{
		//reset tile states so all are selectable
		resetTileState(GL_TILE_BIT_SELECT | GL_TILE_BIT_LIGHT, FALSE);
	}
	else
	{
		//otherwise, reset to no selection and light and manually turn them on
		resetTileState(0);

		//set the outline
		m_vnTiles[m_nLastTile] = GL_TILE_BIT_SEL_OUTLINE;

		//turn on selectable tiles
		for(i = 0; i < 6; i++)
		{
			//get side
			nSide = m_pMap->m_mapPieces[m_nLastTile].m_iSides[i];

			//set it
			if((-1 != nSide) && (FALSE == m_pMap->m_mapPieces[nSide].m_bBorder))
			{
				m_vnTiles[nSide] = GL_TILE_BIT_SELECT | GL_TILE_BIT_LIGHT;
			}
		}
	}

	//draw it
	drawBase();
}

/////////////////////////////////////////////////////////////////////////////
// draw chit placement locations
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::drawChitPaint()
{
	int i;

	//reset to no selection and light and manually turn them on
	resetTileState(0);

	//search tiles for land tiles
	for(i = 0; i < m_nTiles; i++)
	{
		if(FALSE == VALID_RESOURCE(m_pMap->m_mapPieces[i].m_iPermaType))
		{
			continue;
		}

		//set it on
		m_vnTiles[i] = GL_TILE_BIT_SELECT | GL_TILE_BIT_LIGHT;
	}

	//draw it
	drawBase();
}

/////////////////////////////////////////////////////////////////////////////
// draw a tile
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::drawTile(int nTile)
{
	int nType;
	double dX, dZ, dRot;
	BOOL bBorder;
	CMapPiece *pPiece;

	//get the piece in question
	pPiece = &(m_pMap->m_mapPieces[nTile]);

	//see if it's a border tile
	bBorder = pPiece->m_bBorder;

	//get the piece type
	nType = pPiece->m_iPermaType;

	//piece coords
	dX = pPiece->m_glCoords[0];
	dZ = pPiece->m_glCoords[2];

	//draw tile outline if necessary
	if((m_vnTiles[nTile] & GL_TILE_BIT_SELECT) && (m_iSelection == nTile))
	{
		drawTileOutline(nTile, &glHighlight);
	}

	//see if this tile gets a selection outline
	if(m_vnTiles[nTile] & GL_TILE_BIT_SEL_OUTLINE)
	{
		drawTileOutline(nTile, &glGold);
	}

	//set the base color
	if(m_vnTiles[nTile] & GL_TILE_BIT_LIGHT)
	{
		glWhite.UseMaterial();
	}
	else
	{
		glGray.UseMaterial();
	}

	//draw the sea port if it has one
	if((TRUE == VALID_PORT(nType)) || (TRUE == bBorder))
	{
		//handle seaports
		if(TRUE == VALID_PORT(nType))
		{
			//calculate rotation
			dRot = ((pPiece->m_iPermaPortSide + 4) % 6) * 60.;

			//set the texture
			glBindTexture(GL_TEXTURE_2D, m_texSeaPorts[nType - GL_SEAPORT_OFFSET]);
			seaport(dX, 0., dZ, dRot);
		}

		//set the texture
		glBindTexture(GL_TEXTURE_2D, m_texBase[RES_OCEAN]);
	}
	else
	{
		//switch to outline mode if needed
		if(MAP_NOT_PERMANENT == nType)
		{
			glDisable(GL_TEXTURE_2D);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, m_texBase[nType]);
		}
	}

	//reset the tile name
	glLoadName(0);

	//load the tile name if needed
	if(m_vnTiles[nTile] & GL_TILE_BIT_SELECT)
	{
		glLoadName(nTile + 1);
	}

	//draw the tile
	hexagon(dX, 0., dZ, 0.0);

	//reset white
	glWhite.UseMaterial();

	//reenable textures
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/////////////////////////////////////////////////////////////////////////////
// draw a chit
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::drawChit(int nTile)
{
	double dX, dZ;
	CMapPiece *pPiece;

	//get the piece in question
	pPiece = &(m_pMap->m_mapPieces[nTile]);

	//ensure that we want to draw a chit here
	if(MAP_NOT_PERMANENT == pPiece->m_iPermaRoll)
	{
		return;
	}

	//get coords
	dX = pPiece->m_glCoords[0];
	dZ = pPiece->m_glCoords[2];

	//set the texture
	glBindTexture(GL_TEXTURE_2D, m_texChipSea[pPiece->m_iPermaRollTex]);	

	//draw the chit
	disc(dX, 0.1, dZ);

	//draw cylinder if detail is high enough
	if(GL_LOW_DETAIL <= m_iDetail)
	{
		glBindTexture(GL_TEXTURE_2D, m_texChip34[GL_TEX_CHIP34 - 1]);
		cylinder(dX, 0.1, dZ);
	}
}

/////////////////////////////////////////////////////////////////////////////
// outline a specified tile
/////////////////////////////////////////////////////////////////////////////
void CGLMapView::drawTileOutline(int nTile, GLMaterial *mat, int iWidth)
{
	double dX, dZ;

	//set polygon mode to outline
	glDisable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(iWidth);

	//use the sent in color
	mat->UseMaterial();

	//get coords
	dX = m_pMap->m_mapPieces[nTile].m_glCoords[0];
	dZ = m_pMap->m_mapPieces[nTile].m_glCoords[2];

	//draw the outline
	hexagonoutline(dX, 0.1, dZ);

	//reset
	glWhite.UseMaterial();
	glLineWidth(1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
}