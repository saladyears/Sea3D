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
// Maps3D defines
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_MAPS3D_H)
#define DEFINE_MAPS3D_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// tex defines
/////////////////////////////////////////////////////////////////////////////
#define TEX_LAND_SIZE		7
#define TEX_SEA_SIZE		7
#define TEX_SPECIAL_SIZE	2
#define TEX_CHITS_SIZE		10

/////////////////////////////////////////////////////////////////////////////
// image list defines
/////////////////////////////////////////////////////////////////////////////
#define IMAGE_LAND_16		0
#define IMAGE_LAND_32		1
#define IMAGE_LAND_64		2
#define IMAGE_SEA_16		3
#define IMAGE_SEA_32		4
#define IMAGE_SEA_64		5
#define IMAGE_SPECIAL_16	6
#define IMAGE_SPECIAL_32	7
#define IMAGE_SPECIAL_64	8
#define IMAGE_CHITS_16		9
#define IMAGE_CHITS_32		10
#define IMAGE_CHITS_64		11

/////////////////////////////////////////////////////////////////////////////
// GL drawing states
/////////////////////////////////////////////////////////////////////////////
#define GL_DRAW_NORMAL			0
#define GL_DRAW_RES_PAINT		1
#define GL_DRAW_PORT_PAINT		2
#define GL_DRAW_CHIT_PAINT		3

/////////////////////////////////////////////////////////////////////////////
// GL draw bits
/////////////////////////////////////////////////////////////////////////////
#define GL_TILE_BIT_LIGHT			(1)
#define GL_TILE_BIT_SELECT			(2)
#define GL_TILE_BIT_SEL_OUTLINE		(4)

/////////////////////////////////////////////////////////////////////////////
// special tab values
/////////////////////////////////////////////////////////////////////////////
#define SPECIAL_PORTS			0
#define SPECIAL_RANDOM			1
#define SPECIAL_HIDDEN			2
#define SPECIAL_CHITLESS		3

#endif	//DEFINE_MAPS3D_H