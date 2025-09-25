// CIODlg.cpp: 구현 파일
//
// DIO 테스트를 위한 
// 장치 연결 사용 함수
// short WINAPI nmf_PingCheck(short nNmfNo, short nIpAddr0, short nIpAddr1, short nIpAddr2, int lWaitTime);		//장치의 네트워크 연결상태를 확인합니다. (Ping Check)
// short WINAPI nmf_Connect(short nNmfNo, short nIpAddr0, short nIpAddr1, short nIpAddr2);				//장치와 연결을 수행합니다.
// void WINAPI nmf_Disconnect(short nNmfNo);															// 장치와 연결을 해제합니다.
// 장치 정보 함수
// short WINAPI nmf_GetCompo(short nNmfNo, PTNMFCOMPO ptCompo);											// 장치의 구성(Composition) 정보를 확인합니다.
// short WINAPI nmf_GetBrdAllStatus(short nNmfNo, PTNMFALLSTS ptRetAllSts);								// NMF의 모든 정보를 확인합니다.

// DIO 함수
// short WINAPI nmf_DIGet(short nNmfNo, short *pnInStatus);												// [ DI보드 ]를 통합하여 전체Pin의 입력상태를 확인합니다.
// short WINAPI nmf_DOGet(short nNmfNo, short *pnOutStatus);											// [ DO보드 ]를 통합하여 전체Pin의 출력상태를 확인합니다.

// short WINAPI nmf_DOSet(short nNmfNo, short *pnOutStatus);											// [ DO보드 ]를 통합하여 전체Pin의 출력상태를 설정합니다.

// short WINAPI nmf_DOSetPin(short nNmfNo, short nPinNo, short nOutStatus);								// [ DO보드 ]를 통합하여 전체 Pin중 하나의 출력상태를 설정합니다.
// short WINAPI nmf_DOSetPins(short nNmfNo, short nPinCount, short *pnPinNo, short *pnStatus);			// [ DO보드 ]를 통합하여 출력Pin을 다중으로 선택하고 출력상태를 설정합니다.
// short WINAPI nmf_DOSetTogPin(short nNmfNo, short nPinNo);											// [ DO보드 ]를 통합하여 전체 Pin중 하나의 출력상태를 반전시킵니다.
// short WINAPI nmf_DOSetTogPins(short nNmfNo, short nPinCount, short *pnPinNo);						// [ DO보드 ]를 통합하여 출력Pin을 다중으로 선택하고 출력상태를 반전시킵니다.

// short WINAPI nmf_DOSetLimitTime(short nNmfNo, short nPinNo, short nOutStatus, int lTime);			// [ DO보드 ]를 통합하여 단일 출력Pin의 유지시간 제한기능을 설정하고 출력 On/Off를 합니다.
// short WINAPI nmf_DOGetLimitTime(short nNmfNo, short nPinNo, short *pnSet, short *pnStatus, short *pnOutStatus, int *plRemainTime);
																										// [ DO보드 ]를 통합하여 단일 출력 Pin에 설정된 유지시간 제한기능을 확인합니다.
// short WINAPI nmf_DOBrdSetPin(short nNmfNo, short nID, short nPinNo, short nOutStatus);				// [ DO보드 ]를 지정하고 단일Pin의 출력상태를 설정합니다.
// short WINAPI nmf_DOBrdSetPins(short nNmfNo, short nID, short nPinCount, short *pnPinNo, short *pnOutStatus);
																										// [ DO보드 ]를 지정하고 다중Pin의 출력상태를 설정합니다.
// short WINAPI nmf_DOmBrdSetPins(short nNmfNo, short nBrdCount, short *pnID, int *plOnPins, int *plOffPins);
																										// [ DO보드 ]를 여러개 선택하여, 다중Pin의 출력상태를 설정합니다.
// short WINAPI nmf_DOBrdSetTogPin(short nNmfNo, short nID, short nPinNo);								// [ DO보드 ]를 지정하고 단일Pin의 출력상태를 반전시킵니다.
// short WINAPI nmf_DOBrdSetTogPins(short nNmfNo, short nID, short nPinCount, short *pnPinNo);			// [ DO보드 ]를 지정하고 다중Pin의 출력상태를 반전시킵니다.
// short WINAPI nmf_DOmBrdSetTogPins(short nNmfNo, short nBrdCount, short *pnID, int *plPins);			// [ DO보드 ]를 여러개 선택하여, 다중Pin의 출력상태를 반전시킵니다.

#include "pch.h"
#include "CIODlg.h"
#include "PunchingMachineDlg.h"
#include "CEquipmentDlg.h"
#include "afxdialogex.h"

#ifdef TIMERMODE
#define IOTIMER 100
#endif

//----------------------- List Control Title, Combo Box Item Name-------------------------------

// 장치 구성 관련-----------------
// 장치의 보드 구성 List
TCHAR* pm_sBrdCompoL[] = {
_T("BrdID")
,_T("Type")
,NULL
};

// 장치의 보드 구성
TCHAR* pm_sBrdType[] = {
_T("None")
,_T("DO16")
,_T("DI16")
,_T("AI8")
,_T("AO8")
,_T("AI8AO8")
,_T("DO8")
,_T("DI8")
,_T("DI8DO8")
,NULL
};

//-------------------------------------------------------------------------------------------

// 장치의 구성 요소 순번
enum EBrdCompoElem
{
	bceBrdID,			// 보드 ID
	bceBrdType,			// 보드 타입
	bceCount,
};

