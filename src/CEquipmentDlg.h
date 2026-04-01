#pragma once
#include "PunchingMachine.h"
#include "StaticEx.h"
#include "PAIX/NMF.h"
#include "PAIX/NMC2.h"
#include "EnumDefine.h"


// CEquipmentDlg 대화 상자
class CPunchingMachineDlg;
class CEquipmentDlg : public CDialog
{
	DECLARE_DYNAMIC(CEquipmentDlg)

public:
	CEquipmentDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CEquipmentDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EQUIPMENT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBnClickedMfcbuttonEquipmentInitialize();
	afx_msg void OnBnClickedButtonEquipmentAuto();
	afx_msg void OnBnClickedButtonEquipmentManual();
	afx_msg void OnBnClickedMfcbuttonEquipmentStatusclear();
	afx_msg void OnBnClickedMfcbuttonEquipmentSequencePlatein();
	afx_msg void OnBnClickedMfcbuttonEquipmentSequenceCentering();
	afx_msg void OnBnClickedMfcbuttonEquipmentSequenceClamp();
	afx_msg void OnBnClickedMfcbuttonEquipmentSequenceAlignment();
	afx_msg void OnBnClickedMfcbuttonEquipmentSequencePunch();
	afx_msg void OnBnClickedMfcbuttonEquipmentSequenceEject();
	afx_msg void OnBnClickedMfcbuttonEquipmentSequenceRotate();
	afx_msg void OnBnClickedMfcbuttonEquipmentSequenceExit();
	afx_msg void OnBnClickedMfcbuttonEquipmentSequenceAllstop();
	afx_msg void OnBnClickedMfcbuttonEquipmentRollerCw();
	afx_msg void OnBnClickedMfcbuttonEquipmentRollerCcw();
	afx_msg void OnBnClickedMfcbuttonEquipmentRollerUp();
	afx_msg void OnBnClickedMfcbuttonEquipmentRollerDown();
	afx_msg void OnBnClickedMfcbuttonEquipmentCameraUp();
	afx_msg void OnBnClickedMfcbuttonEquipmentCameraDown();
	afx_msg void OnBnClickedMfcbuttonEquipmentCenter2On();
	afx_msg void OnBnClickedMfcbuttonEquipmentCenter1On();
	afx_msg void OnBnClickedMfcbuttonEquipmentCenter2Off();
	afx_msg void OnBnClickedMfcbuttonEquipmentCenter1Off();
	afx_msg void OnBnClickedMfcbuttonEquipmentPstpA1Up();
	afx_msg void OnBnClickedMfcbuttonEquipmentPstpA2Up();
	afx_msg void OnBnClickedMfcbuttonEquipmentPstpA1Down();
	afx_msg void OnBnClickedMfcbuttonEquipmentPstpA2Down();
	afx_msg void OnBnClickedMfcbuttonEquipmentPstpB1Up();
	afx_msg void OnBnClickedMfcbuttonEquipmentPstpB2Up();
	afx_msg void OnBnClickedMfcbuttonEquipmentPstpB1Down();
	afx_msg void OnBnClickedMfcbuttonEquipmentPstpB2Down();
	afx_msg void OnBnClickedMfcbuttonEquipmentLstpA1Up();
	afx_msg void OnBnClickedMfcbuttonEquipmentLstpA2Up();
	afx_msg void OnBnClickedMfcbuttonEquipmentLstpA1Down();
	afx_msg void OnBnClickedMfcbuttonEquipmentLstpA2Down();
	afx_msg void OnBnClickedMfcbuttonEquipmentLstpB1Up();
	afx_msg void OnBnClickedMfcbuttonEquipmentLstpB2Up();
	afx_msg void OnBnClickedMfcbuttonEquipmentLstpB1Down();
	afx_msg void OnBnClickedMfcbuttonEquipmentLstpB2Down();
	afx_msg void OnBnClickedMfcbuttonEquipmentPunch1On();
	afx_msg void OnBnClickedMfcbuttonEquipmentPunch2On();
	afx_msg void OnBnClickedMfcbuttonEquipmentPunch1Off();
	afx_msg void OnBnClickedMfcbuttonEquipmentPunch2Off();
	afx_msg void OnBnClickedMfcbuttonEquipmentPlClamp1Up();
	afx_msg void OnBnClickedMfcbuttonEquipmentPlClamp2Up();
	afx_msg void OnBnClickedMfcbuttonEquipmentPlClamp1Down();
	afx_msg void OnBnClickedMfcbuttonEquipmentPlClamp2Down();
	afx_msg void OnBnClickedMfcbuttonEquipmentPlRotate();
	afx_msg void OnBnClickedMfcbuttonEquipmentVacuum();
	afx_msg void OnBnClickedMfcbuttonEquipmentSetInterval();
	afx_msg void OnBnClickedCheckEquipmentSkipSearchmark();
	afx_msg void OnBnClickedCheckEquipmentSkipPunch();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();

	
public:	// 멤버
	CPunchingMachineDlg*			m_pPunchingMachineDlg;

