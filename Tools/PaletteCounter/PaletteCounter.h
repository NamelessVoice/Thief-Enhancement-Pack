// PaletteCounter.h : main header file for the PALETTECOUNTER application
//

#if !defined(AFX_PALETTECOUNTER_H__C6C26114_4F1C_4511_B32F_EBC69A21C862__INCLUDED_)
#define AFX_PALETTECOUNTER_H__C6C26114_4F1C_4511_B32F_EBC69A21C862__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPaletteCounterApp:
// See PaletteCounter.cpp for the implementation of this class
//

class CPaletteCounterApp : public CWinApp
{
public:
	CPaletteCounterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaletteCounterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPaletteCounterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALETTECOUNTER_H__C6C26114_4F1C_4511_B32F_EBC69A21C862__INCLUDED_)
