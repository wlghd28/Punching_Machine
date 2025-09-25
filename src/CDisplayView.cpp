#include "pch.h"
#include "CDisplayView.h"
#include "PunchingMachineDlg.h"
#include "CMainDlg.h"
#include "CameraSubDlg.h"
#include "CCalibrationDlg.h"

IMPLEMENT_DYNAMIC(CDisplayView, CScrollView)


CDisplayView::CDisplayView
(
	int iWidth, 
	int iHeight,
	CameraSubDlg* ptrCameraSubDlg
)
	: m_dbZoomRatio(MINJOOMRATIO)
	, m_iImageWidth(iWidth  /* - GetSystemMetrics(SM_CXVSCROLL)*/)
	, m_iImageHeight(iHeight /* - GetSystemMetrics(SM_CXHSCROLL)*/)
	, m_pImgBuf(NULL)
	, m_ptrCameraSubDlg(ptrCameraSubDlg)
{
#ifdef GRAYMODE
	// 카메라 영상 비트맵 정보 세팅
	m_ptrBitmapInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
	if (m_ptrBitmapInfo == NULL) return;

	m_ptrBitmapInfo->bmiHeader.biSize = sizeof(m_ptrBitmapInfo->bmiHeader);
	m_ptrBitmapInfo->bmiHeader.biWidth = CAMERA_WIDTH;
	m_ptrBitmapInfo->bmiHeader.biHeight = (-1) * CAMERA_HEIGHT;
	m_ptrBitmapInfo->bmiHeader.biPlanes = 1;
	m_ptrBitmapInfo->bmiHeader.biBitCount = 8;
	m_ptrBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_ptrBitmapInfo->bmiHeader.biSizeImage = CAMERA_WIDTH * CAMERA_HEIGHT;
	m_ptrBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_ptrBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_ptrBitmapInfo->bmiHeader.biClrUsed = 0;
	m_ptrBitmapInfo->bmiHeader.biClrImportant = 0;

	for (int i = 0; i < 256; i++)
	{
		m_ptrBitmapInfo->bmiColors[i].rgbRed = i;
		m_ptrBitmapInfo->bmiColors[i].rgbGreen = i;
		m_ptrBitmapInfo->bmiColors[i].rgbBlue = i;
		m_ptrBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
#endif
#ifdef COLORMODE
	m_ptrBitmapInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER));
	if (m_ptrBitmapInfo == NULL) return;

	m_ptrBitmapInfo->bmiHeader.biSize = sizeof(m_ptrBitmapInfo->bmiHeader);
	m_ptrBitmapInfo->bmiHeader.biWidth = CAMERA_WIDTH;
	m_ptrBitmapInfo->bmiHeader.biHeight = (-1) * CAMERA_HEIGHT;
	m_ptrBitmapInfo->bmiHeader.biPlanes = 1;
	m_ptrBitmapInfo->bmiHeader.biBitCount = 24;
	m_ptrBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_ptrBitmapInfo->bmiHeader.biSizeImage = CAMERA_WIDTH * CAMERA_HEIGHT;
	m_ptrBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_ptrBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_ptrBitmapInfo->bmiHeader.biClrUsed = 0;
	m_ptrBitmapInfo->bmiHeader.biClrImportant = 0;
#endif
}
CDisplayView::~CDisplayView()
{
	if (m_ptrBitmapInfo)
	{
		delete m_ptrBitmapInfo;
		m_ptrBitmapInfo = NULL;
	}
}

void CDisplayView::DoDataExchange(CDataExchange* pDX)
{
	CScrollView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDisplayView, CScrollView)
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/*
	MFC 메시지 처리기
*/
BOOL CDisplayView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName
	, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd
	, UINT nID, CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName,
		dwStyle, rect, pParentWnd, nID, pContext);
}

void CDisplayView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	//이 뷰의 전체 크기를 계산합니다.	
	//SetScrollSizes 초기화를 하지 않으면 에러 발생.
	m_ScrollViewSize.cx = m_iImageWidth;
	m_ScrollViewSize.cy = m_iImageHeight;

