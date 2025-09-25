// CMotionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "PunchingMachineDlg.h"
#include "CMainDlg.h"
#include "CEquipmentDlg.h"
#include "CMotionDlg.h"
#include "DistDlg.h"
#include "CSimpleMotionDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMotionDlg 대화 상자

IMPLEMENT_DYNAMIC(CMotionDlg, CDialog)

CMotionDlg::CMotionDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MOTION_DIALOG, pParent)
	, m_bRemote(FALSE)
	, m_bThreadRun(FALSE)
	, m_hThreadId(NULL)
	, m_nPaixCtlNo(0)
	, m_nTimerId(0)
	, m_pPunchingMachineDlg(NULL)
{

}

CMotionDlg::~CMotionDlg()
{
}

void CMotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CCW4, m_BtnCcw4);
	DDX_Control(pDX, IDC_BTN_CCW3, m_BtnCcw3);
	DDX_Control(pDX, IDC_BTN_CCW2, m_BtnCcw2);
	DDX_Control(pDX, IDC_BTN_CCW1, m_BtnCcw1);
	DDX_Control(pDX, IDC_BTN_CW4, m_BtnCw4);
	DDX_Control(pDX, IDC_BTN_CW3, m_BtnCw3);
	DDX_Control(pDX, IDC_BTN_CW2, m_BtnCw2);
	DDX_Control(pDX, IDC_BTN_CW1, m_BtnCw1);
	DDX_Control(pDX, IDC_STATIC_BUSY4, m_staticBusy4);
	DDX_Control(pDX, IDC_STATIC_BUSY3, m_staticBusy3);
	DDX_Control(pDX, IDC_STATIC_BUSY2, m_staticBusy2);
	DDX_Control(pDX, IDC_STATIC_BUSY1, m_staticBusy1);
	DDX_Control(pDX, IDC_STATIC_PLIMIT4, m_staticPLimit4);
	DDX_Control(pDX, IDC_STATIC_PLIMIT3, m_staticPLimit3);
	DDX_Control(pDX, IDC_STATIC_PLIMIT2, m_staticPLimit2);
	DDX_Control(pDX, IDC_STATIC_PLIMIT1, m_staticPLimit1);
	DDX_Control(pDX, IDC_STATIC_NEAR4, m_staticNear4);
	DDX_Control(pDX, IDC_STATIC_NEAR3, m_staticNear3);
	DDX_Control(pDX, IDC_STATIC_NEAR2, m_staticNear2);
	DDX_Control(pDX, IDC_STATIC_NEAR1, m_staticNear1);
	DDX_Control(pDX, IDC_STATIC_MLIMIT4, m_staticMLimit4);
	DDX_Control(pDX, IDC_STATIC_MLIMIT3, m_staticMLimit3);
	DDX_Control(pDX, IDC_STATIC_MLIMIT2, m_staticMLimit2);
	DDX_Control(pDX, IDC_STATIC_MLIMIT1, m_staticMLimit1);
	DDX_Control(pDX, IDC_STATIC_HOME4, m_staticHome4);
	DDX_Control(pDX, IDC_STATIC_HOME3, m_staticHome3);
	DDX_Control(pDX, IDC_STATIC_HOME2, m_staticHome2);
	DDX_Control(pDX, IDC_STATIC_HOME1, m_staticHome1);
	DDX_Control(pDX, IDC_STATIC_EMER, m_staticEmer);
	DDX_Control(pDX, IDC_STATIC_ALARM4, m_staticAlarm4);
	DDX_Control(pDX, IDC_STATIC_ALARM3, m_staticAlarm3);
	DDX_Control(pDX, IDC_STATIC_ALARM2, m_staticAlarm2);
	DDX_Control(pDX, IDC_STATIC_ALARM1, m_staticAlarm1);
	DDX_Control(pDX, IDC_STATIC_INPOS1, m_staticInpos1);
	DDX_Control(pDX, IDC_STATIC_INPOS2, m_staticInpos2);
	DDX_Control(pDX, IDC_STATIC_INPOS3, m_staticInpos3);
	DDX_Control(pDX, IDC_STATIC_INPOS4, m_staticInpos4);

}


BEGIN_MESSAGE_MAP(CMotionDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_OPEN, OnBtnOpen)
	ON_CBN_SELENDOK(IDC_CB_EMER, OnSelendokCbEmer)
	ON_CBN_SELENDOK(IDC_CB_NEAR1, OnSelendokCbNear1)
	ON_CBN_SELENDOK(IDC_CB_NEAR2, OnSelendokCbNear2)
	ON_CBN_SELENDOK(IDC_CB_NEAR3, OnSelendokCbNear3)
	ON_CBN_SELENDOK(IDC_CB_NEAR4, OnSelendokCbNear4)
	ON_CBN_SELENDOK(IDC_CB_LIMIT1, OnSelendokCbLimit1)
	ON_CBN_SELENDOK(IDC_CB_LIMIT2, OnSelendokCbLimit2)
	ON_CBN_SELENDOK(IDC_CB_LIMIT3, OnSelendokCbLimit3)
	ON_CBN_SELENDOK(IDC_CB_LIMIT4, OnSelendokCbLimit4)
	ON_BN_CLICKED(IDC_BTN_RPM1, OnBtnRpm1)
	ON_BN_CLICKED(IDC_BTN_RPM2, OnBtnRpm2)
	ON_BN_CLICKED(IDC_BTN_RPM3, OnBtnRpm3)
	ON_BN_CLICKED(IDC_BTN_RPM4, OnBtnRpm4)
	ON_BN_CLICKED(IDC_BTN_INC1, OnBtnInc1)
	ON_BN_CLICKED(IDC_BTN_INC2, OnBtnInc2)
	ON_BN_CLICKED(IDC_BTN_INC3, OnBtnInc3)
	ON_BN_CLICKED(IDC_BTN_INC4, OnBtnInc4)
	ON_BN_CLICKED(IDC_BTN_ABS1, OnBtnAbs1)
	ON_BN_CLICKED(IDC_BTN_ABS2, OnBtnAbs2)
	ON_BN_CLICKED(IDC_BTN_ABS3, OnBtnAbs3)
	ON_BN_CLICKED(IDC_BTN_ABS4, OnBtnAbs4)
	ON_BN_CLICKED(IDC_BTN_STOP1, OnBtnStop1)
	ON_BN_CLICKED(IDC_BTN_STOP2, OnBtnStop2)
	ON_BN_CLICKED(IDC_BTN_STOP3, OnBtnStop3)
	ON_BN_CLICKED(IDC_BTN_STOP4, OnBtnStop4)
	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_BTN_CURRENT1, OnBtnCurrent1)
	ON_BN_CLICKED(IDC_BTN_CURRENT2, OnBtnCurrent2)
	ON_BN_CLICKED(IDC_BTN_CURRENT3, OnBtnCurrent3)
	ON_BN_CLICKED(IDC_BTN_CURRENT4, OnBtnCurrent4)
	ON_BN_CLICKED(IDC_BTN_SERVO1, OnBtnServo1)
	ON_BN_CLICKED(IDC_BTN_SERVO2, OnBtnServo2)
	ON_BN_CLICKED(IDC_BTN_SERVO3, OnBtnServo3)
	ON_BN_CLICKED(IDC_BTN_SERVO4, OnBtnServo4)
	ON_BN_CLICKED(IDC_BTN_ALARM1, OnBtnAlarm1)
	ON_BN_CLICKED(IDC_BTN_ALARM2, OnBtnAlarm2)
	ON_BN_CLICKED(IDC_BTN_ALARM3, OnBtnAlarm3)
	ON_BN_CLICKED(IDC_BTN_ALARM4, OnBtnAlarm4)
	ON_CBN_SELENDOK(IDC_CB_PLIMIT1, OnSelendokCbPlimit1)
	ON_CBN_SELENDOK(IDC_CB_PLIMIT2, OnSelendokCbPlimit2)
	ON_CBN_SELENDOK(IDC_CB_PLIMIT3, OnSelendokCbPlimit3)
	ON_CBN_SELENDOK(IDC_CB_PLIMIT4, OnSelendokCbPlimit4)
	ON_MESSAGE(UM_JOG, UmJog)
	ON_BN_CLICKED(IDC_BUTTON_DCC1, &CMotionDlg::OnBnClickedButtonDcc1)
	ON_BN_CLICKED(IDC_BUTTON_DCC2, &CMotionDlg::OnBnClickedButtonDcc2)
	ON_BN_CLICKED(IDC_BUTTON_DCC3, &CMotionDlg::OnBnClickedButtonDcc3)
	ON_BN_CLICKED(IDC_BUTTON_DCC4, &CMotionDlg::OnBnClickedButtonDcc4)
	ON_CBN_SELENDOK(IDC_CB_ALARM1, &CMotionDlg::OnCbnSelendokCbAlarm1)
	ON_CBN_SELENDOK(IDC_CB_ALARM2, &CMotionDlg::OnCbnSelendokCbAlarm2)
	ON_CBN_SELENDOK(IDC_CB_ALARM3, &CMotionDlg::OnCbnSelendokCbAlarm3)
	ON_CBN_SELENDOK(IDC_CB_ALARM4, &CMotionDlg::OnCbnSelendokCbAlarm4)
	ON_CBN_SELENDOK(IDC_CB_INPOS1, &CMotionDlg::OnCbnSelendokCbInpos1)
	ON_CBN_SELENDOK(IDC_CB_INPOS2, &CMotionDlg::OnCbnSelendokCbInpos2)
	ON_CBN_SELENDOK(IDC_CB_INPOS3, &CMotionDlg::OnCbnSelendokCbInpos3)
	ON_CBN_SELENDOK(IDC_CB_INPOS4, &CMotionDlg::OnCbnSelendokCbInpos4)
	ON_CBN_SELENDOK(IDC_CB_PULSE_MODE1, &CMotionDlg::OnCbnSelendokCbPulseMode1)
	ON_CBN_SELENDOK(IDC_CB_PULSE_MODE2, &CMotionDlg::OnCbnSelendokCbPulseMode2)
	ON_CBN_SELENDOK(IDC_CB_PULSE_MODE3, &CMotionDlg::OnCbnSelendokCbPulseMode3)
	ON_CBN_SELENDOK(IDC_CB_PULSE_MODE4, &CMotionDlg::OnCbnSelendokCbPulseMode4)
	ON_BN_CLICKED(IDC_BUTTON6, &CMotionDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMotionDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CMotionDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CMotionDlg::OnBnClickedButton9)

	ON_BN_CLICKED(IDC_BUTTON10, &CMotionDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CMotionDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CMotionDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CMotionDlg::OnBnClickedButton13)
	ON_STN_DBLCLK(IDC_CMD1, &CMotionDlg::OnStnDblclickCmd1)
	ON_BN_CLICKED(IDC_BUTTON5, &CMotionDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_DCC5, &CMotionDlg::OnBnClickedButtonDcc5)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CMotionDlg 메시지 처리기

