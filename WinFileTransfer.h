// WinFileTransfer.h : main header file for the WINFILETRANSFER application
//

#if !defined(AFX_WINFILETRANSFER_H__982BEBC3_D8C9_45B1_BD04_03F02F2B98E5__INCLUDED_)
#define AFX_WINFILETRANSFER_H__982BEBC3_D8C9_45B1_BD04_03F02F2B98E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinFileTransferApp:
// See WinFileTransfer.cpp for the implementation of this class
//

class CWinFileTransferApp : public CWinApp
{
public:
	CWinFileTransferApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinFileTransferApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWinFileTransferApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINFILETRANSFER_H__982BEBC3_D8C9_45B1_BD04_03F02F2B98E5__INCLUDED_)
