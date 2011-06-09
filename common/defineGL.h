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
// OpenGL defines
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_GL_H)
#define DEFINE_GL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// GL drawing modes
/////////////////////////////////////////////////////////////////////////////
#define GL_NORMAL				0
#define GL_PLACE_PLAYERS		4
#define GL_WAIT					6
#define GL_SHOW_ROLLED			7
#define GL_PLACE_ROAD			8
#define GL_PLACE_SHIP			9
#define GL_PLACE_CITY			10
#define GL_PLACE_SETTLEMENT		11
#define GL_PLACE_ROBBER			12
#define GL_CHOOSE_SHIP			13
#define GL_MOVE_SHIP			14
#define GL_PLACE_PORT			15
#define GL_CHITLESS_TILE		16
#define GL_VOLCANO				17
#define GL_GAME_OVER			18
#define GL_PLACE_THIRD_ROAD		19

/////////////////////////////////////////////////////////////////////////////
// GL draw bits
/////////////////////////////////////////////////////////////////////////////
#define	GL_BIT_LIGHT			1
#define GL_BIT_TILE_SELECT		2
#define GL_BIT_HEX_FAN			4
#define GL_BIT_HIDE_CHIPS		8
#define GL_BIT_OUTLINE_CITY		16
#define GL_BIT_INVERT_CITY		32
#define GL_BIT_INVERT_ROAD		64
#define GL_BIT_MOVE_SHIP		128
#define GL_BIT_DIM_BORDERS		256
#define GL_BIT_SEVEN_SELECT		512
#define GL_BIT_PORT_SELECT		1024
#define GL_BIT_CHITLESS_SELECT	2048
#define GL_BIT_VOLCANO			4096

/////////////////////////////////////////////////////////////////////////////
// GL display lists
/////////////////////////////////////////////////////////////////////////////
#define GL_DISPLAY_LISTS	42

//seafarers borders
#define GL_BORDER_AA		0
#define GL_BORDER_CC		1
#define GL_BORDER_EE		2
#define GL_BORDER_GG		3
#define GL_BORDER_DE		4
#define GL_BORDER_HA		5
#define GL_BORDER_ABC		6
#define GL_BORDER_EFG		7
#define GL_BORDER_XXAB		8
#define GL_BORDER_XXBC		9
#define GL_BORDER_XXEF		10
#define GL_BORDER_XXFG		11
#define GL_BORDER_XAB		12
#define GL_BORDER_XBC		13
#define GL_BORDER_XEF		14
#define GL_BORDER_XFG		15
#define GL_BORDER_X_XX		16
#define GL_BORDER_XX_X		17
#define GL_BORDER_XX_XX		18

#define GL_HEX_POLYGON		19
#define GL_HEX_TRIANGLE1	20
#define GL_HEX_TRIANGLE2	21
#define GL_HEX_TRIANGLE3	22
#define GL_HEX_TRIANGLE4	23
#define GL_HEX_TRIANGLE5	24
#define GL_HEX_TRIANGLE6	25
#define GL_HEX_OUTLINE		26
#define GL_SETTLEMENT		27
#define GL_SETTLEMENTA		28
#define GL_CITY				29
#define GL_CITYA			30
#define GL_ROAD				31
#define GL_ROADA			32
#define GL_TILE_EDGE		33
#define GL_ROBBER			34
#define GL_ROBBER_GOURAUD	35
#define GL_SEAPORT			36
#define GL_SEAPORT_EDGE		37
#define GL_SHIP				38
#define GL_SHIPA			39
#define GL_SHIP_OUTLINE		40
#define GL_POINT_CHIT		41

//not a display list, but needed for tiny windows
#define GL_DEVCARD			42
#define GL_PIRATE			43
#define GL_SHIP_MOVE		44

