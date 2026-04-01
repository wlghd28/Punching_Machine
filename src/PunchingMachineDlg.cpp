
// PunchingMachineDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "PunchingMachine.h"
#include "PunchingMachineDlg.h"
#include "CMainDlg.h"
#include "CEquipmentDlg.h"
#include "CIODlg.h"
#include "CMotionDlg.h"
#include "CEngineerDlg.h"
#include "CSimpleMotionDlg.h"
#include "CCalibrationDlg.h"
#include "CCameraControlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_CURRENTTIME 100

// define child window pattern
#define SUB_DLG_NUM (2)
#define ROW_NUM (1)
#define COLUM_NUM (2)

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
};

// CAboutDlg 대화상자


CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPunchingMachineDlg 대화 상자

CPunchingMachineDlg::CPunchingMachineDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PUNCHINGMACHINE_DIALOG, pParent)
	, m_MainDlg(NULL)
	, m_EquipmentDlg(NULL)
#ifdef IOTEST
	, m_IODlg(NULL)
#endif
#ifdef MOTIONTEST
	, m_MotionDlg(NULL)
#endif
	, m_EngineerDlg(NULL)
	, m_SimpleMotionDlg(NULL)
	, m_CalibrationDlg(NULL)
	, m_CameraControlDlg(NULL)
	, m_bCheckEngineerDlg(FALSE)
	, m_bCheckSimpleMotionDlg(FALSE)
	, m_bCheckCalibrationDlg(FALSE)
	, m_bCheckCameraControlDlg(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPunchingMachineDlg::~CPunchingMachineDlg()
{
	m_TabFont_OnFocus.DeleteObject();
	m_TabFont_OffFocus.DeleteObject();
}


void CPunchingMachineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PARENT, m_TabCtrl_Parent);
}

BEGIN_MESSAGE_MAP(CPunchingMachineDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PARENT, &CPunchingMachineDlg::OnSelchangeTabParent)
	ON_COMMAND(IDM_ENGINEER, &CPunchingMachineDlg::OnEngineer)
	ON_COMMAND(IDM_SIMPLEMOTION, &CPunchingMachineDlg::OnSimplemotion)
	ON_COMMAND(IDM_CAMERA_CALIBRATION, &CPunchingMachineDlg::OnCameraCalibration)
	ON_COMMAND(IDM_CAMERA_CONTROL, &CPunchingMachineDlg::OnCameraControl)
	ON_WM_CONTEXTMENU()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_NCCALCSIZE()
	ON_BN_CLICKED(IDC_MFCBUTTON_PARENT_EXIT, &CPunchingMachineDlg::OnBnClickedMfcbuttonParentExit)
END_MESSAGE_MAP()


// CPunchingMachineDlg 메시지 처리기

BOOL CPunchingMachineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// add additional initialization code here
	InitMonitorScreen();

	InitTabCtrl();

	InitFont();

	InitINI();

	// 다이얼로그 생성 실패 시 프로그램 종료
	if (!InitChildDialog()) return FALSE;

	SetTimer(TIMER_CURRENTTIME, 1000, NULL);

	// 프로그램 명 날짜 기입
	CString str_program_title;
	str_program_title.Format("Punching Machine (Build in  %s  on  %s)", __DATE__, __TIME__);
	this->SetWindowTextA(str_program_title);

	::ShowWindow(GetConsoleWindow(), SW_HIDE);
	this->SetMenu(NULL);


	//ShowWindow(SW_MAXIMIZE);
	//CRect rect;
	//GetWindowRect(&rect);
	//printf("wx : %d, wy : %d\n", rect.Width(), rect.Height());

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BOOL CPunchingMachineDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CDialogEx::PreCreateWindow(cs);
}



void CPunchingMachineDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

}


void CPunchingMachineDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