//-------------------------------------------------------------------------------------------
#define INDICATOR_CONN_STS	1
#define INDICATOR_ERROR		2

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	INDICATOR_CONN_STS,		// 통신 연결 상태
	INDICATOR_ERROR,		// 오류 반환
};


// CIODlg 대화 상자

IMPLEMENT_DYNAMIC(CIODlg, CDialog)

CIODlg::CIODlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_IO_DIALOG, pParent)
	,m_Radio_OnOff(0)
	, m_nDevNo(-1)
	, m_hThreadId(NULL)
{

}

CIODlg::~CIODlg()
{
}

void CIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DEVICE, m_ListDevice);
	for (int i = 0; i < MAX_DIO_PINS; i++)
	{
		DDX_Control(pDX, IDC_CHK_OUTPUT_0 + i, m_chkOutput[i]);
		DDX_Control(pDX, IDC_BTN_INPUT_0 + i, m_btnInput[i]);
	}
	DDX_Radio(pDX, IDC_RADIO_LTIME_DO_0, m_Radio_OnOff);
}


BEGIN_MESSAGE_MAP(CIODlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CIODlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_LTIME_SET, &CIODlg::OnBnClickedBtnLtimeSet)
	ON_BN_CLICKED(IDC_BTN_LTIME_GET, &CIODlg::OnBnClickedBtnLtimeGet)
	ON_CBN_SELCHANGE(IDC_CB_SEL_FUNC, &CIODlg::OnSelchangeCbSelFunc)
	ON_COMMAND_RANGE(IDC_CHK_OUTPUT_0, IDC_CHK_OUTPUT_127, &CIODlg::OnBnClickedOutput)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CIODlg 메시지 처리기

BOOL CIODlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetClientRect(&m_RectOrgIODlg);
	MoveWindow(m_RectIODlg);
	//UpdateScrollInfo(0, m_RectIODlg.Width(), m_RectIODlg.Height());
	//if (!Init()) return FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CIODlg::OnDestroy()
{
	CDialog::OnDestroy();

}


BOOL CIODlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
#ifdef TIMERMODE
	KillTimer(IOTIMER);
#endif
#ifdef THREADMODE
	if (m_hThreadId)
	{
		m_bThreadRun = TRUE;
		WaitForSingleObject(m_hThreadId, 1000);
		CloseHandle(m_hThreadId);
		m_hThreadId = NULL;
		nmf_Disconnect(m_nDevNo);
	}
#endif

	return CDialog::DestroyWindow();
}


void CIODlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
#ifdef TIMERMODE
	case IOTIMER:
		ReadIO();
		break;

#endif
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CIODlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	UpdateScrollInfo(nType, cx, cy);
}


void CIODlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nNextPos = 0;
	int nDelta = 0;
	SCROLLINFO si = { 0, };

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &si);

	switch (nSBCode)
	{
	case SB_LINEUP:
		nNextPos = si.nPos - 20;
		nDelta = 20;
		if (nNextPos < 0)
		{
			nNextPos = 0;
			nDelta = si.nPos;
		}
		break;
	case SB_LINEDOWN:
		nNextPos = si.nPos + 20;
		nDelta = -20;
		if (nNextPos * si.nPage >= si.nMax)
		{
			if (si.nPos + si.nPage >= si.nMax)
			{
				nDelta = 0;
				nNextPos = si.nPos;
			}
			else
			{
				nDelta = (20 - ((nNextPos + si.nPage) - si.nMax));
				nNextPos = si.nMax - si.nPage;
				nDelta = -nDelta;
			}
		}
		break;
	case SB_PAGEUP:
		nNextPos = si.nPos - si.nPage;
		nDelta = si.nPage;
		if (nNextPos < 0)
		{
			nNextPos = 0;
			nDelta = si.nPos;
		}
		break;
	case SB_PAGEDOWN:
		nNextPos = si.nPos + si.nPage;
		nDelta = si.nPage;
		if (nNextPos + si.nPage >= si.nMax)
		{
			nNextPos = si.nMax - si.nPage;
			nDelta = nNextPos - si.nPos;
		}
		nDelta = -nDelta;
		break;
	case SB_THUMBTRACK:
		nNextPos = nPos;
		nDelta = -(nNextPos - si.nPos);
		break;

	default:
		return;
	}

	m_nScrollPosV = nNextPos;
	SetScrollPos(SB_VERT, nNextPos, TRUE);
	ScrollWindow(0, nDelta);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CIODlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nNextPos = 0;
	int nDelta = 0;
	SCROLLINFO si = { 0, };
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	GetScrollInfo(SB_HORZ, &si);

	switch (nSBCode)
	{
	case SB_LINERIGHT:
		nNextPos = si.nPos + 20;
		nDelta = -20;
		if (nNextPos * si.nPage >= si.nMax)
		{
			if (si.nPos + si.nPage >= si.nMax)
			{
				nDelta = 0;
				nNextPos = si.nPos;
			}
			else
			{
				nDelta = (20 - ((nNextPos + si.nPage) - si.nMax));
				nNextPos = si.nMax - si.nPage;
				nDelta = -nDelta;
			}
		}
		break;

	case SB_LINELEFT:
		nNextPos = si.nPos - 20;
		nDelta = 20;
		if (nNextPos < 0)
		{
			nNextPos = 0;
			nDelta = si.nPos;
		}
		break;
	case SB_PAGERIGHT:
		nNextPos = si.nPos + si.nPage;
		nDelta = si.nPage;
		if (nNextPos + si.nPage >= si.nMax)
		{
			nNextPos = si.nMax - si.nPage;
			nDelta = nNextPos - si.nPos;
		}
		nDelta = -nDelta;
		break;
	case SB_PAGELEFT:
		nNextPos = si.nPos - si.nPage;
		nDelta = si.nPage;
		if (nNextPos < 0)
		{
			nNextPos = 0;
			nDelta = si.nPos;
		}
		break;
	case SB_THUMBTRACK:
		nNextPos = nPos;
		nDelta = -(nNextPos - si.nPos);
		break;

	default:
		return;
	}

	m_nScrollPosH = nNextPos;
	SetScrollPos(SB_HORZ, nNextPos, TRUE);
	ScrollWindow(nDelta, 0);


	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CIODlg::OnBnClickedBtnConnect()
{
	DeviceConnect();
}


