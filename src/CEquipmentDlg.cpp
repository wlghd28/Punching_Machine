// CEquipmentDlg.cpp: 구현 파일
//

#include "pch.h"
#include "PunchingMachineDlg.h"
#include "CMainDlg.h"
#include "CEquipmentDlg.h"
#include "CameraSubDlg.h"
#include "CIODlg.h"
#include "CMotionDlg.h"
#include "CStatusInformationDlg.h"
#include "afxdialogex.h"

#define TIMERCLOCK_INITIALIZE	0
#define TIMERCLOCK_PLATEIN		100
#define TIMERCLOCK_CENTERING	200
#define TIMERCLOCK_CLAMP		300
#define TIMERCLOCK_ALIGNMENT	400
#define TIMERCLOCK_PUNCH		500
#define TIMERCLOCK_EJECT		600
#define TIMERCLOCK_ROTATE		700
#define TIMERCLOCK_EXIT			800

#define OFFIOTIMERCLOCK			1000


#define TIMER_ALARM				9999


// CEquipmentDlg 대화 상자

#define min(x, y) (x) < (y) ? (x) : (y)
#define max(x, y) (x) > (y) ? (x) : (y)

IMPLEMENT_DYNAMIC(CEquipmentDlg, CDialog)

CEquipmentDlg::CEquipmentDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_EQUIPMENT_DIALOG, pParent)
	, m_iPulsePerMM_Axis0(0)
	, m_iPulsePerMM_Axis1(0)
	, m_iPulsePerMM_Axis2(0)
	, m_iTimeClock_Auto(0)
	, m_iTimeClock_Manual(0)
	, m_iTimeInterval(0)
	, m_iSystemMode(MODE_MANUAL)
	, m_iBypassMode(BYPASS_OFF)
	, m_pPunchingMachineDlg(NULL)
	, m_iCheckAlignment_Move(0)
	, m_dbPulse_Distance_Axis0(0)
	, m_dbPulse_Distance_Axis1(0)
	, m_dbPulse_Distance_Axis2(0)
	, m_iCheckMaxNum_SearchMark(5)
	, m_iCheckCurNum_SearchMark(0)
	, m_bCheckAlarmStatus(FALSE)
	, m_iToggleAlarmIcon(0)
	, m_iCurrent_PunchNumber(0)
	, m_iTotal_PunchNumber(0)
{

}

CEquipmentDlg::~CEquipmentDlg()
{
}

void CEquipmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_CENTER1_ON, m_btnCenter_On[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_CENTER2_ON, m_btnCenter_On[1]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_CENTER1_OFF, m_btnCenter_Off[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_CENTER2_OFF, m_btnCenter_Off[1]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PSTP_A1_UP, m_btnPStpA_Up[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PSTP_A2_UP, m_btnPStpA_Up[1]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PSTP_A1_DOWN, m_btnPStpA_Down[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PSTP_A2_DOWN, m_btnPStpA_Down[1]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PSTP_B1_UP, m_btnPStpB_Up[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PSTP_B2_UP, m_btnPStpB_Up[1]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PSTP_B1_DOWN, m_btnPStpB_Down[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PSTP_B2_DOWN, m_btnPStpB_Down[1]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_ROLLER_UP, m_btnRollerUp);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_ROLLER_DOWN, m_btnRollerDown);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_ROLLER_CW, m_btnRollerCW);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_ROLLER_CCW, m_btnRollerCCW);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PL_ROTATE, m_btnPLRotate);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_VACUUM, m_btnVacuum);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_CAMERA_UP, m_btnCamera_Up);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_CAMERA_DOWN, m_btnCamera_Down);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PL_CLAMP1_UP, m_btnPLClamp_Up[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PL_CLAMP2_UP, m_btnPLClamp_Up[1]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PL_CLAMP1_DOWN, m_btnPLClamp_Down[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PL_CLAMP2_DOWN, m_btnPLClamp_Down[1]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PUNCH1_ON, m_btnPunch_On[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PUNCH2_ON, m_btnPunch_On[1]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PUNCH1_OFF, m_btnPunch_Off[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_PUNCH2_OFF, m_btnPunch_Off[1]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_LSTP_A1_UP, m_btnLStpA_Up[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_LSTP_A2_UP, m_btnLStpA_Up[1]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_LSTP_A1_DOWN, m_btnLStpA_Down[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_LSTP_A2_DOWN, m_btnLStpA_Down[1]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_LSTP_B1_UP, m_btnLStpB_Up[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_LSTP_B2_UP, m_btnLStpB_Up[1]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_LSTP_B1_DOWN, m_btnLStpB_Down[0]);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_LSTP_B2_DOWN, m_btnLStpB_Down[1]);
	DDX_Control(pDX, IDC_EDIT_EQUIPMENT_STATUS, m_EditStatus);
	DDX_Control(pDX, IDC_BUTTON_EQUIPMENT_AUTO, m_btnAuto);
	DDX_Control(pDX, IDC_BUTTON_EQUIPMENT_MANUAL, m_btnManual);
	DDX_Control(pDX, IDC_MFCBUTTON_EQUIPMENT_INITIALIZE, m_btnInitialize);
	DDX_Control(pDX, IDC_STATIC_EQUIPMENT_AXIS0_MLIMIT, m_StaticAxis0_MLimit);
	DDX_Control(pDX, IDC_STATIC_EQUIPMENT_AXIS1_MLIMIT, m_StaticAxis1_MLimit);
	DDX_Control(pDX, IDC_STATIC_EQUIPMENT_AXIS2_MLIMIT, m_StaticAxis2_MLimit);
	DDX_Control(pDX, IDC_STATIC_EQUIPMENT_AXIS0_PLIMIT, m_StaticAxis0_PLimit);
	DDX_Control(pDX, IDC_STATIC_EQUIPMENT_AXIS1_PLIMIT, m_StaticAxis1_PLimit);
	DDX_Control(pDX, IDC_STATIC_EQUIPMENT_AXIS2_PLIMIT, m_StaticAxis2_PLimit);
	DDX_Control(pDX, IDC_CHECK_EQUIPMENT_SKIP_SEARCHMARK, m_Check_Skip_SearchMark);
	DDX_Control(pDX, IDC_CHECK_EQUIPMENT_SKIP_PUNCH, m_Check_Skip_Punch);
}


BEGIN_MESSAGE_MAP(CEquipmentDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_INITIALIZE, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentInitialize)
	ON_BN_CLICKED(IDC_BUTTON_EQUIPMENT_AUTO, &CEquipmentDlg::OnBnClickedButtonEquipmentAuto)
	ON_BN_CLICKED(IDC_BUTTON_EQUIPMENT_MANUAL, &CEquipmentDlg::OnBnClickedButtonEquipmentManual)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_STATUSCLEAR, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentStatusclear)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_PLATEIN, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequencePlatein)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_CENTERING, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceCentering)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_CLAMP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceClamp)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_ALIGNMENT, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceAlignment)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_PUNCH, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequencePunch)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_EJECT, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceEject)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_EXIT, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceExit)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_ALLSTOP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceAllstop)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_ROLLER_CW, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentRollerCw)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_ROLLER_CCW, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentRollerCcw)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_ROLLER_UP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentRollerUp)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_ROLLER_DOWN, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentRollerDown)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_CAMERA_UP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentCameraUp)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_CAMERA_DOWN, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentCameraDown)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_CENTER2_ON, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentCenter2On)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_CENTER1_ON, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentCenter1On)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_CENTER2_OFF, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentCenter2Off)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_CENTER1_OFF, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentCenter1Off)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PSTP_A1_UP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpA1Up)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PSTP_A2_UP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpA2Up)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PSTP_A1_DOWN, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpA1Down)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PSTP_A2_DOWN, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpA2Down)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PSTP_B1_UP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpB1Up)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PSTP_B2_UP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpB2Up)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PSTP_B1_DOWN, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpB1Down)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PSTP_B2_DOWN, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpB2Down)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_LSTP_A1_UP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpA1Up)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_LSTP_A2_UP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpA2Up)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_LSTP_A1_DOWN, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpA1Down)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_LSTP_A2_DOWN, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpA2Down)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_LSTP_B1_UP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpB1Up)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_LSTP_B2_UP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpB2Up)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_LSTP_B1_DOWN, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpB1Down)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_LSTP_B2_DOWN, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpB2Down)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PUNCH1_ON, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPunch1On)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PUNCH2_ON, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPunch2On)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PUNCH1_OFF, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPunch1Off)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PUNCH2_OFF, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPunch2Off)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PL_CLAMP1_UP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPlClamp1Up)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PL_CLAMP2_UP, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPlClamp2Up)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PL_CLAMP1_DOWN, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPlClamp1Down)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PL_CLAMP2_DOWN, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPlClamp2Down)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_PL_ROTATE, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPlRotate)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_VACUUM, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentVacuum)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_SET_INTERVAL, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSetInterval)
	ON_BN_CLICKED(IDC_CHECK_EQUIPMENT_SKIP_SEARCHMARK, &CEquipmentDlg::OnBnClickedCheckEquipmentSkipSearchmark)
	ON_BN_CLICKED(IDC_CHECK_EQUIPMENT_SKIP_PUNCH, &CEquipmentDlg::OnBnClickedCheckEquipmentSkipPunch)
	ON_BN_CLICKED(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_ROTATE, &CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceRotate)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CEquipmentDlg 메시지 처리기

BOOL CEquipmentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetClientRect(&m_RectOrgEquipmentDlg);
	MoveWindow(m_RectEquipmentDlg);
	//UpdateScrollInfo(0, m_RectEquipmentDlg.Width(), m_RectEquipmentDlg.Height());
	//if (!Init()) return FALSE;
	SetIOImage();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEquipmentDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CEquipmentDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	Sequence_KillTimer();
	KillTimer(TIMER_ALARM);
	return CDialog::DestroyWindow();
}


void CEquipmentDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	//UpdateScrollInfo(nType, cx, cy);
}

void CEquipmentDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nNextPos = 0;
	int nDelta = 0;
	SCROLLINFO si = { 0, };

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);

	switch (nSBCode)
	{
	case SB_LINEUP:
		nNextPos = si.nPos - 20;
		nDelta = 20;
		if (nNextPos < 0)
		{
			nNextPos = 0;
			nDelta = si.nPos;
		}
		break;
	case SB_LINEDOWN:
		nNextPos = si.nPos + 20;
		nDelta = -20;
		if (nNextPos * si.nPage >= si.nMax)
		{
			if (si.nPos + si.nPage >= si.nMax)
			{
				nDelta = 0;
				nNextPos = si.nPos;
			}
			else
			{
				nDelta = (20 - ((nNextPos + si.nPage) - si.nMax));
				nNextPos = si.nMax - si.nPage;
				nDelta = -nDelta;
			}
		}
		break;
	case SB_PAGEUP:
		nNextPos = si.nPos - si.nPage;
		nDelta = si.nPage;
		if (nNextPos < 0)
		{
			nNextPos = 0;
			nDelta = si.nPos;
		}
		break;
	case SB_PAGEDOWN:
		nNextPos = si.nPos + si.nPage;
		nDelta = si.nPage;
		if (nNextPos + si.nPage >= si.nMax)
		{
			nNextPos = si.nMax - si.nPage;
			nDelta = nNextPos - si.nPos;
		}
		nDelta = -nDelta;
		break;
	case SB_THUMBTRACK:
		nNextPos = nPos;
		nDelta = -(nNextPos - si.nPos);
		break;

	default:
		return;
	}

	m_nScrollPosV = nNextPos;
	SetScrollPos(SB_VERT, nNextPos, TRUE);
	ScrollWindow(0, nDelta);


	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CEquipmentDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nNextPos = 0;
	int nDelta = 0;
	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_HORZ, &si);

	switch (nSBCode)
	{
	case SB_LINERIGHT:
		nNextPos = si.nPos + 20;
		nDelta = -20;
		if (nNextPos * si.nPage >= si.nMax)
		{
			if (si.nPos + si.nPage >= si.nMax)
			{
				nDelta = 0;
				nNextPos = si.nPos;
			}
			else
			{
				nDelta = (20 - ((nNextPos + si.nPage) - si.nMax));
				nNextPos = si.nMax - si.nPage;
				nDelta = -nDelta;
			}
		}
		break;

	case SB_LINELEFT:
		nNextPos = si.nPos - 20;
		nDelta = 20;
		if (nNextPos < 0)
		{
			nNextPos = 0;
			nDelta = si.nPos;
		}
		break;
	case SB_PAGERIGHT:
		nNextPos = si.nPos + si.nPage;
		nDelta = si.nPage;
		if (nNextPos + si.nPage >= si.nMax)
		{
			nNextPos = si.nMax - si.nPage;
			nDelta = nNextPos - si.nPos;
		}
		nDelta = -nDelta;
		break;
	case SB_PAGELEFT:
		nNextPos = si.nPos - si.nPage;
		nDelta = si.nPage;
		if (nNextPos < 0)
		{
			nNextPos = 0;
			nDelta = si.nPos;
		}
		break;
	case SB_THUMBTRACK:
		nNextPos = nPos;
		nDelta = -(nNextPos - si.nPos);
		break;

	default:
		return;
	}

	m_nScrollPosH = nNextPos;
	SetScrollPos(SB_HORZ, nNextPos, TRUE);
	ScrollWindow(nDelta, 0);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CEquipmentDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TIMERID_INITIALIZE:
		Sequence_Initialize();
		break;
	case TIMERID_AUTOSEQUENCE:
		Sequence_Auto();
		break;
	case TIMERID_PLATEIN:
		Sequence_PlateIN();
		break;
	case TIMERID_CENTERING:
		Sequence_Centering();
		break;
	case TIMERID_CLAMP:
		Sequence_Clamp();
		break;
	case TIMERID_ALIGNMENT:
		Sequence_Alignment();
		break;
	case TIMERID_PUNCH:
		Sequence_Punch();
		break;
	case TIMERID_EJECT:
		Sequence_Eject();
		break;
	case TIMERID_ROTATE:
		Sequence_Rotate();
		break;
	case TIMERID_EXIT:
		Sequence_Exit();
		break;

	case OFFTIMERID_ROLLER_CW:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_ROLLER_CW, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_ROLLER_CW);
	}
		break;
	case OFFTIMERID_ROLLER_CCW:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_ROLLER_CCW, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_ROLLER_CCW);
	}
		break;
	case OFFTIMERID_LSTP_B_DOWN:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_LSTP_B_DOWN, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_LSTP_B_DOWN);
	}
		break;
	case OFFTIMERID_LSTP_B_UP:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_LSTP_B_UP, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_LSTP_B_UP);
	}
		break;
	case OFFTIMERID_CENTER_OFF:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_CENTER_OFF, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_CENTER_OFF);
	}
		break;
	case OFFTIMERID_CENTER_ON:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_CENTER_ON, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_CENTER_ON);
	}
		break;
	case OFFTIMERID_PSTP_B_DOWN:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PSTP_B_DOWN, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_PSTP_B_DOWN);
	}
		break;
	case OFFTIMERID_PSTP_B_UP:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PSTP_B_UP, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_PSTP_B_UP);
	}
		break;
	case OFFTIMERID_LSTP_A_DOWN:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_LSTP_A_DOWN, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_LSTP_A_DOWN);
	}
		break;
	case OFFTIMERID_LSTP_A_UP:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_LSTP_A_UP, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_LSTP_A_UP);
	}
		break;
	case OFFTIMERID_PSTP_A_DOWN:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PSTP_A_DOWN, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_PSTP_A_DOWN);
	}
		break;
	case OFFTIMERID_PSTP_A_UP:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PSTP_A_UP, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_PSTP_A_UP);
	}
		break;
	case OFFTIMERID_PL_CLAMP:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PL_CLAMP, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_PL_CLAMP);
	}
		break;
	case OFFTIMERID_PL_ROTATE:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PL_ROTATE, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_PL_ROTATE);
	}
		break;
	case OFFTIMERID_PUNCH_ON:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PUNCH_ON, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_PUNCH_ON);
	}
		break;
	case OFFTIMERID_PUNCH_OFF:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PUNCH_OFF, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_PUNCH_OFF);
	}
		break;
	case OFFTIMERID_ROLLER_UP:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_ROLLER_UP, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_ROLLER_UP);
	}
		break;
	case OFFTIMERID_CAMERA_DOWN:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_CAMERA_DOWN, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_CAMERA_DOWN);
	}
		break;
	case OFFTIMERID_ROLLER_DOWN:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_ROLLER_DOWN, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_ROLLER_DOWN);
	}
		break;
	case OFFTIMERID_CAMERA_UP:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_CAMERA_UP, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_CAMERA_UP);
	}
		break;
	case OFFTIMERID_VACUUM_OFF:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_VACUUM_OFF, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_VACUUM_OFF);
	}
		break;
	case OFFTIMERID_VACUUM_ON:
	{
		Enmf_FUNC_RESULT eRet;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_VACUUM_ON, 0);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) KillTimer(OFFTIMERID_VACUUM_ON);
	}
		break;

	case TIMER_ALARM:
		if (m_iToggleAlarmIcon == 0)
		{
			m_iToggleAlarmIcon = 1;
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS))->SetIcon(m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_ALARM1);
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS))->Invalidate(FALSE);
		}
		else
		{
			m_iToggleAlarmIcon = 0;
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS))->SetIcon(m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_ALARM2);
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS))->Invalidate(FALSE);
		}
		break;

	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CEquipmentDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT nID = pWnd->GetDlgCtrlID();

	switch (nID) {
	default:
		//pDC->SetBkColor(RGB(255, 255, 255));
		break;
	}


	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CEquipmentDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDCtl)
	{
	case IDC_BUTTON_EQUIPMENT_AUTO:
	{
		CDC dc;
		RECT rect;
		CBrush cbr;
		dc.Attach(lpDrawItemStruct->hDC);
		rect = lpDrawItemStruct->rcItem;
		if (m_iSystemMode == MODE_AUTO)
		{
			cbr.CreateSolidBrush(RGB(204, 102, 102));
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(RGB(204, 102, 102));
		}
		else
		{
			cbr.CreateSolidBrush(GetSysColor(COLOR_3DFACE));
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(GetSysColor(COLOR_3DFACE));
		}


		// 검은색 Pen 객체를 생성한다. (바깥쪽 테두리에 사용)
		HPEN h_out_border_pen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		// 사각형 내부를 채우지 않도록 DC에 NULL_BRUSH를 설정한다.
		HGDIOBJ h_out_border_brush = ::SelectObject(dc, ::GetStockObject(NULL_BRUSH));
		// 바깥쪽 테두리를 만들기 위해서 검은색 Pen 객체를 DC에 연결한다.
		HGDIOBJ h_out_pen = ::SelectObject(dc, h_out_border_pen);
		// 내부가 채워지지 않는 사각형을 버튼 크기로 그린다.
		::Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
		// 이전에 사용하던 Pen 객체를 DC에 다시 연결한다.
		::SelectObject(dc, h_out_border_pen);
		// 검은색 Pen 객체를 제거한다.
		::DeleteObject(h_out_border_pen);
		// 테두리 브러시 해제
		::DeleteObject(h_out_border_brush);
		::DeleteObject(h_out_pen);

		// 버튼 컨트롤에서 Caption 정보를 복사한다.
		char name[12];
		int length = ::GetDlgItemText(this->m_hWnd, IDC_BUTTON_EQUIPMENT_AUTO, name, 12);
		// 버튼의 Caption 정보를 버튼 영역의 가운데에 출력한다.
		::DrawText(dc, name, length, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		cbr.DeleteObject();
		dc.DeleteDC();
	}
		break;
	case IDC_BUTTON_EQUIPMENT_MANUAL:
	{
		CDC dc;
		RECT rect;
		CBrush cbr;
		dc.Attach(lpDrawItemStruct->hDC);
		rect = lpDrawItemStruct->rcItem;
		if (m_iSystemMode == MODE_MANUAL)
		{
			cbr.CreateSolidBrush(RGB(204, 102, 102));
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(RGB(204, 102, 102));
		}
		else
		{
			cbr.CreateSolidBrush(GetSysColor(COLOR_3DFACE));
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(GetSysColor(COLOR_3DFACE));
		}


		// 검은색 Pen 객체를 생성한다. (바깥쪽 테두리에 사용)
		HPEN h_out_border_pen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		// 사각형 내부를 채우지 않도록 DC에 NULL_BRUSH를 설정한다.
		HGDIOBJ h_out_border_brush = ::SelectObject(dc, ::GetStockObject(NULL_BRUSH));
		// 바깥쪽 테두리를 만들기 위해서 검은색 Pen 객체를 DC에 연결한다.
		HGDIOBJ h_out_pen = ::SelectObject(dc, h_out_border_pen);
		// 내부가 채워지지 않는 사각형을 버튼 크기로 그린다.
		::Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
		// 이전에 사용하던 Pen 객체를 DC에 다시 연결한다.
		::SelectObject(dc, h_out_border_pen);
		// 검은색 Pen 객체를 제거한다.
		::DeleteObject(h_out_border_pen);
		// 테두리 브러시 해제
		::DeleteObject(h_out_border_brush);
		::DeleteObject(h_out_pen);

		// 버튼 컨트롤에서 Caption 정보를 복사한다.
		char name[12];
		int length = ::GetDlgItemText(this->m_hWnd, IDC_BUTTON_EQUIPMENT_MANUAL, name, 12);
		// 버튼의 Caption 정보를 버튼 영역의 가운데에 출력한다.
		::DrawText(dc, name, length, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		cbr.DeleteObject();
		dc.DeleteDC();
	}
		break;
	default:
		CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
		break;
	}
}


void CEquipmentDlg::OnBnClickedButtonEquipmentAuto()
{
	m_btnAuto.Invalidate(FALSE);
	m_btnManual.Invalidate(FALSE);
	AutoMode();
}

void CEquipmentDlg::OnBnClickedButtonEquipmentManual()
{
	m_btnAuto.Invalidate(FALSE);
	m_btnManual.Invalidate(FALSE);
	ManualMode();
}

void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentInitialize()
{
	Sequence_EnableWindow(FALSE);
	InitMode();
}

void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentStatusclear()
{
	m_EditStatus.SetSel(0, -1);
	m_EditStatus.Clear();

	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->m_EditStatus.SetSel(0, -1, TRUE);
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->m_EditStatus.Clear();

	//m_EditStatus.SetWindowText("");
	//m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->m_EditStatus.SetWindowText("");
	//m_EditStatus.Invalidate(FALSE);
	//m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->m_EditStatus.Invalidate(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentRollerCw()
{
	IO_Roller_CW();
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentRollerCcw()
{
	IO_Roller_CCW();
}



void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentRollerUp()
{
	IO_Roller_UpDown(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentRollerDown()
{
	IO_Roller_UpDown(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentCameraUp()
{
	IO_Camera_UpDown(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentCameraDown()
{
	IO_Camera_UpDown(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentCenter2On()
{
	IO_Center_OnOff(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentCenter1On()
{
	IO_Center_OnOff(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentCenter2Off()
{
	IO_Center_OnOff(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentCenter1Off()
{
	IO_Center_OnOff(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpA1Up()
{
	IO_PStpA_UpDown(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpA2Up()
{
	IO_PStpA_UpDown(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpA1Down()
{
	IO_PStpA_UpDown(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpA2Down()
{
	IO_PStpA_UpDown(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpB1Up()
{
	IO_PStpB_UpDown(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpB2Up()
{
	IO_PStpB_UpDown(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpB1Down()
{
	IO_PStpB_UpDown(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPstpB2Down()
{
	IO_PStpB_UpDown(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpA1Up()
{
	IO_LStpA_UpDown(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpA2Up()
{
	IO_LStpA_UpDown(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpA1Down()
{
	IO_LStpA_UpDown(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpA2Down()
{
	IO_LStpA_UpDown(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpB1Up()
{
	IO_LStpB_UpDown(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpB2Up()
{
	IO_LStpB_UpDown(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpB1Down()
{
	IO_LStpB_UpDown(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentLstpB2Down()
{
	IO_LStpB_UpDown(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPunch1On()
{
	IO_Punch_OnOff(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPunch2On()
{
	IO_Punch_OnOff(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPunch1Off()
{
	IO_Punch_OnOff(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPunch2Off()
{
	IO_Punch_OnOff(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPlClamp1Up()
{
	IO_PL_Clamp_UpDown(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPlClamp2Up()
{
	IO_PL_Clamp_UpDown(TRUE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPlClamp1Down()
{
	IO_PL_Clamp_UpDown(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPlClamp2Down()
{
	IO_PL_Clamp_UpDown(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentPlRotate()
{
	IO_PL_Rotate();
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentVacuum()
{
	IO_Vacuum_OnOff();
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequencePlatein()
{
	if (m_iSystemMode == MODE_AUTO) return;
	m_iTimeClock_Manual = TIMERCLOCK_PLATEIN;
	SetTimer(TIMERID_PLATEIN, m_iTimeInterval, FALSE);
	Sequence_EnableWindow(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceCentering()
{
	if (m_iSystemMode == MODE_AUTO) return;
	m_iTimeClock_Manual = TIMERCLOCK_CENTERING;
	SetTimer(TIMERID_CENTERING, m_iTimeInterval, FALSE);
	Sequence_EnableWindow(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceClamp()
{
	if (m_iSystemMode == MODE_AUTO) return;
	m_iTimeClock_Manual = TIMERCLOCK_CLAMP;
	SetTimer(TIMERID_CLAMP, m_iTimeInterval, FALSE);
	Sequence_EnableWindow(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceAlignment()
{
	if (m_iSystemMode == MODE_AUTO) return;
	m_iTimeClock_Manual = TIMERCLOCK_ALIGNMENT;
	SetTimer(TIMERID_ALIGNMENT, m_iTimeInterval, FALSE);
	Sequence_EnableWindow(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequencePunch()
{
	if (m_iSystemMode == MODE_AUTO) return;
	m_iTimeClock_Manual = TIMERCLOCK_PUNCH;
	SetTimer(TIMERID_PUNCH, m_iTimeInterval, FALSE);
	Sequence_EnableWindow(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceEject()
{
	if (m_iSystemMode == MODE_AUTO) return;
	m_iTimeClock_Manual = TIMERCLOCK_EJECT;
	SetTimer(TIMERID_EJECT, m_iTimeInterval, FALSE);
	Sequence_EnableWindow(FALSE);
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceRotate()
{
	if (m_iSystemMode == MODE_AUTO) return;
	m_iTimeClock_Manual = TIMERCLOCK_ROTATE;
	SetTimer(TIMERID_ROTATE, m_iTimeInterval, FALSE);
	Sequence_EnableWindow(FALSE);
}



void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceExit()
{
	if (m_iSystemMode == MODE_AUTO) return;
	m_iTimeClock_Manual = TIMERCLOCK_EXIT;
	SetTimer(TIMERID_EXIT, m_iTimeInterval, FALSE);
	Sequence_EnableWindow(FALSE);
}

void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSequenceAllstop()
{
	Sequence_KillTimer();
}


void CEquipmentDlg::OnBnClickedMfcbuttonEquipmentSetInterval()
{
	// Edit Contorl에서 값 얻어온다
	int iValue = GetDlgItemInt(IDC_EDIT_EQUIPMENT_INTERVAL);

	if (iValue <= 0)
	{
		AfxMessageBox("시퀀스의 시간간격이 0초가 될 수 없습니다.", MB_ICONERROR);
		SetDlgItemInt(IDC_EDIT_EQUIPMENT_INTERVAL, m_iTimeInterval);
		return;
	}

	m_iTimeInterval = iValue;

	// ini 파일에 저장
	CString str;
	str.Format("%d", iValue);
	WritePrivateProfileString("SEQUENCE", "INTERVAL", str.GetBuffer() , m_pPunchingMachineDlg->m_strPath_motion_ini);

	AfxMessageBox("정보가 저장되었습니다.", MB_ICONINFORMATION);
}


void CEquipmentDlg::OnBnClickedCheckEquipmentSkipSearchmark()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->m_Check_Skip_SearchMark.SetCheck(m_Check_Skip_SearchMark.GetCheck());
}

void CEquipmentDlg::OnBnClickedCheckEquipmentSkipPunch()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->m_Check_Skip_Punch.SetCheck(m_Check_Skip_Punch.GetCheck());
}



/*
	메서드
*/

void CEquipmentDlg::SetScreenRect(CRect rect)
{
	m_RectEquipmentDlg = rect;
}


BOOL CEquipmentDlg::Init()
{
	GetDlgItem(IDC_PICTURECONTROL_EQUIPMENT_BASEIMAGE)->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);

	for(int i = 0; i < 2; i++)
	{
		m_btnCenter_On[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_btnCenter_Off[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_btnPStpA_Up[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_btnPStpA_Down[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_btnPStpB_Up[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_btnPStpB_Down[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_btnPLClamp_Up[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_btnPLClamp_Down[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_btnPunch_On[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_btnPunch_Off[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_btnLStpA_Up[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_btnLStpA_Down[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_btnLStpB_Up[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		m_btnLStpB_Down[i].ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);

		m_btnCenter_On[i].BringWindowToTop();
		m_btnCenter_Off[i].BringWindowToTop();
		m_btnPStpA_Up[i].BringWindowToTop();
		m_btnPStpA_Down[i].BringWindowToTop();
		m_btnPStpB_Up[i].BringWindowToTop();
		m_btnPStpB_Down[i].BringWindowToTop();
		m_btnPLClamp_Up[i].BringWindowToTop();
		m_btnPLClamp_Down[i].BringWindowToTop();
		m_btnPunch_On[i].BringWindowToTop();
		m_btnPunch_Off[i].BringWindowToTop();
		m_btnLStpA_Up[i].BringWindowToTop();
		m_btnLStpA_Down[i].BringWindowToTop();
		m_btnLStpB_Up[i].BringWindowToTop();
		m_btnLStpB_Down[i].BringWindowToTop();
	}

	m_btnRollerUp.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_btnRollerDown.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_btnRollerCW.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_btnRollerCCW.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_btnPLRotate.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_btnVacuum.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_btnCamera_Up.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_btnCamera_Down.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);

	m_btnRollerUp.BringWindowToTop();
	m_btnRollerDown.BringWindowToTop();
	m_btnRollerCW.BringWindowToTop();
	m_btnRollerCCW.BringWindowToTop();
	m_btnPLRotate.BringWindowToTop();
	m_btnVacuum.BringWindowToTop();
	m_btnCamera_Up.BringWindowToTop();
	m_btnCamera_Down.BringWindowToTop();


	m_StaticAxis0_MLimit.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_StaticAxis0_PLimit.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_StaticAxis1_MLimit.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_StaticAxis1_PLimit.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_StaticAxis2_MLimit.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_StaticAxis2_PLimit.ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);

	m_StaticAxis0_MLimit.BringWindowToTop();
	m_StaticAxis0_PLimit.BringWindowToTop();
	m_StaticAxis1_MLimit.BringWindowToTop();
	m_StaticAxis1_PLimit.BringWindowToTop();
	m_StaticAxis2_MLimit.BringWindowToTop();
	m_StaticAxis2_PLimit.BringWindowToTop();

	m_StaticAxis0_MLimit.SetBkColor(clSilver);
	m_StaticAxis0_PLimit.SetBkColor(clSilver);
	m_StaticAxis1_MLimit.SetBkColor(clSilver);
	m_StaticAxis1_PLimit.SetBkColor(clSilver);
	m_StaticAxis2_MLimit.SetBkColor(clSilver);
	m_StaticAxis2_PLimit.SetBkColor(clSilver);



	m_fontMode.CreateFont(
		20,                            // 글자높이
		10,                            // 글자너비
		0,                            // 출력각도
		0,                            // 기준 선에서의각도
		FW_BOLD,						// 글자굵기
		FALSE,                        // Italic 적용여부
		FALSE,                        // 밑줄적용여부
		FALSE,                        // 취소선적용여부
		DEFAULT_CHARSET,            // 문자셋종류
		OUT_DEFAULT_PRECIS,            // 출력정밀도
		CLIP_CHARACTER_PRECIS,        // 클리핑정밀도
		PROOF_QUALITY,                // 출력문자품질
		DEFAULT_PITCH,                // 글꼴Pitch
		_T("돋움체")                // 글꼴
	);

	m_fontStatus.CreateFont(
		16,                            // 글자높이
		8,                            // 글자너비
		0,                            // 출력각도
		0,                            // 기준 선에서의각도
		FW_BOLD,						// 글자굵기
		FALSE,                        // Italic 적용여부
		FALSE,                        // 밑줄적용여부
		FALSE,                        // 취소선적용여부
		DEFAULT_CHARSET,            // 문자셋종류
		OUT_DEFAULT_PRECIS,            // 출력정밀도
		CLIP_CHARACTER_PRECIS,        // 클리핑정밀도
		PROOF_QUALITY,                // 출력문자품질
		DEFAULT_PITCH,                // 글꼴Pitch
		_T("돋움체")                // 글꼴
	);

	m_btnAuto.SetFont(&m_fontMode, FALSE);
	m_btnManual.SetFont(&m_fontMode, FALSE);
	//m_btnInitialize.SetFont(&m_fontMode, FALSE);
	m_EditStatus.SetFont(&m_fontStatus, FALSE);

	m_pPunchingMachineDlg = (CPunchingMachineDlg*)AfxGetMainWnd();


	// ini 파일 정보 읽어온다
	char cArrBuf[256] = { 0, };
	GetPrivateProfileString("EQUIPMENT", "PULSE_PER_MM_AXIS0", 0, cArrBuf, 256, m_pPunchingMachineDlg->m_strPath_motion_ini);
	m_iPulsePerMM_Axis0 = atoi(cArrBuf);
	GetPrivateProfileString("EQUIPMENT", "PULSE_PER_MM_AXIS1", 0, cArrBuf, 256, m_pPunchingMachineDlg->m_strPath_motion_ini);
	m_iPulsePerMM_Axis1 = atoi(cArrBuf);
	GetPrivateProfileString("EQUIPMENT", "PULSE_PER_MM_AXIS2", 0, cArrBuf, 256, m_pPunchingMachineDlg->m_strPath_motion_ini);
	m_iPulsePerMM_Axis2 = atoi(cArrBuf);
	GetPrivateProfileString("SEQUENCE", "INTERVAL", "1000", cArrBuf, 256, m_pPunchingMachineDlg->m_strPath_motion_ini);
	m_iTimeInterval = atoi(cArrBuf);
	SetDlgItemInt(IDC_EDIT_EQUIPMENT_INTERVAL, m_iTimeInterval);
	GetPrivateProfileString("ALIGNMENT", "maxSearchCount", "5", cArrBuf, 256, m_pPunchingMachineDlg->m_strPath_camera_ini);
	m_iCheckMaxNum_SearchMark = atoi(cArrBuf);
	GetPrivateProfileString("PUNCH", "CurrentNumber", "0", cArrBuf, 256, m_pPunchingMachineDlg->m_strPath_punchdata_ini);
	m_iCurrent_PunchNumber = atoi(cArrBuf);
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->
		GetDlgItem(IDC_EDIT_STATUSINFORMATION_CURRENT_NUMBEROFPLATES)->SetWindowText(cArrBuf);
	GetPrivateProfileString("PUNCH", "TotalNumber", "0", cArrBuf, 256, m_pPunchingMachineDlg->m_strPath_punchdata_ini);
	m_iTotal_PunchNumber = atoi(cArrBuf);
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->
		GetDlgItem(IDC_EDIT_STATUSINFORMATION_TOTAL_NUMBEROFPLATES)->SetWindowText(cArrBuf);

	return TRUE;
}

void CEquipmentDlg::DisplayStatus(DeviceStatus Status)
{
	CString strArrStatus[(unsigned int)(DeviceStatus::STATUS_LEN)] =
	{
		"초기화 시작\r\n",
		"초기화 종료\r\n",
		"펀치 고정\r\n",
		"판재 삽입\r\n",
		"판재 감지1\r\n",
		"판재 감지2\r\n",
		"롤러 CW 작동\r\n",
		"롤러 CCW 작동\r\n",
		"왼쪽 스토퍼 내림\r\n",
		"왼쪽 스토퍼 올림\r\n",
		"센터링 종료\r\n",
		"센터링 시작\r\n",
		"정면 스토퍼 내림\r\n",
		"정면 스토퍼 올림\r\n",
		"왼쪽 스토퍼 내림\r\n",
		"왼쪽 스토퍼 올림\r\n",
		"정면 스토퍼 내림\r\n",
		"정면 스토퍼 올림\r\n",
		"판재 고정\r\n",
		"판재 회전\r\n",
		"펀치 시작\r\n",
		"펀치 종료\r\n",
		"롤러 올림\r\n",
		"카메라 내림\r\n",
		"롤러 내림\r\n",
		"카메라 올림\r\n",
		"진공 끔\r\n",
		"진공 킴\r\n",
		"카메라1 검사 실패\r\n",
		"카메라2 검사 실패\r\n"
	};

	// 현재 시간 구한다
	CTime CurrentTime = CTime::GetCurrentTime();

	// 현재 시간 + 상태문구 합친다
	CString strTime_Status;
	strTime_Status.Format("%02d:%02d:%02d    %s",
		CurrentTime.GetHour(),
		CurrentTime.GetMinute(),
		CurrentTime.GetSecond(),
		strArrStatus[(unsigned int)Status].GetBuffer()
	);

	if (m_EditStatus.GetLineCount() > 100)
	{
		m_EditStatus.SetSel(0, 0);
		m_EditStatus.Clear();
		m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->m_EditStatus.SetSel(0, 0);
		m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->m_EditStatus.Clear();
	}

	// get the initial text length
	int iTotalLength = m_EditStatus.GetWindowTextLength();

	// put the selection at the end of text
	m_EditStatus.SetSel(iTotalLength, iTotalLength);

	// replace the selection
	m_EditStatus.ReplaceSel(strTime_Status);

	// put the selection at the end of text
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->m_EditStatus.SetSel(iTotalLength, iTotalLength);

	// replace the selection
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->m_EditStatus.ReplaceSel(strTime_Status);


	// 자동 로그 저장
	CString strLogFileName;

	strLogFileName.Format(".\\Log\\Log_%04d%02d%02d.txt",
		CurrentTime.GetYear(),
		CurrentTime.GetMonth(),
		CurrentTime.GetDay()
	);

	if (m_fpStatusLog.Open(strLogFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite, NULL))
	{
		m_fpStatusLog.SeekToEnd();
		m_fpStatusLog.WriteString(strTime_Status);
		m_fpStatusLog.Close();
	}
	
}

void CEquipmentDlg::InitMode()
{
	Sequence_KillTimer();
	//KillTimer(TIMERID_AUTOSEQUENCE);
	SetTimer(TIMERID_INITIALIZE, m_iTimeInterval, FALSE);
	m_iTimeClock_Manual = TIMERCLOCK_INITIALIZE;
	m_iTimeClock_Auto = TIMERCLOCK_INITIALIZE;
}

void CEquipmentDlg::AutoMode()
{
	Sequence_KillTimer();
	//KillTimer(TIMERID_INITIALIZE);
	SetTimer(TIMERID_AUTOSEQUENCE, m_iTimeInterval, FALSE);
	//m_iTimeClock_Auto= TIMERCLOCK_INITIALIZE;
	m_iSystemMode = MODE_AUTO;

	if (!m_bCheckAlarmStatus)
	{
		((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
			GetDlgItem(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS))->SetIcon(m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_START);
		((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
			GetDlgItem(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS))->Invalidate(FALSE);
	}
}

void CEquipmentDlg::ManualMode()
{
	Sequence_KillTimer();
	//KillTimer(TIMERID_AUTOSEQUENCE);
	m_iSystemMode = MODE_MANUAL;

	if (!m_bCheckAlarmStatus)
	{
		((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
			GetDlgItem(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS))->SetIcon(m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_STOP);
		((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
			GetDlgItem(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS))->Invalidate(FALSE);
	}
}

void CEquipmentDlg::OffBypass()
{
	m_iBypassMode = BYPASS_OFF;
}

void CEquipmentDlg::OnBypass()
{
	m_iBypassMode = BYPASS_ON;
}

void CEquipmentDlg::SetIOImage()
{
	for (int i = 0; i < 1; i++)
	{
		m_btnCenter_On[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_CENTER_ON]));
		m_btnCenter_Off[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_CENTER_OFF]));
		m_btnPStpA_Up[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_PSTP_A_UP]));
		m_btnPStpA_Down[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_PSTP_A_DOWN]));
		m_btnPStpB_Up[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_PSTP_B_UP]));
		m_btnPStpB_Down[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_PSTP_B_DOWN]));
		m_btnPLClamp_Up[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_PL_CLAMP_UP]));
		m_btnPLClamp_Down[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_PL_CLAMP_DOWN]));
		m_btnPunch_On[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_PUNCH_ON]));
		m_btnPunch_Off[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_PUNCH_OFF]));
		m_btnLStpA_Up[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_LSTP_A_UP]));
		m_btnLStpA_Down[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_LSTP_A_DOWN]));
		m_btnLStpB_Up[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_LSTP_B_UP]));
		m_btnLStpB_Down[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_LSTP_B_DOWN]));
	}

	m_btnRollerUp.SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDO_PinStatus[IO_OUT_ROLLER_UP]));
	m_btnRollerDown.SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDO_PinStatus[IO_OUT_ROLLER_DOWN]));
	m_btnRollerCW.SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDO_PinStatus[IO_OUT_ROLLER_CW]));
	m_btnRollerCCW.SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDO_PinStatus[IO_OUT_ROLLER_CCW]));
	//m_btnPLRotate.SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDO_PinStatus[IO_OUT_PL_ROTATE]));
	//m_btnVacuum.SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDO_PinStatus[IO_OUT_VACUUM_ON]));
	m_btnCamera_Up.SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_CAMERA_UP]));
	m_btnCamera_Down.SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_iArrDI_PinStatus[IO_IN_CAMERA_DOWN]));

	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_EQUIPMENT_PUNCH_IN))->SetIcon(AfxGetApp()->LoadIcon(IDI_RED_OFF + m_iArrDI_PinStatus[IO_IN_PUNCH_IN]));
	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_EQUIPMENT_PLATE_IN))->SetIcon(AfxGetApp()->LoadIcon(IDI_RED_OFF + m_iArrDI_PinStatus[IO_IN_PLATE_IN]));
	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_EQUIPMENT_PLATE_ON_A))->SetIcon(AfxGetApp()->LoadIcon(IDI_RED_OFF + m_iArrDI_PinStatus[IO_IN_PLATE_A_ON]));
	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_EQUIPMENT_PLATE_ON_B))->SetIcon(AfxGetApp()->LoadIcon(IDI_RED_OFF + m_iArrDI_PinStatus[IO_IN_PLATE_B_ON]));
	
	for (int i = 0; i < 1; i++)
	{
		m_btnCenter_On[i].Invalidate(FALSE);
		m_btnCenter_Off[i].Invalidate(FALSE);
		m_btnPStpA_Up[i].Invalidate(FALSE);
		m_btnPStpA_Down[i].Invalidate(FALSE);
		m_btnPStpB_Up[i].Invalidate(FALSE);
		m_btnPStpB_Down[i].Invalidate(FALSE);
		m_btnPLClamp_Up[i].Invalidate(FALSE);
		m_btnPLClamp_Down[i].Invalidate(FALSE);
		m_btnPunch_On[i].Invalidate(FALSE);
		m_btnPunch_Off[i].Invalidate(FALSE);
		m_btnLStpA_Up[i].Invalidate(FALSE);
		m_btnLStpA_Down[i].Invalidate(FALSE);
		m_btnLStpB_Up[i].Invalidate(FALSE);
		m_btnLStpB_Down[i].Invalidate(FALSE);
	}

	m_btnRollerUp.Invalidate(FALSE);
	m_btnRollerDown.Invalidate(FALSE);
	m_btnRollerCW.Invalidate(FALSE);
	m_btnRollerCCW.Invalidate(FALSE);
	//m_btnPLRotate.Invalidate(FALSE);
	//m_btnVacuum.Invalidate(FALSE);
	m_btnCamera_Up.Invalidate(FALSE);
	m_btnCamera_Down.Invalidate(FALSE);

	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_EQUIPMENT_PUNCH_IN))->Invalidate(FALSE);
	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_EQUIPMENT_PLATE_IN))->Invalidate(FALSE);
	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_EQUIPMENT_PLATE_ON_A))->Invalidate(FALSE);
	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_EQUIPMENT_PLATE_ON_B))->Invalidate(FALSE);
}


// 장비 IO 구동
void CEquipmentDlg::IO_Roller_CW()
{
	if (m_iSystemMode == MODE_AUTO) return;



	Enmf_FUNC_RESULT eRet;

	if (m_iArrDO_PinStatus[IO_OUT_ROLLER_CW])
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_ROLLER_CW, 0);
	}
	else
	{
		if (m_iArrDO_PinStatus[IO_OUT_ROLLER_CCW])
		{
			AfxMessageBox("롤러가 동작중입니다!", MB_ICONSTOP);
			return;
		}
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_ROLLER_CW, 1);
	}

}

void CEquipmentDlg::IO_Roller_CCW()
{
	if (m_iSystemMode == MODE_AUTO) return;

	Enmf_FUNC_RESULT eRet;

	if (m_iArrDO_PinStatus[IO_OUT_ROLLER_CCW])
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_ROLLER_CCW, 0);
	}
	else
	{
		if (m_iArrDO_PinStatus[IO_OUT_ROLLER_CW])
		{
			AfxMessageBox("롤러가 동작중입니다!", MB_ICONSTOP);
			return;
		}

		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_ROLLER_CCW, 1);
	}

}

void CEquipmentDlg::IO_LStpB_UpDown(BOOL bValue)
{
	if (m_iSystemMode == MODE_AUTO) return;

	Enmf_FUNC_RESULT eRet;

	// Up
	if (bValue)
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_LSTP_B_UP, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_LSTP_B_UP, OFFIOTIMERCLOCK, FALSE);
	}
	// Down
	else
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_LSTP_B_DOWN, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_LSTP_B_DOWN, OFFIOTIMERCLOCK, FALSE);
	}
}

void CEquipmentDlg::IO_Center_OnOff(BOOL bValue)
{
	if (m_iSystemMode == MODE_AUTO) return;

	Enmf_FUNC_RESULT eRet;

	// On
	if (bValue)
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_CENTER_ON, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_CENTER_ON, OFFIOTIMERCLOCK, FALSE);
	}
	// Off
	else
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_CENTER_OFF, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_CENTER_OFF, OFFIOTIMERCLOCK, FALSE);
	}
}

void CEquipmentDlg::IO_PStpB_UpDown(BOOL bValue)
{
	if (m_iSystemMode == MODE_AUTO) return;

	Enmf_FUNC_RESULT eRet;

	// Up
	if (bValue)
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PSTP_B_UP, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_PSTP_B_UP, OFFIOTIMERCLOCK, FALSE);
	}
	// Down
	else
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PSTP_B_DOWN, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_PSTP_B_DOWN, OFFIOTIMERCLOCK, FALSE);
	}
}

void CEquipmentDlg::IO_LStpA_UpDown(BOOL bValue)
{
	if (m_iSystemMode == MODE_AUTO) return;

	Enmf_FUNC_RESULT eRet;

	// Up
	if (bValue)
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_LSTP_A_UP, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_LSTP_A_UP, OFFIOTIMERCLOCK, FALSE);
	}
	// Down
	else
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_LSTP_A_DOWN, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_LSTP_A_DOWN, OFFIOTIMERCLOCK, FALSE);
	}
}

void CEquipmentDlg::IO_PStpA_UpDown(BOOL bValue)
{
	if (m_iSystemMode == MODE_AUTO) return;

	Enmf_FUNC_RESULT eRet;

	// Up
	if (bValue)
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PSTP_A_UP, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_PSTP_A_UP, OFFIOTIMERCLOCK, FALSE);
	}
	// Down
	else
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PSTP_A_DOWN, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_PSTP_A_DOWN, OFFIOTIMERCLOCK, FALSE);
	}
}

void CEquipmentDlg::IO_PL_Clamp_UpDown(BOOL bValue)
{
	if (m_iSystemMode == MODE_AUTO) return;

	Enmf_FUNC_RESULT eRet;

	// Up
	if (bValue)
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PL_CLAMP, 0);
	}
	// Down
	else
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PL_CLAMP, 1);
	}
}

void CEquipmentDlg::IO_Punch_OnOff(BOOL bValue)
{
	if (m_iSystemMode == MODE_AUTO) return;

	Enmf_FUNC_RESULT eRet;

	// Off
	if (bValue)
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PUNCH_OFF, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_PUNCH_OFF, OFFIOTIMERCLOCK, FALSE);
	}
	// On
	else
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PUNCH_ON, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_PUNCH_ON, OFFIOTIMERCLOCK, FALSE);
	}
}

