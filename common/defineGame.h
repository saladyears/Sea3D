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
// gameflow defines
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_GAME_H)
#define DEFINE_GAME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// resource types
/////////////////////////////////////////////////////////////////////////////
#define RES_SIZE			19
#define RES_TIMBER			0
#define RES_WHEAT			1
#define RES_ORE				2
#define RES_CLAY			3
#define RES_SHEEP			4
#define RES_GOLD			5
#define RES_DESERT			6
#define RES_OCEAN			7
#define RES_PORT3			8
#define RES_PORTTIMBER		9
#define RES_PORTWHEAT		10
#define RES_PORTORE			11
#define RES_PORTCLAY		12
#define RES_PORTSHEEP		13
#define RES_BLANK			14
#define RES_RANDOM			15
#define RES_HIDDEN			16
#define RES_JUNGLE			17
#define RES_VOLCANO			18

/////////////////////////////////////////////////////////////////////////////
// maps3d extra defines
/////////////////////////////////////////////////////////////////////////////
#define RES_CHIT_SIZE		10
#define RES_CHIT_2			17

/////////////////////////////////////////////////////////////////////////////
// development card types
/////////////////////////////////////////////////////////////////////////////
#define CARD_SIZE			5
#define CARD_VICTORY		0
#define CARD_MONOPOLY		1
#define CARD_ROAD_BUILD		2
#define CARD_YEAR_PLENTY	3
#define CARD_SOLDIER		4

/////////////////////////////////////////////////////////////////////////////
// multiuse modes
/////////////////////////////////////////////////////////////////////////////
#define MULTIUSE_YEAR		0
#define MULTIUSE_STEAL		1
#define MULTIUSE_LOSE		2
#define MULTIUSE_GOLD		3

/////////////////////////////////////////////////////////////////////////////
// constant lookup values
/////////////////////////////////////////////////////////////////////////////
const int u_34Chips[] = {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11};
const int u_56Chips[] = {2, 5, 4, 6, 3, 9, 8, 11, 11, 10, 6, 3, 8, 4,\
						 8, 10, 11, 12, 10, 5, 4, 9, 5, 9, 12, 3, 2, 6};



#endif	//DEFINE_GAME_H