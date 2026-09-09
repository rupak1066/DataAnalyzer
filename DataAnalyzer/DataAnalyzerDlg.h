
// DataAnalyzerDlg.h : header file
//

#pragma once
#include "doc.h"
#include "A.h"

// CDataAnalyzerDlg dialog
class CDataAnalyzerDlg : public CDialogEx
{
// Construction
public:
	CDataAnalyzerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATAANALYZER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// Tab controll variable
	CTabCtrl m_tab;
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	// variable for the tree ctrl
	CTreeCtrl m_tree;
	void update_tree();
	//list ctrl variable
	CListCtrl m_list;
	void update_list();
	//load button
	afx_msg void OnClickedLoadButton();
	//doc 
	Doc m_doc;
	A test_dlg;
};
