// CStatusInformationDlg.cpp: 구현 파일
//

#include "pch.h"
#include "CStatusInformationDlg.h"
#include "PunchingMachineDlg.h"
#include "CEquipmentDlg.h"
#include "CEquipmentDlg.h"
#include "afxdialogex.h"

#define STARTBUTTONCOLOR 0x00DF74
#define STOPBUTTONCOLOR 0x848484
#define OFFBYPASSBUTTONCOLOR 0x0080FF
#define ONBYPASSBUTTONCOLOR 0x848484
#define INITBUTTONONCOLOR 0xD8CEF6
#define INITBUTTONOFFCOLOR 0xAC58FA

// CStatusInformationDlg 대화 상자

IMPLEMENT_DYNAMIC(CStatusInformationDlg, CDialog)

CStatusInformationDlg::CStatusInformationDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_STATUSINFORMATION_DIALOG, pParent)
	, m_pPunchingMachineDlg(NULL)
	, m_bCheckClickedInitBtn(FALSE)
{

}

CStatusInformationDlg::~CStatusInformationDlg()
{
}

void CStatusInformationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_STATUSINFORMATION_STATUS, m_EditStatus);
	DDX_Control(pDX, IDC_BUTTON_STATUSINFORMATION_AUTO, m_btnAuto);
	DDX_Control(pDX, IDC_BUTTON_STATUSINFORMATION_MANUAL, m_btnManual);
	DDX_Control(pDX, IDC_BUTTON_STATUSINFORMATION_INITIALIZE, m_btnInitialize);
	DDX_Control(pDX, IDC_CHECK_STATUSINFORMATION_SKIP_SEARCHMARK, m_Check_Skip_SearchMark);
	DDX_Control(pDX, IDC_CHECK_STATUSINFORMATION_SKIP_PUNCH, m_Check_Skip_Punch);
	DDX_Control(pDX, IDC_CHECK_STATUSINFORMATION_SKIP, m_Check_Skip);
	DDX_Control(pDX, IDC_BUTTON_STATUSINFORMATION_OFFBYPASS, m_btnOffBypass);
	DDX_Control(pDX, IDC_BUTTON_STATUSINFORMATION_ONBYPASS, m_btnOnBypass);
}


BEGIN_MESSAGE_MAP(CStatusInformationDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_STATUSINFORMATION_AUTO, &CStatusInformationDlg::OnBnClickedButtonStatusinformationAuto)
	ON_BN_CLICKED(IDC_BUTTON_STATUSINFORMATION_MANUAL, &CStatusInformationDlg::OnBnClickedButtonStatusinformationManual)
	ON_BN_CLICKED(IDC_BUTTON_STATUSINFORMATION_INITIALIZE, &CStatusInformationDlg::OnBnClickedButtonStatusinformationInitialize)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()

	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_MFCBUTTON_STATUSINFORMATION_STATUSCLEAR, &CStatusInformationDlg::OnBnClickedMfcbuttonStatusinformationStatusclear)
	ON_BN_CLICKED(IDC_CHECK_STATUSINFORMATION_SKIP_SEARCHMARK, &CStatusInformationDlg::OnBnClickedCheckStatusinformationSkipSearchmark)
	ON_BN_CLICKED(IDC_CHECK_STATUSINFORMATION_SKIP_PUNCH, &CStatusInformationDlg::OnBnClickedCheckStatusinformationSkipPunch)
	ON_BN_CLICKED(IDC_BUTTON_STATUSINFORMATION_OFFBYPASS, &CStatusInformationDlg::OnBnClickedButtonStatusinformationOffbypass)
	ON_BN_CLICKED(IDC_BUTTON_STATUSINFORMATION_ONBYPASS, &CStatusInformationDlg::OnBnClickedButtonStatusinformationOnbypass)
	ON_WM_LBUTTONDBLCLK()

END_MESSAGE_MAP()


