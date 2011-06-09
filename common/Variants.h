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

#if !defined(AFX_VARIANTS_H__19822CB1_FDAB_4DF6_B655_46FE8BC0D7B2__INCLUDED_)
#define AFX_VARIANTS_H__19822CB1_FDAB_4DF6_B655_46FE8BC0D7B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "define.h"

//conditional include
#ifndef AI_DLL
#include "NDKMessage.h"
#endif

//////////////////////////////////////////////////////////////////////
// class which holds information about one color
//////////////////////////////////////////////////////////////////////
class CVariants : public CObject
{
public:
	DECLARE_SERIAL(CVariants)

	//{{AFX_VIRTUAL(CVariants)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	CVariants();
	CVariants(const CVariants &data);
	virtual ~CVariants();

	//assignment operator
	CVariants &operator =(const CVariants &data);

	//compare operator
	BOOL operator ==(const CVariants &data);

	//streamline copy function
	void copy(const CVariants &data);

	//returns the variant string
	CString getVariantString(void);

//conditional include
#ifndef AI_DLL
	//////////////////////////////////////////////////////////////////
	// network functions
	//////////////////////////////////////////////////////////////////
	void SerializeToMessage(int &iStart, CNDKMessage &msg);
	void SerializeFromMessage(int &iStart, CNDKMessage &msg);
#endif

public:
	//////////////////////////////////////////////////////////////////
	// all the data
	//////////////////////////////////////////////////////////////////

	//the bit string of variants on/off
	int m_iVariants;

	//variant settings
	int m_iStockRoads;
	int m_iStockCities;
	int m_iStockSettlements;
	int m_iStockShips;
	int m_iAllowedCards;
	int m_iPoints;

	//how many rounds no 7's should last
	int m_nNoSevens;

	//number of trade offers per round
	int m_nTradeOffers;

private:
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);
};

#endif // !defined(AFX_VARIANTS_H__19822CB1_FDAB_4DF6_B655_46FE8BC0D7B2__INCLUDED_)
