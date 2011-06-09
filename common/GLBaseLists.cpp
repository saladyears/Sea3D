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
#ifdef MAPS3D
#include "Maps3D.h"
#else
#include "settlers.h"
#endif
#include "GLBaseView.h"

/////////////////////////////////////////////////////////////////////////////
// create all the display lists
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::buildLists()
{
	int i;
	
	//create display lists
	for(i = 0; i < GL_DISPLAY_LISTS; i++)
	{
		m_lists[i] = glGenLists(1);
	}

	//tiles
	buildTiles();

	//settlements
	buildSettlement();

	//cities
	buildCity();

	//roads
	buildRoad();

	//build ship
	buildShip();
	buildShipOutline();

	//build robber
	buildRobber();

	//build seaport
	buildSeaport();

	//build sea borders
	buildBorders();

	//extra points
	buildExtra();
}

/////////////////////////////////////////////////////////////////////////////
// hexagon tiles
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::buildTiles()
{
	//build the hexagon
	glNewList(m_lists[GL_HEX_POLYGON], GL_COMPILE);
		glBegin(GL_POLYGON);
			glNormal3d(0.0f, 1.0f, 0.0f);
			glTexCoord2d(1.00, 0.50); glVertex3d(X, 0.0, 0.0);
			glTexCoord2d(0.75, 1.00); glVertex3d((X / 2), 0.0, -Z);
			glTexCoord2d(0.25, 1.00); glVertex3d(-(X / 2), 0.0, -Z);
			glTexCoord2d(0.00, 0.50); glVertex3d(-X, 0.0, 0.0);
			glTexCoord2d(0.25, 0.00); glVertex3d(-(X / 2), 0.0, Z);
			glTexCoord2d(0.75, 0.00); glVertex3d((X / 2), 0.0, Z);
			glTexCoord2d(1.00, 0.50); glVertex3d(X, 0.0, 0.0);
		glEnd();
	glEndList();

	//a tile edge along the board
	glNewList(m_lists[GL_TILE_EDGE], GL_COMPILE);
		glBegin(GL_QUADS);
			glNormal3d(0.0f, 0.0f, 1.0f);
			glVertex3d(-(X / 2), 0.0, 0.0);
			glVertex3d((X / 2), 0.0, 0.0);
			glVertex3d((X / 2), -TILE_DEPTH, 0.0);
			glVertex3d(-(X / 2), -TILE_DEPTH, 0.0);
		glEnd();
	glEndList();

	//hexagon outline for selection
	glNewList(m_lists[GL_HEX_OUTLINE], GL_COMPILE);
		glBegin(GL_LINES);
			glNormal3d(0.0f, 1.0f, 0.0f);
			glVertex3d(X, 0.0, 0.0);
			glVertex3d((X / 2), 0.0, -Z);
			glVertex3d((X / 2), 0.0, -Z);
			glVertex3d(-(X / 2), 0.0, -Z);
			glVertex3d(-(X / 2), 0.0, -Z);
			glVertex3d(-X, 0.0, 0.0);
			glVertex3d(-X, 0.0, 0.0);
			glVertex3d(-(X / 2), 0.0, Z);
			glVertex3d(-(X / 2), 0.0, Z);
			glVertex3d((X / 2), 0.0, Z);
			glVertex3d((X / 2), 0.0, Z);
			glVertex3d(X, 0.0, 0.0);
		glEnd();
	glEndList();

	//trangles for easier click detection
	glNewList(m_lists[GL_HEX_TRIANGLE1], GL_COMPILE);
		glBegin(GL_TRIANGLES);
			glNormal3d(0.0f, 1.0f, 0.0f);
			glTexCoord2d(0.50, 0.50); glVertex3d(0.0, 0.0, 0.0);
			glTexCoord2d(0.25, 0.00); glVertex3d(-(X / 2), 0.0, Z);
			glTexCoord2d(0.00, 0.50); glVertex3d(-X, 0.0, 0.0);
		glEnd();
	glEndList();

	glNewList(m_lists[GL_HEX_TRIANGLE2], GL_COMPILE);
		glBegin(GL_TRIANGLES);
			glNormal3d(0.0f, 1.0f, 0.0f);
			glTexCoord2d(0.50, 0.50); glVertex3d(0.0, 0.0, 0.0);
			glTexCoord2d(0.00, 0.50); glVertex3d(-X, 0.0, 0.0);
			glTexCoord2d(0.25, 1.00); glVertex3d(-(X / 2), 0.0, -Z);
		glEnd();
	glEndList();

	glNewList(m_lists[GL_HEX_TRIANGLE3], GL_COMPILE);
		glBegin(GL_TRIANGLES);
			glNormal3d(0.0f, 1.0f, 0.0f);
			glTexCoord2d(0.50, 0.50); glVertex3d(0.0, 0.0, 0.0);
			glTexCoord2d(0.25, 1.00); glVertex3d(-(X / 2), 0.0, -Z);
			glTexCoord2d(0.75, 1.00); glVertex3d((X / 2), 0.0, -Z);
		glEnd();
	glEndList();

	glNewList(m_lists[GL_HEX_TRIANGLE4], GL_COMPILE);
		glBegin(GL_TRIANGLES);
			glNormal3d(0.0f, 1.0f, 0.0f);
			glTexCoord2d(0.50, 0.50); glVertex3d(0.0, 0.0, 0.0);
			glTexCoord2d(0.75, 1.00); glVertex3d((X / 2), 0.0, -Z);
			glTexCoord2d(1.00, 0.50); glVertex3d(X, 0.0, 0.0);
		glEnd();
	glEndList();

	glNewList(m_lists[GL_HEX_TRIANGLE5], GL_COMPILE);
		glBegin(GL_TRIANGLES);
			glNormal3d(0.0f, 1.0f, 0.0f);
			glTexCoord2d(0.50, 0.50); glVertex3d(0.0, 0.0, 0.0);
			glTexCoord2d(1.00, 0.50); glVertex3d(X, 0.0, 0.0);
			glTexCoord2d(0.75, 0.00); glVertex3d((X / 2), 0.0, Z);
		glEnd();
	glEndList();

	glNewList(m_lists[GL_HEX_TRIANGLE6], GL_COMPILE);
		glBegin(GL_TRIANGLES);
			glNormal3d(0.0f, 1.0f, 0.0f);
			glTexCoord2d(0.50, 0.50); glVertex3d(0.0, 0.0, 0.0);
			glTexCoord2d(0.75, 0.00); glVertex3d((X / 2), 0.0, Z);
			glTexCoord2d(0.25, 0.00); glVertex3d(-(X / 2), 0.0, Z);
		glEnd();
	glEndList();
}

