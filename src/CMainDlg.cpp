// CMainDlg.cpp: 구현 파일
//

#include "pch.h"
#include "PunchingMachineDlg.h"
#include "CMainDlg.h"
#include "CEquipmentDlg.h"
#include "CameraSubDlg.h"
#include "CStatusInformationDlg.h"
#include "afxdialogex.h"

// define child window pattern
#define SUB_DLG_NUM (2)
#define ROW_NUM (1)
#define COLUM_NUM (2)


// CMainDlg 대화 상자

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)

CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MAIN_DIALOG, pParent)
	, m_pPunchingMachineDlg(NULL)
	, m_pCameraSubDlgs(NULL)
	, m_pCStatusInformationDlg(NULL)
{

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURECONTROL_MAIN_LOGO, m_PictureContorl_Main_Logo);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_RBUTTONUP()
	ON_MESSAGE(WM_MY_DISCOVERY_MESSAGE, &CMainDlg::OnMyDiscoveryMsg)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMainDlg 메시지 처리기
BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	MoveWindow(m_RectMainDlg);
	InitIcon();
	//Init();
	//initCameraSubDlg();
	//initStatusInformationDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMainDlg::OnDestroy()
{
	CDialog::OnDestroy();

}

BOOL CMainDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	for (int i = 0; i < SUB_DLG_NUM; i++)
	{
		if (m_pCameraSubDlgs[i])
		{
			m_pCameraSubDlgs[i].DestroyWindow();
			delete m_pCameraSubDlgs[i];
		}
	}
	m_pCameraSubDlgs = NULL;
	
	return CDialog::DestroyWindow();
}


void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnSysCommand(nID, lParam);

	if (61458 == nID)
	{
		return;
	}
}

void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch (nIDEvent)
	{
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}



void CMainDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
}

HCURSOR CMainDlg::OnQueryDragIcon()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialog::OnQueryDragIcon();
}


void CMainDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
		//printf("닫을때 진입!\n");

	CDialog::OnClose();
}

void CMainDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//OnMyDiscoveryMsg(NULL, NULL);

	CDialog::OnRButtonUp(nFlags, point);
}

void CMainDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (ClickControl(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS, point))
	{
		if (IDYES == AfxMessageBox("알람을 초기화 하시겠습니까?", MB_YESNO))
		{
			m_pPunchingMachineDlg->GetPtrEquipmentDlg()->AlarmOFF();
		}

	}

	CDialog::OnLButtonDblClk(nFlags, point);
}


LRESULT CMainDlg::OnMyDiscoveryMsg(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox(_T("Tips: it'll discovery camera again!"));

	IMV_DeviceList deviceInfoList;
	if (IMV_OK != IMV_EnumDevices(&deviceInfoList, interfaceTypeAll))
	{
		AfxMessageBox(_T("Enum devices failed!"));
		return 0;
	}

	unsigned int iShowCameraNum = deviceInfoList.nDevNum < SUB_DLG_NUM ? deviceInfoList.nDevNum : SUB_DLG_NUM;
	for (unsigned int iIndex = 0; iIndex < iShowCameraNum; ++iIndex)
	{
		m_pCameraSubDlgs[iIndex].setCamera(iIndex);
	}

	return 0;
}


/*
	멤버함수
*/

void CMainDlg::SetScreenRect(CRect rect)
{
	m_RectMainDlg = rect;
}

CameraSubDlg* CMainDlg::GetPtrCameraSubDlg()
{
	return m_pCameraSubDlgs;
}

CStatusInformationDlg* CMainDlg::GetPtrStatusInformationDlg()
{
	return m_pCStatusInformationDlg;
}

void CMainDlg::Init()
{
	m_pPunchingMachineDlg = (CPunchingMachineDlg*)AfxGetMainWnd();
}