BOOL CPunchingMachineDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch (wParam)
	{
	case 16:
		return PostMessage(WM_SYSCOMMAND, wParam, lParam);
	case 61728:		
		return PostMessage(WM_SYSCOMMAND, wParam, lParam);
	case 61456:
		return PostMessage(WM_SYSCOMMAND, wParam, lParam);
	case 61440:
		return PostMessage(WM_SYSCOMMAND, wParam, lParam);
	case 61472:
		return PostMessage(WM_SYSCOMMAND, wParam, lParam);
	case 61488:
		return PostMessage(WM_SYSCOMMAND, wParam, lParam);
	case 61536:
		return PostMessage(WM_SYSCOMMAND, wParam, lParam);
	default:
		return CDialogEx::OnCommand(wParam, lParam);
	}

}

void CPunchingMachineDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TIMER_CURRENTTIME:
	{
		CTime CurTime = CTime::GetCurrentTime();
		CString strCurTime;
		strCurTime.Format("%04d-%02d-%02d  %02d:%02d:%02d",
			CurTime.GetYear(),
			CurTime.GetMonth(),
			CurTime.GetDay(),
			CurTime.GetHour(),
			CurTime.GetMinute(),
			CurTime.GetSecond()
		);

		((CStatic*)GetDlgItem(IDC_STATIC_PARENT_CURRENTTIME))->SetWindowText(strCurTime);
		((CStatic*)GetDlgItem(IDC_STATIC_PARENT_CURRENTTIME))->Invalidate(FALSE);
	}
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPunchingMachineDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CPunchingMachineDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);
}

void CPunchingMachineDlg::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	CDialogEx::OnNcCalcSize(bCalcValidRects, lpncsp);
}


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CPunchingMachineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPunchingMachineDlg::OnClose()
{
	KillTimer(TIMER_CURRENTTIME);

	// 자식 다이얼로그 해제
#ifdef MOTIONTEST
	if (m_MotionDlg)
	{
		m_MotionDlg->DestroyWindow();
		delete m_MotionDlg;
		m_MotionDlg = NULL;
	}
#endif

#ifdef IOTEST
	if (m_IODlg)
	{
		m_IODlg->DestroyWindow();
		delete m_IODlg;
		m_IODlg = NULL;
	}
#endif

	if (m_EquipmentDlg)
	{
		m_EquipmentDlg->DestroyWindow();
		delete m_EquipmentDlg;
		m_EquipmentDlg = NULL;
	}

	if (m_MainDlg)
	{
		m_MainDlg->DestroyWindow();
		delete m_MainDlg;
		m_MainDlg = NULL;
	}

	if (m_EngineerDlg)
	{
		m_EngineerDlg->DestroyWindow();
		delete m_EngineerDlg;
		m_EngineerDlg = NULL;
	}

	if (m_CalibrationDlg)
	{
		m_CalibrationDlg->DestroyWindow();
		delete m_CalibrationDlg;
		m_CalibrationDlg = NULL;
	}

	if (m_SimpleMotionDlg)
	{
		m_SimpleMotionDlg->DestroyWindow();
		delete m_SimpleMotionDlg;
		m_SimpleMotionDlg = NULL;
	}

	if (m_CameraControlDlg)
	{
		m_CameraControlDlg->DestroyWindow();
		delete m_CameraControlDlg;
		m_CameraControlDlg = NULL;
	}

	this->DestroyWindow();

	CDialogEx::OnClose();
}

void CPunchingMachineDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogEx::PostNcDestroy();
}




void CPunchingMachineDlg::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	// TODO: add control notification processing code here

	//CDialog::OnNcLButtonDblClk(nHitTest, point);
	ShowWindow(SW_MINIMIZE);
}


void CPunchingMachineDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// add control notification processing code here
	//OnMyDiscoveryMsg(NULL, NULL);

	CDialogEx::OnRButtonUp(nFlags, point);
}

