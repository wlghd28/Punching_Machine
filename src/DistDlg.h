#pragma once
#include "PunchingMachine.h"

// CDistDlg 대화 상자입니다.

class CDistDlg : public CDialog
{
	DECLARE_DYNAMIC(CDistDlg)

public:
	CDistDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CDistDlg(int nEquipNo,CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDistDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MOTION_MOVESETTING_DIALOG };

protected:
	int m_nEquipNo;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnRpm1();
	afx_msg void OnBnClickedBtnRpm2();
	afx_msg void OnBnClickedBtnRpm3();
	afx_msg void OnBnClickedBtnRpm4();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
