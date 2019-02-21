// WinFileTransferDlg.h : header file
//

#if !defined(AFX_WINFILETRANSFERDLG_H__8277F155_6A6A_43D6_A1D4_8B3E0F90CAFE__INCLUDED_)
#define AFX_WINFILETRANSFERDLG_H__8277F155_6A6A_43D6_A1D4_8B3E0F90CAFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWinFileTransferDlg dialog

class CWinFileTransferDlg : public CDialog
{
// Construction
public:
	CWinFileTransferDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWinFileTransferDlg)
	enum { IDD = IDD_WINFILETRANSFER_DIALOG };
	CString	m_PORT;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinFileTransferDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWinFileTransferDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClient();
	afx_msg void OnServer();
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit1();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINFILETRANSFERDLG_H__8277F155_6A6A_43D6_A1D4_8B3E0F90CAFE__INCLUDED_)