void CEquipmentDlg::IO_Roller_UpDown(BOOL bValue)
{
	if (m_iSystemMode == MODE_AUTO) return;

	Enmf_FUNC_RESULT eRet;

	// Up
	if (bValue)
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_ROLLER_UP, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_ROLLER_UP, OFFIOTIMERCLOCK, FALSE);
	}
	// Down
	else
	{
		// Camera Down >> Roller Up
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_ROLLER_DOWN, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_ROLLER_DOWN, OFFIOTIMERCLOCK, FALSE);
	}
}

void CEquipmentDlg::IO_Camera_UpDown(BOOL bValue)
{
	if (m_iSystemMode == MODE_AUTO) return;

	Enmf_FUNC_RESULT eRet;

	// Up
	if (bValue)
	{
		// Roller Down >> Camera Up
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_CAMERA_UP, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_CAMERA_UP, OFFIOTIMERCLOCK, FALSE);
	}
	// Down
	else
	{
		// Camera Up >> Roller Down
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_CAMERA_DOWN, 1);
		if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_CAMERA_DOWN, OFFIOTIMERCLOCK, FALSE);
	}
}

void CEquipmentDlg::IO_PL_Rotate()
{
	if (m_iSystemMode == MODE_AUTO) return;

	Enmf_FUNC_RESULT eRet;

	if (m_iArrDO_PinStatus[IO_OUT_PL_ROTATE])
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PL_ROTATE, 0);
	else
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_PL_ROTATE, 1);
}