#ifdef DISPLAY_VER_2
	m_PrintViewSize.cx = m_iImageWidth;
	m_PrintViewSize.cy = m_iImageHeight;

	m_CaptureSize.cx = CAMERA_WIDTH;
	m_CaptureSize.cy = CAMERA_HEIGHT;
#endif

	SetScrollSizes(MM_TEXT, m_ScrollViewSize);
#ifdef DISPLAY_VER_1
	SetScaledPixelRatio();
#endif
	SetTextZoomRatio();
	EnableScrollBarCtrl(SB_BOTH, FALSE);
	//Test();
}


void CDisplayView::OnDraw(CDC* pDC)
{
	pDC->SetStretchBltMode(COLORONCOLOR);	// 이미지 축소 시 깨짐현상 방지
	//printf("%d\n", pDC->GetStretchBltMode());
	//printf("%p %d %d\n", this, m_ViewSize.cx, m_ViewSize.cy);
#ifdef DISPLAY_VER_1
	StretchDIBits
	(
		pDC->m_hDC,							// 출력 대상의 DC 핸들
		0,									// 출력 대상의 좌상귀 x 좌표
		0,									// 출력 대상의 좌상귀 y 좌표
		(int)m_ScrollViewSize.cx,			// 출력 대상의 사각형 너비
		(int)m_ScrollViewSize.cy,			// 출력 대상의 사각형 높이
		0,									// DIB 원본의 좌상귀 x 좌표
		0,									// DIB 원본의 좌상귀 y 좌표
		CAMERA_WIDTH,						// DIB 원본 사각형 너비
		CAMERA_HEIGHT,						// DIB 원본 사각형 높이
		m_pImgBuf,							// 픽셀 데이터 시작 주소
		m_ptrBitmapInfo,					// BITMAPINFO 구조체 시작 주소
		DIB_RGB_COLORS,						// RGB 또는 색상 테이블 인덱스
		SRCCOPY								// 래스터 연산 코드
	);
#endif

#ifdef DISPLAY_VER_2
	register CPoint pt = GetScrollPosition();
	CalPrintViewSize(pt);
	CalCaptureSize(pt);

	// DIB 원본의 좌상귀 y좌표가 왜인지는 모르겠으나 상하반전이 되어있음...(추후 알아볼것)
	StretchDIBits
	(
		pDC->GetSafeHdc(),											// 출력 대상의 DC 핸들
		m_PrintLeftUpper.x,											// 출력 대상의 좌상귀 x 좌표
		m_PrintLeftUpper.y,											// 출력 대상의 좌상귀 y 좌표
		m_PrintViewSize.cx,											// 출력 대상의 사각형 너비
		m_PrintViewSize.cy,											// 출력 대상의 사각형 높이
		m_CapLeftUpper.x,											// DIB 원본의 좌상귀 x 좌표
		CAMERA_HEIGHT - m_CaptureSize.cy - m_CapLeftUpper.y,			// DIB 원본의 좌상귀 y 좌표
		m_CaptureSize.cx,											// DIB 원본 사각형 너비
		m_CaptureSize.cy,											// DIB 원본 사각형 높이
		m_pImgBuf,													// 픽셀 데이터 시작 주소
		m_ptrBitmapInfo,											// BITMAPINFO 구조체 시작 주소
		DIB_RGB_COLORS,												// RGB 또는 색상 테이블 인덱스
		SRCCOPY														// 래스터 연산 코드
	);
#endif
}


BOOL CDisplayView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	if (SB_THUMBTRACK == LOBYTE(nScrollCode))
	{
		SCROLLINFO info;
		if (GetScrollInfo(SB_HORZ, &info, SIF_TRACKPOS))
			nPos = info.nTrackPos;
	}
	else if (SB_THUMBTRACK == HIBYTE(nScrollCode))
	{
		SCROLLINFO info;
		if (GetScrollInfo(SB_VERT, &info, SIF_TRACKPOS))
			nPos = info.nTrackPos;
	}
	Invalidate(FALSE);
	return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}

