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
// ini defines
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_INI_H)
#define DEFINE_INI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// .ini file defines
/////////////////////////////////////////////////////////////////////////////
#define INI_FILE			CString(APP->getDir() + "S3D.ini")
#define INI_GENERAL			"General Settings"
#define INI_NETWORK			"Network Settings"
#define INI_OPENGL			"OpenGL Settings"
#define INI_VARIANT			"Variant Settings"
#define INI_DISPLAY			"Display Settings"

#define INI_SOUNDS			"Play Sounds"
#define INI_LEGEND			"Show Legend"
#define INI_AUTOTRADE		"Enable Auto-trading"
#define INI_LARGE_FONTS		"Large Fonts"
#define INI_ANIMATE			"Animate Controls"
#define INI_EMBED_CHAT		"Embed the Chat Window"
#define INI_AUTO_ROLL		"Auto-Roll In Network Games"
#define INI_TOUR			"Prompt Tour"
#define INI_TRADE_TIMEOUT	"Trade Timeout In Network Games"
#define INI_ASK_HOTSEAT		"Ask for next Hotseat Player"
#define INI_AUTO_ROLL_H		"Auto-Roll in Hotseat Games"
#define INI_SPLASH			"Show splash screen"

#define INI_NUM_IPS			5
#define INI_IP_STRING		"Recent IP #"
#define INI_LAST_HOST		"Hosted Last Game"
#define INI_FIREWALL		"Hide Firewall Warning"
#define INI_PORTFOR			"Port Forwarding"
#define INI_PORTFOR_IP		"Port Forwarding IP"
#define INI_PORTFOR_PORT	"Port Forwarding Port"

#define INI_TEXTURES		"Texture Size"
#define INI_MIPMAP			"Mipmapping"
#define INI_DETAIL			"OpenGL Detail"
#define INI_WIREFRAME		"Wireframe Robber"
#define INI_GOURAUD			"Gouraud Robber"
#define INI_EURO			"Vivid Textures"
#define INI_SEATILES		"Seafarers Port Tiles"
#define INI_BORDERS			"Seafarers Borders"
#define INI_NOLOAD			"Do Not Load Seafarers Borders"
#define INI_CAMERA_POS		"Camera Position"
#define INI_ROTATION		"Map Rotation"
#define INI_LOOK_POS		"Look Position"
#define INI_HIGHLIGHT		"Highlight Selectable Objects"
#define INI_HIGHLIGHT_COLOR	"Higlight Color"
#define INI_WHITE			"Outline White Player"
#define INI_WHITE_COLOR		"White Player Outline Color"
#define INI_LONGEST_COLOR	"Longest Road Outline Color"
#define INI_SMALL_TILES		"Smaller Tile Textures"

#define INI_VAR_TRADE_ZERO	"Zero Card Trades"
#define INI_VAR_EQUAL_ODDS	"Equal Roll Odds"
#define INI_VAR_TRADE_BUILD	"Trade After Build"
#define INI_VAR_NO_7		"No 7's First Round"
#define INI_VAR_SHOW_CARDS	"Show All Cards"
#define INI_VAR_ROUND_UP	"Round Losses Up Not Down"
#define INI_VAR_HIDE_CHIPS	"Show Chips After Placements"
#define INI_VAR_DESERTS		"Deserts/Jungles/Volcanoes"
#define INI_VAR_SPECIAL		"Special Build Phase"
#define INI_VAR_TOUR_START	"Tournament Start"
#define INI_VAR_NO_TRADE	"No Trading"
#define INI_VAR_NO_SEVENS	"Rounds of no 7's"
#define INI_VAR_LIMIT_TRADE	"Limit Trade Offers Per Turn"
#define INI_VAR_LIMIT_NUM	"Trade Offfers Limit"

#define INI_MAXIMIZED		"Start Maximized"
#define INI_CHAT_OFFSET_X	"Chat Offset X"
#define INI_CHAT_OFFSET_Y	"Chat Offset Y"
#define INI_CHAT_SIZE_X		"Chat Size X"
#define INI_CHAT_SIZE_Y		"Chat Size Y"
#define INI_STAT_OFFSET_X	"Stat Offset X"
#define INI_STAT_OFFSET_Y	"Stat Offset Y"
#define INI_STAT_SIZE_X		"Stat Size X"
#define INI_STAT_SIZE_Y		"Stat Size Y"
#define INI_LEGEND_OFFSET_X	"Legend Offset X"
#define INI_LEGEND_OFFSET_Y	"Legend Offset Y"
#define INI_AUTOHIDE		"Auto-hide Player Displays"
#define INI_AUTOHIDE_ANIMATE	"Use Auto-hide Animation"
#define INI_AUTOHIDE_NET	"Always Show Self In Network Games"
#define INI_AUTOHIDE_OTHER	"Always Show Current Player in Hotseat Games"
#define INI_AUTOHIDE_BANK	"Always Show Bank"
#define INI_EMBED_WIDTH		"Embedded Chat Width"
#define INI_EMBED_HEIGHT	"Embedded Chat Height"

#endif	//DEFINE_INI_H