void CIODlg::OnBnClickedBtnLtimeSet()
{
	LimitTimeSet();
}


void CIODlg::OnBnClickedBtnLtimeGet()
{
	LimitTimeGet();
}


void CIODlg::OnSelchangeCbSelFunc()
{
	CComboBox* pCombo;		// 콤보 박스 포인터
	CString			str;
	int				nDOPinCount = 0;	// 총 디지털출력 Pin 개수
	int				nDIPinCount = 0;	// 총 디지털입력 Pin 개수

	// 장치 접속 여부 확인(버튼의 Caption으로 확인)
	GetDlgItem(IDC_BTN_CONNECT)->GetWindowText(str);
	if (str != _T("Disconnect")) return;

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_SEL_FUNC);

	m_ePinMode = (EPinMode)pCombo->GetCurSel();		// 기준 모드 설정

	switch (m_ePinMode)
	{
	case epmTotal:
	case epmPin:
		// Pin을 기준으로 일괄 설정하는 경우
		GetDlgItem(IDC_STATIC_DO_TYPE)->SetWindowText(_T("Pin"));
		GetDlgItem(IDC_STATIC_DI_TYPE)->SetWindowText(_T("Pin"));
		for (int i = 0; i < MAX_BRD; i++)
		{
			str.Format(_T("0x%02X"), i * BRD_VS_DIO_PINS);
			GetDlgItem(IDC_STATIC_DOH_0 + i)->SetWindowText(str);
			GetDlgItem(IDC_STATIC_DIH_0 + i)->SetWindowText(str);
			if (m_tCompo.nType[i] == nmf_btDO16) nDOPinCount += 16;
			if (m_tCompo.nType[i] == nmf_btDO8)	 nDOPinCount += 8;
			if (m_tCompo.nType[i] == nmf_btDI16) nDIPinCount += 16;
			if (m_tCompo.nType[i] == nmf_btDI8)	 nDIPinCount += 8;
		}
		// 실제 사용 되는 Pin만 활성화
		// Output
		for (int i = 0; i < nDOPinCount; i++)
		{
			m_chkOutput[i].EnableWindow(true);
		}
		for (int i = nDOPinCount; i < 128; i++)
		{
			m_chkOutput[i].EnableWindow(false);
		}
		// Input
		for (int i = 0; i < nDIPinCount; i++)
		{
			m_btnInput[i].EnableWindow(true);
		}
		for (int i = nDIPinCount; i < 128; i++)
		{
			m_btnInput[i].EnableWindow(false);
		}
		break;
	case epmBrd:
		// 지정 보드의 Pin을 기준으로 설정하는 경우
		GetDlgItem(IDC_STATIC_DO_TYPE)->SetWindowText(_T("Brd"));
		GetDlgItem(IDC_STATIC_DI_TYPE)->SetWindowText(_T("Brd"));
		for (int i = 0; i < MAX_BRD; i++)
		{
			str.Format(_T("ID: %d"), i);
			GetDlgItem(IDC_STATIC_DOH_0 + i)->SetWindowText(str);
			GetDlgItem(IDC_STATIC_DIH_0 + i)->SetWindowText(str);

		}
		// 실제 사용 되는 Pin만 활성화
		for (int i = 0; i < 128; i++)
		{
			m_chkOutput[i].EnableWindow(false);
			m_btnInput[i].EnableWindow(false);
		}

		for (int j = 0; j < MAX_BRD; j++)
		{
			// Output
			if (m_tAllSts.tDO[j].nID != -1)
			{
				if (m_tCompo.nType[j] == nmf_btDO16) nDOPinCount = BRD_VS_DIO_PINS;
				else nDOPinCount = 8;
				for (int i = 0; i < nDOPinCount; i++)
				{
					m_chkOutput[i + j * BRD_VS_DIO_PINS].EnableWindow(true);
				}
			}
			// Input
			if (m_tAllSts.tDI[j].nID != -1)
			{
				if (m_tCompo.nType[j] == nmf_btDI16) nDIPinCount = BRD_VS_DIO_PINS;
				else nDIPinCount = 8;
				for (int i = 0; i < nDIPinCount; i++)
				{
					m_btnInput[i + j * BRD_VS_DIO_PINS].EnableWindow(true);
				}
			}
		}
		break;
	}
	// Off로 초기화 (Pin 기준에서 전환 될 때)
	for (int i = 0; i < MAX_DIO_PINS; i++)
	{
		m_chkOutput[i].SetIcon(AfxGetApp()->LoadIcon(IDI_RED_OFF));
		m_btnInput[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF));
	}


}