BOOL CDisplayView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_ptrCameraSubDlg->m_iCheck1v1Mode == 1) return FALSE;

	ScreenToClient(&pt);
	m_curMousePos_All = pt + GetScrollPosition();

	if (zDelta > 0)
	{
		if (m_dbZoomRatio < MAXJOOMRATIO)
		{
			m_dbZoomRatio *= SCALERATIO;
			SetScaleScroll(m_iImageWidth, m_iImageHeight, m_dbZoomRatio);
			CalSetScrollPos(m_dbZoomRatio, pt, SCALERATIO);
#ifdef DISPLAY_VER_1
			SetScaledPixelRatio();
#endif
			SetTextZoomRatio();
			Invalidate(FALSE);
			//RedrawWindow();
		}
	}
	else if (zDelta < 0)
	{
		if (m_dbZoomRatio > MINJOOMRATIO)
		{
			m_dbZoomRatio /= SCALERATIO;
			SetScaleScroll(m_iImageWidth, m_iImageHeight, m_dbZoomRatio);
			CalSetScrollPos(m_dbZoomRatio, pt, 1 / SCALERATIO);
#ifdef DISPLAY_VER_1
			SetScaledPixelRatio();
#endif
			SetTextZoomRatio();
			Invalidate(FALSE);
			if(m_dbZoomRatio <= MINJOOMRATIO) EnableScrollBarCtrl(SB_BOTH, FALSE);
			//RedrawWindow();
		}
	}

	SetTextMouseCoordinate(pt);

	return FALSE;
}

void CDisplayView::OnMouseMove(UINT nFlags, CPoint point)
{
	m_curMousePos_All = point + GetScrollPosition();

	if (nFlags == MK_LBUTTON
		&& (m_dbZoomRatio > MINJOOMRATIO || m_ptrCameraSubDlg->m_iCheck1v1Mode == 1))
	{
		ScrollToPosition(GetScrollPosition() - (point - m_curMousePos_View));
		Invalidate(FALSE);
	}
	else
		SetTextMouseCoordinate(point);

	m_curMousePos_View = point;
	
	CScrollView::OnMouseMove(nFlags, point);
}

void CDisplayView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPunchingMachineDlg* pPunchingMachineDlg = (CPunchingMachineDlg*)AfxGetMainWnd();
	if (pPunchingMachineDlg == NULL) return;

	if (pPunchingMachineDlg->m_bCheckCalibrationDlg)
	{
		CCalibrationDlg* pCalibrationDlg = pPunchingMachineDlg->GetPtrCalibrationDlg();
		if (pCalibrationDlg == NULL) return;

		if (pCalibrationDlg->m_CheckBtn_Picking1[m_ptrCameraSubDlg->m_iCameraIndex - 1].GetCheck())
			pCalibrationDlg->SetPickPoint1(m_curScaledPos, m_ptrCameraSubDlg->m_iCameraIndex - 1);

		if (pCalibrationDlg->m_CheckBtn_Picking2[m_ptrCameraSubDlg->m_iCameraIndex - 1].GetCheck())
			pCalibrationDlg->SetPickPoint2(m_curScaledPos, m_ptrCameraSubDlg->m_iCameraIndex - 1);
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CDisplayView::OnSize(UINT nType, int cx, int cy)
{
	//CScrollView::OnSize(nType, cx, cy);
}

BOOL CDisplayView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return FALSE;
}

/*
	멤버함수
*/
void CDisplayView::SetPtrImgBuf(unsigned char* pImg)
{
	m_pImgBuf = pImg;
}