BOOL CMotionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetClientRect(&m_RectOrgMotionDlg);
	MoveWindow(m_RectMotionDlg);
	//UpdateScrollInfo(0, m_RectMotionDlg.Width(), m_RectMotionDlg.Height());
	//Init();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMotionDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMotionDlg::OnTimer(UINT_PTR  nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//ReadSeonsor();
	CDialog::OnTimer(nIDEvent);
}

void CMotionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	UpdateScrollInfo(nType, cx, cy);
}


void CMotionDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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


void CMotionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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




void CMotionDlg::OnBtnOpen()
{
	// TODO: Add your control notification handler code here
	CString str;
	short nret;
	m_nPaixCtlNo = GetDlgItemInt(IDC_EDIT_EQUIP);
	if (m_nPaixCtlNo < 1 || m_nPaixCtlNo > 254)
		return;
	GetDlgItemText(IDC_BTN_OPEN, str);
	if (str == "Open")
	{
		m_bRemote = FALSE;
		nret = nmc_OpenDevice(m_nPaixCtlNo);

		if (nret == 0)
			m_bRemote = TRUE;
		else
		{
			AfxMessageBox("모션 연결 실패!", MB_ICONERROR);
			return;
		}

		if (m_bRemote == TRUE)
		{
			SetDlgItemText(IDC_BTN_OPEN, "Close");
			m_bThreadRun = FALSE;
			m_hThreadId = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadProc, this, 0, NULL);

			// 가감속 속도설정 (2023.01.15 추가)
			InitSpeed();
		}
	}
	else
	{
		if (m_hThreadId)
		{
			m_bThreadRun = TRUE;
			WaitForSingleObject(m_hThreadId, 1000);
			CloseHandle(m_hThreadId);
			m_hThreadId = NULL;
		}
		nmc_CloseDevice(m_nPaixCtlNo);
		m_bRemote = FALSE;
		SetDlgItemText(IDC_BTN_OPEN, "Open");

	}
	//MessageBox
}

void CMotionDlg::OnSelendokCbEmer()
{
	// TODO: Add your control notification handler code here
	short nRet = 0, nLogic;

	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_CB_EMER);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	if (m_bRemote == FALSE)
	{
		pCombo = NULL;
		return;
	}

	nRet = nmc_SetEmgLogic(m_nPaixCtlNo, 0, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}

void CMotionDlg::OnSelendokCbNear1()
{
	// TODO: Add your control notification handler code here
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_NEAR1);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetNearLogic(m_nPaixCtlNo, 0, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}

void CMotionDlg::OnSelendokCbNear2()
{
	// TODO: Add your control notification handler code here
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_NEAR2);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetNearLogic(m_nPaixCtlNo, 1, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}

void CMotionDlg::OnSelendokCbNear3()
{
	// TODO: Add your control notification handler code here
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_NEAR3);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetNearLogic(m_nPaixCtlNo, 2, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}

void CMotionDlg::OnSelendokCbNear4()
{
	// TODO: Add your control notification handler code here
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_NEAR4);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetNearLogic(m_nPaixCtlNo, 3, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}

void CMotionDlg::OnSelendokCbLimit1()
{
	// TODO: Add your control notification handler code here
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_LIMIT1);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetMinusLimitLogic(m_nPaixCtlNo, 0, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}

void CMotionDlg::OnSelendokCbLimit2()
{
	// TODO: Add your control notification handler code here
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_LIMIT2);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetMinusLimitLogic(m_nPaixCtlNo, 1, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}

void CMotionDlg::OnSelendokCbLimit3()
{
	// TODO: Add your control notification handler code here
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_LIMIT3);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetMinusLimitLogic(m_nPaixCtlNo, 2, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}

void CMotionDlg::OnSelendokCbLimit4()
{
	// TODO: Add your control notification handler code here
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_LIMIT4);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetMinusLimitLogic(m_nPaixCtlNo, 3, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}


