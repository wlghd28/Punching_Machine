// CCalibrationDlg.cpp: 구현 파일
//

#include "pch.h"
#include "PunchingMachineDlg.h"
#include "CCalibrationDlg.h"
#include "afxdialogex.h"


// CCalibrationDlg 대화 상자

IMPLEMENT_DYNAMIC(CCalibrationDlg, CDialog)

CCalibrationDlg::CCalibrationDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CALIBRATION_DIALOG, pParent)
	, m_pPunchingMachineDlg(NULL)
{
	for (int i = 0; i < 2; i++)
	{
		m_PickPoint1[i].x = 0;
		m_PickPoint2[i].x = 0;
		m_PickPoint1[i].y = 0;
		m_PickPoint2[i].y = 0;
		m_dbOffset[i] = 0;
	}
}

CCalibrationDlg::~CCalibrationDlg()
{
}

void CCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_CALIBRATION_CAMERA1_PICKING_POINT1, m_CheckBtn_Picking1[0]);
	DDX_Control(pDX, IDC_CHECK_CALIBRATION_CAMERA2_PICKING_POINT1, m_CheckBtn_Picking1[1]);
	DDX_Control(pDX, IDC_CHECK_CALIBRATION_CAMERA1_PICKING_POINT2, m_CheckBtn_Picking2[0]);
	DDX_Control(pDX, IDC_CHECK_CALIBRATION_CAMERA2_PICKING_POINT2, m_CheckBtn_Picking2[1]);
}


BEGIN_MESSAGE_MAP(CCalibrationDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_CALIBRATION_CAMERA1_PICKING_POINT1, &CCalibrationDlg::OnBnClickedCheckCalibrationCamera1PickingPoint1)
	ON_BN_CLICKED(IDC_CHECK_CALIBRATION_CAMERA1_PICKING_POINT2, &CCalibrationDlg::OnBnClickedCheckCalibrationCamera1PickingPoint2)
	ON_BN_CLICKED(IDC_CHECK_CALIBRATION_CAMERA2_PICKING_POINT1, &CCalibrationDlg::OnBnClickedCheckCalibrationCamera2PickingPoint1)
	ON_BN_CLICKED(IDC_CHECK_CALIBRATION_CAMERA2_PICKING_POINT2, &CCalibrationDlg::OnBnClickedCheckCalibrationCamera2PickingPoint2)
	ON_BN_CLICKED(IDC_MFCBUTTON_CALIBRATION_CAMERA1_CALOFFSET, &CCalibrationDlg::OnBnClickedMfcbuttonCalibrationCamera1Caloffset)
	ON_BN_CLICKED(IDC_MFCBUTTON_CALIBRATION_CAMERA2_CALOFFSET, &CCalibrationDlg::OnBnClickedMfcbuttonCalibrationCamera2Caloffset)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CCalibrationDlg 메시지 처리기
BOOL CCalibrationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCalibrationDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

BOOL CCalibrationDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_pPunchingMachineDlg)
		m_pPunchingMachineDlg->m_bCheckCalibrationDlg = FALSE;

	return CDialog::DestroyWindow();
}


void CCalibrationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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



void CCalibrationDlg::OnBnClickedCheckCalibrationCamera1PickingPoint1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CCalibrationDlg::OnBnClickedCheckCalibrationCamera1PickingPoint2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CCalibrationDlg::OnBnClickedCheckCalibrationCamera2PickingPoint1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CCalibrationDlg::OnBnClickedCheckCalibrationCamera2PickingPoint2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CCalibrationDlg::OnBnClickedMfcbuttonCalibrationCamera1Caloffset()
{
	CalOffset(0);
}

void CCalibrationDlg::OnBnClickedMfcbuttonCalibrationCamera2Caloffset()
{
	CalOffset(1);
}



/*
	멤버함수
*/
void CCalibrationDlg::Init()
{
	m_pPunchingMachineDlg = (CPunchingMachineDlg*)AfxGetMainWnd();

	// ini 파일 정보 읽어온다
	char cArrBuf[256] = { 0, };
	GetPrivateProfileString("Camera_1", "OFFSET", 0, cArrBuf, 256, m_pPunchingMachineDlg->m_strPath_camera_ini);
	GetDlgItem(IDC_EDIT_CALIBRATION_CAMERA1_OFFSET)->SetWindowText(cArrBuf);
	m_dbOffset[0] = atof(cArrBuf);
	GetPrivateProfileString("Camera_2", "OFFSET", 0, cArrBuf, 256, m_pPunchingMachineDlg->m_strPath_camera_ini);
	GetDlgItem(IDC_EDIT_CALIBRATION_CAMERA2_OFFSET)->SetWindowText(cArrBuf);
	m_dbOffset[1] = atof(cArrBuf);
}

