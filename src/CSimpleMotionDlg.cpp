// CSimpleMotionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "CSimpleMotionDlg.h"
#include "PunchingMachineDlg.h"
#include "CMainDlg.h"
#include "CEquipmentDlg.h"
#include "CMotionDlg.h"
#include "afxdialogex.h"

// CSimpleMotionDlg 대화 상자

IMPLEMENT_DYNAMIC(CSimpleMotionDlg, CDialog)

CSimpleMotionDlg::CSimpleMotionDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SIMPLE_MOTION_DIALOG, pParent)
	, m_pPunchingMachineDlg(NULL)
{

}

CSimpleMotionDlg::~CSimpleMotionDlg()
{
}

void CSimpleMotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOVERBUTTON_SIMPLEMOTION_AXIS0_MINUS, m_BtnAxis0_Minus);
	DDX_Control(pDX, IDC_HOVERBUTTON_SIMPLEMOTION_AXIS0_PLUS, m_BtnAxis0_Plus);
	DDX_Control(pDX, IDC_HOVERBUTTON_SIMPLEMOTION_AXIS1_MINUS, m_BtnAxis1_Minus);
	DDX_Control(pDX, IDC_HOVERBUTTON_SIMPLEMOTION_AXIS1_PLUS, m_BtnAxis1_Plus);
	DDX_Control(pDX, IDC_HOVERBUTTON_SIMPLEMOTION_AXIS2_MINUS, m_BtnAxis2_Minus);
	DDX_Control(pDX, IDC_HOVERBUTTON_SIMPLEMOTION_AXIS2_PLUS, m_BtnAxis2_Plus);
	DDX_Control(pDX, IDC_STATIC_SIMPLEMOTION_AXIS0_MLIMIT, m_StaticAxis0_MLimit);
	DDX_Control(pDX, IDC_STATIC_SIMPLEMOTION_AXIS0_PLIMIT, m_StaticAxis0_PLimit);
	DDX_Control(pDX, IDC_STATIC_SIMPLEMOTION_AXIS1_MLIMIT, m_StaticAxis1_MLimit);
	DDX_Control(pDX, IDC_STATIC_SIMPLEMOTION_AXIS1_PLIMIT, m_StaticAxis1_PLimit);
	DDX_Control(pDX, IDC_STATIC_SIMPLEMOTION_AXIS2_MLIMIT, m_StaticAxis2_MLimit);
	DDX_Control(pDX, IDC_STATIC_SIMPLEMOTION_AXIS2_PLIMIT, m_StaticAxis2_PLimit);
}

BEGIN_MESSAGE_MAP(CSimpleMotionDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MFCBUTTON_SIMPLEMOTION_HOMING, &CSimpleMotionDlg::OnBnClickedMfcbuttonSimplemotionHoming)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIMPLEMOTION_AXIS2_RELATIVE_MOVE, &CSimpleMotionDlg::OnBnClickedMfcbuttonSimplemotionAxis2RelativeMove)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIMPLEMOTION_AXIS1_RELATIVE_MOVE, &CSimpleMotionDlg::OnBnClickedMfcbuttonSimplemotionAxis1RelativeMove)
	ON_BN_CLICKED(IDC_MFCBUTTON_SIMPLEMOTION_AXIS0_RELATIVE_MOVE, &CSimpleMotionDlg::OnBnClickedMfcbuttonSimplemotionAxis0RelativeMove)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CSimpleMotionDlg 메시지 처리기
BOOL CSimpleMotionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSimpleMotionDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CSimpleMotionDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_pPunchingMachineDlg)
		m_pPunchingMachineDlg->m_bCheckSimpleMotionDlg = FALSE;

	return CDialog::DestroyWindow();
}

void CSimpleMotionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nID)
	{
	case SC_CLOSE:
		DestroyWindow();
		break;
	defualt:
		break;
	}
	CDialog::OnSysCommand(nID, lParam);
}


void CSimpleMotionDlg::OnBnClickedMfcbuttonSimplemotionHoming()
{
	Homing();
}

void CSimpleMotionDlg::OnBnClickedMfcbuttonSimplemotionAxis2RelativeMove()
{
	short nRet;
	CString str;
	double dbValue = 0;
	double dbPulsePos = 0;
	GetDlgItemText(IDC_EDIT_SIMPLEMOTION_AXIS2_RELATIVE_MOVE, str);
	dbValue = _ttof(str);

	// mm to Pulse
	dbPulsePos = dbValue * m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_iPulsePerMM_Axis2;

	nRet = nmc_RelMove(m_pPunchingMachineDlg->GetPtrMotionDlg()->GetPaixCtlNo(), 2, dbPulsePos);
}


