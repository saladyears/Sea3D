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
// code macros
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_CODE_H)
#define DEFINE_CODE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// OpenGL tile/corner or tile/side selection
/////////////////////////////////////////////////////////////////////////////
#define ENCODE_SEL(iTile, iSide) \
	(iTile * 10) + \
	(iSide * 1)

#define DECODE_SEL(ID, iTile, iSide) \
	{ \
	iTile = (ID / 10); \
	iSide = (ID % 10); \
	}

/////////////////////////////////////////////////////////////////////////////
// initial settlements
/////////////////////////////////////////////////////////////////////////////
#define ENCODE_INIT_SETTLEMENT(iPlayer, iTile1, iTile2, iTile3, iCorner) \
	(iPlayer	* 10000000) + \
	(iTile1		* 100000) + \
	(iTile2		* 1000) + \
	(iTile3		* 10) + \
	(iCorner	* 1)

#define DECODE_INIT_SETTLEMENT(ID, iPlayer, iTile1, iTile2, iTile3, iCorner) \
	{ \
	iPlayer		= (ID / 10000000);	ID -= (iPlayer * 10000000); \
	iTile1		= (ID / 100000);	ID -= (iTile1 * 100000); \
	iTile2		= (ID / 1000);		ID -= (iTile2 * 1000); \
	iTile3		= (ID / 10);		ID -= (iTile3 * 10); \
	iCorner		= (ID / 1); \
	}

/////////////////////////////////////////////////////////////////////////////
// in-game settlements
/////////////////////////////////////////////////////////////////////////////
#define ENCODE_SETTLEMENT(iLongest, iTile1, iTile2, iTile3, iCorner) \
	(iLongest	* 10000000) + \
	(iTile1		* 100000) + \
	(iTile2		* 1000) + \
	(iTile3		* 10) + \
	(iCorner	* 1)

#define DECODE_SETTLEMENT(ID, iLongest, iTile1, iTile2, iTile3, iCorner) \
	{ \
	iLongest	= (ID / 10000000);	ID -= (iLongest * 10000000); \
	iTile1		= (ID / 100000);	ID -= (iTile1 * 100000); \
	iTile2		= (ID / 1000);		ID -= (iTile2 * 1000); \
	iTile3		= (ID / 10);		ID -= (iTile3 * 10); \
	iCorner		= (ID / 1); \
	}

/////////////////////////////////////////////////////////////////////////////
// initial roads
/////////////////////////////////////////////////////////////////////////////
#define ENCODE_INIT_ROAD(iTile1, iCorner1, iTile2, iCorner2, iLastCity) \
	(iTile1		* 10000000) + \
	(iCorner1	* 1000000) + \
	(iTile2		* 10000) + \
	(iCorner2	* 1000) + \
	(iLastCity	* 1)

#define DECODE_INIT_ROAD(ID, iTile1, iCorner1, iTile2, iCorner2, iLastCity) \
	{ \
	iTile1		= (ID / 10000000);	ID -= (iTile1 * 10000000); \
	iCorner1	= (ID / 1000000);	ID -= (iCorner1 * 1000000); \
	iTile2		= (ID / 10000);		ID -= (iTile2 * 10000); \
	iCorner2	= (ID / 1000);		ID -= (iCorner2 * 1000); \
	iLastCity	= (ID / 1); \
	}

/////////////////////////////////////////////////////////////////////////////
// in-game roads
/////////////////////////////////////////////////////////////////////////////
#define ENCODE_ROAD(ID, bFree, iTile1, iSide1, iTile2, iSide2, iLongest) \
	{\
	BOOL bNeg = (iLongest < 0); \
	ID = \
	(bFree			* 100000000) + \
	(iTile1			* 1000000) + \
	(iSide1			* 100000) + \
	(iTile2			* 1000) + \
	(iSide2			* 100) + \
	(abs(iLongest)	* 1); \
	if(bNeg) ID *=- 1; \
	}

#define DECODE_ROAD(ID, bFree, iTile1, iSide1, iTile2, iSide2, iLongest) \
	{ \
	BOOL bNeg = (ID < 0); \
	if(bNeg) ID *= -1; \
	bFree =		(ID / 100000000);	ID -= (bFree * 100000000); \
	iTile1 =	(ID / 1000000);		ID -= (iTile1 * 1000000); \
	iSide1 =	(ID / 100000);		ID -= (iSide1 * 100000); \
	iTile2 =	(ID / 1000);		ID -= (iTile2 * 1000); \
	iSide2 =	(ID / 100);			ID -= (iSide2 * 100); \
	iLongest =	(ID / 1); \
	if(bNeg) iLongest *= -1; \
	}

/////////////////////////////////////////////////////////////////////////////
// ship movement
/////////////////////////////////////////////////////////////////////////////
#define ENCODE_SHIP_MOVE(ID, iLongestOwner, iOldTile, iOldSide, iNewTile, iNewSide, iLongest) \
	{\
	BOOL bNeg = (iLongest < 0); \
	ID =  \
	(iLongestOwner	* 10000000) + \
	(iOldTile		* 100000) + \
	(iOldSide		* 10000) + \
	(iNewTile		* 100) + \
	(iNewSide		* 10) + \
	(abs(iLongest)	* 1); \
	if(bNeg) ID *= -1; \
	}

