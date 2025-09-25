// CameraSubDlg.cpp: 구현 파일
//

#include "pch.h"
#include "PunchingMachineDlg.h"
#include "CMainDlg.h"
#include "CEquipmentDlg.h"
#include "CCalibrationDlg.h"
#include "CameraSubDlg.h"
#include "CDisplayView.h"
#include "afxdialogex.h"
#include <sstream>

extern ::CString getParseLanguageString(::CString strOrl);
#define STD_WIDTH (960)
#define STD_HEIGHT (529)
#define STD_FONT_SIZE (100)

double g_widthRate = 0.0;
double g_heightRate = 0.0;

// CameraSubDlg 대화 상자

IMPLEMENT_DYNAMIC(CameraSubDlg, CDialog)

CameraSubDlg::CameraSubDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CAMERA_DIALOG, pParent)
	, m_pMvCamera(NULL)
	, m_pSearchMark(NULL)
	, m_iCameraIndex(1)
	, m_iEngineerMode(0)
	, m_pThreadSearchMark(NULL)
	, m_ptrCameraDisplay(NULL)
	//, m_iFlag_Show_Hide_CenterMark(0)
	, m_iFlag_Camera_Disconnect(1)
	, m_iThread_timercount(0)
	, m_pDisplayView(NULL)
	, m_iCheck1v1Mode(0)
	, m_iSearchMarkMode(0)
	, m_iCheckAlignment_SearchMark(0)
	, m_iCheckOK_SearchMark(0)
	, m_iPixel_SearchMark(0)
	, m_dbDistMM_X_SearchedMarkCenter(0)
	, m_dbDistMM_Y_SearchedMarkCenter(0)
{
	m_hBrushCenterLine = (HBRUSH)CreateSolidBrush(RGB(96, 96, 96));
}

CameraSubDlg::~CameraSubDlg()
{
	DeleteObject(m_hBrushCenterLine);
}

BOOL CameraSubDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//// Initialize the text display of the interface according to the currently configured language
	//{
	//	this->SetWindowText(getParseLanguageString(_T("_MAINFORM_TEXT")));
	//	GetDlgItem(IDC_STATIC_CAMERA)->SetWindowText(getParseLanguageString(_T("_MAINFORM_CAMERA")));
	//	GetDlgItem(IDC_STATIC_TRIGGER)->SetWindowText(getParseLanguageString(_T("_MAINFORM_SOFT_TRIGGER")));
	//	GetDlgItem(IDC_STATIC_TRIGGER_MODE)->SetWindowText(getParseLanguageString(_T("_MAINFORM_TRIGGER_MODE")));
	//	GetDlgItem(IDC_BUTTON_TRIGGER)->SetWindowText(getParseLanguageString(_T("_MAINFORM_SOFTWARE_TRIGGER")));
	//	GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(getParseLanguageString(_T("_MAINFORM_CONNECT")));
	//	GetDlgItem(IDC_BUTTON_DISCONNECT)->SetWindowText(getParseLanguageString(_T("_MAINFORM_DISCONNECT")));
	//	GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText(getParseLanguageString(_T("_MAINFORM_PLAY")));
	//	GetDlgItem(IDC_BUTTON_STOPPLAY)->SetWindowText(getParseLanguageString(_T("_MAINFORM_STOP_PLAY")));
	//}

	MoveWindow(m_rectDlg);

	CRect rectSubDlg;
	GetClientRect(rectSubDlg);

	g_widthRate = (double)rectSubDlg.BottomRight().x / STD_WIDTH;
	g_heightRate = (double)rectSubDlg.BottomRight().y / STD_HEIGHT;

	//GetDlgItem(IDC_STATIC_CAMERA)->MoveWindow(
	//	(int)(10 * g_widthRate),
	//	(int)(10 * g_heightRate),
	//	(int)((STD_WIDTH - 20) * g_widthRate),
	//	(int)((STD_HEIGHT - 20) * g_heightRate));



	for (int iItemId = IDC_STATIC_TRIGGER; iItemId <= IDC_STATIC_TRIGGER_MODE; ++iItemId)
	{
		CWnd* pWnd = GetDlgItem(iItemId);
		if (NULL == pWnd)
		{
			continue;
		}

		// scale controls based on screen size
		CRect rectTmp;
		pWnd->GetWindowRect(&rectTmp);
		ScreenToClient(&rectTmp);

		CRect rectAdaptor;
		//rectAdaptor.TopLeft().x = (int)((rectTmp.TopLeft().x) * g_widthRate);
		//rectAdaptor.TopLeft().y = (int)((rectTmp.TopLeft().y) * g_heightRate);
		//rectAdaptor.BottomRight().x = (int)((rectTmp.BottomRight().x) * g_widthRate);
		//rectAdaptor.BottomRight().y = (int)((rectTmp.BottomRight().y) * g_heightRate);
		rectAdaptor.TopLeft().x = (int)((rectTmp.TopLeft().x) * 1);
		rectAdaptor.TopLeft().y = (int)((rectTmp.TopLeft().y) * 1);
		rectAdaptor.BottomRight().x = (int)((rectTmp.BottomRight().x) * 1);
		rectAdaptor.BottomRight().y = (int)((rectTmp.BottomRight().y) * 1);

		pWnd->MoveWindow(rectAdaptor);
	
		// scale font based on screen size
		if (IDC_STATIC_TRIGGER == iItemId)
		{
			double iFontRate = g_widthRate;
			if (g_heightRate < g_widthRate)
			{
				iFontRate = g_heightRate;
			}

			int iFontSize = (int)(STD_FONT_SIZE * iFontRate);
			m_font.CreatePointFont(iFontSize, "");
		}
	}

	double dbCameraDisplayRatio = (double)1024 / (double)1280;
	CRect recttmp;
	CRect rectDisplay;
	CRect rectCenterMark;
	CRect rectBtnHorReverse;
	CRect rectBtnVerReverse;
	CRect rectBtn1vs1mode;
	CRect rectAlignmentStatus;
	CRect rectAlignmentAccuracy;
	CRect rectMouseCoordinate;

	m_ptrCameraDisplay = (CListBox*)GetDlgItem(IDC_LIST_DISPLAY);
	m_ptrCameraDisplay->GetWindowRect(&recttmp);
	ScreenToClient(&recttmp);
	rectDisplay.left = rectSubDlg.left + 5;
	rectDisplay.top = recttmp.top;
	rectDisplay.right = rectSubDlg.right - 5;
	rectDisplay.bottom = (int)(rectDisplay.top + (double)rectDisplay.Width() * dbCameraDisplayRatio);

	m_ptrCameraDisplay->MoveWindow(CRect(0 ,0 ,0 ,0));
	m_rectCameraDisplay = rectDisplay;

	rectSubDlg.BottomRight().y = rectDisplay.BottomRight().y + 60;
	GetDlgItem(IDC_STATIC_CAMERA)->MoveWindow(rectSubDlg);

	m_rectDlg.BottomRight().y = rectSubDlg.BottomRight().y + 60;
	MoveWindow(m_rectDlg);

	m_btnReverseX.GetClientRect(&rectBtnHorReverse);
	m_btnReverseY.GetClientRect(&rectBtnVerReverse);
	m_btn1vs1mode.GetClientRect(&rectBtn1vs1mode);
	GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS)->GetClientRect(&rectAlignmentStatus);
	GetDlgItem(IDC_STATIC_CAMERA_SEARCHMARK_ACCURACY)->GetClientRect(&rectAlignmentAccuracy);
	GetDlgItem(IDC_STATIC_CAMERA_MOUSE_COORDINATE)->GetClientRect(&rectMouseCoordinate);

	m_btnReverseX.MoveWindow
	(
		rectDisplay.left, 
		rectDisplay.bottom + 1, 
		rectBtnHorReverse.Width(), 
		rectBtnHorReverse.Height()
	);
	m_btnReverseY.MoveWindow
	(
		rectDisplay.left + rectBtnHorReverse.Width(), 
		rectDisplay.bottom + 1, 
		rectBtnVerReverse.Width(), 
		rectBtnVerReverse.Height()
	);
	m_btn1vs1mode.MoveWindow
	(
		rectDisplay.left + rectBtnHorReverse.Width() + rectBtnVerReverse.Width(), 
		rectDisplay.bottom + 1, 
		rectBtn1vs1mode.Width(), 
		rectBtn1vs1mode.Height()
	);
	GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS)->MoveWindow
	(
		rectDisplay.left + rectBtnHorReverse.Width() + rectBtnVerReverse.Width() + rectBtn1vs1mode.Width() + 50, 
		rectDisplay.bottom + 1, 
		rectAlignmentStatus.Width(), 
		rectAlignmentStatus.Height()
	);
	GetDlgItem(IDC_STATIC_CAMERA_SEARCHMARK_ACCURACY)->MoveWindow
	(
		rectDisplay.left + rectBtnHorReverse.Width() + rectBtnVerReverse.Width() + rectBtn1vs1mode.Width() + rectAlignmentStatus.Width() + 50,
		rectDisplay.bottom + 1,
		rectAlignmentAccuracy.Width(),
		rectAlignmentAccuracy.Height()
	);
	GetDlgItem(IDC_STATIC_CAMERA_MOUSE_COORDINATE)->MoveWindow
	(
		rectDisplay.right - rectMouseCoordinate.Width(), 
		rectDisplay.bottom + 1, 
		rectMouseCoordinate.Width(), 
		rectMouseCoordinate.Height()
	);


	// 카메라 디스플레이 컨트롤 테두리 스타일 변경
	//m_ptrCameraDisplay->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);

	// 카메라 디스플레이 컨트롤 스타일 변경
	//m_ptrCameraDisplay->ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);

	// ScrollView 생성
	CreateDisplayView();


	// Font 설정
	m_fontSearchMarkAccuracy.CreateFont(
		40,                            // 글자높이
		20,                            // 글자너비
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
	GetDlgItem(IDC_STATIC_CAMERA_ALIGNMENT_STATUS)->SetFont(&m_fontSearchMarkAccuracy);


	// disable all controls
	enableWindowAll(FALSE);
	m_btnDisconnect.EnableWindow(FALSE);
	m_btnSearchMarkStart.EnableWindow(FALSE);
	m_btnSearchMarkStop.EnableWindow(FALSE);
	m_btnShowCenterMark.EnableWindow(FALSE);
	m_btnHideCenterMark.EnableWindow(FALSE);
	m_btnReverseX.EnableWindow(FALSE);
	m_btnReverseY.EnableWindow(FALSE);


	// ini 파라메타 초기화
	InitINIParameter();

	// icon 초기화
	InitIcon();

	return TRUE;
}


