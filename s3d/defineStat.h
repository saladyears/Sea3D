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

#if !defined(STAT_DEFINE_H)
#define STAT_DEFINE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// trade defines (with bank)
/////////////////////////////////////////////////////////////////////////////
#define TRADE_2FOR1				0
#define TRADE_3FOR1				1
#define TRADE_4FOR1				2

/////////////////////////////////////////////////////////////////////////////
// resource stat defines
/////////////////////////////////////////////////////////////////////////////
#define IS_BANK(i) (i == RES_STAT_BOARD || i == RES_STAT_DEV || i == RES_STAT_LOST_7 || i == RES_STAT_TRADE_BANK || i == RES_STAT_BUILD || i == RES_STAT_ADJUST || i == RES_STAT_GOLD || i == RES_STAT_DISCOVER || i == RES_STAT_TRADE_JUNGLE)
#define RES_STAT_BOARD			0
#define RES_STAT_STOLEN			1
#define RES_STAT_DEV			2
#define RES_STAT_MONOPOLY		3
#define RES_STAT_LOST_7			4
#define RES_STAT_LOST_STOLEN	5
#define RES_STAT_LOST_BOARD		6
#define RES_STAT_TRADE_BANK		7
#define RES_STAT_BUILD			8
#define RES_STAT_TRADE			9
#define RES_STAT_ADJUST			10
#define RES_STAT_GOLD			11
#define RES_STAT_DISCOVER		12
#define RES_STAT_TRADE_JUNGLE	13

/////////////////////////////////////////////////////////////////////////////
// stat defines
/////////////////////////////////////////////////////////////////////////////
#define STAT_SIZE					25
#define STAT_HEAD_ROLL				0
#define STAT_ROLL_FREQ				1
#define STAT_ROLL_PLAYER			2
#define STAT_HEAD_TIME				3
#define STAT_TIME_TURN				4
#define STAT_HEAD_GOODS_RECV		5
#define STAT_GOODS_RECV_TOTAL		6
#define STAT_GOODS_RECV_BOARD		7
#define STAT_GOODS_RECV_STEAL		8
#define STAT_GOODS_RECV_DEVCARD		9
#define STAT_GOODS_RECV_GOLD		10
#define STAT_GOODS_RECV_DISCOVER	11
#define STAT_HEAD_GOODS_LOST		12
#define STAT_GOODS_LOST_TOTAL		13
#define STAT_GOODS_LOST_BOARD		14
#define STAT_GOODS_LOST_STEAL		15
#define STAT_GOODS_LOST_7			16
#define STAT_HEAD_TRADES			17
#define STAT_TRADE_NUMBER			18
#define STAT_TRADE_NUM_OTHERS		19
#define STAT_TRADE_RATIO			20
#define STAT_TRADE_TO_BANK			21
#define STAT_TRADE_FROM_BANK		22
#define STAT_TRADE_TO_OTHERS		23
#define STAT_TRADE_FROM_OTHERS		24

/////////////////////////////////////////////////////////////////////////////
// stat generator defines
/////////////////////////////////////////////////////////////////////////////
//PLAYER STATS
#define GEN_NUM_PLAYER_STATS		13
#define GEN_PLAYER_GAMES_PLAYED		(1)
#define GEN_PLAYER_GAMES_WON		(1 << 1)
#define GEN_PLAYER_POWER_RANK		(1 << 2)
#define GEN_PLAYER_AVE_POINTS		(1 << 3)
#define GEN_PLAYER_AVE_RES			(1 << 4)
#define GEN_PLAYER_AVE_TRADES_O		(1 << 5)
#define GEN_PLAYER_AVE_TRADES_B		(1 << 6)
#define GEN_PLAYER_AVE_NUM_TO_O		(1 << 7)
#define GEN_PLAYER_AVE_NUM_FROM_O	(1 << 8)
#define GEN_PLAYER_AVE_NUM_TO_B		(1 << 9)
#define GEN_PLAYER_AVE_NUM_FROM_B	(1 << 10)
#define GEN_PLAYER_RATIO_O			(1 << 11)
#define GEN_PLAYER_RATIO_B			(1 << 12)

//GAME STATS
#define GEN_NUM_GAME_STATS			10
#define GEN_GAME_GAMES_PLAYED		(1)
#define GEN_GAME_AVE_TURNS			(1 << 1)
#define GEN_GAME_AVE_PLAYERS		(1 << 2)
#define GEN_GAME_AVE_LENGTH			(1 << 3)
#define GEN_GAME_LONGEST_GAME		(1 << 4)
#define GEN_GAME_SHORTEST_GAME		(1 << 5)
#define GEN_GAME_LONGEST_TURN		(1 << 6)
#define GEN_GAME_MOST_7				(1 << 7)
#define GEN_GAME_FEWEST_7			(1 << 8)
#define GEN_GAME_MOST_LOST_7		(1 << 9)


#endif //DEFINE_H