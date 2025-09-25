// CEngineerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "CEngineerDlg.h"
#include "PunchingMachineDlg.h"
#include "CMainDlg.h"
#include "CameraSubDlg.h"
#include "SearchMark.h"
#include "CalculateAngle.h"
#include "afxdialogex.h"


// CEngineerDlg 대화 상자

IMPLEMENT_DYNAMIC(CEngineerDlg, CDialog)

CEngineerDlg::CEngineerDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ENGINEER_DIALOG, pParent)
	, m_pPunchingMachineDlg(NULL)
	, m_pMainDlg(NULL)
	, m_pCameraSubDlg(NULL)
	, m_pCalculateAngle(NULL)
	, m_iSearchingAlgorithm(0)
{

}

CEngineerDlg::~CEngineerDlg()
{

}

void CEngineerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDCANCEL, m_btn_Cancel);
	DDX_Control(pDX, IDC_CHECK_ENGINEER_CAMERA1_MODE, m_Check_Engineer_Mode[0]);
	DDX_Control(pDX, IDC_CHECK_ENGINEER_CAMERA2_MODE, m_Check_Engineer_Mode[1]);
	DDX_Control(pDX, IDC_CHECK_ENGINEER_SHOWCONSOLE, m_Check_ShowConsole);
	DDX_Radio(pDX, IDC_RADIO_ENGINEER_ALIGNMENT_HOUGHCIRCLES, m_iSearchingAlgorithm);
}


BEGIN_MESSAGE_MAP(CEngineerDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDCANCEL, &CEngineerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ENGINEER_CAMERA1_LOAD_PARAMETER, &CEngineerDlg::OnBnClickedButtonEngineerCamera1LoadParameter)
	ON_BN_CLICKED(IDC_BUTTON_ENGINEER_CAMERA1_SAVE_PARAMETER, &CEngineerDlg::OnBnClickedButtonEngineerCamera1SaveParameter)
	ON_BN_CLICKED(IDC_BUTTON_ENGINEER_CAMERA2_LOAD_PARAMETER, &CEngineerDlg::OnBnClickedButtonEngineerCamera2LoadParameter)
	ON_BN_CLICKED(IDC_BUTTON_ENGINEER_CAMERA2_SAVE_PARAMETER, &CEngineerDlg::OnBnClickedButtonEngineerCamera2SaveParameter)
	ON_BN_CLICKED(IDC_CHECK_ENGINEER_CAMERA2_MODE, &CEngineerDlg::OnBnClickedCheckEngineerCamera2Mode)
	ON_BN_CLICKED(IDC_CHECK_ENGINEER_CAMERA1_MODE, &CEngineerDlg::OnBnClickedCheckEngineerCamera1Mode)
	ON_BN_CLICKED(IDC_MFCBUTTON_ENGINEER_CALCULATE_ANGLE, &CEngineerDlg::OnBnClickedMfcbuttonEngineerCalculateAngle)
	ON_BN_CLICKED(IDC_CHECK_ENGINEER_SHOWCONSOLE, &CEngineerDlg::OnBnClickedCheckEngineerShowconsole)
	ON_BN_CLICKED(IDC_BUTTON_ENGINEER_ALIGNMENT_SAVE_PARAMETER, &CEngineerDlg::OnBnClickedButtonEngineerAlignmentSaveParameter)
	ON_BN_CLICKED(IDC_BUTTON_ENGINEER_ALIGNMENT_LOAD_PARAMETER, &CEngineerDlg::OnBnClickedButtonEngineerAlignmentLoadParameter)
END_MESSAGE_MAP()


// CEngineerDlg 메시지 처리기
BOOL CEngineerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPunchingMachineDlg = ((CPunchingMachineDlg *)GetParent());

	LoadOpenCVParameter(0);
	LoadOpenCVParameter(1);
	LoadAlignmentParameter();

	if (m_pPunchingMachineDlg != NULL)
	{
		m_pMainDlg = m_pPunchingMachineDlg->GetPtrMainDlg();
	}

	if (m_pMainDlg != NULL)
	{
		m_pCameraSubDlg = m_pMainDlg->GetPtrCameraSubDlg();
	}

	if (m_pCameraSubDlg != NULL)
	{
		for (int i = 0; i < 2; i++)
		{
			m_Check_Engineer_Mode[i].SetCheck((BOOL)(m_pCameraSubDlg[i].m_iEngineerMode));
		}	
	}

	m_Check_ShowConsole.SetCheck(::IsWindowVisible(GetConsoleWindow()));

	m_pCalculateAngle = new CCalculateAngle();
	if (m_pCalculateAngle == NULL) return FALSE;

	//m_Check_Engineer_Mode.SetState((BOOL)(m_PunchingMachineDlg->GetPtrMainDlg()->GetPtrCameraSubDlg()->m_iEngineerMode));



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CEngineerDlg::OnDestroy()
{
	CDialog::OnDestroy();

}


