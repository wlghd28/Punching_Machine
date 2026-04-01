
// PunchingMachineDlg.h: 헤더 파일
//

#pragma once
#include <windows.h>
#include <afxmt.h>
#include "CTabCtrlEx.h"
#include "PictureControlEx.h"

#define MOTIONTEST
#define IOTEST

// CPunchingMachineDlg 대화 상자
class CMainDlg;
class CEquipmentDlg;
#ifdef IOTEST
class CIODlg;
#endif
#ifdef MOTIONTEST
class CMotionDlg;
#endif

class CEngineerDlg;
class CSimpleMotionDlg;
class CCalibrationDlg;
class CCameraControlDlg;
class CPunchingMachineDlg : public CDialogEx
{
// 생성입니다.
public:
	CPunchingMachineDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CPunchingMachineDlg();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PUNCHINGMACHINE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	afx_msg void OnClose();
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
// 구현입니다.

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeTabParent(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEngineer();
	afx_msg void OnSimplemotion();
	afx_msg void OnCameraCalibration();
	afx_msg void OnCameraControl();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnBnClickedMfcbuttonParentExit();
	DECLARE_MESSAGE_MAP()

private:
	// difine function
	void InitMonitorScreen();
	void InitFont();
	void InitTabCtrl();
	BOOL InitChildDialog();
	void InitINI();

private:
	// define various
	CFont m_TabFont_OnFocus;
	CFont m_TabFont_OffFocus;
	CTabCtrlEx m_TabCtrl_Parent;
	CMainDlg* m_MainDlg;
	CEquipmentDlg* m_EquipmentDlg;
#ifdef IOTEST
	CIODlg* m_IODlg;
#endif
#ifdef MOTIONTEST
	CMotionDlg* m_MotionDlg;
#endif
	CEngineerDlg* m_EngineerDlg;
	CSimpleMotionDlg* m_SimpleMotionDlg;
	CCalibrationDlg* m_CalibrationDlg;
	CCameraControlDlg* m_CameraControlDlg;
public:
	CString	m_strPath_opencv_parameter_ini = ".\\config\\opencv_parameter.ini";
	CString	m_strPath_motion_ini = ".\\config\\motion.ini";
	CString	m_strPath_camera_ini = ".\\config\\camera.ini";
	CString	m_strPath_punchdata_ini = ".\\config\\punchdata.ini";

	BOOL m_bCheckEngineerDlg = FALSE;
	BOOL m_bCheckSimpleMotionDlg = FALSE;
	BOOL m_bCheckCalibrationDlg = FALSE;
	BOOL m_bCheckCameraControlDlg = FALSE;

public:
	CEngineerDlg* GetPtrEngineerDlg();
	CCalibrationDlg* GetPtrCalibrationDlg();
	CSimpleMotionDlg* GetPtrSimpleMotionDlg();
	CMainDlg* GetPtrMainDlg();
	CEquipmentDlg* GetPtrEquipmentDlg();
#ifdef IOTEST
	CIODlg* GetPtrIODlg();
#endif
#ifdef MOTIONTEST
	CMotionDlg* GetPtrMotionDlg();
#endif

	void Wait(DWORD dwMillisecond);
	void CaptureScreen();
};