void  CDisplayView::SetScaleScroll(int iWidth, int iHeight, double dbZoomRatio)
{
	if (dbZoomRatio == MINJOOMRATIO)
	{
		m_ScrollViewSize.cx = (int)((double)iWidth * dbZoomRatio);
		m_ScrollViewSize.cy = (int)((double)iHeight * dbZoomRatio);
	}
	else
	{
		m_ScrollViewSize.cx = (int)((double)(iWidth - GetSystemMetrics(SM_CXVSCROLL)) * dbZoomRatio);
		m_ScrollViewSize.cy = (int)((double)(iHeight -GetSystemMetrics(SM_CXHSCROLL)) * dbZoomRatio);
	}

	SetScrollSizes(MM_TEXT, m_ScrollViewSize);
}

void CDisplayView::CalSetScrollPos(double dbZoomRatio, CPoint pt, double dbScaleRatio)
{
	POINT tmpPoint;

	m_curMousePos_All.x = (int)(((double)m_curMousePos_All.x / (double)(dbZoomRatio / dbScaleRatio)) * (double)dbZoomRatio);
	m_curMousePos_All.y = (int)(((double)m_curMousePos_All.y / (double)(dbZoomRatio / dbScaleRatio)) * (double)dbZoomRatio);

	tmpPoint.x = m_curMousePos_All.x - pt.x;
	tmpPoint.y = m_curMousePos_All.y - pt.y;

	if (dbZoomRatio <= MINJOOMRATIO)
	{
		tmpPoint.x = 0;
		tmpPoint.y = 0;
	}

	ScrollToPosition(tmpPoint);
}

void CDisplayView::Set1v1Mode(BOOL bSwitch)
{
	double dbArrParam[2][3] =
	{
		{
			1.0,
			(double)m_iImageWidth,
			(double)m_iImageHeight
		},
		{ 
			1.0, 
			(double)CAMERA_WIDTH, 
			(double)CAMERA_HEIGHT 
		}
	};

	m_dbZoomRatio = dbArrParam[bSwitch][0];
	SetScaleScroll(dbArrParam[bSwitch][1], dbArrParam[bSwitch][2], dbArrParam[bSwitch][0]);
#ifdef DISPLAY_VER_1
	SetScaledPixelRatio();
#endif
	SetTextZoomRatio();
	ScrollToPosition(CPoint(0, 0));
	Invalidate(FALSE);
	EnableScrollBarCtrl(SB_BOTH, bSwitch);
}

void CDisplayView::SetTextMouseCoordinate(CPoint point)
{
#ifdef DISPLAY_VER_1
	CString strPoint;
	m_curScaledPos.x = (int)((double)m_curMousePos_All.x / m_dbScaledPixelXRatio);
	m_curScaledPos.y = (int)((double)m_curMousePos_All.y / m_dbScaledPixelYRatio);
	strPoint.Format("X:%d, Y:%d", m_curScaledPos.x, m_curScaledPos.y);
#endif

#ifdef DISPLAY_VER_2
	CPoint Relative_Coordinates;
	//Relative_Coordinates.x = m_curMousePos_All.x - (GetScrollPosition().x / (int)m_dbZoomRatio * (int)m_dbZoomRatio);
	//Relative_Coordinates.y = m_curMousePos_All.y - (GetScrollPosition().y / (int)m_dbZoomRatio * (int)m_dbZoomRatio);
	Relative_Coordinates.x = m_curMousePos_All.x - GetScrollPosition().x;
	Relative_Coordinates.y = m_curMousePos_All.y - GetScrollPosition().y;

	CString strPoint;
	m_curScaledPos.x = 
		(int)(m_CapLeftUpper.x + (int)((double)Relative_Coordinates.x * (double)m_CaptureSize.cx / (double)m_PrintViewSize.cx));
	m_curScaledPos.y = 
		(int)(m_CapLeftUpper.y + (int)((double)Relative_Coordinates.y * (double)m_CaptureSize.cy / (double)m_PrintViewSize.cy));
	strPoint.Format("X:%d  Y:%d", m_curScaledPos.x, m_curScaledPos.y);
#endif

	m_ptrCameraSubDlg->GetDlgItem(IDC_STATIC_CAMERA_MOUSE_COORDINATE)->SetWindowTextA(strPoint);
	strPoint.Empty();
}
void CDisplayView::SetTextZoomRatio()
{
	CString strJoomRatio;
	strJoomRatio.Format("Zoom:%d%%", (int)((double)m_ScrollViewSize.cx / (double)CAMERA_WIDTH * (double)100));
	m_ptrCameraSubDlg->GetDlgItem(IDC_STATIC_CAMERA_JOOMRATIO)->SetWindowTextA(strJoomRatio);
	strJoomRatio.Empty();
}