HBRUSH CameraSubDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	UINT nID = pWnd->GetDlgCtrlID();

	// TODO:  여기서 DC의 특성을 변경합니다.
	switch (nID) {
	case IDC_LIST_DISPLAY:
	{
		//pDC->SetBkMode(TRANSPARENT); //투명하게 설정 
		//return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
		break;

	case IDC_STATIC_CAMERA_ALIGNMENT_STATUS:
	{
		COLORREF textcolor[2] = { RGB(255, 0, 0) , RGB(0, 0, 255) };
		pDC->SetTextColor(textcolor[(BOOL)m_iCheckOK_SearchMark]);
	}
		break;
	case IDC_STATIC_CAMERA_SEARCHMARK_ACCURACY:
	{
		COLORREF textcolor[2] = { RGB(255, 0, 0) , RGB(0, 0, 255) };
		pDC->SetTextColor(textcolor[(BOOL)m_iCheckOK_SearchMark]);
	}
		break;
	default:
		break;
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CameraSubDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.

	// 카메라 디스플레이 영역 테두리 그리기
	int iOffset = 1;
	CRect rect;
	rect = m_rectCameraDisplay;
	rect.left -= iOffset;
	rect.top -= iOffset;
	rect.right += iOffset;
	rect.bottom += iOffset;

	CPen pen, * poldpen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	poldpen = dc.SelectObject(&pen);
	dc.Rectangle(&rect);

	dc.SelectObject(&poldpen);
	pen.DeleteObject();
}

void CameraSubDlg::OnNcPaint()
{
	
}

BOOL CameraSubDlg::OnNcActivate(BOOL bActive)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//this->OnNcPaint();
	return __super::OnNcActivate(bActive);
}

void CameraSubDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDCtl)
	{

	default:
		__super::OnDrawItem(nIDCtl, lpDrawItemStruct);
		break;
	}
}



void CameraSubDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{

	default:
		break;
	}
	__super::OnTimer(nIDEvent);
}

LRESULT CameraSubDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch (message)
	{
	case WM_CLOSE:
	break;

	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CameraSubDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{

		// Shield ESCbutton close dialog action
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CameraSubDlg::OnMyDisconnectMsg(WPARAM wParam, LPARAM lParam)
{
	OnBnClickedButtonDisconnect();
	AfxMessageBox(_T("Camera is disconnected!!!"));
	return 0;
}

BOOL CameraSubDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return __super::OnEraseBkgnd(pDC);
	//return FALSE;
}

void CameraSubDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CameraSubDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	OnBnClickedButtonDisconnect();

	if (m_pDisplayView)
	{
		m_pDisplayView->DestroyWindow();
		m_pDisplayView = NULL;
	}

	if (m_pSearchMark)
	{
		delete m_pSearchMark;
		m_pSearchMark = NULL;
	}

	//DWORD dwresult;
	// (**중요)AfxBeginThread로 생성한 스레드는 CloseHandle()을 할 필요가 없다.
	//if (m_pThreadCenterMark)
	//{
	//	m_iThreadkeyCenterMark = 1;
	//	WaitForSingleObject(m_pThreadCenterMark->m_hThread, INFINITE);
	//	m_pThreadCenterMark = NULL;
	//}

	//if (m_pThreadSearchMark)
	//{
	//	::GetExitCodeThread(m_pThreadSearchMark->m_hThread, &dwresult);
	//	AfxEndThread(dwresult, TRUE);
	//	m_pThreadSearchMark = NULL;
	//}

	OnBnClickedButtonSearchMarkStop();

	return __super::DestroyWindow();
}


void CameraSubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CAMERA, m_editCameraName);
	DDX_Control(pDX, IDC_COMBO_TRIGGERMODE, m_cmbTriggerMode);
	DDX_Control(pDX, IDC_BUTTON_TRIGGER, m_btnTrigger);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_BUTTON_DISCONNECT, m_btnDisconnect);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_btnPlay);
	DDX_Control(pDX, IDC_BUTTON_STOPPLAY, m_btnStopPlay);
	DDX_Control(pDX, IDC_LIST_DISPLAY, m_lbShowWindow);
	DDX_Control(pDX, IDC_BUTTON_SHOW_CENTERMARK, m_btnShowCenterMark);
	DDX_Control(pDX, IDC_BUTTON_HIDE_CENTERMARK, m_btnHideCenterMark);
	DDX_Control(pDX, IDC_BUTTON_REVERSE_X, m_btnReverseX);
	DDX_Control(pDX, IDC_BUTTON_REVERSE_Y, m_btnReverseY);
	DDX_Control(pDX, IDC_BUTTON_SEARCH_MARK_START, m_btnSearchMarkStart);
	DDX_Control(pDX, IDC_BUTTON_SEARCH_MARK_STOP, m_btnSearchMarkStop);
	DDX_Control(pDX, IDC_MFCBUTTON_CAMERA_1V1MODE, m_btn1vs1mode);
}


