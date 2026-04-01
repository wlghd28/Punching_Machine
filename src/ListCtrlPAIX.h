
#pragma once


// CListCtrlPAIX

#define STATIC_LIST_CTRL		0
#define EDIT_LIST_CTRL			1
#define COMBO_LIST_CTRL			2

#define IDC_EDIT_CHILD	0xFF//에티터 식별자
#define IDC_COMBO_CHILD 0xEE//콤보상자 식별자
#define IDC_CHECK_CHILD 0xAA//체크상자 식별자

#define WM_LISTCTRL_VSCROL (WM_USER + 10)
#define CCMSG_INSERT_CLIPBOARD	WM_USER+11
#define CCMSG_COPY_CLIPBOARD	WM_USER+12
#define CCMSG_PASTE_CLIPBOARD	WM_USER+13
#define CCMSG_CUT_CLIPBOARD		WM_USER+14
#define CCMSG_SELITEM_DELETE	WM_USER+15
#define WM_LISTCTRL_PASTE_END	WM_USER+16

#define WM_LISTCTRL_USER1		WM_USER+17
#define WM_LISTCTRL_USER2		WM_USER+18
#define WM_LISTCTRL_USER3		WM_USER+19

#define COPY_CLIP			0
#define CUT_CLIP			1

typedef enum ControlType{
	ControlNone,
	ControlEdit,
	ControlCombo,
	ControlCheck
};

typedef struct tagLvItemData
{
	tagLvItemData()
	{
		memset(this,0,sizeof(*this));
	}

	COLORREF clrText;
	COLORREF clrBkgnd;
	int nFontEffect;
	int nFontSize;//0:normal
	bool nChkState;
	int nImage;
	int nProgressPercent;

}LVITEMDATA;




class CListCtrlPAIX : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlPAIX)

public:
	CListCtrlPAIX();
	virtual ~CListCtrlPAIX();
private:

	CEdit	**m_pEdit;
	CComboBox **m_pCombo;
	CButton **m_pCheck;
	bool	m_bEdit;
	int	m_nRow,m_nCol;
	CString	m_strValue;
	CFont	*m_font,*m_cb_font;
	bool	m_bStartApp;
	CObArray	*m_pColumnsCtrl;//컬럼컨트럴 배열
	int *m_pCCInfo;//전역 컬럼컨트럴 정보변수
	int m_nCountCtrl[3];//각 컨트럴 수량 정보 배열(0:에디터 1:콤보상자 2:체크상자)

	int *m_pRCInfo;//전역 Row컨트럴 정보변수
	int *m_pComboDesc;
	int *m_pComboNo;
	LPSTR m_pString[1024];
	int m_iStringStartPoint;
	short *m_pnCheck;


protected:
	DECLARE_MESSAGE_MAP()
	CMenu m_menu;//리스트 메뉴 
	CMenu* m_pUmenu;//사용자 정의 메뉴

	afx_msg void OnSelectItemInsert()		{	SelectItemInsert();		}
	afx_msg void OnCopyClipboard()	{	CopyClipboard();	}
	afx_msg void OnPasteClipboard()	{	PasteClipboard();	}
	afx_msg void OnCutClipboard()	{	CutClipboard();		}
	afx_msg void OnSelItemDelete()	{	SelectItemDelete();	}
	afx_msg void OnListUserCMD1()	{	LIstUserCMD1();	}
	afx_msg void OnListUserCMD2()	{	LIstUserCMD2();	}
	afx_msg void OnListUserCMD3()	{	LIstUserCMD3();	}
public:
	int m_iTotalRow;
	BOOL m_bPopMenuEnable;
	BYTE m_ucClipType;
	int m_iMaxItemCount;
	int m_iItemNoStartText;

