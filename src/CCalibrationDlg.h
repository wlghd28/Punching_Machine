#pragma once

#include "PunchingMachine.h"

// CCalibrationDlg 대화 상자
class CPunchingMachineDlg;
class CCalibrationDlg : public CDialog
{
	DECLARE_DYNAMIC(CCalibrationDlg)

public:
	CCalibrationDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCalibrationDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALIBRATION_DIALOG };
#endif
protected:
	// MFC 이벤트 처리기
	afx_msg void OnBnClickedCheckCalibrationCamera1PickingPoint1();
	afx_msg void OnBnClickedCheckCalibrationCamera1PickingPoint2();
	afx_msg void OnBnClickedCheckCalibrationCamera2PickingPoint1();
	afx_msg void OnBnClickedCheckCalibrationCamera2PickingPoint2();
	afx_msg void OnBnClickedMfcbuttonCalibrationCamera1Caloffset();
	afx_msg void OnBnClickedMfcbuttonCalibrationCamera2Caloffset();
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
public:
	// 멤버변수
	CPunchingMachineDlg* m_pPunchingMachineDlg;

	// 0 : Camera1, 1 : Camera2
	//UINT32		m_iCameraDPI = 2540;
	CPoint		m_PickPoint1[2];
	CPoint		m_PickPoint2[2];
	double		m_dbOffset[2];
	CButton		m_CheckBtn_Picking1[2];
	CButton		m_CheckBtn_Picking2[2];

public:
	// 멤버함수
	void Init();
	void SetPickPoint1(CPoint point, int iIndex);
	void SetPickPoint2(CPoint point, int iIndex);
	void CalOffset(int iIndex);
};