void CIODlg::OnBnClickedOutput(UINT nCtrlID)
{
	DO_Output(nCtrlID);
}


/*
	멤버함수
*/


void CIODlg::SetScreenRect(CRect rect)
{
	m_RectIODlg = rect;
}

BOOL CIODlg::Init()
{
	int			iListWidth[bceCount] = { 50,90 };		// Col 너비
	CComboBox* pCombo;		// 콤보 박스 포인터
	CRect		rect;			// 상태창에 쓰이는 

	// Status 바 만들기
	if (!m_StatusDevice.Create(this))
	{
		return FALSE;
	}
	m_StatusDevice.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	this->GetClientRect(&rect);  //get client rect...  
	m_StatusDevice.SetPaneInfo(0, INDICATOR_CONN_STS, SBPS_NORMAL, rect.Width() / 4);
	m_StatusDevice.SetPaneInfo(1, INDICATOR_ERROR, SBPS_NORMAL, rect.Width() / 4 * 3);
	m_StatusDevice.MoveWindow(rect.left, m_RectOrgIODlg.bottom - 20, rect.Width(), 20);

	//초기화
	m_StatusDevice.SetPaneText(0, _T("Disconnected"));

	pCombo = (CComboBox*)GetDlgItem(IDC_CB_SEL_FUNC);
	pCombo->SetCurSel(0);

	m_ePinMode = (EPinMode)pCombo->GetCurSel();

	// IP 초기화
	GetDlgItem(IDC_EDIT_IP_0)->SetWindowText(_T("192"));
	GetDlgItem(IDC_EDIT_IP_1)->SetWindowText(_T("168"));
	GetDlgItem(IDC_EDIT_IP_2)->SetWindowText(_T("0"));
	GetDlgItem(IDC_EDIT_IP_3)->SetWindowText(_T("11"));

	m_ToolTip.Create(this);  //. 생성
	m_ToolTip.AddTool(GetDlgItem(IDC_CB_SEL_FUNC), _T("Output에 사용되는 함수를 선택합니다. DIO 확인은 nmf_DOBrdSetPin 함수 기준으로 보드 기준, 일괄 Pin 기준으로 나뉩니다."));  //. 툴팁추가

	// DIO 버튼 초기화
	for (int i = 0; i < MAX_DIO_PINS; i++)
	{
		m_chkOutput[i].SetIcon(AfxGetApp()->LoadIcon(IDI_RED_OFF));
		m_btnInput[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF));
	}

	m_ListDevice.ListAddColum(pm_sBrdCompoL, iListWidth, bceCount);

	for (int i = 0; i < MAX_BRD; i++)
	{
		m_ListDevice.ListInputData(i, 0, i);
	}

	m_pPunchingMachineDlg = (CPunchingMachineDlg*)AfxGetMainWnd();
	DeviceConnect();
}



void CIODlg::DeviceConnect()
{
	EResult		eRet;
	short		nIP[4];
	CString		str;

	// EDIT에서 IP를 추출
	for (int i = 0; i < 4; i++)
	{
		GetDlgItem(IDC_EDIT_IP_0 + i)->GetWindowText(str);
		nIP[i] = _ttoi(str);
	}

	GetDlgItem(IDC_BTN_CONNECT)->GetWindowText(str);
	if (str == _T("Connect"))
	{
		m_nDevNo = nIP[3];
		eRet = (EResult)nmf_PingCheck(m_nDevNo, nIP[0], nIP[1], nIP[2], 50);	// Ping Check 
		if (eRet != nmf_R_OK)
		{
			AfxMessageBox("IO 연결 실패!", MB_ICONERROR);
			return;
		}

		eRet = (EResult)nmf_Connect(m_nDevNo, nIP[0], nIP[1], nIP[2]);			// 장치 연결
		if (eRet != nmf_R_OK)
		{
			AfxMessageBox("IO 연결 실패!", MB_ICONERROR);
			return;
		}

		eRet = (EResult)nmf_GetCompo(m_nDevNo, &m_tCompo);						// 장치 구성 정보
		if (eRet != nmf_R_OK)
		{
			AfxMessageBox("IO 연결 실패!", MB_ICONERROR);
			return;
		}

		for (int i = 0; i < MAX_BRD; i++)
		{
			m_ListDevice.ListInputData(i, 1, pm_sBrdType[m_tCompo.nType[i]]);
		}
#ifdef TIMERMODE
		SetTimer(IOTIMER, 100, NULL);
#endif
#ifdef THREADMODE
		m_bThreadRun = FALSE;
		m_hThreadId = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadProc, this, 0, NULL);
		if (!m_hThreadId) return;
#endif
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("Disconnect"));
		m_StatusDevice.SetPaneText(0, _T("Connected"));
		OnSelchangeCbSelFunc();
	}
	else
	{
		nmf_Disconnect(m_nDevNo);
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("Connect"));
		m_StatusDevice.SetPaneText(0, _T("Disconnected"));
#ifdef TIMERMODE
		KillTimer(IOTIMER);
#endif
#ifdef THREADMODE
		if (m_hThreadId)
		{
			m_bThreadRun = TRUE;
			WaitForSingleObject(m_hThreadId, 1000);
			CloseHandle(m_hThreadId);
			m_hThreadId = NULL;
		}
#endif
	}
}