// CStatusInformationDlg 메시지 처리기
BOOL CStatusInformationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CStatusInformationDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CStatusInformationDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::DestroyWindow();
}

void CStatusInformationDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (ClickControl(IDC_STATIC_STATUSINFORMATION_CURRENT_NUMBEROFPLATES, point))
	{
		if (IDYES == AfxMessageBox("펀칭 수를 초기화 하시겠습니까?", MB_YESNO))
		{
			m_pPunchingMachineDlg->GetPtrEquipmentDlg()->ResetCurrentPunchNumber();
		}

	}

	CDialog::OnLButtonDblClk(nFlags, point);
}


BOOL CStatusInformationDlg::PreTranslateMessage(MSG* pMsg)
{
	CRect rect;

	//메시지가 LBUTTONDOWN일때 실행
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_MFCBUTTON_STATUSINFORMATION_INITIALIZE);

		//해당버튼 버튼변수에 넣고
		pButton->GetWindowRect(rect);

		//버튼위치 가져오기 입니다.
		//위에 구한 버튼위치와 마우스위치가 일치하면 실행
		if (rect.PtInRect(pMsg->pt))
		{
			m_bCheckClickedInitBtn = TRUE;
		}
		
	}
		break;
	case WM_LBUTTONUP:
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_MFCBUTTON_STATUSINFORMATION_INITIALIZE);

		//해당버튼 버튼변수에 넣고
		pButton->GetWindowRect(rect);

		//버튼위치 가져오기 입니다.
		//위에 구한 버튼위치와 마우스위치가 일치하면 실행
		if (rect.PtInRect(pMsg->pt))
		{
			m_bCheckClickedInitBtn = FALSE;
		}
	}
		break;
	default:
		break;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH CStatusInformationDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT nID = pWnd->GetDlgCtrlID();

	switch (nID) {
	case IDC_MFCBUTTON_STATUSINFORMATION_AUTO:
		break;
	case IDC_MFCBUTTON_STATUSINFORMATION_MANUAL:
		break;
	default:
		break;
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}



void CStatusInformationDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDCtl)
	{
	case IDC_BUTTON_STATUSINFORMATION_AUTO:
	{
		CDC dc;
		RECT rect;
		CBrush cbr;
		dc.Attach(lpDrawItemStruct->hDC);
		rect = lpDrawItemStruct->rcItem;
		HPEN h_out_border_pen;

		if (m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_iSystemMode == MODE_AUTO)
		{
			cbr.CreateSolidBrush(STARTBUTTONCOLOR);
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(STARTBUTTONCOLOR);
			dc.SetTextColor(RGB(255, 255, 255));
			h_out_border_pen = ::CreatePen(PS_SOLID, 1, STARTBUTTONCOLOR);
		}
		else
		{
			cbr.CreateSolidBrush(RGB(255, 255, 255));
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(RGB(255, 255, 255));
			dc.SetTextColor(RGB(255, 255, 255));
			h_out_border_pen = ::CreatePen(PS_SOLID, 1, STOPBUTTONCOLOR);
		}

		// 검은색 Pen 객체를 생성한다. (바깥쪽 테두리에 사용)
		//HPEN h_out_border_pen = ::CreatePen(PS_SOLID, 1, STARTBUTTONCOLOR);
		// 사각형 내부를 채우지 않도록 DC에 NULL_BRUSH를 설정한다.
		HGDIOBJ h_out_border_brush = ::SelectObject(dc, ::GetStockObject(NULL_BRUSH));
		// 바깥쪽 테두리를 만들기 위해서 검은색 Pen 객체를 DC에 연결한다.
		HGDIOBJ h_out_pen = ::SelectObject(dc, h_out_border_pen);
		// 내부가 채워지지 않는 사각형을 버튼 크기로 그린다.
		::Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
		// 이전에 사용하던 Pen 객체를 DC에 다시 연결한다.
		::SelectObject(dc, h_out_border_pen);
		// 검은색 Pen 객체를 제거한다.
		::DeleteObject(h_out_border_pen);
		// 테두리 브러시 해제
		::DeleteObject(h_out_border_brush);
		::DeleteObject(h_out_pen);

		// 버튼 컨트롤에서 Caption 정보를 복사한다.
		char name[12];
		int length = ::GetDlgItemText(this->m_hWnd, IDC_BUTTON_STATUSINFORMATION_AUTO, name, 12);
		// 버튼의 Caption 정보를 버튼 영역의 가운데에 출력한다.
		::DrawText(dc, name, length, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		cbr.DeleteObject();
		dc.DeleteDC();
	}
	break;
	case IDC_BUTTON_STATUSINFORMATION_MANUAL:
	{
		CDC dc;
		RECT rect;
		CBrush cbr;
		dc.Attach(lpDrawItemStruct->hDC);
		rect = lpDrawItemStruct->rcItem;

		// 검은색 Pen 객체를 생성한다. (바깥쪽 테두리에 사용)
		HPEN h_out_border_pen;

		if (m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_iSystemMode == MODE_MANUAL)
		{
			cbr.CreateSolidBrush(STOPBUTTONCOLOR);
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(STOPBUTTONCOLOR);
			dc.SetTextColor(RGB(255, 255, 255));
			h_out_border_pen = ::CreatePen(PS_SOLID, 1, STOPBUTTONCOLOR);
		}
		else
		{
			cbr.CreateSolidBrush(RGB(255, 255, 255));
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(RGB(255, 255, 255));
			dc.SetTextColor(RGB(255, 255, 255));
			h_out_border_pen = ::CreatePen(PS_SOLID, 1, STARTBUTTONCOLOR);
		}


		// 검은색 Pen 객체를 생성한다. (바깥쪽 테두리에 사용)
		//HPEN h_out_border_pen = ::CreatePen(PS_SOLID, 1, STARTBUTTONCOLOR);
		// 사각형 내부를 채우지 않도록 DC에 NULL_BRUSH를 설정한다.
		HGDIOBJ h_out_border_brush = ::SelectObject(dc, ::GetStockObject(NULL_BRUSH));
		// 바깥쪽 테두리를 만들기 위해서 검은색 Pen 객체를 DC에 연결한다.
		HGDIOBJ h_out_pen = ::SelectObject(dc, h_out_border_pen);
		// 내부가 채워지지 않는 사각형을 버튼 크기로 그린다.
		::Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
		// 이전에 사용하던 Pen 객체를 DC에 다시 연결한다.
		::SelectObject(dc, h_out_border_pen);
		// 검은색 Pen 객체를 제거한다.
		::DeleteObject(h_out_border_pen);
		// 테두리 브러시 해제
		::DeleteObject(h_out_border_brush);
		::DeleteObject(h_out_pen);

		// 버튼 컨트롤에서 Caption 정보를 복사한다.
		char name[12];
		int length = ::GetDlgItemText(this->m_hWnd, IDC_BUTTON_STATUSINFORMATION_MANUAL, name, 12);
		// 버튼의 Caption 정보를 버튼 영역의 가운데에 출력한다.
		::DrawText(dc, name, length, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		cbr.DeleteObject();
		dc.DeleteDC();
	}
	break;
	case IDC_BUTTON_STATUSINFORMATION_OFFBYPASS:
	{
		CDC dc;
		RECT rect;
		CBrush cbr;
		dc.Attach(lpDrawItemStruct->hDC);
		rect = lpDrawItemStruct->rcItem;
		HPEN h_out_border_pen;
		if (m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_iBypassMode == BYPASS_OFF)
		{
			cbr.CreateSolidBrush(OFFBYPASSBUTTONCOLOR);
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(OFFBYPASSBUTTONCOLOR);
			dc.SetTextColor(RGB(255, 255, 255));
			h_out_border_pen = ::CreatePen(PS_SOLID, 1, OFFBYPASSBUTTONCOLOR);
		}
		else
		{
			cbr.CreateSolidBrush(RGB(255, 255, 255));
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(RGB(255, 255, 255));
			dc.SetTextColor(RGB(255, 255, 255));
			h_out_border_pen = ::CreatePen(PS_SOLID, 1, ONBYPASSBUTTONCOLOR);
		}


		// 검은색 Pen 객체를 생성한다. (바깥쪽 테두리에 사용)
		//HPEN h_out_border_pen = ::CreatePen(PS_SOLID, 1, OFFBYPASSBUTTONCOLOR);
		// 사각형 내부를 채우지 않도록 DC에 NULL_BRUSH를 설정한다.
		HGDIOBJ h_out_border_brush = ::SelectObject(dc, ::GetStockObject(NULL_BRUSH));
		// 바깥쪽 테두리를 만들기 위해서 검은색 Pen 객체를 DC에 연결한다.
		HGDIOBJ h_out_pen = ::SelectObject(dc, h_out_border_pen);
		// 내부가 채워지지 않는 사각형을 버튼 크기로 그린다.
		::Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
		// 이전에 사용하던 Pen 객체를 DC에 다시 연결한다.
		::SelectObject(dc, h_out_border_pen);
		// 검은색 Pen 객체를 제거한다.
		::DeleteObject(h_out_border_pen);
		// 테두리 브러시 해제
		::DeleteObject(h_out_border_brush);
		::DeleteObject(h_out_pen);

		// 버튼 컨트롤에서 Caption 정보를 복사한다.
		char name[12];
		int length = ::GetDlgItemText(this->m_hWnd, IDC_BUTTON_STATUSINFORMATION_OFFBYPASS, name, 12);
		// 버튼의 Caption 정보를 버튼 영역의 가운데에 출력한다.
		::DrawText(dc, name, length, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		cbr.DeleteObject();
		dc.DeleteDC();
	}
		break;

	case IDC_BUTTON_STATUSINFORMATION_ONBYPASS:
	{
		CDC dc;
		RECT rect;
		CBrush cbr;
		dc.Attach(lpDrawItemStruct->hDC);
		rect = lpDrawItemStruct->rcItem;
		HPEN h_out_border_pen;
		if (m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_iBypassMode == BYPASS_ON)
		{
			cbr.CreateSolidBrush(ONBYPASSBUTTONCOLOR);
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(ONBYPASSBUTTONCOLOR);
			dc.SetTextColor(RGB(255, 255, 255));
			h_out_border_pen = ::CreatePen(PS_SOLID, 1, ONBYPASSBUTTONCOLOR);
		}
		else
		{
			cbr.CreateSolidBrush(RGB(255, 255, 255));
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(RGB(255, 255, 255));
			dc.SetTextColor(RGB(255, 255, 255));
			h_out_border_pen = ::CreatePen(PS_SOLID, 1, OFFBYPASSBUTTONCOLOR);
		}


		// 검은색 Pen 객체를 생성한다. (바깥쪽 테두리에 사용)
		//HPEN h_out_border_pen = ::CreatePen(PS_SOLID, 1, OFFBYPASSBUTTONCOLOR);
		// 사각형 내부를 채우지 않도록 DC에 NULL_BRUSH를 설정한다.
		HGDIOBJ h_out_border_brush = ::SelectObject(dc, ::GetStockObject(NULL_BRUSH));
		// 바깥쪽 테두리를 만들기 위해서 검은색 Pen 객체를 DC에 연결한다.
		HGDIOBJ h_out_pen = ::SelectObject(dc, h_out_border_pen);
		// 내부가 채워지지 않는 사각형을 버튼 크기로 그린다.
		::Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
		// 이전에 사용하던 Pen 객체를 DC에 다시 연결한다.
		::SelectObject(dc, h_out_border_pen);
		// 검은색 Pen 객체를 제거한다.
		::DeleteObject(h_out_border_pen);
		// 테두리 브러시 해제
		::DeleteObject(h_out_border_brush);
		::DeleteObject(h_out_pen);

		// 버튼 컨트롤에서 Caption 정보를 복사한다.
		char name[12];
		int length = ::GetDlgItemText(this->m_hWnd, IDC_BUTTON_STATUSINFORMATION_ONBYPASS, name, 12);
		// 버튼의 Caption 정보를 버튼 영역의 가운데에 출력한다.
		::DrawText(dc, name, length, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		cbr.DeleteObject();
		dc.DeleteDC();
	}
		break;
	case IDC_MFCBUTTON_STATUSINFORMATION_INITIALIZE:
	{
		CDC dc;
		RECT rect;
		CBrush cbr;
		dc.Attach(lpDrawItemStruct->hDC);
		rect = lpDrawItemStruct->rcItem;
		HPEN h_out_border_pen;
		// 클릭 된 경우
		if (m_bCheckClickedInitBtn)
		{
			cbr.CreateSolidBrush(INITBUTTONONCOLOR);
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(INITBUTTONONCOLOR);
			dc.SetTextColor(RGB(255, 255, 255));
			h_out_border_pen = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		}
		// 클릭 안 된 경우
		else
		{
			cbr.CreateSolidBrush(INITBUTTONOFFCOLOR);
			::FillRect(dc, &rect, cbr);
			dc.SetBkColor(INITBUTTONOFFCOLOR);
			dc.SetTextColor(RGB(255, 255, 255));
			h_out_border_pen = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		}


		// 검은색 Pen 객체를 생성한다. (바깥쪽 테두리에 사용)
		//HPEN h_out_border_pen = ::CreatePen(PS_SOLID, 1, OFFBYPASSBUTTONCOLOR);
		// 사각형 내부를 채우지 않도록 DC에 NULL_BRUSH를 설정한다.
		HGDIOBJ h_out_border_brush = ::SelectObject(dc, ::GetStockObject(NULL_BRUSH));
		// 바깥쪽 테두리를 만들기 위해서 검은색 Pen 객체를 DC에 연결한다.
		HGDIOBJ h_out_pen = ::SelectObject(dc, h_out_border_pen);
		// 내부가 채워지지 않는 사각형을 버튼 크기로 그린다.
		::Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
		// 이전에 사용하던 Pen 객체를 DC에 다시 연결한다.
		::SelectObject(dc, h_out_border_pen);
		// 검은색 Pen 객체를 제거한다.
		::DeleteObject(h_out_border_pen);
		// 테두리 브러시 해제
		::DeleteObject(h_out_border_brush);
		::DeleteObject(h_out_pen);

		// 버튼 컨트롤에서 Caption 정보를 복사한다.
		char name[12];
		int length = ::GetDlgItemText(this->m_hWnd, IDC_MFCBUTTON_STATUSINFORMATION_INITIALIZE, name, 12);
		// 버튼의 Caption 정보를 버튼 영역의 가운데에 출력한다.
		::DrawText(dc, name, length, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		cbr.DeleteObject();
		dc.DeleteDC();
	}
		break;



	default:
		CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
		break;
	}
}


void CStatusInformationDlg::OnBnClickedButtonStatusinformationAuto()
{
	m_btnAuto.Invalidate(FALSE);
	m_btnManual.Invalidate(FALSE);
	m_pPunchingMachineDlg->GetPtrEquipmentDlg()->AutoMode();
}

void CStatusInformationDlg::OnBnClickedButtonStatusinformationManual()
{
	m_btnAuto.Invalidate(FALSE);
	m_btnManual.Invalidate(FALSE);
	m_pPunchingMachineDlg->GetPtrEquipmentDlg()->ManualMode();
}

void CStatusInformationDlg::OnBnClickedButtonStatusinformationOffbypass()
{
	m_btnOffBypass.Invalidate(FALSE);
	m_btnOnBypass.Invalidate(FALSE);
	m_pPunchingMachineDlg->GetPtrEquipmentDlg()->OffBypass();
}

void CStatusInformationDlg::OnBnClickedButtonStatusinformationOnbypass()
{
	m_btnOffBypass.Invalidate(FALSE);
	m_btnOnBypass.Invalidate(FALSE);
	m_pPunchingMachineDlg->GetPtrEquipmentDlg()->OnBypass();
}

void CStatusInformationDlg::OnBnClickedButtonStatusinformationInitialize()
{
	m_pPunchingMachineDlg->GetPtrEquipmentDlg()->InitMode();
}

void CStatusInformationDlg::OnBnClickedMfcbuttonStatusinformationStatusclear()
{
	m_EditStatus.SetSel(0, -1);
	m_EditStatus.Clear();

	m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_EditStatus.SetSel(0, -1, TRUE);
	m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_EditStatus.Clear();

	//m_EditStatus.SetWindowText("");
	//m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_EditStatus.SetWindowText("");
	//m_EditStatus.Invalidate(FALSE);
	//m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_EditStatus.Invalidate(FALSE);
}

void CStatusInformationDlg::OnBnClickedCheckStatusinformationSkipSearchmark()
{
	m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_Check_Skip_SearchMark.SetCheck(m_Check_Skip_SearchMark.GetCheck());
}


void CStatusInformationDlg::OnBnClickedCheckStatusinformationSkipPunch()
{
	m_pPunchingMachineDlg->GetPtrEquipmentDlg()->m_Check_Skip_Punch.SetCheck(m_Check_Skip_Punch.GetCheck());
}


/*
	멤버함수
*/
void CStatusInformationDlg::Init()
{
	m_fontMode.CreateFont(
		20,                            // 글자높이
		10,                            // 글자너비
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

	m_fontStatus.CreateFont(
		16,                            // 글자높이
		8,                            // 글자너비
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

	//m_btnAuto.EnableWindowsTheming(FALSE);
	//m_btnManual.EnableWindowsTheming(FALSE);
	m_btnAuto.SetFont(&m_fontMode, TRUE);
	m_btnManual.SetFont(&m_fontMode, TRUE);
	m_btnOffBypass.SetFont(&m_fontMode, TRUE);
	m_btnOnBypass.SetFont(&m_fontMode, TRUE);
	m_btnInitialize.SetFont(&m_fontMode, TRUE);
	m_EditStatus.SetFont(&m_fontStatus, TRUE);
	m_Check_Skip_SearchMark.SetFont(&m_fontStatus, TRUE);
	m_Check_Skip_Punch.SetFont(&m_fontStatus, TRUE);
	m_Check_Skip.SetFont(&m_fontStatus, TRUE);
	((CStatic*)GetDlgItem(IDC_STATIC_STATUSINFORMATION_CURRENT_NUMBEROFPLATES))->SetFont(&m_fontStatus, TRUE);
	((CStatic*)GetDlgItem(IDC_STATIC_STATUSINFORMATION_TOTAL_NUMBEROFPLATES))->SetFont(&m_fontStatus, TRUE);
	((CStatic*)GetDlgItem(IDC_EDIT_STATUSINFORMATION_CURRENT_NUMBEROFPLATES))->SetFont(&m_fontStatus, TRUE);
	((CStatic*)GetDlgItem(IDC_EDIT_STATUSINFORMATION_TOTAL_NUMBEROFPLATES))->SetFont(&m_fontStatus, TRUE);

	m_pPunchingMachineDlg = (CPunchingMachineDlg*)AfxGetMainWnd();
}


// 컨트롤 클릭 상태 확인 함수
bool CStatusInformationDlg::ClickControl(int IDC_TARTGET, CPoint pt)
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

