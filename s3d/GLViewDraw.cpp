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
#include "settlers.h"
#include "GLView.h"
#include "SettlersView.h"
#include "ColorDatabase.h"
#include "rand.h"
#include "defineIni.h"
#include "defineVariants.h"
#include "defineColors.h"
#include "defineMap.h"
#include "defineDebug.h"
#include "defineCode.h"
#include "defineFile.h"

/////////////////////////////////////////////////////////////////////////////
// draw the GL scene
/////////////////////////////////////////////////////////////////////////////
void CGLView::DrawScene()
{

	//clear things out
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();

#ifndef DEBUG_SPECIAL_DRAW
	//set the camera point
	gluLookAt(m_dCamera[0], m_dCamera[1], m_dCamera[2], m_dLook[0], m_dLook[1], m_dLook[2], 0., 1.0, 0.);

	switch(m_iMode)
	{
	case GL_NORMAL: drawNormal(); break;
	case GL_PLACE_PLAYERS: drawPlayerPlace(); break;
	case GL_PLACE_THIRD_ROAD: drawRoadPlace(); break;
	case GL_WAIT: drawNormal(); break;
	case GL_SHOW_ROLLED: drawGoods(); break;
	case GL_PLACE_ROAD: drawRoadPlace(); break;
	case GL_MOVE_SHIP:
	case GL_PLACE_SHIP: drawShipPlace(); break;
	case GL_PLACE_CITY: drawCityPlace(); break;
	case GL_PLACE_SETTLEMENT: drawSettlePlace(); break;
	case GL_PLACE_ROBBER: drawRobberPlace(); break;
	case GL_CHOOSE_SHIP: drawShipChoose(); break;
	case GL_PLACE_PORT: drawPortPlace(); break;
	case GL_CHITLESS_TILE: drawChitlessTile(); break;
	case GL_VOLCANO: drawVolcano(); break;
	case GL_GAME_OVER: drawNormal(); break;
	}

	//screenshot, if needed
	if(TRUE == m_bScreen)
	{
		screen();
	}
#endif	//DEBUG_SPECIAL_DRAW

#ifdef DEBUG_SPECIAL_DRAW

	//set the camera point
	gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0., 1.0, 0.);

	//push the matrix
	glPushMatrix();

	//rotation
	glRotated(m_dxRot, 1.0f, 0.0f, 0.0f);
	glRotated(m_dyRot, 0.0f, 1.0f, 0.0f);

	//pop the matrix
	glPopMatrix();

#endif	//DEBUG_SPECIAL_DRAW