BOOL CEngineerDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_pCalculateAngle)
	{
		delete m_pCalculateAngle;
		m_pCalculateAngle = NULL;
	}

	if(m_pPunchingMachineDlg)
		m_pPunchingMachineDlg->m_bCheckEngineerDlg = FALSE;

	return CDialog::DestroyWindow();
}


void CEngineerDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CDialog::PostNcDestroy();
}



BOOL CEngineerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::PreTranslateMessage(pMsg);
}


void CEngineerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nID)
	{
		case SC_CLOSE:
			DestroyWindow();
			break;
		defualt:
			break;
	}

	CDialog::OnSysCommand(nID, lParam);
}


void CEngineerDlg::OnBnClickedCancel()
{
	DestroyWindow();
	//CDialog::OnCancel();
}


void CEngineerDlg::OnBnClickedButtonEngineerCamera1LoadParameter()
{
	LoadOpenCVParameter(0);
}


void CEngineerDlg::OnBnClickedButtonEngineerCamera1SaveParameter()
{
	if (AfxMessageBox(_T("Do you want save parameter?"), MB_YESNO) == IDYES)
	{
		SaveOpenCVParameter(0);
	}
}


void CEngineerDlg::OnBnClickedButtonEngineerCamera2LoadParameter()
{
	LoadOpenCVParameter(1);
}


void CEngineerDlg::OnBnClickedButtonEngineerCamera2SaveParameter()
{
	if (AfxMessageBox(_T("Do you want save parameter?"), MB_YESNO) == IDYES)
	{
		SaveOpenCVParameter(1);
	}
}


void CEngineerDlg::OnBnClickedCheckEngineerCamera1Mode()
{
	m_pCameraSubDlg[0].m_iEngineerMode = m_Check_Engineer_Mode[0].GetCheck();

	//if (m_Check_Engineer_Mode[0].GetCheck())
	//{
	//	m_pCameraSubDlg[0].m_iEngineerMode = 1;
	//}
	//else
	//{
	//	m_pCameraSubDlg[0].m_iEngineerMode = 0;
	//}
}



void CEngineerDlg::OnBnClickedCheckEngineerCamera2Mode()
{
	m_pCameraSubDlg[1].m_iEngineerMode = m_Check_Engineer_Mode[1].GetCheck();

	//if (m_Check_Engineer_Mode[1].GetCheck())
	//{
	//	m_pCameraSubDlg[1].m_iEngineerMode = 1;
	//}
	//else
	//{
	//	m_pCameraSubDlg[1].m_iEngineerMode = 0;
	//}
}


void CEngineerDlg::OnBnClickedCheckEngineerShowconsole()
{
	int iArr[2] = { SW_HIDE, SW_SHOW };
	::ShowWindow(GetConsoleWindow(), iArr[m_Check_ShowConsole.GetCheck()]);

	//if (m_Check_ShowConsole.GetCheck())
	//{
	//	::ShowWindow(GetConsoleWindow(), SW_SHOW);
	//}
	//else
	//{
	//	::ShowWindow(GetConsoleWindow(), SW_HIDE);
	//}
}


void CEngineerDlg::OnBnClickedMfcbuttonEngineerCalculateAngle()
{
	// CalAngle 스레드 생성 및 시작
	m_pThreadCalAngle = AfxBeginThread(CEngineerDlg::ThreadCalAngle, this);
	if (!m_pThreadCalAngle) { AfxMessageBox(_T("CreatingThread failed!")); return; }
}


void CEngineerDlg::OnBnClickedButtonEngineerAlignmentSaveParameter()
{
	if (AfxMessageBox(_T("Do you want save parameter?"), MB_YESNO) == IDYES)
	{
		SaveAlignmentParameter();
	}
}