void CMotionDlg::OnBtnRpm1()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	int iStart, iAcc, iDrive, iDec;
	int nRet;
	int nCheck = 0;
	iStart = GetDlgItemInt(IDC_EDIT_START1);
	iAcc = GetDlgItemInt(IDC_EDIT_ACC1);
	iDrive = GetDlgItemInt(IDC_EDIT_MAX1);
	iDec = GetDlgItemInt(IDC_EDIT_DEC1);

	nCheck = IsDlgButtonChecked(IDC_CHECK_SCURVE);

	if (iStart < 1 || iAcc < 1 || iDrive < 1 || iDec < 0)
		return;
	if (nCheck == 0)
		nRet = nmc_SetSpeed(m_nPaixCtlNo, 0, iStart, iAcc, iDec, iDrive);
	else
		nRet = nmc_SetSCurveSpeed(m_nPaixCtlNo, 0, iStart, iAcc, iDec, iDrive);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnRpm2()
{
	// TODO: Add your control notification handler code here

	if (m_bRemote == FALSE)    return;
	int iStart, iAcc, iDrive, iDec;
	int nRet;
	int nCheck = 0;

	iStart = GetDlgItemInt(IDC_EDIT_START2);
	iAcc = GetDlgItemInt(IDC_EDIT_ACC2);
	iDrive = GetDlgItemInt(IDC_EDIT_MAX2);
	iDec = GetDlgItemInt(IDC_EDIT_DEC2);

	nCheck = IsDlgButtonChecked(IDC_CHECK_SCURVE);

	if (iStart < 1 || iAcc < 1 || iDrive < 1 || iDec < 0)
		return;
	if (nCheck == 0)
		nRet = nmc_SetSpeed(m_nPaixCtlNo, 1, iStart, iAcc, iDec, iDrive);
	else
		nRet = nmc_SetSCurveSpeed(m_nPaixCtlNo, 1, iStart, iAcc, iDec, iDrive);
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnRpm3()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	int iStart, iAcc, iDrive, iDec;
	int nRet;
	int nCheck = 0;

	iStart = GetDlgItemInt(IDC_EDIT_START3);
	iAcc = GetDlgItemInt(IDC_EDIT_ACC3);
	iDrive = GetDlgItemInt(IDC_EDIT_MAX3);
	iDec = GetDlgItemInt(IDC_EDIT_DEC3);

	nCheck = IsDlgButtonChecked(IDC_CHECK_SCURVE);

	if (iStart < 1 || iAcc < 1 || iDrive < 1 || iDec < 0)
		return;
	if (nCheck == 0)
		nRet = nmc_SetSpeed(m_nPaixCtlNo, 2, iStart, iAcc, iDec, iDrive);
	else
		nRet = nmc_SetSCurveSpeed(m_nPaixCtlNo, 2, iStart, iAcc, iDec, iDrive);
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}

}

void CMotionDlg::OnBtnRpm4()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	int iStart, iAcc, iDrive, iDec;
	int nRet;
	int nCheck = 0;

	iStart = GetDlgItemInt(IDC_EDIT_START4);
	iAcc = GetDlgItemInt(IDC_EDIT_ACC4);
	iDrive = GetDlgItemInt(IDC_EDIT_MAX4);
	iDec = GetDlgItemInt(IDC_EDIT_DEC4);

	nCheck = IsDlgButtonChecked(IDC_CHECK_SCURVE);

	if (iStart < 1 || iAcc < 1 || iDrive < 1 || iDec < 0)
		return;
	if (nCheck == 0)
		nRet = nmc_SetSpeed(m_nPaixCtlNo, 3, iStart, iAcc, iDec, iDrive);
	else
		nRet = nmc_SetSCurveSpeed(m_nPaixCtlNo, 3, iStart, iAcc, iDec, iDrive);
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}

}

