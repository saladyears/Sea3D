// TexCreateDlg.h : header file
//

#if !defined(AFX_TEXCREATEDLG_H__27839DAF_907B_49D8_9F1F_1302AE34A188__INCLUDED_)
#define AFX_TEXCREATEDLG_H__27839DAF_907B_49D8_9F1F_1302AE34A188__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTexCreateDlg dialog

class CTexCreateDlg : public CDialog
{
// Construction
public:
	CTexCreateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTexCreateDlg)
	enum { IDD = IDD_TEXCREATE_DIALOG };
	CEdit	m_TextEdit;
	CEdit	m_TexEdit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTexCreateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strTexName;
	CString m_strTextDir;

	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTexCreateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTexBrowseButton();
	afx_msg void OnTextBrowseButton();
	afx_msg void OnCreateTexButton();
	afx_msg void OnCreateTextButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXCREATEDLG_H__27839DAF_907B_49D8_9F1F_1302AE34A188__INCLUDED_)
