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
#include "NDKClientSocket.h"
#include "NDKClient.h"


////////////////////////////////////////////////////////////////////////////////
// Constructor / Destructor                                                   //
////////////////////////////////////////////////////////////////////////////////

// Constructor.
CNDKClientSocket::CNDKClientSocket(CNDKClient* pClient)
{
	ASSERT(pClient != NULL);

	m_pClient = pClient;
}


// Destructor.
CNDKClientSocket::~CNDKClientSocket()
{
}


////////////////////////////////////////////////////////////////////////////////
// Overrides from CSocket                                                     //
////////////////////////////////////////////////////////////////////////////////

// Called when data is received.
void CNDKClientSocket::OnReceive(int nErrorCode) 
{
	CString str;
	UINT ui;

	//remove read notifications
	VERIFY(AsyncSelect(/*FD_READ | */FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE));

	CSocket::OnReceive(nErrorCode);

	ASSERT(m_pClient != NULL);

	if (m_pClient != NULL)
		m_pClient->ProcessPendingRead(nErrorCode);

	//reset notifications to default
	if(0 != GetSockName(str, ui))
	{
		VERIFY(AsyncSelect());
	}
}