/////////////////////////////////////////////////////////////////////////////
// GL drawing defines
/////////////////////////////////////////////////////////////////////////////
#define X				5.0
#define Z				4.3301270189221932338186158537647
#define D2R				0.017451587f	//(PI/180)
#define R2D				57.272729876f	//(180/PI)
#define GL_AMB_MULT		0.85
#define SCALE			1.1111111111111111111111111111111 //(X / 4.5) maps GL scale to board game scale
#define GL_ROBBER_SLICE	15
#define GL_ROBBER_STACK	20
#define TILE_DEPTH		0.23
#define EXTRA_DEPTH		0.10

/////////////////////////////////////////////////////////////////////////////
// settlement defines
/////////////////////////////////////////////////////////////////////////////
#define SLEN			(1.4 * SCALE)
#define SWID			(1.0 * SCALE)
#define SHIGH			(0.7 * SCALE)
#define SPEAK			(1.2 * SCALE)

/////////////////////////////////////////////////////////////////////////////
// ship defines
/////////////////////////////////////////////////////////////////////////////
#define SHWID			(0.8 * SCALE)
#define SHTOP			(1.5 * SCALE)
#define SHMID			(0.5 * SCALE)
#define SHLEN			(1.8 * SCALE)
#define SHMAST			(0.5 * SCALE)
#define SHHULLR			(0.3 * SCALE)
#define SHHULLRB		(0.4 * SCALE)
#define SHMASTR			(0.5 * SCALE)
#define SHSLICES		(6)

/////////////////////////////////////////////////////////////////////////////
// city defines
/////////////////////////////////////////////////////////////////////////////
#define CLEN			(2.0 * SCALE)
#define CWID			(1.0 * SCALE)
#define CLOW			(0.8 * SCALE)
#define CHIGH			(1.5 * SCALE)
#define CPEAK			(2.0 * SCALE)

/////////////////////////////////////////////////////////////////////////////
// road defines
/////////////////////////////////////////////////////////////////////////////
#define RLEN			(2.5 * SCALE)
#define RWID			(0.5 * SCALE)
#define RHIGH			(0.5 * SCALE)

/////////////////////////////////////////////////////////////////////////////
// chip defines
/////////////////////////////////////////////////////////////////////////////
#define CHIPDIAM		(2.6 * SCALE)

/////////////////////////////////////////////////////////////////////////////
// extra point chit defines
/////////////////////////////////////////////////////////////////////////////
#define EXTRAWID		(1.95 * SCALE)

/////////////////////////////////////////////////////////////////////////////
// GL texture defines
/////////////////////////////////////////////////////////////////////////////
#define GL_TEX_LARGE		0
#define GL_TEX_MED			1
#define GL_TEX_SMALL		2
#define GL_TEX_TINY			3
#define GL_BASE_TEXTURES	RES_SIZE
#define GL_TEX_CHIP34		19
#define GL_TEX_CHIP56		28
#define GL_TEX_CHIPSEA		10
#define GL_TEX_PIECES		8
#define GL_TEX_ABOUT		6
#define GL_TEX_SEAPORTS		7
#define GL_SEAPORT_OFFSET	8
#define GL_TEX_SEABORDERS	14

/////////////////////////////////////////////////////////////////////////////
// GL piece texture defines
/////////////////////////////////////////////////////////////////////////////
#define GL_PIECE_CITY		0
#define GL_PIECE_CITYA		1
#define GL_PIECE_SETTLE		2
#define GL_PIECE_SETTLEA	3
#define GL_PIECE_ROAD		4
#define GL_PIECE_ROADA		5
#define GL_PIECE_SHIP		6
#define GL_PIECE_SHIPA		7

/////////////////////////////////////////////////////////////////////////////
// GL detail settings
/////////////////////////////////////////////////////////////////////////////
#define GL_DETAIL_LEVELS	5
#define GL_NO_DETAIL		0
#define GL_LOW_DETAIL		1
#define GL_LIGHT_DETAIL		2
#define GL_MED_DETAIL		3
#define GL_HIGH_DETAIL		4

#endif	//DEFINE_GL_H