#ifdef DEBUG_BUILD_CARDS
	//CARD CREATION
	GLuint imgRes[IMG_SIZE];
	readTextures(IMG_SIZE, &imgRes[0], "tex\\rese.tex", FALSE);

	//set luminiescent
	glWhite.UseMaterial();

	//use textures
	glBindTexture(GL_TEXTURE_2D, imgRes[IMG_ORE]);

	gluLookAt(14.0, 6.0, -16.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glLineWidth(2.0);
	
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	static int iCards = 1;
	static int drawCount = 25;

	//draw the card object
	for(int i = 0; i < iCards; i++)
	{
		glTranslated(0.0, 0.02, 0.0);
		glBegin(GL_QUADS);
			glNormal3f(0.0, 1.0, 0.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(-1.45, 0.0, -2.1);
			glTexCoord2d(1.00, 0.00); glVertex3d(1.45, 0.0, -2.1);
			glTexCoord2d(1.00, 1.00); glVertex3d(1.45, 0.0, 2.1);
			glTexCoord2d(0.00, 1.00); glVertex3d(-1.45, 0.0, 2.1);	
		glEnd();
	}

	if(iCards < 42 && 50 == drawCount)
	{
		//take a screen shot
		ilutGLScreenie();

		//set next increment
		if(1 == iCards) iCards = 7;
		else iCards += 7;
	
		drawCount = 0;
	}
	else if(25 == drawCount)
	{
		Sleep(100);
	}

	++drawCount;
	
	//clean textures
	glDeleteTextures(IMG_SIZE, &imgRes[0]);
#endif	//DEBUG_BUILD_CARDS

	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// take a screen shot
/////////////////////////////////////////////////////////////////////////////
void CGLView::screen()
{
	ILuint iluImage;

	//set flag so we don't do this twice
	m_bScreen = FALSE;

	//generate image
	ilGenImages(1, &iluImage);

	//bind the image
	ilBindImage(iluImage);

	//screen shot it
	if(FALSE == ilutGLScreen())
	{
		return;
	}

	//enable file overwrite
	ilEnable(IL_FILE_OVERWRITE);

	//now save the image
	if(FALSE == ilSaveImage((char * const) (LPCTSTR) m_strScreenPath))
	{
		return;
	}

	//disable overwrite
	ilDisable(IL_FILE_OVERWRITE);

	//cleanup
	ilDeleteImages(1, &iluImage);
}

/////////////////////////////////////////////////////////////////////////////
// drawing start code
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawStart()
{
	//push the matrix
	glPushMatrix();

	//rotation
	glRotated(m_dxRot, 1.0f, 0.0f, 0.0f);
	glRotated(m_dyRot, 0.0f, 1.0f, 0.0f);
}

/////////////////////////////////////////////////////////////////////////////
// end drawing code
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawEnd()
{
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// the base drawing mechanism
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawBase()
{
	int i;

	//initialize drawing
	drawStart();

	//run through the piece list
	for(i = 0; i < m_nTiles; i++)
	{
		//draw this tile
		drawTile(i);

		//draw the chips
		drawChip(i);

		//draw the settlements/cities
		drawCities(i);

		//reset base name
		glLoadName(0);

		//draw roads
		drawRoads(i);
	}

	//draw robber depending on graphics detail
	if(GL_MED_DETAIL <= m_iDetail)
	{
		drawRobber();
	}
	else
	{
		//draw the robber outline
		drawTileOutline(m_iRobber, &glRed);

		if(-1 != m_iPirate)
		{
			drawTileOutline(m_iPirate, &glRed);
		}
	}

	//cleanup
	drawEnd();
}

/////////////////////////////////////////////////////////////////////////////
// normal drawing
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawNormal()
{
	//set drawing bits
	m_nGLDraw = GL_BIT_LIGHT;

	//set chip drawing if we are in the game and we are not playing with the
	//variant the hides chips
	if( (FALSE == m_pGame->m_bInGame) && 
		(IS_VARIANT(VARIANT_HIDE_CHIPS)))
	{
		m_nGLDraw |= GL_BIT_HIDE_CHIPS;
	}

	//reset the lighting
	resetLight();

	//draw base code
	drawBase();
}

/////////////////////////////////////////////////////////////////////////////
// player placement
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawPlayerPlace()
{
	CPlayerGame *pPlayer;
	GLMaterial mat;

	//get the current player game
	pPlayer = &(m_pGame->m_players[m_iPlayer]);

	//set drawing bits
	m_nGLDraw = GL_BIT_LIGHT | GL_BIT_INVERT_ROAD;

	//set the hex selection for roads if needed
	if(FALSE == m_bCity)
	{
		m_nGLDraw |= GL_BIT_HEX_FAN;
	}

	//check hide chips variant
	if(IS_VARIANT(VARIANT_HIDE_CHIPS))
	{
		m_nGLDraw |= GL_BIT_HIDE_CHIPS;
	}

	//reset the lighting
	resetLight();

	//draw
	drawBase();

	//set polygon mode to outline
	glDisable(GL_TEXTURE_2D);

	//set the player material
	playerColor(pPlayer, &mat);

	//initialize drawing for settlement or road placement circles/lines
	drawStart();

	//draw city placements
	if(TRUE == m_bCity)
	{
		drawInitialSettlePlace(&mat);
	}
	//draw road places
	else
	{
		drawInitialRoadPlace(&mat);
	}

	//cleanup
	drawEnd();

	//reenable textures
	glWhite.UseMaterial();
	glEnable(GL_TEXTURE_2D);
}

/////////////////////////////////////////////////////////////////////////////
// draw initial settlement places
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawInitialSettlePlace(GLMaterial *pMat)
{
	int i, j;
	int iSide, nSide2;
	int iName;
	double dX, dZ;
	double x, z;

	//actually draw all the possible selection places
	for(i = 0; i < m_nTiles; i++)
	{
		//skip non-land tiles and tiles that do not get inital placements
		if( (FALSE == VALID_LAND(m_pBoard[i].getType())) || 
			(FALSE == m_pBoard[i].getInitial()) ||
			//skip non home-island tiles if using trade routes as well
			((TRUE == MAP->m_bUsesTradeRoutes) && (FALSE == m_bIncrease) &&
			(m_pBoard[i].getIsland() != m_pGame->m_players[m_iPlayer].m_nHomeIsland)) ||
			//skip volcanoes
			(RES_VOLCANO == m_pBoard[i].getType())
			)
		{
			continue;
		}

		for(j = 0; j < 6; j++)
		{
			//get the side in question
			iSide = m_pBoard[i].getSide(j);
			nSide2 = m_pBoard[i].getSide((j + 5) % 6);

			//only draw circles on squares with a higher number
			if( ((iSide > i) || (FALSE == m_pBoard[iSide].getInitial())) && 
				(FALSE == cityCheck(i, j)) && 
				(RES_VOLCANO != m_pBoard[iSide].getType()) && 
				(RES_VOLCANO != m_pBoard[nSide2].getType())
				)
			{
				//calculate name
				iName = ENCODE_SEL(i, j);
				
				//load the name for selection
				glLoadName(iName + 1);

				//get the adjustment
				adjust(j, x, z);
				
				dX = m_pBoard[i].getCoord(0) + x;
				dZ = m_pBoard[i].getCoord(2) + z;

				disc(dX, 0.2, dZ, .6);

				//see if we are over it
				if(m_iSelection == iName)
				{
					glHighlight.UseMaterial();
					ring(dX, 0.2, dZ, 0.5, 0.6);
					pMat->UseMaterial();
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// draw initial road/ship places
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawInitialRoadPlace(GLMaterial *pMat)
{
	int i;
	int iSize;
	int iT1 = -1, iC1 = -1;
	int iTile1, iCorner;
	int iMinus;
	int iTile2, iTile3;
	double x = 0., z = 0.;
	BOOL b12, b13, b23;
	BOOL bGood = FALSE;
	BOOL bShip = FALSE;
	BOOL bValid;

	//deconstruct the last built city
	DECODE_SEL(m_iLastCity, iTile1, iCorner);

	//set the trailing value
	iMinus = (iCorner + 5) % 6;

	//get the nearest tiles
	iTile2 = m_pBoard[iTile1].getSide(iCorner);
	iTile3 = m_pBoard[iTile1].getSide(iMinus);

	//determine what we are looking up
	if(MAP->m_iMode && MAP_SEAFARERS)
	{
		bShip = (0 < m_iPossibleShips.size());
	}

	//see if we skip a placement due to two ocean tiles
	if(TRUE == bShip)
	{
		//set the array size
		iSize = m_iPossibleShips.size();
	}
	else
	{
		//set the array size
		iSize = m_iPossibleRoads.size();
	}

	//get invalid sides
	invalidPlaces(bShip, iTile1, iCorner, b12, b13, b23);

	//see if we have a highlight
	if(0 <= m_iSelection)
	{
		//get stuff
		DECODE_SEL(m_iSelection, iT1, iC1);

		//derotate
		m_iSelection = deRotate(iT1, iC1);

		for(i = 0; i < iSize; i++)
		{
			if((FALSE == bShip) && (m_iSelection == m_iPossibleRoads[i])) 
			{
				bGood = TRUE; 
				break;
			}
			else if(TRUE == bShip && (m_iSelection == m_iPossibleShips[i]))
			{
				bGood = TRUE;
				break;
			}
		}

		//if we didn't find it, reset the corner marker
		if(FALSE == bGood)
		{
			iC1 = -1;
		}
	}

	//set the line width
	glLineWidth(5.0);

	//set up translation
	adjust(iCorner, x, z);
	glTranslated(m_pBoard[iTile1].getCoord(0) + x, 0.00, m_pBoard[iTile1].getCoord(2) + z);

	//set the initial rotation
	glRotated(0.0 + (iCorner % 2 ? 60. : 0.), 0.0, 1.0, 0.0);

	//determine first line validity
	if(0 == iCorner || 1 == iCorner) bValid = b13;
	else if(2 == iCorner || 3 == iCorner) bValid = b12;
	else bValid = b23;

	//draw the three lines
	if(TRUE == bValid)
	{
		if((iC1 == (((iCorner % 2) + 2) % 6)) || (iC1 == (((iCorner % 2) + 5) % 6)))
		{
			glHighlight.UseMaterial();
		}

		glBegin(GL_LINES);
			glVertex3d(0., 0.1, 0.);
			glVertex3d(X, 0.1, 0.);
		glEnd();

		if(-1 != iC1)
		{
			pMat->UseMaterial();
		}
	}

	//determine second line validity
	if(0 == iCorner || 1 == iCorner) bValid = b23;
	else if(2 == iCorner || 3 == iCorner) bValid = b13;
	else bValid = b12;

	glRotated(120, 0.0, 1.0, 0.0);
	if(TRUE == bValid)
	{
		if((iC1 == (((iCorner % 2) + 1) % 6)) || (iC1 == (((iCorner % 2) + 4) % 6)))
		{
			glHighlight.UseMaterial();
		}

		glBegin(GL_LINES);
			glVertex3d(0., 0.1, 0.);
			glVertex3d(X, 0.1, 0.);
		glEnd();

		if(-1 != iC1)
		{
			pMat->UseMaterial();
		}
	}

	//determine third line validity
	if(0 == iCorner || 1 == iCorner) bValid = b12;
	else if(2 == iCorner || 3 == iCorner) bValid = b23;
	else bValid = b13;

	glRotated(120, 0.0, 1.0, 0.0);
	if(TRUE == bValid)
	{
		if((iC1 == (((iCorner % 2) + 0) % 6)) || (iC1 == (((iCorner % 2) + 3) % 6)))
		{
			glHighlight.UseMaterial();
		}

		glBegin(GL_LINES);
			glVertex3d(0., 0.1, 0.);
			glVertex3d(X, 0.1, 0.);
		glEnd();

		if(-1 != iC1)
		{
			pMat->UseMaterial();
		}
	}

	//reset line width
	glLineWidth(1.0);
}

/////////////////////////////////////////////////////////////////////////////
// draw normally, but outline the cities/settlements that just got set
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawGoods()
{
	int i;
	int iSize;
	int iTile;

	//set drawing bits
	m_nGLDraw = GL_BIT_OUTLINE_CITY | GL_BIT_DIM_BORDERS;

	//reset the lighting
	resetLight();

	//get tile size
	iSize = m_iLastTiles.size();

	//turn on goods squares
	for(i = 0; i < iSize; i++)
	{
		m_bLight[m_iLastTiles[i]] = TRUE;
	}

	//draw
	drawBase();

	//draw the gold outlines
	drawStart();

	for(i = 0; i < iSize; i++)
	{
		iTile = m_iLastTiles[i];
		drawTileOutline(iTile , (iTile != m_iRobber) ? &glGold : &glRed);
	}

	//finish
	drawEnd();
}

/////////////////////////////////////////////////////////////////////////////
// draw road placements
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawRoadPlace(void)
{
	int i;
	int ID, ID2;
	int iTile;
	int iCorner;
	int iSize;
	int iT1, iC1, iTemp;
	double x, z;
	double dRot;
	CPlayerGame *pPlayer;
	GLMaterial mat;
	static int save1 = 0, save2 = 0;

	//set drawing bits
	m_nGLDraw = GL_BIT_HEX_FAN | GL_BIT_DIM_BORDERS;

	//reset the lights
	resetLight();

	//get road list size
	iSize = m_iPossibleRoads.size();

	//now turn on hexes that can be displayed on
	for(i = 0; i < iSize; i++)
	{
		m_bLight[m_iPossibleRoads[i] / 10] = TRUE;
	}

	//draw
	drawBase();

	//set up colors
	glDisable(GL_TEXTURE_2D);

	//get the current player game
	pPlayer = &(m_pGame->m_players[m_iPlayer]);

	//set the material
	playerColor(pPlayer, &mat);

	//draw the possible line places
	drawStart();

	//set the line width
	glLineWidth(5.0);

	//see if we have a selection
	if(0 <= m_iSelection)
	{
		//get stuff
		DECODE_SEL(m_iSelection, iT1, iC1);

		//derotate
		m_iSelection = deRotate(iT1, iC1);
		iTemp = m_iSelection;

		//skip artifacts
		if(save1 == save2 && m_iSelection != save1)
		{
			m_iSelection = save1;
		}

		//adjust saves
		save2 = save1;
		save1 = iTemp;
	}

	//run through the list of road places and draw the lines
	for(i = 0; i < iSize; i += 2)
	{
		//deconstruct
		ID = m_iPossibleRoads[i];
		ID2 = m_iPossibleRoads[i + 1];

		DECODE_SEL(ID, iTile, iCorner);

		//set the rotation
		dRot = (iCorner - 2) * 60.;

		//set it
		if(ID == m_iSelection || ID2 == m_iSelection)
		{
			glHighlight.UseMaterial();
		}
		else
		{
			mat.UseMaterial();
		}

		glPushMatrix();
			//set up translation
			adjust(iCorner, x, z);
			glTranslated(m_pBoard[iTile].getCoord(0) + x, 0.00, m_pBoard[iTile].getCoord(2) + z);

			//rotate accurately
			glRotated(dRot, 0.0, 1.0, 0.0);

			//draw the line
			glBegin(GL_LINES);
				glVertex3d(0., 0.1, 0.);
				glVertex3d(X, 0.1, 0.);
			glEnd();
		glPopMatrix();
	}

	//finish drawing
	drawEnd();

	//reset line width
	glLineWidth(1.0);

	//reset the color
	glWhite.UseMaterial();
	
	//reenable texture
	glEnable(GL_TEXTURE_2D);
}

/////////////////////////////////////////////////////////////////////////////
// draw possible ship places
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawShipPlace()
{
	int i;
	int ID, ID2;
	int iTile;
	int iCorner;
	int iSize;
	int iT1, iC1, iTemp;
	double x, z;
	double dRot;
	CPlayerGame *pPlayer;
	GLMaterial mat;
	static int save1 = 0, save2 = 0;

	//set drawing bits
	m_nGLDraw = GL_BIT_HEX_FAN | GL_BIT_DIM_BORDERS;

	//reset the lighting
	resetLight();

	//get ship list size
	iSize = m_iPossibleShips.size();

	//now turn on hexes that can be displayed on
	for(i = 0; i < iSize; i++)
	{
		m_bLight[m_iPossibleShips[i] / 10] = TRUE;
	}

	//draw base
	drawBase();

	//set ship placement lines
	drawStart();

	//draw a nice red outline around the pirate
	if(-1 != m_iPirate)
	{
		drawTileOutline(m_iPirate, &glRed);
	}

	//set up colors
	glDisable(GL_TEXTURE_2D);

	//get the current player game
	pPlayer = &(m_pGame->m_players[m_iPlayer]);

	//create the material
	playerColor(pPlayer, &mat);

	//set the line width
	glLineWidth(5.0);

	//see if we have a selection
	if(0 <= m_iSelection)
	{
		//get stuff
		DECODE_SEL(m_iSelection, iT1, iC1);

		//derotate
		m_iSelection = deRotate(iT1, iC1);
		iTemp = m_iSelection;

		//skip artifacts
		if(save1 == save2 && m_iSelection != save1)
		{
			m_iSelection = save1;
		}

		//adjust saves
		save2 = save1;
		save1 = iTemp;
	}

	//run through the list of road places and draw the lines
	for(i = 0; i < iSize; i += 2)
	{
		//deconstruct
		ID = m_iPossibleShips[i];
		ID2 = m_iPossibleShips[i + 1];

		DECODE_SEL(ID, iTile, iCorner);

		//set the rotation
		dRot = (iCorner - 2) * 60.;

		//set it
		if(ID == m_iSelection || ID2 == m_iSelection)
		{
			glHighlight.UseMaterial();
		}
		else
		{
			mat.UseMaterial();
		}

		glPushMatrix();
			//set up translation
			adjust(iCorner, x, z);
			glTranslated(m_pBoard[iTile].getCoord(0) + x, 0.00, m_pBoard[iTile].getCoord(2) + z);

			//rotate accurately
			glRotated(dRot, 0.0, 1.0, 0.0);

			//draw the line
			glBegin(GL_LINES);
				glVertex3d(0., 0.1, 0.);
				glVertex3d(X, 0.1, 0.);
			glEnd();
		glPopMatrix();
	}

	//finish drawing
	drawEnd();

	//reset line width
	glLineWidth(1.0);

	//reset the color
	glWhite.UseMaterial();
	
	//reenable texture
	glEnable(GL_TEXTURE_2D);
}

/////////////////////////////////////////////////////////////////////////////
// draw possible port places
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawPortPlace()
{
	int i;
	int iSize;

	//set drawing bits
	m_nGLDraw = GL_BIT_DIM_BORDERS | GL_BIT_PORT_SELECT;

	//reset the lighting
	resetLight();
	resetTileSelect();

	//if we have no port tile yet
	if(-1 == m_nPortTile)
	{
		//get port list size
		iSize = m_iPossiblePorts.size();

		//turn on tiles that can be selected
		for(i = 0; i < iSize; i++)
		{
			m_bLight[m_iPossiblePorts[i]] = TRUE;
			m_bTileSelect[m_iPossiblePorts[i]] = TRUE;
		}
	}
	//we have a port tile, so we want the land tile it faces
	else
	{
		//get port list size
		iSize = m_iPossiblePortSides.size();

		//turn on tiles that can be selected
		for(i = 0; i < iSize; i++)
		{
			m_bLight[m_iPossiblePortSides[i]] = TRUE;
			m_bTileSelect[m_iPossiblePortSides[i]] = TRUE;
		}
	}
	
	//load base name
	glLoadName(0);

	//draw base
	drawBase();
}

/////////////////////////////////////////////////////////////////////////////
// draw possible chitless tile replacements
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawChitlessTile()
{
	int i;

	//set drawing bits
	m_nGLDraw = GL_BIT_DIM_BORDERS | GL_BIT_CHITLESS_SELECT;

	//reset the lighting
	resetLight();

	//turn on lighting for chitless tile
	m_bLight[m_nChitlessTile] = TRUE;

	//turn on lighting for selection options
	for(i = 0; i < m_nTiles; i++)
	{
		if(TRUE == m_bTileSelect[i])
		{
			m_bLight[i] = TRUE;
		}
	}

	//load base name
	glLoadName(0);

	//draw base
	drawBase();

	//finish up drawing
	drawStart();

	//draw the tile outline for the chitless tile
	drawTileOutline(m_nChitlessTile, &glRed);

	drawEnd();
}

/////////////////////////////////////////////////////////////////////////////
// draw possible city places
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawCityPlace()
{
	int i;
	int iSize;

	//set drawing bits
	m_nGLDraw = GL_BIT_OUTLINE_CITY | GL_BIT_INVERT_CITY | GL_BIT_INVERT_ROAD |
				GL_BIT_DIM_BORDERS;

	//reset the lighting
	resetLight();

	//get city list size
	iSize = m_iPossibleCities.size();

	//now turn on hexes that can be displayed on
	for(i = 0; i < iSize; i++)
	{
		m_bLight[m_iPossibleCities[i]] = TRUE;
	}

	//load a base name
	glLoadName(0);

	//draw it
	drawBase();
}

/////////////////////////////////////////////////////////////////////////////
// draw possible settlement places
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawSettlePlace(void)
{
	int i;
	int iSide1, iSide2, iSide3;
	int iCorner;
	int iSize;
	int iName;
	double x, z;
	double dX, dZ;
	CPlayerGame *pPlayer;
	GLMaterial mat;

	//set drawing bits
	m_nGLDraw = GL_BIT_DIM_BORDERS;

	//reset the lighting
	resetLight();

	//get settlement list size
	iSize = m_iPossibleSettlements.size();

	//set up lighting
	for(i = 0; i < iSize; i++)
	{
		DECODE_SEL(m_iPossibleSettlements[i], iSide1, iCorner);

		//get the other two sides
		iSide2 = m_pBoard[iSide1].getSide(iCorner);
		iSide3 = m_pBoard[iSide1].getSide((iCorner + 5) % 6);

		//turn on lights
		m_bLight[iSide1] = TRUE;
		m_bLight[iSide2] = TRUE;
		m_bLight[iSide3] = TRUE;
	}

	//draw
	drawBase();

	//now draw placement circles
	drawStart();

	//turn off textures briefly
	glDisable(GL_TEXTURE_2D);

	//get the current player game
	pPlayer = &(m_pGame->m_players[m_iPlayer]);

	//get the color
	playerColor(pPlayer, &mat);

	//now actually draw all the possible selection places
	for(i = 0; i < iSize; i++)
	{
		//decode the id
		DECODE_SEL(m_iPossibleSettlements[i], iSide1, iCorner);

		//calculate name
		iName = ENCODE_SEL(iSide1, iCorner);
		
		//load the name for selection
		glLoadName(iName + 1);

		//get the adjustment
		adjust(iCorner, x, z);

		//set coords
		dX = m_pBoard[iSide1].getCoord(0) + x;
		dZ = m_pBoard[iSide1].getCoord(2) + z;

		//draw the disc
		disc(dX, 0.2, dZ, .6);

		//see if we are over it
		if(m_iSelection == iName)
		{
			glHighlight.UseMaterial();
			ring(dX, 0.2, dZ, 0.5, 0.6);
			mat.UseMaterial();
		}
	}

	//cleanup
	drawEnd();

	//reenable textures
	glWhite.UseMaterial();
	glEnable(GL_TEXTURE_2D);
}

/////////////////////////////////////////////////////////////////////////////
// draw robber placement
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawRobberPlace()
{
	int i;

	//set drawing bits
	m_nGLDraw = GL_BIT_LIGHT | GL_BIT_DIM_BORDERS | GL_BIT_SEVEN_SELECT;

	//reset the lights
	resetLight();

	//turn off the light for the current robber place
	m_bLight[m_iRobber] = FALSE;

	//if in seafarers mode, the pirate tile is also off
	if(MAP->m_iMode & MAP_SEAFARERS)
	{
		if(-1 != m_iPirate)
		{
			m_bLight[m_iPirate] = FALSE;
		}

		//turn off hidden tiles
		for(i = 0; i < m_nTiles; i++)
		{
			if((RES_HIDDEN == m_pBoard[i].getType()) || (RES_DESERT == m_pBoard[i].getType()))
			{
				m_bLight[i] = FALSE;
			}
		}
	}
	//turn off sea tiles if not in seafarers mode
	else
	{
		for(i = 0; i < m_nTiles; i++)
		{
			if((TRUE == VALID_OCEAN(m_pBoard[i].getType())) || (RES_DESERT == m_pBoard[i].getType()))
			{
				m_bLight[i] = FALSE;
			}
		}
	}

	//load the first name
	glLoadName(0);

	//draw
	drawBase();
}

/////////////////////////////////////////////////////////////////////////////
// draw ships available for selection
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawShipChoose()
{
	int i;
	int iSize;

	//set drawing bits
	m_nGLDraw = GL_BIT_INVERT_CITY | GL_BIT_INVERT_ROAD | GL_BIT_MOVE_SHIP | 
				GL_BIT_DIM_BORDERS;

	//reset lighting
	resetLight();

	//get city list size
	iSize = m_iPossibleShipMoves.size();

	//now turn on hexes that can be displayed on
	for(i = 0; i < iSize; i++)
	{
		m_bLight[m_iPossibleShipMoves[i] / 10] = TRUE;
	}

	//load a base name
	glLoadName(0);

	//draw
	drawBase();
}

/////////////////////////////////////////////////////////////////////////////
// draw erupting volcanoes
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawVolcano()
{
	int i;

	//set drawing bits
	m_nGLDraw = GL_BIT_INVERT_CITY | GL_BIT_DIM_BORDERS | GL_BIT_VOLCANO;

	//reset lighting
	resetLight();

	//turn on volcano hexes
	for(i = 0; i < (int) m_nLastVolcano.size(); i++)
	{
		m_bLight[m_nLastVolcano[i]] = TRUE;
	}

	//draw
	drawBase();
}

/////////////////////////////////////////////////////////////////////////////
// draw an individual tile
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawTile(int iTile)
{
	int i;
	int iSide;
	int iType;
	BOOL bBorderTile;

	//get the tile type
	iType = m_pBoard[iTile].getType();

	//determine if this is a border tile
	bBorderTile = m_pBoard[iTile].getBorder();

	//see if this is a volcano tile
	if(GL_BIT_VOLCANO & m_nGLDraw)
	{
		//check for this tile
		for(i = 0; i < (int) m_nLastVolcano.size(); i++)
		{
			if(m_nLastVolcano[i] == iTile)
			{
				drawTileVolcano(iTile);
				return;
			}
		}
	}

	//set the base color
	m_bLight[iTile] ? glWhite.UseMaterial() : glGray.UseMaterial();

	//if it's a border, dim it if that's turned on
	if((TRUE == bBorderTile) && (GL_BIT_DIM_BORDERS & m_nGLDraw) &&
		(FALSE == (GL_BIT_PORT_SELECT & m_nGLDraw)))
	{
		glGray.UseMaterial();
	}

	//load the name for selection
	if(GL_BIT_SEVEN_SELECT & m_nGLDraw)
	{
		if(((FALSE == (MAP->m_iMode & MAP_SEAFARERS)) && (TRUE == VALID_OCEAN(m_pBoard[iTile].getType()))) ||
			(RES_HIDDEN == m_pBoard[iTile].getType()) || 
			(RES_DESERT == m_pBoard[iTile].getType()) ||
			(TRUE == m_pBoard[iTile].getBorder()))
		{
			glLoadName(0);
		}
		else
		{
			glLoadName(iTile + 1);
		}

		//draw tile selection for mouse overs
		if((m_iSelection == iTile) && (iTile != m_iRobber) && (iTile != m_iPirate))
		{
			drawTileOutline(iTile, &glHighlight);
		}
	}

	//load the name for selection
	if( (GL_BIT_PORT_SELECT & m_nGLDraw) || 
		(GL_BIT_CHITLESS_SELECT & m_nGLDraw))
	{
		if(TRUE == m_bTileSelect[iTile])
		{
			glLoadName(iTile + 1);
		}
		else
		{
			glLoadName(0);
		}

		//draw tile selection for mouseovers
		if(m_iSelection == iTile)
		{
			drawTileOutline(iTile, &glHighlight);
		}
	}

	//draw the sea port if it has one
	if((MAP->m_iMode & MAP_SEAFARERS) && (TRUE == VALID_PORT(iType)) && (TRUE == m_bSeaTiles))
	{
		//set the texture
		glBindTexture(GL_TEXTURE_2D, m_texSeaPorts[iType - GL_SEAPORT_OFFSET]);
		seaport(m_pBoard[iTile].getCoord(0), 0., m_pBoard[iTile].getCoord(2), m_pBoard[iTile].getRotation());

		//set the texture
		glBindTexture(GL_TEXTURE_2D, m_texBase[RES_OCEAN]);
	}
	else
	{
		//set the texture
		glBindTexture(GL_TEXTURE_2D, m_texBase[iType]);
	}
	
	//draw it
	if(GL_BIT_HEX_FAN & m_nGLDraw)
	{
		//draw the fan
		hexagonfan(m_pBoard[iTile].getCoord(0), 0., m_pBoard[iTile].getCoord(2), m_pBoard[iTile].getRotation(), iTile);
	}
	else
	{
		//draw the hexagon
		hexagon(m_pBoard[iTile].getCoord(0), 0., m_pBoard[iTile].getCoord(2), m_pBoard[iTile].getRotation());
	}

	//draw outside edges
	if(TRUE == bBorderTile)
	{
		//turn off textures
		glDisable(GL_TEXTURE_2D);

		//run through each side and look for edges
		for(i = 0; i < 6; i++)
		{
			//get the side
			iSide = m_pBoard[iTile].getSide(i);
			
			//check it
			if(iSide == -1)
			{
				//draw it
				glEdge.UseMaterial();
				drawTileEdge(iTile, i);

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBlack.UseMaterial();
				drawTileEdge(iTile, i);

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}

		//turn on textures
		glEnable(GL_TEXTURE_2D);
	}

	//reset base color
	glWhite.UseMaterial();
}

/////////////////////////////////////////////////////////////////////////////
// outline a specified tile
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawTileOutline(int iTile, GLMaterial *mat, int iWidth)
{
	//set polygon mode to outline
	glDisable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth((GLfloat) iWidth);

	//use the sent in color
	mat->UseMaterial();

	//draw the outline
	hexagonoutline(m_pBoard[iTile].getCoord(0), 0.1, m_pBoard[iTile].getCoord(2));

	//reset
	glWhite.UseMaterial();
	glLineWidth(1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
}

/////////////////////////////////////////////////////////////////////////////
// draw a chip for a tile
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawChip(int iTile)
{
	int i;
	int iSide = 0;
	GLuint *pTex = NULL;
	double dX, dZ;

	//see if chip drawing is turned on
	if(GL_BIT_HIDE_CHIPS & m_nGLDraw)
	{
		return;
	}

	//see if we're good and if this is a chitless tile
	if( (FALSE == VALID_RESOURCE(m_pBoard[iTile].getType())) ||
		(TRUE == m_pBoard[iTile].getChitless()))
	{
		return;
	}
	
	//see if this chit is on an active volcano
	if(GL_BIT_VOLCANO & m_nGLDraw)
	{
		for(i = 0; i < (int) m_nLastVolcano.size(); i++)
		{
			if(m_nLastVolcano[i] == iTile)
			{
				return;
			}
		}
	}

	//set the base color
	m_bLight[iTile] ? glWhite.UseMaterial() : glGray.UseMaterial();

	//set the texture array
	switch(m_pBoard[iTile].getRollTexType())
	{
	case MAP_USES_34_CHIPS: pTex = &m_texChip34[0]; iSide = GL_TEX_CHIP34 - 1; break;
	case MAP_USES_56_CHIPS: pTex = &m_texChip56[0]; iSide = GL_TEX_CHIP56 - 1; break;
	case MAP_USES_SEA_CHIPS: pTex = &m_texChipSea[0]; iSide = GL_TEX_CHIP34 - 1; break;
	}

	//get coords
	dX = m_pBoard[iTile].getCoord(0);
	dZ = m_pBoard[iTile].getCoord(2);

	//set the chip texture
	glBindTexture(GL_TEXTURE_2D, pTex[m_pBoard[iTile].getRollTex()]);
	disc(dX, 0.1, dZ);

	//draw cylinder if detail is high enough
	if(GL_LOW_DETAIL <= m_iDetail)
	{
		glBindTexture(GL_TEXTURE_2D, m_texChip34[iSide]);
		cylinder(dX, 0.1, dZ);
	}

	//reset base color
	glWhite.UseMaterial();
}

/////////////////////////////////////////////////////////////////////////////
// run through the list and draw settlements and cities
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawCities(int iTile)
{
	int i, j;
	int ID;
	int iExtra;
	int iCode;
	int iSide1, iSide2;
	int iType1, iType2;
	int iSettle, iPlayer;
	double dX, dZ;
	double x, y, z;
	BOOL bDark;
	BOOL bWhiteOutline;
	BOOL bLine = FALSE;
	CPlayerGame *pPlayer;

	//see if we're within bounds
	if(TRUE == VALID_OCEAN(m_pBoard[iTile].getType()))
	{
		return;
	}

	//set polygon mode to outline
	if(GL_HIGH_DETAIL > m_iDetail)
	{
		glDisable(GL_TEXTURE_2D);
	}

	//search for cities
	for(i = 0; i < 6; i++)
	{
		GLMaterial mat;

		//get the code
		iCode = m_pBoard[iTile].getCity(i);

		//see if there's even a city/settlement here
		if(0 == iCode) 
		{
			continue;
		}

		//reset the line and dark variables
		bLine = FALSE;

		//deconstruct the city code
		DECODE_SEL(iCode, iSettle, iPlayer);

		iSide1 = m_pBoard[iTile].getSide(i);
		iSide2 = m_pBoard[iTile].getSide((i + 5) % 6);
		iType1 = m_pBoard[iSide1].getType();
		iType2 = m_pBoard[iSide2].getType();

		//we only want to draw it once, thereofore draw on the lowest numbered
		//land tile only
		if( ((iSide1 < iTile) && (TRUE == VALID_LAND(iType1))) || 
			((iSide2 < iTile) && (TRUE == VALID_LAND(iType2))))
		{
			continue;
		}

		//get extra points on this corner
		iExtra = m_pBoard[iTile].getExtra(i);

		//get tile coords
		dX = m_pBoard[iTile].getCoord(0);
		dZ = m_pBoard[iTile].getCoord(2);

		//get the current player game
		pPlayer = &(m_pGame->m_players[iPlayer]);

		//set the white outline flag
		bWhiteOutline = (m_bWhiteOutline && (CR_WHITE == pPlayer->m_iColor));

		//create the material
		mat.SetMaterial(COLOR_BLACK);
		mat.SetAmbient(COLORS->getDrawColor(pPlayer->m_iColor));
		
		//set dynamic lighting, if enabled
		if(GL_LIGHT_DETAIL <= m_iDetail)
		{
			mat.MultAmbient(.65);
			mat.SetDiffuse(COLORS->getDrawColor(pPlayer->m_iColor));
			mat.MultDiffuse(.50);
		}
		
		//see if it should be dark
		if(GL_BIT_INVERT_CITY & m_nGLDraw)
		{
			bDark = !m_bLight[iTile] || !m_bLight[iSide1] || !m_bLight[iSide2] || m_iPlayer != iPlayer;
		}
		else
		{
			bDark = !m_bLight[iTile] && !m_bLight[iSide1] && !m_bLight[iSide2];
		}

		//darken if if it's not on a lighted tile
		if(bDark)
		{
			mat.MultAmbient(.5);
		}

		//set the height adjustment for extra points
		y = iExtra * EXTRA_DEPTH;

		//get the corner adjustment
		adjust(i, x, z);

		//set tiles to white
		glWhite.UseMaterial();

		//draw extra points
		for(j = 0; j < iExtra; j++)
		{
			extra(dX + x, j * EXTRA_DEPTH, dZ + z, 0.0);
		}

		//use the material
		mat.UseMaterial();

		//create the city ID
		ID = ENCODE_CITY(iTile, iSide1, iSide2, i);

		//now actually draw it
		if(iSettle == 1)
		{
			//load the name
			if((GL_BIT_INVERT_CITY & m_nGLDraw) && !bDark && iPlayer == m_iPlayer) 
			{
				glLoadName(ID + 1);
			}

			settlement(dX + x, y, dZ + z);
		}
		else
		{
			city(dX + x, y, dZ + z);
		}

		//draw the outline if necessary
		if( (GL_BIT_OUTLINE_CITY & m_nGLDraw) && !bDark)
		{
			if(m_iSelection == ID)
			{
				glHighlight.UseMaterial();
			}
			else
			{
				mat.SetMaterial(COLOR_GOLDOUTLINE);
				mat.SetEmission(COLOR_GOLDOUTLINE);
				mat.UseMaterial();
			}
			glLineWidth(1.0);
		}
		else
		{
			//if we're in high detail mode, skip the outline
			if((GL_HIGH_DETAIL <= m_iDetail) && (FALSE == bWhiteOutline))
			{
				continue;
			}

			if(TRUE == bWhiteOutline)
			{
				glWhiteOutline.UseMaterial();
				glLineWidth(1.2f);
			}
			else
			{
				mat.MultAmbient(GL_AMB_MULT);
				mat.UseMaterial();
			}
		}

		//set outline mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		//draw the settlement
		if(iSettle == 1)
		{
			settlement(dX + x, y, dZ + z);
		}
		else
		{
			city(dX + x, y, dZ + z);
		}
		
		//reset
		glLineWidth(1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glWhite.UseMaterial();

	//set polygon mode to outline
	if(GL_HIGH_DETAIL > m_iDetail)
	{
		glEnable(GL_TEXTURE_2D);
	}
}

/////////////////////////////////////////////////////////////////////////////
// draw all roads on the map
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawRoads(int iTile)
{
	int i, j;
	int iPlayer, iSide;
	int iComp;
	BOOL bDark;
	BOOL bLongest;
	BOOL bShip = FALSE;
	BOOL bMovable = FALSE;
	BOOL bWhiteOutline;
	double dRot;
	double dX, dZ;
	double x, z;
	GLMaterial mat;
	CPlayerGame *pPlayer;

	//set polygon mode to outline
	if(GL_HIGH_DETAIL > m_iDetail)
	{
		glDisable(GL_TEXTURE_2D);
	}

	//search for roads
	for(i = 0; i < 6; i++)
	{
		//get the player
		bMovable = FALSE;
		bShip = FALSE;
		iPlayer = m_pBoard[iTile].getRoad(i);

		//look for a ship if there's no road there
		if(-1 == iPlayer)
		{
			iPlayer = m_pBoard[iTile].getShip(i);
			bShip = TRUE;
		}

		//check to see if there's a road
		if(iPlayer == -1) 
		{
			continue;
		}

		//get the other side
		iSide = m_pBoard[iTile].getSide(i);

		//check for continue
		if(iSide < iTile) 
		{
			continue;
		}

		//get the player
		pPlayer = &(m_pGame->m_players[iPlayer]);

		//see if this is a white outline player
		bWhiteOutline = (m_bWhiteOutline && (CR_WHITE == pPlayer->m_iColor));

		//create the material
		mat.SetMaterial(COLOR_BLACK);
		mat.SetAmbient(COLORS->getDrawColor(pPlayer->m_iColor));
		
		//set dynamic lighting, if enabled
		if(GL_LIGHT_DETAIL <= m_iDetail)
		{
			mat.MultAmbient(.65);
			mat.SetDiffuse(COLORS->getDrawColor(pPlayer->m_iColor));
			mat.MultDiffuse(.50);
		}

		//default to not being part of longest road
		bLongest = FALSE;

		//set the dark variable
		if(GL_BIT_INVERT_ROAD & m_nGLDraw)
		{
			bDark = !m_bLight[iTile] || !m_bLight[iSide];
		}
		else
		{
			bDark = !m_bLight[iTile] && !m_bLight[iSide];
		}
		
		//darken if necessary
		if(bDark) 
		{
			mat.MultAmbient(.5);
		}

		//set the material
		mat.UseMaterial();

		//set the rotation correctly
		dRot = -30. + (60. * (i % 3));

		//now actually draw it
		adjustroad(i, x, z);

		//get the coordinates
		dX = m_pBoard[iTile].getCoord(0);
		dZ = m_pBoard[iTile].getCoord(2);

		//check for longest road or movable ships
		iComp = ENCODE_SEL(iTile, i);

		//check for movable ships
		if(GL_BIT_MOVE_SHIP & m_nGLDraw)
		{
			for(j = 0; j < (int) m_iPossibleShipMoves.size(); j++)
			{
				if(m_iPossibleShipMoves[j] == iComp)
				{
					bMovable = TRUE;
					break;
				}
			}
		}

		//draw the road
		if(FALSE == bShip)
		{
			road(dX + x, 0.0, dZ + z, dRot);
		}
		else
		{
			//load the selection name if it's movable
			if(TRUE == bMovable)
			{
				glLoadName(iComp + 1);
			}
			ship(dX + x, 0.0, dZ + z, dRot);
		}
		
		//if it's movable, draw the outline
		if(TRUE == bMovable)
		{
			//see if we're hovering over it
			if(m_iSelection == iComp)
			{
				glHighlight.UseMaterial();
			}
			else
			{
				glGold.UseMaterial();
			}
			shipoutline(dX + x, 0.0, dZ + z, dRot);
			mat.UseMaterial();
			glLoadName(0);
		}

		//search for a part of longest road (but not when we're moving ships)
		if((5 <= m_iLongestLength) && (FALSE == (GL_BIT_MOVE_SHIP & m_nGLDraw)))
		{
			for(j = 0; j < (int) m_iLongestRoad.size(); j++)
			{
				if(iComp == m_iLongestRoad[j])
				{
					bLongest = TRUE;
					break;
				}
			}
		}

		//draw the outline
		if(FALSE == bLongest)
		{
			//continue if needed
			if((GL_HIGH_DETAIL <= m_iDetail) && ((FALSE == bWhiteOutline) || (TRUE == bMovable)))
			{
				continue;
			}

			if((TRUE == bWhiteOutline) && (FALSE == bMovable))
			{
				glWhiteOutline.UseMaterial();
				glLineWidth(1.2f);
			}
			else
			{
				mat.MultAmbient(GL_AMB_MULT);
				mat.UseMaterial();
			}
		}
		else
		{
			glLongestOutline.UseMaterial();
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		//draw the road outline
		if(FALSE == bShip)
		{
			road(dX + x, 0.0, dZ + z, dRot);
		}
		else
		{
			shipoutline(dX + x, 0.0, dZ + z, dRot);
		}
		
		//reset
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//reset the color
	glWhite.UseMaterial();
	glLineWidth(1.0);

	//reset textures
	if(GL_HIGH_DETAIL > m_iDetail)
	{
		glEnable(GL_TEXTURE_2D);
	}
}

/////////////////////////////////////////////////////////////////////////////
// drawt the robber
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawRobber(void)
{
	BOOL bWire;
	double dX, dZ;

	//get tile coords
	dX = m_pBoard[m_iRobber].getCoord(0);
	dZ = m_pBoard[m_iRobber].getCoord(2);

	//offset
	dX += -2.0;
	dZ += -2.0;
	
	GLfloat amb[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat dif[] = {0.25f, 0.25f, 0.25f, 0.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);

	bWire = GetPrivateProfileInt(INI_OPENGL, INI_WIREFRAME, 0, INI_FILE);
	
	//set to wireframe if desired
	if(TRUE == bWire)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//turn off textures
	glDisable(GL_TEXTURE_2D);

	//draw it
	robber(dX, 0., dZ);

	//turn textures back on
	glEnable(GL_TEXTURE_2D);

	//set back to fill
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//draw the pirate
	if(-1 != m_iPirate)
	{
		//get tile coords
		dX = m_pBoard[m_iPirate].getCoord(0);
		dZ = m_pBoard[m_iPirate].getCoord(2);

		//offset it if it's a seaport
		if(TRUE == VALID_PORT(m_pBoard[m_iPirate].getType()))
		{
			dX += 2.0;
			dZ += -2.0;
		}

		//draw it
		ship(dX, 0.0, dZ, 0.0);
	}

	glWhite.UseMaterial();
}

/////////////////////////////////////////////////////////////////////////////
// tile edge code
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawTileEdge(int iTile, int iSide)
{
	double x, z;
	double dRot = 0.;

	//get the offsets
	adjustroad(iSide, x, z);

	//get the rotation
	if(iSide == 1 || iSide == 4) dRot = 120.;
	else if(iSide == 0 || iSide == 3) dRot = 60.;

	glPushMatrix();
		//set the coordinates
		glTranslated(m_pBoard[iTile].getCoord(0) + x, 0.0, m_pBoard[iTile].getCoord(2) + z);
		glRotated(dRot, 0.0, 1.0, 0.0);

		//call it
		glCallList(m_lists[GL_TILE_EDGE]);

	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// draw a volcano outline simulating lave pouring down! =p
/////////////////////////////////////////////////////////////////////////////
void CGLView::drawTileVolcano(int nTile)
{
	double dRot;
	double dX, dZ;
	double deltaR, deltaG, deltaB;
	GLMaterial glCurrent;
	GLMaterial glLast;
	GLMaterial glMiddle;

	//get the tile rotation and coords
	dRot = m_pBoard[nTile].getRotation();
	dX = m_pBoard[nTile].getCoord(0);
	dZ = m_pBoard[nTile].getCoord(2);

	//prepare colors
	glLast.SetMaterial(COLOR_BLACK);
	glLast.SetEmission(COLOR_WHITE);
	glCurrent = glLast;
	glMiddle = glLast;

	//calculate delta colors
	deltaR = (glLava.GetRe() - glLast.GetRe()) / 3.0;
	deltaG = (glLava.GetGe() - glLast.GetGe()) / 3.0;
	deltaB = (glLava.GetBe() - glLast.GetBe()) / 3.0;

	//calculate middle color
	glMiddle.SetRe(glLast.GetRe() + (glLava.GetRe() - glLast.GetRe() / 2.0));
	glMiddle.SetGe(glLast.GetGe() + (glLava.GetGe() - glLast.GetGe() / 2.0));
	glMiddle.SetBe(glLast.GetBe() + (glLava.GetBe() - glLast.GetBe() / 2.0));

	//set current
	glCurrent.SetRe(glCurrent.GetRe() + deltaR);
	glCurrent.SetGe(glCurrent.GetGe() + deltaG);
	glCurrent.SetBe(glCurrent.GetBe() + deltaB);

	//disable textures
	glDisable(GL_TEXTURE_2D);

	//draw special outline
	glPushMatrix();
		glTranslated(dX, 0.0, dZ);
		glRotated((dRot - 60.) + 60.0 * m_nVolcanoRoll, 0.0, 1.0, 0.0);

		//initial lines
		glBegin(GL_TRIANGLES);
			glNormal3d(0.0f, 1.0f, 0.0f);
			
			//first pairs
			glLast.UseMaterial();
			glVertex3d(X, 0.0, 0.0);
			glCurrent.UseMaterial();
			glVertex3d((X / 2), 0.0, -Z);
			glMiddle.UseMaterial();
			glVertex3d(0.0, 0.0, 0.0);

			glLast.UseMaterial();
			glVertex3d(X, 0.0, 0.0);
			glCurrent.UseMaterial();
			glVertex3d((X / 2), 0.0, Z);
			glMiddle.UseMaterial();
			glVertex3d(0.0, 0.0, 0.0);

			//increment
			glLast = glCurrent;
			glCurrent.SetRe(glCurrent.GetRe() + deltaR);
			glCurrent.SetGe(glCurrent.GetGe() + deltaG);
			glCurrent.SetBe(glCurrent.GetBe() + deltaB);

			//second pairs
			glLast.UseMaterial();
			glVertex3d((X / 2), 0.0, -Z);
			glCurrent.UseMaterial();
			glVertex3d(-(X / 2), 0.0, -Z);
			glMiddle.UseMaterial();
			glVertex3d(0.0, 0.0, 0.0);

			glLast.UseMaterial();
			glVertex3d((X / 2), 0.0, Z);
			glCurrent.UseMaterial();
			glVertex3d(-(X / 2), 0.0, Z);
			glMiddle.UseMaterial();
			glVertex3d(0.0, 0.0, 0.0);

			//increment
			glLast = glCurrent;
			glCurrent.SetRe(glCurrent.GetRe() + deltaR);
			glCurrent.SetGe(glCurrent.GetGe() + deltaG);
			glCurrent.SetBe(glCurrent.GetBe() + deltaB);

			//final pairs
			glLast.UseMaterial();
			glVertex3d(-(X / 2), 0.0, -Z);
			glCurrent.UseMaterial();
			glVertex3d(-X, 0.0, 0.0);
			glMiddle.UseMaterial();
			glVertex3d(0.0, 0.0, 0.0);
			
			glLast.UseMaterial();
			glVertex3d(-(X / 2), 0.0, Z);
			glCurrent.UseMaterial();
			glVertex3d(-X, 0.0, 0.0);
			glMiddle.UseMaterial();
			glVertex3d(0.0, 0.0, 0.0);
		glEnd();		
	glPopMatrix();

	//reset the color
	glWhite.UseMaterial();
	
	//reenable texture
	glEnable(GL_TEXTURE_2D);

}

/////////////////////////////////////////////////////////////////////////////
// fill a material with a player's color
/////////////////////////////////////////////////////////////////////////////
void CGLView::playerColor(CPlayerGame *pPlayer, GLMaterial *pMat)
{
	//create the material
	pMat->SetMaterial(COLOR_BLACK);
	pMat->SetAmbient(COLORS->getDrawColor(pPlayer->m_iColor));
	
	//set dynamic lighting, if enabled
	if(GL_LIGHT_DETAIL <= m_iDetail)
	{
		pMat->MultAmbient(.65);
		pMat->SetDiffuse(COLORS->getDrawColor(pPlayer->m_iColor));
		pMat->MultDiffuse(.50);
	}

	//use it
	pMat->UseMaterial();
}