/////////////////////////////////////////////////////////////////////////////
// settlements
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::buildSettlement()
{
	//build the settlement
	glNewList(m_lists[GL_SETTLEMENT], GL_COMPILE);
		//front face
		glBegin(GL_POLYGON);
			glNormal3f(0.0, 0.0, 1.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(SWID / 2, 0.0, SLEN / 2);
			glTexCoord2d(1.00, 0.00); glVertex3d(-SWID / 2, 0.0, SLEN / 2);
			glTexCoord2d(1.00, 0.58); glVertex3d(-SWID / 2, SHIGH, SLEN / 2);
			glTexCoord2d(0.50, 0.98); glVertex3d(0.0, SPEAK, SLEN / 2);
			glTexCoord2d(0.00, 0.58); glVertex3d(SWID / 2, SHIGH, SLEN / 2);
			glTexCoord2d(0.00, 0.00); glVertex3d(SWID / 2, 0.0, SLEN / 2);
		glEnd();
		//back face
		glBegin(GL_POLYGON);
			glNormal3f(0.0, 0.0, -1.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(SWID / 2, 0.0, -SLEN / 2);
			glTexCoord2d(1.00, 0.00); glVertex3d(-SWID / 2, 0.0, -SLEN / 2);
			glTexCoord2d(1.00, 0.58); glVertex3d(-SWID / 2, SHIGH, -SLEN / 2);
			glTexCoord2d(0.50, 0.98); glVertex3d(0.0, SPEAK, -SLEN / 2);
			glTexCoord2d(0.00, 0.58); glVertex3d(SWID / 2, SHIGH, -SLEN / 2);
			glTexCoord2d(0.00, 0.00); glVertex3d(SWID / 2, 0.0, -SLEN / 2);
		glEnd();
	glEndList();

	glNewList(m_lists[GL_SETTLEMENTA], GL_COMPILE);
		//right side face
		glBegin(GL_POLYGON);
			glNormal3f(1.0, 0.0, 0.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(SWID / 2, 0.0, SLEN / 2);
			glTexCoord2d(0.00, 1.00); glVertex3d(SWID / 2, 0.0, -SLEN / 2);
			glTexCoord2d(1.00, 1.00); glVertex3d(SWID / 2, SHIGH, -SLEN / 2);
			glTexCoord2d(1.00, 0.00); glVertex3d(SWID / 2, SHIGH, SLEN / 2);
			glTexCoord2d(0.00, 0.00); glVertex3d(SWID / 2, 0.0, SLEN / 2);
		glEnd();
		//left side face
		glBegin(GL_POLYGON);
			glNormal3f(-1.0, 0.0, 0.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(-SWID / 2, 0.0, SLEN / 2);
			glTexCoord2d(0.00, 1.00); glVertex3d(-SWID / 2, 0.0, -SLEN / 2);
			glTexCoord2d(1.00, 1.00); glVertex3d(-SWID / 2, SHIGH, -SLEN / 2);
			glTexCoord2d(1.00, 0.00); glVertex3d(-SWID / 2, SHIGH, SLEN / 2);
			glTexCoord2d(0.00, 0.00); glVertex3d(-SWID / 2, 0.0, SLEN / 2);
		glEnd();
		//top right side
		glBegin(GL_POLYGON);
			glNormal3f(1.0, 1.0, 0.0);
			glTexCoord2d(1.00, 1.00); glVertex3d(SWID / 2, SHIGH, SLEN / 2);
			glTexCoord2d(0.00, 1.00); glVertex3d(0.0, SPEAK, SLEN / 2);
			glTexCoord2d(0.00, 0.00); glVertex3d(0.0, SPEAK, -SLEN / 2);
			glTexCoord2d(1.00, 0.00); glVertex3d(SWID / 2, SHIGH, -SLEN / 2);
			glTexCoord2d(1.00, 1.00); glVertex3d(SWID / 2, SHIGH, SLEN / 2);
		glEnd();
		//top left side
		glBegin(GL_POLYGON);
			glNormal3f(-1.0, 1.0, 0.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(-SWID / 2, SHIGH, SLEN / 2);
			glTexCoord2d(1.00, 0.00); glVertex3d(0.0, SPEAK, SLEN / 2);
			glTexCoord2d(1.00, 1.00); glVertex3d(0.0, SPEAK, -SLEN / 2);
			glTexCoord2d(0.00, 1.00); glVertex3d(-SWID / 2, SHIGH, -SLEN / 2);
			glTexCoord2d(0.00, .00); glVertex3d(-SWID / 2, SHIGH, SLEN / 2);
		glEnd();
	glEndList();
}

/////////////////////////////////////////////////////////////////////////////
// cities
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::buildCity()
{
	//build the city
	glNewList(m_lists[GL_CITY], GL_COMPILE);
		//front face
		glBegin(GL_POLYGON);
			glNormal3f(0.0, 0.0, 1.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, 0.0, CLEN / 2);
			glTexCoord2d(0.00, 1.00); glVertex3d(CWID / 2, CHIGH, CLEN / 2);
			glTexCoord2d(1.00, 1.00); glVertex3d(-CWID / 2, CHIGH, CLEN / 2);
			glTexCoord2d(1.00, 0.00); glVertex3d(-CWID / 2, 0.0, CLEN / 2);
			glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, 0.0, CLEN / 2);
		glEnd();
		//back face
		glBegin(GL_POLYGON);
			glNormal3f(0.0, 0.0, -1.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, 0.0, -CLEN / 2);
			glTexCoord2d(0.00, 0.50); glVertex3d(CWID / 2, CLOW, -CLEN / 2);
			glTexCoord2d(0.50, 0.50); glVertex3d(-CWID / 2, CLOW, -CLEN / 2);
			glTexCoord2d(0.50, 0.00); glVertex3d(-CWID / 2, 0.0, -CLEN / 2);
			glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, 0.0, -CLEN / 2);
		glEnd();
		//second back face
		glBegin(GL_POLYGON);
			glNormal3f(0.0, 0.0, -1.0);
			glTexCoord2d(0.40, 0.40); glVertex3d(CWID / 2, CLOW, 0.0);
			glTexCoord2d(0.40, 0.90); glVertex3d(CWID / 2, CHIGH, 0.0);
			glTexCoord2d(0.90, 0.90); glVertex3d(-CWID / 2, CHIGH, 0.0);
			glTexCoord2d(0.90, 0.40); glVertex3d(-CWID / 2, CLOW, 0.0);
			glTexCoord2d(0.40, 0.40); glVertex3d(CWID / 2, CLOW, 0.0);
		glEnd();
		//top face
		glBegin(GL_POLYGON);
			glNormal3f(0.0, 1.0, 0.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, CLOW, -CLEN / 2);
			glTexCoord2d(0.00, 0.67); glVertex3d(CWID / 2, CLOW, 0.0);
			glTexCoord2d(0.67, 0.67); glVertex3d(-CWID / 2, CLOW, 0.0);
			glTexCoord2d(0.67, 0.00); glVertex3d(-CWID / 2, CLOW, -CLEN / 2);
			glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, CLOW, -CLEN / 2);
		glEnd();
		//top back face
		glBegin(GL_POLYGON);
			glNormal3f(0.0, 1.0, -1.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, CHIGH, 0.0);
			glTexCoord2d(0.00, 0.33); glVertex3d(CWID / 2, CPEAK, CLEN / 4);
			glTexCoord2d(0.33, 0.33); glVertex3d(-CWID / 2, CPEAK, CLEN / 4);
			glTexCoord2d(0.33, 0.00); glVertex3d(-CWID / 2, CHIGH, 0.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(CWID / 2, CHIGH, 0.0);
		glEnd();
		//top front face
		glBegin(GL_POLYGON);
			glNormal3f(0.0, 1.0, 1.0);
			glTexCoord2d(0.10, 0.10); glVertex3d(CWID / 2, CHIGH, CLEN / 2);
			glTexCoord2d(0.10, 0.43); glVertex3d(CWID / 2, CPEAK, CLEN / 4);
			glTexCoord2d(0.43, 0.43); glVertex3d(-CWID / 2, CPEAK, CLEN / 4);
			glTexCoord2d(0.43, 0.10); glVertex3d(-CWID / 2, CHIGH, CLEN / 2);
			glTexCoord2d(0.10, 0.10); glVertex3d(CWID / 2, CHIGH, CLEN / 2);
		glEnd();

	glEndList();

	glNewList(m_lists[GL_CITYA], GL_COMPILE);
		//right side face
		glBegin(GL_POLYGON);
			glNormal3f(1.0, 0.0, 0.0);
			glTexCoord2d(0.02, 0.00); glVertex3d(CWID / 2, 0.0, CLEN / 2);
			glTexCoord2d(0.98, 0.00); glVertex3d(CWID / 2, 0.0, -CLEN / 2);
			glTexCoord2d(0.98, 0.40); glVertex3d(CWID / 2, CLOW, -CLEN / 2);
			glTexCoord2d(0.48, 0.40); glVertex3d(CWID / 2, CLOW, 0.0);
			glTexCoord2d(0.48, 0.71); glVertex3d(CWID / 2, CHIGH, 0.0);
			glTexCoord2d(0.25, 0.95); glVertex3d(CWID / 2, CPEAK, CLEN / 4);
			glTexCoord2d(0.02, 0.71); glVertex3d(CWID / 2, CHIGH, CLEN / 2);
			glTexCoord2d(0.02, 0.00); glVertex3d(CWID / 2, 0.0, CLEN / 2);
		glEnd();
		//left side face
		glBegin(GL_POLYGON);
			glNormal3f(-1.0, 0.0, 0.0);
			glTexCoord2d(0.02, 0.00); glVertex3d(-CWID / 2, 0.0, CLEN / 2);
			glTexCoord2d(0.98, 0.00); glVertex3d(-CWID / 2, 0.0, -CLEN / 2);
			glTexCoord2d(0.98, 0.40); glVertex3d(-CWID / 2, CLOW, -CLEN / 2);
			glTexCoord2d(0.48, 0.40); glVertex3d(-CWID / 2, CLOW, 0.0);
			glTexCoord2d(0.48, 0.71); glVertex3d(-CWID / 2, CHIGH, 0.0);
			glTexCoord2d(0.25, 0.95); glVertex3d(-CWID / 2, CPEAK, CLEN / 4);
			glTexCoord2d(0.02, 0.71); glVertex3d(-CWID / 2, CHIGH, CLEN / 2);
			glTexCoord2d(0.02, 0.00); glVertex3d(-CWID / 2, 0.0, CLEN / 2);
		glEnd();
	glEndList();
}

/////////////////////////////////////////////////////////////////////////////
// roads
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::buildRoad()
{
	//build the road
	glNewList(m_lists[GL_ROAD], GL_COMPILE);
		//front face
		glBegin(GL_QUADS);
			glNormal3f(0.0, 0.0, 1.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(RWID / 2, 0.0, RLEN / 2);
			glTexCoord2d(1.00, 0.00); glVertex3d(-RWID / 2, 0.0, RLEN / 2);
			glTexCoord2d(1.00, 1.00); glVertex3d(-RWID / 2, RHIGH, RLEN / 2);
			glTexCoord2d(0.00, 1.00); glVertex3d(RWID / 2, RHIGH, RLEN / 2);
		glEnd();
		//back face
		glBegin(GL_QUADS);
			glNormal3f(0.0, 0.0, -1.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(RWID / 2, 0.0, -RLEN / 2);
			glTexCoord2d(1.00, 0.00); glVertex3d(-RWID / 2, 0.0, -RLEN / 2);
			glTexCoord2d(1.00, 1.00); glVertex3d(-RWID / 2, RHIGH, -RLEN / 2);
			glTexCoord2d(0.00, 1.00); glVertex3d(RWID / 2, RHIGH, -RLEN / 2);
		glEnd();
	glEndList();

	glNewList(m_lists[GL_ROADA], GL_COMPILE);
		//right side face
		glBegin(GL_QUADS);
			glNormal3f(1.0, 0.0, 0.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(RWID / 2, 0.0, RLEN / 2);
			glTexCoord2d(1.00, 0.00); glVertex3d(RWID / 2, 0.0, -RLEN / 2);
			glTexCoord2d(1.00, 1.00); glVertex3d(RWID / 2, RHIGH, -RLEN / 2);
			glTexCoord2d(0.00, 1.00); glVertex3d(RWID / 2, RHIGH, RLEN / 2);
		glEnd();
		//left side face
		glBegin(GL_QUADS);
			glNormal3f(-1.0, 0.0, 0.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(-RWID / 2, 0.0, RLEN / 2);
			glTexCoord2d(1.00, 0.00); glVertex3d(-RWID / 2, 0.0, -RLEN / 2);
			glTexCoord2d(1.00, 1.00); glVertex3d(-RWID / 2, RHIGH, -RLEN / 2);
			glTexCoord2d(0.00, 1.00); glVertex3d(-RWID / 2, RHIGH, RLEN / 2);
		glEnd();
		//top side
		glBegin(GL_QUADS);
			glNormal3f(0.0, 1.0, 0.0);
			glTexCoord2d(0.00, 0.00); glVertex3d(RWID / 2, RHIGH, RLEN / 2);
			glTexCoord2d(1.00, 0.00); glVertex3d(RWID / 2, RHIGH, -RLEN / 2);
			glTexCoord2d(1.00, 1.00); glVertex3d(-RWID / 2, RHIGH, -RLEN / 2);
			glTexCoord2d(0.00, 1.00); glVertex3d(-RWID / 2, RHIGH, RLEN / 2);
		glEnd();
	glEndList();
}

/////////////////////////////////////////////////////////////////////////////
// create the ship display list
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::buildShip()
{
	int i;
	double dY, dZ;
	double dTX, dTY;
	CVector v1Old, v2Old;
	CVector v1New, v2New;
	CVector x;

	//build the ship
	glNewList(m_lists[GL_SHIPA], GL_COMPILE);
		//front curved face
		glBegin(GL_QUADS);
			glNormal3f(0.0, 0.0, 1.0);
			glTexCoord2d(0.8, 0.0); glVertex3d(SHWID / 2, SHHULLR, SHLEN / 2);
			glTexCoord2d(0.8, 1.0); glVertex3d(-SHWID / 2, SHHULLR, SHLEN / 2);
			glTexCoord2d(1.0, 1.0); glVertex3d(-SHWID / 2, SHMID, SHLEN / 2);
			glTexCoord2d(1.0, 0.0); glVertex3d(SHWID / 2, SHMID, SHLEN / 2);
		glEnd();

		//the curves
		glBegin(GL_QUAD_STRIP);
			//set the first two
			glNormal3f(0.0, 0.0, 1.0);
			//set the vectors
			v1Old = CVector(-SHWID / 2, SHHULLR, SHLEN / 2);
			v2Old = CVector(SHWID / 2, SHHULLR, SHLEN / 2);

			//draw the first two
			glTexCoord2d(0.8, 1.0); glVertex3dv(v1Old);
			glTexCoord2d(0.8, 0.0); glVertex3dv(v2Old);

			for(i = 1; i <= SHSLICES; i++)
			{
				dZ = ((SHLEN / 2) - SHHULLR) - (SHHULLR * cos(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));
				dY = SHHULLR + (SHHULLR * sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

				//set the comparison vectors
				v1New = CVector(-SHWID / 2, dY, dZ);
				v2New = CVector(SHWID / 2, dY, dZ);

				//now, we cross product
				x = CrossProduct(v1Old - v2Old, v1New - v1Old);
				x.Normalize();

				//set the new normal
				glNormal3dv(x);

				//draw it
				glTexCoord2d(0.8 - ((0.4 / SHSLICES) * (double) i), 1.0); glVertex3dv(v1New);
				glTexCoord2d(0.8 - ((0.4 / SHSLICES) * (double) i), 0.0); glVertex3dv(v2New);

				//reset the old vectors
				v1Old = v1New;
				v2Old = v2New;
			}
		glEnd();

		//back curved face
		glBegin(GL_QUADS);
			glNormal3f(0.0, 0.0, -1.0);
			glTexCoord2d(0.7, 0.0); glVertex3d(SHWID / 2, SHHULLRB, -SHLEN / 2);
			glTexCoord2d(0.7, 1.0); glVertex3d(-SHWID / 2, SHHULLRB, -SHLEN / 2);
			glTexCoord2d(0.8, 1.0); glVertex3d(-SHWID / 2, SHMID, -SHLEN / 2);
			glTexCoord2d(0.8, 0.0); glVertex3d(SHWID / 2, SHMID, -SHLEN / 2);
		glEnd();

		//the curves
		glBegin(GL_QUAD_STRIP);
			//set the first two
			glNormal3f(0.0, 0.0, -1.0);

			//set the vectors
			v1Old = CVector(SHWID / 2, SHHULLRB, -SHLEN / 2);
			v2Old = CVector(-SHWID / 2, SHHULLRB, -SHLEN / 2);

			//draw the first two
			glTexCoord2d(0.7, 0.0); glVertex3dv(v1Old);
			glTexCoord2d(0.7, 1.0); glVertex3dv(v2Old);

			for(i = 1; i <= SHSLICES; i++)
			{
				dZ = (-(SHLEN / 2) + SHHULLRB) - (SHHULLRB * cos(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));
				dY = SHHULLRB + (SHHULLRB * sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

				//set the comparison vectors
				v1New = CVector(SHWID / 2, dY, dZ);
				v2New = CVector(-SHWID / 2, dY, dZ);

				//now, we cross product
				x = CrossProduct(v1Old - v2Old, v1New - v1Old);
				x.Normalize();

				//set the new normal
				glNormal3dv(x);

				//draw it
				glTexCoord2d(0.7 - ((0.5 / SHSLICES) * (double) i), 0.0); glVertex3dv(v1New);
				glTexCoord2d(0.7 - ((0.5 / SHSLICES) * (double) i), 1.0); glVertex3dv(v2New);

				//reset the old vectors
				v1Old = v1New;
				v2Old = v2New;
			}
		glEnd();

		//front top half face
		glBegin(GL_QUADS);
			glNormal3f(0.0, 1.0, 0.0);
			glTexCoord2d(0.2, 0.0); glVertex3d(SHWID / 2, SHMID, SHLEN / 2);
			glTexCoord2d(0.65, 0.0); glVertex3d(SHWID / 2, SHMID, SHMAST / 2);
			glTexCoord2d(0.65, 1.0); glVertex3d(-SHWID / 2, SHMID, SHMAST / 2);
			glTexCoord2d(0.2, 1.0); glVertex3d(-SHWID / 2, SHMID, SHLEN / 2);
		glEnd();
		//back top half face
		glBegin(GL_QUADS);
			glNormal3f(0.0, 1.0, 0.0);
			glTexCoord2d(0.5, 0.0); glVertex3d(SHWID / 2, SHMID, -SHLEN / 2);
			glTexCoord2d(0.95, 0.0); glVertex3d(SHWID / 2, SHMID, -SHMAST / 2);
			glTexCoord2d(0.95, 1.0); glVertex3d(-SHWID / 2, SHMID, -SHMAST / 2);
			glTexCoord2d(0.5, 1.0); glVertex3d(-SHWID / 2, SHMID, -SHLEN / 2);
		glEnd();

		//mast front face
		glBegin(GL_QUADS);
			glNormal3f(0.0, 0.0, 1.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(SHWID / 2, SHMID, SHMAST / 2);
			glTexCoord2d(0.0, 1.0); glVertex3d(-SHWID / 2, SHMID, SHMAST / 2);
			glTexCoord2d(0.45, 1.0); glVertex3d(-SHWID / 2, SHTOP - SHMASTR, SHMAST / 2);
			glTexCoord2d(0.45, 0.0); glVertex3d(SHWID / 2, SHTOP - SHMASTR, SHMAST / 2);
		glEnd();
		//mast back face
		glBegin(GL_QUADS);
			glNormal3f(0.0, 0.0, -1.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(SHWID / 2, SHMID, -SHMAST / 2);
			glTexCoord2d(0.0, 1.0); glVertex3d(-SHWID / 2, SHMID, -SHMAST / 2);
			glTexCoord2d(0.8, 1.0); glVertex3d(-SHWID / 2, SHTOP, -SHMAST / 2);
			glTexCoord2d(0.8, 0.0); glVertex3d(SHWID / 2, SHTOP, -SHMAST / 2);
		glEnd();

		//the curved face
		glBegin(GL_QUAD_STRIP);
			//set the vectors
			v1Old = CVector(SHWID / 2, SHTOP - SHMASTR, SHMAST / 2);
			v2Old = CVector(-SHWID / 2, SHTOP - SHMASTR, SHMAST / 2);

			//draw the first two
			glNormal3f(0.0, 0.0, 1.0);
			glTexCoord2d(0.45, 0.0); glVertex3dv(v1Old);
			glTexCoord2d(0.45, 1.0); glVertex3dv(v2Old);

			for(i = 1; i <= SHSLICES; i++)
			{
				dZ = ((SHMAST / 2) - SHMASTR) - (SHMASTR * cos(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));
				dY = (SHTOP - SHMASTR) + (SHMASTR * sin(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));

				//set the comparison vectors
				v1New = CVector(SHWID / 2, dY, dZ);
				v2New = CVector(-SHWID / 2, dY, dZ);

				//now, we cross product
				x = CrossProduct(v1Old - v2Old, v1New - v1Old);
				x.Normalize();

				//set the new normal
				glNormal3dv(x);

				//draw it
				glTexCoord2d(0.45 + ((0.55 / SHSLICES) * (double) i), 0.0); glVertex3dv(v1New);
				glTexCoord2d(0.45 + ((0.55 / SHSLICES) * (double) i), 1.0); glVertex3dv(v2New);

				//reset the old vectors
				v1Old = v1New;
				v2Old = v2New;
			}
		glEnd();
	glEndList();

	//SIDE FACES
	glNewList(m_lists[GL_SHIP], GL_COMPILE);
		//side face
		glBegin(GL_QUADS);
			glNormal3f(1.0, 0.0, 0.0);
			glTexCoord2d(0.01, 0.198); glVertex3d(SHWID / 2, SHHULLR, SHLEN / 2);
			glTexCoord2d(0.01, 0.33); glVertex3d(SHWID / 2, SHMID, SHLEN / 2);
			glTexCoord2d(0.22, 0.33); glVertex3d(SHWID / 2, SHMID, (SHLEN / 2) - SHHULLR);
			glTexCoord2d(0.22, 0.198); glVertex3d(SHWID / 2, SHHULLR, (SHLEN / 2) - SHHULLR);
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(1.0, 0.0, 0.0);
			glTexCoord2d(0.22, 0.0); glVertex3d(SHWID / 2, 0.0, (SHLEN / 2) - SHHULLR);
			glTexCoord2d(0.22, 0.33); glVertex3d(SHWID / 2, SHMID, (SHLEN / 2) - SHHULLR);
			glTexCoord2d(0.72, 0.33); glVertex3d(SHWID / 2, SHMID, -(SHLEN / 2) + SHHULLRB);
			glTexCoord2d(0.72, 0.0); glVertex3d(SHWID / 2, 0.0, -(SHLEN / 2) + SHHULLRB);
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(1.0, 0.0, 0.0);
			glTexCoord2d(1.00, 0.264); glVertex3d(SHWID / 2, SHHULLRB, -SHLEN / 2);
			glTexCoord2d(1.00, 0.33); glVertex3d(SHWID / 2, SHMID, -SHLEN / 2);
			glTexCoord2d(0.72, 0.33); glVertex3d(SHWID / 2, SHMID, -(SHLEN / 2) + SHHULLRB);
			glTexCoord2d(0.72, 0.264); glVertex3d(SHWID / 2, SHHULLRB, -(SHLEN / 2) + SHHULLRB);
		glEnd();

		//create the polygon
		glBegin(GL_POLYGON);
			//set the first two coords
			glNormal3f(1.0, 0.0, 0.0);
			glTexCoord2d(0.22, 0.198); glVertex3d(SHWID / 2, SHHULLR, (SHLEN / 2) - SHHULLR);
			glTexCoord2d(0.01, 0.198); glVertex3d(SHWID / 2, SHHULLR, SHLEN / 2);
		
			for(i = 1; i <= SHSLICES; i++)
			{
				dZ = ((SHLEN / 2) - SHHULLR) - (SHHULLR * cos(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));
				dY = SHHULLR + (SHHULLR * sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

				//calc tex coords
				dTX = 0.22 - (0.21 * cos(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));
				dTY = 0.198 + (0.198 * sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

				//draw it
				glTexCoord2d(dTX, dTY); glVertex3d(SHWID / 2, dY, dZ);
			}
		glEnd();

		//create the polygon
		glBegin(GL_POLYGON);
			//set the first two coords
			glNormal3f(1.0, 0.0, 0.0);
			glTexCoord2d(0.72, 0.264); glVertex3d(SHWID / 2, SHHULLRB, -(SHLEN / 2) + SHHULLRB);
			glTexCoord2d(1.00, 0.264); glVertex3d(SHWID / 2, SHHULLRB, -SHLEN / 2);
		
			for(i = 1; i <= SHSLICES; i++)
			{
				dZ = (-(SHLEN / 2) + SHHULLRB) - (SHHULLRB * cos(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));
				dY = SHHULLRB + (SHHULLRB * sin(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));

				//calc tex coords
				dTX = 0.72 - (0.28 * cos(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));
				dTY = 0.264 + (0.264 * sin(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));

				//draw it
				glTexCoord2d(dTX, dTY); glVertex3d(SHWID / 2, dY, dZ);
			}
		glEnd();

		//side face
		glBegin(GL_QUADS);
			glNormal3f(-1.0, 0.0, 0.0);
			glTexCoord2d(0.01, 0.198); glVertex3d(-SHWID / 2, SHHULLR, SHLEN / 2);
			glTexCoord2d(0.01, 0.33); glVertex3d(-SHWID / 2, SHMID, SHLEN / 2);
			glTexCoord2d(0.22, 0.33); glVertex3d(-SHWID / 2, SHMID, (SHLEN / 2) - SHHULLR);
			glTexCoord2d(0.22, 0.198); glVertex3d(-SHWID / 2, SHHULLR, (SHLEN / 2) - SHHULLR);
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(-1.0, 0.0, 0.0);
			glTexCoord2d(0.22, 0.0); glVertex3d(-SHWID / 2, 0.0, (SHLEN / 2) - SHHULLR);
			glTexCoord2d(0.22, 0.33); glVertex3d(-SHWID / 2, SHMID, (SHLEN / 2) - SHHULLR);
			glTexCoord2d(0.72, 0.33); glVertex3d(-SHWID / 2, SHMID, -(SHLEN / 2) + SHHULLRB);
			glTexCoord2d(0.72, 0.0); glVertex3d(-SHWID / 2, 0.0, -(SHLEN / 2) + SHHULLRB);
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(-1.0, 0.0, 0.0);
			glTexCoord2d(1.00, 0.264); glVertex3d(-SHWID / 2, SHHULLRB, -SHLEN / 2);
			glTexCoord2d(1.00, 0.33); glVertex3d(-SHWID / 2, SHMID, -SHLEN / 2);
			glTexCoord2d(0.72, 0.33); glVertex3d(-SHWID / 2, SHMID, -(SHLEN / 2) + SHHULLRB);
			glTexCoord2d(0.72, 0.264); glVertex3d(-SHWID / 2, SHHULLRB, -(SHLEN / 2) + SHHULLRB);
		glEnd();

		//create the polygon
		glBegin(GL_POLYGON);
			//set the first two coords
			glNormal3f(-1.0, 0.0, 0.0);
			glTexCoord2d(0.22, 0.198); glVertex3d(-SHWID / 2, SHHULLR, (SHLEN / 2) - SHHULLR);
			glTexCoord2d(0.01, 0.198); glVertex3d(-SHWID / 2, SHHULLR, SHLEN / 2);
		
			for(i = 1; i <= SHSLICES; i++)
			{
				dZ = ((SHLEN / 2) - SHHULLR) - (SHHULLR * cos(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));
				dY = SHHULLR + (SHHULLR * sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

				//calc tex coords
				dTX = 0.22 - (0.21 * cos(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));
				dTY = 0.198 + (0.198 * sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

				//draw it
				glTexCoord2d(dTX, dTY); glVertex3d(-SHWID / 2, dY, dZ);
			}
		glEnd();

		//create the polygon
		glBegin(GL_POLYGON);
			//set the first two coords
			glNormal3f(-1.0, 0.0, 0.0);
			glTexCoord2d(0.72, 0.264); glVertex3d(-SHWID / 2, SHHULLRB, -(SHLEN / 2) + SHHULLRB);
			glTexCoord2d(1.00, 0.264); glVertex3d(-SHWID / 2, SHHULLRB, -SHLEN / 2);
		
			for(i = 1; i <= SHSLICES; i++)
			{
				dZ = (-(SHLEN / 2) + SHHULLRB) - (SHHULLRB * cos(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));
				dY = SHHULLRB + (SHHULLRB * sin(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));

				//calc tex coords
				dTX = 0.72 - (0.28 * cos(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));
				dTY = 0.264 + (0.264 * sin(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));

				//draw it
				glTexCoord2d(dTX, dTY); glVertex3d(-SHWID / 2, dY, dZ);
			}
		glEnd();

		//mast side
		glBegin(GL_QUADS);
			glNormal3f(1.0, 0.0, 0.0);
			glTexCoord2d(0.35, 0.33); glVertex3d(SHWID / 2, SHMID, SHMAST / 2);
			glTexCoord2d(0.68, 0.33); glVertex3d(SHWID / 2, SHMID, -SHMAST / 2);
			glTexCoord2d(0.68, 0.66); glVertex3d(SHWID / 2, SHTOP - SHMASTR, -SHMAST / 2);
			glTexCoord2d(0.35, 0.66); glVertex3d(SHWID / 2, SHTOP - SHMASTR, SHMAST / 2);
		glEnd();

		//create the polygon
		glBegin(GL_POLYGON);
			//set the first two coords
			glNormal3f(1.0, 0.0, 0.0);
			glTexCoord2d(0.68, 0.66); glVertex3d(SHWID / 2, SHTOP - SHMASTR, (SHMAST / 2) - SHMASTR);
			glTexCoord2d(0.35, 0.66); glVertex3d(SHWID / 2, SHTOP - SHMASTR, SHMAST / 2);
		
			for(i = 1; i <= SHSLICES; i++)
			{
				dZ = ((SHMAST / 2) - SHMASTR) - (SHMASTR * cos(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));
				dY = (SHTOP - SHMASTR) + (SHMASTR * sin(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));

				//calc tex coords
				dTX = 0.68 + (0.23 * cos(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));
				dTY = 0.66 - (0.33 * sin(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));

				//draw it
				glTexCoord2d(dTX, dTY); glVertex3d(SHWID / 2, dY, dZ);
			}
		glEnd();

		//mast side
		glBegin(GL_QUADS);
			glNormal3f(-1.0, 0.0, 0.0);
			glTexCoord2d(0.35, 0.33); glVertex3d(-SHWID / 2, SHMID, SHMAST / 2);
			glTexCoord2d(0.68, 0.33); glVertex3d(-SHWID / 2, SHMID, -SHMAST / 2);
			glTexCoord2d(0.68, 0.66); glVertex3d(-SHWID / 2, SHTOP - SHMASTR, -SHMAST / 2);
			glTexCoord2d(0.35, 0.66); glVertex3d(-SHWID / 2, SHTOP - SHMASTR, SHMAST / 2);
		glEnd();

		//create the polygon
		glBegin(GL_POLYGON);
			//set the first two coords
			glNormal3f(-1.0, 0.0, 0.0);
			glTexCoord2d(0.68, 0.66); glVertex3d(-SHWID / 2, SHTOP - SHMASTR, (SHMAST / 2) - SHMASTR);
			glTexCoord2d(0.35, 0.66); glVertex3d(-SHWID / 2, SHTOP - SHMASTR, SHMAST / 2);
		
			for(i = 1; i <= SHSLICES; i++)
			{
				dZ = ((SHMAST / 2) - SHMASTR) - (SHMASTR * cos(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));
				dY = (SHTOP - SHMASTR) + (SHMASTR * sin(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));

				//calc tex coords
				dTX = 0.68 + (0.23 * cos(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));
				dTY = 0.66 - (0.33 * sin(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));

				//draw it
				glTexCoord2d(dTX, dTY); glVertex3d(-SHWID / 2, dY, dZ);
			}
		glEnd();
	glEndList();
}

/////////////////////////////////////////////////////////////////////////////
// create the ship outline list
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::buildShipOutline()
{
	int i;
	double dY, dZ;

	//build the ship outline
	glNewList(m_lists[GL_SHIP_OUTLINE], GL_COMPILE);

		//start with the sides and then draw the cross lines
		glBegin(GL_LINE_LOOP);
			glNormal3f(1.0, 0.0, 0.0);
			glVertex3d(SHWID / 2, SHTOP, -SHMAST / 2);

			//mast curve
			for(i = SHSLICES; i >= 1; i--)
			{
				dZ = ((SHMAST / 2) - SHMASTR) - (SHMASTR * cos(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));
				dY = (SHTOP - SHMASTR) + (SHMASTR * sin(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));

				//draw it
				glVertex3d(SHWID / 2, dY, dZ);
			}
			
			//mast base
			glVertex3d(SHWID / 2, SHMID, SHMAST / 2);

			//front top
			glVertex3d(SHWID / 2, SHMID, SHLEN / 2);

			//front curve
			for(i = 1; i <= SHSLICES; i++)
			{
				dZ = ((SHLEN / 2) - SHHULLR) - (SHHULLR * cos(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));
				dY = SHHULLR + (SHHULLR * sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

				//draw it
				glVertex3d(SHWID / 2, dY, dZ);
			}
		
			//base
			glVertex3d(SHWID / 2, 0.0, -(SHLEN / 2) + SHHULLRB);

			//back curve
			for(i = SHSLICES; i >= 1; i--)
			{
				dZ = (-(SHLEN / 2) + SHHULLRB) - (SHHULLRB * cos(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));
				dY = SHHULLRB + (SHHULLRB * sin(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));

				//draw it
				glVertex3d(SHWID / 2, dY, dZ);
			}

			//back corner
			glVertex3d(SHWID / 2, SHMID, -SHLEN / 2);

			//back top
			glVertex3d(SHWID / 2, SHMID, -SHMAST / 2);
		glEnd();

		//now repeat for the other side
		glBegin(GL_LINE_LOOP);
			glNormal3f(-1.0, 0.0, 0.0);
			glVertex3d(-SHWID / 2, SHTOP, -SHMAST / 2);

			//mast curve
			for(i = SHSLICES; i >= 1; i--)
			{
				dZ = ((SHMAST / 2) - SHMASTR) - (SHMASTR * cos(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));
				dY = (SHTOP - SHMASTR) + (SHMASTR * sin(D2R * (180.0 - ((90.0 / (double) SHSLICES) * i))));

				//draw it
				glVertex3d(-SHWID / 2, dY, dZ);
			}
			
			//mast base
			glVertex3d(-SHWID / 2, SHMID, SHMAST / 2);

			//front top
			glVertex3d(-SHWID / 2, SHMID, SHLEN / 2);

			//front curve
			for(i = 1; i <= SHSLICES; i++)
			{
				dZ = ((SHLEN / 2) - SHHULLR) - (SHHULLR * cos(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));
				dY = SHHULLR + (SHHULLR * sin(D2R * (180.0 + ((90.0 / (double) SHSLICES) * i))));

				//draw it
				glVertex3d(-SHWID / 2, dY, dZ);
			}
		
			//base
			glVertex3d(-SHWID / 2, 0.0, -(SHLEN / 2) + SHHULLRB);

			//back curve
			for(i = SHSLICES; i >= 1; i--)
			{
				dZ = (-(SHLEN / 2) + SHHULLRB) - (SHHULLRB * cos(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));
				dY = SHHULLRB + (SHHULLRB * sin(D2R * (0.0 - ((90.0 / (double) SHSLICES) * i))));

				//draw it
				glVertex3d(-SHWID / 2, dY, dZ);
			}

			//back corner
			glVertex3d(-SHWID / 2, SHMID, -SHLEN / 2);

			//back top
			glVertex3d(-SHWID / 2, SHMID, -SHMAST / 2);
		glEnd();

		//now the cross lines
		glBegin(GL_LINES);
			//mast top
			glNormal3f(0.0, 0.0, -1.0);
			glVertex3d(SHWID / 2, SHTOP, -SHMAST / 2);
			glVertex3d(-SHWID / 2, SHTOP, -SHMAST / 2);

			//mast base front
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3d(SHWID / 2, SHMID, SHMAST / 2);
			glVertex3d(-SHWID / 2, SHMID, SHMAST / 2);

			//mast base back
			glNormal3f(0.0, 0.0, -1.0);
			glVertex3d(SHWID / 2, SHMID, -SHMAST / 2);
			glVertex3d(-SHWID / 2, SHMID, -SHMAST / 2);

			//hull front
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3d(SHWID / 2, SHMID, SHLEN / 2);
			glVertex3d(-SHWID / 2, SHMID, SHLEN / 2);

			//hull back
			glNormal3f(0.0, 0.0, -1.0);
			glVertex3d(SHWID / 2, SHMID, -SHLEN / 2);
			glVertex3d(-SHWID / 2, SHMID, -SHLEN / 2);
		glEnd();
	glEndList();
}

/////////////////////////////////////////////////////////////////////////////
// create the robber display list
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::buildRobber()
{
	int i, j, k;
	double dRadii[GL_ROBBER_STACK];
	double dHeight = 3.0;
	double dTwoPi = 360 * D2R;
	double dH;
	double dT;
	CVector v1, v2;
	CVector points[GL_ROBBER_STACK][GL_ROBBER_SLICE];
	CVector gouraud;

	dRadii[19] = 0.0;
	dRadii[18] = 0.33;
	dRadii[17] = 0.42;
	dRadii[16] = 0.47;
	dRadii[15] = 0.5;
	dRadii[14] = 0.47;
	dRadii[13] = 0.40;
	//start increase for thick middle part
	dRadii[12] = 0.49;
	dRadii[11] = 0.57;
	dRadii[10] = 0.65;
	dRadii[9] = 0.67;
	dRadii[8] = 0.69;
	dRadii[7] = 0.70;
	dRadii[6] = 0.69;
	dRadii[5] = 0.67;
	dRadii[4] = 0.61;
	dRadii[3] = 0.52;
	dRadii[2] = 0.4;
	dRadii[1] = 0.55;
	dRadii[0] = 0.55;

	//create points
	for(i = 0; i < GL_ROBBER_STACK; i++)
	{
		for(j = 0; j < GL_ROBBER_SLICE; j++)
		{
			//calculate height
			dH = (dHeight / GL_ROBBER_STACK) * i;

			//calculate theta
			dT = (dTwoPi / GL_ROBBER_SLICE) * j;

			//create point
			points[i][j].Set(dRadii[i] * cos(dT), dH, dRadii[i] * sin(dT));
		}
	}

	//create face normals
	for(i = 0; i < GL_ROBBER_STACK - 1; i++)
	{
		for(j = 0; j < GL_ROBBER_SLICE; j++)
		{
			//set wrap around
			k = (j < GL_ROBBER_SLICE - 1) ? j : -1;

			//create face vectors
			v1 = points[i][j] - points[i][k + 1];
			v2 = points[i + 1][k + 1] - points[i][k + 1];

			//create face normal
			m_normal[i][j] = CrossProduct(v1, v2);
			m_normal[i][j].Normalize();
		}
	}

	//build the robber
	glNewList(m_lists[GL_ROBBER], GL_COMPILE);

	//draw
	for(i = 0; i < GL_ROBBER_STACK - 1; i++)
	{
		for(j = 0; j < GL_ROBBER_SLICE; j++)
		{
			//set wrap around
			k = (j < GL_ROBBER_SLICE - 1) ? j : -1;

			glBegin(GL_QUADS);
				//normal
				glNormal3dv(m_normal[i][j]);

				//two bottom points
				glVertex3dv(points[i][j]);
				glVertex3dv(points[i][k + 1]);

				//two top points
				glVertex3dv(points[i + 1][k + 1]);
				glVertex3dv(points[i + 1][j]);
			glEnd();
		}
	}

	//finish up
	glEndList();

	//build gouraud robber
	glNewList(m_lists[GL_ROBBER_GOURAUD], GL_COMPILE);

	//draw
	for(i = 0; i < GL_ROBBER_STACK - 1; i++)
	{
		for(j = 0; j < GL_ROBBER_SLICE; j++)
		{
			//set wrap around
			k = (j < GL_ROBBER_SLICE - 1) ? j : -1;

			glBegin(GL_QUADS);
				//two bottom points
				gouraud = calcPointVector(i, j);
				glNormal3dv(gouraud);
				glVertex3dv(points[i][j]);
				
				gouraud = calcPointVector(i, j + 1);
				glNormal3dv(gouraud);
				glVertex3dv(points[i][k + 1]);

				//two top points
				gouraud = calcPointVector(i + 1, j + 1);
				glNormal3dv(gouraud);
				glVertex3dv(points[i + 1][k + 1]);

				gouraud = calcPointVector(i + 1, j);
				glNormal3dv(gouraud);
				glVertex3dv(points[i + 1][j]);
			glEnd();
		}
	}

	//finish up
	glEndList();
}

/////////////////////////////////////////////////////////////////////////////
// calculate Gouraud shading vector
/////////////////////////////////////////////////////////////////////////////
CVector CGLBaseView::calcPointVector(int i, int j)
{
	int k;
	int iNum = 2;
	CVector normal;
	
	//set wrap
	k = (j == 0) ? GL_ROBBER_SLICE - 1 : j - 1;

	//get lower faces
	if(i > 0)
	{
		normal += m_normal[i - 1][j];
		normal += m_normal[i - 1][k];

		iNum = 4;
	}

	//get upper faces
	normal += m_normal[i][j];
	normal += m_normal[i][k];

	//average
	normal /= iNum;

	//normalize
	normal.Normalize();

	return normal;
}

/////////////////////////////////////////////////////////////////////////////
// create the seaport display list
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::buildSeaport()
{
	int i;
	int iSlices = 20;
	double dRadius = 1.01;
	double fWidth = X * .91;
	double fLength = X * 1.14;
	double dTheta;
	double dX, dZ;
	double dXConvert;

	//calculate conversion
	dXConvert = dRadius / fWidth;

	//calculate sea port
	glNewList(m_lists[GL_SEAPORT], GL_COMPILE);
		
		glBegin(GL_POLYGON);

			//set the normal
			glNormal3f(0.0, 1.0, 0.0);
		
			//create the circle
			for(i = 0; i < iSlices; i++)
			{
				//calculate theta
				dTheta = ((360. / (iSlices + 3)) * i) + 122;

				//convert to radians
				dTheta *= D2R;

				//calc coordinates
				dX = dRadius * cos(dTheta);
				dZ = dRadius * sin(dTheta);

				//calculate texture coordinates
				glTexCoord2d(0.5 - (dX * dXConvert), 0.2 + (dZ * dXConvert));
				
				//set the point
				glVertex3d(dX, TILE_DEPTH, dZ - ((fLength / 2) - dRadius));
			}

			//create the angled part
			glTexCoord2d(0.00, 1.00); glVertex3d(fWidth / 2, TILE_DEPTH, fLength / 2);
			glTexCoord2d(1.00, 1.00); glVertex3d(-fWidth / 2, TILE_DEPTH, fLength / 2);

		glEnd();

	//finish up
	glEndList();

	//build the seaport edge list
	glNewList(m_lists[GL_SEAPORT_EDGE], GL_COMPILE);

		glBegin(GL_QUAD_STRIP);

			//create the circle
			for(i = 0; i < iSlices; i++)
			{
				//calculate theta
				dTheta = ((360. / (iSlices + 3)) * i) + 122;

				//convert to radians
				dTheta *= D2R;

				//calc coordinates
				dX = dRadius * cos(dTheta);
				dZ = dRadius * sin(dTheta);

				//no texture coords
				
				//set the point
				glVertex3d(dX, 0.0, dZ - ((fLength / 2) - dRadius));
				glVertex3d(dX, TILE_DEPTH, dZ - ((fLength / 2) - dRadius));
			}

			//create the angled part
			glVertex3d(fWidth / 2, 0.00, fLength / 2);
			glVertex3d(fWidth / 2, TILE_DEPTH, fLength / 2);

			glVertex3d(-fWidth / 2, 0.00, fLength / 2);
			glVertex3d(-fWidth / 2, TILE_DEPTH, fLength / 2);

			//end at the first
			dTheta = 122 * D2R;

			//calc coordinates
			dX = dRadius * cos(dTheta);
			dZ = dRadius * sin(dTheta);

			//set the point
			glVertex3d(dX, 0.0, dZ - ((fLength / 2) - dRadius));
			glVertex3d(dX, TILE_DEPTH, dZ - ((fLength / 2) - dRadius));

		glEnd();

	//finish up
	glEndList();
}

/////////////////////////////////////////////////////////////////////////////
// build the extra points display list
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::buildExtra()
{
	//calculate sea port
	glNewList(m_lists[GL_POINT_CHIT], GL_COMPILE);
		//top face
		glBegin(GL_QUADS);
			glTexCoord2d(1.00, 0.00); glVertex3d(-EXTRAWID / 2, EXTRA_DEPTH, -EXTRAWID / 2);
			glTexCoord2d(0.00, 0.00); glVertex3d(EXTRAWID / 2, EXTRA_DEPTH, -EXTRAWID / 2);
			glTexCoord2d(0.00, 1.00); glVertex3d(EXTRAWID / 2, EXTRA_DEPTH, EXTRAWID / 2);
			glTexCoord2d(1.00, 1.00); glVertex3d(-EXTRAWID / 2, EXTRA_DEPTH, EXTRAWID / 2);
		glEnd();

		//side faces (no tex coords)
		glBegin(GL_QUADS);
			glVertex3d(-EXTRAWID / 2, 0.0, -EXTRAWID / 2);
			glVertex3d(-EXTRAWID / 2, EXTRA_DEPTH, -EXTRAWID / 2);
			glVertex3d(-EXTRAWID / 2, EXTRA_DEPTH, EXTRAWID / 2);
			glVertex3d(-EXTRAWID / 2, 0.0, EXTRAWID / 2);

			glVertex3d(EXTRAWID / 2, 0.0, -EXTRAWID / 2);
			glVertex3d(EXTRAWID / 2, EXTRA_DEPTH, -EXTRAWID / 2);
			glVertex3d(EXTRAWID / 2, EXTRA_DEPTH, EXTRAWID / 2);
			glVertex3d(EXTRAWID / 2, 0.0, EXTRAWID / 2);

			glVertex3d(-EXTRAWID / 2, 0.0, -EXTRAWID / 2);
			glVertex3d(-EXTRAWID / 2, EXTRA_DEPTH, -EXTRAWID / 2);
			glVertex3d(EXTRAWID / 2, EXTRA_DEPTH, -EXTRAWID / 2);
			glVertex3d(EXTRAWID / 2, 0.0, -EXTRAWID / 2);

			glVertex3d(-EXTRAWID / 2, 0.0, EXTRAWID / 2);
			glVertex3d(-EXTRAWID / 2, EXTRA_DEPTH, EXTRAWID / 2);
			glVertex3d(EXTRAWID / 2, EXTRA_DEPTH, EXTRAWID / 2);
			glVertex3d(EXTRAWID / 2, 0.0, EXTRAWID / 2);
		glEnd();

	//finish up
	glEndList();
}

/////////////////////////////////////////////////////////////////////////////
// build the sea border display list
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::buildBorders()
{
	//because GL can only handle convex polygons, these objects are sometimes
	//split into more than one polygon to ensure that all are convex

	//AA corner
	glNewList(m_lists[GL_BORDER_AA], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-X * 2.25, 0.0, -Z * 1.12);
			glTexCoord2d(0.00, 0.45); glVertex3d(-X * 2.25, 0.0, Z * 0.12);	
			glTexCoord2d(0.12, 0.00); glVertex3d(-X * 1.75, 0.0, Z * 1.12);
			glTexCoord2d(0.33, 0.00); glVertex3d(-X * 0.75, 0.0, Z * 1.12);	
			glTexCoord2d(0.44, 0.45); glVertex3d(-X * 0.25, 0.0, Z * 0.12);
			glTexCoord2d(0.44, 1.00); glVertex3d(-X * 0.25, 0.0, -Z * 1.12);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.44, 1.00); glVertex3d(-X * 0.25, 0.0, -Z * 1.12);
			glTexCoord2d(0.44, 0.45); glVertex3d(-X * 0.25, 0.0, Z * 0.12);
			glTexCoord2d(0.66, 0.45); glVertex3d(X * 0.75, 0.0, Z * 0.12);
			glTexCoord2d(0.66, 1.00); glVertex3d(X * 0.75, 0.0, -Z * 1.12);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.67, 1.00); glVertex3d(X * 0.75, 0.0, -Z * 1.12);
			glTexCoord2d(0.67, 0.45); glVertex3d(X * 0.75, 0.0, Z * 0.12);
			glTexCoord2d(0.81, 0.00); glVertex3d(X * 1.25, 0.0, Z * 1.12);
			glTexCoord2d(1.00, 0.00); glVertex3d(X * 2.25, 0.0, Z * 1.12);
			glTexCoord2d(1.00, 1.00); glVertex3d(X * 2.25, 0.0, -Z * 1.12);
		glEnd();
	glEndList();

	//CC corner
	glNewList(m_lists[GL_BORDER_CC], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-X * 2.25, 0.0, -Z * 1.12);
			glTexCoord2d(0.00, 0.00); glVertex3d(-X * 2.25, 0.0, Z * 1.12);
			glTexCoord2d(0.22, 0.00); glVertex3d(-X * 1.25, 0.0, Z * 1.12);
			glTexCoord2d(0.33, 0.45); glVertex3d(-X * 0.75, 0.0, Z * 0.12);
			glTexCoord2d(0.33, 1.00); glVertex3d(-X * 0.75, 0.0, -Z * 1.12);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.33, 1.00); glVertex3d(-X * 0.75, 0.0, -Z * 1.12);
			glTexCoord2d(0.33, 0.45); glVertex3d(-X * 0.75, 0.0, Z * 0.12);
			glTexCoord2d(0.55, 0.45); glVertex3d(X * 0.25, 0.0, Z * 0.12);
			glTexCoord2d(0.55, 1.00); glVertex3d(X * 0.25, 0.0, -Z * 1.12);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.56, 1.00); glVertex3d(X * 0.25, 0.0, -Z * 1.12);
			glTexCoord2d(0.56, 0.45); glVertex3d(X * 0.25, 0.0, Z * 0.12);
			glTexCoord2d(0.67, 0.00); glVertex3d(X * 0.75, 0.0, Z * 1.12);
			glTexCoord2d(0.88, 0.00); glVertex3d(X * 1.75, 0.0, Z * 1.12);
			glTexCoord2d(1.00, 0.45); glVertex3d(X * 2.25, 0.0, Z * 0.12);
			glTexCoord2d(1.00, 1.00); glVertex3d(X * 2.25, 0.0, -Z * 1.12);
		glEnd();
	glEndList();

	//EE corner
	glNewList(m_lists[GL_BORDER_EE], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-X * 2.25, 0.0, -Z * 1.12);
			glTexCoord2d(0.00, 0.45); glVertex3d(-X * 2.25, 0.0, Z * 0.12);	
			glTexCoord2d(0.12, 0.00); glVertex3d(-X * 1.75, 0.0, Z * 1.12);
			glTexCoord2d(0.33, 0.00); glVertex3d(-X * 0.75, 0.0, Z * 1.12);	
			glTexCoord2d(0.44, 0.45); glVertex3d(-X * 0.25, 0.0, Z * 0.12);
			glTexCoord2d(0.44, 1.00); glVertex3d(-X * 0.25, 0.0, -Z * 1.12);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.44, 1.00); glVertex3d(-X * 0.25, 0.0, -Z * 1.12);
			glTexCoord2d(0.44, 0.45); glVertex3d(-X * 0.25, 0.0, Z * 0.12);
			glTexCoord2d(0.67, 0.45); glVertex3d(X * 0.75, 0.0, Z * 0.12);
			glTexCoord2d(0.67, 1.00); glVertex3d(X * 0.75, 0.0, -Z * 1.12);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.67, 1.00); glVertex3d(X * 0.75, 0.0, -Z * 1.12);
			glTexCoord2d(0.67, 0.45); glVertex3d(X * 0.75, 0.0, Z * 0.12);
			glTexCoord2d(0.78, 0.00); glVertex3d(X * 1.25, 0.0, Z * 1.12);
			glTexCoord2d(1.00, 0.00); glVertex3d(X * 2.25, 0.0, Z * 1.12);
			glTexCoord2d(1.00, 1.00); glVertex3d(X * 2.25, 0.0, -Z * 1.12);
		glEnd();
	glEndList();

	//GG corner
	glNewList(m_lists[GL_BORDER_GG], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-X * 2.25, 0.0, -Z * 1.12);
			glTexCoord2d(0.00, 0.00); glVertex3d(-X * 2.25, 0.0, Z * 1.12);
			glTexCoord2d(0.22, 0.00); glVertex3d(-X * 1.25, 0.0, Z * 1.12);
			glTexCoord2d(0.33, 0.45); glVertex3d(-X * 0.75, 0.0, Z * 0.12);
			glTexCoord2d(0.33, 1.00); glVertex3d(-X * 0.75, 0.0, -Z * 1.12);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.33, 1.00); glVertex3d(-X * 0.75, 0.0, -Z * 1.12);
			glTexCoord2d(0.33, 0.45); glVertex3d(-X * 0.75, 0.0, Z * 0.12);
			glTexCoord2d(0.55, 0.45); glVertex3d(X * 0.25, 0.0, Z * 0.12);
			glTexCoord2d(0.55, 1.00); glVertex3d(X * 0.25, 0.0, -Z * 1.12);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.55, 1.00); glVertex3d(X * 0.25, 0.0, -Z * 1.12);
			glTexCoord2d(0.55, 0.45); glVertex3d(X * 0.25, 0.0, Z * 0.12);
			glTexCoord2d(0.66, 0.00); glVertex3d(X * 0.75, 0.0, Z * 1.12);
			glTexCoord2d(0.88, 0.00); glVertex3d(X * 1.75, 0.0, Z * 1.12);
			glTexCoord2d(1.00, 0.45); glVertex3d(X * 2.25, 0.0, Z * 0.12);
			glTexCoord2d(1.00, 1.00); glVertex3d(X * 2.25, 0.0, -Z * 1.12);
		glEnd();
	glEndList();

	//DE side
	glNewList(m_lists[GL_BORDER_DE], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-X * 2.00, 0.0, -Z * 1.12);
			glTexCoord2d(0.00, 0.45); glVertex3d(-X * 2.00, 0.0, Z * 0.12);
			glTexCoord2d(0.25, 0.45); glVertex3d(-X * 1.00, 0.0, Z * 0.12);
			glTexCoord2d(0.25, 1.00); glVertex3d(-X * 1.00, 0.0, -Z * 1.12);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.25, 1.00); glVertex3d(-X * 1.00, 0.0, -Z * 1.12);
			glTexCoord2d(0.25, 0.45); glVertex3d(-X * 1.00, 0.0, Z * 0.12);
			glTexCoord2d(0.375, 0.00); glVertex3d(-X * 0.50, 0.0, Z * 1.12);
			glTexCoord2d(0.625, 0.00); glVertex3d(X * 0.50, 0.0, Z * 1.12);
			glTexCoord2d(0.75, 0.45); glVertex3d(X * 1.00, 0.0, Z * 0.12);
			glTexCoord2d(0.75, 1.00); glVertex3d(X * 1.00, 0.0, -Z * 1.12);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.75, 1.00); glVertex3d(X * 1.00, 0.0, -Z * 1.12);
			glTexCoord2d(0.75, 0.45); glVertex3d(X * 1.00, 0.0, Z * 0.12);
			glTexCoord2d(1.00, 0.45); glVertex3d(X * 2.00, 0.0, Z * 0.12);
			glTexCoord2d(1.00, 1.00); glVertex3d(X * 2.00, 0.0, -Z * 1.12);
		glEnd();
	glEndList();

	//HA side
	glNewList(m_lists[GL_BORDER_HA], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-X * 2.00, 0.0, -Z * 1.12);
			glTexCoord2d(0.00, 0.45); glVertex3d(-X * 2.00, 0.0, Z * 0.12);
			glTexCoord2d(0.25, 0.45); glVertex3d(-X * 1.00, 0.0, Z * 0.12);
			glTexCoord2d(0.25, 1.00); glVertex3d(-X * 1.00, 0.0, -Z * 1.12);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.26, 1.00); glVertex3d(-X * 1.00, 0.0, -Z * 1.12);
			glTexCoord2d(0.26, 0.45); glVertex3d(-X * 1.00, 0.0, Z * 0.12);
			glTexCoord2d(0.385, 0.00); glVertex3d(-X * 0.50, 0.0, Z * 1.12);
			glTexCoord2d(0.625, 0.00); glVertex3d(X * 0.50, 0.0, Z * 1.12);
			glTexCoord2d(0.75, 0.45); glVertex3d(X * 1.00, 0.0, Z * 0.12);
			glTexCoord2d(0.75, 1.00); glVertex3d(X * 1.00, 0.0, -Z * 1.12);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.75, 1.00); glVertex3d(X * 1.00, 0.0, -Z * 1.12);
			glTexCoord2d(0.75, 0.45); glVertex3d(X * 1.00, 0.0, Z * 0.12);
			glTexCoord2d(1.00, 0.45); glVertex3d(X * 2.00, 0.0, Z * 0.12);
			glTexCoord2d(1.00, 1.00); glVertex3d(X * 2.00, 0.0, -Z * 1.12);
		glEnd();
	glEndList();

	//ABC side
	glNewList(m_lists[GL_BORDER_ABC], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-Z * 4.0, 0.0, -0.75 * X);
			glTexCoord2d(0.00, 0.33); glVertex3d(-Z * 4.0, 0.0, 0.25 * X);
			glTexCoord2d(0.125, 0.00); glVertex3d(-Z * 3.0, 0.0, 0.75 * X);
			glTexCoord2d(0.25, 0.33); glVertex3d(-Z * 2.0, 0.0, 0.25 * X);
			glTexCoord2d(0.25, 1.00); glVertex3d(-Z * 2.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.25, 1.00); glVertex3d(-Z * 2.0, 0.0, -0.75 * X);
			glTexCoord2d(0.25, 0.34); glVertex3d(-Z * 2.0, 0.0, 0.25 * X);
			glTexCoord2d(0.375, 0.00); glVertex3d(-Z * 1.0, 0.0, 0.75 * X);
			glTexCoord2d(0.50, 0.34); glVertex3d(0.0, 0.0, 0.25 * X);
			glTexCoord2d(0.50, 1.00); glVertex3d(0.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.50, 1.00); glVertex3d(0.0, 0.0, -0.75 * X);
			glTexCoord2d(0.50, 0.33); glVertex3d(0.0, 0.0, 0.25 * X);
			glTexCoord2d(0.625, 0.00); glVertex3d(Z * 1.0, 0.0, 0.75 * X);
			glTexCoord2d(0.75, 0.33); glVertex3d(Z * 2.0, 0.0, 0.25 * X);
			glTexCoord2d(0.75, 1.00); glVertex3d(Z * 2.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.75, 1.00); glVertex3d(Z * 2.0, 0.0, -0.75 * X);
			glTexCoord2d(0.75, 0.33); glVertex3d(Z * 2.0, 0.0, 0.25 * X);
			glTexCoord2d(0.875, 0.00); glVertex3d(Z * 3.0, 0.0, 0.75 * X);
			glTexCoord2d(1.00, 0.33); glVertex3d(Z * 4.0, 0.0, 0.25 * X);
			glTexCoord2d(1.00, 1.00); glVertex3d(Z * 4.0, 0.0, -0.75 * X);
		glEnd();
	glEndList();

	//EFG side
	glNewList(m_lists[GL_BORDER_EFG], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-Z * 4.0, 0.0, -0.75 * X);
			glTexCoord2d(0.00, 0.33); glVertex3d(-Z * 4.0, 0.0, 0.25 * X);
			glTexCoord2d(0.125, 0.00); glVertex3d(-Z * 3.0, 0.0, 0.75 * X);
			glTexCoord2d(0.25, 0.33); glVertex3d(-Z * 2.0, 0.0, 0.25 * X);
			glTexCoord2d(0.25, 1.00); glVertex3d(-Z * 2.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.25, 1.00); glVertex3d(-Z * 2.0, 0.0, -0.75 * X);
			glTexCoord2d(0.25, 0.34); glVertex3d(-Z * 2.0, 0.0, 0.25 * X);
			glTexCoord2d(0.375, 0.00); glVertex3d(-Z * 1.0, 0.0, 0.75 * X);
			glTexCoord2d(0.50, 0.34); glVertex3d(0.0, 0.0, 0.25 * X);
			glTexCoord2d(0.50, 1.00); glVertex3d(0.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.50, 1.00); glVertex3d(0.0, 0.0, -0.75 * X);
			glTexCoord2d(0.50, 0.34); glVertex3d(0.0, 0.0, 0.25 * X);
			glTexCoord2d(0.625, 0.00); glVertex3d(Z * 1.0, 0.0, 0.75 * X);
			glTexCoord2d(0.75, 0.34); glVertex3d(Z * 2.0, 0.0, 0.25 * X);
			glTexCoord2d(0.75, 1.00); glVertex3d(Z * 2.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.75, 1.00); glVertex3d(Z * 2.0, 0.0, -0.75 * X);
			glTexCoord2d(0.75, 0.33); glVertex3d(Z * 2.0, 0.0, 0.25 * X);
			glTexCoord2d(0.875, 0.00); glVertex3d(Z * 3.0, 0.0, 0.75 * X);
			glTexCoord2d(1.00, 0.34); glVertex3d(Z * 4.0, 0.0, 0.25 * X);
			glTexCoord2d(1.00, 1.00); glVertex3d(Z * 4.0, 0.0, -0.75 * X);
		glEnd();
	glEndList();

	//XXAB side
	glNewList(m_lists[GL_BORDER_XXAB], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-Z * 3.0, 0.0, -0.75 * X);
			glTexCoord2d(0.00, 0.34); glVertex3d(-Z * 3.0, 0.0, 0.25 * X);
			glTexCoord2d(0.17, 0.01); glVertex3d(-Z * 2.0, 0.0, 0.75 * X);
			glTexCoord2d(0.33, 0.34); glVertex3d(-Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 1.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 1.0, 0.0, -0.75 * X);
			glTexCoord2d(0.33, 0.34); glVertex3d(-Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.50, 0.01); glVertex3d(0.0, 0.0, 0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 1.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 1.0, 0.0, -0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.84, 0.01); glVertex3d(Z * 2.0, 0.0, 0.75 * X);
			glTexCoord2d(1.00, 0.34); glVertex3d(Z * 3.0, 0.0, 0.25 * X);
			glTexCoord2d(1.00, 1.00); glVertex3d(Z * 3.0, 0.0, -0.75 * X);
		glEnd();
	glEndList();

	//XXBC side
	glNewList(m_lists[GL_BORDER_XXBC], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-Z * 3.0, 0.0, -0.75 * X);
			glTexCoord2d(0.00, 0.33); glVertex3d(-Z * 3.0, 0.0, 0.25 * X);
			glTexCoord2d(0.17, 0.00); glVertex3d(-Z * 2.0, 0.0, 0.75 * X);
			glTexCoord2d(0.33, 0.33); glVertex3d(-Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 1.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 1.0, 0.0, -0.75 * X);
			glTexCoord2d(0.33, 0.33); glVertex3d(-Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.50, 0.00); glVertex3d(0.0, 0.0, 0.75 * X);
			glTexCoord2d(0.66, 0.33); glVertex3d(Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 1.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 1.0, 0.0, -0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.84, 0.01); glVertex3d(Z * 2.0, 0.0, 0.75 * X);
			glTexCoord2d(1.00, 0.34); glVertex3d(Z * 3.0, 0.0, 0.25 * X);
			glTexCoord2d(1.00, 1.00); glVertex3d(Z * 3.0, 0.0, -0.75 * X);
		glEnd();
	glEndList();

	//XXEF side
	glNewList(m_lists[GL_BORDER_XXEF], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-Z * 3.0, 0.0, -0.75 * X);
			glTexCoord2d(0.00, 0.34); glVertex3d(-Z * 3.0, 0.0, 0.25 * X);
			glTexCoord2d(0.17, 0.01); glVertex3d(-Z * 2.0, 0.0, 0.75 * X);
			glTexCoord2d(0.33, 0.34); glVertex3d(-Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 1.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 1.0, 0.0, -0.75 * X);
			glTexCoord2d(0.33, 0.34); glVertex3d(-Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.50, 0.01); glVertex3d(0.0, 0.0, 0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 1.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 1.0, 0.0, -0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.84, 0.01); glVertex3d(Z * 2.0, 0.0, 0.75 * X);
			glTexCoord2d(1.00, 0.34); glVertex3d(Z * 3.0, 0.0, 0.25 * X);
			glTexCoord2d(1.00, 1.00); glVertex3d(Z * 3.0, 0.0, -0.75 * X);
		glEnd();
	glEndList();

	//XXFG side
	glNewList(m_lists[GL_BORDER_XXFG], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-Z * 3.0, 0.0, -0.75 * X);
			glTexCoord2d(0.00, 0.34); glVertex3d(-Z * 3.0, 0.0, 0.25 * X);
			glTexCoord2d(0.17, 0.01); glVertex3d(-Z * 2.0, 0.0, 0.75 * X);
			glTexCoord2d(0.33, 0.34); glVertex3d(-Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 1.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 1.0, 0.0, -0.75 * X);
			glTexCoord2d(0.33, 0.34); glVertex3d(-Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.50, 0.01); glVertex3d(0.0, 0.0, 0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 1.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 1.0, 0.0, -0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 1.0, 0.0, 0.25 * X);
			glTexCoord2d(0.84, 0.01); glVertex3d(Z * 2.0, 0.0, 0.75 * X);
			glTexCoord2d(1.00, 0.34); glVertex3d(Z * 3.0, 0.0, 0.25 * X);
			glTexCoord2d(1.00, 1.00); glVertex3d(Z * 3.0, 0.0, -0.75 * X);
		glEnd();
	glEndList();

	//XAB side
	glNewList(m_lists[GL_BORDER_XAB], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-Z * 2.5, 0.0, -0.75 * X);
			glTexCoord2d(0.00, 0.34); glVertex3d(-Z * 2.5, 0.0, 0.25 * X);
			glTexCoord2d(0.17, 0.01); glVertex3d(-Z * 1.5, 0.0, 0.75 * X);
			glTexCoord2d(0.33, 0.34); glVertex3d(-Z * 0.5, 0.0, 0.25 * X);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 0.5, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 0.5, 0.0, -0.75 * X);
			glTexCoord2d(0.33, 0.34); glVertex3d(-Z * 0.5, 0.0, 0.25 * X);
			glTexCoord2d(0.50, 0.01); glVertex3d(Z * 0.5, 0.0, 0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 1.5, 0.0, 0.25 * X);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 1.5, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 1.5, 0.0, -0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 1.5, 0.0, 0.25 * X);
			glTexCoord2d(0.84, 0.01); glVertex3d(Z * 2.5, 0.0, 0.75 * X);
			glTexCoord2d(0.84, 1.00); glVertex3d(Z * 2.5, 0.0, -0.75 * X);
		glEnd();
	glEndList();

	//XBC side
	glNewList(m_lists[GL_BORDER_XBC], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.17, 1.00); glVertex3d(-Z * 2.5, 0.0, -0.75 * X);
			glTexCoord2d(0.17, 0.00); glVertex3d(-Z * 2.5, 0.0, 0.75 * X);
			glTexCoord2d(0.33, 0.33); glVertex3d(-Z * 1.5, 0.0, 0.25 * X);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 1.5, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 1.5, 0.0, -0.75 * X);
			glTexCoord2d(0.33, 0.33); glVertex3d(-Z * 1.5, 0.0, 0.25 * X);
			glTexCoord2d(0.50, 0.00); glVertex3d(-Z * 0.5, 0.0, 0.75 * X);
			glTexCoord2d(0.66, 0.33); glVertex3d(Z * 0.5, 0.0, 0.25 * X);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 0.5, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 0.5, 0.0, -0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 0.5, 0.0, 0.25 * X);
			glTexCoord2d(0.84, 0.01); glVertex3d(Z * 1.5, 0.0, 0.75 * X);
			glTexCoord2d(1.00, 0.34); glVertex3d(Z * 2.5, 0.0, 0.25 * X);
			glTexCoord2d(1.00, 1.00); glVertex3d(Z * 2.5, 0.0, -0.75 * X);
		glEnd();
	glEndList();

	//XEF side
	glNewList(m_lists[GL_BORDER_XEF], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-Z * 2.5, 0.0, -0.75 * X);
			glTexCoord2d(0.00, 0.34); glVertex3d(-Z * 2.5, 0.0, 0.25 * X);
			glTexCoord2d(0.17, 0.01); glVertex3d(-Z * 1.5, 0.0, 0.75 * X);
			glTexCoord2d(0.33, 0.34); glVertex3d(-Z * 0.5, 0.0, 0.25 * X);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 0.5, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 0.5, 0.0, -0.75 * X);
			glTexCoord2d(0.33, 0.34); glVertex3d(-Z * 0.5, 0.0, 0.25 * X);
			glTexCoord2d(0.50, 0.01); glVertex3d(Z * 0.5, 0.0, 0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 1.5, 0.0, 0.25 * X);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 1.5, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 1.5, 0.0, -0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 1.5, 0.0, 0.25 * X);
			glTexCoord2d(0.84, 0.01); glVertex3d(Z * 2.5, 0.0, 0.75 * X);
			glTexCoord2d(0.84, 1.00); glVertex3d(Z * 2.5, 0.0, -0.75 * X);
		glEnd();
	glEndList();

	//XFG side
	glNewList(m_lists[GL_BORDER_XFG], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.17, 1.00); glVertex3d(-Z * 2.5, 0.0, -0.75 * X);
			glTexCoord2d(0.17, 0.01); glVertex3d(-Z * 2.5, 0.0, 0.75 * X);
			glTexCoord2d(0.33, 0.34); glVertex3d(-Z * 1.5, 0.0, 0.25 * X);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 1.5, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.33, 1.00); glVertex3d(-Z * 1.5, 0.0, -0.75 * X);
			glTexCoord2d(0.33, 0.34); glVertex3d(-Z * 1.5, 0.0, 0.25 * X);
			glTexCoord2d(0.50, 0.01); glVertex3d(-Z * 0.5, 0.0, 0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 0.5, 0.0, 0.25 * X);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 0.5, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.66, 1.00); glVertex3d(Z * 0.5, 0.0, -0.75 * X);
			glTexCoord2d(0.66, 0.34); glVertex3d(Z * 0.5, 0.0, 0.25 * X);
			glTexCoord2d(0.84, 0.01); glVertex3d(Z * 1.5, 0.0, 0.75 * X);
			glTexCoord2d(1.00, 0.34); glVertex3d(Z * 2.5, 0.0, 0.25 * X);
			glTexCoord2d(1.00, 1.00); glVertex3d(Z * 2.5, 0.0, -0.75 * X);
		glEnd();
	glEndList();

	//X_XX side
	glNewList(m_lists[GL_BORDER_X_XX], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.25, 1.00); glVertex3d(-Z * 1.5, 0.0, -0.75 * X);
			glTexCoord2d(0.25, 0.01); glVertex3d(-Z * 1.5, 0.0, 0.75 * X);
			glTexCoord2d(0.50, 0.34); glVertex3d(-Z * 0.5, 0.0, 0.25 * X);
			glTexCoord2d(0.50, 1.00); glVertex3d(-Z * 0.5, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.50, 1.00); glVertex3d(-Z * 0.5, 0.0, -0.75 * X);
			glTexCoord2d(0.50, 0.34); glVertex3d(-Z * 0.5, 0.0, 0.25 * X);
			glTexCoord2d(0.75, 0.01); glVertex3d(Z * 0.5, 0.0, 0.75 * X);
			glTexCoord2d(1.00, 0.34); glVertex3d(Z * 1.5, 0.0, 0.25 * X);
			glTexCoord2d(1.00, 1.00); glVertex3d(Z * 1.5, 0.0, -0.75 * X);
		glEnd();
	glEndList();

	//XX_X side
	glNewList(m_lists[GL_BORDER_XX_X], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-Z * 1.5, 0.0, -0.75 * X);
			glTexCoord2d(0.00, 0.34); glVertex3d(-Z * 1.5, 0.0, 0.25 * X);
			glTexCoord2d(0.25, 0.01); glVertex3d(-Z * 0.5, 0.0, 0.75 * X);
			glTexCoord2d(0.50, 0.34); glVertex3d(Z * 0.5, 0.0, 0.25 * X);
			glTexCoord2d(0.50, 1.00); glVertex3d(Z * 0.5, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.50, 1.00); glVertex3d(Z * 0.5, 0.0, -0.75 * X);
			glTexCoord2d(0.50, 0.34); glVertex3d(Z * 0.5, 0.0, 0.25 * X);
			glTexCoord2d(0.75, 0.01); glVertex3d(Z * 1.5, 0.0, 0.75 * X);
			glTexCoord2d(0.75, 1.00); glVertex3d(Z * 1.5, 0.0, -0.75 * X);
		glEnd();
	glEndList();

	//XX side
	glNewList(m_lists[GL_BORDER_XX_XX], GL_COMPILE);
		glBegin(GL_POLYGON);
			glTexCoord2d(0.00, 1.00); glVertex3d(-Z * 2.0, 0.0, -0.75 * X);
			glTexCoord2d(0.00, 0.33); glVertex3d(-Z * 2.0, 0.0, 0.25 * X);
			glTexCoord2d(0.25, 0.00); glVertex3d(-Z * 1.0, 0.0, 0.75 * X);
			glTexCoord2d(0.50, 0.33); glVertex3d(-Z * 0.0, 0.0, 0.25 * X);
			glTexCoord2d(0.50, 1.00); glVertex3d(-Z * 0.0, 0.0, -0.75 * X);
		glEnd();
		glBegin(GL_POLYGON);
			glTexCoord2d(0.50, 1.00); glVertex3d(Z * 0.0, 0.0, -0.75 * X);
			glTexCoord2d(0.50, 0.33); glVertex3d(Z * 0.0, 0.0, 0.25 * X);
			glTexCoord2d(0.75, 0.00); glVertex3d(Z * 1.0, 0.0, 0.75 * X);
			glTexCoord2d(1.00, 0.33); glVertex3d(Z * 2.0, 0.0, 0.25 * X);
			glTexCoord2d(1.00, 1.00); glVertex3d(Z * 2.0, 0.0, -0.75 * X);
		glEnd();
	glEndList();
}