//String 16진수를 Data 16진수로 바꿈 
short CIODlg::ConvertToHex(CString data)
{
	WCHAR wcBuf[20] = { 0, };
	short nBuf = 0;
	short nResult = 0;
	int i, j;

	memcpy(wcBuf, data, data.GetLength() * sizeof(TCHAR));
	for (j = 0; j < data.GetLength(); j++)
	{
		i = data.GetLength() - j - 1;
		if (wcBuf[i] >= '0' && wcBuf[i] <= '9')	nBuf = wcBuf[i] - '0';
		if (wcBuf[i] >= 'a' && wcBuf[i] <= 'f')	nBuf = wcBuf[i] - 'a' + 10;
		if (wcBuf[i] >= 'A' && wcBuf[i] <= 'F')	nBuf = wcBuf[i] - 'A' + 10;
		(j == 0) ? nResult += nBuf : nResult += nBuf * j * 16;
	}

	return nResult;
}
//-------------------------------------------------------------------------------------------


// 반환값 상태바에 표시 
void CIODlg::ResponseError(EResult eRet)
{
	CString str;

	switch (eRet)
	{
	case nmf_R_OK:						str = _T("정상");
		break;
	case nmf_R_NOT_CONNECT:			    str.Format(_T("%d, 장치와 연결이 끊어진 경우"), eRet);
		break;
	case nmf_R_SOCKET_ERR:				str.Format(_T("%d, 소켓 에러"), eRet);
		break;
	case nmf_R_UNKOWN:					str.Format(_T("%d, 지원되지 않는 함수 호출"), eRet);
		break;
	case nmf_R_INVALID:					str.Format(_T("%d, 함수 인자값에 오류발생"), eRet);
		break;
	case nmf_R_SYNTAX_ERR:				str.Format(_T("%d, 함수 호출 시, 구문 오류 발생"), eRet);
		break;
	case nmf_R_CONN_ERR:				str.Format(_T("%d, 연결되어 있지 않음"), eRet);
		break;
	case nmf_R_INVALID_NMF:				str.Format(_T("%d, 유효하지 않은 NMF장치번호"), eRet);
		break;
	case nmf_R_FUNC_NOT_ENOUGH:			str.Format(_T("%d, 함수별 지정된 길이만큼 전송/수신하여야 하나 부족한 경우(예:수행 결과로 수신한 응답 데이터가 원하는 길이보다 부족한 경우)"), eRet);
		break;
	case nmf_R_NOT_RESPONSE:			str.Format(_T("%d, 함수 호출 시, 응답이 없는 경우"), eRet);
		break;
	case nmf_R_CMDNO_ERR:				str.Format(_T("%d, 함수 호출 시, 식별코드에 오류 발생"), eRet);
		break;
	case nmf_R_NOT_EXISTS:				str.Format(_T("%d, 네트워크 장치가 식별되지 않는 경우, 방화벽이나 연결 상태를 확인"), eRet);
		break;
	case nmf_R_ICMP_LOAD_FAIL:			str.Format(_T("%d, ICMP.DLL 로드 실패, nmf_PingCheck 호출시 발생. PC에 DLL유무를 확인"), eRet);
		break;
	case nmf_R_FILE_LOAD_FAIL:			str.Format(_T("%d, F/W file 로드 실패"), eRet);
		break;
	case nmf_R_SYNCHRONIZE:				str.Format(_T("%d, 동기화 오류(뮤텍스,세마포어등에서 대기오류)"), eRet);
		break;
	case nmf_R_SOCKET_TX_TMO:			str.Format(_T("%d, 소켓 데이터 전송중 Time-out"), eRet);
		break;
	case nmf_R_SOCKET_RX_TMO:			str.Format(_T("%d, 소켓 데이터 수신중 Time-out"), eRet);
		break;
	case nmf_R_BUF_OVERFLOW:			str.Format(_T("%d, 지정 Buffer크기를 초과(ex.수신버퍼 초과)"), eRet);
		break;
	case nmf_R_MCU_FAIL:				str.Format(_T("%d, MCU동작상의 실패"), eRet);
		break;
	case nmf_R_FUNC_FAIL:				str.Format(_T("%d, 함수수행에 실패한 경우"), eRet);
		break;
	case nmf_R_HW_PART_NONE:			str.Format(_T("%d, 수행하려고 하는 기능의 H/W장치나 구성이 없는경우 예)CPU모델에서 Counter기능 수행"), eRet);
		break;
	case nmf_R_BRD_INVALID:				str.Format(_T("%d, 함수에서 지정한 보드가 없거나 범위를 초과한 경우 예)DO보드가 2개인데 3번 보드에 1Pin에 ON명령 하달시"), eRet);
		break;
	case nmf_R_BRD_MISMATCH_DO:			str.Format(_T("%d, 함수에서 지정한 보드가 없거나 DO가 아닌 경우 예)DI, AO, AI 보드에 DO명령이 전달됨"), eRet);
		break;
	case nmf_R_BRD_MISMATCH_DI:			str.Format(_T("%d, 함수에서 지정한 보드가 없거나 DI가 아닌 경우 예)AO, AI, DO 보드에 DI명령이 전달됨"), eRet);
		break;
	case nmf_R_BRD_MISMATCH_AI:			str.Format(_T("%d, 함수에서 지정한 보드가 없거나 AI가 아닌 경우 예)DO, DI, AO 보드에 AI명령이 전달됨"), eRet);
		break;
	case nmf_R_BRD_MISMATCH_AO:			str.Format(_T("%d, 함수에서 지정한 보드가 없거나 AO가 아닌 경우 예)DO, DI, AI 보드에 AO명령이 전달됨"), eRet);
		break;
	case nmf_R_BRD_INVALID_PIN:			str.Format(_T("%d, 해당 보드의 Pin 범위를 초과"), eRet);
		break;
	case nmf_R_AI_SRATE_ERR:			str.Format(_T("%d, AI 샘플링 적용 시, 샘플링 속도 설정 범위 초과 (0~ 100000)"), eRet);
		break;
	case nmf_R_AI_INVALID_RANGE:		str.Format(_T("%d, AI 입력 전압 Range 설정 범위 초과 (0 ~ 1)"), eRet);
		break;
	case nmf_R_AI_MEM_BLOCK_RANGE_ERR:  str.Format(_T("%d, AI 외부 메모리 설정 시 최대 Block 초과 외부 메모리 참고"), eRet);
		break;
	case nmf_R_AO_RANGE_OVER:           str.Format(_T("%d, AO에서 출력 전압 Range를 넘어선 경우 예) ±5V Range에 6V가 설정 될 경우)"), eRet);
		break;
	default:	break;
	}

	if (eRet > 0)
	{
		str = _T("해당 장치는 NMF가 아닙니다.");
	}
	if (eRet <= nmf_R_ARG_RNG_OVER && eRet > nmf_R_ARG_ERR)
	{
		str.Format(_T("%d, %d번 함수 인자 범위 초과"), eRet, nmf_R_ARG_RNG_OVER - eRet + 1);
	}
	if (eRet <= nmf_R_ARG_ERR && eRet >= nmf_R_ARG_ERR - 500)
	{
		str.Format(_T("%d, %d번 함수 인자 오류"), eRet, nmf_R_ARG_ERR - eRet + 1);
	}

	if (eRet <= nmf_R_FW_ARG_RNG_OVER && eRet > nmf_R_FW_ARG_ERR)
	{
		str.Format(_T("%d, %d번 프로토콜 인자 범위 초과"), eRet, nmf_R_FW_ARG_RNG_OVER - eRet + 1);
	}
	if (eRet <= nmf_R_FW_ARG_ERR && eRet > nmf_R_FW_ARG_ERR - 500)
	{
		str.Format(_T("%d, %d번 프로토콜 인자 오류"), eRet, nmf_R_FW_ARG_ERR - eRet + 1);
	}
	m_StatusDevice.SetPaneText(1, str);
}


