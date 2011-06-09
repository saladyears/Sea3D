// MapInfoDialog.cpp : implementation file
//

#include "stdafx.h"
#include "settlers.h"
#include "MapInfoDialog.h"
#include "MapDatabase.h"
#include "SettlersView.h"
#include "defineColors.h"
#include "defineVariants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapInfoDialog dialog


CMapInfoDialog::CMapInfoDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CMapInfoDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMapInfoDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMapInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapInfoDialog)
	DDX_Control(pDX, IDC_INFO_EDIT, m_InfoEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapInfoDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CMapInfoDialog)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapInfoDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// setup
/////////////////////////////////////////////////////////////////////////////
BOOL CMapInfoDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();
	
	//help system
	m_strHelp = HELP_MAPINFO;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// hide the menu selection when closing the window
/////////////////////////////////////////////////////////////////////////////
void CMapInfoDialog::hideMenu()
{
	CMenu *pMenu;
	CMenu *pSubmenu;

	//get the current menu
	pMenu = AfxGetMainWnd()->GetMenu();

	//get the submenu
	pSubmenu = pMenu->GetSubMenu(3);

	//set the checked status
	pSubmenu->CheckMenuItem(3, MF_UNCHECKED | MF_BYPOSITION);
}

/////////////////////////////////////////////////////////////////////////////
// update main menu
/////////////////////////////////////////////////////////////////////////////
void CMapInfoDialog::OnCancel() 
{
	hideMenu();

	CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// update main menu
/////////////////////////////////////////////////////////////////////////////
void CMapInfoDialog::OnOK() 
{
	hideMenu();

	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// set the map information
/////////////////////////////////////////////////////////////////////////////
void CMapInfoDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CString str;

	//set map info
	if(TRUE == bShow)
	{
		m_InfoEdit.SetWindowText("");

		//set the team text
		if(TRUE == GAME->m_bTeam)
		{
			str = "This is a team game.\n\n";
		}
		//set the ladder text
		if(TRUE == GAME->m_bLadder)
		{
			//get rid of the extra newline
			str = str.Left(str.GetLength() - 1);

			//add the ladder text
			str += "This is a ladder game.\n\n";
		}

		//create info string
		str += MAPS->getMapInfo(MAP->m_uiID);

		//add variant string
		str += VARIANTS->getVariantString();

		//set the map string
		m_InfoEdit.addString(str, FALSE, COLOR_LTGRAY);
	}

	CHelpDialog::OnShowWindow(bShow, nStatus);
}
