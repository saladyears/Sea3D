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
// constants and defines related to the GUI drawing
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_AI_H)
#define DEFINE_AI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// the AI version define
/////////////////////////////////////////////////////////////////////////////
#define AI_VER			"0.1.0"

/////////////////////////////////////////////////////////////////////////////
// return messages
/////////////////////////////////////////////////////////////////////////////

//error messages
#define AI_ERR_SHUTDOWN			-4
#define AI_ERR_NO_DLL			-3
#define AI_ERR_NO_FUNC			-2
#define AI_ERR_INVALID_PARAMS	-1

//in game messages
#define AI_FINISHED				0
#define AI_INITIAL_SETTLEMENT	1
#define AI_INITIAL_ROAD			2
#define AI_NORMAL_ROLL			3
#define AI_SEVEN_ROLL			4
#define AI_LOST_CARDS			5
#define AI_TRADE_REJECT			6
#define AI_TRADE_ACCEPT			7
#define AI_TRADE_COUNTER		8
#define AI_TRADE_OFFER			9
#define AI_TRADE				10
#define AI_BUILD_ROAD			11
#define AI_BUILD_SETTLEMENT		12
#define AI_BUILD_CITY			13
#define AI_BUY_DEVCARD			14
#define AI_PLAY_SOLDIER			15
#define AI_PLAY_VICTORY			16
#define AI_PLAY_YEAR			17
#define AI_PLAY_MONOPOLY		18
#define AI_PLAY_ROAD			19

/////////////////////////////////////////////////////////////////////////////
// playing styles
/////////////////////////////////////////////////////////////////////////////
#define AI_STYLE_DEFAULT		0
#define AI_STYLE_CITIES			1
#define AI_STYLE_EXPAND			2
#define AI_STYLE_DEV_CARDS		4
#define AI_STYLE_PORTS			8

/////////////////////////////////////////////////////////////////////////////
// board states
/////////////////////////////////////////////////////////////////////////////
#define AI_STATE_INITIAL		1
#define AI_STATE_ROLL			2
#define AI_STATE_TURN			3
#define AI_STATE_LOSE_CARDS		4
#define AI_STATE_TRADE_OFFER	5

#endif //DEFINE_AI_H