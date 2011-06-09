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
// history defines
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_HISTORY_H)
#define DEFINE_HISTORY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// undo defines
/////////////////////////////////////////////////////////////////////////////
#define UNDO_SETTLE_PLACE		0
#define UNDO_ROAD_PLACE			1
#define UNDO_FIRST_ROLL			2
#define UNDO_ROLL				3
#define UNDO_ACTION_START		4
#define UNDO_ACTION_END			5
#define UNDO_ACTION				6
#define UNDO_ROAD				7
#define UNDO_CITY				8
#define UNDO_SETTLEMENT			9
#define UNDO_ROBBER_PLACE		10
#define UNDO_BUY_DEVCARD		11
#define UNDO_PLAY_VICTORY		12
#define UNDO_PLAY_MONOPOLY		13
#define UNDO_PLAY_ROAD			14
#define UNDO_PLAY_YEAR			15
#define UNDO_PLAY_SOLDIER		16
#define UNDO_TRADE				17
#define UNDO_STEAL				18
#define UNDO_LOSECARDS			19
#define UNDO_WIN				20
#define UNDO_WASTE				21
#define UNDO_SHIP_PLACE			22
#define UNDO_SHIP				23
#define UNDO_PIRATE_PLACE		24
#define UNDO_MOVE_SHIP			25
#define UNDO_GOLD				26
#define UNDO_CHITLESS			27
#define UNDO_VOLCANO			28
#define UNDO_TRADE_JUNGLE		29
#define UNDO_SPECIAL_BUILD		30
#define UNDO_SPECIAL_BUILD_END	31
#define UNDO_THIRD_ROAD_PLACE	32

/////////////////////////////////////////////////////////////////////////////
// history image list defines
/////////////////////////////////////////////////////////////////////////////
#define HISTORY_SIZE				55
#define HISTORY_OFFSET				20
#define HISTORY_SETTLEW				0
#define HISTORY_SETTLEB				1
#define HISTORY_SETTLEG				2
#define HISTORY_SETTLEO				3
#define HISTORY_SETTLER				4
#define HISTORY_SETTLEBN			5
#define HISTORY_CITYW				6
#define HISTORY_CITYB				7
#define HISTORY_CITYG				8
#define HISTORY_CITYO				9
#define HISTORY_CITYR				10
#define HISTORY_CITYBN				11
#define HISTORY_ROADW				12
#define HISTORY_ROADB				13
#define HISTORY_ROADG				14
#define HISTORY_ROADO				15
#define HISTORY_ROADR				16
#define HISTORY_ROADBN				17
#define HISTORY_TURN_WHITE			18
#define HISTORY_TURN_BLUE			19
#define HISTORY_TURN_GREEN			20
#define HISTORY_TURN_ORANGE			21
#define HISTORY_TURN_RED			22
#define HISTORY_TURN_BROWN			23
#define HISTORY_DICE				24
#define HISTORY_TURNSTART			25
#define HISTORY_ACTION				26
#define HISTORY_ROBBER				27
#define HISTORY_BUYCARD				28
#define HISTORY_VICTORY				29
#define HISTORY_SOLDIER				30
#define HISTORY_ROADBUILD			31
#define HISTORY_YEARPLENTY			32
#define HISTORY_MONOPOLY			33
#define HISTORY_TURNEND				34
#define HISTORY_INITPLACE			35
#define HISTORY_TRADE				36
#define HISTORY_STEAL				37
#define HISTORY_NOSTEAL				38
#define HISTORY_LOSECARDS			39
#define HISTORY_WIN					40
#define HISTORY_WASTE				41
#define HISTORY_ADJUST				42
#define HISTORY_SHIPW				43
#define HISTORY_SHIPB				44
#define HISTORY_SHIPG				45
#define HISTORY_SHIPO				46
#define HISTORY_SHIPR				47
#define HISTORY_SHIPBN				48
#define HISTORY_PIRATE				49
#define HISTORY_MOVE				50
#define HISTORY_GOLD				51
#define HISTORY_CHITLESS			52
#define HISTORY_VOLCANO				53
#define HISTORY_JUNGLE				54

#endif	//DEFINE_HISTORY_H