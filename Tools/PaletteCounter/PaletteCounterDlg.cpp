// PaletteCounterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PaletteCounter.h"


//*****************
#include "gif.h"
#include "fstream.h"
#include "math.h"
#include <vector>
#include <algorithm>
//*****************

#include "PaletteCounterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaletteCounterDlg dialog

CPaletteCounterDlg::CPaletteCounterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteCounterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPaletteCounterDlg)
	m_strFile = _T("");
	m_strFileInfo = _T("");
	m_strPalCount = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPaletteCounterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaletteCounterDlg)
	DDX_Control(pDX, IDC_PALETTE_LIST, m_lcPalettes);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strFile);
	DDX_Text(pDX, IDC_EDIT_FILEINFO, m_strFileInfo);
	DDX_Text(pDX, IDC_EDIT_PAL_COUNT, m_strPalCount);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPaletteCounterDlg, CDialog)
	//{{AFX_MSG_MAP(CPaletteCounterDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaletteCounterDlg message handlers

BOOL CPaletteCounterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

/*
	m_lcColours.InsertColumn(0, "Colour", LVCFMT_LEFT, 50);
	m_lcColours.InsertColumn(1, "Red", LVCFMT_LEFT, 50);
	m_lcColours.InsertColumn(2, "Green", LVCFMT_LEFT, 50);
	m_lcColours.InsertColumn(3, "Blue", LVCFMT_LEFT, 50);

	m_lcColours.SetExtendedStyle(m_lcColours.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
*/
	m_lcPalettes.InsertColumn(0, "Count", LVCFMT_LEFT, 40);
	m_lcPalettes.InsertColumn(1, "Files", LVCFMT_LEFT, 1000);
	m_lcPalettes.SetExtendedStyle(m_lcPalettes.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

//	m_strFile = "C:\\Games\\DromEd\\Obj\\Original\\Txt16";
	UpdateData(false);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPaletteCounterDlg::OnPaint() 
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPaletteCounterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPaletteCounterDlg::OnButton1() 
{
	UpdateData(true);

	OPENFILENAME ofnInfo;
	char szFile[260];       // buffer for file name
	szFile[0] = '\0';

	ZeroMemory(&ofnInfo, sizeof(ofnInfo));
	ofnInfo.lStructSize = sizeof(ofnInfo);
	ofnInfo.hwndOwner = this->m_hWnd;
	ofnInfo.lpstrFile = szFile;
	ofnInfo.nMaxFile = sizeof(szFile);
	ofnInfo.Flags = OFN_HIDEREADONLY;
	ofnInfo.lpstrFile[0] = '\0';
	ofnInfo.lpstrDefExt = ".gif";
	ofnInfo.lpstrTitle = "Choose a folder containing .gif files.";
	ofnInfo.lpstrFilter = "GIF images (*.gif)\0*.gif";
	ofnInfo.nFilterIndex = 1;
	

	if ( GetOpenFileName(&ofnInfo) != IDOK )
		return;

	CString xstr = ofnInfo.lpstrFile;
	for (int i = xstr.GetLength()-1; i >= 0; i-- )
	{
		if ( xstr[i] == '\\' )
		{
			xstr = xstr.Left(i);
			break;
		}
	}

	m_strFile = xstr;
	UpdateData(false);

	// Done with getting path
	
	m_lcPalettes.DeleteAllItems();
	std::vector<ColourTable> Palettes;
	std::vector<ColourTable>::iterator pPal;

	// TODO: Add your control notification handler code here
	GIFHeader Header;
	fstream fGif;


	CString str;

	CString strFilename = m_strFile;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	CString strPath = m_strFile;//


	hFind = FindFirstFile(strPath + "\\*.gif", &FindFileData);

	if ( hFind == INVALID_HANDLE_VALUE )
	{
		MessageBox("No valid files found.", "Warning", MB_ICONEXCLAMATION);
		return;
	}

	do
	{
		bool bOk = true;
		strFilename = FindFileData.cFileName;

		
		fGif.clear();
		fGif.open(strPath + "\\" + strFilename, ios::binary | ios::in | ios::out | ios::nocreate);
		
		fGif.seekg(0L);

		if ( fGif.fail() )
		{
			CString strError = "a";
			MessageBox("Failed to open file \"" + strPath + "\\" + strFilename + "\".", "Warning", MB_ICONEXCLAMATION);
			bOk = false;
		}

		m_strFileInfo.Format("Reading %s...", strFilename);
		UpdateData(false);
		UpdateWindow();

		fGif.read((char *) &Header, sizeof(GIFHeader));

		if ( strncmp(Header.Signature, "GIF89a", 6) )
		{
			if ( strncmp(Header.Signature, "GIF87a", 6) )
			{
				MessageBox("Not a valid GIF file \"" + strPath + "\\" + strFilename + "\".", "Warning", MB_ICONEXCLAMATION);
				bOk = false;
			}
		}
		
		if (bOk)
			{

			int iX = Header.Width;
			int iY = Header.Height;
			unsigned int iNumberOfColours = static_cast<unsigned int>(pow(2, ((Header.ColourInfo & 7) +1)));
			
			Header.Signature[6] = '\0';
			m_strFileInfo.Format("Read %s: %s: %ix%i, %i colours", strFilename, Header.Signature, iX, iY, iNumberOfColours);
		//	MessageBox(str);

			ColourTable Palette;

			fGif.read((char *) &Palette, iNumberOfColours*3);
			Palette.strFiles = strFilename;

			/*
			m_lcColours.DeleteAllItems();
			for ( int i = 0; i < 256; i++ )
			{
				str.Format("%i", i);
				int iCol = m_lcColours.InsertItem(m_lcColours.GetItemCount(),str);

				str.Format("%i", Palette.Colour[i][0]);
				m_lcColours.SetItemText(iCol, 1, str);

				str.Format("%i", Palette.Colour[i][1]);
				m_lcColours.SetItemText(iCol, 2, str);

				str.Format("%i", Palette.Colour[i][2]);
				m_lcColours.SetItemText(iCol, 3, str);

			}
			*/

			for (pPal = Palettes.begin(); pPal != Palettes.end(); pPal++ )
			{
				if (*pPal == Palette )
				{
					pPal->strFiles += ", " + strFilename;
					pPal->iCount++;
					break;
				}
			}

			if ( pPal == Palettes.end() )
				Palettes.push_back(Palette);


		} // if bOk

		fGif.close();

		m_strPalCount.Format("%i unique palettes found,", Palettes.size());
		UpdateData(false);
		UpdateWindow();
	} while (FindNextFile (hFind, &FindFileData) != 0);

	std::vector<ColourTable>::iterator j, k;
	ColourTable temp;
	
	for ( j = Palettes.begin(); j != Palettes.end(); j++ )
	{
		for ( k = Palettes.begin(); k != Palettes.end(); k++ )
		{
			if ( *j < *k )
			{
				temp = *j;
				*j = *k;
				*k = temp;
			}
		}
	}

	OPENFILENAME ofnSave;

	ZeroMemory(&ofnSave, sizeof(ofnSave));
	strcpy(szFile, "Palette Counter Report.csv");
	ofnSave.lStructSize = sizeof(ofnSave);
	ofnSave.hwndOwner = this->m_hWnd;
	ofnSave.lpstrFile = szFile;
	ofnSave.nMaxFile = sizeof(szFile);
	ofnSave.Flags = OFN_HIDEREADONLY;
	ofnSave.lpstrDefExt = ".csv";
	ofnSave.lpstrTitle = "Save report...";
	ofnSave.lpstrFilter = "CSV files (*.csv)\0*.csv";
	ofnSave.nFilterIndex = 1;

	bool bReport = false;
	

	if ( GetSaveFileName(&ofnSave) == IDOK )
		bReport = true;

	CString strOut = ofnSave.lpstrFile;
//	MessageBox(strOut);

	ofstream ofReport;

	if ( bReport)
		ofReport.open(strOut, ios::out);

	for (pPal = Palettes.begin(); pPal != Palettes.end(); pPal++ )
	{
		str.Format("%i", pPal->iCount);
		int iItem = m_lcPalettes.InsertItem(m_lcPalettes.GetItemCount(), str);
		m_lcPalettes.SetItemText(iItem, 1, pPal->strFiles);
		
		if ( bReport)
			ofReport << pPal->iCount << ", " << pPal->strFiles << endl;
	}

	if ( bReport)
		ofReport.close();


	UpdateData(false);

}