#define DECODE_SHIP_MOVE(ID, iLongestOwner, iOldTile, iOldSide, iNewTile, iNewSide, iLongest) \
	{ \
	BOOL bNeg = (ID < 0); \
	if(bNeg) ID *= -1; \
	iLongestOwner	= (ID / 10000000);	ID -= (iLongestOwner * 10000000); \
	iOldTile		= (ID / 100000);	ID -= (iOldTile * 100000); \
	iOldSide		= (ID / 10000);		ID -= (iOldSide * 10000); \
	iNewTile		= (ID / 100);		ID -= (iNewTile * 100); \
	iNewSide		= (ID / 10);		ID -= (iNewSide * 10); \
	iLongest		= (ID / 1); \
	if(bNeg) iLongest *= -1; \
	}

/////////////////////////////////////////////////////////////////////////////
// city placement
/////////////////////////////////////////////////////////////////////////////
#define ENCODE_CITY(iTile1, iTile2, iTile3, iCorner) \
	(iTile1		* 100000) + \
	(iTile2		* 1000) + \
	(iTile3		* 10) + \
	(iCorner	* 1)

#define DECODE_CITY(ID, iTile1, iTile2, iTile3, iCorner) \
	{ \
	iTile1	= (ID / 100000);	ID -= (iTile1 * 100000); \
	iTile2	= (ID / 1000);		ID -= (iTile2 * 1000); \
	iTile3	= (ID / 10);		ID -= (iTile3 * 10); \
	iCorner = (ID / 1); \
	}

/////////////////////////////////////////////////////////////////////////////
// robber placement
/////////////////////////////////////////////////////////////////////////////
#define ENCODE_ROBBER(iDie1, iDie2, iPrevTile, iPlayer, bSoldier) \
	(iDie1		* 100000) + \
	(iDie2		* 10000) + \
	(iPrevTile	* 100) + \
	(iPlayer	* 10) + \
	(bSoldier	* 1)

#define DECODE_ROBBER(ID, iDie1, iDie2, iPrevTile, iPlayer, bSoldier) \
	{ \
	iDie1		= (ID / 100000);	ID -= (iDie1 * 100000); \
	iDie2		= (ID / 10000);		ID -= (iDie2 * 10000); \
	iPrevTile	= (ID / 100);		ID -= (iPrevTile * 100); \
	iPlayer		= (ID / 10);		ID -= (iPlayer * 10); \
	bSoldier	= (ID / 1); \
	}

/////////////////////////////////////////////////////////////////////////////
// stolen resource
/////////////////////////////////////////////////////////////////////////////
#define ENCODE_STEAL(bPirate, iPlayer, iRes) \
	(bPirate	* 100) + \
	(iPlayer	* 10) + \
	(iRes		* 1);

#define DECODE_STEAL(ID, bPirate, iPlayer, iRes) \
	{ \
	bPirate		= (ID / 100);	ID -= (bPirate * 100); \
	iPlayer		= (ID / 10);	ID -= (iPlayer * 10); \
	iRes		= (ID / 1); \
	}

/////////////////////////////////////////////////////////////////////////////
// chitless swapping
/////////////////////////////////////////////////////////////////////////////
#define ENCODE_CHITLESS(iTile1, iTile2) \
	(iTile1		* 100) +  \
	(iTile2		* 1)

#define DECODE_CHITLESS(ID, iTile1, iTile2) \
	{ \
	iTile1		= (ID / 100);	ID -= (iTile1 * 100); \
	iTile2		= (ID / 1); \
	}

/////////////////////////////////////////////////////////////////////////////
// network information
/////////////////////////////////////////////////////////////////////////////
#define ENCODE_NET(iPlayer, iTile, iCorner) \
	(iPlayer	* 1000) +  \
	(iTile		* 10) +  \
	(iCorner	* 1)

#define DECODE_NET(ID, iPlayer, iTile, iCorner) \
	{ \
	iPlayer		= (ID / 1000);	ID -= (iPlayer * 1000); \
	iTile		= (ID / 10);	ID -= (iTile * 10); \
	iCorner		= (ID / 1); \
	}

/////////////////////////////////////////////////////////////////////////////
// volcano information
/////////////////////////////////////////////////////////////////////////////
#define ENCODE_VOLCANO(nPlayer, nTile, nCorner, nPrevious, nExtra) \
	(nPlayer	* 100000) + \
	(nTile		* 1000) +  \
	(nCorner	* 100) +  \
	(nPrevious	* 10) + \
	(nExtra		* 1)

#define DECODE_VOLCANO(ID, nPlayer, nTile, nCorner, nPrevious, nExtra) \
	{ \
	nPlayer		= (ID / 100000);	ID -= (nPlayer * 100000); \
	nTile		= (ID / 1000);		ID -= (nTile * 1000); \
	nCorner		= (ID / 100);		ID -= (nCorner * 100); \
	nPrevious	= (ID / 10);		ID -= (nPrevious * 10); \
	nExtra		= (ID / 1);			ID -= (nExtra * 1); \
	}


#endif	//DEFINE_CODE_H