BEGIN_MESSAGE_MAP(CameraSubDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CameraSubDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CameraSubDlg::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_TRIGGER, &CameraSubDlg::OnBnClickedButtonTrigger)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CameraSubDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOPPLAY, &CameraSubDlg::OnBnClickedButtonStopplay)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGERMODE, &CameraSubDlg::OnCbnSelchangeComboTriggermode)
	ON_MESSAGE(WM_MY_DISCONNECT_MESSAGE, &CameraSubDlg::OnMyDisconnectMsg)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DWMCOLORIZATIONCOLORCHANGED()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_SHOW_CENTERMARK, &CameraSubDlg::OnBnClickedButtonShowCenterfocus)
	ON_BN_CLICKED(IDC_BUTTON_HIDE_CENTERMARK, &CameraSubDlg::OnBnClickedButtonHideCenterfocus)
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
	ON_WM_NCACTIVATE()
	ON_WM_DRAWITEM()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_BN_CLICKED(IDC_BUTTON_REVERSE_X, &CameraSubDlg::OnBnClickedButtonReverseX)
	ON_BN_CLICKED(IDC_BUTTON_REVERSE_Y, &CameraSubDlg::OnBnClickedButtonReverseY)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_MARK_START, &CameraSubDlg::OnBnClickedButtonSearchMarkStart)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_MARK_STOP, &CameraSubDlg::OnBnClickedButtonSearchMarkStop)
	ON_BN_CLICKED(IDC_MFCBUTTON_CAMERA_1V1MODE, &CameraSubDlg::OnBnClickedMfcbuttonCamera1v1mode)
END_MESSAGE_MAP()


// CameraSubDlg 메시지 처리기
void CameraSubDlg::OnOK()
{
	// Shield close dialog action
}

void CameraSubDlg::OnBnClickedButtonConnect()
{
	if (NULL == m_pMvCamera)
	{
		AfxMessageBox(_T("Connect failed, camera is null!"));
		return;
	}

	if (!m_pMvCamera->open((IMvCameraSink *)this))
	{

		// processing connect failure
		AfxMessageBox(_T("Connect failed!"));
		return;
	}

	IMV_DeviceInfo devInfo;
	if (!m_pMvCamera->getDeviceInfo(devInfo))
	{

		// processing connect failure
		AfxMessageBox(_T("getDeviceInfo failed!"));
		return;
	}

	std::string showCamName = "";

	showCamName.append(devInfo.cameraName);
	showCamName.append(" [");
	if (devInfo.nCameraType == typeGigeCamera)
	{
		showCamName.append(devInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress);
	}
	else
	{
		showCamName.append(devInfo.serialNumber);
	}
	showCamName.append("]");

	m_editCameraName.SetWindowText((LPCSTR)showCamName.c_str());


	// initialize camera properties
	initParamProperty();


	// control property control state
	enableProperties(TRUE);


	// Control related button status
	m_btnConnect.EnableWindow(FALSE);
	m_btnDisconnect.EnableWindow(TRUE);
	m_btnPlay.EnableWindow(TRUE);

	m_iFlag_Camera_Disconnect = 0;
}


void CameraSubDlg::OnBnClickedButtonDisconnect()
{
	if (NULL != m_pMvCamera)
	{
		OnBnClickedButtonStopplay();
		m_pMvCamera->close();
	}
	GetDlgItem(IDC_STATIC_CAMERA)->SetWindowText(getParseLanguageString(_T("_MAINFORM_CAMERA")));


	// Control property control edit status
	enableProperties(FALSE);


	// Control related button status
	m_btnConnect.EnableWindow(TRUE);
	m_btnDisconnect.EnableWindow(FALSE);
	m_btnPlay.EnableWindow(FALSE);
	m_btnStopPlay.EnableWindow(FALSE);
	m_btnSearchMarkStart.EnableWindow(FALSE);
	m_btnSearchMarkStop.EnableWindow(FALSE);

	m_iFlag_Camera_Disconnect = 1;
	// Display 화면 지운다.
	InvalidateRect(m_rectCameraDisplay, FALSE);
}


void CameraSubDlg::OnBnClickedButtonTrigger()
{
	if (!m_pMvCamera)
	{
		AfxMessageBox(_T("Trigger failed!, camera is null!"));
		return;
	}

	IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();

	if (IMV_OK != IMV_ExecuteCommandFeature(devHandle, "TriggerSoftware"))
	{
		AfxMessageBox(_T("TriggerSoftware failed!"));
	}
}


void CameraSubDlg::OnBnClickedButtonPlay()
{
	if (NULL != m_pMvCamera)
	{
		m_pMvCamera->startDisplay((VR_HWND)m_lbShowWindow.GetSafeHwnd());
	}
	else
	{
		AfxMessageBox(_T("Start play failed!, camera is null!"));
		return;
	}

	// Control related button status
	m_btnPlay.EnableWindow(FALSE);
	m_btnStopPlay.EnableWindow(TRUE);
	m_btnSearchMarkStart.EnableWindow(TRUE);
	m_btnSearchMarkStop.EnableWindow(TRUE);
	OnCbnSelchangeComboTriggermode();
	m_btnShowCenterMark.EnableWindow(TRUE);
	m_btnHideCenterMark.EnableWindow(FALSE);
	m_btnReverseX.EnableWindow(TRUE);
	m_btnReverseY.EnableWindow(TRUE);
}


void CameraSubDlg::OnBnClickedButtonStopplay()
{
	if (NULL != m_pMvCamera)
	{

		// When triggermode is turned on, it seems that there is a deadlock in the SDK. For now, do this first, and then follow this problem
		// 		procTriggerMode("Off");
		// 		int iIndex = m_cmbTriggerMode.FindStringExact(0, _T("Off"));
		// 		m_cmbTriggerMode.SetCurSel(iIndex);

		m_pMvCamera->stopDisplay();
	}

	// 控制相关按钮状态
	// Control related button status
	m_btnPlay.EnableWindow(TRUE);
	m_btnStopPlay.EnableWindow(FALSE);
	m_btnTrigger.EnableWindow(FALSE);
	m_btnSearchMarkStart.EnableWindow(FALSE);
	m_btnSearchMarkStop.EnableWindow(FALSE);
	m_btnShowCenterMark.EnableWindow(FALSE);
	m_btnHideCenterMark.EnableWindow(FALSE);
	m_btnReverseX.EnableWindow(FALSE);
	m_btnReverseY.EnableWindow(FALSE);
}

void CameraSubDlg::OnCbnSelchangeComboTriggermode()
{
	TCHAR szBuf[24] = { 0 };
	int iIndex = m_cmbTriggerMode.GetCurSel();
	m_cmbTriggerMode.GetLBText(iIndex, szBuf);

#ifdef UNICODE
	std::string strBuf = WStringToString(szBuf);
#else
	std::string strBuf = szBuf;
#endif // UNICODE

	bool bRet = procTriggerMode(strBuf.c_str());
	if (!bRet)
	{
		AfxMessageBox(_T("Set trigger mode failed!"));
	}

	if (strBuf == "On")
	{
		if ((FALSE == m_btnPlay.IsWindowEnabled())
			&& (TRUE == m_btnStopPlay.IsWindowEnabled()))
		{
			m_btnTrigger.EnableWindow(TRUE);
		}
		else
		{
			m_btnTrigger.EnableWindow(FALSE);
		}
		m_btnSearchMarkStart.EnableWindow(FALSE);
		m_btnSearchMarkStop.EnableWindow(FALSE);
	}
	else
	{
		m_btnTrigger.EnableWindow(FALSE);
		m_btnSearchMarkStart.EnableWindow(TRUE);
		m_btnSearchMarkStop.EnableWindow(TRUE);
	}
}