void CEngineerDlg::OnBnClickedButtonEngineerAlignmentLoadParameter()
{
	LoadAlignmentParameter();
}


/*
	멤버함수
*/

void CEngineerDlg::SaveOpenCVParameter(unsigned int index)
{
	CString str_iniFilePath = ".\\config\\opencv_parameter.ini";

	int iArrID_minDist[2] = { IDC_EDIT_ENGINEER_CAMERA1_MINDIST, IDC_EDIT_ENGINEER_CAMERA2_MINDIST };
	int iArrID_Threshold_max_canny[2] = { IDC_EDIT_ENGINEER_CAMERA1_THRESHOLD_MAX_CANNY, IDC_EDIT_ENGINEER_CAMERA2_THRESHOLD_MAX_CANNY };
	int iArrID_Threshold_min_canny[2] = { IDC_EDIT_ENGINEER_CAMERA1_THRESHOLD_MIN_CANNY, IDC_EDIT_ENGINEER_CAMERA2_THRESHOLD_MIN_CANNY };
	int iArrID_Threshold_detection[2] = { IDC_EDIT_ENGINEER_CAMERA1_THRESHOLD_DETECTION, IDC_EDIT_ENGINEER_CAMERA2_THRESHOLD_DETECTION };
	int iArrID_minRadius[2] = { IDC_EDIT_ENGINEER_CAMERA1_MINRADIUS, IDC_EDIT_ENGINEER_CAMERA2_MINRADIUS };
	int iArrID_maxRadius[2] = { IDC_EDIT_ENGINEER_CAMERA1_MAXRADIUS, IDC_EDIT_ENGINEER_CAMERA2_MAXRADIUS };

	int iArrID_Threshold_Blocksize[2] = { IDC_EDIT_ENGINEER_CAMERA1_THRESHOLD_BLOCKSIZE, IDC_EDIT_ENGINEER_CAMERA2_THRESHOLD_BLOCKSIZE };
	int iArrID_Threshold_Max_Offset[2] = { IDC_EDIT_ENGINEER_CAMERA1_THRESHOLD_MAX_OFFSET, IDC_EDIT_ENGINEER_CAMERA2_THRESHOLD_MAX_OFFSET };
	int iArrID_Threshold_Min_Offset[2] = { IDC_EDIT_ENGINEER_CAMERA1_THRESHOLD_MIN_OFFSET, IDC_EDIT_ENGINEER_CAMERA2_THRESHOLD_MIN_OFFSET };
	int iArrID_Approx_Size[2] = { IDC_EDIT_ENGINEER_CAMERA1_APPROX_SIZE, IDC_EDIT_ENGINEER_CAMERA2_APPROX_SIZE };
	int iArrID_Approx_Minarea[2] = { IDC_EDIT_ENGINEER_CAMERA1_APPROX_MINAREA, IDC_EDIT_ENGINEER_CAMERA2_APPROX_MINAREA };
	int iArrID_Approx_Minratio[2] = { IDC_EDIT_ENGINEER_CAMERA1_APPROX_MINRATIO, IDC_EDIT_ENGINEER_CAMERA2_APPROX_MINRATIO };

	CString str_Section;
	CString str_minDist;
	CString str_Threshold_max_canny;
	CString str_Threshold_min_canny;
	CString str_Threshold_detection;
	CString str_minRadius;
	CString str_maxRadius;

	CString str_Threshold_Blocksize;
	CString str_Threshold_Max_Offset;
	CString str_Threshold_Min_Offset;
	CString str_Approx_Size;
	CString str_Approx_Minarea;
	CString str_Approx_Minratio;

	int iminDist = GetDlgItemInt(iArrID_minDist[index]);
	int iThreshold_max_canny = GetDlgItemInt(iArrID_Threshold_max_canny[index]);
	int iThreshold_min_canny = GetDlgItemInt(iArrID_Threshold_min_canny[index]);
	int iThreshold_detection = GetDlgItemInt(iArrID_Threshold_detection[index]);
	GetDlgItemText(iArrID_minRadius[index], str_minRadius);
	GetDlgItemText(iArrID_maxRadius[index], str_maxRadius);

	int iThreshold_Blocksize = GetDlgItemInt(iArrID_Threshold_Blocksize[index]);
	int iThreshold_Max_Offset = GetDlgItemInt(iArrID_Threshold_Max_Offset[index]);
	int iThreshold_Min_Offset = GetDlgItemInt(iArrID_Threshold_Min_Offset[index]);
	int iApprox_Size = GetDlgItemInt(iArrID_Approx_Size[index]);
	int iApprox_Minarea = GetDlgItemInt(iArrID_Approx_Minarea[index]);
	int iApprox_Minratio = GetDlgItemInt(iArrID_Approx_Minratio[index]);

	str_Section.Format("Camera_%d", index + 1);
	str_minDist.Format("%d", iminDist);
	str_Threshold_max_canny.Format("%d", iThreshold_max_canny);
	str_Threshold_min_canny.Format("%d", iThreshold_min_canny);
	str_Threshold_detection.Format("%d", iThreshold_detection);

	str_Threshold_Blocksize.Format("%d", iThreshold_Blocksize);
	str_Threshold_Max_Offset.Format("%d", iThreshold_Max_Offset);
	str_Threshold_Min_Offset.Format("%d", iThreshold_Min_Offset);
	str_Approx_Size.Format("%d", iApprox_Size);
	str_Approx_Minarea.Format("%d", iApprox_Minarea);
	str_Approx_Minratio.Format("%d", iApprox_Minratio);

	WritePrivateProfileString(str_Section, "minDist", str_minDist.GetBuffer(), str_iniFilePath);
	WritePrivateProfileString(str_Section, "threshold_max_canny", str_Threshold_max_canny.GetBuffer(), str_iniFilePath);
	WritePrivateProfileString(str_Section, "threshold_min_canny", str_Threshold_min_canny.GetBuffer(), str_iniFilePath);
	WritePrivateProfileString(str_Section, "threshold_detection", str_Threshold_detection.GetBuffer(), str_iniFilePath);
	WritePrivateProfileString(str_Section, "minRadius", str_minRadius.GetBuffer(), str_iniFilePath);
	WritePrivateProfileString(str_Section, "maxRadius", str_maxRadius.GetBuffer(), str_iniFilePath);

	WritePrivateProfileString(str_Section, "threshold_blocksize", str_Threshold_Blocksize.GetBuffer(), str_iniFilePath);
	WritePrivateProfileString(str_Section, "threshold_max_offset", str_Threshold_Max_Offset.GetBuffer(), str_iniFilePath);
	WritePrivateProfileString(str_Section, "threshold_min_offset", str_Threshold_Min_Offset.GetBuffer(), str_iniFilePath);
	WritePrivateProfileString(str_Section, "approx_size", str_Approx_Size.GetBuffer(), str_iniFilePath);
	WritePrivateProfileString(str_Section, "approx_minarea", str_Approx_Minarea.GetBuffer(), str_iniFilePath);
	WritePrivateProfileString(str_Section, "approx_minratio", str_Approx_Minratio.GetBuffer(), str_iniFilePath);

	str_iniFilePath.Empty();
}
void CEngineerDlg::LoadOpenCVParameter(unsigned int index)
{
	CString str_iniFilePath = ".\\config\\opencv_parameter.ini";

	int iArrID_minDist[2] = { IDC_EDIT_ENGINEER_CAMERA1_MINDIST, IDC_EDIT_ENGINEER_CAMERA2_MINDIST };
	int iArrID_Threshold_max_canny[2] = { IDC_EDIT_ENGINEER_CAMERA1_THRESHOLD_MAX_CANNY, IDC_EDIT_ENGINEER_CAMERA2_THRESHOLD_MAX_CANNY };
	int iArrID_Threshold_min_canny[2] = { IDC_EDIT_ENGINEER_CAMERA1_THRESHOLD_MIN_CANNY, IDC_EDIT_ENGINEER_CAMERA2_THRESHOLD_MIN_CANNY };
	int iArrID_Threshold_detection[2] = { IDC_EDIT_ENGINEER_CAMERA1_THRESHOLD_DETECTION, IDC_EDIT_ENGINEER_CAMERA2_THRESHOLD_DETECTION };
	int iArrID_minRadius[2] = { IDC_EDIT_ENGINEER_CAMERA1_MINRADIUS, IDC_EDIT_ENGINEER_CAMERA2_MINRADIUS };
	int iArrID_maxRadius[2] = { IDC_EDIT_ENGINEER_CAMERA1_MAXRADIUS, IDC_EDIT_ENGINEER_CAMERA2_MAXRADIUS };

	int iArrID_Threshold_Blocksize[2] = { IDC_EDIT_ENGINEER_CAMERA1_THRESHOLD_BLOCKSIZE, IDC_EDIT_ENGINEER_CAMERA2_THRESHOLD_BLOCKSIZE };
	int iArrID_Threshold_Max_Offset[2] = { IDC_EDIT_ENGINEER_CAMERA1_THRESHOLD_MAX_OFFSET, IDC_EDIT_ENGINEER_CAMERA2_THRESHOLD_MAX_OFFSET };
	int iArrID_Threshold_Min_Offset[2] = { IDC_EDIT_ENGINEER_CAMERA1_THRESHOLD_MIN_OFFSET, IDC_EDIT_ENGINEER_CAMERA2_THRESHOLD_MIN_OFFSET };
	int iArrID_Approx_Size[2] = { IDC_EDIT_ENGINEER_CAMERA1_APPROX_SIZE, IDC_EDIT_ENGINEER_CAMERA2_APPROX_SIZE };
	int iArrID_Approx_Minarea[2] = { IDC_EDIT_ENGINEER_CAMERA1_APPROX_MINAREA, IDC_EDIT_ENGINEER_CAMERA2_APPROX_MINAREA };
	int iArrID_Approx_Minratio[2] = { IDC_EDIT_ENGINEER_CAMERA1_APPROX_MINRATIO, IDC_EDIT_ENGINEER_CAMERA2_APPROX_MINRATIO };

	// ini 파일이 없을 경우 에러 메시지 출력
	if (!PathFileExists(str_iniFilePath))
	{
		AfxMessageBox(_T("ini file does not exist!"));
	}
	else
	{
		char str_Section[256];
		char str_minDist[256];
		char str_Threshold_max_canny[256];
		char str_Threshold_min_canny[256];
		char str_Threshold_detection[256];
		char str_minRadius[256];
		char str_maxRadius[256];

		char str_Threshold_Blocksize[256];
		char str_Threshold_Max_Offset[256];
		char str_Threshold_Min_Offset[256];
		char str_Approx_Size[256];
		char str_Approx_Minarea[256];
		char str_Approx_Minratio[256];

		sprintf_s(str_Section, "Camera_%d", index + 1);

		GetPrivateProfileString(str_Section, "minDist", "0", str_minDist, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "threshold_max_canny", "0", str_Threshold_max_canny, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "threshold_min_canny", "0", str_Threshold_min_canny, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "threshold_detection", "0", str_Threshold_detection, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "minRadius", "0", str_minRadius, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "maxRadius", "0", str_maxRadius, 256, str_iniFilePath);

		GetPrivateProfileString(str_Section, "threshold_blocksize", "3", str_Threshold_Blocksize, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "threshold_max_offset", "20", str_Threshold_Max_Offset, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "threshold_min_offset", "1", str_Threshold_Min_Offset, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "approx_size", "0", str_Approx_Size, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "approx_minarea", "0", str_Approx_Minarea, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "approx_minratio", "0", str_Approx_Minratio, 256, str_iniFilePath);

		// EditControl에 ini파일로부터 받아온 값들 넣어준다
		SetDlgItemText(iArrID_minDist[index], str_minDist);
		SetDlgItemText(iArrID_Threshold_max_canny[index], str_Threshold_max_canny);
		SetDlgItemText(iArrID_Threshold_min_canny[index], str_Threshold_min_canny);
		SetDlgItemText(iArrID_Threshold_detection[index], str_Threshold_detection);
		SetDlgItemText(iArrID_minRadius[index], str_minRadius);
		SetDlgItemText(iArrID_maxRadius[index], str_maxRadius);

		SetDlgItemText(iArrID_Threshold_Blocksize[index], str_Threshold_Blocksize);
		SetDlgItemText(iArrID_Threshold_Max_Offset[index], str_Threshold_Max_Offset);
		SetDlgItemText(iArrID_Threshold_Min_Offset[index], str_Threshold_Min_Offset);
		SetDlgItemText(iArrID_Approx_Size[index], str_Approx_Size);
		SetDlgItemText(iArrID_Approx_Minarea[index], str_Approx_Minarea);
		SetDlgItemText(iArrID_Approx_Minratio[index], str_Approx_Minratio);
	}

	str_iniFilePath.Empty();
}

