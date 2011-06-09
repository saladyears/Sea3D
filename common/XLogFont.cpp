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
#include "XLogFont.h"

/////////////////////////////////////////////////////////////////////////////
// create a logfont with the given stuff
/////////////////////////////////////////////////////////////////////////////
const LOGFONT GetLogFont(int iHeight, CString szName, bool bBold, bool bItalic)
{
	char szFont[LF_FACESIZE];
	LOGFONT font;

	//copy the font name in
	strncpy(szFont, szName, LF_FACESIZE);

	font.lfHeight				= (LONG) iHeight;
	font.lfWidth				= (LONG) 0;
	font.lfEscapement			= (LONG) 0;
	font.lfOrientation			= (LONG) 0;
	font.lfWeight				= (LONG) (bBold ? FW_BOLD : FW_NORMAL);
	font.lfItalic				= (BYTE) (bItalic);
	font.lfUnderline			= (BYTE) FALSE;
	font.lfStrikeOut			= (BYTE) FALSE;
	font.lfCharSet				= (BYTE) ANSI_CHARSET;
	font.lfOutPrecision			= (BYTE) OUT_DEFAULT_PRECIS;
	font.lfClipPrecision		= (BYTE) CLIP_DEFAULT_PRECIS;
	font.lfQuality				= (BYTE) DEFAULT_QUALITY;
	font.lfPitchAndFamily		= (BYTE) (VARIABLE_PITCH|FF_DONTCARE);
	memcpy(&(font.lfFaceName), szFont, LF_FACESIZE);

	return font;
}

/////////////////////////////////////////////////////////////////////////////
// assignment operator for extended log font
/////////////////////////////////////////////////////////////////////////////
XLOGFONT &XLOGFONT::operator = (const XLOGFONT &lf)
{
	this->font.lfHeight			= (LONG) lf.font.lfHeight;
	this->font.lfWidth			= (LONG) lf.font.lfWidth;
	this->font.lfEscapement		= (LONG) lf.font.lfEscapement;
	this->font.lfOrientation	= (LONG) lf.font.lfOrientation;
	this->font.lfWeight			= (LONG) lf.font.lfWeight;
	this->font.lfItalic			= (BYTE) lf.font.lfItalic;
	this->font.lfUnderline		= (BYTE) lf.font.lfUnderline;
	this->font.lfStrikeOut		= (BYTE) lf.font.lfWidth;
	this->font.lfCharSet		= (BYTE) lf.font.lfCharSet;
	this->font.lfOutPrecision	= (BYTE) lf.font.lfOutPrecision;
	this->font.lfClipPrecision	= (BYTE) lf.font.lfClipPrecision;
	this->font.lfQuality		= (BYTE) lf.font.lfQuality;
	this->font.lfPitchAndFamily = (BYTE) lf.font.lfPitchAndFamily;
	memcpy(&(this->font.lfFaceName), &(lf.font.lfFaceName), LF_FACESIZE);

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// assignment operator for extended log font
/////////////////////////////////////////////////////////////////////////////
XLOGFONT &XLOGFONT::operator = (const LOGFONT &lf)
{
	this->font.lfHeight			= (LONG) lf.lfHeight;
	this->font.lfWidth			= (LONG) lf.lfWidth;
	this->font.lfEscapement		= (LONG) lf.lfEscapement;
	this->font.lfOrientation	= (LONG) lf.lfOrientation;
	this->font.lfWeight			= (LONG) lf.lfWeight;
	this->font.lfItalic			= (BYTE) lf.lfItalic;
	this->font.lfUnderline		= (BYTE) lf.lfUnderline;
	this->font.lfStrikeOut		= (BYTE) lf.lfWidth;
	this->font.lfCharSet		= (BYTE) lf.lfCharSet;
	this->font.lfOutPrecision	= (BYTE) lf.lfOutPrecision;
	this->font.lfClipPrecision	= (BYTE) lf.lfClipPrecision;
	this->font.lfQuality		= (BYTE) lf.lfQuality;
	this->font.lfPitchAndFamily = (BYTE) lf.lfPitchAndFamily;
	memcpy(&(this->font.lfFaceName), &(lf.lfFaceName), LF_FACESIZE);
	
	return *this;
}

//////////////////////////////////////////////////////////////////////
// overloaded comparison operator
//////////////////////////////////////////////////////////////////////
BOOL XLOGFONT::operator ==(const XLOGFONT &lf)
{

	if(font.lfHeight			!= lf.font.lfHeight) return FALSE;
	if(font.lfWidth				!= lf.font.lfWidth) return FALSE;
	if(font.lfEscapement		!= lf.font.lfEscapement) return FALSE;
	if(font.lfOrientation		!= lf.font.lfOrientation) return FALSE;
	if(font.lfWeight			!= lf.font.lfWeight) return FALSE;
	if(font.lfItalic			!= lf.font.lfItalic) return FALSE;
	if(font.lfUnderline			!= lf.font.lfUnderline) return FALSE;
	if(font.lfStrikeOut			!= lf.font.lfWidth) return FALSE;
	if(font.lfCharSet			!= lf.font.lfCharSet) return FALSE;
	if(font.lfOutPrecision		!= lf.font.lfOutPrecision) return FALSE;
	if(font.lfClipPrecision		!= lf.font.lfClipPrecision) return FALSE;
	if(font.lfQuality			!= lf.font.lfQuality) return FALSE;
	if(font.lfPitchAndFamily	!= lf.font.lfPitchAndFamily) return FALSE;
	if(strncmp(font.lfFaceName, lf.font.lfFaceName, LF_FACESIZE) != 0) return FALSE;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// overloaded operator to read in a log font
//////////////////////////////////////////////////////////////////////
CArchive &operator >> (CArchive &in, XLOGFONT &x)
{
	CString strIn;

	in >> x.font.lfHeight;
	in >> x.font.lfWidth;
	in >> x.font.lfEscapement;
	in >> x.font.lfOrientation;
	in >> x.font.lfWeight;
	in >> x.font.lfItalic;
	in >> x.font.lfUnderline;
	in >> x.font.lfWidth;
	in >> x.font.lfCharSet;
	in >> x.font.lfOutPrecision;
	in >> x.font.lfClipPrecision;
	in >> x.font.lfQuality;
	in >> x.font.lfPitchAndFamily;

	//read in the name as a CString
	in >> strIn;

	//save it to the face name
	strncpy(x.font.lfFaceName, strIn, LF_FACESIZE);

	return in;
}

//////////////////////////////////////////////////////////////////////
// overloaded operator to save a log font
//////////////////////////////////////////////////////////////////////
CArchive &operator << (CArchive &out, XLOGFONT &x)
{
	out << x.font.lfHeight;
	out << x.font.lfWidth;
	out << x.font.lfEscapement;
	out << x.font.lfOrientation;
	out << x.font.lfWeight;
	out << x.font.lfItalic;
	out << x.font.lfUnderline;
	out << x.font.lfWidth;
	out << x.font.lfCharSet;
	out << x.font.lfOutPrecision;
	out << x.font.lfClipPrecision;
	out << x.font.lfQuality;
	out << x.font.lfPitchAndFamily;

	//convert to a CString to save
	out << CString(x.font.lfFaceName);

	return out;
}