#ifdef DISPLAY_VER_1
void CDisplayView::SetScaledPixelRatio()
{
	m_dbScaledPixelXRatio = (double)m_ScrollViewSize.cx / (double)CAMERA_WIDTH;
	m_dbScaledPixelYRatio = (double)m_ScrollViewSize.cy / (double)CAMERA_HEIGHT;
}
#endif

#ifdef DISPLAY_VER_2
void CDisplayView::CalPrintViewSize(CPoint pt)
{
	//int A = 0, B = 0;
	//
	//A = (pt.x / (int)m_dbZoomRatio) * (int)m_dbZoomRatio;
	//B = ((pt.x + m_iImageWidth) / (int)m_dbZoomRatio + 1) * (int)m_dbZoomRatio;
	//if (B > m_ScrollViewSize.cx) B = m_ScrollViewSize.cx;	
	//m_PrintViewSize.cx = B - A;

	//m_PrintLeftUpper.x = A;

	//A = (pt.y / (int)m_dbZoomRatio) * (int)m_dbZoomRatio;
	//B = ((pt.y + m_iImageHeight) / (int)m_dbZoomRatio + 1) * (int)m_dbZoomRatio;
	//if (B > m_ScrollViewSize.cy) B = m_ScrollViewSize.cy;
	//m_PrintViewSize.cy = B - A;

	//m_PrintLeftUpper.y = A;	

	m_PrintViewSize.cx = m_ScrollViewSize.cx / (int)m_dbZoomRatio;
	m_PrintViewSize.cy = m_ScrollViewSize.cy / (int)m_dbZoomRatio;
	m_PrintLeftUpper.x = pt.x;
	m_PrintLeftUpper.y = pt.y;
}

void CDisplayView::CalCaptureSize(CPoint pt)
{
	//int A = 0, B = 0;

	//A = (pt.x / (int)m_dbZoomRatio) * (int)m_dbZoomRatio * CAMERA_WIDTH / m_ScrollViewSize.cx;
	//B = ((pt.x + m_iImageWidth) / (int)m_dbZoomRatio + 1) * (int)m_dbZoomRatio * CAMERA_WIDTH / m_ScrollViewSize.cx;	
	//if (B > CAMERA_WIDTH) B = CAMERA_WIDTH;
	//m_CaptureSize.cx = B - A;

	//m_CapLeftUpper.x = A;

	//A = ((pt.y / (int)m_dbZoomRatio) * (int)m_dbZoomRatio * CAMERA_HEIGHT) / m_ScrollViewSize.cy;
	//B = ((pt.y + m_iImageHeight) / (int)m_dbZoomRatio + 1) * (int)m_dbZoomRatio * CAMERA_HEIGHT / m_ScrollViewSize.cy;
	//if (B > CAMERA_HEIGHT) B = CAMERA_HEIGHT;
	//m_CaptureSize.cy = B - A;
	 
	//m_CapLeftUpper.y = A;

	m_CaptureSize.cx = CAMERA_WIDTH / (int)m_dbZoomRatio;
	m_CaptureSize.cy = CAMERA_HEIGHT / (int)m_dbZoomRatio;

	m_CapLeftUpper.x = pt.x * CAMERA_WIDTH / m_ScrollViewSize.cx;
	m_CapLeftUpper.y = pt.y * CAMERA_HEIGHT / m_ScrollViewSize.cy;

}
#endif

