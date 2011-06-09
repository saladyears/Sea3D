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
#include "define.h"
#include "defineCode.h"

/////////////////////////////////////////////////////////////////////////////
// adjust a placement disc to draw on a corner
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::adjust(int i, double &x, double &z)
{
	switch(i)
	{
	//TOP LEFT
	case 0: x = -X / 2; z = -Z; break;
	//LEFT
	case 1: x = -X; z = 0.; break;
	//BOTTOM LEFT
	case 2: x = -X / 2; z = Z; break;
	//BOTTOM RIGHT
	case 3: x = X / 2; z = Z; break;
	//TOP RIGHT
	case 4: x = X; z = 0.; break;
	//TOP
	case 5: x = X / 2; z = -Z; break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// adjust a road to fit a side
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::adjustroad(int i, double &x, double &z)
{
	switch(i)
	{
	//TOP LEFT
	case 0: x = -(X * .75); z = -(Z / 2); break;
	//BOTTOM LEFT
	case 1: x = -(X * .75); z = (Z / 2); break;
	//BOTTOM
	case 2: x = 0.; z = Z; break;
	//BOTTOM RIGHT
	case 3: x = (X * .75); z = (Z / 2); break;
	//TOP RIGHT
	case 4: x = (X * .75); z = -(Z / 2); break;
	//TOP
	case 5: x = 0.; z = -Z; break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// robber code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::robber(double x, double y, double z)
{
	glPushMatrix();
		glTranslated(x, y, z);
		glScaled(1.2, 1.2, 1.2);
		if(TRUE == m_bGouraud)
		{
			glCallList(m_lists[GL_ROBBER_GOURAUD]);
		}
		else
		{
			glCallList(m_lists[GL_ROBBER]);
		}
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// road code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::road(double x, double y, double z, double dRot)
{
	glPushMatrix();
		glTranslated(x, y, z);
		glRotated(dRot, 0.0, 1.0, 0.0);

		if(GL_HIGH_DETAIL <= m_iDetail)
		{
			glBindTexture(GL_TEXTURE_2D, m_texPieces[GL_PIECE_ROAD]);
		}
		glCallList(m_lists[GL_ROAD]);

		if(GL_HIGH_DETAIL <= m_iDetail)
		{
			glBindTexture(GL_TEXTURE_2D, m_texPieces[GL_PIECE_ROADA]);
		}
		glCallList(m_lists[GL_ROADA]);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// ship code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::ship(double x, double y, double z, double dRot)
{
	glPushMatrix();
		glTranslated(x, y, z);
		glRotated(dRot, 0.0, 1.0, 0.0);

		if(GL_HIGH_DETAIL <= m_iDetail)
		{
			glBindTexture(GL_TEXTURE_2D, m_texPieces[GL_PIECE_SHIPA]);
		}
		glCallList(m_lists[GL_SHIPA]);

		if(GL_HIGH_DETAIL <= m_iDetail)
		{
			glBindTexture(GL_TEXTURE_2D, m_texPieces[GL_PIECE_SHIP]);
		}
		glCallList(m_lists[GL_SHIP]);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// ship outline code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::shipoutline(double x, double y, double z, double dRot)
{
	glPushMatrix();
		glTranslated(x, y, z);
		glRotated(dRot, 0.0, 1.0, 0.0);
		glCallList(m_lists[GL_SHIP_OUTLINE]);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// hexagon code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::hexagon(double x, double y, double z, double dRot)
{
	glPushMatrix();
		glTranslated(x, y, z);
		glRotated(dRot, 0.0, 1.0, 0.0);
		glCallList(m_lists[GL_HEX_POLYGON]);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// hexagon fan code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::hexagonfan(double x, double y, double z, double dRot, int iName)
{
	glPushMatrix();
		glTranslated(x, y, z);
		glRotated(dRot, 0.0, 1.0, 0.0);
		//call the lists
		glLoadName(ENCODE_SEL(iName, 1));
		glCallList(m_lists[GL_HEX_TRIANGLE1]);
		glLoadName(ENCODE_SEL(iName, 2));
		glCallList(m_lists[GL_HEX_TRIANGLE2]);
		glLoadName(ENCODE_SEL(iName, 3));
		glCallList(m_lists[GL_HEX_TRIANGLE3]);
		glLoadName(ENCODE_SEL(iName, 4));
		glCallList(m_lists[GL_HEX_TRIANGLE4]);
		glLoadName(ENCODE_SEL(iName, 5));
		glCallList(m_lists[GL_HEX_TRIANGLE5]);
		glLoadName(ENCODE_SEL(iName, 6));
		glCallList(m_lists[GL_HEX_TRIANGLE6]);
		glLoadName(0);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// hexagon outline code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::hexagonoutline(double x, double y, double z)
{
	glPushMatrix();
		glTranslated(x, y, z);
		glCallList(m_lists[GL_HEX_OUTLINE]);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// settlement code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::settlement(double x, double y, double z)
{
	glPushMatrix();
		glTranslated(x, y, z);

		if(GL_HIGH_DETAIL <= m_iDetail)
		{
			glBindTexture(GL_TEXTURE_2D, m_texPieces[GL_PIECE_SETTLE]);
		}
		glCallList(m_lists[GL_SETTLEMENT]);

		if(GL_HIGH_DETAIL <= m_iDetail)
		{
			glBindTexture(GL_TEXTURE_2D, m_texPieces[GL_PIECE_SETTLEA]);
		}
		glCallList(m_lists[GL_SETTLEMENTA]);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// city code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::city(double x, double y, double z)
{
	glPushMatrix();
		glTranslated(x, y, z);
		if(GL_HIGH_DETAIL <= m_iDetail)
		{
			glBindTexture(GL_TEXTURE_2D, m_texPieces[GL_PIECE_CITY]);
		}

		glCallList(m_lists[GL_CITY]);

		if(GL_HIGH_DETAIL <= m_iDetail)
		{
			glBindTexture(GL_TEXTURE_2D, m_texPieces[GL_PIECE_CITYA]);
		}

		glCallList(m_lists[GL_CITYA]);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// disc code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::disc(double x, double y, double z, double dScale)
{
	glPushMatrix();
		glTranslated(x, y, z);
		glRotated(90.0, -1.0, 0.0, 0.0);
		glScaled(dScale, dScale, dScale);
		gluDisk(m_pQuad, 0.0, CHIPDIAM / 2, 20, 5);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// ring code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::ring(double x, double y, double z, double dThickness, double dScale)
{
	glPushMatrix();
		glTranslated(x, y, z);
		glRotated(90.0, -1.0, 0.0, 0.0);
		glScaled(dScale, dScale, dScale);
		gluDisk(m_pQuad, CHIPDIAM / 2, (CHIPDIAM / 2) + dThickness, 20, 5);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// cylinder code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::cylinder(double x, double y, double z, double dScale)
{
	glPushMatrix();
		glTranslated(x, 0., z);
		glRotated(-90.0, 1.0, 0.0, 0.0);
		glScaled(dScale, dScale, dScale);
		gluCylinder(m_pQuad, CHIPDIAM / 2, CHIPDIAM / 2, y, 20, 5);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// disc outline code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::discoutline(double x, double y, double z, double dScale)
{
	glPushMatrix();
		glTranslated(x, y, z);
		glRotated(90.0, 1.0, 0.0, 0.0);
		glScaled(dScale, dScale, dScale);
		gluDisk(m_pQuad, 1.3, 1.5, 10, 5);
		glScaled(1.0, 1.0, 1.0);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// seaport code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::seaport(double x, double y, double z, double dRot)
{
	glPushMatrix();
		glTranslated(x, y, z);
		glRotated(dRot, 0.0, 1.0, 0.0);
		glTranslated(0.0, 0.0, 1.0);
		glCallList(m_lists[GL_SEAPORT]);
		glCallList(m_lists[GL_SEAPORT_EDGE]);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// border code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::border(int iBorder, double x, double y, double z, double dRot)
{
	glPushMatrix();
		glTranslated(x, y, z);
		glRotated(dRot, 0.0, 1.0, 0.0);
		glCallList(m_lists[iBorder]);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// extra point code
/////////////////////////////////////////////////////////////////////////////
void CGLBaseView::extra(double x, double y, double z, double dRot)
{
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, m_texSeaPorts[GL_TEX_SEAPORTS - 1]);
		glTranslated(x, y, z);
		glRotated(dRot, 0.0, 1.0, 0.0);
		glCallList(m_lists[GL_POINT_CHIT]);
	glPopMatrix();
}