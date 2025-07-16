
// glimDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "glim.h"
#include "glimDlg.h"
#include "afxdialogex.h"

#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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


// CglimDlg 대화 상자



CglimDlg::CglimDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIM_DIALOG, pParent),
	m_dragIndex(-1), m_bCircleDrawn(FALSE)
{
	m_radius = 100;     // 기본값 (수정 가능)
	m_thickness = 2;    // 기본값 (수정 가능)
}

void CglimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CglimDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_INIT, &CglimDlg::OnBnClickedBtnInit)
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CglimDlg::OnBnClickedBtnRandom)
END_MESSAGE_MAP()


// CglimDlg 메시지 처리기

BOOL CglimDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CglimDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void DrawCircle(CDC* pDC, CPointF center, double radius, int thickness)
{
	const int N = 180;
	double dTheta = 2.0 * 3.141592 / N;
	CPen pen(PS_SOLID, thickness, RGB(0, 0, 0));
	CPen* oldPen = pDC->SelectObject(&pen);
	POINT prev = { center.x + radius, center.y };
	for (int i = 1; i <= N; ++i) {
		double theta = i * dTheta;
		POINT curr = { (LONG)(center.x + radius * cos(theta)),
					   (LONG)(center.y + radius * sin(theta)) };
		pDC->MoveTo(prev);
		pDC->LineTo(curr);
		prev = curr;
	}
	pDC->SelectObject(oldPen);
}

void CglimDlg::OnPaint()
{
	CPaintDC dc(this);
	// 점 그리기 (3개까지)
	for (size_t i = 0; i < m_points.size(); ++i) {
		CBrush brush(RGB(0, 0, 0));
		CBrush* oldBrush = dc.SelectObject(&brush);
		dc.Ellipse(m_points[i].x - 7, m_points[i].y - 7, m_points[i].x + 7, m_points[i].y + 7); // 작은 원 (점 표시)
		dc.SelectObject(oldBrush);

		// 좌표 표시
		CString str;
		str.Format(_T("(%d, %d)"), m_points[i].x, m_points[i].y);
		dc.TextOut(m_points[i].x + 10, m_points[i].y - 15, str);
	}

	// 정원 그리기 (3점 모두 찍은 후)
	if (m_bCircleDrawn && m_points.size() == 3) {
		// m_radius를 사용자 입력값으로 사용 (아니면 m_circleRadius)
		DrawCircle(&dc, m_circleCenter, m_circleRadius, m_thickness);
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CglimDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CglimDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bCircleDrawn) {
		// 3점 중에 클릭한 점이 있으면 드래그 시작
		for (size_t i = 0; i < m_points.size(); ++i) {
			if (abs(point.x - m_points[i].x) < 10 && abs(point.y - m_points[i].y) < 10) {
				m_dragIndex = (int)i;
				break;
			}
		}
	}
	else if (m_points.size() < 3) {
		// 3개 미만이면 점 추가
		m_points.push_back(point);
		if (m_points.size() == 3) {
			CalcCircleFrom3Points();
			m_bCircleDrawn = TRUE;
		}
		Invalidate();
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CglimDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_dragIndex = -1;
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CglimDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_dragIndex >= 0 && m_bCircleDrawn) {
		m_points[m_dragIndex] = point;
		CalcCircleFrom3Points();
		Invalidate();
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CglimDlg::CalcCircleFrom3Points()
{
	if (m_points.size() != 3) return;
	// double형 좌표로 계산
	double x1 = m_points[0].x, y1 = m_points[0].y;
	double x2 = m_points[1].x, y2 = m_points[1].y;
	double x3 = m_points[2].x, y3 = m_points[2].y;
	double a = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
	if (fabs(a) < 1e-6) return; // 세 점이 일직선
	double bx = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / (2 * a);
	double by = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / (2 * a);
	m_circleCenter.x = bx;
	m_circleCenter.y = by;
	m_circleRadius = sqrt((bx - x1) * (bx - x1) + (by - y1) * (by - y1));
	// m_radius = m_circleRadius; // 사용자가 입력값 따로 입력시 주석 처리
}

void CglimDlg::OnBnClickedBtnInit()
{
	m_points.clear();
	m_bCircleDrawn = FALSE;
	m_dragIndex = -1;
	Invalidate();
}

void CglimDlg::OnBnClickedBtnRandom()
{
	// 기존 점 목록을 지웁니다.
	m_points.clear();

	// 클라이언트 영역의 크기를 가져옵니다.
	CRect rc;
	GetClientRect(&rc);

	// 랜덤 시드를 초기화합니다.
	srand((unsigned)time(NULL));

	// 3개의 새로운 랜덤 점을 생성합니다.
	for (int i = 0; i < 3; ++i) {
		int x = rand() % (rc.Width() - 40) + 20;  // 경계선에서 20픽셀 안쪽
		int y = rand() % (rc.Height() - 40) + 20;
		m_points.push_back(CPoint(x, y));
	}

	// 3개의 점이 생성되었으므로 원을 계산하고 그릴 준비를 합니다.
	if (m_points.size() == 3) {
		CalcCircleFrom3Points();
		m_bCircleDrawn = TRUE;
	}

	// 화면을 갱신하여 새로운 점과 원을 그립니다.
	Invalidate();
}
