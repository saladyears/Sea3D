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

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// NDK 2.0 - Network Development Kit                                          //
//                                                                            //
// Authors: Sebastien Lachance                                                //
//                                                                            //
// E-mail:  netblitz@rocler.qc.ca                                             //
//                                                                            //
// -------------------------------------------------------------------------- //
//                                                                            //
// Permission to use, copy, modify, and distribute this software for any      //
// purpose and without fee is hereby granted. This is no guarantee about the  //
// use of this software. For any comments, bugs or thanks, please email us.   //
//                                                                            //
// -------------------------------------------------------------------------- //
//                                                                            //
// Targeted Platform: Any Windows version                                     //
//                                                                            //
// Compiled with:     Visual C++ 6.0                                          //
//                                                                            //
// Last modification: January 2002                                            //
//                                                                            //
// History:                                                                   //
//                                                                            //
// 1- First release of this file.                                             //
// 2- The class is renamed and some optimizations are applied. Hungarian      //
//    notation is used.                                                       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes                                                                   //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "NDKMessageData.h"
#include "NDKDataBuffer.h"


////////////////////////////////////////////////////////////////////////////////
// Constructors / Destructor                                                  //
////////////////////////////////////////////////////////////////////////////////

// Constructor.
CNDKMessageData::CNDKMessageData()
{
	m_dataType = NDKDataString;
}


// Constructor from a UCHAR.
CNDKMessageData::CNDKMessageData(UCHAR ucData)
{
	m_union.uc = ucData;
	m_dataType = NDKDataUChar;
}


// Constructor from a char.
CNDKMessageData::CNDKMessageData(char cData)
{
	m_union.c  = cData;
	m_dataType = NDKDataChar;
}


// Constructor from a USHORT.
CNDKMessageData::CNDKMessageData(USHORT usData)
{
	m_union.us = usData;
	m_dataType = NDKDataUShort;
}


// Constructor from a short.
CNDKMessageData::CNDKMessageData(short sData)
{
	m_union.s  = sData;
	m_dataType = NDKDataShort;
}


// Constructor from a UINT.
CNDKMessageData::CNDKMessageData(UINT unData)
{
	m_union.un = unData;
	m_dataType = NDKDataUInt;
}


// Constructor from a int.
CNDKMessageData::CNDKMessageData(int nData)
{
	m_union.n  = nData;
	m_dataType = NDKDataInt;
}


// Constructor from a ULONG.
CNDKMessageData::CNDKMessageData(ULONG ulData)
{
	m_union.ul = ulData;
	m_dataType = NDKDataULong;
}


// Constructor from a long.
CNDKMessageData::CNDKMessageData(long lData)
{
	m_union.l  = lData;
	m_dataType = NDKDataLong;
}


// Constructor from a float.
CNDKMessageData::CNDKMessageData(float fData)
{
	m_union.f  = fData;
	m_dataType = NDKDataFloat;
}


// Constructor from a double.
CNDKMessageData::CNDKMessageData(double dData)
{
	m_union.d  = dData;
	m_dataType = NDKDataDouble;
}


// Constructor from a CString.
CNDKMessageData::CNDKMessageData(const CString& strData)
{
	m_strData  = strData;
	m_dataType = NDKDataString;
}


// Constructor from a untyped data.
CNDKMessageData::CNDKMessageData(LPVOID pData, UINT unLength)
{
	m_dataBuffer.SetBuffer(pData, unLength);
	m_dataType = NDKDataBuffer;
}


// Copy-Constructor.
CNDKMessageData::CNDKMessageData(const CNDKMessageData& messageData)
{
	m_dataType = messageData.m_dataType;
	m_union    = messageData.m_union;
	m_strData  = messageData.m_strData;
	m_dataBuffer.SetBuffer(messageData.m_dataBuffer.GetBuffer(), 
						   messageData.m_dataBuffer.GetLength());
}


// Destructor.
CNDKMessageData::~CNDKMessageData()
{
}


/////////////////////////////////////////////////////////////////////////////
// Attributes
/////////////////////////////////////////////////////////////////////////////

