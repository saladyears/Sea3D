/* 
 * Settlers3D - Copyright (C) 2001-2003 Jason Fugate (jfugate@settlers3d.net) 
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the Free 
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version. 
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 * for more details. 
 */

/////////////////////////////////////////////////////////////////////////////
// constants and defines related to the app
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_H)
#define DEFINE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// the file version define
/////////////////////////////////////////////////////////////////////////////
#define VER_BASE			1.00f	//base file version
#define VER_GAMEMODE		1.01f	//includes game mode and res display
#define VER_NETWORK			1.02f	//includes network support
#define VER_NETWORKa		1.03f	//first revision of network support
#define VER_NETWORKb		1.04f	//second revision of network support
#define VER_NETWORKc		1.05f	//third revision of network support
#define VER_NETWORKd		1.06f	//fourth revision of network support (0.8.6)
#define VER_VARIANTS		1.07f	//includes variants class (0.8.9)
#define VER_ROADLENGTH		1.08f	//includes road length for players (0.9.1)
#define VER_AI				1.09f	//includes AI support (0.9.2)
#define VER_AUTOHIDE		1.10f	//includes auto-hide player buttons (0.9.3)
#define VER_094				1.11f	//includes cleanup of some GL code (0.9.4)

#define VER_SEAFARERS		2.00f	//includes inital seafarers revisions (1.0.1)
#define VER_FOUR_ISLANDS	2.01f	//includes four islands map revisions (1.0.2)
#define VER_INTO_DESERT		2.02f	//includes into the desert map revisions (1.0.3)
#define VER_GREATER_CATAN	2.03f	//includes greater catan map revisions (1.0.4)
#define VER_JUNGLES			2.04f	//includes volcanoes and jungles (1.0.8)
#define VER_LADDER			2.05f	//includes ladder changes (1.1.0)
#define VER_OCEANSIV_FIX	2.06f	//includes fixes for volcanoes in Oceans IV (1.1.2)
#define VER_113				2.07f	//includes miscellaneous variants (1.1.3)
#define VER_TEAM			2.08f	//includes team play
#define FILE_VER			VER_TEAM

/////////////////////////////////////////////////////////////////////////////
// the game version define
/////////////////////////////////////////////////////////////////////////////
#define GAME_VER			"1.2.0"

/////////////////////////////////////////////////////////////////////////////
// the time limit
/////////////////////////////////////////////////////////////////////////////
#define TIME_LIMIT			COleDateTimeSpan(168, 0, 0, 0)

/////////////////////////////////////////////////////////////////////////////
// default S3D connection port
/////////////////////////////////////////////////////////////////////////////
#define PORT				7176

/////////////////////////////////////////////////////////////////////////////
// function typedefs
/////////////////////////////////////////////////////////////////////////////
typedef void (*LPFNCALLBACK) (DWORD);

/////////////////////////////////////////////////////////////////////////////
// multimedia timer resolution
/////////////////////////////////////////////////////////////////////////////
#define TARGET_RESOLUTION 1         // 1-millisecond target resolution

/////////////////////////////////////////////////////////////////////////////
// game modes
/////////////////////////////////////////////////////////////////////////////
#define GAME_MODE_HOTSEAT	1
#define GAME_MODE_NETWORK	2

/////////////////////////////////////////////////////////////////////////////
// app defines
/////////////////////////////////////////////////////////////////////////////
#ifdef MAPS3D
#define APP				((CMaps3DApp *) AfxGetApp())
#else
#define APP				((CSettlersApp *) AfxGetApp())
#endif
#define VIEW			(APP->getView())
#define SOUND(x)		(APP->playSound(x))
#define GAME			(VIEW->getGame())
#define CONTROLPANEL	(VIEW->getControlPanel())
#define CONTROLS		(VIEW->getControlsButton())
#define ACTIONS			(VIEW->getActionsButton())
#define ADMIN			(VIEW->getAdminButton())
#define MESSAGES		(VIEW->getMessageButton())
#define UNDO			(VIEW->getUndoDialog())
#define STAT			(VIEW->getStatDialog())
#define CONNECTOR		(APP->getConnector())
#define THISVIEW		((CSettlersView *) this)
#define RAND			(((double) genrand()) / double(unsigned long(-1)))
#define VARIANTS		(&(GAME->m_variants))
#define IS_VARIANT(x)	(VARIANTS->m_iVariants & x)
#define IS_NUMBER(x)	(x == '0' || x == '1' || x == '2' || x == '3' || x == '4' || x == '5' || x == '6' || x == '7' || x == '8' || x == '9')