void CEquipmentDlg::IO_Vacuum_OnOff()
{
	if (m_iSystemMode == MODE_AUTO) return;

	Enmf_FUNC_RESULT eRet;

	//if (m_iArrDI_PinStatus[IO_IN_VACUUM_ON])
	if (m_iArrDO_PinStatus[IO_OUT_VACUUM_ON])
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_VACUUM_ON, 0);
		//if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_VACUUM_OFF, OFFIOTIMERCLOCK, FALSE);
	}
	else
	{
		eRet = (EResult)nmf_DOSetPin(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, IO_OUT_VACUUM_ON, 1);
		//if (eRet == (Enmf_FUNC_RESULT)nmf_R_OK) SetTimer(OFFTIMERID_VACUUM_ON, OFFIOTIMERCLOCK, FALSE);
	}
}


// 장비 모션구동
void CEquipmentDlg::Homing()
{
	short nHomeMode, nEndMode;
	long loffset;
	short nret;

	// 0축 (RY)
	nHomeMode = ((CComboBox*)m_pPunchingMachineDlg->GetPtrMotionDlg()->GetDlgItem(IDC_CB_HOME1))->GetCurSel();
	nEndMode = ((CComboBox*)m_pPunchingMachineDlg->GetPtrMotionDlg()->GetDlgItem(IDC_CB_CLEAR1))->GetCurSel();
	loffset = m_pPunchingMachineDlg->GetPtrMotionDlg()->GetDlgItemInt(IDC_EDIT_OFFSET1);
	nret= nmc_HomeMove(m_pPunchingMachineDlg->GetPtrMotionDlg()->GetPaixCtlNo(), 0, nHomeMode, nEndMode, loffset, 0);

	// 1축 (X)
	nHomeMode = ((CComboBox*)m_pPunchingMachineDlg->GetPtrMotionDlg()->GetDlgItem(IDC_CB_HOME2))->GetCurSel();
	nEndMode = ((CComboBox*)m_pPunchingMachineDlg->GetPtrMotionDlg()->GetDlgItem(IDC_CB_CLEAR2))->GetCurSel();
	loffset = m_pPunchingMachineDlg->GetPtrMotionDlg()->GetDlgItemInt(IDC_EDIT_OFFSET2);
	nret = nmc_HomeMove(m_pPunchingMachineDlg->GetPtrMotionDlg()->GetPaixCtlNo(), 1, nHomeMode, nEndMode, loffset, 0);

	// 2축 (LY)
	nHomeMode = ((CComboBox*)m_pPunchingMachineDlg->GetPtrMotionDlg()->GetDlgItem(IDC_CB_HOME3))->GetCurSel();
	nEndMode = ((CComboBox*)m_pPunchingMachineDlg->GetPtrMotionDlg()->GetDlgItem(IDC_CB_CLEAR3))->GetCurSel();
	loffset = m_pPunchingMachineDlg->GetPtrMotionDlg()->GetDlgItemInt(IDC_EDIT_OFFSET3);
	nret = nmc_HomeMove(m_pPunchingMachineDlg->GetPtrMotionDlg()->GetPaixCtlNo(), 2, nHomeMode, nEndMode, loffset, 0);

}

