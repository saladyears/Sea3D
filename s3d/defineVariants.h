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
// variants defines
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_VARIANTS_H)
#define DEFINE_VARIANTS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// variants
/////////////////////////////////////////////////////////////////////////////
#define VARIANT_TRADE_ZERO			(1)
#define VARIANT_EQUAL_ODDS			(2)
#define VARIANT_ALTER_ROADS			(4)
#define VARIANT_ALTER_CITIES		(8)
#define VARIANT_ALTER_SETTLEMENTS	(16)
#define VARIANT_ALTER_ALLOWED_CARDS	(32)
#define VARIANT_ALTER_POINTS		(64)
#define VARIANT_TRADE_AFTER_BUILD	(128)
#define VARIANT_NO_7_FIRST_ROUND	(256)
#define VARIANT_SHOW_ALL_CARDS		(512)
#define VARIANT_ROUND_LOST_UP		(1024)
#define VARIANT_HIDE_CHIPS			(2048)
#define VARIANT_ALTER_SHIPS			(4096)
#define VARIANT_USE_JUNGLE			(8192)
#define VARIANT_USE_VOLCANO			(16384)
#define VARIANT_USE_VOLCANO_GOLD	(32768)
#define VARIANT_SPECIAL_BUILD		(65536)
#define VARIANT_TOURNAMENT_START	(131072)
#define VARIANT_NO_TRADING			(262144)
#define VARIANT_LIMIT_TRADING		(524288)

//acceptable ladder variants
#define VARIANT_ACCEPTABLE_LADDER	(VARIANT_TRADE_AFTER_BUILD | VARIANT_NO_7_FIRST_ROUND | VARIANT_ROUND_LOST_UP | VARIANT_USE_JUNGLE | VARIANT_USE_VOLCANO | VARIANT_USE_VOLCANO_GOLD | VARIANT_SPECIAL_BUILD | VARIANT_TOURNAMENT_START | VARIANT_LIMIT_TRADING)

#endif	//DEFINE_VARIANTS_H