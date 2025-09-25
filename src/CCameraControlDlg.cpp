// CCameraControlDlg.cpp: 구현 파일
//

#include "pch.h"
#include "PunchingMachineDlg.h"
#include "CMainDlg.h"
#include "CameraSubDlg.h"
#include "CCameraControlDlg.h"
#include "afxdialogex.h"


// CCameraControlDlg 대화 상자

IMPLEMENT_DYNAMIC(CCameraControlDlg, CDialog)

CCameraControlDlg::CCameraControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CAMERA_CONTROL_DIALOG, pParent)
	, m_pPunchingMachineDlg(NULL)
{

}

CCameraControlDlg::~CCameraControlDlg()
{
}

void CCameraControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraControlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA1_CONNECT, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera1Connect)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA1_DISCONNECT, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera1Disconnect)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA1_PLAY, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera1Play)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA1_STOPPLAY, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera1Stopplay)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA1_SHOW_CENTERMARK, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera1ShowCentermark)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA1_HIDE_CENTERMARK, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera1HideCentermark)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA1_SEARCH_MARK_START, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera1SearchMarkStart)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA1_SEARCH_MARK_STOP, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera1SearchMarkStop)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA2_CONNECT, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera2Connect)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA2_DISCONNECT, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera2Disconnect)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA2_PLAY, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera2Play)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA2_STOPPLAY, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera2Stopplay)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA2_SHOW_CENTERMARK, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera2ShowCentermark)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA2_HIDE_CENTERMARK, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera2HideCentermark)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA2_SEARCH_MARK_START, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera2SearchMarkStart)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_CONTROL_CAMERA2_SEARCH_MARK_STOP, &CCameraControlDlg::OnBnClickedButtonCameraControlCamera2SearchMarkStop)
END_MESSAGE_MAP()


// CCameraControlDlg 메시지 처리기

BOOL CCameraControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CCameraControlDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_pPunchingMachineDlg)
		m_pPunchingMachineDlg->m_bCheckCameraControlDlg = FALSE;

	return CDialog::DestroyWindow();
}



void CCameraControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// Camera1
void CCameraControlDlg::OnBnClickedButtonCameraControlCamera1Connect()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].OnBnClickedButtonConnect();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera1Disconnect()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].OnBnClickedButtonDisconnect();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera1Play()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].OnBnClickedButtonPlay();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera1Stopplay()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].OnBnClickedButtonStopplay();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera1ShowCentermark()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].ShowCenterMark();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera1HideCentermark()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].HideCenterMark();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera1SearchMarkStart()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].OnBnClickedButtonSearchMarkStart();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera1SearchMarkStop()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[0].OnBnClickedButtonSearchMarkStop();
}



// Camera2
void CCameraControlDlg::OnBnClickedButtonCameraControlCamera2Connect()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].OnBnClickedButtonConnect();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera2Disconnect()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].OnBnClickedButtonDisconnect();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera2Play()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].OnBnClickedButtonPlay();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera2Stopplay()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].OnBnClickedButtonStopplay();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera2ShowCentermark()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].ShowCenterMark();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera2HideCentermark()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].HideCenterMark();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera2SearchMarkStart()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].OnBnClickedButtonSearchMarkStart();
}


void CCameraControlDlg::OnBnClickedButtonCameraControlCamera2SearchMarkStop()
{
	m_pPunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()[1].OnBnClickedButtonSearchMarkStop();
}


/*
	멤버함수
*/
void CCameraControlDlg::Init()
{
	m_pPunchingMachineDlg = (CPunchingMachineDlg*)AfxGetMainWnd();
}