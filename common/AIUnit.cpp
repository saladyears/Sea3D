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

#include "stdafx.h"
#include "AIUnit.h"
#include "defineGame.h"

//conditional includes
#ifndef AI_DLL
#include "settlers.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAIUnit, CObject, 1)

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CAIUnit::CAIUnit()
{
	int i;

	//no ID yet
	m_uiID = 0;

	//no style yet
	m_iStyle = AI_STYLE_DEFAULT;

	//reset all weights
	for(i = 0; i < RES_SIZE; i++)
	{
		m_dWeights[i] = 1.0;
	}

	//repeat multiplier
	m_dRepeatMult = 1.0;
}

//////////////////////////////////////////////////////////////////////
// copy constructor
//////////////////////////////////////////////////////////////////////
CAIUnit::CAIUnit(const CAIUnit &data)
{
	copy(data);
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CAIUnit::~CAIUnit()
{

}

//////////////////////////////////////////////////////////////////////
// assignment operator
//////////////////////////////////////////////////////////////////////
CAIUnit &CAIUnit::operator =(const CAIUnit &data)
{
	copy(data);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// streamline copy function
//////////////////////////////////////////////////////////////////////
void CAIUnit::copy(const CAIUnit &data)
{
	//AI name
	m_strName			= data.m_strName;

	//AI player ID
	m_uiID				= data.m_uiID;

	//playing style
	m_iStyle			= data.m_iStyle;
}

//////////////////////////////////////////////////////////////////////
// save or load this puppy
//////////////////////////////////////////////////////////////////////
void CAIUnit::Serialize(CArchive &ar)
{
	//save information
	if(ar.IsStoring())
	{
		save(ar);
	}
	//load information
	else
	{
		load(ar);
	}
}

//////////////////////////////////////////////////////////////////////
// load data
//////////////////////////////////////////////////////////////////////
void CAIUnit::load(CArchive &ar)
{
	//name
	ar >> m_strName;

	//AI player ID
	ar >> m_uiID;

	//playing style
	ar >> m_iStyle;
}

//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CAIUnit::save(CArchive &ar)
{
	//name
	ar << m_strName;

	//AI player ID
	ar << m_uiID;

	//playing style
	ar << m_iStyle;
}

//////////////////////////////////////////////////////////////////////
// set the AI playing style
//////////////////////////////////////////////////////////////////////
void CAIUnit::addStyle(int iStyle)
{
	//set style
	m_iStyle |= iStyle;

	//set weights
	switch(iStyle)
	{
	case AI_STYLE_CITIES:
		m_dWeights[RES_TIMBER]		*= 0.5f;
		m_dWeights[RES_WHEAT]		*= 0.9f;
		m_dWeights[RES_ORE]			*= 1.0f;
		m_dWeights[RES_CLAY]		*= 0.5f;
		m_dWeights[RES_SHEEP]		*= 0.5f;
		m_dWeights[RES_DESERT]		*= 0.0f;
		m_dWeights[RES_OCEAN]		*= 0.0f;
		m_dWeights[RES_PORT3]		*= 0.5f;
		m_dWeights[RES_PORTTIMBER]	*= 0.3f;
		m_dWeights[RES_PORTWHEAT]	*= 0.3f;
		m_dWeights[RES_PORTORE]		*= 0.3f;
		m_dWeights[RES_PORTCLAY]	*= 0.3f;
		m_dWeights[RES_PORTSHEEP]	*= 0.3f;
		m_dWeights[RES_GOLD]		*= 1.0f;
		m_dRepeatMult *= 1.1f;
		break;
	case AI_STYLE_EXPAND:
		m_dWeights[RES_TIMBER]		*= 0.9f;
		m_dWeights[RES_WHEAT]		*= 0.7f;
		m_dWeights[RES_ORE]			*= 0.4f;
		m_dWeights[RES_CLAY]		*= 1.0f;
		m_dWeights[RES_SHEEP]		*= 0.6f;
		m_dWeights[RES_DESERT]		*= 0.0f;
		m_dWeights[RES_OCEAN]		*= 0.0f;
		m_dWeights[RES_PORT3]		*= 0.5f;
		m_dWeights[RES_PORTTIMBER]	*= 0.3f;
		m_dWeights[RES_PORTWHEAT]	*= 0.3f;
		m_dWeights[RES_PORTORE]		*= 0.3f;
		m_dWeights[RES_PORTCLAY]	*= 0.3f;
		m_dWeights[RES_PORTSHEEP]	*= 0.3f;
		m_dWeights[RES_GOLD]		*= 1.0f;
		m_dRepeatMult *= 0.2f;
		break;
	case AI_STYLE_DEV_CARDS:
		m_dWeights[RES_TIMBER]		*= 0.5f;
		m_dWeights[RES_WHEAT]		*= 0.8f;
		m_dWeights[RES_ORE]			*= 0.9f;
		m_dWeights[RES_CLAY]		*= 0.5f;
		m_dWeights[RES_SHEEP]		*= 0.8f;
		m_dWeights[RES_DESERT]		*= 0.0f;
		m_dWeights[RES_OCEAN]		*= 0.0f;
		m_dWeights[RES_PORT3]		*= 0.5f;
		m_dWeights[RES_PORTTIMBER]	*= 0.3f;
		m_dWeights[RES_PORTWHEAT]	*= 0.3f;
		m_dWeights[RES_PORTORE]		*= 0.3f;
		m_dWeights[RES_PORTCLAY]	*= 0.3f;
		m_dWeights[RES_PORTSHEEP]	*= 0.3f;
		m_dWeights[RES_GOLD]		*= 1.0f;
		m_dRepeatMult *= 0.5f;
		break;
	case AI_STYLE_PORTS:
		//easier to get resources that are more common
		m_dWeights[RES_TIMBER]		*= 0.6f;
		m_dWeights[RES_WHEAT]		*= 0.6f;
		m_dWeights[RES_ORE]			*= 0.4f;
		m_dWeights[RES_CLAY]		*= 0.4f;
		m_dWeights[RES_SHEEP]		*= 0.6f;
		m_dWeights[RES_DESERT]		*= 0.0f;
		m_dWeights[RES_OCEAN]		*= 0.0f;
		m_dWeights[RES_PORT3]		*= 0.5f;
		m_dWeights[RES_PORTTIMBER]	*= 0.8f;
		m_dWeights[RES_PORTWHEAT]	*= 0.8f;
		m_dWeights[RES_PORTORE]		*= 0.8f;
		m_dWeights[RES_PORTCLAY]	*= 0.8f;
		m_dWeights[RES_PORTSHEEP]	*= 0.8f;
		m_dWeights[RES_GOLD]		*= 1.0f;
		m_dRepeatMult *= 2.5f;
		break;
	}
}