void CPunchingMachineDlg::OnSelchangeTabParent(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (IDC_TAB_PARENT == pNMHDR->idFrom)
	{
		int select = m_TabCtrl_Parent.GetCurSel();
		switch (select) {
		case 0:
			m_MainDlg->ShowWindow(SW_SHOW);
			m_EquipmentDlg->ShowWindow(SW_HIDE);
#ifdef IOTEST
			m_IODlg->ShowWindow(SW_HIDE);
#endif
#ifdef MOTIONTEST
			m_MotionDlg->ShowWindow(SW_HIDE);
#endif
			break;
		case 1:
			m_MainDlg->ShowWindow(SW_HIDE);
			m_EquipmentDlg->ShowWindow(SW_SHOW);
#ifdef IOTEST
			m_IODlg->ShowWindow(SW_HIDE);
#endif
#ifdef MOTIONTEST
			m_MotionDlg->ShowWindow(SW_HIDE);
#endif
			break;
#ifdef IOTEST
		case 2:
			m_MainDlg->ShowWindow(SW_HIDE);
			m_EquipmentDlg->ShowWindow(SW_HIDE);
			m_IODlg->ShowWindow(SW_SHOW);
#ifdef MOTIONTEST
			m_MotionDlg->ShowWindow(SW_HIDE);
#endif
			break;
#endif
#ifdef MOTIONTEST
		case 3:
			m_MainDlg->ShowWindow(SW_HIDE);
			m_EquipmentDlg->ShowWindow(SW_HIDE);
#ifdef IOTEST
			m_IODlg->ShowWindow(SW_HIDE);
#endif
			m_MotionDlg->ShowWindow(SW_SHOW);
			break;
#endif

		default:
			m_MainDlg->ShowWindow(SW_HIDE);
			m_EquipmentDlg->ShowWindow(SW_HIDE);
#ifdef IOTEST
			m_IODlg->ShowWindow(SW_HIDE);
#endif
#ifdef MOTIONTEST
			m_MotionDlg->ShowWindow(SW_HIDE);
#endif
			break;
		}
	}
	*pResult = 0;
}



BOOL CPunchingMachineDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_RETURN)   //Enter 키면 무시..	
		return TRUE;

	if (pMsg->wParam == VK_ESCAPE)   //ESC 키면 무시..
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

// 컨텍스트 메뉴
void CPunchingMachineDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CPoint tempPoint = point;
	ScreenToClient(&tempPoint);

	if (tempPoint.y > GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYMENU))
	{
		CMenu menu;

		// 리소스에서 IDR_MAINFRAME을 로드합니다.
		menu.LoadMenu(IDR_MENU);

		// 메뉴바의 네 번째 아이템을 가져옵니다. ( 0부터 시작 )
		CMenu* pMenu = menu.GetSubMenu(0);


		// 컨텍스트 메뉴를 x,y 좌표에 출력합니다. 
		pMenu->TrackPopupMenu(TPM_LEFTALIGN,
			point.x,
			point.y,
			AfxGetMainWnd());
	}
	else
	{
		CMenu* pSysMenu = GetSystemMenu(FALSE);
		pSysMenu->TrackPopupMenu(TPM_LEFTALIGN,
			point.x,
			point.y,
			AfxGetMainWnd());
	}
}



// 메뉴바 설정 옵션
void CPunchingMachineDlg::OnEngineer()
{
	if (m_EngineerDlg == NULL) return;
	
	if (!m_bCheckEngineerDlg)
	{
		m_EngineerDlg->Create(IDD_ENGINEER_DIALOG, this);
		m_EngineerDlg->ShowWindow(SW_SHOW);
		m_bCheckEngineerDlg = TRUE;
	}
}

void CPunchingMachineDlg::OnSimplemotion()
{
	if (m_SimpleMotionDlg == NULL) return;

	if (!m_bCheckSimpleMotionDlg)
	{
		m_SimpleMotionDlg->Create(IDD_SIMPLE_MOTION_DIALOG, this);
		m_SimpleMotionDlg->ShowWindow(SW_SHOW);
		m_bCheckSimpleMotionDlg = TRUE;
	}
}


void CPunchingMachineDlg::OnCameraCalibration()
{
	if (m_CalibrationDlg == NULL) return;

	if (!m_bCheckCalibrationDlg)
	{
		m_CalibrationDlg->Create(IDD_CALIBRATION_DIALOG, this);
		m_CalibrationDlg->ShowWindow(SW_SHOW);
		m_bCheckCalibrationDlg = TRUE;
	}
}