void CIODlg::DO_Output(UINT nCtrlID)
{
	// 수동으로 만든 함수
	EResult		eRet;

	CButton* pChkBox;
	short		nPinIndex;
	short		nBrdID;		// 보드 ID
	short		nPinNo;		// Pin 번호
	short		nPinStatus[MAX_DIO_PINS];	// Pin 상태

	nPinIndex = nCtrlID - IDC_CHK_OUTPUT_0;		// 클릭된 CheckBox의 ID에서 Pin 번호 추출

	switch (m_ePinMode)
	{
	case epmTotal:
		// [ DO보드 ]를 정렬하여 전체Pin의 출력상태를 설정합니다.
		for (int i = 0; i < MAX_DIO_PINS; i++)							// 최대 DO 전체에 출력 상태를 설정합니다. (최대 128개)
		{
			pChkBox = (CButton*)GetDlgItem(IDC_CHK_OUTPUT_0 + i);		// 체크 버튼을 인스턴스
			nPinStatus[i] = pChkBox->GetCheck();						// 체크 유무에따라 DO Status 설정(o = Off, 1 = On)
		}
		eRet = (EResult)nmf_DOSet(m_nDevNo, nPinStatus);
		break;

	case epmPin:
		// [ DO보드 ]의 출력Pin을 일괄적으로 정렬하고, 전체 Pin중 하나의 출력상태를 설정합니다.
		nPinNo = nPinIndex;
		pChkBox = (CButton*)GetDlgItem(nCtrlID);		// 체크 버튼을 인스턴스
		nPinStatus[0] = pChkBox->GetCheck();			// 체크 유무에따라 DO Status 설정(o = Off, 1 = On)
		//eRet = (EResult)nmf_DOSetPin(m_nDevNo, nPinNo, nPinStatus[0]);

		// Pin 기준 함수들을 예제로 활용 할 수 있습니다.
		// [ DO보드 ]의 출력Pin을 일괄적으로 정렬하고, 다중으로 선택하고 출력상태를 설정합니다.
		// eRet = (EResult)nmf_DOSetPins(m_nDevNo, short nPinCount, short *pnPinNo, short *pnStatus);

		// [ DO보드 ]의 출력Pin을 일괄적으로 정렬하고, 전체 Pin중 하나의 출력상태를 반전시킵니다.
		// eRet = (EResult)nmf_DOSetTogPin(short nNmfNo, short nPinNo);

		// [ DO보드 ]의 출력Pin을 일괄적으로 정렬하고, 다중으로 선택하고 출력상태를 반전시킵니다.
		// eRet = (EResult)nmf_DOSetTogPins(short nNmfNo, short nPinCount, short *pnPinNo);

	case epmBrd:
		// [ DO보드 ]를 지정하고 단일Pin의 출력상태를 반전시킵니다. 
		nBrdID = nPinIndex / BRD_VS_DIO_PINS;
		nPinNo = nPinIndex % BRD_VS_DIO_PINS;

		pChkBox = (CButton*)GetDlgItem(nCtrlID);		// 체크 버튼을 인스턴스
		nPinStatus[0] = pChkBox->GetCheck();			// 체크 유무에따라 DO Status 설정(o = Off, 1 = On)

		eRet = (EResult)nmf_DOBrdSetPin(m_nDevNo, nBrdID, nPinNo, nPinStatus[0]);

		// 보드 기준 함수들을 예제로 활용 할 수 있습니다.
		// [ DO보드 ]를 지정하고 단일Pin의 출력상태를 설정합니다.
		// eRet = (EResult)nmf_DOBrdSetPins(short nNmfNo, short nID, short nPinCount, short *pnPinNo, short *pnOutStatus);

		// [ DO보드 ]를 여러개 선택하여, 다중Pin의 출력상태를 설정합니다.
		// eRet = (EResult)nmf_DOmBrdSetPins(short nNmfNo, short nBrdCount, short *pnID, int *plOnPins, int *plOffPins);

		// [ DO보드 ]를 지정하고 단일Pin의 출력상태를 반전시킵니다.
		// eRet = (EResult)nmf_DOBrdSetTogPin(short nNmfNo, short nID, short nPinNo);

		// [ DO보드 ]를 지정하고 다중Pin의 출력상태를 반전시킵니다.
		// eRet = (EResult)nmf_DOBrdSetTogPins(short nNmfNo, short nID, short nPinCount, short *pnPinNo);			

		// [ DO보드 ]를 여러개 선택하여, 다중Pin의 출력상태를 반전시킵니다.
		// eRet = (EResult)nmf_DOmBrdSetTogPins(short nNmfNo, short nBrdCount, short *pnID, int *plPins);
		break;
	}
	ResponseError(eRet);
}


