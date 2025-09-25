#pragma once
#include "PunchingMachine.h"
#include "StaticEx.h"
#include "HoverButton.h"
#include "PAIX/NMC2.h"

#define DEVICENUM 10

#define clSilver RGB(192,192,192)
#define clLime RGB(0,255,0)
#define clRed RGB(255,0,0)
#define clWhite RGB(255,255,255)
#define clBlack RGB(0,0,0)

#if !defined(AFX_VC403SDLG_H__C6E0AB39_278E_4EF5_8C55_6242B60AA684__INCLUDED_)
#define AFX_VC403SDLG_H__C6E0AB39_278E_4EF5_8C55_6242B60AA684__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMotionDlg 대화 상자
class CPunchingMachineDlg;
class CMotionDlg : public CDialog
{
	DECLARE_DYNAMIC(CMotionDlg)

public:
	CMotionDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMotionDlg();

	bool m_bThreadRun;
	void ReadSeonsor(void);

// 대화 상자 데이터입니다.
	CHoverButton	m_BtnCcw4;
	CHoverButton	m_BtnCcw3;
	CHoverButton	m_BtnCcw2;
	CHoverButton	m_BtnCcw1;
	CHoverButton	m_BtnCw4;
	CHoverButton	m_BtnCw3;
	CHoverButton	m_BtnCw2;
	CHoverButton	m_BtnCw1;
	CStaticEx	m_staticBusy4;
	CStaticEx	m_staticBusy3;
	CStaticEx	m_staticBusy2;
	CStaticEx	m_staticBusy1;
	CStaticEx	m_staticPLimit4;
	CStaticEx	m_staticPLimit3;
	CStaticEx	m_staticPLimit2;
	CStaticEx	m_staticPLimit1;
	CStaticEx	m_staticNear4;
	CStaticEx	m_staticNear3;
	CStaticEx	m_staticNear2;
	CStaticEx	m_staticNear1;
	CStaticEx	m_staticMLimit4;
	CStaticEx	m_staticMLimit3;
	CStaticEx	m_staticMLimit2;
	CStaticEx	m_staticMLimit1;
	CStaticEx	m_staticHome4;
	CStaticEx	m_staticHome3;
	CStaticEx	m_staticHome2;
	CStaticEx	m_staticHome1;
	CStaticEx	m_staticEmer;

	CStaticEx	m_staticAlarm4;
	CStaticEx	m_staticAlarm3;
	CStaticEx	m_staticAlarm2;
	CStaticEx	m_staticAlarm1;

	CStaticEx	m_staticInpos1;
	CStaticEx	m_staticInpos2;
	CStaticEx	m_staticInpos3;
	CStaticEx	m_staticInpos4;
	//IDC_STATIC_ALARM1
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVC403SDlg)
public:
	virtual BOOL DestroyWindow();
protected:
	//}}AFX_VIRTUAL



#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOTION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnOpen();
	afx_msg void OnSelendokCbEmer();
	afx_msg void OnSelendokCbNear1();
	afx_msg void OnSelendokCbNear2();
	afx_msg void OnSelendokCbNear3();
	afx_msg void OnSelendokCbNear4();
	afx_msg void OnSelendokCbLimit1();
	afx_msg void OnSelendokCbLimit2();
	afx_msg void OnSelendokCbLimit3();
	afx_msg void OnSelendokCbLimit4();
	afx_msg void OnBtnRpm1();
	afx_msg void OnBtnRpm2();
	afx_msg void OnBtnRpm3();
	afx_msg void OnBtnRpm4();
	afx_msg void OnBtnInc1();
	afx_msg void OnBtnInc2();
	afx_msg void OnBtnInc3();
	afx_msg void OnBtnInc4();
	afx_msg void OnBtnAbs1();
	afx_msg void OnBtnAbs2();
	afx_msg void OnBtnAbs3();
	afx_msg void OnBtnAbs4();
	afx_msg void OnBtnStop1();
	afx_msg void OnBtnStop2();
	afx_msg void OnBtnStop3();
	afx_msg void OnBtnStop4();
	afx_msg void OnBtnCurrent1();
	afx_msg void OnBtnCurrent2();
	afx_msg void OnBtnCurrent3();
	afx_msg void OnBtnCurrent4();
	afx_msg void OnBtnServo1();
	afx_msg void OnBtnServo2();
	afx_msg void OnBtnServo3();
	afx_msg void OnBtnServo4();
	afx_msg void OnBtnAlarm1();
	afx_msg void OnBtnAlarm2();
	afx_msg void OnBtnAlarm3();
	afx_msg void OnBtnAlarm4();
	afx_msg void OnSelendokCbPlimit1();
	afx_msg void OnSelendokCbPlimit2();
	afx_msg void OnSelendokCbPlimit3();
	afx_msg void OnSelendokCbPlimit4();
	//}}AFX_MSG
	afx_msg LRESULT UmJog(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDcc1();
	afx_msg void OnBnClickedButtonDcc2();
	afx_msg void OnBnClickedButtonDcc3();
	afx_msg void OnBnClickedButtonDcc4();
	afx_msg void OnCbnSelendokCbAlarm1();
	afx_msg void OnCbnSelendokCbAlarm2();
	afx_msg void OnCbnSelendokCbAlarm3();
	afx_msg void OnCbnSelendokCbAlarm4();
	afx_msg void OnCbnSelendokCbInpos1();
	afx_msg void OnCbnSelendokCbInpos2();
	afx_msg void OnCbnSelendokCbInpos3();
	afx_msg void OnCbnSelendokCbInpos4();
	afx_msg void OnCbnSelendokCbPulseMode1();
	afx_msg void OnCbnSelendokCbPulseMode2();
	afx_msg void OnCbnSelendokCbPulseMode3();
	afx_msg void OnCbnSelendokCbPulseMode4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	//	afx_msg void OnBnClickedBtnHome1();
	//	afx_msg void OnBnClickedBtnHome2();
	//	afx_msg void OnBnClickedBtnHome3();
	//	afx_msg void OnBnClickedBtnHome4();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnStnDblclickCmd1();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButtonDcc5();
	
	
public:
	// 멤버변수
	CRect m_RectOrgMotionDlg;
	CRect m_RectMotionDlg;
	HICON m_hIcon;
	BOOL m_bRemote;		// 장치 연결 변수
	int m_nPaixCtlNo;	// 장치 번호
	int m_nTimerId;
	HANDLE m_hThreadId;
	CBitmap m_OutOn, m_OutOff;
	NMCAXESMOTIONOUT OutStatus;
	CPunchingMachineDlg* m_pPunchingMachineDlg;

	// 스크롤바 관련 변수
	int m_nScrollPosH;
	int m_nScrollPosV;
	int m_nPageSizeV;
	int m_nPageSizeH;

public:
	// 멤버함수
	static UINT ThreadProc(void *pVoid);
	void Init();
	void SetScreenRect(CRect rect);
	int GetPaixCtlNo();
	void InitSpeed();
	void UpdateScrollInfo(UINT nType, int cx, int cy);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VC403SDLG_H__C6E0AB39_278E_4EF5_8C55_6242B60AA684__INCLUDED_)