void CPunchingMachineDlg::OnCameraControl()
{
	if (m_CameraControlDlg == NULL) return;

	if (!m_bCheckCameraControlDlg)
	{
		m_CameraControlDlg->Create(IDD_CAMERA_CONTROL_DIALOG, this);
		m_CameraControlDlg->ShowWindow(SW_SHOW);
		m_bCheckCameraControlDlg = TRUE;
	}
}


void CPunchingMachineDlg::OnBnClickedMfcbuttonParentExit()
{
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}




/*
	멤버함수
*/
void CPunchingMachineDlg::InitMonitorScreen()
{
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	int iWidth = 1280;
	int iHeight = 800;

	// 1280 x 800 해상도일 때 테두리 없앤다.
	if (cx == iWidth && cy == iHeight)
	{
		ModifyStyleEx(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE, 0);
		ModifyStyle(DS_MODALFRAME | WS_DLGFRAME | WS_CAPTION, 0);
	}

	::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, iWidth, iHeight, SWP_FRAMECHANGED);
}

void CPunchingMachineDlg::InitFont()
{
	m_TabFont_OnFocus.CreateFont
	(
		40, 
		0, 
		0, 
		0, 
		FW_BOLD,
		FALSE, 
		FALSE, 
		FALSE, 
		0, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_ROMAN, 
		_T("Arial")
	);

	m_TabFont_OffFocus.CreateFont
	(
		30,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		FALSE,
		FALSE,
		0,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_ROMAN,
		_T("Arial")
	);


	// Tab 폰트 설정
	//m_TabCtrl_Parent.SetFont(&m_TabFont_OnFocus, TRUE);
	m_TabCtrl_Parent.SetFonts(&m_TabFont_OnFocus, &m_TabFont_OffFocus);

	((CStatic*)GetDlgItem(IDC_STATIC_PARENT_CURRENTTIME))->SetFont(&m_TabFont_OffFocus);
}

void CPunchingMachineDlg::InitTabCtrl()
{
	// Tab 크기 설정
	CRect rectParentDlg;
	GetClientRect(rectParentDlg);
	m_TabCtrl_Parent.MoveWindow
	(
		rectParentDlg.left,
		rectParentDlg.top,
		rectParentDlg.Width(),
		rectParentDlg.Height() + GetSystemMetrics(SM_CYMENU)
	);


	// Tab 아이템 추가
	m_TabCtrl_Parent.InsertItem(0, _T("메인"));
	m_TabCtrl_Parent.InsertItem(1, _T("장비"));
#ifdef IOTEST
	m_TabCtrl_Parent.InsertItem(2, _T("입출력"));
#endif
#ifdef MOTIONTEST
	m_TabCtrl_Parent.InsertItem(3, _T("모션"));
#endif
	m_TabCtrl_Parent.SetCurSel(0);

	// Tab 아이템 사이즈 설정
	m_TabCtrl_Parent.SetItemSize(CSize(100, 40));

	((CStatic*)GetDlgItem(IDC_STATIC_PARENT_CURRENTTIME))->MoveWindow(rectParentDlg.Width() - 400, rectParentDlg.bottom - 20, 250, 30);
	GetDlgItem(IDC_MFCBUTTON_PARENT_EXIT)->MoveWindow(rectParentDlg.Width() - 120, rectParentDlg.bottom - 20, 100, 30);
}