void CMotionDlg::OnBtnInc1()
{
	// TODO: Add your control notification handler code here
	int iRet;
	double fDist;
	CString str;
	if (m_bRemote == FALSE)	return;

	//iPulse = GetDlgItemInt(IDC_PULSE1);
	//iRet = nmc2_rel_move(m_nPaixCtlNo, 0, iPulse);
	GetDlgItemText(IDC_PULSE1, str);
	fDist = atof(str);
	iRet = nmc_RelMove(m_nPaixCtlNo, 0, fDist);
	switch (iRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnInc2()
{
	// TODO: Add your control notification handler code here
	int iRet;
	CString str;
	double fDist;
	if (m_bRemote == FALSE)	return;
	//iPulse = GetDlgItemInt(IDC_PULSE2);
	//iRet = nmc2_rel_move(m_nPaixCtlNo, 1, iPulse);
	GetDlgItemText(IDC_PULSE2, str);
	fDist = atof(str);
	iRet = nmc_RelMove(m_nPaixCtlNo, 1, fDist);

	switch (iRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnInc3()
{
	// TODO: Add your control notification handler code here
	int iRet;
	double fDist;
	CString str;
	if (m_bRemote == FALSE)	return;
	//iPulse = GetDlgItemInt(IDC_PULSE3);
	//iRet = nmc2_rel_move(m_nPaixCtlNo, 2, iPulse);
	GetDlgItemText(IDC_PULSE3, str);
	fDist = atof(str);
	iRet = nmc_RelMove(m_nPaixCtlNo, 2, fDist);

	switch (iRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnInc4()
{
	// TODO: Add your control notification handler code here
	int iRet;
	double fDist;
	CString str;
	if (m_bRemote == FALSE)	return;
	//iRet = nmc2_rel_move(m_nPaixCtlNo, 3, iPulse);
	//iPulse = GetDlgItemInt(IDC_PULSE4);
	GetDlgItemText(IDC_PULSE4, str);

	fDist = atof(str);
	iRet = nmc_RelMove(m_nPaixCtlNo, 3, fDist);

	switch (iRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnAbs1()
{
	// TODO: Add your control notification handler code here
	int iRet;
	double fDist;
	CString str;
	if (m_bRemote == FALSE)	return;
	//iPulse = GetDlgItemInt(IDC_PULSE1);
	//iRet = nmc2_abs_move(m_nPaixCtlNo, 0, iPulse);
	GetDlgItemText(IDC_PULSE1, str);
	fDist = atof(str);
	iRet = nmc_AbsMove(m_nPaixCtlNo, 0, fDist);
	switch (iRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnAbs2()
{
	// TODO: Add your control notification handler code here
	int iRet;
	double fDist;
	CString str;
	if (m_bRemote == FALSE)	return;
	//iPulse = GetDlgItemInt(IDC_PULSE2);
	//iRet = nmc2_abs_move(m_nPaixCtlNo, 0, iPulse);
	GetDlgItemText(IDC_PULSE2, str);
	fDist = atof(str);
	iRet = nmc_AbsMove(m_nPaixCtlNo, 1, fDist);

	switch (iRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnAbs3()
{
	// TODO: Add your control notification handler code here
	int iRet;
	double fDist;
	CString str;
	if (m_bRemote == FALSE)	return;
	//iPulse = GetDlgItemInt(IDC_PULSE3);
	//iRet = nmc2_abs_move(m_nPaixCtlNo, 2, iPulse);
	GetDlgItemText(IDC_PULSE3, str);
	fDist = atof(str);
	iRet = nmc_AbsMove(m_nPaixCtlNo, 2, fDist);

	switch (iRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnAbs4()
{
	// TODO: Add your control notification handler code here
	int iRet;
	double fDist;
	CString str;
	if (m_bRemote == FALSE)	return;
	//iPulse = GetDlgItemInt(IDC_PULSE4);
	//iRet = nmc2_abs_move(m_nPaixCtlNo, 0, iPulse);
	GetDlgItemText(IDC_PULSE4, str);
	fDist = atof(str);
	iRet = nmc_AbsMove(m_nPaixCtlNo, 3, fDist);

	switch (iRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnStop1()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)	return;
	/*
		즉시 정지는
		nmc_SuddenStop(m_nPaixCtlNo,0);
	*/
	nmc_DecStop(m_nPaixCtlNo, 0);
}

void CMotionDlg::OnBtnStop2()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)	return;
	nmc_DecStop(m_nPaixCtlNo, 1);
}

void CMotionDlg::OnBtnStop3()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)	return;
	nmc_DecStop(m_nPaixCtlNo, 2);
}

void CMotionDlg::OnBtnStop4()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)	return;
	nmc_DecStop(m_nPaixCtlNo, 3);
}

BOOL CMotionDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_hThreadId)
	{
		//if( m_nTimerId )
		//{
		//	KillTimer(m_nTimerId);
	//		m_nTimerId = 0;
	//	}
		m_bThreadRun = TRUE;
		WaitForSingleObject(m_hThreadId, 1000);
		CloseHandle(m_hThreadId);
		m_hThreadId = NULL;
		nmc_CloseDevice(m_nPaixCtlNo);
	}
	return CDialog::DestroyWindow();
}

LRESULT CMotionDlg::UmJog(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)	return FALSE;
	int iRet;
	switch (lParam)
	{
	case 0:	// CW
	case 1:	//	CCW
		iRet = nmc_JogMove(m_nPaixCtlNo, wParam, (short)lParam);
		break;
	case -1:	// STOP
		iRet = nmc_DecStop(m_nPaixCtlNo, wParam);
		break;
	}
	switch (iRet)
	{
	case 1:
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
		return FALSE;
	}
	return TRUE;
}

void CMotionDlg::OnBtnCurrent1()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BTN_CURRENT1, str);

	if (str == "ON")
	{
		nRet = nmc_SetCurrentOn(m_nPaixCtlNo, 0, 1);
		SetDlgItemText(IDC_BTN_CURRENT1, "OFF");
	}
	else
	{
		nRet = nmc_SetCurrentOn(m_nPaixCtlNo, 0, 0);
		SetDlgItemText(IDC_BTN_CURRENT1, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnCurrent2()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BTN_CURRENT2, str);

	if (str == "ON")
	{
		nRet = nmc_SetCurrentOn(m_nPaixCtlNo, 1, 1);
		SetDlgItemText(IDC_BTN_CURRENT2, "OFF");
	}
	else
	{
		nRet = nmc_SetCurrentOn(m_nPaixCtlNo, 1, 0);
		SetDlgItemText(IDC_BTN_CURRENT2, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}

}

void CMotionDlg::OnBtnCurrent3()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BTN_CURRENT3, str);

	if (str == "ON")
	{
		nRet = nmc_SetCurrentOn(m_nPaixCtlNo, 2, 1);
		SetDlgItemText(IDC_BTN_CURRENT3, "OFF");
	}
	else
	{
		nRet = nmc_SetCurrentOn(m_nPaixCtlNo, 2, 0);
		SetDlgItemText(IDC_BTN_CURRENT3, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}

}

void CMotionDlg::OnBtnCurrent4()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BTN_CURRENT4, str);

	if (str == "ON")
	{
		nRet = nmc_SetCurrentOn(m_nPaixCtlNo, 3, 1);
		SetDlgItemText(IDC_BTN_CURRENT4, "OFF");
	}
	else
	{
		nRet = nmc_SetCurrentOn(m_nPaixCtlNo, 3, 0);
		SetDlgItemText(IDC_BTN_CURRENT4, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}

}

void CMotionDlg::OnBtnServo1()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BTN_SERVO1, str);

	if (str == "ON")
	{
		nRet = nmc_SetServoOn(m_nPaixCtlNo, 0, 1);
		SetDlgItemText(IDC_BTN_SERVO1, "OFF");
	}
	else
	{
		nRet = nmc_SetServoOn(m_nPaixCtlNo, 0, 0);
		SetDlgItemText(IDC_BTN_SERVO1, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnServo2()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BTN_SERVO2, str);

	if (str == "ON")
	{
		nRet = nmc_SetServoOn(m_nPaixCtlNo, 1, 1);
		SetDlgItemText(IDC_BTN_SERVO2, "OFF");
	}
	else
	{
		nRet = nmc_SetServoOn(m_nPaixCtlNo, 1, 0);
		SetDlgItemText(IDC_BTN_SERVO2, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnServo3()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BTN_SERVO3, str);

	if (str == "ON")
	{
		nRet = nmc_SetServoOn(m_nPaixCtlNo, 2, 1);
		SetDlgItemText(IDC_BTN_SERVO3, "OFF");
	}
	else
	{
		nRet = nmc_SetServoOn(m_nPaixCtlNo, 2, 0);
		SetDlgItemText(IDC_BTN_SERVO3, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnServo4()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BTN_SERVO4, str);

	if (str == "ON")
	{
		nRet = nmc_SetServoOn(m_nPaixCtlNo, 3, 1);
		SetDlgItemText(IDC_BTN_SERVO4, "OFF");
	}
	else
	{
		nRet = nmc_SetServoOn(m_nPaixCtlNo, 3, 0);
		SetDlgItemText(IDC_BTN_SERVO4, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnAlarm1()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BTN_ALARM1, str);

	if (str == "ON")
	{
		nRet = nmc_SetAlarmResetOn(m_nPaixCtlNo, 0, 1);
		SetDlgItemText(IDC_BTN_ALARM1, "OFF");
	}
	else
	{
		nRet = nmc_SetAlarmResetOn(m_nPaixCtlNo, 0, 0);
		SetDlgItemText(IDC_BTN_ALARM1, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnAlarm2()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BTN_ALARM2, str);

	if (str == "ON")
	{
		nRet = nmc_SetAlarmResetOn(m_nPaixCtlNo, 1, 1);
		SetDlgItemText(IDC_BTN_ALARM2, "OFF");
	}
	else
	{
		nRet = nmc_SetAlarmResetOn(m_nPaixCtlNo, 1, 0);
		SetDlgItemText(IDC_BTN_ALARM2, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnAlarm3()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BTN_ALARM3, str);

	if (str == "ON")
	{
		nRet = nmc_SetAlarmResetOn(m_nPaixCtlNo, 2, 1);
		SetDlgItemText(IDC_BTN_ALARM3, "OFF");
	}
	else
	{
		nRet = nmc_SetAlarmResetOn(m_nPaixCtlNo, 2, 0);
		SetDlgItemText(IDC_BTN_ALARM3, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBtnAlarm4()
{
	// TODO: Add your control notification handler code here
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BTN_ALARM4, str);

	if (str == "ON")
	{
		nRet = nmc_SetAlarmResetOn(m_nPaixCtlNo, 3, 1);
		SetDlgItemText(IDC_BTN_ALARM4, "OFF");
	}
	else
	{
		nRet = nmc_SetAlarmResetOn(m_nPaixCtlNo, 3, 0);
		SetDlgItemText(IDC_BTN_ALARM4, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::ReadSeonsor(void)
{
	if (m_bRemote == FALSE)	return;
	int iRet;

	NMCAXESEXPR nmcData;
	iRet = nmc_GetAxesExpress(m_nPaixCtlNo, &nmcData);

	switch (iRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
		return;
	default:
		return;
	case 0:
		break;
	}

	// SimpleMotionDlg 에서 Limit 표시
	if (m_pPunchingMachineDlg->m_bCheckSimpleMotionDlg)
	{
		m_pPunchingMachineDlg->GetPtrSimpleMotionDlg()->m_StaticAxis0_MLimit.SetBkColor(nmcData.nMLimit[0] ? clRed : clBlack);
		m_pPunchingMachineDlg->GetPtrSimpleMotionDlg()->m_StaticAxis0_PLimit.SetBkColor(nmcData.nPLimit[0] ? clRed : clBlack);
		m_pPunchingMachineDlg->GetPtrSimpleMotionDlg()->m_StaticAxis1_MLimit.SetBkColor(nmcData.nMLimit[1] ? clRed : clBlack);
		m_pPunchingMachineDlg->GetPtrSimpleMotionDlg()->m_StaticAxis1_PLimit.SetBkColor(nmcData.nPLimit[1] ? clRed : clBlack);
		m_pPunchingMachineDlg->GetPtrSimpleMotionDlg()->m_StaticAxis2_MLimit.SetBkColor(nmcData.nMLimit[2] ? clRed : clBlack);
		m_pPunchingMachineDlg->GetPtrSimpleMotionDlg()->m_StaticAxis2_PLimit.SetBkColor(nmcData.nPLimit[2] ? clRed : clBlack);
	}

	// EquipmentDlg 에서 Limit 표시
	if (m_pPunchingMachineDlg->GetPtrEquipmentDlg()->IsWindowVisible())
	{
		m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_StaticAxis0_MLimit.SetBkColor(nmcData.nMLimit[0] ? clRed : clBlack);
		m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_StaticAxis0_PLimit.SetBkColor(nmcData.nPLimit[0] ? clRed : clBlack);
		m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_StaticAxis1_MLimit.SetBkColor(nmcData.nMLimit[1] ? clRed : clBlack);
		m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_StaticAxis1_PLimit.SetBkColor(nmcData.nPLimit[1] ? clRed : clBlack);
		m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_StaticAxis2_MLimit.SetBkColor(nmcData.nMLimit[2] ? clRed : clBlack);
		m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_StaticAxis2_PLimit.SetBkColor(nmcData.nPLimit[2] ? clRed : clBlack);
	}


	if (IsWindowVisible())
	{
		m_staticBusy4.SetBkColor(nmcData.nBusy[3] ? clRed : clBlack);
		m_staticBusy3.SetBkColor(nmcData.nBusy[2] ? clRed : clBlack);
		m_staticBusy2.SetBkColor(nmcData.nBusy[1] ? clRed : clBlack);
		m_staticBusy1.SetBkColor(nmcData.nBusy[0] ? clRed : clBlack);

		m_staticPLimit4.SetBkColor(nmcData.nPLimit[3] ? clRed : clBlack);
		m_staticPLimit3.SetBkColor(nmcData.nPLimit[2] ? clRed : clBlack);
		m_staticPLimit2.SetBkColor(nmcData.nPLimit[1] ? clRed : clBlack);
		m_staticPLimit1.SetBkColor(nmcData.nPLimit[0] ? clRed : clBlack);

		m_staticNear4.SetBkColor(nmcData.nNear[3] ? clRed : clBlack);
		m_staticNear3.SetBkColor(nmcData.nNear[2] ? clRed : clBlack);
		m_staticNear2.SetBkColor(nmcData.nNear[1] ? clRed : clBlack);
		m_staticNear1.SetBkColor(nmcData.nNear[0] ? clRed : clBlack);

		m_staticMLimit4.SetBkColor(nmcData.nMLimit[3] ? clRed : clBlack);
		m_staticMLimit3.SetBkColor(nmcData.nMLimit[2] ? clRed : clBlack);
		m_staticMLimit2.SetBkColor(nmcData.nMLimit[1] ? clRed : clBlack);
		m_staticMLimit1.SetBkColor(nmcData.nMLimit[0] ? clRed : clBlack);

		m_staticHome4.SetBkColor(nmcData.nHome[3] ? clRed : clBlack);
		m_staticHome3.SetBkColor(nmcData.nHome[2] ? clRed : clBlack);
		m_staticHome2.SetBkColor(nmcData.nHome[1] ? clRed : clBlack);
		m_staticHome1.SetBkColor(nmcData.nHome[0] ? clRed : clBlack);

		m_staticEmer.SetBkColor(nmcData.nEmer[0] ? clRed : clBlack);

		m_staticAlarm4.SetBkColor(nmcData.nAlarm[3] ? clRed : clBlack);
		m_staticAlarm3.SetBkColor(nmcData.nAlarm[2] ? clRed : clBlack);
		m_staticAlarm2.SetBkColor(nmcData.nAlarm[1] ? clRed : clBlack);
		m_staticAlarm1.SetBkColor(nmcData.nAlarm[0] ? clRed : clBlack);

		m_staticInpos1.SetBkColor(nmcData.nInpo[0] ? clRed : clBlack);
		m_staticInpos2.SetBkColor(nmcData.nInpo[1] ? clRed : clBlack);
		m_staticInpos3.SetBkColor(nmcData.nInpo[2] ? clRed : clBlack);
		m_staticInpos4.SetBkColor(nmcData.nInpo[3] ? clRed : clBlack);

		CString str;

		str.Format("%.3f", nmcData.dCmd[0]);
		SetDlgItemText(IDC_CMD1, str);
		str.Format("%.3f", nmcData.dCmd[1]);
		SetDlgItemText(IDC_CMD2, str);
		str.Format("%.3f", nmcData.dCmd[2]);
		SetDlgItemText(IDC_CMD3, str);
		str.Format("%.3f", nmcData.dCmd[3]);
		SetDlgItemText(IDC_CMD4, str);

		str.Format("%.3f", nmcData.dEnc[0]);
		SetDlgItemText(IDC_ENC1, str);
		str.Format("%.3f", nmcData.dEnc[1]);
		SetDlgItemText(IDC_ENC2, str);
		str.Format("%.3f", nmcData.dEnc[2]);
		SetDlgItemText(IDC_ENC3, str);
		str.Format("%.3f", nmcData.dEnc[3]);
		SetDlgItemText(IDC_ENC4, str);


		NMCAXESMOTIONOUT CurOutStatus;
		iRet = nmc_GetAxesMotionOut(m_nPaixCtlNo, &CurOutStatus);

		switch (iRet)
		{
		case NMC_NOTCONNECT:
			m_bRemote = FALSE;
			return;
		default:
			return;
		case 0:
			break;
		}
		CBitmapButton* pButton = NULL;
		for (int i = 0; i < 4; i++)
		{

			pButton = (CBitmapButton*)GetDlgItem(IDC_BTN_CURRENT1 + i);
			if (pButton)
			{
				if (CurOutStatus.nCurrentOn[i] != OutStatus.nCurrentOn[i])
					pButton->SetBitmap(CurOutStatus.nCurrentOn[i] == 1 ? m_OutOn : m_OutOff);
				OutStatus.nCurrentOn[i] = CurOutStatus.nCurrentOn[i];
			}



			pButton = (CBitmapButton*)GetDlgItem(IDC_BTN_SERVO1 + i);
			if (pButton)
			{
				if (CurOutStatus.nServoOn[i] != OutStatus.nServoOn[i])
					pButton->SetBitmap(CurOutStatus.nServoOn[i] == 1 ? m_OutOn : m_OutOff);
				OutStatus.nServoOn[i] = CurOutStatus.nServoOn[i];
			}


			pButton = (CBitmapButton*)GetDlgItem(IDC_BUTTON_DCC1 + i);
			if (pButton)
			{
				if (CurOutStatus.nDCCOn[i] != OutStatus.nDCCOn[i])
					pButton->SetBitmap(CurOutStatus.nDCCOn[i] == 1 ? m_OutOn : m_OutOff);
				OutStatus.nDCCOn[i] = CurOutStatus.nDCCOn[i];
			}

			pButton = (CBitmapButton*)GetDlgItem(IDC_BTN_ALARM1 + i);
			if (pButton)
			{
				if (CurOutStatus.nAlarmResetOn[i] != OutStatus.nAlarmResetOn[i])
					pButton->SetBitmap(CurOutStatus.nAlarmResetOn[i] == 1 ? m_OutOn : m_OutOff);
				OutStatus.nAlarmResetOn[i] = CurOutStatus.nAlarmResetOn[i];
			}
		}
		pButton = NULL;
	}
}

void CMotionDlg::OnSelendokCbPlimit1()
{
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_PLIMIT1);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetPlusLimitLogic(m_nPaixCtlNo, 0, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}

void CMotionDlg::OnSelendokCbPlimit2()
{
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_PLIMIT2);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetPlusLimitLogic(m_nPaixCtlNo, 1, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}

void CMotionDlg::OnSelendokCbPlimit3()
{
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_PLIMIT3);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetPlusLimitLogic(m_nPaixCtlNo, 2, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}

void CMotionDlg::OnSelendokCbPlimit4()
{
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_PLIMIT4);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetPlusLimitLogic(m_nPaixCtlNo, 3, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}


void CMotionDlg::OnBnClickedButtonDcc1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BUTTON_DCC1, str);

	if (str == "ON")
	{
		nRet = nmc_SetDccOn(m_nPaixCtlNo, 0, 1);
		SetDlgItemText(IDC_BUTTON_DCC1, "OFF");
	}
	else
	{
		nRet = nmc_SetDccOn(m_nPaixCtlNo, 0, 0);
		SetDlgItemText(IDC_BUTTON_DCC1, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBnClickedButtonDcc2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BUTTON_DCC2, str);

	if (str == "ON")
	{
		nRet = nmc_SetDccOn(m_nPaixCtlNo, 1, 1);
		SetDlgItemText(IDC_BUTTON_DCC2, "OFF");
	}
	else
	{
		nRet = nmc_SetDccOn(m_nPaixCtlNo, 1, 0);
		SetDlgItemText(IDC_BUTTON_DCC2, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBnClickedButtonDcc3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BUTTON_DCC3, str);

	if (str == "ON")
	{
		nRet = nmc_SetDccOn(m_nPaixCtlNo, 2, 1);
		SetDlgItemText(IDC_BUTTON_DCC3, "OFF");
	}
	else
	{
		nRet = nmc_SetDccOn(m_nPaixCtlNo, 2, 0);
		SetDlgItemText(IDC_BUTTON_DCC3, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBnClickedButtonDcc4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bRemote == FALSE)    return;
	CString str;
	int nRet;
	GetDlgItemText(IDC_BUTTON_DCC4, str);

	if (str == "ON")
	{
		nRet = nmc_SetDccOn(m_nPaixCtlNo, 3, 1);
		SetDlgItemText(IDC_BUTTON_DCC4, "OFF");
	}
	else
	{
		nRet = nmc_SetDccOn(m_nPaixCtlNo, 3, 0);
		SetDlgItemText(IDC_BUTTON_DCC4, "ON");
	}
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnCbnSelendokCbAlarm1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_ALARM1);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetAlarmLogic(m_nPaixCtlNo, 0, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
	return;
}

void CMotionDlg::OnCbnSelendokCbAlarm2()
{
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_ALARM2);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetAlarmLogic(m_nPaixCtlNo, 1, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
}

void CMotionDlg::OnCbnSelendokCbAlarm3()
{
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_ALARM3);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetAlarmLogic(m_nPaixCtlNo, 2, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
}

void CMotionDlg::OnCbnSelendokCbAlarm4()
{
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_ALARM4);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetAlarmLogic(m_nPaixCtlNo, 3, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
}

void CMotionDlg::OnCbnSelendokCbInpos1()
{
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_INPOS1);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetInPoLogic(m_nPaixCtlNo, 0, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
}

void CMotionDlg::OnCbnSelendokCbInpos2()
{
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_INPOS2);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetInPoLogic(m_nPaixCtlNo, 1, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
}

void CMotionDlg::OnCbnSelendokCbInpos3()
{
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_INPOS3);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetInPoLogic(m_nPaixCtlNo, 2, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
}

void CMotionDlg::OnCbnSelendokCbInpos4()
{
	int nRet = 0, nLogic;
	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_INPOS4);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetInPoLogic(m_nPaixCtlNo, 3, nLogic);

	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
}


void CMotionDlg::OnCbnSelendokCbPulseMode1()
{
	int nRet = 0, nLogic;

	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;
	pCombo = (CComboBox*)GetDlgItem(IDC_CB_PULSE_MODE1);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetPulseMode(m_nPaixCtlNo, 0, nLogic);
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
	pCombo = NULL;
}

void CMotionDlg::OnCbnSelendokCbPulseMode2()
{
	int nRet = 0, nLogic;

	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;
	pCombo = (CComboBox*)GetDlgItem(IDC_CB_PULSE_MODE2);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetPulseMode(m_nPaixCtlNo, 1, nLogic);
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnCbnSelendokCbPulseMode3()
{
	int nRet = 0, nLogic;

	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;
	pCombo = (CComboBox*)GetDlgItem(IDC_CB_PULSE_MODE3);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetPulseMode(m_nPaixCtlNo, 2, nLogic);
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnCbnSelendokCbPulseMode4()
{
	int nRet = 0, nLogic;

	CComboBox* pCombo = NULL;

	if (m_bRemote == FALSE)	return;
	pCombo = (CComboBox*)GetDlgItem(IDC_CB_PULSE_MODE4);
	VERIFY(pCombo);
	nLogic = pCombo->GetCurSel();

	nRet = nmc_SetPulseMode(m_nPaixCtlNo, 3, nLogic);
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double pulse[4];
	short nAxis[4];
	int nCount = 0;
	int nCheck;
	short nRet;
	CString str;

	nCheck = IsDlgButtonChecked(IDC_MO1);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE1, str);
		pulse[nCount] = -atof(str);
		nAxis[nCount] = 0;
		nCount++;
	}
	nCheck = IsDlgButtonChecked(IDC_MO2);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE2, str);
		pulse[nCount] = -atof(str);
		nAxis[nCount] = 1;
		nCount++;
	}
	nCheck = IsDlgButtonChecked(IDC_MO3);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE3, str);
		pulse[nCount] = -atof(str);
		nAxis[nCount] = 2;
		nCount++;
	}
	nCheck = IsDlgButtonChecked(IDC_MO4);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE4, str);
		pulse[nCount] = -atof(str);

		nAxis[nCount] = 3;
		nCount++;
	}
	nRet = nmc_VarRelMove(m_nPaixCtlNo, nCount, nAxis, pulse);
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double pulse[4];
	short nAxis[4];
	int nCount = 0;
	int nCheck;
	short nRet;
	CString str;

	nCheck = IsDlgButtonChecked(IDC_MO1);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE1, str);
		pulse[nCount] = atof(str);

		nAxis[nCount] = 0;
		nCount++;
	}
	nCheck = IsDlgButtonChecked(IDC_MO2);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE2, str);
		pulse[nCount] = atof(str);
		nAxis[nCount] = 1;
		nCount++;
	}
	nCheck = IsDlgButtonChecked(IDC_MO3);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE3, str);
		pulse[nCount] = atof(str);
		nAxis[nCount] = 2;
		nCount++;
	}
	nCheck = IsDlgButtonChecked(IDC_MO4);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE4, str);
		pulse[nCount] = atof(str);
		nAxis[nCount] = 3;
		nCount++;
	}
	nRet = nmc_VarRelMove(m_nPaixCtlNo, nCount, nAxis, pulse);
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBnClickedButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double pulse[4];
	short nAxis[4];
	int nCount = 0;
	int nCheck;
	short nRet;
	CString str;
	nCheck = IsDlgButtonChecked(IDC_MO1);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE1, str);
		pulse[nCount] = -atof(str);
		nAxis[nCount] = 0;
		nCount++;
	}
	nCheck = IsDlgButtonChecked(IDC_MO2);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE2, str);
		pulse[nCount] = -atof(str);
		nAxis[nCount] = 1;
		nCount++;
	}
	nCheck = IsDlgButtonChecked(IDC_MO3);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE3, str);
		pulse[nCount] = -atof(str);
		nAxis[nCount] = 2;
		nCount++;
	}
	nCheck = IsDlgButtonChecked(IDC_MO4);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE4, str);
		pulse[nCount] = -atof(str);
		nAxis[nCount] = 3;
		nCount++;
	}
	nRet = nmc_VarRelMove(m_nPaixCtlNo, nCount, nAxis, pulse);
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}

