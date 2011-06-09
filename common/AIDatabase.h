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

#if !defined(AFX_AIDATABASE_H__835E9A1D_39CA_4AFE_A582_7F35A4A0529A__INCLUDED_)
#define AFX_AIDATABASE_H__835E9A1D_39CA_4AFE_A582_7F35A4A0529A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// instance definition
//////////////////////////////////////////////////////////////////////
#define AI_DATABASE		CAIDatabase::instance()

//////////////////////////////////////////////////////////////////////
// included files
//////////////////////////////////////////////////////////////////////
#include "AIUnit.h"

//////////////////////////////////////////////////////////////////////
// static class for reading from the AI database
//////////////////////////////////////////////////////////////////////
class CAIDatabase  
{
public:
	//////////////////////////////////////////////////////////////////
	// constructor / destructor
	//////////////////////////////////////////////////////////////////
	CAIDatabase();
	virtual ~CAIDatabase();

protected:
	/////////////////////////////////////////////////////////////////////////
	// a pointer to a static instance of this class
	/////////////////////////////////////////////////////////////////////////
	static CAIDatabase *m_pAIDatabase;

	//opens the database and gets information
	static void open(void);

	//the array of AI's
	static CArray <CAIUnit, CAIUnit> m_AI;

public:
	/////////////////////////////////////////////////////////////////////
	// instanstiation/destruction methods
	/////////////////////////////////////////////////////////////////////
	static CAIDatabase *instance(void);
	static void release(void);

	//////////////////////////////////////////////////////////////////
	// information methods
	//////////////////////////////////////////////////////////////////
	static int numAI(void);

	//////////////////////////////////////////////////////////////////
	// database methods
	//////////////////////////////////////////////////////////////////
	static CString getAIName(int iAI);
	static UINT getAIID(int iAI);
	static int getAIStyle(int iAI);
	static float getAIRepeatMult(int iAI);
	static float *getAIWeights(int iAI);

};

#endif // !defined(AFX_AIDATABASE_H__835E9A1D_39CA_4AFE_A582_7F35A4A0529A__INCLUDED_)