BOOL CPunchingMachineDlg::InitChildDialog()
{
	CRect rect;
	m_TabCtrl_Parent.GetClientRect(rect);
	rect.BottomRight().y -= 40;

	// Main 다이얼로그 생성
	m_MainDlg = new CMainDlg();
	if (m_MainDlg == NULL) return FALSE;
	m_MainDlg->SetScreenRect(rect);
	m_MainDlg->Create(IDD_MAIN_DIALOG, &m_TabCtrl_Parent);
	m_MainDlg->ShowWindow(SW_SHOW);

	// Equipment 다이얼로그 생성
	m_EquipmentDlg = new CEquipmentDlg();
	if (m_EquipmentDlg == NULL) return FALSE;
	m_EquipmentDlg->SetScreenRect(rect);
	m_EquipmentDlg->Create(IDD_EQUIPMENT_DIALOG, &m_TabCtrl_Parent);
	m_EquipmentDlg->ShowWindow(SW_HIDE);

#ifdef IOTEST
	// IO 다이얼로그 생성
	m_IODlg = new CIODlg();
	if (m_IODlg == NULL) return FALSE;
	m_IODlg->SetScreenRect(rect);
	m_IODlg->Create(IDD_IO_DIALOG, &m_TabCtrl_Parent);
	m_IODlg->ShowWindow(SW_HIDE);
#endif

#ifdef MOTIONTEST
	// Motion 다이얼로그 생성
	m_MotionDlg = new CMotionDlg();
	if (m_MotionDlg == NULL) return FALSE;
	m_MotionDlg->SetScreenRect(rect);
	m_MotionDlg->Create(IDD_MOTION_DIALOG, &m_TabCtrl_Parent);
	m_MotionDlg->ShowWindow(SW_HIDE);
#endif

	m_MainDlg->Init();
	m_MainDlg->initCameraSubDlg();
	m_MainDlg->initStatusInformationDlg();
	m_EquipmentDlg->Init();
	m_IODlg->Init();
	m_MotionDlg->Init();

	// Engineer 다이얼로그 생성
	m_EngineerDlg = new CEngineerDlg();
	if (m_EngineerDlg == NULL) return FALSE;

	// Calibration 다이얼로그 생성
	m_CalibrationDlg = new CCalibrationDlg();
	if (m_CalibrationDlg == NULL) return FALSE;

	// SimpleMotion 다이얼로그 생성
	m_SimpleMotionDlg = new CSimpleMotionDlg();
	if (m_SimpleMotionDlg == NULL) return FALSE;

	// CameraControl 다이얼로그 생성
	m_CameraControlDlg = new CCameraControlDlg();
	if (m_CameraControlDlg == NULL) return FALSE;

	return TRUE;
}

