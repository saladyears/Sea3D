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
// image display defines
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_DISPLAY_H)
#define DEFINE_DISPLAY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// tiny resource types for the player windows and legend button
/////////////////////////////////////////////////////////////////////////////
#define IMG_SIZE			42
#define IMG_TIMBER			0
#define IMG_WHEAT			1
#define IMG_ORE				2
#define IMG_CLAY			3
#define IMG_SHEEP			4
#define IMG_TIMBER_S		5
#define IMG_WHEAT_S			6
#define IMG_ORE_S			7
#define IMG_CLAY_S			8
#define IMG_SHEEP_S			9
#define IMG_DEVCARD			10
#define IMG_DEVCARD_S		11
#define IMG_RESCARD			12
#define IMG_RESCARD_S		13
#define IMG_SOLDIER			14
#define IMG_SMILEY			15
#define IMG_SETTLEW			16
#define IMG_CITYW			22
#define IMG_ROADW			28
#define IMG_ROADB			29
#define IMG_ROADG			30
#define IMG_ROADO			31
#define IMG_ROADR			32
#define IMG_ROADBN			33
#define IMG_SHIPW			34
#define IMG_SHIPB			35
#define IMG_SHIPG			36
#define IMG_SHIPO			37
#define IMG_SHIPR			38
#define IMG_SHIPBN			39
#define IMG_JUNGLE			40
#define IMG_JUNGLE_S		41

/////////////////////////////////////////////////////////////////////////////
// tiny window defines
/////////////////////////////////////////////////////////////////////////////
#define TINY_SIZE			27
#define TINY_SETTLEW		0
#define TINY_SETTLEB		1
#define TINY_SETTLEG		2
#define TINY_SETTLEO		3
#define TINY_SETTLER		4
#define TINY_SETTLEBN		5
#define TINY_ROADW			6
#define TINY_ROADB			7
#define TINY_ROADG			8
#define TINY_ROADO			9
#define TINY_ROADR			10
#define TINY_ROADBN			11
#define TINY_CITYW			12
#define TINY_CITYB			13
#define TINY_CITYG			14
#define TINY_CITYO			15
#define TINY_CITYR			16
#define TINY_CITYBN			17
#define TINY_SHIPW			18
#define TINY_SHIPB			19
#define TINY_SHIPG			20
#define TINY_SHIPO			21
#define TINY_SHIPR			22
#define TINY_SHIPBN			23
#define TINY_DEVCARD		24
#define TINY_SPOTLIGHTOFF	25
#define TINY_SPOTLIGHTON	26

/////////////////////////////////////////////////////////////////////////////
// splash screen defines
/////////////////////////////////////////////////////////////////////////////
#define SPLASH_HOTSEAT		1
#define SPLASH_NETWORK		2
#define SPLASH_OPEN			3
#define SPLASH_TOUR			4
#define SPLASH_SKIP			5
#define SPLASH_NEW			6

/////////////////////////////////////////////////////////////////////////////
// connector images
/////////////////////////////////////////////////////////////////////////////
#define CONNECT_SIZE		3
#define CONNECT_LADDER		0
#define CONNECT_LOCK		1
#define CONNECT_RESTART		2

/////////////////////////////////////////////////////////////////////////////
// dice rolls (for reading in the dice textures)
/////////////////////////////////////////////////////////////////////////////
#define DICE_SIZE			6

/////////////////////////////////////////////////////////////////////////////
// deck resources (for reading in the deck textures)
/////////////////////////////////////////////////////////////////////////////
#define DECK_SIZE			36
#define DECK_LEVELS			6

/////////////////////////////////////////////////////////////////////////////
// control panel defines (for reading in the button bitmaps)
/////////////////////////////////////////////////////////////////////////////
#define BUTTON_SIZE			10
#define BUTTON_PLAY			0
#define BUTTON_TRADE		1
#define BUTTON_END			2
#define BUTTON_BANK			3
#define BUTTON_ROLL			4
#define BUTTON_PLAY_GRAY	5
#define BUTTON_TRADE_GRAY	6
#define BUTTON_END_GRAY		7
#define BUTTON_BANK_GRAY	8
#define BUTTON_ROLL_GRAY	9

/////////////////////////////////////////////////////////////////////////////
// toolbar defines (for reading in the toolbar/splash screen bitmaps)
/////////////////////////////////////////////////////////////////////////////
#define TOOLBAR_SIZE		5
#define TOOLBAR				0
#define TOOLBAR_GRAY		1
#define TOOLBARCHILD		2
#define TOOLBARCHILD_GRAY	3
#define SPLASH_SCREEN		4


#endif	//DEFINE_DISPLAY_H