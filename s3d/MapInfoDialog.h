#if !defined(AFX_MAPINFODIALOG_H__F6E70E38_4C32_433A_9FE2_57C4B6938C48__INCLUDED_)
#define AFX_MAPINFODIALOG_H__F6E70E38_4C32_433A_9FE2_57C4B6938C48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapInfoDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HelpDialog.h"
#include "ChatControl.h"

/////////////////////////////////////////////////////////////////////////////
// CMapInfoDialog dialog

class CMapInfoDialog : public CHelpDialog
{
// Construction
public:
	CMapInfoDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMapInfoDialog)
	enum { IDD = IDD_MAP_INFO_DIALOG };
	CChatControl	m_InfoEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapInfoDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void hideMenu(void);

	// Generated message map functions
	//{{AFX_MSG(CMapInfoDialog)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPINFODIALOG_H__F6E70E38_4C32_433A_9FE2_57C4B6938C48__INCLUDED_)