void CameraSubDlg::OnBnClickedButtonSearchMarkStart()
{
	SearchMark();
	m_iSearchMarkMode = 1;
}


void CameraSubDlg::OnBnClickedButtonSearchMarkStop()
{
	m_iSearchMarkMode = 0;
	m_btnSearchMarkStart.EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_CAMERA_SEARCHMARK_ACCURACY)->SetWindowText("");
	GetDlgItem(IDC_STATIC_CAMERA_ALIGNMENT_STATUS)->SetWindowText("");
}

void CameraSubDlg::OnBnClickedButtonShowCenterfocus()
{
	ShowCenterMark();
}


void CameraSubDlg::OnBnClickedButtonHideCenterfocus()
{
	HideCenterMark();
}


void CameraSubDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags, point);
}


void CameraSubDlg::OnMouseHover(UINT nFlags, CPoint point)
{
	__super::OnMouseHover(nFlags, point);
}


void CameraSubDlg::OnBnClickedButtonReverseX()
{
	if (m_pMvCamera->getCMvDisplayHelper() == NULL) return;
	
	if(m_pMvCamera->getCMvDisplayHelper()->m_iCheckReverseX == 1)
		m_pMvCamera->getCMvDisplayHelper()->m_iCheckReverseX = 0;
	else
		m_pMvCamera->getCMvDisplayHelper()->m_iCheckReverseX = 1;
	
}


void CameraSubDlg::OnBnClickedButtonReverseY()
{
	if (m_pMvCamera->getCMvDisplayHelper() == NULL) return;

	if (m_pMvCamera->getCMvDisplayHelper()->m_iCheckReverseY == 1)
		m_pMvCamera->getCMvDisplayHelper()->m_iCheckReverseY = 0;
	else
		m_pMvCamera->getCMvDisplayHelper()->m_iCheckReverseY = 1;
}

void CameraSubDlg::OnBnClickedMfcbuttonCamera1v1mode()
{
	m_iCheck1v1Mode = m_iCheck1v1Mode ? 0 : 1;
	m_pDisplayView->Set1v1Mode((BOOL)m_iCheck1v1Mode);
}


/*
	멤버함수
*/
CMvCamera* CameraSubDlg::getCMvCameraPtr()
{
	return m_pMvCamera;
}

void CameraSubDlg::setSearchMark(unsigned int index)
{
	m_pSearchMark = new CSearchMark();

	if (!m_pSearchMark)
	{
		AfxMessageBox(_T("m_pSearchMark is NULL!"));
	}
}


void CameraSubDlg::onCameraDisconnect()
{
	PostMessage(WM_MY_DISCONNECT_MESSAGE, NULL, NULL);
}

void CameraSubDlg::setRect(CRect rect)
{
	m_rectDlg = rect;
}

void CameraSubDlg::setCamera(unsigned int index)
{
	// 激活子界面
	// activate sub interface
	enableWindowAll(TRUE);

	// 控制属性控件状态
	// control property control state
	enableProperties(FALSE);

	// 控制相关按钮状态
	// control related button status
	m_btnDisconnect.EnableWindow(FALSE);
	m_btnPlay.EnableWindow(FALSE);
	m_btnStopPlay.EnableWindow(FALSE);
	m_btnTrigger.EnableWindow(FALSE);

	if (m_pMvCamera)
	{
		delete m_pMvCamera;
		m_pMvCamera = NULL;
	}
	m_pMvCamera = new CMvCamera(index, m_pDisplayView);
	m_iCameraIndex = index + 1;
	//printf("%d\n", m_iCameraIndex);
	if (!m_pMvCamera)
	{
		AfxMessageBox(_T("m_pMvCamera is NULL!"));
	}
}

void CameraSubDlg::enableProperties(BOOL bEnable)
{
	if (FALSE == bEnable)
	{
		m_cmbTriggerMode.ResetContent();
	}
	m_cmbTriggerMode.EnableWindow(bEnable);
}

void CameraSubDlg::enableWindowAll(BOOL bEnbale)
{
	for (int iItemId = IDC_STATIC_CAMERA; iItemId <= IDC_BUTTON_STOPPLAY; ++iItemId)
	{
		CWnd* pWnd = GetDlgItem(iItemId);
		if (NULL == pWnd)
		{
			continue;
		}

		pWnd->EnableWindow(bEnbale);
	}
}


void CameraSubDlg::initParamProperty()
{
	IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();

	// TriggerMode
	m_cmbTriggerMode.Clear();

	IMV_String enumSymbol;
	int ret = IMV_GetEnumFeatureSymbol(devHandle, "TriggerMode", &enumSymbol);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get TriggerMode symbol value failed!"));
		return;
	}

	std::string triggerModeSymbol = std::string(enumSymbol.str);

	unsigned int nEntryNum = 0;
	ret = IMV_GetEnumFeatureEntryNum(devHandle, "TriggerMode", &nEntryNum);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get TriggerMode settable enumeration number failed!"));
		return;
	}

	IMV_EnumEntryList enumEntryList;
	enumEntryList.nEnumEntryBufferSize = sizeof(IMV_EnumEntryInfo) * nEntryNum;
	enumEntryList.pEnumEntryInfo = (IMV_EnumEntryInfo*)malloc(sizeof(IMV_EnumEntryInfo) * nEntryNum);
	if (NULL == enumEntryList.pEnumEntryInfo)
	{
		AfxMessageBox(_T("Malloc pEnumEntryInfo failed!"));
		return;
	}

	if (IMV_OK != IMV_GetEnumFeatureEntrys(devHandle, "TriggerMode", &enumEntryList))
	{
		free(enumEntryList.pEnumEntryInfo);
		enumEntryList.pEnumEntryInfo = NULL;
		AfxMessageBox(_T("Get TriggerMode settable enumeration value list failed!"));
		return;
	}

	for (unsigned int iIndex = 0; iIndex < nEntryNum; iIndex++)
	{
		m_cmbTriggerMode.InsertString(iIndex, (LPCTSTR)enumEntryList.pEnumEntryInfo[iIndex].name);
		if (triggerModeSymbol == std::string(enumEntryList.pEnumEntryInfo[iIndex].name))
		{
			m_cmbTriggerMode.SetCurSel(iIndex);
		}
	}
	bool bEnable = IMV_FeatureIsWriteable(devHandle, "TriggerMode");
	m_cmbTriggerMode.EnableWindow(bEnable);

	free(enumEntryList.pEnumEntryInfo);
	enumEntryList.pEnumEntryInfo = NULL;
}

bool CameraSubDlg::procTriggerMode(const char* pMode)
{
	if (NULL == m_pMvCamera)
	{
		AfxMessageBox(_T("Set triggerMode failed, camera is null!"));
		return false;
	}

	IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();

	if (IMV_OK != IMV_SetEnumFeatureSymbol(devHandle, "TriggerSelector", "FrameStart"))
	{
		return false;
	}

	if (IMV_OK != IMV_SetEnumFeatureSymbol(devHandle, "TriggerMode", pMode))
	{
		return false;
	}

	return true;
}