void CMotionDlg::OnBnClickedButton9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double pulse[4];
	short nAxis[4];
	int nCount = 0;
	int nCheck;
	short nRet;
	CString str;
	nCheck = IsDlgButtonChecked(IDC_MO1);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE1, str);
		pulse[nCount] = atof(str);
		nAxis[nCount] = 0;
		nCount++;
	}
	nCheck = IsDlgButtonChecked(IDC_MO2);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE2, str);
		pulse[nCount] = atof(str);
		nAxis[nCount] = 1;
		nCount++;
	}
	nCheck = IsDlgButtonChecked(IDC_MO3);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE3, str);
		pulse[nCount] = atof(str);
		nAxis[nCount] = 2;
		nCount++;
	}
	nCheck = IsDlgButtonChecked(IDC_MO4);
	if (nCheck == 1)
	{
		GetDlgItemText(IDC_PULSE4, str);
		pulse[nCount] = atof(str);
		nAxis[nCount] = 3;
		nCount++;
	}
	nRet = nmc_VarAbsMove(m_nPaixCtlNo, nCount, nAxis, pulse);
	switch (nRet)
	{
	case NMC_NOTCONNECT:
		m_bRemote = FALSE;
	}
}


void CMotionDlg::OnBnClickedButton10()
{
	short nHomeMode, nEndMode;
	long loffset;
	short nret;
	nHomeMode = ((CComboBox*)GetDlgItem(IDC_CB_HOME1))->GetCurSel();
	nEndMode = ((CComboBox*)GetDlgItem(IDC_CB_CLEAR1))->GetCurSel();
	loffset = GetDlgItemInt(IDC_EDIT_OFFSET1);
	nret = nmc_HomeMove(m_nPaixCtlNo, 0, nHomeMode, nEndMode, loffset, 0);

}

