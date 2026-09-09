// A.cpp : implementation file
//

#include "pch.h"
#include "DataAnalyzer.h"
#include "A.h"
#include "afxdialogex.h"


// A dialog

IMPLEMENT_DYNAMIC(A, CDialogEx)

A::A(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_A, pParent)
{

}

A::~A()
{
}

void A::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM2, m_graph);
	DDX_Control(pDX, IDC_G_X, m_combo_x);
	DDX_Control(pDX, IDC_G_Y, m_combo_y);
}


BEGIN_MESSAGE_MAP(A, CDialogEx)
	ON_CBN_SELCHANGE(IDC_G_X, &A::OnCbnSelchangeGX)
	ON_CBN_SELCHANGE(IDC_G_Y, &A::OnCbnSelchangeGY)
	ON_BN_CLICKED(IDC_EXPORT, &A::OnBnClickedExport)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_GRAPH_RBTN_OP1, &A::OnPopupOption1)
	ON_COMMAND(ID_GRAPH_RBTN_OP2, &A::OnPopupOption2)
END_MESSAGE_MAP()


void A::update_components(Doc* s_doc)
{
	m_doc = s_doc;
	initialize_combobox();
	update_graph();
}
void A::initialize_combobox()
{
	if (m_doc == nullptr)
		return;
	m_combo_x.ResetContent();
	m_combo_y.ResetContent();
	auto headers = m_doc->getHeaders();
	for (CString& str : headers)
	{
		m_combo_x.AddString(str);
		m_combo_y.AddString(str);
	}
}
void A::update_comboboxes()
{
	int x_sel_id, y_sel_id;
	CString x_sel, y_sel;
	//get the selection ids
	x_sel_id = m_combo_x.GetCurSel();
	y_sel_id = m_combo_y.GetCurSel();

	//store the current selection strings which can be fetched with sel ids
	if (x_sel_id != CB_ERR)
	{
		m_combo_x.GetLBText(x_sel_id, x_sel);
		//m_combo_y.ResetContent();
	}
	if (y_sel_id != CB_ERR)
	{
		m_combo_y.GetLBText(y_sel_id, y_sel);
		//m_combo_y.ResetContent();
	}
	m_combo_x.ResetContent();
	m_combo_y.ResetContent();
	auto headers = m_doc->getHeaders();
	for (CString& str : headers)
	{
		if (str != y_sel)
		{
			int id = m_combo_x.AddString(str);
			if (str == x_sel)
				m_combo_x.SetCurSel(id);
		}
		if (str != x_sel)
		{
			int id = m_combo_y.AddString(str);
			if (str == y_sel)
				m_combo_y.SetCurSel(id);
		}
		
	}
	update_graph();
}

void A::clear_graph()
{
	if(m_graph.GetBottomAxis())
		m_graph.GetBottomAxis()->GetLabel()->SetText(_T(""));
	if (m_graph.GetLeftAxis())
		m_graph.GetLeftAxis()->GetLabel()->SetText(_T(""));
	m_graph.RemoveAllSeries();
}

