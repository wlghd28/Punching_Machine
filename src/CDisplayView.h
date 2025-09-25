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

    // MFC �޽��� ó����
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
    // ���̾�α� �ڵ�
    CameraSubDlg*       m_ptrCameraSubDlg;

    // ��� ����
    double              m_dbZoomRatio;
    int                 m_iImageWidth;
    int                 m_iImageHeight;
    CSize               m_ScrollViewSize;                     // ��ũ�Ѻ� ������
    BITMAPINFO*         m_ptrBitmapInfo;                      // ī�޶� ���� ��Ʈ�� ����
    unsigned char*      m_pImgBuf;
    CPoint              m_curMousePos_All;                    // ��ũ�Ѻ� ��ü���� ���� ���콺 �������� ��ǥ
    CPoint              m_curMousePos_View;                   // ��ũ�Ѻ� �ڽ����� ���� ���콺 �������� ��ǥ
    CPoint              m_curScaledPos;                       // ��ũ�Ѻ� (1280 x 1024 ����) �����ϸ� �� ���콺 �������� ��ǥ
#ifdef DISPLAY_VER_1
    double              m_dbScaledPixelXRatio;                // ���� ���� �̹����� 1�ȼ��� ��ũ�Ѻ��� �ȼ� ���� ����
    double              m_dbScaledPixelYRatio;                // ���� ���� �̹����� 1�ȼ��� ��ũ�Ѻ��� �ȼ� ���� ����
#endif
#ifdef DISPLAY_VER_2
    CSize               m_PrintViewSize;                      // ������ ��µǴ� ������ ������ (StretchDIBits �Լ��� ���� �Ű�����)
    CPoint              m_PrintLeftUpper;                     // ��� ������ �»�� ��ǥ
    CSize               m_CaptureSize;                        // �����̹������� Ȯ����Ѿ��ϴ� ���� ������
    CPoint              m_CapLeftUpper;                       // �����̹������� ĸ���� �κ��� �»�� ��ǥ
#endif

    // ��� �Լ�
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