void CameraSubDlg::AutoConnect()
{
	if (NULL == m_pMvCamera)
	{
		AfxMessageBox(_T("카메라 연결 실패!"), MB_ICONERROR);
		return;
	}

	if (!m_pMvCamera->open((IMvCameraSink*)this))
	{
		// processing connect failure
		AfxMessageBox(_T("카메라 연결 실패!"), MB_ICONERROR);
		return;
	}

	IMV_DeviceInfo devInfo;
	if (!m_pMvCamera->getDeviceInfo(devInfo))
	{
		// processing connect failure
		AfxMessageBox(_T("카메라 연결 실패!"), MB_ICONERROR);
		return;
	}

	std::string showCamName = "";

	showCamName.append(devInfo.cameraName);
	showCamName.append(" [");
	if (devInfo.nCameraType == typeGigeCamera)
	{
		showCamName.append(devInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress);
	}
	else
	{
		showCamName.append(devInfo.serialNumber);
	}
	showCamName.append("]");

	m_editCameraName.SetWindowText((LPCSTR)showCamName.c_str());


	// initialize camera properties
	initParamProperty();


	// control property control state
	enableProperties(TRUE);


	// Control related button status
	m_btnConnect.EnableWindow(FALSE);
	m_btnDisconnect.EnableWindow(TRUE);
	m_btnPlay.EnableWindow(TRUE);

	m_iFlag_Camera_Disconnect = 0;

	if (NULL != m_pMvCamera)
	{
		m_pMvCamera->startDisplay((VR_HWND)m_lbShowWindow.GetSafeHwnd());
	}
	else
	{
		AfxMessageBox(_T("영상출력 실패!"));
		return;
	}

	// Control related button status
	m_btnPlay.EnableWindow(FALSE);
	m_btnStopPlay.EnableWindow(TRUE);
	m_btnSearchMarkStart.EnableWindow(TRUE);
	m_btnSearchMarkStop.EnableWindow(TRUE);
	OnCbnSelchangeComboTriggermode();
	m_btnShowCenterMark.EnableWindow(TRUE);
	m_btnHideCenterMark.EnableWindow(FALSE);
	m_btnReverseX.EnableWindow(TRUE);
	m_btnReverseY.EnableWindow(TRUE);

	ShowCenterMark();
}



void CameraSubDlg::ShowCenterMark()
{
	if (m_pMvCamera == NULL) return;
	if (m_pMvCamera->getCMvDisplayHelper() == NULL) return;
	m_pMvCamera->getCMvDisplayHelper()->m_iCheckCenterGrid = 1;

	m_btnShowCenterMark.EnableWindow(FALSE);
	m_btnHideCenterMark.EnableWindow(TRUE);
}

void CameraSubDlg::HideCenterMark()
{
	if (m_pMvCamera == NULL) return;
	if (m_pMvCamera->getCMvDisplayHelper() == NULL) return;
	m_pMvCamera->getCMvDisplayHelper()->m_iCheckCenterGrid = 0;

	m_btnShowCenterMark.EnableWindow(TRUE);
	m_btnHideCenterMark.EnableWindow(FALSE);
}

void CameraSubDlg::CreateDisplayView()
{
	m_pDisplayView = new CDisplayView
	(
		m_rectCameraDisplay.Width(), 
		m_rectCameraDisplay.Height(),
		this
	);
	if (m_pDisplayView == NULL) return;
	m_pDisplayView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | WS_CLIPCHILDREN,
		m_rectCameraDisplay, this, 10000 * m_iCameraIndex, NULL);

	m_pDisplayView->OnInitialUpdate();
}


void CameraSubDlg::SearchMark()
{
	m_pThreadSearchMark = AfxBeginThread(CameraSubDlg::ThreadSearchMark, this);
	if (!m_pThreadSearchMark) { AfxMessageBox(_T("CreatingThread failed!")); return; }
}

void CameraSubDlg::Alignment_SearchMark()
{
	m_pThreadAlignment_SearchMark = AfxBeginThread(CameraSubDlg::ThreadAlignment_SearchMark , this);
	if (!m_pThreadAlignment_SearchMark) { AfxMessageBox(_T("CreatingThread failed!")); return; }
}

void CameraSubDlg::InitINIParameter()
{
	CString str_iniFilePath = ".\\config\\camera.ini";

	// ini 파일이 없을 경우 에러 메시지 출력
	if (!PathFileExists(str_iniFilePath))
	{
		AfxMessageBox(_T("ini file does not exist!"));
	}
	else
	{
		char str_maxErrorPixel[256];

		GetPrivateProfileString("ALIGNMENT", "maxErrorPixel", 0, str_maxErrorPixel, 256, str_iniFilePath);

		m_iPixel_SearchMark = _ttoi(str_maxErrorPixel);
	}

	str_iniFilePath.Empty();
}


void CameraSubDlg::InitIcon()
{
	CRect rect;
	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_hIcon_OK = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_OK), IMAGE_ICON, 120, 80, 0);
	m_hIcon_NG = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_NG), IMAGE_ICON, 120, 80, 0);

	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->MoveWindow(rect.left, rect.top, 100, 60);
	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->SetIcon(NULL);
	((CStatic*)GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->Invalidate(FALSE);
}