void CEngineerDlg::SaveAlignmentParameter()
{
	CString str_iniFilePath = ".\\config\\camera.ini";

	// ini 파일이 없을 경우 에러 메시지 출력
	if (!PathFileExists(str_iniFilePath))
	{
		AfxMessageBox(_T("ini file does not exist!"));
	}
	else
	{
		char str_buf[256] = { 0, };

		int imaxSearchCount = GetDlgItemInt(IDC_EDIT_ENGINEER_ALIGNMENT_MAX_SEARCH_COUNT);
		int imaxErrorPixel = GetDlgItemInt(IDC_EDIT_ENGINEER_ALIGNMENT_MAX_ERROR_PIXEL);
		int iCheckImageCut = ((CButton*)GetDlgItem(IDC_CHECK_ENGINEER_ALIGNMENT_IMAGECUT))->GetCheck();

		sprintf_s(str_buf, "%d", imaxSearchCount);
		WritePrivateProfileString("ALIGNMENT", "maxSearchCount", str_buf, str_iniFilePath);
		sprintf_s(str_buf, "%d", imaxErrorPixel);
		WritePrivateProfileString("ALIGNMENT", "maxErrorPixel", str_buf, str_iniFilePath);
		sprintf_s(str_buf, "%d", iCheckImageCut);
		WritePrivateProfileString("ALIGNMENT", "CheckImageCut", str_buf, str_iniFilePath);
		UpdateData(TRUE);
		sprintf_s(str_buf, "%d", m_iSearchingAlgorithm);
		WritePrivateProfileString("ALIGNMENT", "SearchingAlgorithm", str_buf, str_iniFilePath);
	}

	str_iniFilePath.Empty();
}

