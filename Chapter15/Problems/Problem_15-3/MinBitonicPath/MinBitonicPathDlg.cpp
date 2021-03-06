
// MinBitonicPathDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MinBitonicPath.h"
#include "MinBitonicPathDlg.h"
#include "afxdialogex.h"

#include <string>
#include <sstream>

#include "MinBitonicPathAlgo.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMinBitonicPathDlg 对话框



CMinBitonicPathDlg::CMinBitonicPathDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MINBITONICPATH_DIALOG, pParent)
	, m_Coordinate(_T(""))
	, m_Point(NULL)
	, m_PointChain(NULL)
	, m_PointNum(0)
	, m_PathLength(0)
	, m_EditItem(-1)
	, m_EditSubitem(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMinBitonicPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POINT_LIST, m_PointList);
	DDX_Text(pDX, IDC_POINT_EDIT, m_Coordinate);
}

BEGIN_MESSAGE_MAP(CMinBitonicPathDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_POINT_LIST, &CMinBitonicPathDlg::OnDblclkPointList)
	ON_EN_KILLFOCUS(IDC_POINT_EDIT, &CMinBitonicPathDlg::OnKillfocusPointEdit)
	ON_BN_CLICKED(IDC_GET_MIN_BITONIC_PATH, &CMinBitonicPathDlg::OnGetMinBitonicPath)
	ON_BN_CLICKED(IDC_CLEAR, &CMinBitonicPathDlg::OnClear)
END_MESSAGE_MAP()


void CMinBitonicPathDlg::InitPointList(CPoint point[], int n)
{
	unsigned long listCtrlStyle = m_PointList.GetExtendedStyle();
	listCtrlStyle |= (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_PointList.SetExtendedStyle(listCtrlStyle);

	m_PointList.InsertColumn(0, CString("Index"), LVCFMT_CENTER, 45);
	m_PointList.InsertColumn(1, CString("X"), LVCFMT_CENTER, 64);
	m_PointList.InsertColumn(2, CString("Y"), LVCFMT_CENTER, 64);

	int i = 0;

	if(n > 0 && point != NULL)
	{
		for(i = 0; i < n; ++i)
		{
			ostringstream oss;
			oss << i;
			m_PointList.InsertItem(i, CString(oss.str().c_str()));

			oss.str("");
			oss << point[i].x;
			m_PointList.SetItemText(i, 1, CString(oss.str().c_str()));

			oss.str("");
			oss << point[i].y;
			m_PointList.SetItemText(i, 2, CString(oss.str().c_str()));
		}
	}

	m_PointList.InsertItem(i, CString(""));
}


// CMinBitonicPathDlg 消息处理程序

BOOL CMinBitonicPathDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_POINT_EDIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_POINT_EDIT)->ModifyStyle(ES_LEFT, ES_CENTER);

	m_MinBitonicPathFigure.SubclassDlgItem(IDC_MIN_BITONIC_PATH_FIGURE, this);

	m_PointNum = 7;
	m_Point = new CPoint[m_PointNum];
	m_PointChain = new int[m_PointNum-1];

	m_Point[0] = CPoint(0, 6);
	m_Point[1] = CPoint(1, 0);
	m_Point[2] = CPoint(2, 3);
	m_Point[3] = CPoint(5, 4);
	m_Point[4] = CPoint(6, 1);
	m_Point[5] = CPoint(7, 5);
	m_Point[6] = CPoint(8, 2);

	InitPointList(m_Point, m_PointNum);

	m_PathLength = GetMinBitonicPath(m_Point, m_PointNum, m_PointChain);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMinBitonicPathDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMinBitonicPathDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CMinBitonicPathDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMinBitonicPathDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;

	CWnd *pWnd = GetDlgItem(IDC_MIN_BITONIC_PATH_FIGURE);
	if(pWnd && pWnd->m_hWnd)
	{
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);
		rect.bottom = cy - 13;
		rect.right = cx - 12;
		pWnd->MoveWindow(rect);
	}

	pWnd = GetDlgItem(IDC_CLEAR);
	int button_top = 0;
	if(pWnd && pWnd->m_hWnd)
	{
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);
		int b = rect.bottom;
		rect.bottom = cy - 12;
		rect.top += (rect.bottom - b);
		button_top = rect.top;
		pWnd->MoveWindow(rect);
	}

	pWnd = GetDlgItem(IDC_GET_MIN_BITONIC_PATH);
	if(pWnd && pWnd->m_hWnd)
	{
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);
		int b = rect.bottom;
		rect.bottom = button_top - 10;
		rect.top += (rect.bottom - b);
		button_top = rect.top;
		pWnd->MoveWindow(rect);
	}

	pWnd = GetDlgItem(IDC_POINT_LIST);
	if(pWnd && pWnd->m_hWnd)
	{
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);
		rect.bottom = button_top - 9;
		pWnd->MoveWindow(rect);
	}

	Invalidate();
}