// 장비 초기 원점 이동
void CEquipmentDlg::MoveInitPoint()
{
	short nRet;
	short nAxisCount, nAxisNo[8] = { 0, };
	double dPulsePos[8] = { 0, };
	nAxisCount = 3;
	nAxisNo[0] = 0; nAxisNo[1] = 1; nAxisNo[2] = 2;
	dPulsePos[0] = 0; dPulsePos[1] = 0; dPulsePos[2] = 0;
	nRet = nmc_VarAbsMove(m_pPunchingMachineDlg->GetPtrMotionDlg()->GetPaixCtlNo(), nAxisCount, nAxisNo, dPulsePos);
}

void CEquipmentDlg::Alignment_Move()
{
	m_pThreadAlignment_Move = AfxBeginThread(CEquipmentDlg::ThreadAlignment_Move, this);
	if (!m_pThreadAlignment_Move) { AfxMessageBox(_T("CreatingThread failed!")); return; }
}


// 시퀀스 기능
void CEquipmentDlg::Sequence_Initialize()
{
	Enmf_FUNC_RESULT eRet = Enmf_FUNC_RESULT::nmf_R_NOT_CONNECT;

	switch (m_iTimeClock_Manual)
	{
	case TIMERCLOCK_INITIALIZE:
		Homing();
		for (int i = 0; i < MAX_DIO_PINS; i++)
		{
			m_iArrSet_PinStatus[i] = 0;
		}
		m_iArrSet_PinStatus[IO_OUT_LSTP_B_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_CENTER_OFF] = 1;
		m_iArrSet_PinStatus[IO_OUT_PSTP_B_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_ROLLER_UP] = 1;
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_CAMERA_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_VACUUM_OFF] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
			DisplayStatus(DeviceStatus::STATUS_INITIALIZE_START);
		}
		else Sequence_ErrorHandler(TIMERID_INITIALIZE, eRet);
		break;
	case TIMERCLOCK_INITIALIZE + 1:
		m_iTimeClock_Manual++;
		break;
	case TIMERCLOCK_INITIALIZE + 2:
		m_iArrSet_PinStatus[IO_OUT_LSTP_B_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_CENTER_OFF] = 0;
		m_iArrSet_PinStatus[IO_OUT_PSTP_B_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_ROLLER_UP] = 0;
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_CAMERA_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_VACUUM_OFF] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
			DisplayStatus(DeviceStatus::STATUS_INITIALIZE_START);
		}
		else Sequence_ErrorHandler(TIMERID_INITIALIZE, eRet);
		break;
	default:
		KillTimer(TIMERID_INITIALIZE);
		m_iTimeClock_Manual = TIMERCLOCK_INITIALIZE;
		DisplayStatus(DeviceStatus::STATUS_INITIALIZE_END);
		Sequence_EnableWindow(TRUE);
		break;
	}
}