void A::update_graph()
{
	if (m_doc == nullptr)
		return;
	
	int xIndex = m_combo_x.GetCurSel();
	int yIndex = m_combo_y.GetCurSel();
	if (xIndex == CB_ERR || yIndex== CB_ERR)
		return;

	CString XstrSelected;
	m_combo_x.GetLBText(xIndex, XstrSelected);
	CString YstrSelected;
	m_combo_y.GetLBText(yIndex, YstrSelected);

	m_graph.RemoveAllSeries();
	// TODO:  Add extra initialization here
	m_graph.EnableRefresh(FALSE);
	CChartStandardAxis* pBottomAxis =
		m_graph.CreateStandardAxis(CChartCtrl::BottomAxis);

	// Create Y axis (left)
	CChartStandardAxis* pLeftAxis =
		m_graph.CreateStandardAxis(CChartCtrl::LeftAxis);
	COLORREF redAxisTextColor = RGB(255, 0, 0);
	COLORREF greenAxisTextColor = RGB(0, 255, 0);

	//m_graph.GetBottomAxis()->SetTextColor(redAxisTextColor); // X axis
	//m_graph.GetLeftAxis()->SetTextColor(greenAxisTextColor);   // Y axis

	pBottomAxis->GetLabel()->SetText(TChartString(XstrSelected));
	pLeftAxis->GetLabel()->SetText(TChartString(YstrSelected));
	CChartAxisLabel* xLabel = pBottomAxis->GetLabel();
	CChartAxisLabel* yLabel = pLeftAxis->GetLabel();

	xLabel->SetColor(RGB(255, 0, 0));
	yLabel->SetColor(RGB(0, 0, 255));
	
	// Turn refresh back on
	m_graph.EnableRefresh(true);

	// Clear the old graph first


	// get the data for x and y //later it would be fetched from combo_box
	auto x = m_doc->get_collumn(XstrSelected);
	auto y = m_doc->get_collumn(YstrSelected);
	int g_size = min(x.size(), y.size());

	//set range
	auto max_x = std::max_element(x.begin(), x.end());
	auto max_y = std::max_element(y.begin(), y.end());
	int m_x = *max_x;
	int m_y = *max_y;
	pBottomAxis->SetMinMax(0, m_x);
	pLeftAxis->SetMinMax(0, m_y);

	// Create new line
	CChartLineSerie* pLineSeries =
		m_graph.CreateLineSerie();

	// Add points
	pLineSeries->SetPoints(x.data(), y.data(), g_size);
	pLineSeries->SetColor(RGB(0, 0, 255));
	pLineSeries->SetWidth(3);
	// Redraw
	m_graph.RefreshCtrl();

}

void A::OnCbnSelchangeGX()
{
	update_comboboxes();
	// TODO: Add your control notification handler code here
}


void A::OnCbnSelchangeGY()
{
	update_comboboxes();
	
	// TODO: Add your control notification handler code here
}

void A::RBTN_option(CPoint point)
{
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, ID_GRAPH_RBTN_OP1, _T("Export Graph"));
	menu.AppendMenu(MF_STRING, ID_GRAPH_RBTN_OP2, _T("Export Data"));
	CPoint screenPoint = point;
	ClientToScreen(&screenPoint);
	UINT selected = menu.TrackPopupMenu(
		TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		screenPoint.x,
		screenPoint.y,
		this
	);
	
}

void A::OnBnClickedExport()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(
		FALSE,
		L"bmp",
		L"graph.bmp",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Bitmap Files (*.bmp)|*.bmp||"
	);

	if (dlg.DoModal() == IDOK)
	{
		// Get exactly what the user entered
		CString path = dlg.GetPathName();

		// Make sure it has .bmp extension
		if (path.Right(4).CompareNoCase(L".bmp") != 0)
		{
			path += L".bmp";
		}

		CRect rect;
		m_graph.GetClientRect(&rect);

		// Convert CString to the type ChartCtrl needs
		TChartString filename(path.GetString());

		m_graph.SaveAsImage(
			filename,
			rect,
			24
		);
	}
}
void A::OnContextMenu(CWnd* pWnd, CPoint point)
{
	
	// Convert screen coordinates to A's client coordinates
	ScreenToClient(&point);

	// Check whether the click is inside the custom control's rectangle
	CWnd* pControl = GetDlgItem(IDC_CUSTOM2);

	CRect rect;
	pControl->GetWindowRect(&rect);

	ScreenToClient(&rect);

	if (rect.PtInRect(point))
	{
		RBTN_option(point);
		//OnBnClickedExport();
	}
}

void A::OnPopupOption1() //export graph
{
	OnBnClickedExport();
	AfxMessageBox(_T("Graph Saved!"));
}
void A::OnPopupOption2()//export data
{
	int x_id = m_combo_x.GetCurSel();
	int y_id = m_combo_y.GetCurSel();
	CString x_str, y_str;
	m_combo_x.GetLBText(x_id, x_str);
	m_combo_y.GetLBText(y_id, y_str);
	std::vector<double> dx = m_doc->get_collumn(x_str);
	std::vector<double> dy = m_doc->get_collumn(y_str);

	//Savefile pop up
	CFileDialog dlg(
		FALSE, 
		_T("csv"),
		_T("untitled.csv"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("CSV Files (*.csv)|*.csv|Text Files (*.txt)|*.txt||"),
		this
	);
	if (dlg.DoModal() != IDOK)
		return;
	CString filepath = dlg.GetPathName();
	m_doc->saveData(filepath, x_str, y_str, dx, dy);
	AfxMessageBox(_T("Data Saved!"));
}