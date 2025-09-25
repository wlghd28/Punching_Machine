#pragma once

#include "PunchingMachine.h"

// CCameraControlDlg 대화 상자
class CPunchingMachineDlg;
class CCameraControlDlg : public CDialog
{
	DECLARE_DYNAMIC(CCameraControlDlg)

public:
	CCameraControlDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCameraControlDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA_CONTROL_DIALOG };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnBnClickedButtonCameraControlCamera1Connect();
	afx_msg void OnBnClickedButtonCameraControlCamera1Disconnect();
	afx_msg void OnBnClickedButtonCameraControlCamera1Play();
	afx_msg void OnBnClickedButtonCameraControlCamera1Stopplay();
	afx_msg void OnBnClickedButtonCameraControlCamera1ShowCentermark();
	afx_msg void OnBnClickedButtonCameraControlCamera1HideCentermark();
	afx_msg void OnBnClickedButtonCameraControlCamera1SearchMarkStart();
	afx_msg void OnBnClickedButtonCameraControlCamera1SearchMarkStop();
	afx_msg void OnBnClickedButtonCameraControlCamera2Connect();
	afx_msg void OnBnClickedButtonCameraControlCamera2Disconnect();
	afx_msg void OnBnClickedButtonCameraControlCamera2Play();
	afx_msg void OnBnClickedButtonCameraControlCamera2Stopplay();
	afx_msg void OnBnClickedButtonCameraControlCamera2ShowCentermark();
	afx_msg void OnBnClickedButtonCameraControlCamera2HideCentermark();
	afx_msg void OnBnClickedButtonCameraControlCamera2SearchMarkStart();
	afx_msg void OnBnClickedButtonCameraControlCamera2SearchMarkStop();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();


public:
	// 멤버변수
	CPunchingMachineDlg* m_pPunchingMachineDlg;

	// 멤버함수
	void Init();
};