void CEquipmentDlg::Sequence_Auto()
{
	if (m_iSystemMode == MODE_MANUAL)
	{
		KillTimer(TIMERID_AUTOSEQUENCE);
		m_iTimeClock_Auto = TIMERCLOCK_INITIALIZE;
		return;
	}

	Enmf_FUNC_RESULT eRet = Enmf_FUNC_RESULT::nmf_R_NOT_CONNECT;

	switch (m_iTimeClock_Auto)
	{
		// Initialize
	case TIMERCLOCK_INITIALIZE:
		Homing();
		for (int i = 0; i < MAX_DIO_PINS; i++)
		{
			m_iArrSet_PinStatus[i] = 0;
		}
		m_iArrSet_PinStatus[IO_OUT_LSTP_B_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_CENTER_OFF] = 1;
		m_iArrSet_PinStatus[IO_OUT_PSTP_B_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_ROLLER_UP] = 1;
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_CAMERA_DOWN] = 1;
		//m_iArrSet_PinStatus[IO_OUT_VACUUM_OFF] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
			DisplayStatus(DeviceStatus::STATUS_INITIALIZE_START);
		}
		else Sequence_ErrorHandler(TIMERID_INITIALIZE, eRet);
		break;
	case TIMERCLOCK_INITIALIZE + 1:
		m_iTimeClock_Auto++;
		break;
	case TIMERCLOCK_INITIALIZE + 2:
		m_iArrSet_PinStatus[IO_OUT_LSTP_B_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_CENTER_OFF] = 0;
		m_iArrSet_PinStatus[IO_OUT_PSTP_B_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_ROLLER_UP] = 0;
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_CAMERA_DOWN] = 0;
		//m_iArrSet_PinStatus[IO_OUT_VACUUM_OFF] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto = TIMERCLOCK_PLATEIN;
			DisplayStatus(DeviceStatus::STATUS_INITIALIZE_START);
		}
		else Sequence_ErrorHandler(TIMERID_INITIALIZE, eRet);
		break;

		// Plate In
	case TIMERCLOCK_PLATEIN:
		//printf("Plate In\n");
		Homing();
		m_iTimeClock_Auto++;
		break;
	case TIMERCLOCK_PLATEIN + 1:
		if (m_iArrDI_PinStatus[IO_IN_PLATE_IN])
		{
			//MoveInitPoint();
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->SetIcon(NULL);
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->Invalidate(FALSE);
			for (int i = 0; i < 2; i++)
			{
				((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[i].GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->SetIcon(NULL);
				((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[i].GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->Invalidate(FALSE);
			}
			m_iTimeClock_Auto++;
		}
		break;
	case TIMERCLOCK_PLATEIN + 2:
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CW] = 1;
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_UP] = !m_iBypassMode;
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_UP] = !m_iBypassMode;
		//m_iArrSet_PinStatus[IO_OUT_LSTP_A_UP] = 1;
		//m_iArrSet_PinStatus[IO_OUT_PSTP_A_UP] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
			DisplayStatus(DeviceStatus::STATUS_OUT_ROLLER_CW);
			DisplayStatus(DeviceStatus::STATUS_OUT_LSTP_B_UP);
			DisplayStatus(DeviceStatus::STATUS_OUT_PSTP_B_UP);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_PLATEIN + 3:
		m_iTimeClock_Auto++;
		break;
	case TIMERCLOCK_PLATEIN + 4:
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_UP] = 0;
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_UP] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_PLATEIN + 5:
		if (m_iArrDI_PinStatus[IO_IN_PLATE_A_ON])
		{
			m_iBypassMode ?
				m_iTimeClock_Auto = TIMERCLOCK_EXIT : m_iTimeClock_Auto++;
		}
		break;
	case TIMERCLOCK_PLATEIN + 6:
		m_iTimeClock_Auto++;
		break;
	case TIMERCLOCK_PLATEIN + 7:
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CW] = 0;
		m_iArrSet_PinStatus[IO_OUT_CAMERA_UP] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto = TIMERCLOCK_CENTERING;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;

		// Centering
	case TIMERCLOCK_CENTERING:
		m_iArrSet_PinStatus[IO_OUT_CENTER_ON] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
			DisplayStatus(DeviceStatus::STATUS_OUT_CENTER_ON);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_CENTERING + 1:
		m_iTimeClock_Auto++;
		break;
	case TIMERCLOCK_CENTERING + 2:
		m_iArrSet_PinStatus[IO_OUT_CENTER_OFF] = 1;
		m_iArrSet_PinStatus[IO_OUT_CENTER_ON] = 0;
		m_iArrSet_PinStatus[IO_OUT_CAMERA_UP] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
			DisplayStatus(DeviceStatus::STATUS_OUT_CENTER_OFF);
			DisplayStatus(DeviceStatus::STATUS_OUT_CAMERA_UP);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_CENTERING + 3:
		m_iTimeClock_Auto++;
		break;
	case TIMERCLOCK_CENTERING + 4:
		m_iArrSet_PinStatus[IO_OUT_CENTER_OFF] = 0;
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CCW] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_CENTERING + 5:
		m_iTimeClock_Auto = TIMERCLOCK_CLAMP;
		break;

		// Clamp
	case TIMERCLOCK_CLAMP:
		if (m_iArrDI_PinStatus[IO_IN_PUNCH_IN])
		{
			m_iTimeClock_Auto++;
		}
		break;
	case TIMERCLOCK_CLAMP + 1:
			m_iTimeClock_Auto++;
		break;
	case TIMERCLOCK_CLAMP + 2:
		m_iArrSet_PinStatus[IO_OUT_PL_CLAMP] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
			DisplayStatus(DeviceStatus::STATUS_OUT_PL_CLAMP);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_CLAMP + 3:
		m_iTimeClock_Auto++;
		break;
	case TIMERCLOCK_CLAMP + 4:
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CCW] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_CLAMP + 5:
		m_iTimeClock_Auto = TIMERCLOCK_ALIGNMENT;
		break;

		// Alignment
	case TIMERCLOCK_ALIGNMENT:
		if (m_Check_Skip_SearchMark.GetCheck())
		{
			m_iTimeClock_Auto = TIMERCLOCK_PUNCH;
		}
		else
		{
			m_iCheckCurNum_SearchMark++;
			if (m_iCheckCurNum_SearchMark > m_iCheckMaxNum_SearchMark)
			{
				// Alignment 상태표시
				((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
					GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->SetIcon(m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_NG);
				((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
					GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->Invalidate(FALSE);

				DisplayStatus(DeviceStatus::STATUS_CAMERA1_SEARCHMARK_ERROR);
				AlarmON();
				m_pPunchingMachineDlg->CaptureScreen();
				m_iCheckCurNum_SearchMark = 0;
				m_iTimeClock_Auto = TIMERCLOCK_EJECT;
				break;
			}

			// Camera 1, 2  SearchMark
			for (int i = 0; i < 2; i++)
			{
				m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[i].m_iCheckAlignment_SearchMark = 0;
				m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[i].Alignment_SearchMark();
			}
			m_iTimeClock_Auto++;
		}
		break;
	case TIMERCLOCK_ALIGNMENT + 1:
		// 두 카메라중 하나라도 SearchMark 실패한 경우
		if (m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].m_iCheckAlignment_SearchMark == -1)
		{
			// Alignment 상태표시
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->SetIcon(m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_NG);
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->Invalidate(FALSE);

			DisplayStatus(DeviceStatus::STATUS_CAMERA1_SEARCHMARK_ERROR);

			AlarmON();

			m_pPunchingMachineDlg->CaptureScreen();

			m_iTimeClock_Auto = TIMERCLOCK_EJECT;
		}

		if (m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].m_iCheckAlignment_SearchMark == -1)
		{
			// Alignment 상태표시
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->SetIcon(m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_NG);
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->Invalidate(FALSE);

			DisplayStatus(DeviceStatus::STATUS_CAMERA2_SEARCHMARK_ERROR);

			AlarmON();

			m_pPunchingMachineDlg->CaptureScreen();

			m_iTimeClock_Auto = TIMERCLOCK_EJECT;
		}

		// 두 카메라의 SearchMark 로직이 끝나면 장비이동
		if (m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].m_iCheckAlignment_SearchMark == 1 &&
			m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].m_iCheckAlignment_SearchMark == 1)
		{
			// SearchMark 성공 시 다음루틴으로 넘어간다
			if (m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].m_iCheckOK_SearchMark == 1 &&
				m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].m_iCheckOK_SearchMark == 1)
			{
				m_iTimeClock_Auto++;
				break;
			}
			//printf("Test");

			m_iCheckAlignment_Move = 0;
			Alignment_Move();
			m_iTimeClock_Auto++;
		}
		break;
	case TIMERCLOCK_ALIGNMENT + 2:
		// 장비이동이 끝나면 Search Mark 검증
		if (m_iCheckAlignment_Move == 1)
		{
			// SearchMark 실패 시 다시 Alignment 로직 시작
			if (m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].m_iCheckOK_SearchMark == 0 ||
				m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].m_iCheckOK_SearchMark == 0)
			{
				m_iTimeClock_Auto = TIMERCLOCK_ALIGNMENT;
				break;
			}

			// Alignment 상태표시
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->SetIcon(m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_OK);
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->Invalidate(FALSE);

			m_iCheckCurNum_SearchMark = 0;
			m_iTimeClock_Auto = TIMERCLOCK_PUNCH;
		}
		break;

		// Punch
	case TIMERCLOCK_PUNCH:
		if (m_Check_Skip_Punch.GetCheck())
		{
			m_iTimeClock_Auto = TIMERCLOCK_EJECT;
		}
		else
		{
			m_iArrSet_PinStatus[IO_OUT_PUNCH_ON] = 1;
			eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
			if (eRet == nmf_R_OK)
			{
				m_iTimeClock_Auto++;
				DisplayStatus(DeviceStatus::STATUS_OUT_PUNCH_ON);
				IncreasePunchNumber();
			}
			else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		}
		break;
	case TIMERCLOCK_PUNCH + 1:
		if (m_iArrDI_PinStatus[IO_IN_PUNCH_ON])
		{
			m_iArrSet_PinStatus[IO_OUT_PUNCH_ON] = 0;
			eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
			if (eRet == nmf_R_OK)
			{
				m_iTimeClock_Auto++;
			}
			else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		}
		break;
	case TIMERCLOCK_PUNCH + 2:
		m_iArrSet_PinStatus[IO_OUT_PUNCH_OFF] = 1;
		m_iArrSet_PinStatus[IO_OUT_PL_CLAMP] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_PUNCH + 3:
		m_iTimeClock_Auto++;
		break;
	case TIMERCLOCK_PUNCH + 4:
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_PSTP_B_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_LSTP_B_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_DOWN] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
			DisplayStatus(DeviceStatus::STATUS_OUT_PUNCH_OFF);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_PUNCH + 5:
		m_iTimeClock_Auto++;
		break;
	case TIMERCLOCK_PUNCH + 6:
		m_iArrSet_PinStatus[IO_OUT_PUNCH_OFF] = 0;
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CW] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_PUNCH + 7:
		m_iTimeClock_Auto = TIMERCLOCK_EJECT;
		break;

		// Eject
	case TIMERCLOCK_EJECT:
		m_iArrSet_PinStatus[IO_OUT_PL_CLAMP] = 0;
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CW] = 1;
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_PSTP_B_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_LSTP_B_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_DOWN] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
			DisplayStatus(DeviceStatus::STATUS_OUT_LSTP_B_DOWN);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_EJECT + 1:
		if (!m_iArrDI_PinStatus[IO_IN_PUNCH_IN])
		{
			m_iTimeClock_Auto++;
		}
		break;
	case TIMERCLOCK_EJECT + 2:
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_PSTP_B_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_LSTP_B_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_CAMERA_DOWN] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
			DisplayStatus(DeviceStatus::STATUS_OUT_LSTP_A_DOWN);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_EJECT + 3:
		m_iTimeClock_Auto++;
		break;
	case TIMERCLOCK_EJECT + 4:
		m_iArrSet_PinStatus[IO_OUT_CAMERA_DOWN] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto++;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_EJECT + 5:
		m_iTimeClock_Auto = TIMERCLOCK_EXIT;
		break;

		// Exit
	case TIMERCLOCK_EXIT:
		if (!m_iArrDI_PinStatus[IO_IN_PLATE_A_ON] && !m_iArrDI_PinStatus[IO_IN_PLATE_B_ON])
		{
			m_iTimeClock_Auto++;
		}
		break;
	case TIMERCLOCK_EXIT + 1:
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CW] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Auto = TIMERCLOCK_PLATEIN;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;

	default:
		break;
	}
}