void CSimpleMotionDlg::OnBnClickedMfcbuttonSimplemotionAxis1RelativeMove()
{
	short nRet;
	CString str;
	double dbValue = 0;
	double dbPulsePos = 0;
	GetDlgItemText(IDC_EDIT_SIMPLEMOTION_AXIS1_RELATIVE_MOVE, str);
	dbValue = _ttof(str);

	// mm to Pulse
	dbPulsePos = dbValue * m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_iPulsePerMM_Axis1;

	nRet = nmc_RelMove(m_pPunchingMachineDlg->GetPtrMotionDlg()->GetPaixCtlNo(), 1, dbPulsePos);
}


void CSimpleMotionDlg::OnBnClickedMfcbuttonSimplemotionAxis0RelativeMove()
{
	short nRet;
	CString str;
	double dbValue = 0;
	double dbPulsePos = 0;
	GetDlgItemText(IDC_EDIT_SIMPLEMOTION_AXIS0_RELATIVE_MOVE, str);
	dbValue = _ttof(str);

	// mm to Pulse
	dbPulsePos = dbValue * m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_iPulsePerMM_Axis0;

	nRet = nmc_RelMove(m_pPunchingMachineDlg->GetPtrMotionDlg()->GetPaixCtlNo(), 0, dbPulsePos);
}


/*
	멤버함수
*/
void CSimpleMotionDlg::Init()
{
	//if (m_pMotionDlg == NULL) return;
	m_pPunchingMachineDlg = (CPunchingMachineDlg*)AfxGetMainWnd();

	m_BtnAxis0_Minus.SetAxisDir(0, 1);
	m_BtnAxis0_Plus.SetAxisDir(0, 0);
	m_BtnAxis1_Minus.SetAxisDir(1, 1);
	m_BtnAxis1_Plus.SetAxisDir(1, 0);
	m_BtnAxis2_Minus.SetAxisDir(2, 1);
	m_BtnAxis2_Plus.SetAxisDir(2, 0);

	m_BtnAxis0_Minus.SetParentHwnd(m_pPunchingMachineDlg->GetPtrMotionDlg()->m_hWnd);
	m_BtnAxis0_Plus.SetParentHwnd(m_pPunchingMachineDlg->GetPtrMotionDlg()->m_hWnd);
	m_BtnAxis1_Minus.SetParentHwnd(m_pPunchingMachineDlg->GetPtrMotionDlg()->m_hWnd);
	m_BtnAxis1_Plus.SetParentHwnd(m_pPunchingMachineDlg->GetPtrMotionDlg()->m_hWnd);
	m_BtnAxis2_Minus.SetParentHwnd(m_pPunchingMachineDlg->GetPtrMotionDlg()->m_hWnd);
	m_BtnAxis2_Plus.SetParentHwnd(m_pPunchingMachineDlg->GetPtrMotionDlg()->m_hWnd);

	m_StaticAxis0_MLimit.SetBkColor(clSilver);
	m_StaticAxis0_PLimit.SetBkColor(clSilver);
	m_StaticAxis1_MLimit.SetBkColor(clSilver);
	m_StaticAxis1_PLimit.SetBkColor(clSilver);
	m_StaticAxis2_MLimit.SetBkColor(clSilver);
	m_StaticAxis2_PLimit.SetBkColor(clSilver);


	m_BtnAxis0_Minus.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_DOWN_ARROW));
	m_BtnAxis0_Plus.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_UP_ARROW));
	m_BtnAxis1_Minus.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_LEFT_ARROW));
	m_BtnAxis1_Plus.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_RIGHT_ARROW));
	m_BtnAxis2_Minus.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_DOWN_ARROW));
	m_BtnAxis2_Plus.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_UP_ARROW));
}

void CSimpleMotionDlg::MoveRect(CRect rect)
{
	this->MoveWindow(rect);
}

void CSimpleMotionDlg::Homing()
{
	short nHomeMode, nEndMode;
	long loffset;
	short nret;

	// 0축 (RY)
	nHomeMode = ((CComboBox*)m_pPunchingMachineDlg->GetPtrMotionDlg()->GetDlgItem(IDC_CB_HOME1))->GetCurSel();
	nEndMode = ((CComboBox*)m_pPunchingMachineDlg->GetPtrMotionDlg()->GetDlgItem(IDC_CB_CLEAR1))->GetCurSel();
	loffset = m_pPunchingMachineDlg->GetPtrMotionDlg()->GetDlgItemInt(IDC_EDIT_OFFSET1);
	nret = nmc_HomeMove(m_pPunchingMachineDlg->GetPtrMotionDlg()->GetPaixCtlNo(), 0, nHomeMode, nEndMode, loffset, 0);

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


