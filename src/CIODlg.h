#pragma once
#include "PunchingMachine.h"
#include "PAIX/NMF.h"
#include "ListCtrlPAIX.h"


//#define TIMERMODE
#define THREADMODE

#define EResult Enmf_FUNC_RESULT

// DO Pin 설정 기준 모드 (보드 기준, Pin 일괄 정렬 기준)
enum EPinMode
{
	epmTotal,
	epmPin,
	epmBrd,
	epmCount,
};


// CIODlg 대화 상자
class CPunchingMachineDlg;
class CIODlg : public CDialog
{
	DECLARE_DYNAMIC(CIODlg)

public:
	CIODlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CIODlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnLtimeSet();
	afx_msg void OnBnClickedBtnLtimeGet();
	afx_msg void OnSelchangeCbSelFunc();
	afx_msg void OnBnClickedOutput(UINT nCtrlID);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();


public:
	// 멤버 변수
	CPunchingMachineDlg* m_pPunchingMachineDlg;
	CRect			m_RectOrgIODlg;
	CRect			m_RectIODlg;
	CListCtrlPAIX	m_ListDevice;					// Device Mount 
	//CBitmapButton	m_chkOutput[MAX_DIO_PINS];		// 최대 보드당 DO 개 만큼 생성
	//CBitmapButton	m_btnInput[MAX_DIO_PINS];		// 최대 보드당 DI 개 만큼 생성
	CMFCButton		m_chkOutput[MAX_DIO_PINS];		// 최대 보드당 DO 개 만큼 생성
	CMFCButton		m_btnInput[MAX_DIO_PINS];		// 최대 보드당 DI 개 만큼 생성
	CStatusBar		m_StatusDevice;					// 상태창
	CToolTipCtrl	m_ToolTip;						// 툴팁 생성
	TNMF_COMPO		m_tCompo;						// 장치의 구성 정보
	short			m_nDevNo;						// Device 번호
	EPinMode		m_ePinMode;						// DO Pin 설정 기준 모드
	TNMF_ALL_STS	m_tAllSts;						// 장치의 전체 상태 정보
	int				m_Radio_OnOff;					// 출력 유지 제한에서 On,Off의 Radio 버튼 값


	// 스크롤바 관련 변수
	int m_nScrollPosH;
	int m_nScrollPosV;
	int m_nPageSizeV;
	int m_nPageSizeH;


#ifdef THREADMODE
	HANDLE			m_hThreadId;
	bool			m_bThreadRun;
#endif


	// 멤버 함수
	BOOL Init();
	void DeviceConnect();
	short ConvertToHex(CString data);			// 10진수 !6진수로
	void ResponseError(EResult eRet);
	void DO_Output(UINT nCtrlID);
	void LimitTimeSet();
	void LimitTimeGet();

#ifdef THREADMODE
	static UINT ThreadProc(void* pVoid);
	void ReadIO();
#endif

public:
	void SetScreenRect(CRect rect);
	void UpdateScrollInfo(UINT nType, int cx, int cy);

};
