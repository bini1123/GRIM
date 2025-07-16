// glimDlg.h: 헤더 파일
//

#pragma once
#include <afxwin.h>


// CglimDlg 대화 상자
class CglimDlg : public CDialogEx
{
// 생성입니다.
public:
	CglimDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIM_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
public:
	std::vector<CPoint> m_points;
	void CalcCircleFrom3Points();

protected:
	HICON m_hIcon;
	int m_dragIndex;
	CPointF m_circleCenter;
	double m_circleRadius;
	double m_radius;
	int m_thickness;
	BOOL m_bCircleDrawn;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnInit();
	afx_msg void OnBnClickedBtnRandom();

	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
