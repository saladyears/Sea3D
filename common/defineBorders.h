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
// border macros
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_BORDERS_H)
#define DEFINE_BORDERS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// borders with no X tiles - the smallest possible borders
/////////////////////////////////////////////////////////////////////////////
#define ASSIGN_MAP_BORDERS_NO_X \
	{ \
	int nBorder = 0; \
	pMap->m_mapBorders.resize(8); \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_AA; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 6.715; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 6.810; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_HA; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 4.590; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 11.060; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_GG; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 2.465; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 15.310; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_EFG; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = -X * 2.125; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 14.25; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_EE; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -5.215; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = Z * -10.19; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_DE; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -3.090; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = Z * -5.94; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_CC; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -0.965; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = Z * -1.69; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_ABC; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 3.625; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 2.75; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	}

/////////////////////////////////////////////////////////////////////////////
// borders with one X tile
/////////////////////////////////////////////////////////////////////////////
#define ASSIGN_MAP_BORDERS_X \
	{ \
	int nBorder = 0; \
	pMap->m_mapBorders.resize(10); \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_AA; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 8.215; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 7.810; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_HA; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 6.090; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 12.060; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_GG; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 3.965; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 16.310; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XFG; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 0.500; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 16.00; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XEF; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -3.25; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 13.50; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_EE; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -5.215; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = Z * -10.19; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_DE; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -3.090; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = Z * -5.94; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_CC; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -0.965; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = Z * -1.69; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XAB; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 6.250; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 4.50; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XBC; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 2.50; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 2.00; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	}

/////////////////////////////////////////////////////////////////////////////
// borders with one XX tile
/////////////////////////////////////////////////////////////////////////////
#define ASSIGN_MAP_BORDERS_XX \
	{\
	int nBorder = 0; \
	pMap->m_mapBorders.resize(10); \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_AA; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 9.715; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 8.810; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_HA; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 7.590; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 13.060; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_GG; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 5.465; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 17.310; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXFG; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 1.625; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 16.75; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXEF; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -2.875; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 13.75; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_EE; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -5.215; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 10.19; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_DE; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -3.090; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 5.94; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_CC; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -0.965; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 1.69; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXBC; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 2.875; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 2.25; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXAB; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 7.375; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 5.25; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	}

/////////////////////////////////////////////////////////////////////////////
// borders with one XX and one X tile
/////////////////////////////////////////////////////////////////////////////
#define ASSIGN_MAP_BORDERS_XX_X \
	{\
	int nBorder = 0; \
	pMap->m_mapBorders.resize(12); \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_AA; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 11.215; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 9.810; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_HA; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 9.090; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 14.060; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_GG; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 6.965; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 18.310; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXFG; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 3.125; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 17.75; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_X_XX; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = -X * 0.25; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 15.50; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XEF; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -3.250; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 13.50; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_EE; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -5.215; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 10.19; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_DE; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -3.090; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 5.94; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_CC; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -0.965; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 1.69; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XBC; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 2.500; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 2.00; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XX_X; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 5.500; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 4.00; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXAB; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 8.875; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 6.25; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	}

/////////////////////////////////////////////////////////////////////////////
// borders with two XX tiles
/////////////////////////////////////////////////////////////////////////////
#define ASSIGN_MAP_BORDERS_XX_XX \
	{\
	int nBorder = 0; \
	pMap->m_mapBorders.resize(12); \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_AA; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 12.715; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 10.810; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_HA; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 10.590; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 15.060; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_GG; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 8.465; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 19.310; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXFG; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 4.625; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 18.75; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XX_XX; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 0.875; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 16.25; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXEF; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -2.875; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 13.75; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_EE; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -5.215; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 10.19; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_DE; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -3.090; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 5.94; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_CC; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -0.965; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 1.69; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXBC; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 2.875; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 2.25; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XX_XX; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 6.625; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 4.75; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXAB; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 10.375; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 7.25; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	}

/////////////////////////////////////////////////////////////////////////////
// borders with two XX tiles and on X tile
/////////////////////////////////////////////////////////////////////////////
#define ASSIGN_MAP_BORDERS_XX_X_XX \
	{\
	int nBorder = 0; \
	pMap->m_mapBorders.resize(14); \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_AA; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 14.215; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 11.810; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_HA; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 12.090; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 16.060; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_GG; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 9.965; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 20.310; \
	pMap->m_mapBorders[nBorder++].m_dRot = -60.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXFG; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 6.125; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 19.75; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XX_X; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 0.50; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 16.00; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_X_XX; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 2.75; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 17.50; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXEF; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -2.875; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 13.75; \
	pMap->m_mapBorders[nBorder++].m_dRot = 30.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_EE; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -5.215; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 10.19; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_DE; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -3.090; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 5.94; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_CC; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * -0.965; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 1.69; \
	pMap->m_mapBorders[nBorder++].m_dRot = 120.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXBC; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 2.875; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 2.25; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_X_XX; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 6.250; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 4.50; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XX_X; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 8.500; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 6.00; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	pMap->m_mapBorders[nBorder].m_nTexture = GL_BORDER_XXAB; \
	pMap->m_mapBorders[nBorder].m_glCoords[0] = X * 11.875; \
	pMap->m_mapBorders[nBorder].m_glCoords[2] = -Z * 8.25; \
	pMap->m_mapBorders[nBorder++].m_dRot = -150.; \
	}
#endif	//DEFINE_BORDERS_H