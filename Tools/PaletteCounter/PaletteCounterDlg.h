// PaletteCounterDlg.h : header file
//

#if !defined(AFX_PALETTECOUNTERDLG_H__E846D435_F593_4966_B555_0A5D9411B4B8__INCLUDED_)
#define AFX_PALETTECOUNTERDLG_H__E846D435_F593_4966_B555_0A5D9411B4B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPaletteCounterDlg dialog

class CPaletteCounterDlg : public CDialog
{
// Construction
public:
	CPaletteCounterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPaletteCounterDlg)
	enum { IDD = IDD_PALETTECOUNTER_DIALOG };
	CListCtrl	m_lcPalettes;
	CString	m_strFile;
	CString	m_strFileInfo;
	CString	m_strPalCount;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaletteCounterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPaletteCounterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALETTECOUNTERDLG_H__E846D435_F593_4966_B555_0A5D9411B4B8__INCLUDED_)
