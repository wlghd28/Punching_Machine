#pragma once
#include "PunchingMachine.h"

// CEngineerDlg 대화 상자
class CPunchingMachineDlg;
class CMainDlg;
class CameraSubDlg;
class CCalculateAngle;
class CEngineerDlg : public CDialog
{
	DECLARE_DYNAMIC(CEngineerDlg)

public:
	CEngineerDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CEngineerDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENGINEER_DIALOG };
#endif
public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonEngineerCamera1LoadParameter();
	afx_msg void OnBnClickedButtonEngineerCamera1SaveParameter();
	afx_msg void OnBnClickedButtonEngineerCamera2LoadParameter();
	afx_msg void OnBnClickedButtonEngineerCamera2SaveParameter();
	afx_msg void OnBnClickedCheckEngineerCamera2Mode();
	afx_msg void OnBnClickedCheckEngineerCamera1Mode();
	afx_msg void OnBnClickedButtonEngineerAlignmentSaveParameter();
	afx_msg void OnBnClickedButtonEngineerAlignmentLoadParameter();
	afx_msg void OnBnClickedMfcbuttonEngineerCalculateAngle();
	afx_msg void OnBnClickedCheckEngineerShowconsole();
	DECLARE_MESSAGE_MAP()
public:
	void SaveOpenCVParameter(unsigned int index);
	void LoadOpenCVParameter(unsigned int index);
	void SaveAlignmentParameter();
	void LoadAlignmentParameter();
	static UINT ThreadCalAngle(LPVOID _method);
public:
	CPunchingMachineDlg*		m_pPunchingMachineDlg;
	CMainDlg*					m_pMainDlg;
	CameraSubDlg*				m_pCameraSubDlg;
	CCalculateAngle*			m_pCalculateAngle;
	CWinThread*					m_pThreadCalAngle = NULL;			// CalculateAngle 기능 스레드 변수

	CMFCButton					m_btn_Cancel;
	CButton						m_Check_Engineer_Mode[2];
	CButton						m_Check_ShowConsole;
	int							m_iSearchingAlgorithm;
};