void CMotionDlg::OnBnClickedButton11()
{
	short nHomeMode, nEndMode;
	long loffset;
	short nret;
	nHomeMode = ((CComboBox*)GetDlgItem(IDC_CB_HOME2))->GetCurSel();
	nEndMode = ((CComboBox*)GetDlgItem(IDC_CB_CLEAR2))->GetCurSel();
	loffset = GetDlgItemInt(IDC_EDIT_OFFSET2);
	nret = nmc_HomeMove(m_nPaixCtlNo, 1, nHomeMode, nEndMode, loffset, 0);
}

void CMotionDlg::OnBnClickedButton12()
{
	short nHomeMode, nEndMode;
	long loffset;
	short nret;
	nHomeMode = ((CComboBox*)GetDlgItem(IDC_CB_HOME3))->GetCurSel();
	nEndMode = ((CComboBox*)GetDlgItem(IDC_CB_CLEAR3))->GetCurSel();
	loffset = GetDlgItemInt(IDC_EDIT_OFFSET3);
	nret = nmc_HomeMove(m_nPaixCtlNo, 2, nHomeMode, nEndMode, loffset, 0);
}

void CMotionDlg::OnBnClickedButton13()
{
	short nHomeMode, nEndMode;
	long loffset;
	short nret;
	nHomeMode = ((CComboBox*)GetDlgItem(IDC_CB_HOME4))->GetCurSel();
	nEndMode = ((CComboBox*)GetDlgItem(IDC_CB_CLEAR4))->GetCurSel();
	loffset = GetDlgItemInt(IDC_EDIT_OFFSET4);
	nret = nmc_HomeMove(m_nPaixCtlNo, 3, nHomeMode, nEndMode, loffset, 0);
}