void CIODlg::LimitTimeSet()
{
	EResult			eRet;
	CString			str;
	short			nOutputPinNo;					// 출력 제한 시간을 설정 할 핀
	long			lSetTime;						// 출력 제한 설정 시간

	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_LTIME_PIN_NO)->GetWindowText(str);
	nOutputPinNo = ConvertToHex(str);		//String 16진수를 Data 16진수로 변환

	GetDlgItem(IDC_EDIT_SET_LTIME)->GetWindowText(str);
	lSetTime = _ttoi(str);

	eRet = (EResult)nmf_DOSetLimitTime(m_nDevNo, nOutputPinNo, m_Radio_OnOff, lSetTime);
	ResponseError(eRet);
}


void CIODlg::LimitTimeGet()
{
	EResult			eRet;
	CString			str;
	short			nOutputPinNo;				// 출력 제한 시간을 설정 할 핀
	short			nPinSet;					// 출력 제한 시 핀의 출력 상태
	short			nStatus;					// 출력 제한 상태
	short			nOutStatus;					// Pin의 현상태
	long			lRemainTime;				// 출력 제한 남은 시간
	long			lSetTime;					// 출력 제한 설정 시간

	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_LTIME_PIN_NO)->GetWindowText(str);
	nOutputPinNo = ConvertToHex(str);		//String 16진수를 Data 16진수로 변환

	eRet = (EResult)nmf_DOGetLimitTime(m_nDevNo, nOutputPinNo, &nPinSet, &nStatus, &nOutStatus, (int*)&lRemainTime);
	if (eRet != nmf_R_OK) return;


	m_Radio_OnOff = nPinSet;
	UpdateData(FALSE);

	if (nOutStatus == 0)
	{
		str.Format(_T("Off"));
	}
	else
	{
		str.Format(_T("On"));
	}
	GetDlgItem(IDC_STATIC_LTIME_OUTPUT)->SetWindowText(str);
	switch (nStatus)
	{
	case 0:
		str.Format(_T("설정안됨"));
		break;
	case 1:
		str.Format(_T("제한시간 진행 중"));
		break;
	case 2:
		str.Format(_T("제한 시간 종료"));
		break;
	}
	GetDlgItem(IDC_STATIC_LTIME_STATUS)->SetWindowText(str);

	GetDlgItem(IDC_EDIT_SET_LTIME)->GetWindowText(str);
	lSetTime = _ttoi(str);
	str.Format(_T("%d/"), lSetTime - lRemainTime);
	GetDlgItem(IDC_STATIC_CUR_LTIME)->SetWindowText(str);
}


#ifdef THREADMODE
UINT CIODlg::ThreadProc(void* pVoid)
{
	CIODlg* pdlg = (CIODlg*)pVoid;
	CPunchingMachineDlg* pPunchingMachineDlg = (CPunchingMachineDlg*)AfxGetMainWnd();
	while (!pdlg->m_bThreadRun)
	{
		pPunchingMachineDlg->Wait(100);
		pdlg->ReadIO();
	}

	return 0;
}
#endif