void CCalibrationDlg::SetPickPoint1(CPoint point, int iIndex)
{
	if (iIndex < 0) return;

	int iArrIDCol[2] = { IDC_EDIT_CALIBRATION_CAMERA1_PICKPOINT1_COL, IDC_EDIT_CALIBRATION_CAMERA2_PICKPOINT1_COL };
	int iArrIDRow[2] = { IDC_EDIT_CALIBRATION_CAMERA1_PICKPOINT1_ROW, IDC_EDIT_CALIBRATION_CAMERA2_PICKPOINT1_ROW };
	m_PickPoint1[iIndex] = point;

	SetDlgItemInt(iArrIDCol[iIndex], m_PickPoint1[iIndex].x);
	SetDlgItemInt(iArrIDRow[iIndex], m_PickPoint1[iIndex].y);

	AfxMessageBox("첫번째 좌표가 선택되었습니다.", MB_ICONINFORMATION);

	m_CheckBtn_Picking1[iIndex].SetCheck(0);
}

void CCalibrationDlg::SetPickPoint2(CPoint point, int iIndex)
{
	if (iIndex < 0) return;

	int iArrIDCol[2] = { IDC_EDIT_CALIBRATION_CAMERA1_PICKPOINT2_COL, IDC_EDIT_CALIBRATION_CAMERA2_PICKPOINT2_COL };
	int iArrIDRow[2] = { IDC_EDIT_CALIBRATION_CAMERA1_PICKPOINT2_ROW, IDC_EDIT_CALIBRATION_CAMERA2_PICKPOINT2_ROW };
	m_PickPoint2[iIndex] = point;

	SetDlgItemInt(iArrIDCol[iIndex], m_PickPoint2[iIndex].x);
	SetDlgItemInt(iArrIDRow[iIndex], m_PickPoint2[iIndex].y);

	AfxMessageBox("두번째 좌표가 선택되었습니다.", MB_ICONINFORMATION);

	m_CheckBtn_Picking2[iIndex].SetCheck(0);
}

void CCalibrationDlg::CalOffset(int iIndex)
{
	int iArrID_RealDistance[2] = { IDC_EDIT_CALIBRATION_CAMERA1_REALDISTANCE ,IDC_EDIT_CALIBRATION_CAMERA2_REALDISTANCE };
	int iArrID_Offset[2] = { IDC_EDIT_CALIBRATION_CAMERA1_OFFSET ,IDC_EDIT_CALIBRATION_CAMERA2_OFFSET };
	CString strSection_ini[2] = { "Camera_1", "Camera_2" };
	double dbXPoint1 = (double)m_PickPoint1[iIndex].x;
	double dbYPoint1 = (double)m_PickPoint1[iIndex].y;
	double dbXPoint2 = (double)m_PickPoint2[iIndex].x;
	double dbYPoint2 = (double)m_PickPoint2[iIndex].y;
	double dbXDist = dbXPoint1 - dbXPoint2;
	double dbYDist = dbYPoint1 - dbYPoint2;

	CString str;
	GetDlgItemText(iArrID_RealDistance[iIndex], str);
	double dbRealMMDistance = _ttof(str);	// 실제 두 점 사이의 거리
	double dbDisplayPixelDistance = sqrt(dbXDist * dbXDist + dbYDist * dbYDist);		// 영상에서의 두 점 사이의 거리
	
																						
	// 화면상의 거리 Pixel to MM
	double dbDisplayMMDistance = dbDisplayPixelDistance * (double)25.4 / (double)CAMERA_DPI;

	if (dbDisplayMMDistance == 0)
	{
		AfxMessageBox("두 점의 거리가 0 이 될 수 없습니다.", MB_ICONERROR);
		return;
	}

	// Offset = 실제거리 / 화면상의거리 (Motion 쪽에선 Offset 값을 곱해주면 됨)
 	m_dbOffset[iIndex] = dbRealMMDistance / dbDisplayMMDistance;
	str.Format("%lf", m_dbOffset[iIndex]);
	GetDlgItem(iArrID_Offset[iIndex])->SetWindowText(str);

	// ini 파일에 정보저장
	WritePrivateProfileString(strSection_ini[iIndex], "OFFSET", str, m_pPunchingMachineDlg->m_strPath_camera_ini);

	AfxMessageBox("Offset 값이 저장되었습니다.", MB_ICONINFORMATION);
}


