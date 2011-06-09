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
// control tab display defines
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_TAB_H)
#define DEFINE_TAB_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// control panel
/////////////////////////////////////////////////////////////////////////////
#define PANEL_BACKGROUND			RGB(25, 25, 25)
#define PANEL_HEIGHT				63

/////////////////////////////////////////////////////////////////////////////
// creation flags
/////////////////////////////////////////////////////////////////////////////
#define TAB_FLAGS			(TCS_BOTTOM | TCS_FIXEDWIDTH)
#define LIST_FLAGS			(LVS_ALIGNTOP | LVS_NOLABELWRAP | LVS_NOSCROLL | LVS_ICON | LVS_SINGLESEL | LVS_SHAREIMAGELISTS)

/////////////////////////////////////////////////////////////////////////////
// list control sizes
/////////////////////////////////////////////////////////////////////////////
#define LIST_ICON_SPACING		40
#define LIST_CHIT_SPACING		26

/////////////////////////////////////////////////////////////////////////////
// control tab colors
/////////////////////////////////////////////////////////////////////////////
#define TAB_COLOR_BACKGROUND		COLOR_BUILDGRAY
#define TAB_COLOR_BORDER			COLOR_DKGRAY
#define TAB_COLOR_NOT_SELECTED		COLOR_BUILDGRAY

/////////////////////////////////////////////////////////////////////////////
// control tab sizes
/////////////////////////////////////////////////////////////////////////////
#define TAB_HEIGHT				58
#define TAB_CURVE				6
#define TAB_ITEM_WIDTH			50
#define TAB_ITEM_HEIGHT			17
#define TAB_ITEM_CURVE			5

#define TAB_OFFSET				4
#define TAB_SPACING				8
#define TAB_TILES_WIDTH			286
#define TAB_SPECIAL_WIDTH		210

#endif	//DEFINE_TAB_H