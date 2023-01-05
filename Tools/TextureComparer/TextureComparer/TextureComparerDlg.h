// m_oglWindowDlg.h : header file
//

#pragma once
#include "OpenGLControl.h"
#include "afxwin.h"
#include "afxcmn.h"


// CTextureComparerDlg dialog
class CTextureComparerDlg : public CDialog
{
// Construction
public:
	CTextureComparerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TEXTURECOMPARER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	COpenGLControl m_oglWindow;

	string m_sBaseOldPath;
	string m_sBaseNewPath;

public:
	BOOL CTextureComparerDlg::PreTranslateMessage(MSG* pMsg);

	void addDirToTree(CString strPath, HTREEITEM treeParent);
	bool CTextureComparerDlg::isNumberPowerOfTwo(int number);

	void initTree();

	afx_msg void OnStnClickedOpengl();
	CTreeCtrl m_cTreeCtrl;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
	CEdit m_cStatusBox;
};
