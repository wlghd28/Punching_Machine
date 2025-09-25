#pragma once
#include "PunchingMachine.h"
#include "PictureControlEx.h"

// CMainDlg 대화 상자
class CPunchingMachineDlg;
class CameraSubDlg;
class CStatusInformationDlg;
class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMainDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif
public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg LRESULT OnMyDiscoveryMsg(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	// difine function
	void initCameraSubDlg();
	void initStatusInformationDlg();

private:
	// define various
	CRect					m_RectMainDlg;
	CPunchingMachineDlg*	m_pPunchingMachineDlg;
	CameraSubDlg*			m_pCameraSubDlgs;
	CStatusInformationDlg*	m_pCStatusInformationDlg;

public:
	// Icon 
	HICON m_hIcon_OK;
	HICON m_hIcon_NG;
	HICON m_hIcon_START;
	HICON m_hIcon_STOP;
	HICON m_hIcon_ALARM1;
	HICON m_hIcon_ALARM2;
	CPictureControlEx m_PictureContorl_Main_Logo;

public:
	void Init();
	void InitIcon();
	void SetScreenRect(CRect rect);
	bool ClickControl(int IDC_TARTGET, CPoint pt);
	CameraSubDlg* GetPtrCameraSubDlg();
	CStatusInformationDlg* GetPtrStatusInformationDlg();
};
