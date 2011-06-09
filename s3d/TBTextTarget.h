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

// TBTextTarget.h: Schnittstelle für die Klasse TBTabTextTarget.
//
// Notice: 
// This code belongs to Thomas B. Th. Blenkers, Germany
// 
// You may use and modify this software, as long as you agree to the 
// statements below
// - my name is not to be removed 
// - this disclaimer is not to be removed 
// - the source is provides "as is", without warranty of any kind
// - I do not warrant this code is free of bugs or does even work
//   in any special case. Nor shall I be responsible for anything 
//   at all that resulted in using this code.  Really. Nothing.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TBTEXTTARGET_H__A8EE7808_A8A0_11D2_BE7E_0020187099A1__INCLUDED_)
#define AFX_TBTEXTTARGET_H__A8EE7808_A8A0_11D2_BE7E_0020187099A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TBTextTarget : public IDropTarget  
{
public:
	TBTextTarget();
	virtual ~TBTextTarget();

	// Interface IDropTarget
	HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	HRESULT STDMETHODCALLTYPE DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);
	HRESULT STDMETHODCALLTYPE DragLeave(void);
	HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD __RPC_FAR *pdwEffect);

	// pure virtual function HAS to be overwritten in derived class
	// (at least you like to do something with the dropped text?)
	virtual void ProcessData(CString Data)=0;
	virtual HRESULT dragOver(void) = 0;
	virtual HRESULT dragLeave(void) = 0;
	virtual HRESULT dragEnter(void) = 0;
	virtual void drop(void) = 0;

	// Interface IUnknown	
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);
	ULONG tb_RefCount;
	CFile *tb_pDragFile;
};

#endif // !defined(AFX_TBTEXTTARGET_H__A8EE7808_A8A0_11D2_BE7E_0020187099A1__INCLUDED_)
