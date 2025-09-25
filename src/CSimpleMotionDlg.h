#pragma once

#include "PunchingMachine.h"
#include "HoverButton.h"
#include "StaticEx.h"
#include "PAIX/NMC2.h"


// CSimpleMotionDlg 대화 상자
class CPunchingMachineDlg;
class CMotionDlg;
class CSimpleMotionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSimpleMotionDlg)

public:
	CSimpleMotionDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSimpleMotionDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMPLE_MOTION_DIALOG };
#endif
protected:
	// MFC 메시지 처리기
	afx_msg void OnBnClickedMfcbuttonSimplemotionHoming();
	afx_msg void OnBnClickedMfcbuttonSimplemotionAxis2RelativeMove();
	afx_msg void OnBnClickedMfcbuttonSimplemotionAxis1RelativeMove();
	afx_msg void OnBnClickedMfcbuttonSimplemotionAxis0RelativeMove();
protected:
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
public:
	// 멤버변수
	CPunchingMachineDlg* m_pPunchingMachineDlg;
	CHoverButton	m_BtnAxis0_Minus;
	CHoverButton	m_BtnAxis0_Plus;
	CHoverButton	m_BtnAxis1_Minus;
	CHoverButton	m_BtnAxis1_Plus;
	CHoverButton	m_BtnAxis2_Minus;
	CHoverButton	m_BtnAxis2_Plus;
	CStaticEx		m_StaticAxis0_MLimit;
	CStaticEx		m_StaticAxis0_PLimit;
	CStaticEx		m_StaticAxis1_MLimit;
	CStaticEx		m_StaticAxis1_PLimit;
	CStaticEx		m_StaticAxis2_MLimit;
	CStaticEx		m_StaticAxis2_PLimit;

public:
	// 멤버함수
	void Init();
	void MoveRect(CRect rect);
	void Homing();
};