// 스레드
UINT CameraSubDlg::ThreadSearchMark(LPVOID _method)
{
	// 다이얼로그 인스턴스
	CPunchingMachineDlg* punchingmachinedlg = (CPunchingMachineDlg*)AfxGetApp()->GetMainWnd();
	CameraSubDlg* camerasubdlg = (CameraSubDlg*)_method;


	// Donbo Mark Search 시작
	if (!camerasubdlg->m_pMvCamera) { AfxMessageBox(_T("Search Mark failed!, camera is null!")); return 0; }
	if (!camerasubdlg->m_pSearchMark) { AfxMessageBox(_T("Search Mark failed!, SearchMark is null!")); return 0; }
	// IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();
	CMvImageBuf* pConvertedImage = camerasubdlg->m_pMvCamera->getCMvDisplayHelper()->getCMvImageConvertHelper()->getConvertedImage();


	if (NULL != pConvertedImage)
	{
		CString str_iniFilePath = punchingmachinedlg->m_strPath_opencv_parameter_ini;
		char str_Section[256];
		char str_CheckImageCut[256];
		char str_SearchingAlgorithm[256];

		sprintf_s(str_Section, "Camera_%d", camerasubdlg->m_iCameraIndex);
		GetPrivateProfileString("ALIGNMENT", "SearchingAlgorithm", 0, str_SearchingAlgorithm, 256, punchingmachinedlg->m_strPath_camera_ini);
		GetPrivateProfileString("ALIGNMENT", "CheckImageCut", 0, str_CheckImageCut, 256, punchingmachinedlg->m_strPath_camera_ini);
		
		int iSearchingAlgorithm = _ttoi(str_SearchingAlgorithm);
		int iCheckImageCut = _ttoi(str_CheckImageCut);
		int iResult = -1;

		CString	str_SearchMarkAccuracy;
		double dbAccuracyMaxPixelDist = 0;
		double dbMarkCenterPointX = 0;
		double dbMarkCenterPointY = 0;
		double dbAccuracy = 0;

		// 0 : HoughCircles, 1 : findContours
		switch (iSearchingAlgorithm)
		{
		case 0:
		{
			char str_minDist[256];
			char str_iThreshold_max_canny[256];
			char str_iThreshold_min_canny[256];
			char str_iThreshold_detection[256];
			char str_dbminRadius[256];
			char str_dbmaxRadius[256];

			GetPrivateProfileString(str_Section, "minDist", 0, str_minDist, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "threshold_max_canny", 0, str_iThreshold_max_canny, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "threshold_min_canny", 0, str_iThreshold_min_canny, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "threshold_detection", 0, str_iThreshold_detection, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "minRadius", "0", str_dbminRadius, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "maxRadius", "0", str_dbmaxRadius, 256, str_iniFilePath);

			int iminDist = _ttoi(str_minDist);
			int iThreshold_max_canny = _ttoi(str_iThreshold_max_canny);
			int iThreshold_min_canny = _ttoi(str_iThreshold_min_canny);
			int iThreshold_detection = _ttoi(str_iThreshold_detection);
			double dbminRadius = _ttof(str_dbminRadius);
			double dbmaxRadius = _ttof(str_dbmaxRadius);

			if (iminDist == 0) { printf("Paramter error!, minDist is empty!"); return 0; }
			if (iThreshold_max_canny == 0) { printf("Paramter error!, threshold max canny is empty!"); return 0; }
			if (iThreshold_min_canny == 0) { printf("Paramter error!, threshold min canny is empty!"); return 0; }
			if (iThreshold_detection == 0) { printf("Paramter error!, threshold detection is empty!"); return 0; }
			if (dbminRadius < 0) { printf("Paramter error!, minRadius is invalid!"); return 0; }
			if (dbmaxRadius < 0) { printf("Paramter error!, maxRadius is invalid!"); return 0; }
			if (pConvertedImage->bufPtr() == NULL) { printf("Image error!, Image source is empty!"); return 0; }

			// OpenCV 알고리즘 적용
			camerasubdlg->m_btnSearchMarkStart.EnableWindow(FALSE);

			while (camerasubdlg->m_iSearchMarkMode == 1)
			{
				iResult = camerasubdlg->m_pSearchMark->GetDistFromCircle
				(
					pConvertedImage->bufPtr(),									// 이미지 메모리 버퍼
					(int)pConvertedImage->imageWidth(),							// 이미지 가로 픽셀 수
					(int)pConvertedImage->imageHeight(),						// 이미지 세로 픽셀 수
					(int*)(&camerasubdlg->m_distSearchedMarkCenter.cx),			// 값이 구해질 가로 픽셀거리변수
					(int*)(&camerasubdlg->m_distSearchedMarkCenter.cy),			// 값이 구해질 세로 픽셀거리변수
					1,															// dp = 1 >> 입력 이미지와 같은 해상도, dp = 2 >> 입력 이미지의 절반 해상도
					(double)iminDist,											// 검출할 원의 최소거리
					(double)iThreshold_max_canny,								// 이미지 엣지화 하는데 쓰이는 최대 경계값
					(double)iThreshold_min_canny,								// 이미지 엣지화 하는데 쓰이는 최소 경계값
					(double)iThreshold_detection,								// 축적 배열에서 원검출을 위한 임계값
					(double)dbminRadius,										// 검출될 원의 최소 반지름
					(double)dbmaxRadius,										// 검출될 원의 최대 반지름
					(int)iCheckImageCut,										// 이미지 자르기 옵션	
					(int)camerasubdlg->m_iEngineerMode,							// 엔지니어 모드 상태		
					(int)camerasubdlg->m_iCameraIndex
				);

				if (iResult == -1) { printf("Searching Mark failed!"); }
				// 정확도 계산 및 OK, NG 표시
				else
				{
					dbAccuracyMaxPixelDist = 200;
					dbMarkCenterPointX = camerasubdlg->m_distSearchedMarkCenter.cx;
					dbMarkCenterPointY = camerasubdlg->m_distSearchedMarkCenter.cy;
					dbAccuracy = (double)100 - (sqrt(pow(dbMarkCenterPointX, (double)2) + pow(dbMarkCenterPointY, (double)2)) * (double)100 / dbAccuracyMaxPixelDist);

					str_SearchMarkAccuracy.Format("%.1lf%%", dbAccuracy);
					camerasubdlg->GetDlgItem(IDC_STATIC_CAMERA_SEARCHMARK_ACCURACY)->SetWindowText(str_SearchMarkAccuracy);
					str_SearchMarkAccuracy.Empty();

					if ((abs(camerasubdlg->m_distSearchedMarkCenter.cx) < (long)camerasubdlg->m_iPixel_SearchMark) &&
						(abs(camerasubdlg->m_distSearchedMarkCenter.cy) < (long)camerasubdlg->m_iPixel_SearchMark))
					{
						printf("OK\n");
						camerasubdlg->m_iCheckOK_SearchMark = 1;
						//camerasubdlg->GetDlgItem(IDC_STATIC_CAMERA_ALIGNMENT_STATUS)->SetWindowText("OK");
						((CStatic*)camerasubdlg->GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->SetIcon(camerasubdlg->m_hIcon_OK);
						((CStatic*)camerasubdlg->GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->Invalidate(FALSE);
					}
					else
					{
						printf("NG\n");
						camerasubdlg->m_iCheckOK_SearchMark = 0;
						//camerasubdlg->GetDlgItem(IDC_STATIC_CAMERA_ALIGNMENT_STATUS)->SetWindowText("NG");
						((CStatic*)camerasubdlg->GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->SetIcon(camerasubdlg->m_hIcon_NG);
						((CStatic*)camerasubdlg->GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->Invalidate(FALSE);
					}
				}

				//if (camerasubdlg->m_iEngineerMode == 1) break;
				punchingmachinedlg->Wait(500);
			}
		}
			break;
		case 1:
		{
			char str_Threshold_Blocksize[256];
			char str_Threshold_Max_Offset[256];
			char str_Threshold_Min_Offset[256];
			char str_Approx_Size[256];
			char str_Approx_Minarea[256];
			char str_Approx_Minratio[256];

			GetPrivateProfileString(str_Section, "threshold_blocksize", "3", str_Threshold_Blocksize, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "threshold_max_offset", "20", str_Threshold_Max_Offset, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "threshold_min_offset", "1", str_Threshold_Min_Offset, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "approx_size", "0", str_Approx_Size, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "approx_minarea", "0", str_Approx_Minarea, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "approx_minratio", "0", str_Approx_Minratio, 256, str_iniFilePath);

			int iThreshold_Blocksize = _ttoi(str_Threshold_Blocksize);
			double dbThreshold_Max_Offset = _ttof(str_Threshold_Max_Offset);
			double dbThreshold_Min_Offset = _ttof(str_Threshold_Min_Offset);
			int iApprox_Size = _ttoi(str_Approx_Size);
			double dbApprox_Minarea = _ttof(str_Approx_Minarea);
			double dbApprox_Minratio = _ttof(str_Approx_Minratio);

			// OpenCV 알고리즘 적용
			camerasubdlg->m_btnSearchMarkStart.EnableWindow(FALSE);

			while (camerasubdlg->m_iSearchMarkMode == 1)
			{
				iResult = camerasubdlg->m_pSearchMark->GetDistFromContours
				(
					pConvertedImage->bufPtr(),									// 이미지 메모리 버퍼
					(int)pConvertedImage->imageWidth(),							// 이미지 가로 픽셀 수
					(int)pConvertedImage->imageHeight(),						// 이미지 세로 픽셀 수
					(int*)(&camerasubdlg->m_distSearchedMarkCenter.cx),			// 값이 구해질 가로 픽셀거리변수
					(int*)(&camerasubdlg->m_distSearchedMarkCenter.cy),			// 값이 구해질 세로 픽셀거리변수
					(int)iThreshold_Blocksize,									// 적응형 Threshold에 쓰일 행렬 사이즈
					(double)dbThreshold_Max_Offset,								// 적응형 Threshold에 적용될 때 평균 값에서 빼줄 값
					(double)dbThreshold_Min_Offset,								// 적응형 Threshold에 적용될 때 평균 값에서 빼줄 값
					(int)iApprox_Size,											// 검출된 윤곽선들을 근사화 했을 때 객체를 판별하기 위한 최소 꼭지점의 개수
					(double)dbApprox_Minarea,									// 검출될 객체의 최소 넓이
					(double)dbApprox_Minratio,									// 검출될 객체의 넓이와 외곽선 길이의 최소비율 (1에 가까울수록 원)
					(int)iCheckImageCut,										// 이미지 자르기 옵션			
					(int)camerasubdlg->m_iEngineerMode,							// 엔지니어 모드 상태	
					(int)camerasubdlg->m_iCameraIndex
				);

				if (iResult == -1) { printf("Searching Mark failed!"); }

				// 정확도 계산 및 OK, NG 표시
				else
				{
					dbAccuracyMaxPixelDist = 200;
					dbMarkCenterPointX = camerasubdlg->m_distSearchedMarkCenter.cx;
					dbMarkCenterPointY = camerasubdlg->m_distSearchedMarkCenter.cy;
					dbAccuracy = (double)100 - (sqrt(pow(dbMarkCenterPointX, (double)2) + pow(dbMarkCenterPointY, (double)2)) * (double)100 / dbAccuracyMaxPixelDist);

					str_SearchMarkAccuracy.Format("%.1lf%%", dbAccuracy);
					camerasubdlg->GetDlgItem(IDC_STATIC_CAMERA_SEARCHMARK_ACCURACY)->SetWindowText(str_SearchMarkAccuracy);
					str_SearchMarkAccuracy.Empty();

					if ((abs(camerasubdlg->m_distSearchedMarkCenter.cx) < (long)camerasubdlg->m_iPixel_SearchMark) &&
						(abs(camerasubdlg->m_distSearchedMarkCenter.cy) < (long)camerasubdlg->m_iPixel_SearchMark))
					{
						printf("OK\n");
						camerasubdlg->m_iCheckOK_SearchMark = 1;
						//camerasubdlg->GetDlgItem(IDC_STATIC_CAMERA_ALIGNMENT_STATUS)->SetWindowText("OK");
						((CStatic*)camerasubdlg->GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->SetIcon(camerasubdlg->m_hIcon_OK);
						((CStatic*)camerasubdlg->GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->Invalidate(FALSE);
					}
					else
					{
						printf("NG\n");
						camerasubdlg->m_iCheckOK_SearchMark = 0;
						//camerasubdlg->GetDlgItem(IDC_STATIC_CAMERA_ALIGNMENT_STATUS)->SetWindowText("NG");
						((CStatic*)camerasubdlg->GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->SetIcon(camerasubdlg->m_hIcon_NG);
						((CStatic*)camerasubdlg->GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->Invalidate(FALSE);
					}
				}

				//if (camerasubdlg->m_iEngineerMode == 1) break;
				punchingmachinedlg->Wait(500);
			}
		}
			break;

		default:
			break;
		}

		delete pConvertedImage;
		pConvertedImage = NULL;
	}

	if (camerasubdlg->m_iFlag_Camera_Disconnect == 0)	camerasubdlg->m_btnSearchMarkStart.EnableWindow(TRUE);
	camerasubdlg->m_pThreadSearchMark = NULL;

	return 0;	// 스레드 자동 소멸
}

UINT CameraSubDlg::ThreadAlignment_SearchMark(LPVOID _method)
{
	// 다이얼로그 인스턴스
	CPunchingMachineDlg* punchingmachinedlg = (CPunchingMachineDlg*)AfxGetApp()->GetMainWnd();
	CameraSubDlg* camerasubdlg = (CameraSubDlg*)_method;

	// Donbo Mark 서칭 시작
	if (!camerasubdlg->m_pMvCamera) { AfxMessageBox(_T("Search Mark failed!, camera is null!")); return 0; }
	if (!camerasubdlg->m_pSearchMark) { AfxMessageBox(_T("Search Mark failed!, SearchMark is null!")); return 0; }
	// IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();
	CMvImageBuf* pConvertedImage = camerasubdlg->m_pMvCamera->getCMvDisplayHelper()->getCMvImageConvertHelper()->getConvertedImage();

	if (NULL != pConvertedImage)
	{
		CString str_iniFilePath = punchingmachinedlg->m_strPath_opencv_parameter_ini;
		char str_Section[256];
		char str_CheckImageCut[256];
		char str_SearchingAlgorithm[256];

		sprintf_s(str_Section, "Camera_%d", camerasubdlg->m_iCameraIndex);
		GetPrivateProfileString("ALIGNMENT", "SearchingAlgorithm", 0, str_SearchingAlgorithm, 256, punchingmachinedlg->m_strPath_camera_ini);
		GetPrivateProfileString("ALIGNMENT", "CheckImageCut", 0, str_CheckImageCut, 256, punchingmachinedlg->m_strPath_camera_ini);

		int iCheckImageCut = _ttoi(str_CheckImageCut);
		int iSearchingAlgorithm = _ttoi(str_SearchingAlgorithm);
		int iResult = -1;

		// 0 : HoughCircles, 1 : findContours
		switch (iSearchingAlgorithm)
		{
		case 0:
		{
			char str_minDist[256];
			char str_Threshold_max_canny[256];
			char str_Threshold_min_canny[256];
			char str_Threshold_detection[256];
			char str_minRadius[256];
			char str_maxRadius[256];

			GetPrivateProfileString(str_Section, "minDist", 0, str_minDist, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "threshold_max_canny", 0, str_Threshold_max_canny, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "threshold_min_canny", 0, str_Threshold_min_canny, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "threshold_detection", 0, str_Threshold_detection, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "minRadius", "0", str_minRadius, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "maxRadius", "0", str_maxRadius, 256, str_iniFilePath);


			int iminDist = _ttoi(str_minDist);
			int iThreshold_max_canny = _ttoi(str_Threshold_max_canny);
			int iThreshold_min_canny = _ttoi(str_Threshold_min_canny);
			int iThreshold_detection = _ttoi(str_Threshold_detection);
			double dbminRadius = _ttof(str_minRadius);
			double dbmaxRadius = _ttof(str_maxRadius);


			if (iminDist == 0) { printf("Paramter error!, minDist is empty!"); return 0; }
			if (iThreshold_max_canny == 0) { printf("Paramter error!, threshold max canny is empty!"); return 0; }
			if (iThreshold_min_canny == 0) { printf("Paramter error!, threshold min canny is empty!"); return 0; }
			if (iThreshold_detection == 0) { printf("Paramter error!, threshold detection is empty!"); return 0; }
			if (dbminRadius < 0) { printf("Paramter error!, minRadius is invalid!"); return 0; }
			if (dbmaxRadius < 0) { printf("Paramter error!, maxRadius is invalid!"); return 0; }
			if (pConvertedImage->bufPtr() == NULL) { printf("Image error!, Image source is empty!"); return 0; }

			// OpenCV 알고리즘 적용
			//camerasubdlg->m_btnSearchMarkStart.EnableWindow(FALSE);

			iResult = camerasubdlg->m_pSearchMark->GetDistFromCircle
			(
				pConvertedImage->bufPtr(),									// 이미지 메모리 버퍼
				(int)pConvertedImage->imageWidth(),							// 이미지 가로 픽셀 수
				(int)pConvertedImage->imageHeight(),						// 이미지 세로 픽셀 수
				(int*)(&camerasubdlg->m_distSearchedMarkCenter.cx),			// 값이 구해질 가로 픽셀거리변수
				(int*)(&camerasubdlg->m_distSearchedMarkCenter.cy),			// 값이 구해질 세로 픽셀거리변수
				1,															// dp = 1 >> 입력 이미지와 같은 해상도, dp = 2 >> 입력 이미지의 절반 해상도
				(double)iminDist,											// 검출할 원의 최소거리
				(double)iThreshold_max_canny,								// 이미지 엣지화 하는데 쓰이는 최대 경계값
				(double)iThreshold_min_canny,								// 이미지 엣지화 하는데 쓰이는 최소 경계값
				(double)iThreshold_detection,								// 축적 배열에서 원검출을 위한 임계값
				(double)dbminRadius,										// 검출될 원의 최소 반지름
				(double)dbmaxRadius,										// 검출될 원의 최대 반지름
				(int)iCheckImageCut,										// 이미지 자르기 옵션			
				(int)camerasubdlg->m_iEngineerMode,							// 엔지니어 모드 상태	
				(int)camerasubdlg->m_iCameraIndex
			);
		}
			break;
		case 1:
		{
			char str_Threshold_Blocksize[256];
			char str_Threshold_Max_Offset[256];
			char str_Threshold_Min_Offset[256];
			char str_Approx_Size[256];
			char str_Approx_Minarea[256];
			char str_Approx_Minratio[256];

			GetPrivateProfileString(str_Section, "threshold_blocksize", "3", str_Threshold_Blocksize, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "threshold_max_offset", "0", str_Threshold_Max_Offset, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "threshold_min_offset", "0", str_Threshold_Min_Offset, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "approx_size", "0", str_Approx_Size, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "approx_minarea", "0", str_Approx_Minarea, 256, str_iniFilePath);
			GetPrivateProfileString(str_Section, "approx_minratio", "0", str_Approx_Minratio, 256, str_iniFilePath);

			int iThreshold_Blocksize = _ttoi(str_Threshold_Blocksize);
			double dbThreshold_Max_Offset = _ttof(str_Threshold_Max_Offset);
			double dbThreshold_Min_Offset = _ttof(str_Threshold_Min_Offset);
			int iApprox_Size = _ttoi(str_Approx_Size);
			double dbApprox_Minarea = _ttof(str_Approx_Minarea);
			double dbApprox_Minratio = _ttof(str_Approx_Minratio);

			// OpenCV 알고리즘 적용
			iResult = camerasubdlg->m_pSearchMark->GetDistFromContours
			(
				pConvertedImage->bufPtr(),									// 이미지 메모리 버퍼
				(int)pConvertedImage->imageWidth(),							// 이미지 가로 픽셀 수
				(int)pConvertedImage->imageHeight(),						// 이미지 세로 픽셀 수
				(int*)(&camerasubdlg->m_distSearchedMarkCenter.cx),			// 값이 구해질 가로 픽셀거리변수
				(int*)(&camerasubdlg->m_distSearchedMarkCenter.cy),			// 값이 구해질 세로 픽셀거리변수
				(int)iThreshold_Blocksize,									// 적응형 Threshold에 쓰일 행렬 사이즈
				(double)dbThreshold_Max_Offset,								// 적응형 Threshold에 적용될 때 평균 값에서 빼줄 값
				(double)dbThreshold_Min_Offset,								// 적응형 Threshold에 적용될 때 평균 값에서 빼줄 값
				(int)iApprox_Size,											// 검출된 윤곽선들을 근사화 했을 때 객체를 판별하기 위한 최소 꼭지점의 개수
				(double)dbApprox_Minarea,									// 검출될 객체의 최소 넓이
				(double)dbApprox_Minratio,									// 검출될 객체의 넓이와 외곽선 길이의 최소비율 (1에 가까울수록 원)
				(int)iCheckImageCut,										// 이미지 자르기 옵션			
				(int)camerasubdlg->m_iEngineerMode,							// 엔지니어 모드 상태	
				(int)camerasubdlg->m_iCameraIndex
			);
		}
			break;
		default:
			break;
		}

		delete pConvertedImage;
		pConvertedImage = NULL;

		if (iResult == -1) 
		{ 
			//AfxMessageBox(_T("Searching Mark failed!"));
			//camerasubdlg->m_btnAlignment.EnableWindow(TRUE);
			camerasubdlg->m_iCheckAlignment_SearchMark = -1;
			camerasubdlg->m_pThreadAlignment_SearchMark = NULL;
			return 0;
		}

		printf("Center X : %d, Y : %d\n", camerasubdlg->m_distSearchedMarkCenter.cx, camerasubdlg->m_distSearchedMarkCenter.cy);

		//printf("Center X abs : %d, Y : %d\n", abs(camerasubdlg->m_distSearchedMarkCenter.cx), abs(camerasubdlg->m_distSearchedMarkCenter.cy));

		if ((abs(camerasubdlg->m_distSearchedMarkCenter.cx) < (long)camerasubdlg->m_iPixel_SearchMark) &&
			(abs(camerasubdlg->m_distSearchedMarkCenter.cy) < (long)camerasubdlg->m_iPixel_SearchMark))
		{
			printf("OK\n");
			camerasubdlg->m_iCheckOK_SearchMark = 1;
		}
		else
		{
			printf("NG\n");
			camerasubdlg->m_iCheckOK_SearchMark = 0;
		}

		// 정확도 계산 및 OK, NG 표시	
		CString	str_SearchMarkAccuracy;
		double dbAccuracyMaxPixelDist = 200;
		double dbMarkCenterPointX = camerasubdlg->m_distSearchedMarkCenter.cx;
		double dbMarkCenterPointY = camerasubdlg->m_distSearchedMarkCenter.cy;
		double dbAccuracy = (double)100 - (sqrt(pow(dbMarkCenterPointX, (double)2) + pow(dbMarkCenterPointY, (double)2)) * (double)100 / dbAccuracyMaxPixelDist);

		str_SearchMarkAccuracy.Format("%.1lf%%", dbAccuracy);
		camerasubdlg->GetDlgItem(IDC_STATIC_CAMERA_SEARCHMARK_ACCURACY)->SetWindowText(str_SearchMarkAccuracy);
		str_SearchMarkAccuracy.Empty();

		//camerasubdlg->m_iCheckOK_SearchMark ?
		//	camerasubdlg->GetDlgItem(IDC_STATIC_CAMERA_ALIGNMENT_STATUS)->SetWindowText("OK") :
		//	camerasubdlg->GetDlgItem(IDC_STATIC_CAMERA_ALIGNMENT_STATUS)->SetWindowText("NG");

		camerasubdlg->m_iCheckOK_SearchMark ?
			((CStatic*)camerasubdlg->GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->SetIcon(camerasubdlg->m_hIcon_OK) :
			((CStatic*)camerasubdlg->GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->SetIcon(camerasubdlg->m_hIcon_NG);
		((CStatic*)camerasubdlg->GetDlgItem(IDC_PICTURECONTROL_CAMERA_ALIGNMENT_STATUS))->Invalidate(FALSE);
		

		// Pixel To MM
		char str_Offset[256] = { 0, };
		GetPrivateProfileString(str_Section, "OFFSET", "1", str_Offset, 256, punchingmachinedlg->m_strPath_camera_ini);
		double dbOffset = _ttof(str_Offset);
		int iCameraDPI = CAMERA_DPI;
		camerasubdlg->m_dbDistMM_X_SearchedMarkCenter = ((double)camerasubdlg->m_distSearchedMarkCenter.cx * (double)25.4 / (double)iCameraDPI) * dbOffset;
		camerasubdlg->m_dbDistMM_Y_SearchedMarkCenter = ((double)camerasubdlg->m_distSearchedMarkCenter.cy * (double)25.4 / (double)iCameraDPI) * dbOffset;

		printf("Camera %d | X : %lf | Y : %lf\n",
			camerasubdlg->m_iCameraIndex,
			camerasubdlg->m_dbDistMM_X_SearchedMarkCenter,
			camerasubdlg->m_dbDistMM_Y_SearchedMarkCenter
		);

	}

	camerasubdlg->m_iCheckAlignment_SearchMark = 1;
	camerasubdlg->m_pThreadAlignment_SearchMark = NULL;

	return 0;	// 스레드 자동 소멸
}