void CEquipmentDlg::Sequence_PlateIN()
{
	Enmf_FUNC_RESULT eRet;
	switch (m_iTimeClock_Manual)
	{
	case TIMERCLOCK_PLATEIN:
		//printf("Plate In\n");
		Homing();
		m_iTimeClock_Manual++;
		break;
	case TIMERCLOCK_PLATEIN + 1:
		if (m_iArrDI_PinStatus[IO_IN_PLATE_IN])
		{
			//MoveInitPoint();
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->SetIcon(NULL);
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->Invalidate(FALSE);
			for (int i = 0; i < 2; i++)
			{
				((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[i].GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->SetIcon(NULL);
				((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[i].GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->Invalidate(FALSE);
			}
			m_iTimeClock_Manual++;
		}
		break;
	case TIMERCLOCK_PLATEIN + 2:
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CW] = 1;
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_UP] = 1;
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_UP] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
			DisplayStatus(DeviceStatus::STATUS_OUT_ROLLER_CW);
			DisplayStatus(DeviceStatus::STATUS_OUT_LSTP_B_UP);
			DisplayStatus(DeviceStatus::STATUS_OUT_PSTP_B_UP);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_PLATEIN + 3:
		m_iTimeClock_Manual++;
		break;
	case TIMERCLOCK_PLATEIN + 4:
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_UP] = 0;
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_UP] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_PLATEIN + 5:
		if (m_iArrDI_PinStatus[IO_IN_PLATE_A_ON]) m_iTimeClock_Manual++;
		break;
	case TIMERCLOCK_PLATEIN + 6:
		m_iTimeClock_Manual++;
		break;
	case TIMERCLOCK_PLATEIN + 7:
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CW] = 0;
		m_iArrSet_PinStatus[IO_OUT_CAMERA_UP] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual = TIMERCLOCK_CENTERING;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	default:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_PLATEIN);
		break;
	}
}

void CEquipmentDlg::Sequence_Centering()
{
	Enmf_FUNC_RESULT eRet;

	switch (m_iTimeClock_Manual)
	{
	case TIMERCLOCK_CENTERING:
		m_iArrSet_PinStatus[IO_OUT_CENTER_ON] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
			DisplayStatus(DeviceStatus::STATUS_OUT_CENTER_ON);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_CENTERING + 1:
		m_iTimeClock_Manual++;
		break;
	case TIMERCLOCK_CENTERING + 2:
		m_iArrSet_PinStatus[IO_OUT_CENTER_OFF] = 1;
		m_iArrSet_PinStatus[IO_OUT_CENTER_ON] = 0;
		m_iArrSet_PinStatus[IO_OUT_CAMERA_UP] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
			DisplayStatus(DeviceStatus::STATUS_OUT_CENTER_OFF);
			DisplayStatus(DeviceStatus::STATUS_OUT_CAMERA_UP);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_CENTERING + 3:
		m_iTimeClock_Manual++;
		break;
	case TIMERCLOCK_CENTERING + 4:
		m_iArrSet_PinStatus[IO_OUT_CENTER_OFF] = 0;
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CCW] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_CENTERING + 5:
		m_iTimeClock_Manual = TIMERCLOCK_CLAMP;
		break;
	default:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_CENTERING);
		break;
	}
}

void CEquipmentDlg::Sequence_Clamp()
{
	Enmf_FUNC_RESULT eRet;

	switch (m_iTimeClock_Manual)
	{
	case TIMERCLOCK_CLAMP:
		if (m_iArrDI_PinStatus[IO_IN_PUNCH_IN])
		{
			m_iTimeClock_Manual++;
		}
		break;
	case TIMERCLOCK_CLAMP + 1:
		m_iTimeClock_Manual++;
		break;
	case TIMERCLOCK_CLAMP + 2:
		m_iArrSet_PinStatus[IO_OUT_PL_CLAMP] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
			DisplayStatus(DeviceStatus::STATUS_OUT_PL_CLAMP);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_CLAMP + 3:
		m_iTimeClock_Manual++;
		break;
	case TIMERCLOCK_CLAMP + 4:
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CCW] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_CLAMP + 5:
		m_iTimeClock_Manual = TIMERCLOCK_ALIGNMENT;
		break;
	default:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_CLAMP);
		break;
	}
}

void CEquipmentDlg::Sequence_Alignment()
{
	Enmf_FUNC_RESULT eRet;
	switch (m_iTimeClock_Manual)
	{
	case TIMERCLOCK_ALIGNMENT:
		if (m_Check_Skip_SearchMark.GetCheck())
		{
			m_iTimeClock_Manual = TIMERCLOCK_PUNCH;
		}
		else
		{
			m_iCheckCurNum_SearchMark++;
			if (m_iCheckCurNum_SearchMark > m_iCheckMaxNum_SearchMark)
			{
				// Alignment 상태표시
				((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
					GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->SetIcon(m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_NG);
				((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
					GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->Invalidate(FALSE);
				DisplayStatus(DeviceStatus::STATUS_CAMERA1_SEARCHMARK_ERROR);
				m_iCheckCurNum_SearchMark = 0;
				m_iTimeClock_Manual = TIMERCLOCK_EJECT;
				break;
			}

			// Camera 1, 2  SearchMark
			for (int i = 0; i < 2; i++)
			{
				m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[i].m_iCheckAlignment_SearchMark = 0;
				m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[i].Alignment_SearchMark();
			}
			m_iTimeClock_Manual++;
		}
		break;
	case TIMERCLOCK_ALIGNMENT + 1:
		// 두 카메라중 하나라도 SearchMark 실패한 경우
		if (m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].m_iCheckAlignment_SearchMark == -1)
		{
			// Alignment 상태표시
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->SetIcon(m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_NG);
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->Invalidate(FALSE);
			DisplayStatus(DeviceStatus::STATUS_CAMERA1_SEARCHMARK_ERROR);
			m_iTimeClock_Manual = TIMERCLOCK_EJECT;
		}

		if (m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].m_iCheckAlignment_SearchMark == -1)
		{
			// Alignment 상태표시
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->SetIcon(m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_NG);
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->Invalidate(FALSE);
			DisplayStatus(DeviceStatus::STATUS_CAMERA2_SEARCHMARK_ERROR);
			m_iTimeClock_Manual = TIMERCLOCK_EJECT;
		}

		// 두 카메라의 SearchMark 로직이 끝나면 장비이동
		if (m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].m_iCheckAlignment_SearchMark == 1 &&
			m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].m_iCheckAlignment_SearchMark == 1)
		{
			// SearchMark 검증 실패 시 다시 Alignmenet 로직 시작
			if (m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].m_iCheckOK_SearchMark == 1 &&
				m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].m_iCheckOK_SearchMark == 1)
			{
				m_iTimeClock_Manual++;
				break;
			}
			printf("Test");

			m_iCheckAlignment_Move = 0;
			Alignment_Move();
			m_iTimeClock_Manual++;
		}
		break;
	case TIMERCLOCK_ALIGNMENT + 2:
		// 장비이동이 끝나면 Search Mark 검증
		if (m_iCheckAlignment_Move == 1)
		{
			if (m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].m_iCheckOK_SearchMark == 0 ||
				m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].m_iCheckOK_SearchMark == 0)
			{
				m_iTimeClock_Manual = TIMERCLOCK_ALIGNMENT;
				break;
			}

			// Alignment 상태표시
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->SetIcon(m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_OK);
			((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
				GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->Invalidate(FALSE);

			m_iCheckCurNum_SearchMark = 0;
			m_iTimeClock_Manual = TIMERCLOCK_PUNCH;
		}
		break;
	default:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_ALIGNMENT);
		break;
	}
}

void CEquipmentDlg::Sequence_Punch()
{
	Enmf_FUNC_RESULT eRet;
	switch (m_iTimeClock_Manual)
	{
	case TIMERCLOCK_PUNCH:
		if (m_Check_Skip_Punch.GetCheck())
		{
			m_iTimeClock_Manual = TIMERCLOCK_EJECT;
		}
		else
		{
			m_iArrSet_PinStatus[IO_OUT_PUNCH_ON] = 1;
			eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
			if (eRet == nmf_R_OK)
			{
				m_iTimeClock_Manual++;
				DisplayStatus(DeviceStatus::STATUS_OUT_PUNCH_ON);
				IncreasePunchNumber();
			}
			else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		}
		break;
	case TIMERCLOCK_PUNCH + 1:
		if (m_iArrDI_PinStatus[IO_IN_PUNCH_ON])
		{
			m_iArrSet_PinStatus[IO_OUT_PUNCH_ON] = 0;
			eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
			if (eRet == nmf_R_OK)
			{
				m_iTimeClock_Manual++;
			}
			else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		}
		break;
	case TIMERCLOCK_PUNCH + 2:
		m_iArrSet_PinStatus[IO_OUT_PUNCH_OFF] = 1;
		m_iArrSet_PinStatus[IO_OUT_PL_CLAMP] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_PUNCH + 3:
		m_iTimeClock_Manual++;
		break;
	case TIMERCLOCK_PUNCH + 4:
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_PSTP_B_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_LSTP_B_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_DOWN] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
			DisplayStatus(DeviceStatus::STATUS_OUT_PUNCH_OFF);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_PUNCH + 5:
		m_iTimeClock_Manual++;
		break;
	case TIMERCLOCK_PUNCH + 6:
		m_iArrSet_PinStatus[IO_OUT_PUNCH_OFF] = 0;
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CW] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_PUNCH + 7:
		m_iTimeClock_Manual = TIMERCLOCK_EJECT;
		break;
	default:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_PUNCH);
		break;
	}
}

void CEquipmentDlg::Sequence_Eject()
{
	Enmf_FUNC_RESULT eRet;
	switch (m_iTimeClock_Manual)
	{
	case TIMERCLOCK_EJECT:
		m_iArrSet_PinStatus[IO_OUT_PL_CLAMP] = 0;
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CW] = 1;
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_PSTP_B_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_LSTP_B_DOWN] = 1;
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_DOWN] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
			DisplayStatus(DeviceStatus::STATUS_OUT_LSTP_B_DOWN);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_EJECT + 1:
		if (!m_iArrDI_PinStatus[IO_IN_PUNCH_IN])
		{
			m_iTimeClock_Manual++;
		}
		break;
	case TIMERCLOCK_EJECT + 2:
		m_iArrSet_PinStatus[IO_OUT_PSTP_A_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_PSTP_B_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_LSTP_B_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_LSTP_A_DOWN] = 0;
		m_iArrSet_PinStatus[IO_OUT_CAMERA_DOWN] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
			DisplayStatus(DeviceStatus::STATUS_OUT_LSTP_A_DOWN);
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_EJECT + 3:
		m_iTimeClock_Manual++;
		break;
	case TIMERCLOCK_EJECT + 4:
		m_iArrSet_PinStatus[IO_OUT_CAMERA_DOWN] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
		}
		else Sequence_ErrorHandler(TIMERID_AUTOSEQUENCE, eRet);
		break;
	case TIMERCLOCK_EJECT + 5:
		m_iTimeClock_Manual = TIMERCLOCK_EXIT;
		break;
	default:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_EJECT);
		break;
	}
}

