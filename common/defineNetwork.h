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
// network defines
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_NETWORK_H)
#define DEFINE_NETWORK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// languages
/////////////////////////////////////////////////////////////////////////////
#define LANGUAGES_SIZE		(16)
#define LANGUAGE_ENGLISH		(1)
#define LANGUAGE_DUTCH			(1 << 1)
#define LANGUAGE_GERMAN			(1 << 2)
#define LANGUAGE_SPANISH		(1 << 3)
#define LANGUAGE_ITALIAN		(1 << 4)
#define LANGUAGE_FRENCH			(1 << 5)
#define LANGUAGE_PORTUGUESE		(1 << 6)
#define LANGUAGE_ROMANIAN		(1 << 7)
#define LANGUAGE_ARABIC			(1 << 8)
#define LANGUAGE_SWEDISH		(1 << 9)
#define LANGUAGE_NORWIEGAN		(1 << 10)
#define LANGUAGE_KOREAN			(1 << 11)
#define LANGUAGE_CHINESE		(1 << 12)
#define LANGUAGE_JAPANESE		(1 << 13)
#define LANGUAGE_RUSSIAN		(1 << 14)
#define LANGUAGE_OTHER			(1 << 15)

/////////////////////////////////////////////////////////////////////////////
// network trade defines
/////////////////////////////////////////////////////////////////////////////
#define TRADE_NO_OFFER		0
#define TRADE_OFFER			1
#define TRADE_REJECT		2
#define TRADE_ACCEPT		3
#define TRADE_COUNTER		4
#define TRADE_COFFER		5

/////////////////////////////////////////////////////////////////////////////
// system messages
/////////////////////////////////////////////////////////////////////////////
#define SYS_JOINED_GAME			0
#define SYS_LEFT_GAME			1
#define SYS_BOOTED_FROM_GAME	2
#define SYS_SEND_ERROR			3
#define SYS_RECV_ERROR			4
#define SYS_SERVER_QUIT			5
#define SYS_INVALID_PLAYER		6
#define SYS_VERSION_MISMATCH	7
#define SYS_DROPPED				8
#define SYS_COLOR_ASSIGN		9

//in game messages
#define SYS_PORT_PLACE			10
#define SYS_SETTLE_PLACE		11
#define SYS_ROAD_PLACE			12
#define SYS_SHIP_PLACE			13
#define SYS_CITY_PLACE			14
#define SYS_SHIP_MOVE			15
#define SYS_ROLL				16
#define SYS_ROBBER_PLACE		17
#define SYS_PIRATE_PLACE		18
#define SYS_STEAL				19
#define SYS_TRADE_TO_BANK		20
#define SYS_BUY_DEV_CARD		21
#define SYS_PLAY_DEV_CARD		22
#define SYS_LOSE_CARDS			23
#define SYS_END_TURN			24
#define SYS_TRADE_OFFER			25
#define SYS_TRADE				26
#define SYS_UNDO_ROAD			27
#define SYS_UNDO_SHIP			28
#define SYS_UNDO_SHIP_MOVE		29
#define SYS_UNDO_CITY			30
#define SYS_UNDO_SETTLEMENT		31
#define SYS_UNDO_AUTOTRADE		32
#define SYS_MONOPOLY			33
#define SYS_GOLD				34
#define SYS_CHITLESS			35
#define SYS_TRADE_JUNGLE		36

//others
#define SYS_AUTO_SAVE			37
#define SYS_GAME_FULL			38
#define SYS_LADDER_GAME			39
#define SYS_LADDER_SAMEIP		40
#define SYS_LADDER_WARNIP		41
#define SYS_MAX_PLAYERS			42
#define SYS_STEAL_REPORT		43
#define SYS_QUIT_CUTOFF			44
#define SYS_DOUBLE_JOIN			45
#define SYS_REJOIN_GAME			46
#define SYS_GAME_OVER			47

/////////////////////////////////////////////////////////////////////////////
// system message notifications
/////////////////////////////////////////////////////////////////////////////
#define SYS_NOTIFY_SIZE			9
#define SYS_NOTIFY_BUILD		(1)
#define SYS_NOTIFY_ROLLS		(1 << 1)
#define SYS_NOTIFY_ROBBER		(1 << 2)
#define SYS_NOTIFY_STEAL		(1 << 3)
#define SYS_NOTIFY_TRADE_BANK	(1 << 4)
#define SYS_NOTIFY_TRADE_OTHERS	(1 << 5)
#define SYS_NOTIFY_DEV			(1 << 6)
#define SYS_NOTIFY_LOSE			(1 << 7)
#define SYS_NOTIFY_END			(1 << 8)

