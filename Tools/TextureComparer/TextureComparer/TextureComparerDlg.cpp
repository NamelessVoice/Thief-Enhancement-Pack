// TextureComparerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TextureComparer.h"
#include "TextureComparerDlg.h"
#include <math.h>

//const char* baseNewPath = "C:\\Desktop\\Projects\\EP\\New EP\\ep\\fam";
//const char* baseOldPath = "C:\\Desktop\\test pals";

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextureComparerDlg dialog




CTextureComparerDlg::CTextureComparerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextureComparerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BOOL CTextureComparerDlg::PreTranslateMessage(MSG* pMsg) 
{
  // Cancel enter / esc to close dialog box.
  if(pMsg->message==WM_KEYDOWN)
  {
      if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)
          pMsg->wParam=NULL ;
	  else if ( pMsg->wParam==VK_F5 )
		initTree();
  } 

  return CDialog::PreTranslateMessage(pMsg);
}

void CTextureComparerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_cTreeCtrl);
	DDX_Control(pDX, IDC_EDIT1, m_cStatusBox);
}

BEGIN_MESSAGE_MAP(CTextureComparerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_OPENGL, &CTextureComparerDlg::OnStnClickedOpengl)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CTextureComparerDlg::OnTvnSelchangedTree1)
	ON_EN_CHANGE(IDC_EDIT1, &CTextureComparerDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CTextureComparerDlg message handlers

BOOL CTextureComparerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here


	CRect rect;

	// Get size and position of the picture control
	GetDlgItem(IDC_OPENGL)->GetWindowRect(rect);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	m_oglWindow.oglCreate(rect, this);

	// Setup the OpenGL Window's timer to render
	m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 1, 0);


	initTree();

	UpdateData(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTextureComparerDlg::initTree()
{
	m_cTreeCtrl.DeleteAllItems();
	char currentDir[256];

	GetCurrentDirectory(256, currentDir);

	string iniFileName = string(currentDir) + "\\TextureComparer.ini";
	char INIValue[256];
	GetPrivateProfileString("Paths", "NewTexturePath", "Default", INIValue, 256, iniFileName.c_str());
	m_sBaseNewPath = string(INIValue);

	GetPrivateProfileString("Paths", "OldTexturePath", "Default", INIValue, 256, iniFileName.c_str());
	m_sBaseOldPath = string(INIValue);

	CString path = m_sBaseNewPath.c_str();

	HTREEITEM parent = m_cTreeCtrl.InsertItem(path);
	addDirToTree(path, parent);
	m_cTreeCtrl.Expand(parent, TVE_EXPAND);
}

void CTextureComparerDlg::addDirToTree(CString strPath, HTREEITEM treeParent)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	
	CString strFilename;
	hFind = FindFirstFile(strPath + "\\*", &FindFileData);

	do
	{
		strFilename = FindFileData.cFileName;

		if ( strFilename != "." && strFilename != ".." )
		{
			if ( strFilename.Find(".mtl") == -1 )
			{
				HTREEITEM addedItem = m_cTreeCtrl.InsertItem(strFilename, treeParent);

//				m_cTreeCtrl.SetItemData(addedItem, strPath + "\\" + strFilename);

				if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					addDirToTree(strPath + "\\" + strFilename, addedItem);
					m_cTreeCtrl.Expand(addedItem, TVE_EXPAND);
				} else {
					string* pathString = new string(strPath + "\\" + strFilename);
					m_cTreeCtrl.SetItemData(addedItem, (DWORD_PTR)pathString);
				}
			}
		}
	} while (FindNextFile (hFind, &FindFileData) != 0);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTextureComparerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTextureComparerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//void CTextureComparerDlg::OnBnClickedButtonCalculate()
//{
//	UpdateData(true);

//	CString str;
//	str.Format("Found solution in %i steps.\nMinimum cost is %lf, at radius %lf and height %lf", iSteps, dResult, dResultX, dResultY);
//	MessageBox(str);
//	UpdateData(false);
//}

void CTextureComparerDlg::OnStnClickedOpengl()
{
	// TODO: Add your control notification handler code here
}

void CTextureComparerDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(false);

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	HTREEITEM selected = m_cTreeCtrl.GetSelectedItem();

	if ( m_cTreeCtrl.ItemHasChildren(selected) )
	{
		// Skip dirs
		m_oglWindow.m_bEnabled = false;
		return;
	} else {
		m_oglWindow.m_bEnabled = true;
	}

	CString fileName = m_cTreeCtrl.GetItemText(selected);

	selected = m_cTreeCtrl.GetParentItem(selected);
	while ( m_cTreeCtrl.GetParentItem(selected) != NULL )
	{
		fileName = m_cTreeCtrl.GetItemText(selected) + "\\" + fileName;

		selected = m_cTreeCtrl.GetParentItem(selected);
	}

	string s(fileName);
	size_t pos = s.find_last_of('.');
	s = string(m_sBaseOldPath.c_str()) + "\\" + s.substr(0, pos) + ".*";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(s.c_str(), &FindFileData);

	pos = s.find_last_of('\\');
	s = s.substr(0, pos) + "\\" + FindFileData.cFileName;

	m_oglWindow.SetOriginalTexture(s.c_str());

	CString newFileName = CString(m_sBaseNewPath.c_str()) + "\\" + fileName;
	m_oglWindow.SetNewTexture(newFileName);

	CString str;
	if ( !isNumberPowerOfTwo(m_oglWindow.m_iOldX) || !isNumberPowerOfTwo(m_oglWindow.m_iOldY) || !isNumberPowerOfTwo(m_oglWindow.m_iNewX) || !isNumberPowerOfTwo(m_oglWindow.m_iNewY) || !isNumberPowerOfTwo(m_oglWindow.m_iNewRealX) || !isNumberPowerOfTwo(m_oglWindow.m_iNewRealY) )
	{
		str.Format("Non power-of-two texture dimensions! Old (%ix%i), new (%ix%i), real (%ix%i)",  m_oglWindow.m_iOldX,  m_oglWindow.m_iOldY,  m_oglWindow.m_iNewX,  m_oglWindow.m_iNewY, m_oglWindow.m_iNewRealX,  m_oglWindow.m_iNewRealY);
		m_cStatusBox.SetWindowTextA(str);
	}
	else if ( m_oglWindow.m_iOldX != m_oglWindow.m_iNewX || m_oglWindow.m_iOldY != m_oglWindow.m_iNewY )
	{
		str.Format("Dimensions do not match! Old (%ix%i), new (%ix%i)",  m_oglWindow.m_iOldX,  m_oglWindow.m_iOldY,  m_oglWindow.m_iNewX,  m_oglWindow.m_iNewY);
		m_cStatusBox.SetWindowTextA(str);
	}
	else
	{
		str.Format("(%ix%i)",  m_oglWindow.m_iNewX,  m_oglWindow.m_iNewY);
		m_cStatusBox.SetWindowTextA(str);
	}
	
	UpdateData(true);
}

bool CTextureComparerDlg::isNumberPowerOfTwo(int number)
{
	// Thank you Internet
	return (number & (number - 1)) == 0;
}


void CTextureComparerDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