void CPunchingMachineDlg::InitINI()
{
	// Log 폴더 존재유무 확인
	// 폴더 없을 경우 재생성
	if (GetFileAttributes(".\\Log") == -1)
		CreateDirectory(".\\Log", NULL);


	// config 폴더 존재유무 확인
	// 폴더 없을 경우 재생성
	if (GetFileAttributes(".\\config") == -1)
		CreateDirectory(".\\config", NULL);

	// NG_Image 폴더 존재유무 확인
	// 폴더 없을 경우 재생성
	if (GetFileAttributes(".\\NG_Image") == -1)
		CreateDirectory(".\\NG_Image", NULL);


	// ini 파일 존재유무 확인
	// ini 파일 없을 경우 재생성
	if (PathFileExists(m_strPath_opencv_parameter_ini) == false)
	{
		WritePrivateProfileString("Camera_1", "minDist", "9999", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_1", "threshold_max_canny", "150", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_1", "threshold_min_canny", "0", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_1", "threshold_detection", "40", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_1", "minRadius", "1", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_1", "maxRadius", "2", m_strPath_opencv_parameter_ini);

		WritePrivateProfileString("Camera_2", "minDist", "9999", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_2", "threshold_max_canny", "150", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_2", "threshold_min_canny", "0", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_2", "threshold_detection", "40", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_2", "minRadius", "1", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_2", "maxRadius", "2", m_strPath_opencv_parameter_ini);

		WritePrivateProfileString("Camera_1", "threshold_blocksize", "127", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_1", "threshold_max_offset", "20", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_1", "threshold_min_offset", "1", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_1", "approx_size", "4", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_1", "approx_minarea", "0", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_1", "approx_minratio", "0", m_strPath_opencv_parameter_ini);

		WritePrivateProfileString("Camera_2", "threshold_blocksize", "127", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_2", "threshold_max_offset", "20", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_2", "threshold_min_offset", "1", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_2", "approx_size", "4", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_2", "approx_minarea", "0", m_strPath_opencv_parameter_ini);
		WritePrivateProfileString("Camera_2", "approx_minratio", "0", m_strPath_opencv_parameter_ini);
	}

	if (PathFileExists(m_strPath_motion_ini) == false)
	{
		WritePrivateProfileString("EQUIPMENT", "PULSE_PER_MM_AXIS0", "1000", m_strPath_motion_ini);
		WritePrivateProfileString("EQUIPMENT", "PULSE_PER_MM_AXIS1", "1000", m_strPath_motion_ini);
		WritePrivateProfileString("EQUIPMENT", "PULSE_PER_MM_AXIS2", "1000", m_strPath_motion_ini);
		WritePrivateProfileString("SEQUENCE", "INTERVAL", "1000", m_strPath_motion_ini);
	}

	if (PathFileExists(m_strPath_camera_ini) == false)
	{
		WritePrivateProfileString("Camera_1", "OFFSET", "1", m_strPath_camera_ini);
		WritePrivateProfileString("Camera_2", "OFFSET", "1", m_strPath_camera_ini);
		WritePrivateProfileString("ALIGNMENT", "maxSearchCount", "5", m_strPath_camera_ini);
		WritePrivateProfileString("ALIGNMENT", "maxErrorPixel", "10", m_strPath_camera_ini);
		WritePrivateProfileString("ALIGNMENT", "CheckImageCut", "0", m_strPath_camera_ini);
		WritePrivateProfileString("ALIGNMENT", "SearchingAlgorithm", "0", m_strPath_camera_ini);
	}

	if (PathFileExists(m_strPath_punchdata_ini) == false)
	{
		WritePrivateProfileString("PUNCH", "CurrentNumber", "0", m_strPath_punchdata_ini);
		WritePrivateProfileString("PUNCH", "TotalNumber", "0", m_strPath_punchdata_ini);
	}
}

CEngineerDlg* CPunchingMachineDlg::GetPtrEngineerDlg()
{
	return m_EngineerDlg;
}

CCalibrationDlg* CPunchingMachineDlg::GetPtrCalibrationDlg()
{
	return m_CalibrationDlg;
}

CSimpleMotionDlg* CPunchingMachineDlg::GetPtrSimpleMotionDlg()
{
	return m_SimpleMotionDlg;
}

CMainDlg* CPunchingMachineDlg::GetPtrMainDlg()
{
	return m_MainDlg;
}

CEquipmentDlg* CPunchingMachineDlg::GetPtrEquipmentDlg()
{
	return m_EquipmentDlg;
}

#ifdef IOTEST
CIODlg* CPunchingMachineDlg::GetPtrIODlg()
{
	return m_IODlg;
}
#endif

#ifdef MOTIONTEST
CMotionDlg* CPunchingMachineDlg::GetPtrMotionDlg()
{
	return m_MotionDlg;
}
#endif



void CPunchingMachineDlg::Wait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD t0, t1, diff;
	t0 = GetTickCount64();
	while (TRUE) 
	{
		t1 = GetTickCount64();
		if (t0 <= t1) diff = t1 - t0;
		else diff = 0xFFFFFFFF - t0 + t1;
		if (diff > dwMillisecond)  break;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1);
	}
}



void CPunchingMachineDlg::CaptureScreen()
{
	CClientDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	CImage img;
	img.Create(rect.Width(), rect.Height(), 24);
	CDC* pDC = CDC::FromHandle(img.GetDC());

	// Draw //
	pDC->SetStretchBltMode(COLORONCOLOR);
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dc,
		0, 0, rect.Width(), rect.Height(), SRCCOPY);


	// 이미지 경로
	CTime CurTime = CTime::GetCurrentTime();
	CString strImgpath;
	strImgpath.Format("NG_Image\\%04d%02d%02d_%02d%02d%02d.jpg",
		CurTime.GetYear(),
		CurTime.GetMonth(),
		CurTime.GetDay(),
		CurTime.GetHour(),
		CurTime.GetMinute(),
		CurTime.GetSecond()
	);

	img.Save(strImgpath);

	ReleaseDC(pDC);
	img.ReleaseDC();
	img.Destroy();
}

