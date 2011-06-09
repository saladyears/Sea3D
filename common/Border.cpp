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
#include "Border.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CBorder, CObject, 1)

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CBorder::CBorder()
{
	//no tex yet
	m_nTexture = 0;

	//no coords yet
	::ZeroMemory(m_glCoords, sizeof(m_glCoords));

	//no rotation
	m_dRot = 0.;
}

//////////////////////////////////////////////////////////////////////
// copy constructor
//////////////////////////////////////////////////////////////////////
CBorder::CBorder(const CBorder &data)
{
	copy(data);
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CBorder::~CBorder()
{

}

//////////////////////////////////////////////////////////////////////
// assignment operator
//////////////////////////////////////////////////////////////////////
CBorder &CBorder::operator =(const CBorder &data)
{
	copy(data);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// streamline copy function
//////////////////////////////////////////////////////////////////////
void CBorder::copy(const CBorder &data)
{
	//texture name
	m_nTexture			= data.m_nTexture;

	//gl coords
	memcpy(m_glCoords, data.m_glCoords, sizeof(m_glCoords));

	//rotation
	m_dRot				= data.m_dRot;
}

//////////////////////////////////////////////////////////////////////
// save or load this puppy
//////////////////////////////////////////////////////////////////////
void CBorder::Serialize(CArchive &ar)
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
void CBorder::load(CArchive &ar)
{
	//texture name
	ar >> m_nTexture;

	//coords
	ar.Read(m_glCoords, sizeof(m_glCoords));

	//rotation
	ar >> m_dRot;
}

//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CBorder::save(CArchive &ar)
{
	//texture name
	ar << m_nTexture;

	//coords
	ar.Write(m_glCoords, sizeof(m_glCoords));

	//rotation
	ar << m_dRot;
}

#ifndef MAPS3D
//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CBorder::SerializeToMessage(int &iStart, CNDKMessage &msg)
{
	//texture name
	msg.SetAt(iStart++, m_nTexture);

	//coords
	msg.SetAt(iStart++, m_glCoords, sizeof(m_glCoords));

	//rotation
	msg.SetAt(iStart++, m_dRot);
}

//////////////////////////////////////////////////////////////////////
// load data
//////////////////////////////////////////////////////////////////////
void CBorder::SerializeFromMessage(int &iStart, CNDKMessage &msg)
{
	//texture name
	msg.GetAt(iStart++, m_nTexture);

	//coords
	msg.GetAt(iStart++, m_glCoords, sizeof(m_glCoords));

	//rotation
	msg.GetAt(iStart++, m_dRot);
}
#endif