void CDisplayView::Test()
{
#ifdef GRAYMODE
	m_pImgBuf = (unsigned char*)malloc(sizeof(unsigned char) * CAMERA_WIDTH * CAMERA_HEIGHT);
	if (m_pImgBuf == NULL) return;

	for (int i = 0; i < CAMERA_HEIGHT; i++)
	{
		for (int j = 0; j < CAMERA_WIDTH; j++)
		{
			m_pImgBuf[i * CAMERA_WIDTH + j] = rand() % 256;
		}
	}
#endif
#ifdef COLORMODE
	m_pImgBuf = (unsigned char*)malloc(sizeof(RGBTRIPLE) * CAMERA_WIDTH * CAMERA_HEIGHT);
	if (m_pImgBuf == NULL) return;

	for (int i = 0; i < CAMERA_HEIGHT; i++)
	{
		for (int j = 0; j < CAMERA_WIDTH; j++)
		{
			((RGBTRIPLE*)m_pImgBuf)[i * CAMERA_WIDTH + j].rgbtRed = 255;
			((RGBTRIPLE*)m_pImgBuf)[i * CAMERA_WIDTH + j].rgbtGreen = 255;
			((RGBTRIPLE*)m_pImgBuf)[i * CAMERA_WIDTH + j].rgbtBlue = 255;
		}
	}

	for (int i = 0; i < CAMERA_HEIGHT; i++)
	{
		for (int j = 0; j < (CAMERA_CENTERLINE_THICKNESS >> 1); j++)
		{
			((RGBTRIPLE*)m_pImgBuf)[i * CAMERA_WIDTH + (CAMERA_WIDTH >> 1) + j].rgbtBlue = 0;
			((RGBTRIPLE*)m_pImgBuf)[i * CAMERA_WIDTH + (CAMERA_WIDTH >> 1) + j].rgbtGreen = 0;
			((RGBTRIPLE*)m_pImgBuf)[i * CAMERA_WIDTH + (CAMERA_WIDTH >> 1) + j].rgbtRed = 255;

			((RGBTRIPLE*)m_pImgBuf)[i * CAMERA_WIDTH + (CAMERA_WIDTH >> 1) - (j + 1)].rgbtBlue = 0;
			((RGBTRIPLE*)m_pImgBuf)[i * CAMERA_WIDTH + (CAMERA_WIDTH >> 1) - (j + 1)].rgbtGreen = 0;
			((RGBTRIPLE*)m_pImgBuf)[i * CAMERA_WIDTH + (CAMERA_WIDTH >> 1) - (j + 1)].rgbtRed = 255;
		}
	}

	for (int i = 0; i < CAMERA_WIDTH; i++)
	{
		for (int j = 0; j < (CAMERA_CENTERLINE_THICKNESS >> 1); j++)
		{
			((RGBTRIPLE*)m_pImgBuf)[((CAMERA_HEIGHT >> 1) + j) * CAMERA_WIDTH + i].rgbtBlue = 0;
			((RGBTRIPLE*)m_pImgBuf)[((CAMERA_HEIGHT >> 1) + j) * CAMERA_WIDTH + i].rgbtGreen = 0;
			((RGBTRIPLE*)m_pImgBuf)[((CAMERA_HEIGHT >> 1) + j) * CAMERA_WIDTH + i].rgbtRed = 255;

			((RGBTRIPLE*)m_pImgBuf)[((CAMERA_HEIGHT >> 1) - (j + 1)) * CAMERA_WIDTH + i].rgbtBlue = 0;
			((RGBTRIPLE*)m_pImgBuf)[((CAMERA_HEIGHT >> 1) - (j + 1)) * CAMERA_WIDTH + i].rgbtGreen = 0;
			((RGBTRIPLE*)m_pImgBuf)[((CAMERA_HEIGHT >> 1) - (j + 1)) * CAMERA_WIDTH + i].rgbtRed = 255;
		}
	}
#endif

}