	CRect			m_RectOrgEquipmentDlg;
	CRect			m_RectEquipmentDlg;
	CMFCButton		m_btnCenter_On[2];
	CMFCButton		m_btnCenter_Off[2];
	CMFCButton		m_btnPStpA_Up[2];
	CMFCButton		m_btnPStpA_Down[2];
	CMFCButton		m_btnPStpB_Up[2];
	CMFCButton		m_btnPStpB_Down[2];
	CMFCButton		m_btnRollerUp;
	CMFCButton		m_btnRollerDown;
	CMFCButton		m_btnRollerCW;
	CMFCButton		m_btnRollerCCW;
	CMFCButton		m_btnPLRotate;
	CMFCButton		m_btnVacuum;
	CMFCButton		m_btnCamera_Up;
	CMFCButton		m_btnCamera_Down;
	CMFCButton		m_btnPLClamp_Up[2];
	CMFCButton		m_btnPLClamp_Down[2];
	CMFCButton		m_btnPunch_On[2];
	CMFCButton		m_btnPunch_Off[2];
	CMFCButton		m_btnLStpA_Up[2];
	CMFCButton		m_btnLStpA_Down[2];
	CMFCButton		m_btnLStpB_Up[2];
	CMFCButton		m_btnLStpB_Down[2];

	CButton			m_Check_Skip_SearchMark;
	CButton			m_Check_Skip_Punch;

	CButton			m_btnAuto;
	CButton			m_btnManual;
	CMFCButton		m_btnInitialize;
	CFont			m_fontMode;
	CFont			m_fontStatus;

	CStaticEx		m_StaticAxis0_MLimit;
	CStaticEx		m_StaticAxis0_PLimit;
	CStaticEx		m_StaticAxis1_MLimit;
	CStaticEx		m_StaticAxis1_PLimit;
	CStaticEx		m_StaticAxis2_MLimit;
	CStaticEx		m_StaticAxis2_PLimit;
	CEdit			m_EditStatus;

	// 장비구동 관련
	UINT32			m_iPulsePerMM_Axis0;
	UINT32			m_iPulsePerMM_Axis1;
	UINT32			m_iPulsePerMM_Axis2;
	UINT8			m_iSystemMode;			// 0 : 자동, 1 : 수동
	UINT8			m_iBypassMode;			// 0 : 검사시행, 1 : 검사스킵
	UINT32			m_iTimeClock_Auto;
	UINT32			m_iTimeClock_Manual;
	UINT16			m_iTimeInterval;
	short			m_iArrSet_PinStatus[MAX_DIO_PINS] = { 0, };	// Pin 상태
	short			m_iArrDI_PinStatus[MAX_DIO_PINS] = { 0, };	// Pin 상태
	short			m_iArrDO_PinStatus[MAX_DIO_PINS] = { 0, };	// Pin 상태
	double			m_dbPulse_Distance_Axis0;
	double			m_dbPulse_Distance_Axis1;
	double			m_dbPulse_Distance_Axis2;

	// Alignment
	CWinThread* m_pThreadAlignment_Move = NULL;						// Alignment Move 기능 스레드 변수
	int			m_iCheckAlignment_Move;
	int			m_iCheckMaxNum_SearchMark;
	int			m_iCheckCurNum_SearchMark;

	// 스크롤바 관련 변수
	int m_nScrollPosH;
	int m_nScrollPosV;
	int m_nPageSizeV;
	int m_nPageSizeH;

	// 상태 로그 관련 변수
	CStdioFile m_fpStatusLog;


	// Alarm
	BOOL	m_bCheckAlarmStatus;
	int		m_iToggleAlarmIcon;

	// Punch Number
	int		m_iCurrent_PunchNumber;
	int		m_iTotal_PunchNumber;

public:	// 메서드
	void SetScreenRect(CRect rect);
	BOOL Init();
	void DisplayStatus(DeviceStatus Status);
	void InitMode();
	void AutoMode();
	void ManualMode();
	void OffBypass();
	void OnBypass();
	void SetIOImage();

	// 장비IO구동
	void IO_Roller_CW();
	void IO_Roller_CCW();
	void IO_LStpB_UpDown(BOOL bValue);
	void IO_Center_OnOff(BOOL bValue);
	void IO_PStpB_UpDown(BOOL bValue);
	void IO_LStpA_UpDown(BOOL bValue);
	void IO_PStpA_UpDown(BOOL bValue);
	void IO_PL_Clamp_UpDown(BOOL bValue);
	void IO_Punch_OnOff(BOOL bValue);
	void IO_Roller_UpDown(BOOL bValue);
	void IO_Camera_UpDown(BOOL bValue);
	void IO_PL_Rotate();
	void IO_Vacuum_OnOff();



	// 장비모션구동
	void Homing();
	void MoveInitPoint();
	void Alignment_Move();


	// 시퀀스 기능
	void Sequence_Initialize();
	void Sequence_Auto();
	void Sequence_PlateIN();
	void Sequence_Centering();
	void Sequence_Clamp();
	void Sequence_Alignment();
	void Sequence_Punch();
	void Sequence_Eject();
	void Sequence_Rotate();
	void Sequence_Exit();
	void Sequence_ErrorHandler(UINT32 iTimerID, Enmf_FUNC_RESULT eRet);
	void Sequence_EnableWindow(BOOL value);
	void Sequence_KillTimer();

	void UpdateScrollInfo(UINT nType, int cx, int cy);


	// Alarm
	void AlarmON();
	void AlarmOFF();

	// Punch Number
	void ResetCurrentPunchNumber();
	void IncreasePunchNumber();

	// Thread
	static UINT ThreadAlignment_Move(LPVOID _method);
};