/////////////////////////////////////////////////////////////////////////////
// setup defines
/////////////////////////////////////////////////////////////////////////////
#define CHILD			WS_CHILD | WS_TABSTOP | WS_VISIBLE

/////////////////////////////////////////////////////////////////////////////
// filter/text defines
/////////////////////////////////////////////////////////////////////////////
#define EXPORT_FILTER		"Sea3D Export Files (*.s3d)|*.s3d|All Files (*.*)|*.*||"
#define EXPORT_FLAG			CString("S3D")
#define SCREENSHOT_FILTER	"Windows Bitmap File (*.bmp)|*.bmp|JPEG file (*.jpg)|*.jpg)|"
#define TOOLBAR_MAIN		"S3D Main Toolbar"
#define TOOLBAR_CHILD		"S3D Child Toolbar"

/////////////////////////////////////////////////////////////////////////////
// timer events
/////////////////////////////////////////////////////////////////////////////
#define TIMER_PING				1000
#define TIMER_TIMEOUT			1001
#define TIMER_REJECT_TRADE		1002
#define TIMER_LOSE_CARDS		1003
#define TIMER_GOLD				1004
#define TIMER_CONNECTOR			1005

/////////////////////////////////////////////////////////////////////////////
// macros
/////////////////////////////////////////////////////////////////////////////
#define VALID_RESOURCE(i) (i == RES_TIMBER || i == RES_WHEAT || i == RES_ORE || i == RES_CLAY || i == RES_SHEEP || i == RES_GOLD || i == RES_VOLCANO || i == RES_JUNGLE)
#define VALID_LAND(i) (VALID_RESOURCE(i) || (i == RES_DESERT))
#define VALID_PORT(i) (i == RES_PORT3 || i == RES_PORTTIMBER || i == RES_PORTWHEAT || i == RES_PORTORE || i == RES_PORTCLAY || i == RES_PORTSHEEP)
#define VALID_OCEAN(i) (VALID_PORT(i) || (i == RES_OCEAN))
#define VALID_BUILD(i) (i == GL_SHIP || i == GL_ROAD || i == GL_SETTLEMENT || i == GL_CITY || i == GL_DEVCARD)
#define IS_VOWEL(i) (i == 'a' || i == 'e' || i == 'i' || i == 'o' || i == 'u')
#define IS_NUMBER(i) (i == '0' || i == '1' || i == '2' || i == '3' || i == '4' || i == '5' || i == '6' || i == '7' || i == '8' || i == '9')
#define NEEDS_AN(i) (i == 8 || i == 11)

/////////////////////////////////////////////////////////////////////////////
// turn dialog modes
/////////////////////////////////////////////////////////////////////////////
#define TURN_ROLL			0
#define TURN_GOODS			1
#define TURN_ACTIONS		2

/////////////////////////////////////////////////////////////////////////////
// panel IDs
/////////////////////////////////////////////////////////////////////////////
#define ID_PANEL_BUTTON			2000
#define ID_PANEL_UNDO			2001
#define ID_PLAYER1				2002
#define ID_TINY_BUTTON			2008

/////////////////////////////////////////////////////////////////////////////
// game states
/////////////////////////////////////////////////////////////////////////////
#define GAME_COMPLETE		(0)
#define GAME_NEW			(1)
#define GAME_PRE_TURN		(2)
#define GAME_IN_TURN		(4)

#endif //DEFINE_H
