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
#pragma once
#include "NDKDataBuffer.h"

////////////////////////////////////////////////////////////////////////////////
// Defines                                                                    //
////////////////////////////////////////////////////////////////////////////////

// Enumeration of all C++ basic types.
enum NDKDataType
{
	NDKDataUChar,
	NDKDataChar,
	NDKDataUShort,
	NDKDataShort,
	NDKDataUInt,
	NDKDataInt,
	NDKDataULong,
	NDKDataLong,
	NDKDataFloat,
	NDKDataDouble,
	NDKDataString,
	NDKDataBuffer
};


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// CNDKMessageData (concrete class)                                           //
//                                                                            //
// -------------------------------------------------------------------------- //
//                                                                            //
// This class contains one data. All C++ basic types is supported, plus a     //
// CString and an arbitrary length buffer of untyped data.                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class CNDKMessageData
{
public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors / Destructor                                              //
	////////////////////////////////////////////////////////////////////////////

	// Constructor.
	CNDKMessageData();

	// Constructor from a UCHAR.
	CNDKMessageData(UCHAR ucData);

	// Constructor from a char.
	CNDKMessageData(char cData);

	// Constructor from a USHORT.
	CNDKMessageData(USHORT usData);

	// Constructor from a short.
	CNDKMessageData(short sData);

	// Constructor from a UINT.
	CNDKMessageData(UINT unData);

	// Constructor from a int.
	CNDKMessageData(int nData);

	// Constructor from a ULONG.
	CNDKMessageData(ULONG ulData);

	// Constructor from a long.
	CNDKMessageData(long lData);

	// Constructor from a float.
	CNDKMessageData(float fData);

	// Constructor from a double.
	CNDKMessageData(double dData);

	// Constructor from a CString.
	CNDKMessageData(const CString& strData);

	// Constructor from a untyped data.
	CNDKMessageData(LPVOID pData, UINT unLength);

	// Copy-Constructor.
	CNDKMessageData(const CNDKMessageData& messageData);

	// Destructor.
	virtual ~CNDKMessageData();

	////////////////////////////////////////////////////////////////////////////
	// Attributes                                                             //
	////////////////////////////////////////////////////////////////////////////

	// Gets the data type.
	NDKDataType GetDataType() const;

	// Gets the data represented by a UCHAR.
	BOOL Get(UCHAR& ucData) const;

	 // Gets the data represented by a char.
	BOOL Get(char& cData) const;

	// Gets the data represented by a USHORT.
	BOOL Get(USHORT& usData) const;

	// Gets the data represented by a short.
	BOOL Get(short& sData) const;

	// Gets the data represented by a UINT.
	BOOL Get(UINT& unData) const;

	// Gets the data represented by a int.
	BOOL Get(int& nData) const;

	// Gets the data represented by a ULONG.
	BOOL Get(ULONG& ulData) const;

	// Gets the data represented by a long.
	BOOL Get(long& lData) const;

	// Gets the data represented by a float.
	BOOL Get(float& fData) const;

	// Gets the data represented by a double.
	BOOL Get(double& dData) const;

	// Gets the data represented by a CString.
	BOOL Get(CString& strData) const;

	// Gets the data represented by a untyped data.
	BOOL Get(LPVOID pData, UINT unLength) const;

	////////////////////////////////////////////////////////////////////////////
	// Operations                                                             //
	////////////////////////////////////////////////////////////////////////////

	// Serializes this object.
	void Serialize(CArchive& archive);

	////////////////////////////////////////////////////////////////////////////
	// Operators                                                              //
	////////////////////////////////////////////////////////////////////////////

	// Assignment operator.
	const CNDKMessageData& operator=(const CNDKMessageData& messageData);

private:
	union
	{
		UCHAR  uc;
		char   c;
		USHORT us;
		short  s;
		UINT   un;
		int    n;
		ULONG  ul;
		long   l;
		float  f;
		double d;
	} m_union;
	
	NDKDataType	   m_dataType;
	CString	       m_strData;
	CNDKDataBuffer m_dataBuffer;
};