void CEngineerDlg::LoadAlignmentParameter()
{
	CString str_iniFilePath = ".\\config\\camera.ini";

	// ini 파일이 없을 경우 에러 메시지 출력
	if (!PathFileExists(str_iniFilePath))
	{
		AfxMessageBox(_T("ini file does not exist!"));
	}
	else
	{
		char str_buf[256] = { 0, };

		GetPrivateProfileString("ALIGNMENT", "maxSearchCount", "5", str_buf, 256, str_iniFilePath);
		SetDlgItemText(IDC_EDIT_ENGINEER_ALIGNMENT_MAX_SEARCH_COUNT, str_buf);
		GetPrivateProfileString("ALIGNMENT", "maxErrorPixel", 0, str_buf, 256, str_iniFilePath);
		SetDlgItemText(IDC_EDIT_ENGINEER_ALIGNMENT_MAX_ERROR_PIXEL, str_buf);
		GetPrivateProfileString("ALIGNMENT", "CheckImageCut", 0, str_buf, 256, str_iniFilePath);
		((CButton*)GetDlgItem(IDC_CHECK_ENGINEER_ALIGNMENT_IMAGECUT))->SetCheck(_ttoi(str_buf));
		GetPrivateProfileString("ALIGNMENT", "SearchingAlgorithm", 0, str_buf, 256, str_iniFilePath);
		m_iSearchingAlgorithm = _ttoi(str_buf);
		UpdateData(FALSE);
	}

	str_iniFilePath.Empty();
}


