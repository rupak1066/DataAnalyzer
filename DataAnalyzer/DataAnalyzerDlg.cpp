
// DataAnalyzerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "DataAnalyzer.h"
#include "DataAnalyzerDlg.h"
#include "afxdialogex.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDataAnalyzerDlg dialog



CDataAnalyzerDlg::CDataAnalyzerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DATAANALYZER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataAnalyzerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_LIST, m_list);
}

BEGIN_MESSAGE_MAP(CDataAnalyzerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDataAnalyzerDlg::OnSelchangeTab)
	ON_BN_CLICKED(IDC_LOAD_BUTTON, &CDataAnalyzerDlg::OnClickedLoadButton)
END_MESSAGE_MAP()


// CDataAnalyzerDlg message handlers

BOOL CDataAnalyzerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	//list controlls initialization
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//tab initialization
	m_tab.InsertItem(0, _T("Tab-1"));
	m_tab.InsertItem(1, _T("Tab-2"));

	// Create Graph dialog
	if (!test_dlg.Create(IDD_A, &m_tab))
	{

		DWORD error = GetLastError();

		CString message;
		message.Format(L"Failed to create Graph dialog.\nError code: %lu", error);

		AfxMessageBox(message);

		return FALSE;
	}

	// Get usable area inside tab control
	CRect rect;
	m_tab.GetClientRect(&rect);

	// Adjust rect for the tab headers
	m_tab.AdjustRect(FALSE, &rect);

	// Position Graph dialog inside the tab
	test_dlg.SetWindowPos(
		nullptr,
		rect.left,
		rect.top,
		rect.Width(),
		rect.Height(),
		SWP_NOZORDER
	);

	// Show it
	test_dlg.ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDataAnalyzerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDataAnalyzerDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDataAnalyzerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDataAnalyzerDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	int sel = m_tab.GetCurSel();
	switch (sel)
	{
	case 0:
		test_dlg.ShowWindow(SW_SHOW);
		break;
	case 1:
		test_dlg.ShowWindow(SW_HIDE);
		break;

	}
	*pResult = 0;
}
void CDataAnalyzerDlg::update_tree()
{
	//Update tree after new document is loaded
	m_tree.DeleteAllItems();
	HTREEITEM hRoot = m_tree.InsertItem(_T("Used Variables"), TVI_ROOT, TVI_LAST);
	
	auto headers = m_doc.getHeaders();
	for (const CString& str : headers)
	{
		m_tree.InsertItem(str, hRoot, TVI_LAST);
	}

	// 4. Expand the root node to show the new items
	m_tree.Expand(hRoot, TVE_EXPAND);
	
}
void CDataAnalyzerDlg::update_list()
{
	//hardcoded for now
	auto fields = m_doc.getHeaders();
	auto rows = m_doc.get_rows();

	
	int numColumns = fields.size();

	// Clear any existing columns/items first
	m_list.DeleteAllItems();
	while (m_list.DeleteColumn(0));
	for (int i = 0; i < numColumns; i++)
	{
		// InsertColumn(columnIndex, text, alignment, width)
		m_list.InsertColumn(i, fields[i], LVCFMT_LEFT, 100);
	}

	//insert the data
	for (int r =0;r<rows.size();r++)
	{
		CString str;
		str.Format(_T("%.2f"), rows[r][0]);
		m_list.InsertItem(r, str);
		for (int i = 1; i < rows[r].size(); i++)
		{
			str.Format(_T("%.2f"), rows[r][i]);
			m_list.SetItemText(r, i, str);
		}
	}

}


void CDataAnalyzerDlg::OnClickedLoadButton()
{
	// TODO: Add your control notification handler code here
	//get the file name from here and then need to call doc.load
	// 1. Define the file filter string
	LPCTSTR pszFilter = _T("Csv Files (*.csv)|*.csv|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");

	// 2. Initialize the dialog (TRUE = Open dialog, FALSE = Save As dialog)
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, pszFilter, this);

	// 3. Display the dialog and check if the user clicked "OK"
	if (fileDlg.DoModal() == IDOK)
	{
		

		CString filePath = fileDlg.GetPathName(); // Full path: "C:\Folder\file.txt"
		//first clear the m_doc
		m_doc.clear();
		test_dlg.clear_graph();
		// need to call document class's load here
		if(m_doc.load(filePath))
		{
			update_list();
			update_tree();
			test_dlg.update_components(&m_doc);
		}
		else
		{
			AfxMessageBox(_T("Failed to open file: ") + filePath);
		}
		
	}


}
