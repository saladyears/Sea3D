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
//    notation is used. Adds method are added.                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes                                                                   //
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <afxtempl.h>
#include "NDKDefines.h"
#include "NDKMessageData.h"

////////////////////////////////////////////////////////////////////////////////
// Defines                                                                    //
////////////////////////////////////////////////////////////////////////////////
typedef CArray<CNDKMessageData, CNDKMessageData &> CElements;


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// CNDKMessage (concrete class)                                               //
//                                                                            //
// -------------------------------------------------------------------------- //
//                                                                            //
// This class is the primitive object that can be sent through the            //
// client/server architecture. It can be seen as an array of elements of      //
// different type. These types are all the C++ basic types as well as         //
// CString. It also supports an arbitrary length buffer of untyped data       //
// (LPVOID). The array grows automatically as needed. Elements within the     //
// array are zero-indexed.                                                    //
//                                                                            //
// Each message type is identified with a unique Id (long).                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class NDK_EXT_CLASS CNDKMessage : public CObject
{
public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors / Destructor                                              //
	////////////////////////////////////////////////////////////////////////////

	// Constructor.
	CNDKMessage();

	// Constructor with a unique Id.
	CNDKMessage(long lId);

	// Copy-Constructor.
	CNDKMessage(const CNDKMessage& message);
	
	// Destructor.
	virtual ~CNDKMessage();

	////////////////////////////////////////////////////////////////////////////
	// Attributes                                                             //
	////////////////////////////////////////////////////////////////////////////
 
	// Sets a unique identifier to each message.
	void SetId(long lId);

	// Gets the identifier of this message.
	long GetId() const;

	// Gets the number of elements in the array of this message.
	int GetNbElements() const;

	////////////////////////////////////////////////////////////////////////////
	// Operations                                                             //
	////////////////////////////////////////////////////////////////////////////

	// The following methods are used to add at the end an element in the 
	// internal array of the CNDKMessage to any value.
	void Add(UCHAR ucData);
	void Add(char cData);
	void Add(USHORT usData);
	void Add(short sData);
	void Add(UINT unData);
	void Add(int nData);
	void Add(ULONG ulData);
	void Add(long lData);
	void Add(float fData);
	void Add(double dData);
	void Add(const CString& strData);
	void Add(LPVOID pData, UINT unLength);

	// The following methods are used to set an element at the specified index
	// in the internal array of the CNDKMessage to any value. Index is 
	// zero-indexed.
	void SetAt(long lIndex, UCHAR ucData);
	void SetAt(long lIndex, char cData);
	void SetAt(long lIndex, USHORT usData);
	void SetAt(long lIndex, short sData);
	void SetAt(long lIndex, UINT unData);
	void SetAt(long lIndex, int nData);
	void SetAt(long lIndex, ULONG ulData);
	void SetAt(long lIndex, long lData);
	void SetAt(long lIndex, float fData);
	void SetAt(long lIndex, double dData);
	void SetAt(long lIndex, const CString& strData);
	void SetAt(long lIndex, LPVOID pData, UINT unLength);

	// Gets the data type at the specified index.
	NDKDataType GetDataTypeAt(long lIndex) const;

	// The following methods are used to get an element at the specified index
	// in the internal array of the CNDKMessage. The parameter passed by 
	// reference will be filled with the appropriate data. The type of data 
	// extracted must match the type of data set with a previous call to 
	// SetAt or Add. Index is zero-indexed.
	void GetAt(long lIndex, UCHAR& ucData) const;
	void GetAt(long lIndex, char& cData) const;
	void GetAt(long lIndex, USHORT& usData) const;
	void GetAt(long lIndex, short& sData) const;
	void GetAt(long lIndex, UINT& unData) const;
	void GetAt(long lIndex, int& nData) const;
	void GetAt(long lIndex, ULONG& ulData) const;
	void GetAt(long lIndex, long& lData) const;
	void GetAt(long lIndex, float& fData) const;
	void GetAt(long lIndex, double& dData) const;
	void GetAt(long lIndex, CString& strData) const;
	void GetAt(long lIndex, LPVOID pData, UINT unLength) const;

	// Resets the current index. The current index is used by GetNext method.
	void ResetCurrentIndex();

	// Gets the current index.
	long GetCurrentIndex() const;

	// Returns if the current index is the last.
	BOOL IsLastCurrentIndex();

	// The following methods are used to get an element in the internal array of 
	// the CNDKMessage using the current index. The parameter passed by 
	// reference will be filled with the appropriate data. The type of data 
	// extracted must match the type of data set with a previous call to 
	// SetAt or Add. 
	void GetNext(UCHAR& ucData);
	void GetNext(char& cData);
	void GetNext(USHORT& usData);
	void GetNext(short& sData);
	void GetNext(UINT& unData);
	void GetNext(int& nData);
	void GetNext(ULONG& ulData);
	void GetNext(long& lData);
	void GetNext(float& fData);
	void GetNext(double& dData);
	void GetNext(CString& strData);
	void GetNext(LPVOID pData, UINT unLength);

	// Serializes a CNDKMessage object to a CArchive
	void Serialize(CArchive& archive);

	DECLARE_SERIAL(CNDKMessage)

	////////////////////////////////////////////////////////////////////////////
	// Operators                                                              //
	////////////////////////////////////////////////////////////////////////////

	// Assignment operator.
	const CNDKMessage& operator=(const CNDKMessage& message);

private:
	////////////////////////////////////////////////////////////////////////////
	// Private Attributes                                                     //
	////////////////////////////////////////////////////////////////////////////

	// Returns if the index is valid for the array of elements when setting a
	// value.
	BOOL IsValidSetIndex(long lIndex) const;

	// Returns if the index is valid for the array of elements when getting a
	// value.
	BOOL IsValidGetIndex(long lIndex) const;

private:
	long      m_lId;
	long	  m_lCurrentIndex;
	CElements m_elements;
};