void CMainDlg::initCameraSubDlg()
{
	int iColum = COLUM_NUM;
	int iRow = ROW_NUM;
	m_pCameraSubDlgs = new CameraSubDlg[SUB_DLG_NUM];

	CRect rectMainDlg;
	GetClientRect(&rectMainDlg);

	for (int i = 0; i < SUB_DLG_NUM; i++)
	{
		CRect rect;
		int nWidth = rectMainDlg.Width() / iColum;
		int nHeight = rectMainDlg.Height() / iRow;

		rect.SetRect
		(
			rectMainDlg.TopLeft().x + (i % iColum) * nWidth,
			rectMainDlg.TopLeft().y + (i / iColum) * nHeight + 80,
			rectMainDlg.TopLeft().x + (i % iColum + 1) * nWidth,
			rectMainDlg.TopLeft().y + ((i / iColum) + 1) * nHeight + 80
		);

		m_pCameraSubDlgs[i].setRect(rect);
		m_pCameraSubDlgs[i].Create(IDD_CAMERA_DIALOG, this);
		m_pCameraSubDlgs[i].ShowWindow(SW_SHOW);
	}

	// enum camera 
	IMV_DeviceList deviceInfoList;
	if (IMV_OK != IMV_EnumDevices(&deviceInfoList, interfaceTypeAll))
	{
		AfxMessageBox(_T("Enum devices failed!"));
		return;
	}


	unsigned int iShowCameraNum = deviceInfoList.nDevNum < SUB_DLG_NUM ? deviceInfoList.nDevNum : SUB_DLG_NUM;
	for (unsigned int iIndex = 0; iIndex < iShowCameraNum; ++iIndex)
	{
		//printf("%d\n", iIndex);
		m_pCameraSubDlgs[iIndex].setCamera(iIndex);
		m_pCameraSubDlgs[iIndex].setSearchMark(iIndex);
		m_pCameraSubDlgs[iIndex].AutoConnect();
	}

}

void CMainDlg::initStatusInformationDlg()
{
	if (m_pCStatusInformationDlg != NULL) return;
	m_pCStatusInformationDlg = new CStatusInformationDlg();
	if (m_pCStatusInformationDlg == NULL) return;
	m_pCStatusInformationDlg->Create(IDD_STATUSINFORMATION_DIALOG, this);
	m_pCStatusInformationDlg->ShowWindow(SW_SHOW);

	CRect rect;
	CRect rectStatusInformationDlg;
	CRect rectMainDlg;
	GetClientRect(&rectMainDlg);
	m_pCameraSubDlgs[0].GetWindowRect(rect);
	ScreenToClient(&rect);

	rectStatusInformationDlg.top = rect.bottom + 5;
	rectStatusInformationDlg.left = rectMainDlg.left;
	rectStatusInformationDlg.bottom = rectMainDlg.bottom;
	rectStatusInformationDlg.right = rectMainDlg.right;

	m_pCStatusInformationDlg->MoveWindow(rectStatusInformationDlg);
}

void CMainDlg::InitIcon()
{
	CRect rect;
	GetClientRect(&rect);

	m_hIcon_OK = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_OK), IMAGE_ICON, 120, 80, 0);
	m_hIcon_NG = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_NG), IMAGE_ICON, 120, 80, 0);

	m_hIcon_START = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_START), IMAGE_ICON, 80, 80, 0);
	m_hIcon_STOP = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_STOP), IMAGE_ICON, 80, 80, 0);

	m_hIcon_ALARM1 = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_ALARM1), IMAGE_ICON, 80, 80, 0);
	m_hIcon_ALARM2 = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_ALARM2), IMAGE_ICON, 80, 80, 0);

	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->MoveWindow(rect.Width() / 2 - 60, rect.top, 120, 80);
	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->SetIcon(NULL);
	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_MAIN_ALIGNMENT_STATUS))->Invalidate(FALSE);

	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS))->MoveWindow(rect.left + 20, rect.top + 1, 80, 80);
	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS))->SetIcon(m_hIcon_STOP);
	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_MAIN_EQUIPMENT_STATUS))->Invalidate(FALSE);

	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_MAIN_LOGO))->MoveWindow(rect.Width() - 200, rect.top + 5, 200, 80);

}

// 컨트롤 클릭 상태 확인 함수
bool CMainDlg::ClickControl(int IDC_TARTGET, CPoint pt)
{
	CRect rcIdcTraget;
	GetDlgItem(IDC_TARTGET)->GetWindowRect(rcIdcTraget);
	ScreenToClient(rcIdcTraget);

	if (rcIdcTraget.left < pt.x && rcIdcTraget.top < pt.y
		&& rcIdcTraget.right > pt.x && rcIdcTraget.bottom > pt.y)
		return true;
	else
		return false;
}