// Gets the data type.
NDKDataType CNDKMessageData::GetDataType() const
{
	return m_dataType;
}


// Gets the data represented by a UCHAR.
BOOL CNDKMessageData::Get(UCHAR& ucData) const
{
	ucData = m_union.uc;

	return m_dataType == NDKDataUChar;
}


// Gets the data represented by a char.
BOOL CNDKMessageData::Get(char& cData) const
{
	cData = m_union.c;

	return m_dataType == NDKDataChar;
}


// Gets the data represented by a USHORT.
BOOL CNDKMessageData::Get(USHORT& usData) const
{
	usData = m_union.us;

	return m_dataType == NDKDataUShort;
}


// Gets the data represented by a short.
BOOL CNDKMessageData::Get(short& sData) const
{
	sData = m_union.s;

	return m_dataType == NDKDataShort;
}


// Gets the data represented by a UINT.
BOOL CNDKMessageData::Get(UINT& unData) const
{
	unData = m_union.un;

	return m_dataType == NDKDataUInt;
}


// Gets the data represented by a int.
BOOL CNDKMessageData::Get(int& nData) const
{
	nData = m_union.n;

	return m_dataType == NDKDataInt;
}


// Gets the data represented by a ULONG.
BOOL CNDKMessageData::Get(ULONG& ulData) const
{
	ulData = m_union.ul;

	return m_dataType == NDKDataULong;
}


// Gets the data represented by a long.
BOOL CNDKMessageData::Get(long& lData) const
{
	lData = m_union.l;

	return m_dataType == NDKDataLong;
}


// Gets the data represented by a float.
BOOL CNDKMessageData::Get(float& fData) const
{
	fData = m_union.f;

	return m_dataType == NDKDataFloat;
}


// Gets the data represented by a double.
BOOL CNDKMessageData::Get(double& dData) const
{
	dData = m_union.d;

	return m_dataType == NDKDataDouble;
}


// Gets the data represented by a CString.
BOOL CNDKMessageData::Get(CString& strData) const
{
	strData = m_strData;

	return m_dataType == NDKDataString;
}


// Gets the data represented by a untyped data.
BOOL CNDKMessageData::Get(LPVOID pData, UINT unLength) const
{
	if ((m_dataType == NDKDataBuffer) && (pData != NULL))
	{
		memcpy(pData, 
			   m_dataBuffer.GetBuffer(), 
			   min(unLength, m_dataBuffer.GetLength()));
	}

	return m_dataType == NDKDataBuffer;
}


////////////////////////////////////////////////////////////////////////////////
// Operations                                                                 //
////////////////////////////////////////////////////////////////////////////////

// Serializes this object.
void CNDKMessageData::Serialize(CArchive& archive)
{
	if (archive.IsStoring())
	{
		archive << (UCHAR)m_dataType;

		switch (m_dataType)
		{
		case NDKDataString:
			archive << m_strData;
			break;
		
		case NDKDataBuffer:
			m_dataBuffer.Serialize(archive);
			break;
		
		default:
			archive.Write(&m_union, sizeof(m_union));
			break;
		}
	}
	else
	{
		UCHAR ucDataType = '\0';

		archive >> ucDataType;
		
		m_dataType = (NDKDataType)ucDataType;

		switch((NDKDataType)ucDataType)
		{
		case NDKDataString:
			archive >> m_strData;
			break;
		
		case NDKDataBuffer:
			m_dataBuffer.Serialize(archive);
			break;

		default: 
			archive.Read(&m_union, sizeof(m_union));
			break;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// Operators                                                                  //
////////////////////////////////////////////////////////////////////////////////

// Assignment operator.
const CNDKMessageData& CNDKMessageData::operator=(const CNDKMessageData& messageData)
{
	if (&messageData != this)
	{
		m_dataType = messageData.m_dataType;
		m_union    = messageData.m_union;
		m_strData  = messageData.m_strData;
		m_dataBuffer.SetBuffer(messageData.m_dataBuffer.GetBuffer(), 
							   messageData.m_dataBuffer.GetLength());
	}

	return *this;
}