// 두 카메라 사이의 각도를 계산한다.
UINT CEngineerDlg::ThreadCalAngle(LPVOID _method)
{
	// 다이얼로그 인스턴스
	CEngineerDlg* engineerdlg = (CEngineerDlg*)_method;

	for (int i = 0; i < 2; i++)
	{
		if (!engineerdlg->m_pCameraSubDlg[i]) { AfxMessageBox(_T("Calculate Angle failed!, m_pCameraSubDlgs is null!")); return 0; }
		if (engineerdlg->m_pCameraSubDlg[i].GetDlgItem(IDC_BUTTON_CONNECT)->IsWindowEnabled() == TRUE) { AfxMessageBox(_T("Calculate Angle failed!, Camera is not connected!")); return 0; }
		if (engineerdlg->m_pCameraSubDlg[i].GetDlgItem(IDC_BUTTON_PLAY)->IsWindowEnabled() == TRUE) { AfxMessageBox(_T("Calculate Angle failed!, Camera do not play!")); return 0; }
	}
	if (!engineerdlg->m_pCalculateAngle) { AfxMessageBox(_T("Calculate Angle failed!, m_pCalculateAngle is null!"));  return 0; }

	CString str_iniFilePath = ".\\config\\opencv_parameter.ini";
	char str_Section[256];
	char str_minDist[256];
	char str_iThreshold_canny[256];
	char str_iThreshold_detection[256];
	char str_iminRadius[256];
	char str_imaxRadius[256];

	CMvImageBuf* pArrConvertedImage[2] = { NULL, };
	int iArrminDist[2] = { 0, };
	int iArrThreshold_canny[2] = { 0, };
	int iArrThreshold_detection[2] = { 0, };
	int iArrminRadius[2] = { 0, };
	int iArrmaxRadius[2] = { 0, };


	for (int i = 0; i < 2; i++)
	{
		// 현재 카메라의 Imagesrc 가져온다
		pArrConvertedImage[i] = engineerdlg->m_pCameraSubDlg[i].getCMvCameraPtr()->getCMvDisplayHelper()->getCMvImageConvertHelper()->getConvertedImage();

		sprintf_s(str_Section, "Camera_%d", i + 1);

		GetPrivateProfileString(str_Section, "minDist", 0, str_minDist, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "threshold_canny", 0, str_iThreshold_canny, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "threshold_detection", 0, str_iThreshold_detection, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "minRadius", 0, str_iminRadius, 256, str_iniFilePath);
		GetPrivateProfileString(str_Section, "maxRadius", 0, str_imaxRadius, 256, str_iniFilePath);

		// Engineer Parameter 가져온다
		iArrminDist[i] = _ttoi(str_minDist);
		iArrThreshold_canny[i] = _ttoi(str_iThreshold_canny);
		iArrThreshold_detection[i] = _ttoi(str_iThreshold_detection);
		iArrminRadius[i] = _ttoi(str_iminRadius);
		iArrmaxRadius[i] = _ttoi(str_imaxRadius);

		if (!pArrConvertedImage[i]) { AfxMessageBox(_T("Calculate Angle failed!, Imagesrc is null!"));  return 0; }
		if (iArrminDist[i] == 0) { AfxMessageBox(_T("Paramter error!, minDist is empty!")); return 0; }
		if (iArrThreshold_canny[i] == 0) { AfxMessageBox(_T("Paramter error!, threshold canny is empty!")); return 0; }
		if (iArrThreshold_detection[i] == 0) { AfxMessageBox(_T("Paramter error!, threshold detection is empty!")); return 0; }
		if (iArrminRadius[i] < 0) { AfxMessageBox(_T("Paramter error!, minRadius is invalid!")); return 0; }
		if (iArrmaxRadius[i] < 0) { AfxMessageBox(_T("Paramter error!, maxRadius is invalid!")); return 0; }
	}

	engineerdlg->GetDlgItem(IDC_MFCBUTTON_ENGINEER_CALCULATE_ANGLE)->EnableWindow(FALSE);

	// Angle 계산 함수 호출
	double dbDegree = engineerdlg->m_pCalculateAngle->CalAngle
	(
		pArrConvertedImage[0]->bufPtr(),				// 첫번째 카메라 이미지소스
		(int)pArrConvertedImage[0]->imageWidth(),		// 이미지 가로 픽셀 수
		(int)pArrConvertedImage[0]->imageHeight(),		// 이미지 세로 픽셀 수
		1,												// dp = 1 >> 입력 이미지와 같은 해상도, dp = 2 >> 입력 이미지의 절반 해상도
		iArrminDist[0],
		iArrThreshold_canny[0],
		iArrThreshold_detection[0],
		iArrminRadius[0],
		iArrmaxRadius[0],
		pArrConvertedImage[1]->bufPtr(),				// 두번째 카메라 이미지소스
		(int)pArrConvertedImage[1]->imageWidth(),		// 이미지 가로 픽셀 수
		(int)pArrConvertedImage[1]->imageHeight(),		// 이미지 세로 픽셀 수
		1,												// dp = 1 >> 입력 이미지와 같은 해상도, dp = 2 >> 입력 이미지의 절반 해상도
		iArrminDist[1],
		iArrThreshold_canny[1],
		iArrThreshold_detection[1],
		iArrminRadius[1],
		iArrmaxRadius[1]
	);


	engineerdlg->GetDlgItem(IDC_MFCBUTTON_ENGINEER_CALCULATE_ANGLE)->EnableWindow(TRUE);

	engineerdlg->m_pThreadCalAngle = NULL;

	return 0;	// 스레드 자동 소멸
}


