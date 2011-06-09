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

#if !defined(DRAW_DEFINE_H)
#define DRAW_DEFINE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// panel defines
/////////////////////////////////////////////////////////////////////////////
#define ADMIN_HEIGHT				200
#define SIZE_LEFT					1
#define SIZE_TOP					2
#define SIZE_TOPLEFT				3
#define CONTROLS_WIDTH				316
#define CONTROLS_HEIGHT				43
#define PANEL_WIDTH					165
#define PANEL_ICON					16
#define PANEL_OFFSET				2
#define PANEL_TITLE_LBORD			5
#define PANEL_TITLE_TBORD			2
#define SETUP_PANEL_BUTTON_HEIGHT	40
#define SETUP_PANEL_TITLE			48
#define ROLL_PANEL_TITLE			80
#define CONTROL_PANEL_TITLE			30
#define CONTROL_PANEL_SPACER		5
#define CONTROL_PANEL_ROLL_WIDTH	48
#define CONTROL_PANEL_ROLL_HEIGHT	25
#define CONTROL_PANEL_GOODS_WIDTH	160
#define CONTROL_PANEL_GOODS_HEIGHT	25
#define CONTROL_PANEL_DICE_WIDTH	36
#define CONTROL_PANEL_GOODS_BUTTON	(6 * CONTROL_PANEL_GOODS_HEIGHT + CONTROL_PANEL_DICE_WIDTH + 2 * CONTROL_PANEL_SPACER)
#define CONTROL_PANEL_ACTION_WIDTH	69
#define CONTROL_PANEL_ACTION_HEIGHT	29
#define CONTROL_PANEL_ACTION_X1		3
#define CONTROL_PANEL_ACTION_X2		40
#define CONTROL_PANEL_ACTION_X3		78
#define RES_BUTTON_TYPE_WIDTH		98
#define RES_BUTTON_TYPE_HEIGHT		17
#define RES_BUTTON_TYPE_START_X		50
#define RES_BUTTON_TYPE_START_Y		5
#define RES_BUTTON_REMAIN_WIDTH		65
#define RES_BUTTON_ICON_SIZE		36
#define RES_BUTTON_ICON_START_X		10
#define RES_BUTTON_ICON_START_Y		2
#define PLAYER_BUTTON_WIDTH			210
#define PLAYER_BUTTON_WIDTH_SEA		250
#define PLAYER_BUTTON_HEIGHT		60
#define BANK_BUTTON_WIDTH			210
#define BANK_BUTTON_HEIGHT			45
#define BANK_RES_WIDTH				31
#define BANK_RES_HEIGHT				12
#define STATIC_HEIGHT				12
#define GL_TINY_SIZE				56

#define PLAYER_NAME_OFFSET			12
#define PLAYER_NAME_WIDTH			90
#define PLAYER_POINTS_WIDTH			78
#define PLAYER_EXTRAS_WIDTH			6
#define PLAYER_STOCK_OFFSET			40
#define PLAYER_STOCK_TEXT_WIDTH		12
#define PLAYER_RES_WIDTH			16
#define PLAYER_RES_HEIGHT			22
#define PLAYER_RES_OFFSET			23
#define PLAYER_RES_TEXT_WIDTH		18
#define PLAYER_RES_SPACER			6

#define LEGEND_BUTTON_HEIGHT		128
#define LEGEND_RES_START_X			12
#define LEGEND_RES_START_Y			3
#define LEGEND_RES_WIDTH			16
#define LEGEND_RES_HEIGHT			22
#define LEGEND_RES_SPACER_X			6
#define LEGEND_RES_SPACER_Y			3
#define LEGEND_IMAGE_START_X		150
#define LEGEND_IMAGE_START_Y		31
#define LEGEND_IMAGE_WIDTH			16
#define LEGEND_IMAGE_HEIGHT			16
#define LEGEND_EQUALS_START_X		130
#define LEGEND_EQUALS_START_Y		8
#define LEGEND_EQUALS_WIDTH			10
#define LEGEND_EQUALS_HEIGHT		10
#define LEGEND_COST_WIDTH			10
#define LEGEND_COST_HEIGHT			12

/////////////////////////////////////////////////////////////////////////////
// large font defines
/////////////////////////////////////////////////////////////////////////////
#define LARGE_PLAYER_BUTTON_WIDTH		355
#define LARGE_PLAYER_BUTTON_WIDTH_SEA	425
#define LARGE_PLAYER_BUTTON_HEIGHT		110

#define LARGE_STATIC_HEIGHT				16
#define LARGE_PLAYER_NAME_WIDTH			150
#define LARGE_PLAYER_POINTS_WIDTH		90

#define LARGE_PANEL_ICON				32
#define LARGE_PLAYER_STOCK_OFFSET		60
#define LARGE_PLAYER_STOCK_TEXT_WIDTH	18

#define LARGE_PLAYER_RES_WIDTH			29
#define LARGE_PLAYER_RES_HEIGHT			42
#define LARGE_PLAYER_RES_OFFSET			48
#define LARGE_PLAYER_RES_TEXT_WIDTH		31
#define LARGE_PLAYER_RES_SPACER			10

#define LARGE_BANK_BUTTON_WIDTH			301
#define LARGE_BANK_BUTTON_HEIGHT		55
#define LARGE_BANK_RES_WIDTH			45
#define LARGE_BANK_RES_HEIGHT			18

#define LARGE_LEGEND_BUTTON_HEIGHT		240
#define LARGE_LEGEND_RES_WIDTH			29
#define LARGE_LEGEND_RES_HEIGHT			42
#define LARGE_LEGEND_RES_SPACER_X		10
#define LARGE_LEGEND_RES_SPACER_Y		6
#define LARGE_LEGEND_IMAGE_START_X		214
#define LARGE_LEGEND_IMAGE_START_Y		55
#define LARGE_LEGEND_IMAGE_WIDTH		32
#define LARGE_LEGEND_IMAGE_HEIGHT		32
#define LARGE_LEGEND_EQUALS_START_X		199
#define LARGE_LEGEND_EQUALS_START_Y		16
#define LARGE_LEGEND_EQUALS_WIDTH		15
#define LARGE_LEGEND_EQUALS_HEIGHT		15
#define LARGE_LEGEND_COST_WIDTH			15
#define LARGE_LEGEND_COST_HEIGHT		18


#endif //DEFINE_H