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
// connector defines
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_CONNECTOR_H)
#define DEFINE_CONNECTOR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// connector name max length
/////////////////////////////////////////////////////////////////////////////
#define CONNECTOR_NAME_MAX_LENGTH	32

/////////////////////////////////////////////////////////////////////////////
// connector parse separator
/////////////////////////////////////////////////////////////////////////////
#define CONNECTOR_SEPARATOR			' '

/////////////////////////////////////////////////////////////////////////////
// connector parse positions
/////////////////////////////////////////////////////////////////////////////
#define CONNECTOR_FIELDS		0
#define CONNECTOR_ID			0
#define CONNECTOR_GAME			1
#define CONNECTOR_HOST			2
#define CONNECTOR_IP			3
#define CONNECTOR_PORT			4
#define CONNECTOR_MAP			5
#define CONNECTOR_MAX			6
#define CONNECTOR_CURRENT		7
#define CONNECTOR_PRIVATE		8
#define CONNECTOR_PASSWORD		9
#define CONNECTOR_STATUS		10
#define CONNECTOR_VER			11
#define CONNECTOR_RESTART		12
#define CONNECTOR_LADDER		13
#define CONNECTOR_PLAYER1		14
#define CONNECTOR_PLAYER2		15
#define CONNECTOR_PLAYER3		16
#define CONNECTOR_PLAYER4		17
#define CONNECTOR_PLAYER5		18
#define CONNECTOR_PLAYER6		19

/////////////////////////////////////////////////////////////////////////////
// connector list control positions
/////////////////////////////////////////////////////////////////////////////
#define CONNECTOR_LIST_PRIVATE		0
#define CONNECTOR_LIST_VER			1
#define CONNECTOR_LIST_GAME			2
#define CONNECTOR_LIST_HOST			3
#define CONNECTOR_LIST_MAP			4
#define CONNECTOR_LIST_MAX			5
#define CONNECTOR_LIST_IP			6
#define CONNECTOR_LIST_PORT			7

/////////////////////////////////////////////////////////////////////////////
// connector list control values
/////////////////////////////////////////////////////////////////////////////
#define CONNECTOR_LIST_LADDER		1
#define CONNECTOR_LIST_LOCK			2
#define CONNECTOR_LIST_RESTART		4

/////////////////////////////////////////////////////////////////////////////
// connector server statii
/////////////////////////////////////////////////////////////////////////////
#define CONNECTOR_NEW		0
#define CONNECTOR_UPDATE	1
#define CONNECTOR_INGAME	2
#define CONNECTOR_COMPLETE	3
#define CONNECTOR_DELETE	4

#endif	//DEFINE_CONNECTOR_H