#pragma once
#include "PunchingMachine.h"

// CStatusInformationDlg 대화 상자
class CPunchingMachineDlg;
class CStatusInformationDlg : public CDialog
{
	DECLARE_DYNAMIC(CStatusInformationDlg)

public:
	CStatusInformationDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CStatusInformationDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATUSINFORMATION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBnClickedButtonStatusinformationAuto();
	afx_msg void OnBnClickedButtonStatusinformationManual();
	afx_msg void OnBnClickedButtonStatusinformationOffbypass();
	afx_msg void OnBnClickedButtonStatusinformationOnbypass();
	afx_msg void OnBnClickedButtonStatusinformationInitialize();
	afx_msg void OnBnClickedMfcbuttonStatusinformationStatusclear();
	afx_msg void OnBnClickedCheckStatusinformationSkipSearchmark();
	afx_msg void OnBnClickedCheckStatusinformationSkipPunch();
	DECLARE_MESSAGE_MAP()
public:
	CPunchingMachineDlg* m_pPunchingMachineDlg;
	CEdit m_EditStatus;
	CButton m_btnAuto;
	CButton m_btnManual;
	CButton m_btnOffBypass;
	CButton m_btnOnBypass;
	CButton m_btnInitialize;
	CButton m_Check_Skip_SearchMark;
	CButton m_Check_Skip_Punch;
	CButton m_Check_Skip;


	CFont		m_fontMode;
	CFont		m_fontStatus;


	BOOL		m_bCheckClickedInitBtn;

public:
	void Init();
	bool ClickControl(int IDC_TARTGET, CPoint pt);
};