void CMotionDlg::OnStnDblclickCmd1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bRemote == false)	return;
	nmc_SetCmdPos(m_nPaixCtlNo, 0, 0);
}

void CMotionDlg::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//
	if (m_bRemote == false)	return;
	CDistDlg dlg(m_nPaixCtlNo);
	dlg.DoModal();
}


void CMotionDlg::OnBnClickedButtonDcc5()
{
	short GROUP = 0;
	unsigned long nExeNo;
	short	nRet;

	if (m_bRemote == false)	return;

	nRet = nmc_ContiSetNodeClear(m_nPaixCtlNo, GROUP);               // 노드 버퍼의 초기화
	if (nRet != NMC_OK) return;
	// 그룹0,삼각파형 방지X, 예외처리:종료, 연속 보간축 : 0축, 1축 (2축 연속 보간만 수행)
	//      최대 구동 속도 : 300,출력 포트:지정 안함,출력 pin mask:0,종료 시 출력 값:0
	nRet = nmc_ContiSetMode(m_nPaixCtlNo, GROUP, 0, 0, 0, 1, -1, 300, 2, 0, 0);
	if (nRet != NMC_OK) return;

	//==================== 노드 등록 ====================
	// 1st 노드[직선] === 위치:(0,200), 시작:50, 가속:250, 감속: 0, 구동속도:100
	nRet = nmc_ContiAddNodeLine2Axis(m_nPaixCtlNo, GROUP, 0, 200, 50, 250, 0, 100, -1);
	if (nRet != NMC_OK) return;

	// 2nd 노드[원호] === 중심위치:(100,200), 시작:100, 가속:750, 감속: 500, 구동속도:300, 회전각:-90
	nRet = nmc_ContiAddNodeArc(m_nPaixCtlNo, GROUP, 100, 200, 100, 750, 500, 300, -90, -1);
	if (nRet != NMC_OK) return;

	// 3rd 노드[직선] === 위치:(300,300), 시작:200, 가속:250, 감속: 0, 구동속도:200
	nRet = nmc_ContiAddNodeLine2Axis(m_nPaixCtlNo, GROUP, 300, 300, 200, 250, 0, 200, -1);
	if (nRet != NMC_OK) return;

	// 4th 노드[직선] === 위치:(300,100), 시작:200, 가속:750, 감속: 0, 구동속도:250
	nRet = nmc_ContiAddNodeLine2Axis(m_nPaixCtlNo, GROUP, 300, 100, 200, 750, 0, 250, -1);
	if (nRet != NMC_OK) return;

	// 5th 노드[원호]=== 중심위치:(200,100), 시작:250, 가속:750, 감속: 0, 구동속도:150, 회전각:-90
	nRet = nmc_ContiAddNodeArc(m_nPaixCtlNo, GROUP, 200, 100, 250, 750, 0, 150, -90, -1);
	if (nRet != NMC_OK) return;

	// 6th 노드[직선]=== 위치:(0,0), 시작:150, 가속:125, 감속: 375, 구동속도:200
	nRet = nmc_ContiAddNodeLine2Axis(m_nPaixCtlNo, GROUP, 0, 0, 150, 125, 375, 200, -1);
	if (nRet != NMC_OK) return;

	// 노드 전송 종료
	nRet = nmc_ContiSetCloseNode(m_nPaixCtlNo, GROUP);
	if (nRet != NMC_OK) return;

	// ==================== 연속 보간 실행 ====================
	// 연속보간 실행
	nRet = nmc_ContiRunStop(m_nPaixCtlNo, GROUP, 1);
	if (nRet != NMC_OK) return;
	// =================== 보간이동 종료 대기 ==================
	NMCCONTISTATUS NmcContiStatus;

	while (true) {
		/* 본 while구문의 내용은 사용하실 프로그램 내 주기적으로 실행 되는 곳에서 실행 하셔도 됩니다.
		단, 노드 실행 속도 보다는 갱신 속도가 빨라야 연속 보간이 끊김없이 실행 됩니다. */
		nRet = nmc_ContiGetStatus(m_nPaixCtlNo, &NmcContiStatus);
		if (nRet != NMC_OK) return;

		// 그룹 0의 현재까지 실행한 노드의 수
		nExeNo = NmcContiStatus.uiContiExecutionNum[GROUP];

		if (NmcContiStatus.nContiRun[GROUP] == 0) break;     // 연속보간 이동 끝, while 구문 탈출
	}
}



/*
	멤버함수
*/

void CMotionDlg::SetScreenRect(CRect rect)
{
	m_RectMotionDlg = rect;
}

UINT CMotionDlg::ThreadProc(void* pVoid)
{
	CMotionDlg* pdlg = (CMotionDlg*)pVoid;
	CPunchingMachineDlg* pPunchingMachineDlg = (CPunchingMachineDlg*)AfxGetMainWnd();
	while (!pdlg->m_bThreadRun)
	{
		//Sleep(50);
		pPunchingMachineDlg->Wait(50);
		pdlg->ReadSeonsor();
	}

	return 0;
}

