// TexCreateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TexCreate.h"
#include "TexCreateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTexCreateDlg dialog

CTexCreateDlg::CTexCreateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTexCreateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTexCreateDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTexCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTexCreateDlg)
	DDX_Control(pDX, IDC_TEXT_EDIT, m_TextEdit);
	DDX_Control(pDX, IDC_TEX_EDIT, m_TexEdit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTexCreateDlg, CDialog)
	//{{AFX_MSG_MAP(CTexCreateDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TEX_BROWSE_BUTTON, OnTexBrowseButton)
	ON_BN_CLICKED(IDC_TEXT_BROWSE_BUTTON, OnTextBrowseButton)
	ON_BN_CLICKED(IDC_CREATE_TEX_BUTTON, OnCreateTexButton)
	ON_BN_CLICKED(IDC_CREATE_TEXT_BUTTON, OnCreateTextButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTexCreateDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// init
/////////////////////////////////////////////////////////////////////////////
BOOL CTexCreateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
/////////////////////////////////////////////////////////////////////////////
void CTexCreateDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
/////////////////////////////////////////////////////////////////////////////
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
/////////////////////////////////////////////////////////////////////////////
HCURSOR CTexCreateDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////////////////////////////////////////////////////////////////////////
// select the TEX file
/////////////////////////////////////////////////////////////////////////////
void CTexCreateDlg::OnTexBrowseButton() 
{
	static char BASED_CODE szFilter[] = "TEX files (*.tex)|*.tex|All Files (*.*)|*.*||";

	CFileDialog dlg(TRUE, ".tex", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

	//run it
	if(IDOK == dlg.DoModal())
	{
		//set the edit text
		m_TexEdit.SetWindowText(dlg.GetPathName());

		//set the tex name
		m_strTexName = dlg.GetFileName();
	}
}

/////////////////////////////////////////////////////////////////////////////
// select the text file
/////////////////////////////////////////////////////////////////////////////
void CTexCreateDlg::OnTextBrowseButton() 
{
	int iSlash;
	CString strPath;
	static char BASED_CODE szFilter[] = "Text files (*.txt)|*.txt|All Files (*.*)|*.*||";

	CFileDialog dlg(TRUE, ".txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

	//run it
	if(IDOK == dlg.DoModal())
	{
		//get the path
		strPath = dlg.GetPathName();
		
		//set the edit text
		m_TextEdit.SetWindowText(strPath);

		//find the backslash
		iSlash = strPath.ReverseFind('\\');

		//set the directory
		m_strTextDir = strPath.Left(iSlash + 1);
	}
}

/////////////////////////////////////////////////////////////////////////////
// create a TEX file from the given text file
/////////////////////////////////////////////////////////////////////////////
void CTexCreateDlg::OnCreateTexButton() 
{
	int iSlash;
	int iCount = 0;
	char *read = NULL;
	char *buf = NULL;
	CFile tex;
	FILE *text;
	CFile image;
	DWORD dwSize;
	DWORD dwTotal = 0;
	DWORD dwLength;
	CString strTex;
	CString strText;
	CString strFile;
	CString strPath;

	//get the paths
	m_TexEdit.GetWindowText(strTex);
	m_TextEdit.GetWindowText(strText);

	//open the files
	text = fopen(strText, "r");
	if(NULL == text)
	{
		AfxMessageBox("Could not open " + strText + " file for reading.");
	}

	if(FALSE == tex.Open(strTex, CFile::modeCreate | CFile::modeWrite))
	{
		AfxMessageBox("Could not open " + strTex + " file for writing.");
	}

	//now attach the archives
	CArchive arTex(&tex, CArchive::store);

	//get the size of the text file
	fseek(text, 0, SEEK_END);
	dwLength = ftell(text);
	fseek(text, 0, SEEK_SET);

	while(dwTotal < dwLength)
	{
		//read in the file name
		fscanf(text, "%s", strFile.GetBuffer(1000));
		strFile.ReleaseBuffer();

		//add to the length and see if we're done
		dwTotal += strlen(strFile.GetBuffer(1000)) + 1;

		//create the output file
		if(image.Open(strFile, CFile::modeRead))
		{
			//get the size
			dwSize = image.GetLength();

			//new buffer
			buf = new char[dwSize];

			//read the data and close the file
			image.Read(buf, dwSize);
			image.Close();

			//get just the file name
			iSlash = strFile.ReverseFind('\\');
			strFile = strFile.Right(strFile.GetLength() - iSlash - 1);

			//write to the tex file
			arTex << strFile;
			arTex << dwSize;
			arTex.Write(buf, dwSize);

			//clear the buffer
			delete buf;
			buf = NULL;
		}
		
		//increment the counter
		iCount++;
	}

	//remark how much got done
	strText.Format("%d images were written to %s", iCount, strTex);
	AfxMessageBox(strText);

	//clear the edit boxes
	m_TexEdit.SetWindowText("");
	m_TextEdit.SetWindowText("");

	//close the archives
	arTex.Close();

	//close the files
	tex.Close();
	fclose(text);
}

/////////////////////////////////////////////////////////////////////////////
// create a text file and directory of images from the given TEX file
/////////////////////////////////////////////////////////////////////////////
void CTexCreateDlg::OnCreateTextButton() 
{
	int iCount = 0;
	char *buf = NULL;
	CFile tex;
	CFile text;
	CFile image;
	DWORD dwSize;
	DWORD dwTotal = 0;
	DWORD dwLength;
	CString strTex;
	CString strText;
	CString strFile;
	CString strPath;
	CString strDir;

	//get the paths
	m_TexEdit.GetWindowText(strTex);
	m_TextEdit.GetWindowText(strText);

	//open the files
	if(FALSE == tex.Open(strTex, CFile::modeRead))
	{
		AfxMessageBox("Could not open " + strTex + " file for reading.");
	}

	if(FALSE == text.Open(strText, CFile::modeCreate | CFile::modeWrite))
	{
		AfxMessageBox("Could not open " + strText + " file for writing.");
	}

	//now attach the archives
	CArchive arTex(&tex, CArchive::load);
	CArchive arText(&text, CArchive::store);

	//create the directory path
	strDir = m_strTextDir + m_strTexName;

	//axe the file extension
	strDir = strDir.Left(strDir.GetLength() - 4);

	//create the directory
	CreateDirectory(strDir, NULL);
	strDir += "\\";

	//get the size of the TEX file
	dwLength = tex.GetLength();

	while(dwTotal < dwLength)
	{
		//read in the file name and size
		arTex >> strFile;
		arTex >> dwSize;

		//new buffer
		buf = new char[dwSize];

		//read the data
		arTex.Read(buf, dwSize);

		//create the output file
		strPath = strDir + '\\' + strFile;
		if(image.Open(strPath, CFile::modeCreate | CFile::modeWrite))
		{
			//dump the data and close the file
			image.Write(buf, dwSize);
			image.Close();

			//write to the name file
			strPath += "\n";
			arText.Write(strPath.GetBuffer(strPath.GetLength()), strPath.GetLength());
		}

		//clear the buffer
		delete buf;
		buf = NULL;

		//add to the length and see if we're done
		dwTotal += strlen(strFile.GetBuffer(1000)) + 1;
		dwTotal += sizeof(DWORD);
		dwTotal += dwSize;

		//increment the counter
		iCount++;
	}

	//remark how much got done
	strText.Format("%d images were written to %s", iCount, strDir);
	AfxMessageBox(strText);

	//clear the edit boxes
	m_TexEdit.SetWindowText("");
	m_TextEdit.SetWindowText("");

	//close the archives
	arTex.Close();
	arText.Close();

	//close the files
	tex.Close();
	text.Close();
}
