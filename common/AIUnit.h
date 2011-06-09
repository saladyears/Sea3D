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

#if !defined(AFX_AI_H__4F38E1C1_25A1_408F_852B_8579751F81BB__INCLUDED_)
#define AFX_AI_H__4F38E1C1_25A1_408F_852B_8579751F81BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "defineAI.h"
#include "defineGame.h"

//////////////////////////////////////////////////////////////////////
// class which holds information about one AI
//////////////////////////////////////////////////////////////////////
class CAIUnit : public CObject  
{
public:
	DECLARE_SERIAL(CAIUnit)

	//{{AFX_VIRTUAL(CAIUnit)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	CAIUnit();
	CAIUnit(const CAIUnit &data);
	virtual ~CAIUnit();

	//assignment operator
	CAIUnit &operator =(const CAIUnit &data);

	//compare operator
	BOOL operator ==(const CAIUnit &data);

	//streamline copy function
	void copy(const CAIUnit &data);

	//add an AI playing style
	void addStyle(int iStyle);

protected:
	//////////////////////////////////////////////////////////////////
	// all the data
	//////////////////////////////////////////////////////////////////

	//AI name
	CString m_strName;

	//AI player ID
	UINT m_uiID;

	//AI playing style
	int m_iStyle;

	//tile weights
	float m_dWeights[RES_SIZE];

	//repeat tile multiplier
	float m_dRepeatMult;

private:
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);

	//////////////////////////////////////////////////////////////////////
	// friends
	//////////////////////////////////////////////////////////////////////
	friend class CAIDatabase;
};

#endif // !defined(AFX_AI_H__4F38E1C1_25A1_408F_852B_8579751F81BB__INCLUDED_)
