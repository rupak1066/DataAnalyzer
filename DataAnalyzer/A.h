#pragma once
#define ID_GRAPH_RBTN_OP1 1001
#define ID_GRAPH_RBTN_OP2 1002
#include "ChartCtrl/ChartCtrl.h"
#include "ChartCtrl/ChartLineSerie.h"
#include "ChartCtrl/ChartAxisLabel.h"
#include "Doc.h"
// A dialog

class A : public CDialogEx
{
	DECLARE_DYNAMIC(A)

public:
	A(CWnd* pParent = nullptr);   // standard constructor
	virtual ~A();
	CChartCtrl m_graph;
	void update_components(Doc* s_doc);
	void clear_graph();
	void update_graph();
	void initialize_combobox();
	void update_comboboxes();
	void RBTN_option(CPoint point);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_A };
#endif
private:
	Doc* m_doc = nullptr;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo_x;
	CComboBox m_combo_y;
	afx_msg void OnCbnSelchangeGX();
	afx_msg void OnCbnSelchangeGY();
	afx_msg void OnBnClickedExport();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPopupOption1();
	afx_msg void OnPopupOption2();
};