void CIODlg::ReadIO()
{
	EResult			eRet;
	short			nPinStatus[MAX_DIO_PINS];		// Pin을 기준으로 일괄 설정하는 경우
	CString			str;

	eRet = (EResult)nmf_GetBrdAllStatus(m_nDevNo, &m_tAllSts);
	if (eRet != nmf_R_OK)
	{
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("Connect"));
		//m_StatusDevice.SetPaneText(0, _T("Disconnected"));
		return;
	}
	else
	{
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("Disconnect"));
		//m_StatusDevice.SetPaneText(0, _T("Connected"));
	}

	switch (m_ePinMode)
	{
	case epmTotal:
	case epmPin:
		// Pin을 기준으로 일괄 설정하는 경우
		// Pin을 일괄적으로 확인하는 함수를 사용하여 상태 표시
		eRet = (EResult)nmf_DOGet(m_nDevNo, nPinStatus);
		memcpy(m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_iArrDO_PinStatus, nPinStatus, sizeof(short) * MAX_DIO_PINS);
		if (IsWindowVisible())
		{
			for (int i = 0; i < MAX_DIO_PINS; i++)
			{
				m_chkOutput[i].SetIcon(AfxGetApp()->LoadIcon(IDI_RED_OFF + nPinStatus[i]));
				m_chkOutput[i].Invalidate(FALSE);
			}
		}

		eRet = (EResult)nmf_DIGet(m_nDevNo, nPinStatus);
		memcpy(m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_iArrDI_PinStatus, nPinStatus, sizeof(short) * MAX_DIO_PINS);
		if (eRet != nmf_R_OK)
		{
			str.Format(_T("Error = %d"), eRet);
			GetDlgItem(IDC_STATIC_ERROR)->SetWindowText(str);
		}
		if (IsWindowVisible())
		{
			for (int i = 0; i < MAX_DIO_PINS; i++)
			{
				m_btnInput[i].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + nPinStatus[i]));
				m_btnInput[i].Invalidate(FALSE);
			}
		}
		if (m_pPunchingMachineDlg->GetPtrEquipmentDlg()->IsWindowVisible())
		{
			m_pPunchingMachineDlg->GetPtrEquipmentDlg()->SetIOImage();
		}
		break;
	case epmBrd:
		if (IsWindowVisible())
		{
			// 지정 보드의 Pin을 기준으로 설정하는 경우
			for (int j = 0; j < MAX_BRD; j++)
			{
				// 보드 ID가 존재 여부 확인
				if (m_tAllSts.tDO[j].nID != -1)
				{
					// 보드의 On, Off 상태 처리
					for (int i = 0; i < BRD_VS_DIO_PINS; i++)
					{
						m_chkOutput[i + j * BRD_VS_DIO_PINS].SetIcon(AfxGetApp()->LoadIcon(IDI_RED_OFF + m_tAllSts.tDO[j].nPinStatus[i]));
						m_chkOutput[i + j * BRD_VS_DIO_PINS].Invalidate(FALSE);
					}
				}
				if (m_tAllSts.tDI[j].nID != -1)
				{
					for (int i = 0; i < BRD_VS_DIO_PINS; i++)
					{
						m_btnInput[i + j * BRD_VS_DIO_PINS].SetIcon(AfxGetApp()->LoadIcon(IDI_GREEN_OFF + m_tAllSts.tDI[j].nPinStatus[i]));
						m_btnInput[i + j * BRD_VS_DIO_PINS].Invalidate(FALSE);
					}
				}
			}
		}
		break;
	default:
		break;
	}
	//ResponseError(eRet);
}


void CIODlg::UpdateScrollInfo(UINT nType, int cx, int cy)
{
	int nScrollMaxV = 0;
	int nScrollMaxH = 0;
	int nTotalHeight = 0;
	int nTotalWidth = 0;
	int nDelta = 0;
	SCROLLINFO siV = { 0, }, siH = { 0, };

	m_nPageSizeV = 0;
	m_nPageSizeH = 0;
	nTotalHeight = m_RectOrgIODlg.Height();
	nTotalWidth = m_RectOrgIODlg.Width();


	if (cy < nTotalHeight)
	{
		nScrollMaxV = nTotalHeight - 1;
		m_nPageSizeV = cy;
		m_nScrollPosV = min(m_nScrollPosV, nTotalHeight - m_nPageSizeV - 1);
	}

	if (cx < nTotalWidth)
	{
		nScrollMaxH = nTotalWidth - 1;
		m_nPageSizeH = cx;
		m_nScrollPosH = min(m_nScrollPosH, nTotalWidth - m_nPageSizeH - 1);
	}

	siV.cbSize = sizeof(SCROLLINFO);
	siV.fMask = SIF_ALL;

	siH.cbSize = sizeof(SCROLLINFO);
	siH.fMask = SIF_ALL;

	GetScrollInfo(SB_VERT, &siV);
	GetScrollInfo(SB_HORZ, &siH);

	if (siV.nPage != 0)
	{
		if (cy > siV.nPage)
		{
			if (siV.nPos + cy > siV.nMax)
			{
				nDelta = siV.nPos + cy - siV.nMax;
				ScrollWindow(0, nDelta);
			}
		}
	}

	siV.nMin = 0;
	siV.nMax = nScrollMaxV;
	siV.nPage = m_nPageSizeV;
	siV.nPos = m_nScrollPosV;
	SetScrollInfo(SB_VERT, &siV, TRUE);

	if (siH.nPage != 0)
	{
		if (cx > siH.nPage)
		{
			if (siH.nPos + cx > siH.nMax)
			{
				nDelta = siH.nPos + cx - siH.nMax;
				ScrollWindow(nDelta, 0);
			}
		}
	}

	siH.nMin = 0;
	siH.nMax = nScrollMaxH;
	siH.nPage = m_nPageSizeH;
	siH.nPos = m_nScrollPosH;
	SetScrollInfo(SB_HORZ, &siH, TRUE);
}