private:
	// 에디트 컨트럴 그리기
	void DrawEditCtrl(CEdit *pEdit,int nRow, int nCol);//에디트 컨트럴 그리기
	void DrawComboBoxCtrl(CComboBox *pCombo,int nRow,int nCol);//콤보박스컨트럴
	void DrawCheckBoxCtrl(CButton *pCheck,int nRow,int nCol);//체크 박스 컨트럴
	void DrawNoCheckBoxCtrl(short *pnCheck,int nRow,int nCol);//체크박스 없는 컨트럴
	void AddColumnsCtrl(int *nColumns,int nColumnsSize,int nCtrlType);//컬럼에 자식윈도우 컨트럴 배치
	//수동으로 아이템에 체크박스를 그린다.
	void DrawItemCheckBox(int nRow, int nCol, CDC *pDC,CRect &rect,	LVITEMDATA *pItemData);
	//수동으로 아이템에 텍스트를 그린다.
	void DrawItemText(int nRow, int nCol,CDC *pDC,CRect& rect,LVITEMDATA *pItemData);
	int DrawItemImage(int nRow,int nCol,CDC *pDC,CRect &rect,LVITEMDATA *pItemData);
	void DrawItemProgress(int nRow,int nCol,CDC *pDC,CRect &rect,LVITEMDATA *pItemData);
	int FindColumnCtrl(int nCol);
	bool IsBkImage();
	void SetColumnsCtrl(int nCountCtrl[3]);//컬럼컨트럴의 수량 정보로 컨트럴을 만든다.
	void GetItemFont(CFont &pFont,int nFontEffect,int nFontSize);
	
public:
	CWnd *GetColumnsCtrl(int nColumn);
	
	//han
	void ListAddColum(LPSTR strData, int * pWidth, int iColNums);
	void ListAddColum(LPSTR *pstrData, int * pWidth, int iColNums);
	void ListChageColum(LPSTR strData, LPSTR * pstrData, int * pWidth, int iRowNums, int iColNums);
	void ListInputData(int iRow, int iCol, short nData);
	void ListInputData(int iRow, int iCol, int iData);
	void ListInputData(int iRow, int iCol, double dData);
	void ListInputData(int iRow, int iCol, LPSTR lpzData);

	void ComboBoxClear();
	void AddRowComboBoxSet(int iRow, int iCBCount, LPSTR * pstrData);
	
	void PopupMenuSet(BOOL bPopMenu, int UserMenu);
	int GetSubItemCount();

	void SelectItemInsert();
	void CopyClipboard();// Ctrl C (select item copy)
	void PasteClipboard();//Ctrl V (paste)
	void CutClipboard();//Ctrl V (paste)
	void SelectItemDelete();
	void LIstUserCMD1();
	void LIstUserCMD2();
	void LIstUserCMD3();
	//han end
	void SetRowCtrlInfo(int nRCInfo[][2],int nRCInfoSize, int nRCCount, int nRCDesc[]);
	

	void Enter_Refresh(int iValue);

	//사용자로부터 컬럼컨트럴의 정보를 얻는다.
	void SetColumnsCtrlInfo(int nCCInfo[][2],int nCCInfoSize);
	
	int	GetColumnsCnt();
	void SetProgressPos(int nRow,int nCol,int nPos);
	void SetItemColor(int nRow1,int nRow2,int nCol1,int nCol2,COLORREF clrBkgnd,COLORREF clrText);
	int GetProgressPos(int nRow,int nCol);
	void GetItemColor(int nRow,int nCol,COLORREF &clrBkgnd,COLORREF &clrText);
	void SetItemCheck(int nRow,int nCol,int nChkState);//0:no check 1: check
	int GetItemCheck(int nRow,int nCol);
	void SetItemFont(int nRow,int nCol,int nFontEffect, int nFontSize=0);
	void GetItemFont(int nRow,int nCo,int &nFontEffect, int &nFontSize);
private:
	afx_msg void OnEditChild_LostFocus();
	afx_msg void OnEditChild_AfterUpdate();
	afx_msg void OnComboChild_LostFocus();
	void OnCheckChild_LostFocus(HWND hWnd);
	void OnCheckChild_Click(HWND hWnd);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);


public:
	afx_msg BOOL OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	void OnHdnTracking(NMHEADER *pnmhdr);
	afx_msg void OnLvnInsertitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemActivate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
};

inline int CListCtrlPAIX::GetColumnsCnt() {return GetHeaderCtrl()->GetItemCount();}

