// DistDlg.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "DistDlg.h"
#include "PAIX/NMC2.h"


// CDistDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDistDlg, CDialog)

CDistDlg::CDistDlg(int nEquipNo,CWnd* pParent /*=NULL*/)
	: CDialog(CDistDlg::IDD, pParent)
{
	m_nEquipNo = nEquipNo;
}

CDistDlg::CDistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDistDlg::IDD, pParent)
{

}

CDistDlg::~CDistDlg()
{
}

void CDistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDistDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_RPM1, &CDistDlg::OnBnClickedBtnRpm1)
	ON_BN_CLICKED(IDC_BTN_RPM2, &CDistDlg::OnBnClickedBtnRpm2)
	ON_BN_CLICKED(IDC_BTN_RPM3, &CDistDlg::OnBnClickedBtnRpm3)
	ON_BN_CLICKED(IDC_BTN_RPM4, &CDistDlg::OnBnClickedBtnRpm4)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDistDlg 메시지 처리기입니다.

void CDistDlg::OnBnClickedBtnRpm1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	double fRatio;
	

	GetDlgItemText(IDC_EDIT_LEED1,str);

	fRatio = atof(str);
	if( fRatio <= 0 ) fRatio = 1;

	nmc_SetUnitPerPulse(m_nEquipNo, 0,fRatio);
}
void CDistDlg::OnBnClickedBtnRpm2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	double fRatio;
	

	GetDlgItemText(IDC_EDIT_LEED2,str);

	fRatio = atof(str);
	if( fRatio <= 0 ) fRatio = 1;

	nmc_SetUnitPerPulse(m_nEquipNo, 1,fRatio);
}

void CDistDlg::OnBnClickedBtnRpm3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		CString str;
	double fRatio;
	

	GetDlgItemText(IDC_EDIT_LEED3,str);

	fRatio = atof(str);
	if( fRatio <= 0 ) fRatio = 1;

	nmc_SetUnitPerPulse(m_nEquipNo, 2,fRatio);
}

void CDistDlg::OnBnClickedBtnRpm4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		CString str;
	double fRatio;
	

	GetDlgItemText(IDC_EDIT_LEED4,str);

	fRatio = atof(str);
	if( fRatio <= 0 ) fRatio = 1;

	nmc_SetUnitPerPulse(m_nEquipNo, 3,fRatio);
}

void CDistDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	double fRatio;
	CString str;
	if( bShow )
	{
		for( int i =0; i < 4; i ++)
		{
			fRatio = nmc_GetUnitPerPulse(m_nEquipNo, i);
			str.Format("%f",fRatio);
			SetDlgItemText(IDC_EDIT_LEED1 + i,str);

		}
	}
}