void CEquipmentDlg::Sequence_Rotate()
{
	Enmf_FUNC_RESULT eRet;
	switch (m_iTimeClock_Manual)
	{
	case TIMERCLOCK_ROTATE:
		m_iArrSet_PinStatus[IO_OUT_PL_ROTATE] = 1;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual++;
		}
		else Sequence_ErrorHandler(TIMERID_ROTATE, eRet);
		break;
	case TIMERCLOCK_ROTATE + 1:
		break;
	case TIMERCLOCK_ROTATE + 2:
		break;
	case TIMERCLOCK_ROTATE + 3:
		m_iArrSet_PinStatus[IO_OUT_PL_ROTATE] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual = TIMERCLOCK_EXIT;
		}
		else Sequence_ErrorHandler(TIMERID_ROTATE, eRet);
		break;
	default:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_ROTATE);
		break;
	}
}

void CEquipmentDlg::Sequence_Exit()
{
	Enmf_FUNC_RESULT eRet;
	switch (m_iTimeClock_Manual)
	{
	case TIMERCLOCK_EXIT:
		m_iArrSet_PinStatus[IO_OUT_ROLLER_CW] = 0;
		eRet = (Enmf_FUNC_RESULT)nmf_DOSet(m_pPunchingMachineDlg->GetPtrIODlg()->m_nDevNo, m_iArrSet_PinStatus);
		if (eRet == nmf_R_OK)
		{
			m_iTimeClock_Manual = TIMERCLOCK_PLATEIN;
		}
		else Sequence_ErrorHandler(TIMERID_EXIT, eRet);
		break;
	default:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_EXIT);
		break;
	}
}


void CEquipmentDlg::Sequence_ErrorHandler(UINT32 iTimerID, Enmf_FUNC_RESULT eRet)
{
	switch (iTimerID)
	{
	case TIMERID_INITIALIZE:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_INITIALIZE);
		break;
	case TIMERID_AUTOSEQUENCE:
		m_iTimeClock_Auto = TIMERCLOCK_INITIALIZE;
		break;
	case TIMERID_PLATEIN:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_PLATEIN);
		break;
	case TIMERID_CENTERING:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_CENTERING);
		break;
	case TIMERID_CLAMP:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_CLAMP);
		break;
	case TIMERID_ALIGNMENT:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_ALIGNMENT);
		break;
	case TIMERID_PUNCH:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_PUNCH);
		break;
	case TIMERID_EJECT:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_EJECT);
		break;
	case TIMERID_ROTATE:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_ROTATE);
		break;
	case TIMERID_EXIT:
		Sequence_EnableWindow(TRUE);
		KillTimer(TIMERID_EXIT);
		break;

	default:
		break;
	}
}

void CEquipmentDlg::Sequence_EnableWindow(BOOL value)
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->GetDlgItem(IDC_MFCBUTTON_STATUSINFORMATION_INITIALIZE)->EnableWindow(value);
	GetDlgItem(IDC_MFCBUTTON_EQUIPMENT_INITIALIZE)->EnableWindow(value);
	GetDlgItem(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_PLATEIN)->EnableWindow(value);
	GetDlgItem(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_CENTERING)->EnableWindow(value);
	GetDlgItem(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_CLAMP)->EnableWindow(value);
	GetDlgItem(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_ALIGNMENT)->EnableWindow(value);
	GetDlgItem(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_PUNCH)->EnableWindow(value);
	GetDlgItem(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_EJECT)->EnableWindow(value);
	GetDlgItem(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_ROTATE)->EnableWindow(value);
	GetDlgItem(IDC_MFCBUTTON_EQUIPMENT_SEQUENCE_EXIT)->EnableWindow(value);
}

void CEquipmentDlg::Sequence_KillTimer()
{
	KillTimer(TIMERID_INITIALIZE);
	KillTimer(TIMERID_AUTOSEQUENCE);
	KillTimer(TIMERID_PLATEIN);
	KillTimer(TIMERID_CENTERING);
	KillTimer(TIMERID_CLAMP);
	KillTimer(TIMERID_ALIGNMENT);
	KillTimer(TIMERID_PUNCH);
	KillTimer(TIMERID_EJECT);
	KillTimer(TIMERID_ROTATE);
	KillTimer(TIMERID_EXIT);
	Sequence_EnableWindow(TRUE);
}


void CEquipmentDlg::UpdateScrollInfo(UINT nType, int cx, int cy)
{
	int nScrollMaxV = 0;
	int nScrollMaxH = 0;
	int nTotalHeight = 0;
	int nTotalWidth = 0;
	int nDelta = 0;
	SCROLLINFO siV = { 0, }, siH = { 0, };

	m_nPageSizeV = 0;
	m_nPageSizeH = 0;
	nTotalHeight = m_RectOrgEquipmentDlg.Height();
	nTotalWidth = m_RectOrgEquipmentDlg.Width();

	if (cy < nTotalHeight)
	{
		nScrollMaxV = nTotalHeight - 1;
		m_nPageSizeV = cy;
		m_nScrollPosV = min(m_nScrollPosV, nTotalHeight - m_nPageSizeV - 1);
	}

	if (cx < nTotalWidth)
	{
		nScrollMaxH = nTotalWidth - 1;
		m_nPageSizeH = cx;
		m_nScrollPosH = min(m_nScrollPosH, nTotalWidth - m_nPageSizeH - 1);
	}

	siV.cbSize = sizeof(SCROLLINFO);
	siV.fMask = SIF_ALL;

	siH.cbSize = sizeof(SCROLLINFO);
	siH.fMask = SIF_ALL;

	GetScrollInfo(SB_VERT, &siV);
	GetScrollInfo(SB_HORZ, &siH);

	if (siV.nPage != 0)
	{
		if (cy > siV.nPage)
		{
			if (siV.nPos + cy > siV.nMax)
			{
				nDelta = siV.nPos + cy - siV.nMax;
				ScrollWindow(0, nDelta);
			}
		}
	}

	siV.nMin = 0;
	siV.nMax = nScrollMaxV;
	siV.nPage = m_nPageSizeV;
	siV.nPos = m_nScrollPosV;
	SetScrollInfo(SB_VERT, &siV, TRUE);

	if (siH.nPage != 0)
	{
		if (cx > siH.nPage)
		{
			if (siH.nPos + cx > siH.nMax)
			{
				nDelta = siH.nPos + cx - siH.nMax;
				ScrollWindow(nDelta, 0);
			}
		}
	}

	siH.nMin = 0;
	siH.nMax = nScrollMaxH;
	siH.nPage = m_nPageSizeH;
	siH.nPos = m_nScrollPosH;
	SetScrollInfo(SB_HORZ, &siH, TRUE);
}


void CEquipmentDlg::AlarmON()
{
	m_bCheckAlarmStatus = TRUE;
	SetTimer(TIMER_ALARM, 1000, NULL);
}

void CEquipmentDlg::AlarmOFF()
{
	HICON ArrIcon[2] =
	{
		m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_START,
		m_pPunchingMachineDlg->GetPtrMainDlg()->m_hIcon_STOP
	};

	KillTimer(TIMER_ALARM);
	m_bCheckAlarmStatus = FALSE;
	m_iToggleAlarmIcon = 0;

	((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
		GetDlgItem(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS))->SetIcon(ArrIcon[m_iSystemMode]);
	((CStatic*)m_pPunchingMachineDlg->GetPtrMainDlg()->
		GetDlgItem(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS))->Invalidate(FALSE);
}

void CEquipmentDlg::ResetCurrentPunchNumber()
{
	m_iCurrent_PunchNumber = 0;
	WritePrivateProfileString("PUNCH", "CurrentNumber", "0", m_pPunchingMachineDlg->m_strPath_punchdata_ini);
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->
		GetDlgItem(IDC_EDIT_STATUSINFORMATION_CURRENT_NUMBEROFPLATES)->SetWindowText("0");
}

void CEquipmentDlg::IncreasePunchNumber()
{
	CString strBuf;
	strBuf.Format("%d", ++m_iCurrent_PunchNumber);
	WritePrivateProfileString("PUNCH", "CurrentNumber", strBuf.GetBuffer(), m_pPunchingMachineDlg->m_strPath_punchdata_ini);
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->
		GetDlgItem(IDC_EDIT_STATUSINFORMATION_CURRENT_NUMBEROFPLATES)->SetWindowText(strBuf);

	strBuf.Format("%d", ++m_iTotal_PunchNumber);
	WritePrivateProfileString("PUNCH", "TotalNumber", strBuf.GetBuffer(), m_pPunchingMachineDlg->m_strPath_punchdata_ini);
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrStatusInformationDlg()->
		GetDlgItem(IDC_EDIT_STATUSINFORMATION_TOTAL_NUMBEROFPLATES)->SetWindowText(strBuf);

	strBuf.Empty();
}


// Thread
UINT CEquipmentDlg::ThreadAlignment_Move(LPVOID _method)
{
	// 다이얼로그 인스턴스
	CPunchingMachineDlg* punchingmachinedlg = (CPunchingMachineDlg*)AfxGetApp()->GetMainWnd();
	CEquipmentDlg* equipmentdlg = (CEquipmentDlg*)_method;
	CameraSubDlg* camerasubdlg = (CameraSubDlg*)punchingmachinedlg->GetPtrMainDlg()->GetPtrCameraSubDlg();

	// X축 Pulse 계산
	if(punchingmachinedlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].m_iCheckOK_SearchMark == 0)
		equipmentdlg->m_dbPulse_Distance_Axis1 = round(camerasubdlg[0].m_dbDistMM_X_SearchedMarkCenter * (double)equipmentdlg->m_iPulsePerMM_Axis1);

	// RY축 Pulse 계산
	if (punchingmachinedlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].m_iCheckOK_SearchMark == 0)
		equipmentdlg->m_dbPulse_Distance_Axis0 = round(camerasubdlg[1].m_dbDistMM_Y_SearchedMarkCenter * (double)equipmentdlg->m_iPulsePerMM_Axis0);

	// LY축 Pulse 계산
	if (punchingmachinedlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].m_iCheckOK_SearchMark == 0)
		equipmentdlg->m_dbPulse_Distance_Axis2 = round(camerasubdlg[0].m_dbDistMM_Y_SearchedMarkCenter * (double)equipmentdlg->m_iPulsePerMM_Axis2);

	printf("Axis0 : %lf  Axis1 : %lf  Axis2 : %lf\n", 
		equipmentdlg->m_dbPulse_Distance_Axis0, 
		equipmentdlg->m_dbPulse_Distance_Axis1, 
		equipmentdlg->m_dbPulse_Distance_Axis2);


	// 다축 상대이동
	short nRet;
	int iPaixCtlNo = punchingmachinedlg->GetPtrMotionDlg()->GetPaixCtlNo();
	short nAxisCount = 3;
	short nAxisNo[8];
	double dbPulsePos[8];
	nAxisNo[0] = 0;
	nAxisNo[1] = 1;
	nAxisNo[2] = 2;
	dbPulsePos[0] = equipmentdlg->m_dbPulse_Distance_Axis0 * (-1);
	dbPulsePos[1] = equipmentdlg->m_dbPulse_Distance_Axis1;
	dbPulsePos[2] = equipmentdlg->m_dbPulse_Distance_Axis2 * (-1);

	nRet = nmc_VarRelMove(iPaixCtlNo, nAxisCount, nAxisNo, dbPulsePos);
	//if (nRet == NMC_OK)
	{
		// 모든 축이 이동이 종료하였는지 체크
		short nBusyStatus[8];
		bool bAllMoving = true;
		while (bAllMoving)
		{
			nmc_GetBusyStatusAll(iPaixCtlNo, nBusyStatus);
			bAllMoving = false;
			for (int i = 0; i < 8; i++)
			{
				if (nBusyStatus[i] == 1)
				{
					bAllMoving = true;
					break;
				}
			}
		}
	}

	equipmentdlg->m_iCheckAlignment_Move = 1;

	return 0;	// 스레드 자동 소멸
}

