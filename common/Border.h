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

#if !defined(AFX_BORDER_H__52F58435_8545_4717_A376_2CD0EDA88985__INCLUDED_)
#define AFX_BORDER_H__52F58435_8545_4717_A376_2CD0EDA88985__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#ifndef MAPS3D
#include "NDKMessage.h"
#endif

//////////////////////////////////////////////////////////////////////
// class which holds border piece information
//////////////////////////////////////////////////////////////////////
class CBorder : public CObject  
{
public:
	DECLARE_SERIAL(CBorder)

	//{{AFX_VIRTUAL(CBorder)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	CBorder();
	CBorder(const CBorder &data);
	virtual ~CBorder();

	//assignment operator
	CBorder &operator =(const CBorder &data);

	//compare operator
	BOOL operator ==(const CBorder &data);

	//streamline copy function
	void copy(const CBorder &data);

#ifndef MAPS3D
	//write to/from message
	void SerializeToMessage(int &iStart, CNDKMessage &msg);
	void SerializeFromMessage(int &iStart, CNDKMessage &msg);
#endif

public:
	//////////////////////////////////////////////////////////////////
	// all the data
	//////////////////////////////////////////////////////////////////

	//the piece texture
	int m_nTexture;

	//the GL coordinates
	double m_glCoords[3];

	//any rotation
	double m_dRot;

private:
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);
};

#endif // !defined(AFX_BORDER_H__52F58435_8545_4717_A376_2CD0EDA88985__INCLUDED_)