void CMotionDlg::Init()
{

	SetDlgItemInt(IDC_EDIT_EQUIP, DEVICENUM);

	((CComboBox*)GetDlgItem(IDC_CB_EMER))->SetCurSel(0);

	for (int i = 0; i < 4; i++)
	{
		((CComboBox*)GetDlgItem(IDC_CB_NEAR1 + i))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_CB_LIMIT1 + i))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_CB_PLIMIT1 + i))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_CB_HOME1 + i))->SetCurSel(3);
		((CComboBox*)GetDlgItem(IDC_CB_ALARM1 + i))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_CB_INPOS1 + i))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_CB_PULSE_MODE1 + i))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_CB_HOME1 + i))->SetCurSel(3);
		((CComboBox*)GetDlgItem(IDC_CB_CLEAR1 + i))->SetCurSel(0);
		CheckDlgButton(IDC_MO1 + i, true);
		SetDlgItemInt(IDC_EDIT_START1 + i, 50);
		SetDlgItemInt(IDC_EDIT_ACC1 + i, 2000);
		SetDlgItemInt(IDC_EDIT_MAX1 + i, 2000);
		SetDlgItemInt(IDC_EDIT_DEC1 + i, 2000);
		SetDlgItemInt(IDC_EDIT_OFFSET1 + i, 0);
		SetDlgItemInt(IDC_PULSE1 + i, 2000);
	}

	//*
	m_staticBusy4.SetBkColor(clSilver);
	m_staticBusy3.SetBkColor(clSilver);
	m_staticBusy2.SetBkColor(clSilver);
	m_staticBusy1.SetBkColor(clSilver);
	m_staticPLimit4.SetBkColor(clSilver);
	m_staticPLimit3.SetBkColor(clSilver);
	m_staticPLimit2.SetBkColor(clSilver);
	m_staticPLimit1.SetBkColor(clSilver);
	m_staticNear4.SetBkColor(clSilver);
	m_staticNear3.SetBkColor(clSilver);
	m_staticNear2.SetBkColor(clSilver);
	m_staticNear1.SetBkColor(clSilver);
	m_staticMLimit4.SetBkColor(clSilver);
	m_staticMLimit3.SetBkColor(clSilver);
	m_staticMLimit2.SetBkColor(clSilver);
	m_staticMLimit1.SetBkColor(clSilver);
	m_staticHome4.SetBkColor(clSilver);
	m_staticHome3.SetBkColor(clSilver);
	m_staticHome2.SetBkColor(clSilver);
	m_staticHome1.SetBkColor(clSilver);
	m_staticEmer.SetBkColor(clSilver);

	m_staticAlarm4.SetBkColor(clSilver);
	m_staticAlarm3.SetBkColor(clSilver);
	m_staticAlarm2.SetBkColor(clSilver);
	m_staticAlarm1.SetBkColor(clSilver);

	m_staticInpos1.SetBkColor(clSilver);
	m_staticInpos2.SetBkColor(clSilver);
	m_staticInpos3.SetBkColor(clSilver);
	m_staticInpos4.SetBkColor(clSilver);


	// 1 - CCW
	m_BtnCcw4.SetAxisDir(3, 1);
	m_BtnCcw3.SetAxisDir(2, 1);
	m_BtnCcw2.SetAxisDir(1, 1);
	m_BtnCcw1.SetAxisDir(0, 1);

	m_BtnCw4.SetAxisDir(3, 0);
	m_BtnCw3.SetAxisDir(2, 0);
	m_BtnCw2.SetAxisDir(1, 0);
	m_BtnCw1.SetAxisDir(0, 0);

	m_BtnCcw4.SetParentHwnd(m_hWnd);
	m_BtnCcw3.SetParentHwnd(m_hWnd);
	m_BtnCcw2.SetParentHwnd(m_hWnd);
	m_BtnCcw1.SetParentHwnd(m_hWnd);

	m_BtnCw4.SetParentHwnd(m_hWnd);
	m_BtnCw3.SetParentHwnd(m_hWnd);;
	m_BtnCw2.SetParentHwnd(m_hWnd);
	m_BtnCw1.SetParentHwnd(m_hWnd);

	m_OutOn.LoadBitmap(IDB_BITMAP_RED); // 비트맵 올리고...
	m_OutOff.LoadBitmap(IDB_BITMAP_GRAY); // 비트맵 올리고...


	// 부모다이얼로그 포인터 획득
	m_pPunchingMachineDlg = (CPunchingMachineDlg*)AfxGetMainWnd();

	OnBtnOpen();

	SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(m_RectMotionDlg.Width(), m_RectMotionDlg.Height()));
}

int CMotionDlg::GetPaixCtlNo()
{
	return m_nPaixCtlNo;
}

void CMotionDlg::InitSpeed()
{
	short nRet;
	int iStart = 0;
	int iAcc = 0;
	int iDec = 0;

	// 0번 축 : S자 속도 지정(시작 속도:10, 가속:1000, 감속:1000, 구동 속도:2000)
	iStart = GetDlgItemInt(IDC_EDIT_START1);
	iAcc = GetDlgItemInt(IDC_EDIT_ACC1);
	iDec = GetDlgItemInt(IDC_EDIT_DEC1);
	nRet = nmc_SetSCurveSpeed(m_nPaixCtlNo, 0, iStart, iAcc, iDec, 2000);
	nRet = nmc_SetAccSpeed(m_nPaixCtlNo, 0, iAcc);
	nRet = nmc_SetDecSpeed(m_nPaixCtlNo, 0, iDec);


	// 1번 축 : S자 속도 지정(시작 속도:10, 가속:1000, 감속:1000, 구동 속도:4000)
	iStart = GetDlgItemInt(IDC_EDIT_START2);
	iAcc = GetDlgItemInt(IDC_EDIT_ACC2);
	iDec = GetDlgItemInt(IDC_EDIT_DEC2);
	nRet = nmc_SetSCurveSpeed(m_nPaixCtlNo, 1, iStart, iAcc, iDec, 4000);
	nRet = nmc_SetAccSpeed(m_nPaixCtlNo, 1, iAcc);
	nRet = nmc_SetDecSpeed(m_nPaixCtlNo, 1, iDec);

	// 2번 축 : S자 속도 지정(시작 속도:10, 가속:1000, 감속:1000, 구동 속도:2000)
	iStart = GetDlgItemInt(IDC_EDIT_START3);
	iAcc = GetDlgItemInt(IDC_EDIT_ACC3);
	iDec = GetDlgItemInt(IDC_EDIT_DEC3);
	nRet = nmc_SetSCurveSpeed(m_nPaixCtlNo, 2, iStart, iAcc, iDec, 2000);
	nRet = nmc_SetAccSpeed(m_nPaixCtlNo, 2, iAcc);
	nRet = nmc_SetDecSpeed(m_nPaixCtlNo, 2, iDec);

	// Set Home Speed 
	nRet = nmc_SetHomeSpeed(m_nPaixCtlNo, 0, 2000, 1500, 1000);
	nRet = nmc_SetHomeSpeed(m_nPaixCtlNo, 1, 2000, 1500, 1000);
	nRet = nmc_SetHomeSpeed(m_nPaixCtlNo, 2, 2000, 1500, 1000);
}


void CMotionDlg::UpdateScrollInfo(UINT nType, int cx, int cy)
{
	int nScrollMaxV = 0;
	int nScrollMaxH = 0;
	int nTotalHeight = 0;
	int nTotalWidth = 0;
	int nDelta = 0;
	SCROLLINFO siV = { 0, }, siH = { 0, };

	m_nPageSizeV = 0;
	m_nPageSizeH = 0;
	nTotalHeight = m_RectOrgMotionDlg.Height();
	nTotalWidth = m_RectOrgMotionDlg.Width();

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