void CMinBitonicPathDlg::OnDblclkPointList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	m_EditItem = pNMItemActivate->iItem;
	m_EditSubitem = pNMItemActivate->iSubItem;

	if(m_EditItem > -1 && m_EditSubitem > 0)
	{
		CRect editRect;
		m_PointList.GetSubItemRect(m_EditItem, m_EditSubitem, LVIR_BOUNDS, editRect);
		m_PointList.ClientToScreen(editRect);
		ScreenToClient(editRect);
		GetDlgItem(IDC_POINT_EDIT)->MoveWindow(editRect);
		GetDlgItem(IDC_POINT_EDIT)->ShowWindow(SW_SHOW);

		m_Coordinate = m_PointList.GetItemText(m_EditItem, m_EditSubitem);
		UpdateData(FALSE);

		GetDlgItem(IDC_POINT_EDIT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_POINT_EDIT))->SetSel(-1);
	}

	*pResult = 0;
}

void CMinBitonicPathDlg::OnKillfocusPointEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	int itemCount = m_PointList.GetItemCount();

	UpdateData();
	GetDlgItem(IDC_POINT_EDIT)->ShowWindow(SW_HIDE);
	m_PointList.SetItemText(m_EditItem, m_EditSubitem, m_Coordinate);

	if(m_EditItem == itemCount - 1)
	{
		if(!m_PointList.GetItemText(m_EditItem, m_EditSubitem).IsEmpty())
		{
			ostringstream oss;
			oss << m_EditItem;
			m_PointList.SetItemText(m_EditItem, 0, CString(oss.str().c_str()));
			m_PointList.InsertItem(m_EditItem + 1, CString(""));
		}
	}
	else if(m_EditItem == itemCount - 2)
	{
		if(m_PointList.GetItemText(m_EditItem, 1).IsEmpty()
			&& m_PointList.GetItemText(m_EditItem, 2).IsEmpty())
		{
			m_PointList.SetItemText(m_EditItem, 0, CString(""));
			m_PointList.DeleteItem(m_EditItem + 1);
		}
	}

	Invalidate();
}

void CMinBitonicPathDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (GetFocus() == GetDlgItem(IDC_POINT_EDIT))
	{
		int itemCount = m_PointList.GetItemCount();

		UpdateData();
		GetDlgItem(IDC_POINT_EDIT)->ShowWindow(SW_HIDE);
		m_PointList.SetItemText(m_EditItem, m_EditSubitem, m_Coordinate);

		if (m_EditItem == itemCount - 1)
		{
			if (!m_PointList.GetItemText(m_EditItem, m_EditSubitem).IsEmpty())
			{
				ostringstream oss;
				oss << m_EditItem;
				m_PointList.SetItemText(m_EditItem, 0, CString(oss.str().c_str()));

				itemCount = m_PointList.GetItemCount();

				if (itemCount < m_EditItem + 2)
				{
					m_PointList.InsertItem(m_EditItem + 1, CString(""));
				}
				else
				{
					while (itemCount > m_EditItem + 2)
						m_PointList.DeleteItem(--itemCount);
				}
			}
		}
		else if (m_EditItem == itemCount - 2)
		{
			if (m_PointList.GetItemText(m_EditItem, 1).IsEmpty()
				&& m_PointList.GetItemText(m_EditItem, 2).IsEmpty())
			{
				m_PointList.SetItemText(m_EditItem, 0, CString(""));
				m_PointList.DeleteItem(m_EditItem + 1);
			}
		}

		Invalidate();
	}

	//CDialogEx::OnOK();
}

void CMinBitonicPathDlg::OnGetMinBitonicPath()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	if (m_Point) delete[] m_Point;
	if (m_PointChain) delete[] m_PointChain;

	m_PointNum = m_PointList.GetItemCount() - 1;
	m_Point = (m_PointNum > 0) ? new CPoint[m_PointNum] : NULL;
	m_PointChain = (m_PointNum > 1) ? new int[m_PointNum-1] : NULL;

	for (int i = 0; i < m_PointNum; ++i)
	{
		m_Point[i].x = _wtoi(m_PointList.GetItemText(i, 1));
		m_Point[i].y = _wtoi(m_PointList.GetItemText(i, 2));
	}

	m_PathLength = GetMinBitonicPath(m_Point, m_PointNum, m_PointChain);

	Invalidate();
}

void CMinBitonicPathDlg::OnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Point)
	{
		delete[] m_Point;
		m_Point = NULL;
	}

	if (m_PointChain)
	{
		delete[] m_PointChain;
		m_PointChain = NULL;
	}

	m_PointNum = 0;
	m_PathLength = 0;

	m_PointList.DeleteAllItems();
	m_PointList.InsertItem(0, CString(""));

	Invalidate();
}