/////////////////////////////////////////////////////////////////////////////
// resynch states
/////////////////////////////////////////////////////////////////////////////
#define RESYNCH_SAVE_STATE			(1)
#define RESYNCH_TURN				(2)
#define RESYNCH_PORT_PLACE			(4)
#define RESYNCH_INITIAL_FIRST		(8)
#define RESYNCH_INITIAL_SETTLE		(16)
#define RESYNCH_INITIAL_ROAD		(32)
#define RESYNCH_INITIAL_THIRD_ROAD	(64)
#define RESYNCH_PRE_ROLL			(128)
#define RESYNCH_SHOW_ROLLS			(256)
#define RESYNCH_SHOW_VOLCANO		(512)
#define RESYNCH_LOSE_7				(1024)
#define RESYNCH_ROBBER_STEAL		(2048)
#define RESYNCH_PIRATE_STEAL		(4096)
#define RESYNCH_ROBBER				(8192)
#define RESYNCH_POST_ROLL			(16384)

/////////////////////////////////////////////////////////////////////////////
// network messages
/////////////////////////////////////////////////////////////////////////////
#define S3D_ADD_PLAYER		0
#define S3D_REMOVE_PLAYER	1
#define S3D_COLOR_ASSIGN	2
#define S3D_CHAT_MESSAGE	3
#define S3D_SYS_MESSAGE		4
#define S3D_PING_UPDATE		5

//initial start messages
#define S3D_MAP_INFO		6
#define S3D_START_SEND		7
#define S3D_MAP				8
#define S3D_GAME			9
#define S3D_UNDO			10
#define S3D_START_BOARD		11
#define S3D_BOARD_PIECE		12
#define S3D_START_PLAYER	13
#define S3D_PLAYER_PIECE	14
#define S3D_PLAYER_GAME		15
#define S3D_SAVED_DATA		16
#define S3D_END_SEND		17
#define S3D_END_SAVE		18

//game play messages
#define S3D_DRAW_MODE		19
#define S3D_PANEL_TEXT		20
#define S3D_PORT_PLACE		21
#define S3D_PORT			22
#define S3D_SETTLE_PLACE	23
#define S3D_ROAD_PLACE		24
#define S3D_SHIP_PLACE		25
#define S3D_CITY_PLACE		26
#define S3D_SHIP_MOVE		27
#define S3D_START_TURN		28
#define S3D_ROLL			29
#define S3D_HIDE_GOODS		30
#define S3D_END_TURN		31
#define S3D_PLACE_ROBBER	32
#define S3D_PLACE_PIRATE	33
#define S3D_WAIT_FOR_STEAL	34
#define S3D_STEAL			35
#define S3D_TRADE_TO_BANK	36
#define S3D_BUY_DEV_CARD	37
#define S3D_PLAY_DEV_CARD	38
#define S3D_WAIT_FOR_LOSE	39
#define S3D_WAIT_FOR_GOLD	40
#define S3D_CONTINUE_7		41
#define S3D_CONTINUE_ROLL	42
#define S3D_VERSION_CHECK	43	//THIS CANNOT CHANGE
#define S3D_LOSE_CARDS		44
#define S3D_INIT_GOLD		45
#define S3D_SELECT_GOLD		46
#define S3D_TRADE_OFFER		47
#define S3D_TRADE_REPLY		48
#define S3D_TRADE			49
#define S3D_TRADE_COUNTER	50
#define S3D_CHITLESS		51
#define S3D_WIN				52
#define S3D_TIME_UPDATE		53
#define S3D_RESTART_GAME	54
#define S3D_TERMINATE		55
#define S3D_UNDO_BUILD		56
#define S3D_UNDO_AUTOTRADE	57
#define S3D_VOLCANO			58
#define S3D_HIDE_VOLCANO	59
#define S3D_TRADE_JUNGLE	60
#define S3D_SPECIAL_START	61
#define S3D_SPECIAL_NEXT	62
#define S3D_SPECIAL_END		63
#define S3D_SAVE_STATE		64
#define S3D_RESTORE_STATE	65
#define S3D_REJOIN_SYNCH	66
#define S3D_READY_STATE		67

#endif	//DEFINE_NETWORK_H