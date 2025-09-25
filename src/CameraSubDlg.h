#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "MvCamera.h"
#include "SearchMark.h"
#include "PunchingMachine.h"
#include "GdipButton.h"

// Custom windows message ID, 200 reserved for control
#define WM_MY_MESSAGE_BASE (200)
#define	WM_MY_DISCOVERY_MESSAGE (WM_USER + WM_MY_MESSAGE_BASE + 1)
#define	WM_MY_DISCONNECT_MESSAGE (WM_USER + WM_MY_MESSAGE_BASE + 2)

// CameraSubDlg 대화 상자
class CSearchMark;
class CDisplayView;
class CameraSubDlg : public CDialog, public IMvCameraSink
{
	DECLARE_DYNAMIC(CameraSubDlg)

public:
	CameraSubDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CameraSubDlg();

public:
	// MvCamera status callback function
	virtual void onCameraDisconnect();
	 
	// define function
	void setRect(CRect rect);
	void setCamera(unsigned int index);
	void setSearchMark(unsigned int index);
	CMvCamera* getCMvCameraPtr();
public:
	// MFC event callback processing function
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonTrigger();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStopplay();
	afx_msg void OnBnClickedButtonSearchMarkStart();
	afx_msg void OnBnClickedButtonSearchMarkStop();
	afx_msg void OnCbnSelchangeComboTriggermode();
	afx_msg void OnBnClickedButtonShowCenterfocus();
	afx_msg void OnBnClickedButtonHideCenterfocus();
	afx_msg void OnBnClickedButtonReverseX();
	afx_msg void OnBnClickedButtonReverseY();
	afx_msg LRESULT OnMyDisconnectMsg(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedMfcbuttonCamera1v1mode();
	virtual BOOL DestroyWindow();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA_DIALOG };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

public:
	// Various MFC control variables
	CEdit							m_editCameraName;
	CComboBox						m_cmbTriggerMode;
	CMFCButton						m_btnTrigger;
	CListBox						m_lbShowWindow;
	CMFCButton						m_btnConnect;
	CMFCButton						m_btnDisconnect;
	CMFCButton						m_btnPlay;
	CMFCButton						m_btnStopPlay;
	CMFCButton						m_btnShowCenterMark;
	CMFCButton						m_btnHideCenterMark;
	CMFCButton						m_btnSearchMarkStart;
	CMFCButton						m_btnSearchMarkStop;
	CMFCButton						m_btnEngineerOn;
	CMFCButton						m_btnEngineerOff;
	CMFCButton						m_btnSaveEngineerParameter;
	CMFCButton						m_btnLoadEngineerParameter;
	CMFCButton						m_btnReverseX;
	CMFCButton						m_btnReverseY;
	CMFCButton						m_btn1vs1mode;
	CRect							m_rectDlg;
	CFont							m_font;
	int								m_iCameraIndex;
	CRect							m_rectCameraDisplay;
	CListBox*						m_ptrCameraDisplay;
	HBRUSH							m_hBrushCenterLine;
	int								m_iFlag_Camera_Disconnect;						// 0 : 카메라 연결된 상태, 1: 카메라 연결 끊긴 상태
	int								m_iThread_timercount;							// 30 이상일 경우 m_pThreadSearchMark 강제종료
	CMvCamera*						m_pMvCamera;
	CDisplayView*					m_pDisplayView;
	int								m_iCheck1v1Mode;
	CFont							m_fontSearchMarkAccuracy;

	// SearchMark
	CSearchMark*					m_pSearchMark;
	CWinThread*						m_pThreadSearchMark = NULL;						// Search Mark 기능 스레드 변수
	int								m_iEngineerMode;
	int								m_iSearchMarkMode;								// 0 : Stop, 1 : Start
	CSize							m_distSearchedMarkCenter;						// 비전 검출된 원 중심의 좌표
	double							m_dbDistMM_X_SearchedMarkCenter;
	double							m_dbDistMM_Y_SearchedMarkCenter;

	// Alignment
	CWinThread*						m_pThreadAlignment_SearchMark = NULL;						// Search Mark 기능 스레드 변수
	int								m_iCheckAlignment_SearchMark = 0;								// Alignment Search Mark 로직이 끝났는지 판단 (0 : 안끝남, 1: 끝남)
	int								m_iCheckOK_SearchMark = 0;										// Search Mark 검증, 0 : NG, 1 : OK
	int								m_iPixel_SearchMark;									// Search Mark 검증, 현재 중심과의 픽셀상의 거리 (0 에 가까울수록 정확)				

	// Icon 
	HICON m_hIcon_OK;
	HICON m_hIcon_NG;


public:
	void enableProperties(BOOL bEnable);
	void enableWindowAll(BOOL bEnbale);
	void initParamProperty();
	bool procTriggerMode(const char* pMode);
	void ShowCenterMark();
	void HideCenterMark();
	void CreateDisplayView();

	void SearchMark();
	void Alignment_SearchMark();

	void AutoConnect();
	void InitINIParameter();
	void InitIcon();

	// Thread
	static UINT ThreadSearchMark(LPVOID _method);
	static UINT ThreadAlignment_SearchMark(LPVOID _method);
};
