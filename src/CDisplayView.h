#pragma once

#define MAXJOOMRATIO 50.0
#define MINJOOMRATIO 1.0
#define SCALERATIO 2.0

//#define DISPLAY_VER_1
#define DISPLAY_VER_2

//#define GRAYMODE
#define COLORMODE

class CameraSubDlg;
class CDisplayView : public CScrollView
{
    DECLARE_DYNCREATE(CDisplayView)

public:
    CDisplayView
    (
        int iWidth, 
        int iHeight,
        CameraSubDlg* ptrCameraSubDlg
    );
    ~CDisplayView();

    DECLARE_MESSAGE_MAP()

    // MFC 메시지 처리기
protected:
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    virtual void OnDraw(CDC* /*pDC*/);
    virtual void DoDataExchange(CDataExchange* pDX);
    afx_msg BOOL CDisplayView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll);

public:
    // 다이얼로그 핸들
    CameraSubDlg*       m_ptrCameraSubDlg;

    // 멤버 변수
    double              m_dbZoomRatio;
    int                 m_iImageWidth;
    int                 m_iImageHeight;
    CSize               m_ScrollViewSize;                     // 스크롤뷰 사이즈
    BITMAPINFO*         m_ptrBitmapInfo;                      // 카메라 영상 비트맵 정보
    unsigned char*      m_pImgBuf;
    CPoint              m_curMousePos_All;                    // 스크롤뷰 전체기준 현재 마우스 포인터의 좌표
    CPoint              m_curMousePos_View;                   // 스크롤뷰 박스기준 현재 마우스 포인터의 좌표
    CPoint              m_curScaledPos;                       // 스크롤뷰 (1280 x 1024 기준) 스케일링 된 마우스 포인터의 좌표
#ifdef DISPLAY_VER_1
    double              m_dbScaledPixelXRatio;                // 가로 원본 이미지의 1픽셀당 스크롤뷰의 픽셀 개수 비율
    double              m_dbScaledPixelYRatio;                // 세로 원본 이미지의 1픽셀당 스크롤뷰의 픽셀 개수 비율
#endif
#ifdef DISPLAY_VER_2
    CSize               m_PrintViewSize;                      // 영상이 출력되는 영역의 사이즈 (StretchDIBits 함수에 쓰일 매개변수)
    CPoint              m_PrintLeftUpper;                     // 출력 영역의 좌상귀 좌표
    CSize               m_CaptureSize;                        // 원본이미지에서 확대시켜야하는 영역 사이즈
    CPoint              m_CapLeftUpper;                       // 원본이미지에서 캡쳐할 부분의 좌상귀 좌표
#endif

    // 멤버 함수
    virtual void OnInitialUpdate();
    virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName
        , DWORD dwStyle, const RECT& rect, CWnd* pParentWnd
        , UINT nID, CCreateContext* pContext = NULL);

    void SetPtrImgBuf(unsigned char* pImg);
    void SetScaleScroll(int iWidth, int iHeight, double dbZoomRatio);
    void CalSetScrollPos(double dbZoomRatio, CPoint pt, double dbScaleRatio);
    void Set1v1Mode(BOOL bSwitch);
    void SetTextMouseCoordinate(CPoint point);
    void SetTextZoomRatio();
#ifdef DISPLAY_VER_1
    void SetScaledPixelRatio();
#endif
#ifdef DISPLAY_VER_2
    void CalPrintViewSize(CPoint pt);
    void CalCaptureSize(CPoint pt);
#endif

    void Test();

};

