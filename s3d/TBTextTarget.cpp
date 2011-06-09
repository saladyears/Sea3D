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

// TBTextTarget.cpp: Implementierung der Klasse TBTextTarget.
//
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

#include "stdafx.h"
#include "stdafx.h"
#include "TBTextTarget.h"

#include <afxadv.h> // for CSharedFile

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

TBTextTarget::TBTextTarget()
{
	tb_pDragFile=NULL;
	tb_RefCount=0;
}

TBTextTarget::~TBTextTarget()
{
	if(tb_pDragFile)
	{
		delete tb_pDragFile;
		tb_pDragFile = NULL;
	}
}

// Interface IUnknown (is base of IDropTarget)

// get a pointer to our interface
HRESULT TBTextTarget::QueryInterface(REFIID iid, void ** ppvObject)
{
	// Simplified version...
	// TEST_ NULL 
	if(ppvObject == NULL)
		return E_POINTER;  

	// Supports IID_IUnknown and IID_IDropTarget 
	if(iid == IID_IUnknown || 
	   iid == IID_IDropTarget)
	{
		*ppvObject = (void *) this;
		AddRef();
		return NOERROR;
	}

	return E_NOINTERFACE; 
}

ULONG TBTextTarget::AddRef(void)
{
	return ++tb_RefCount;
}

ULONG TBTextTarget::Release(void)
{
	--tb_RefCount;

	if (tb_RefCount==0 && tb_pDragFile)
	{
		delete tb_pDragFile;
		tb_pDragFile=NULL;
	}

	return tb_RefCount;
}

// Interface IDropTarget

// the drag action continues
HRESULT TBTextTarget::DragOver(
	DWORD /*grfKeyState*/, //Current state of keyboard modifier keys
	POINTL /*pt*/,         //Pointer to the current cursor coordinates
	DWORD * pdwEffect  //Pointer to the effect of the drag-and-drop 
					   // operation
	)
{
	// TODO: Add your own checking here due to the point or the key modifiers
	if (tb_pDragFile)
	{
		*pdwEffect = DROPEFFECT_MOVE;
	}
	else 
		*pdwEffect = DROPEFFECT_NONE;

	return dragOver();
}

// the drag action leaves your window - no dropping
HRESULT TBTextTarget::DragLeave()
{
	if (tb_pDragFile)
	{
		delete tb_pDragFile;
		tb_pDragFile=NULL;
	}

	return dragLeave();
}

// The drag action enters your window - get the file
HRESULT TBTextTarget::DragEnter
				( IDataObject * pDataObject, //Pointer to the interface of the source data object
				  DWORD /*grfKeyState*/,//Current state of keyboard modifier keys
				  POINTL /*pt*/,		//Pointer to the current cursor coordinates
				  DWORD * pdwEffect	//Pointer to the effect of the drag-and-drop 
									// operation
				)
{
	// compare this function with COleDropSource::GetFileData 
	// file  @\Microsoft Visual Studio\VC98\MFC\SRC\OLEDOBJ1.CPP line 119

	STGMEDIUM stgMedium;
	FORMATETC fmt = {CF_TEXT, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	HRESULT ret = pDataObject->GetData(&fmt, &stgMedium);//GetData(CF_TEXT, &stgMedium);
	if (ret != S_OK)
	{
		switch(ret)
		{
		case DV_E_LINDEX :
			TRACE("\nInvalid value for lindex; currently, only -1 is supported");
			break;
		case DV_E_FORMATETC:
			TRACE("\nInvalid value for pFormatetc.");
			break;
		case DV_E_TYMED:
			TRACE("\nInvalid tymed value.");
			break;
		case DV_E_DVASPECT:
			TRACE("\nInvalid dwAspect value.");
			break;
		case OLE_E_NOTRUNNING:
			TRACE("\nObject application is not running.");
			break;
		case STG_E_MEDIUMFULL:
			TRACE("\nAn error occurred when allocating the medium.");
			break;
		default:
			TRACE("\nThe drag'n'drop format does not match CF_TEXT or an error has occured");
			break;
		}
		return FALSE;
	}

	if (stgMedium.pUnkForRelease != NULL)
		return FALSE;

	// convert it to a file, depending on data
	CString strFileName;

	if (tb_pDragFile)
		delete tb_pDragFile;
	tb_pDragFile = NULL;

	TRY
	{
		switch (stgMedium.tymed)
		{
		case TYMED_FILE:
			strFileName = stgMedium.lpszFileName;
			tb_pDragFile = new CFile;
			if (!tb_pDragFile->Open(strFileName,
				CFile::modeReadWrite|CFile::shareExclusive))
			{
				delete tb_pDragFile;
				tb_pDragFile = NULL;
				break;
			}
			// caller is responsible for deleting the actual file,
			//  but we free the file name.
			CoTaskMemFree(stgMedium.lpszFileName);
			break;

		case TYMED_MFPICT:
		case TYMED_HGLOBAL:
			tb_pDragFile = new CSharedFile;
			((CSharedFile*)tb_pDragFile)->SetHandle(stgMedium.hGlobal);
			break;

		case TYMED_ISTREAM:
			tb_pDragFile = new COleStreamFile(stgMedium.pstm);
			break;

		default:
			// type not supported, so return error
			::ReleaseStgMedium(&stgMedium);
			break;
		}
	}
	CATCH_ALL(e)
	{
		delete tb_pDragFile;
		tb_pDragFile = NULL;
	}
	END_CATCH_ALL

	if (tb_pDragFile)
		*pdwEffect = DROPEFFECT_MOVE;
	else 
		*pdwEffect = DROPEFFECT_NONE;

	return dragEnter();
}

// the data have been dropped here, so process it
HRESULT TBTextTarget::Drop(
	IDataObject * /*pDataObject*/,	// Pointer to the interface for the source data
	DWORD /*grfKeyState*/,			// Current state of keyboard modifier keys
	POINTL /*pt*/,					// Pointer to the current cursor coordinates
	DWORD * /*pdwEffect*/			// Pointer to the effect of the drag-and-drop operation
	)
{
	drop();

	if (tb_pDragFile)
	{
		DWORD siz = (DWORD) tb_pDragFile->GetLength();

		// getbuffersetlength has int parameter!		
		if (siz < INT_MAX)
		{
			CString Data;
			
			LPTSTR buffer = Data.GetBufferSetLength((int)siz);

			tb_pDragFile->Read(buffer, (UINT)siz); // typesafe here because GetLength >=0
			
			Data.ReleaseBuffer();

			TRY
			{
				ProcessData(Data);
			}
			CATCH_ALL(e)
			{
#ifdef _DEBUG
				TCHAR   szCause[255];
				CString strFormatted;
				e->GetErrorMessage(szCause, 255);
				strFormatted = _T("Exception: ");
				strFormatted += szCause;
				AfxMessageBox(strFormatted);   
#endif //_DEBUG
				ASSERT(FALSE);
			}
			END_CATCH_ALL;
		}

		delete tb_pDragFile;
		tb_pDragFile = NULL;
		return S_OK;
	}
	return E_INVALIDARG;
}
