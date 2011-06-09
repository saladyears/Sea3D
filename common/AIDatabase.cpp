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
#include "AIDatabase.h"

//conditional includes
#ifndef AI_DLL
#include "settlers.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// static instantiations
/////////////////////////////////////////////////////////////////////////////
CAIDatabase *				CAIDatabase::m_pAIDatabase;
CArray <CAIUnit, CAIUnit>	CAIDatabase::m_AI;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAIDatabase::CAIDatabase()
{

}

CAIDatabase::~CAIDatabase()
{

}

/////////////////////////////////////////////////////////////////////////////
// instance handling
/////////////////////////////////////////////////////////////////////////////
CAIDatabase *CAIDatabase::instance()
{
	//see if a game database already is open
	if(NULL == m_pAIDatabase)
	{
		//get a new audit handle
		m_pAIDatabase = new CAIDatabase();

		//get preliminary information
		open();
	}

	//return the instance
	return m_pAIDatabase;
}

/////////////////////////////////////////////////////////////////////////////
// release data
/////////////////////////////////////////////////////////////////////////////
void CAIDatabase::release()
{
	//delete the static instance if it exists
	if(NULL != m_pAIDatabase)
	{
		delete m_pAIDatabase;
		m_pAIDatabase = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// return the number of players in the database
/////////////////////////////////////////////////////////////////////////////
int CAIDatabase::numAI()
{
	return m_AI.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// opens the database and gets information
/////////////////////////////////////////////////////////////////////////////
void CAIDatabase::open(void)
{
	int i;

	//reset the player array
	m_AI.RemoveAll();

	//resize
	m_AI.SetSize(5);

	//cycle through
	for(i = 0; i < 5; i++)
	{
		//set AI player ID
		m_AI[i].m_uiID = (UINT) i;

		switch(i)
		{
		case 0:
			m_AI[i].m_strName = "City Builder";
			m_AI[i].addStyle(AI_STYLE_CITIES);
			break;
		case 1:
			m_AI[i].m_strName = "Expander";
			m_AI[i].addStyle(AI_STYLE_EXPAND);
			break;
		case 2:
			m_AI[i].m_strName = "Dev Cards";
			m_AI[i].addStyle(AI_STYLE_DEV_CARDS);
			break;
		case 3:
			m_AI[i].m_strName = "Port Authority";
			m_AI[i].addStyle(AI_STYLE_PORTS);
			break;
		case 4:
			m_AI[i].m_strName = "Hybrid";
			m_AI[i].addStyle(AI_STYLE_CITIES);
			m_AI[i].addStyle(AI_STYLE_EXPAND);
			break;
		default: break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// retrieve an AI text name
/////////////////////////////////////////////////////////////////////////////
CString CAIDatabase::getAIName(int iAI)
{
	return m_AI[iAI].m_strName;
}

/////////////////////////////////////////////////////////////////////////////
// retrieve an AI player ID
/////////////////////////////////////////////////////////////////////////////
UINT CAIDatabase::getAIID(int iAI)
{
	return m_AI[iAI].m_uiID;
}

/////////////////////////////////////////////////////////////////////////////
// retrieve an AI style
/////////////////////////////////////////////////////////////////////////////
int CAIDatabase::getAIStyle(int iAI)
{
	return m_AI[iAI].m_iStyle;
}

/////////////////////////////////////////////////////////////////////////////
// return an AI weight
/////////////////////////////////////////////////////////////////////////////
float *CAIDatabase::getAIWeights(int iAI)
{
	return m_AI[iAI].m_dWeights;
}

/////////////////////////////////////////////////////////////////////////////
// return an AI repeat multiplier
/////////////////////////////////////////////////////////////////////////////
float CAIDatabase::getAIRepeatMult(int iAI)
{
	return m_AI[iAI].m_dRepeatMult;
}