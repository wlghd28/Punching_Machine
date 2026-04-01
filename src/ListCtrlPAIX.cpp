// ListCtrlPAIX.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "ListCtrlPAIX.h"

// CListCtrlPAIX

IMPLEMENT_DYNAMIC(CListCtrlPAIX, CListCtrl)


CListCtrlPAIX::CListCtrlPAIX()
{
	m_pEdit=NULL;
	m_pCombo=NULL;
	m_bStartApp=true;
	m_cb_font=NULL;m_font=NULL;
	m_pCCInfo=NULL;
	memset(m_nCountCtrl,0,sizeof(m_nCountCtrl));
	m_bPopMenuEnable = FALSE;
	m_ucClipType = COPY_CLIP;
	m_iTotalRow = 0;
	m_iItemNoStartText = 0;
	m_iMaxItemCount = 32;
}

CListCtrlPAIX::~CListCtrlPAIX()
{
	if(m_font){m_font->DeleteObject();delete m_font;}//전역 폰트 삭제
	if(m_cb_font){m_cb_font->DeleteObject();delete m_cb_font;}//전역 콤보폰트 삭제
	if(m_pCCInfo!=NULL) delete m_pCCInfo;//전역 컬럼컨트럴 변수 삭제
}


BEGIN_MESSAGE_MAP(CListCtrlPAIX, CListCtrl)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, &CListCtrlPAIX::OnNMClick)
	ON_CONTROL(EN_KILLFOCUS,IDC_EDIT_CHILD,OnEditChild_LostFocus)
	ON_CONTROL(EN_UPDATE,IDC_EDIT_CHILD,OnEditChild_AfterUpdate)
	ON_CONTROL(CBN_KILLFOCUS,IDC_COMBO_CHILD,OnComboChild_LostFocus)
		
	ON_NOTIFY_REFLECT(LVN_INSERTITEM, &CListCtrlPAIX::OnLvnInsertitem)
	ON_NOTIFY_REFLECT(LVN_ITEMACTIVATE, &CListCtrlPAIX::OnLvnItemActivate)
	ON_WM_CTLCOLOR()
	ON_NOTIFY_REFLECT_EX(NM_CUSTOMDRAW, &CListCtrlPAIX::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, &CListCtrlPAIX::OnLvnDeleteitem)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(CCMSG_INSERT_CLIPBOARD, OnSelectItemInsert)
	ON_COMMAND(CCMSG_COPY_CLIPBOARD, OnCopyClipboard)
	ON_COMMAND(CCMSG_PASTE_CLIPBOARD, OnPasteClipboard)
	ON_COMMAND(CCMSG_CUT_CLIPBOARD, OnCutClipboard)
	ON_COMMAND(CCMSG_SELITEM_DELETE, OnSelItemDelete)
	ON_COMMAND(WM_LISTCTRL_USER1, OnListUserCMD1)
	ON_COMMAND(WM_LISTCTRL_USER2, OnListUserCMD2)
	ON_COMMAND(WM_LISTCTRL_USER3, OnListUserCMD3)

END_MESSAGE_MAP()



// CListCtrlPAIX 메시지 처리기입니다.

void CListCtrlPAIX::ListAddColum(LPSTR strData, int * pWidth, int iColNums)						//List 에 Colum을 추가 함
{

	int i=0;
	int SelectCount = 0;
    CImageList gapImage;

    LV_COLUMN lvcolumn; //리스트박스 설정
	CString str = _T("");

	//PopupMenuSet(TRUE,1);
	SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);  
	ModifyStyle(0, LVS_SHOWSELALWAYS); //리스트 박스에 줄긋자 자세한사항은 MSDN or Naver...

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_RIGHT;//LVCFMT_CENTER; //LVCFMT_LEFT칼럼기준위지.
	lvcolumn.pszText = strData;
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = pWidth[0];
	InsertColumn(0, &lvcolumn);

	for (i=0; i<iColNums; i++)  // 리스트 박스 셋팅
	{	
		str.Format(_T("%d"), i);
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;//LVCFMT_CENTER; //LVCFMT_LEFT칼럼기준위지.
		lvcolumn.pszText = (LPSTR)(LPCSTR)str;
		lvcolumn.iSubItem = i + 1;
		lvcolumn.cx = pWidth[1];
		InsertColumn(i + 1, &lvcolumn);
	}

	gapImage.Create(1,16,ILC_COLORDDB,1,0); //2번째 파라미터로 높이조절.....
	SetImageList(&gapImage,LVSIL_SMALL);
}

void CListCtrlPAIX::ListAddColum(LPSTR *pstrData, int * pWidth, int iColNums)				//List 에 Colum을 추가 함
{
	int i=0;
	int SelectCount = 0;

    CImageList gapImage;

    LV_COLUMN lvcolumn; //리스트박스 설정
	CString str = _T("");

	SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);  
	ModifyStyle(0, LVS_SHOWSELALWAYS); //리스트 박스에 줄긋자 자세한사항은 MSDN or Naver...

	for (i=0; i<iColNums; i++)  // 리스트 박스 셋팅
	{	
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;//LVCFMT_CENTER; //LVCFMT_LEFT칼럼기준위지.
		lvcolumn.pszText = pstrData[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = pWidth[i];
		InsertColumn(i, &lvcolumn);
	}

	gapImage.Create(1,16,ILC_COLORDDB,1,0); //2번째 파라미터로 높이조절.....
	SetImageList(&gapImage,LVSIL_SMALL);
}

void CListCtrlPAIX::ListChageColum(LPSTR strData, LPSTR * pstrData, int * pWidth, int iRowNums, int iColNums)	 //List의 Colum을 변경함
{
	int i=0;
	int SelectCount = 0;

    LV_COLUMN lvcolumn; //리스트박스 설정
	CString str = _T("");
	
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_RIGHT;//LVCFMT_CENTER; //LVCFMT_LEFT칼럼기준위지.
	lvcolumn.pszText = strData;
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = pWidth[0];
	SetColumn(0, &lvcolumn);

	for (i=0; i<iColNums; i++)  // 리스트 박스 셋팅
	{	
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;//LVCFMT_CENTER; //LVCFMT_LEFT칼럼기준위지.
		lvcolumn.pszText = pstrData[i];
		lvcolumn.iSubItem = i + 1;
		lvcolumn.cx = pWidth[1];
		SetColumn(i + 1, &lvcolumn);  
	}
}


void CListCtrlPAIX::ListInputData(int iRow, int iCol, short nData)				//
{
	CString User_Input_ID = _T(""); //리스트 파라미터 변환 임시 문자열
	LV_ITEM lvitem; // 리스트 컨트롤 관련 설정.
	
	User_Input_ID.Format(_T("%d"), nData);
	if (iCol == 0)
	{
		CImageList gapImage; 
		gapImage.Create(1,20,ILC_COLORDDB,1,0); //높이조절.....
		SetImageList(&gapImage,LVSIL_SMALL); 
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iRow;
		lvitem.iSubItem = iCol;
		lvitem.pszText = (LPSTR)(LPCSTR)User_Input_ID;
		InsertItem(&lvitem);
	}
	else
	{
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iRow;
		lvitem.iSubItem = iCol;
		lvitem.pszText = (LPSTR)(LPCSTR)User_Input_ID;
		SetItem(&lvitem);
	}
}

void CListCtrlPAIX::ListInputData(int iRow, int iCol, int iData)
{
	CString User_Input_ID = _T(""); //리스트 파라미터 변환 임시 문자열
	LV_ITEM lvitem; // 리스트 컨트롤 관련 설정.
	
	User_Input_ID.Format(_T("%d"), iData);
	if (iCol == 0)
	{
		CImageList gapImage; 
		gapImage.Create(1,20,ILC_COLORDDB,1,0); //높이조절.....
		SetImageList(&gapImage,LVSIL_SMALL); 
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iRow;
		lvitem.iSubItem = iCol;
		lvitem.pszText = (LPSTR)(LPCSTR)User_Input_ID;
		InsertItem(&lvitem);
	}
	else
	{
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iRow;
		lvitem.iSubItem = iCol;
		lvitem.pszText = (LPSTR)(LPCSTR)User_Input_ID;
		SetItem(&lvitem);
	}
}

void CListCtrlPAIX::ListInputData(int iRow, int iCol, double dData)
{
	CString User_Input_ID = _T(""); //리스트 파라미터 변환 임시 문자열
	LV_ITEM lvitem; // 리스트 컨트롤 관련 설정.
	User_Input_ID.Format(_T("%0.3f"), dData);
	if (iCol == 0)
	{
		CImageList gapImage; 
		gapImage.Create(1,20,ILC_COLORDDB,1,0); //높이조절.....
		SetImageList(&gapImage,LVSIL_SMALL);
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iRow;
		lvitem.iSubItem = iCol;
		lvitem.pszText = (LPSTR)(LPCSTR)User_Input_ID;
		InsertItem(&lvitem);
	}
	else
	{
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iRow;
		lvitem.iSubItem = iCol;
		lvitem.pszText = (LPSTR)(LPCSTR)User_Input_ID;
		SetItem(&lvitem);
	}
}

void CListCtrlPAIX::ListInputData(int iRow, int iCol, LPSTR lpzData)
{
	CString User_Input_ID = _T(""); //리스트 파라미터 변환 임시 문자열
	LV_ITEM lvitem; // 리스트 컨트롤 관련 설정.
	
	if (iCol == 0)
	{
		CImageList gapImage; 
		gapImage.Create(1,20,ILC_COLORDDB,1,0); //높이조절.....
		SetImageList(&gapImage,LVSIL_SMALL); 

		lvitem.mask = LVIF_TEXT;
		lvitem.iItem =iRow;
		lvitem.iSubItem =0 ;
		lvitem.pszText = lpzData;
		InsertItem(&lvitem);
	}
	else
	{
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem =iRow;
		lvitem.iSubItem =iCol ;
		lvitem.pszText = lpzData;
		SetItem(&lvitem);
	}
}

void CListCtrlPAIX::Enter_Refresh(int iValue)						//End
{
	//if ((iValue > 0 && m_iListDataNum < m_cbPositionNumbers.GetCurSel()) || (iValue < 0 && m_iListDataNum > 0))
	//{
		//ListAddData();
		m_nRow += iValue;
		SetItemState (m_nRow, 0, LVIS_SELECTED|LVIS_FOCUSED);
		SetItemState (m_nRow, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		EnsureVisible(m_nRow, false);	//해당 라인으로 스크롤 이동
	//}
}

BOOL CListCtrlPAIX::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	if(!m_pCCInfo) return false;//사용자가 컬럼정보를 입력했는지 검사
	LPNMLVCUSTOMDRAW pnmlvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	*pResult = 0;
	

	switch(pnmlvcd->nmcd.dwDrawStage)//사용자 그리기 신호 구분
	{
	case CDDS_PREPAINT://컨트럴을 그리기전
		*pResult = CDRF_NOTIFYITEMDRAW;//모든 아이템을 시스템이 그린다.
		break;
	case CDDS_ITEMPREPAINT://아이템을 그리기전
			
		*pResult = CDRF_NOTIFYSUBITEMDRAW;	//모든 서브아이템을 시스템이 그린다.
		break;

	case CDDS_ITEMPREPAINT | CDDS_SUBITEM://아이템과 서브아이템을 그리기전(리포터 모드시에만)
		{
			

			LVITEMDATA *pItemData=(LVITEMDATA*)pnmlvcd->nmcd.lItemlParam;
			int nRow=static_cast <int>(pnmlvcd->nmcd.dwItemSpec);
			int nCol=pnmlvcd->iSubItem;

			
			CDC* pDC = pDC->FromHandle(pnmlvcd->nmcd.hdc);//HDC->CDC
			CRect rect;
			GetSubItemRect(nRow, nCol, LVIR_BOUNDS, rect);
			//커스텀드로우 구조체에 글자와 배경 색상 정보를 아이템데이타 구조체 정보와 일치시킨다.
			pnmlvcd->clrTextBk=pItemData[nCol].clrBkgnd;
			pnmlvcd->clrText=pItemData[nCol].clrText;

			//각 컬럼에 지정되어 있는 컨트럴의 종류를 파악한다.
			int nColumnCtrl=FindColumnCtrl(nCol);
			
			if(nCol==0)//0번 컬럼은 자동으로 그린다.
			{
				//서브아이템을 그릴때 시스템이 자동으로 그린다

				*pResult = CDRF_NOTIFYSUBITEMDRAW;	return false;	
			}
			switch(nColumnCtrl)//각 컬럼별 아이템구분
			{
			case 3://체크상자 그리기
				{
					DrawItemCheckBox(nRow,nCol,pDC,rect,pItemData);
					*pResult = CDRF_SKIPDEFAULT;//사용자가 직접그리는 신호
				}
				break;
			case 4://진행막대 그리기
				{
					DrawItemProgress(nRow,nCol,pDC,rect,pItemData);
					*pResult = CDRF_SKIPDEFAULT;
				}
				break;
			case 5://아이콘을 포함한 글자
				{
					rect.left+=DrawItemImage(nRow,nCol,pDC,rect,pItemData);
					DrawItemText(nRow,nCol,pDC,rect,pItemData);
					*pResult = CDRF_SKIPDEFAULT;
				}
				break;
			default://글자만 그리기
				{
					DrawItemText(nRow,nCol,pDC,rect,pItemData);
					*pResult = CDRF_SKIPDEFAULT;
				}
				break;
			}
		}
	}
	return true;
}

BOOL CListCtrlPAIX::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_pCCInfo) return false;//컬럼에 사용자 정보가 없을때는 기본값 수행
	NM_LISTVIEW *pNotifyStruct=(NM_LISTVIEW *)pNMHDR;
	
	int nRow=pNotifyStruct->iItem;
	int nCol=pNotifyStruct->iSubItem;
	int nColumnsCtrl;//컬럼 컨트럴의 종류
	
	if(nRow==-1 || nCol==0) return false;
	nColumnsCtrl=m_pCCInfo[nCol];//현재 컬럼에 설정되어 있는 컨트럴을 파악한다.
	GetHeaderCtrl()->SetFocus();

	if(nColumnsCtrl==2)//2이면 콤보박스
		DrawComboBoxCtrl((CComboBox*)GetColumnsCtrl(nCol),nRow,nCol);
	else if(nColumnsCtrl==1)//1 이면 에디터
		DrawEditCtrl((CEdit *)GetColumnsCtrl(nCol),nRow,nCol);
	else if(nColumnsCtrl==3)//3이면 체크
		DrawCheckBoxCtrl((CButton*)GetColumnsCtrl(nCol),nRow,nCol);
	
	
	*pResult = 0;
	return true;
}

// 에디트 컨트럴 그리기
void CListCtrlPAIX::DrawEditCtrl(CEdit *pEdit,int nRow, int nCol)
{
	m_bEdit=false;//에디트 상자의 상태(보통)를 설정
	if ((nRow < 0) || nRow >= GetItemCount()/*GetCountPerPage()*/)	return;

	if ((nCol < 0) || nCol >= GetHeaderCtrl()->GetItemCount())	return;

	// Make sure that nSubItem is valid

	if (GetColumnWidth(nCol) < 5) return;

	DWORD dwExStyle = GetExtendedStyle();
	if ((dwExStyle & LVS_EX_FULLROWSELECT) == 0) return;
	

	// make sure that the item is visible
	if (!EnsureVisible(nRow, TRUE)) return;

	
	CRect rect;
	GetSubItemRect(nRow,nCol, LVIR_BOUNDS, rect);

	rect.top += 1;//그리드의 선 보다 아래쪽에
	rect.bottom -= 1;//그리드의 선 보다 윗쪽에
	

	CString strValue = GetItemText(nRow, nCol);
	m_strValue=strValue;
	m_nRow=nRow;m_nCol=nCol;//전역 선택 아이템
	
	pEdit->MoveWindow(rect);
	pEdit->ShowWindow(SW_SHOW);
	
	pEdit->SetFocus();pEdit->SetWindowText(strValue);
	m_bEdit=true;//에디트 상자의 상태(편집)
		
}

void CListCtrlPAIX::OnEditChild_LostFocus()
{
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	CString str = _T("");
	CEdit * pEdit=(CEdit*)GetColumnsCtrl(m_nCol);
	//포커스를 잃으면 에디터를 숨긴다.
	if (pEdit && ::IsWindow(pEdit->m_hWnd))
	{
		pEdit->GetWindowText(str);
		pEdit->ShowWindow(SW_HIDE);		
	}
	
	// set new string in subitem
	SetItemText(m_nRow, m_nCol, str);
	
}

void CListCtrlPAIX::OnEditChild_AfterUpdate()
{
	if(m_bEdit)
	::SendMessage(GetParent()->m_hWnd,WM_USER+1,0,0);
}

void CListCtrlPAIX::DrawComboBoxCtrl(CComboBox *pCombo,int nRow,int nCol)
{
	
	// Make sure that nSubItem is valid
	int i;
	int iStartCount = 0;
	int iCurValue = 0;
	if (GetColumnWidth(nCol) < 5)
		return;

	DWORD dwExStyle = GetExtendedStyle();
	if ((dwExStyle & LVS_EX_FULLROWSELECT) == 0) return;

	
	CRect rect;
	GetSubItemRect(nRow, nCol, LVIR_BOUNDS, rect);
	
	rect.top+=0;rect.left += 1;rect.bottom-=6;
	
	CString	strValue=GetItemText(nRow,nCol);
	m_strValue=strValue;
	m_nRow=nRow;m_nCol=nCol;//전역현재위치 저장 반드시 컨트럴이 보여지기 전에 처리한다. 

	while(pCombo->GetCount())
	{
		pCombo->DeleteString(0);
	}
	i = 0;
	iStartCount = m_pComboNo[m_pComboDesc[nCol]];
	while(m_pString[i + iStartCount] != NULL)
	{
		pCombo->AddString(m_pString[i + iStartCount]);
		if (strValue == m_pString[i + iStartCount])
		{
			iCurValue = i;
		}
		i++;
	}
	pCombo->MoveWindow(rect);
	pCombo->ShowWindow(SW_SHOW);

	
	//콤보상자 자체의 크기를 조정하는 방법 ^^;;
	pCombo->SetItemHeight(-1,rect.Height());//높이 조정 콤보박스 에디터
	//::SendMessage(m_pCombo->m_hWnd,CB_SETITEMHEIGHT, (WPARAM) -1, (LPARAM)rect.Height());
	pCombo->ModifyStyle(CBS_DROPDOWN, CBS_DROPDOWNLIST,SWP_FRAMECHANGED);
	pCombo->SetCurSel(iCurValue);
	pCombo->SetFocus();
	m_bEdit=true;//에디트 상자의 상태(편집)
	
	
}

void CListCtrlPAIX::OnComboChild_LostFocus()
{
	CString str = _T("");
	CComboBox *pCombo=(CComboBox*)GetColumnsCtrl(m_nCol);
	if (pCombo && ::IsWindow(pCombo->m_hWnd))
	{
		pCombo->GetWindowText(str);
		pCombo->ShowWindow(SW_HIDE);
	}

	// set new string in subitem
	SetItemText(m_nRow, m_nCol, str);


}
//체크상자 컨트럴을 다시그린다.
void CListCtrlPAIX::DrawCheckBoxCtrl(CButton *pCheck, int nRow, int nCol)
{
	// Make sure that nSubItem is valid
	
	if (GetColumnWidth(nCol) < 5)
		return;

	DWORD dwExStyle = GetExtendedStyle();
	if ((dwExStyle & LVS_EX_FULLROWSELECT) == 0) return;

	CRect rect;
	GetSubItemRect(nRow, nCol, LVIR_BOUNDS, rect);
	
	rect.left+=1;
	rect.top+=1;rect.bottom-=1;
	
	LVITEMDATA *pItemData=(LVITEMDATA*)GetItemData(nRow);
	CString str=GetItemText(nRow,nCol);
	m_nRow=nRow;m_nCol=nCol;//현재위치 저장 반드시 컨트럴이 보여지기 전에 저장한다.
	
	
	pCheck->MoveWindow(rect);
	pCheck->SetWindowText(str);
	pCheck->ShowWindow(SW_SHOW);
	
	pCheck->SetFocus();
	if(pItemData[nCol].nChkState)
	{
		pCheck->SetCheck(BST_CHECKED);
	}
	else
	{
		pCheck->SetCheck(BST_UNCHECKED);
	}
	m_bEdit=true;//에디트 상자의 상태(편집)
	
	

}

void CListCtrlPAIX::OnCheckChild_LostFocus(HWND hWnd)
{
	CString str = _T("");
	CWnd *pWnd=pWnd->FromHandle(hWnd);//넘겨 받은 핸들을 CWnd로 바꾼다
	CButton *pCheck=(CButton*)pWnd;//CWnd를 체크버튼으로 바꾼다
	pCheck->GetWindowText(str);//체크버튼의 글자를 읽는다.
	pCheck->ShowWindow(SW_HIDE);
	// set new string in subitem
	SetItemText(m_nRow, m_nCol, str);

}
//체크버튼을 글릭했다
void CListCtrlPAIX::OnCheckChild_Click(HWND hWnd)
{
	CWnd *pWnd=pWnd->FromHandle(hWnd);
	CButton *pCheck=(CButton*)pWnd;
	LVITEMDATA *pItemData=(LVITEMDATA*)GetItemData(m_nRow);//해당아이템의 체크상태를 읽는다.
	if(BST_UNCHECKED==pCheck->GetCheck())
	{
		pItemData[m_nCol].nChkState=0;//체크상태를 아이템데이타에 저장한다.

	}
	else if(BST_CHECKED==pCheck->GetCheck())
	{
		pItemData[m_nCol].nChkState=1;
	}
	
	
}


//헤더 컨트럴의 경계선을 끌경우 컨트럴의 포커스를 제거한다.
void CListCtrlPAIX::OnHdnTracking(NMHEADER *pnmhdr)
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!GetItemCount()) return;//아이템이 없으면 그냥 끝낸다.
	CWnd *pWnd=pWnd->FromHandle(pnmhdr->hdr.hwndFrom);
	pWnd->SetFocus();	
}

//아이템이 추가될때
void CListCtrlPAIX::OnLvnInsertitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_pCCInfo) return;//사용자가 컬럼 컨트럴 정보를 입력했는지 검사 

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_bStartApp)//리스트 컨트럴이 처음 만들어 졌을때만 동작한다.
	{
		
		/*//로그폰트 구조체 동적생성
		LOGFONT *plf=(LOGFONT*)malloc(sizeof(LOGFONT));
		memset(plf,0,sizeof(LOGFONT));
		GetFont()->GetLogFont(plf);//사용자 설정 폰트를 얻는다.
		if(plf->lfHeight<=0)
		{//기본 폰트일 경우에는 폰트를 만든다.
			plf->lfHeight=15;plf->lfWidth=0;
			_tcscpy_s(plf->lfFaceName,10,_T("MS Shell Dlg"));
			m_font=new CFont;
			m_font->CreateFontIndirect(plf);SetFont(m_font);
		}
		delete plf;//로그폰트 구조체 제거
		*/
		//컬럼수만큼 개체배열할당
		m_pColumnsCtrl=new CObArray;
		m_pColumnsCtrl->SetSize(GetColumnsCnt());
		//읽어들인 정보로 컬럼에 컨트럴을 배치한다.
		SetColumnsCtrl(m_nCountCtrl);
						
		m_bStartApp=false;
	}
	//아이템데이타를 메모리에 탑재한후 리스트 컨트럴 각 아이템에 저장한다.
	LVITEMDATA *pItemData=new LVITEMDATA[GetColumnsCnt()];
	for(int i=0;i<GetColumnsCnt();i++)
	{
		pItemData[i].clrBkgnd=GetBkColor();
		pItemData[i].clrText=GetTextColor();
	}
	SetItemData(pNMLV->iItem,(DWORD)pItemData);
}


void CListCtrlPAIX::OnLvnItemActivate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
//컬럼의 컨트럴 포인터를 반환한다.
CWnd * CListCtrlPAIX::GetColumnsCtrl(int nColumn)
{
	return (CWnd*)m_pColumnsCtrl->GetAt(nColumn);
}

//컬럼에 컨트럴 실제로 만든다.

void CListCtrlPAIX::AddColumnsCtrl(int *nColumns,int nColumnsSize,int nCtrlType)

{
	int nCount;
	CRect rect(0,0,0,0);
	nCount=nColumnsSize/sizeof(int);
	//전역 콤보폰트가 만들어 지지 않았으면 만든다.
	if(m_cb_font==NULL)
	{
		//로그폰트 구조체를 동적으로 생성	
		LOGFONT *plf=(LOGFONT*)malloc(sizeof(LOGFONT));
		memset(plf,0,sizeof(LOGFONT));
		GetFont()->GetLogFont(plf);	plf->lfHeight-=3;//읽어들인 폰트 크기에서 3만큼 빼고
		
		m_cb_font=new CFont;
		m_cb_font->CreateFontIndirect(plf);delete plf;	//폰트를 만든후 구조체를 지운다.
	}
	//컨트럴 타입별로 사용자가 지정한 수만큼 컨트럴 배열 포인터를 만든다.
	if(nCtrlType==1) m_pEdit=new CEdit *[nCount];//에디터 배열 포인터
	else if(nCtrlType==2) m_pCombo=m_pCombo=new CComboBox*[nCount];//콤보박스 배열 포인터
	else if(nCtrlType==3) m_pCheck=new CButton *[nCount];//체크버튼 배열 포인터
	

	for(int i=0;i<nCount;i++)//각 컨트럴의 수만크 반복한다.
	{
		switch(nCtrlType)
		{
		case 1 ://에디터
			{
				m_pEdit[i]=new CEdit;
				m_pEdit[i]->Create(WS_CHILD| ES_RIGHT, rect, this,IDC_EDIT_CHILD);
				
				m_pEdit[i]->SetFont(GetFont());
				m_pColumnsCtrl->SetAt(nColumns[i],m_pEdit[i]);
				break;
			}
			
		case 2://콤보상자
			{
				m_pCombo[i]=new CComboBox;
				m_pCombo[i]->Create(WS_CHILD | CBS_DROPDOWNLIST,rect,this,IDC_COMBO_CHILD);
				
				m_pCombo[i]->SetFont(GetFont()); //SetFont(m_cb_font);
				m_pColumnsCtrl->SetAt(nColumns[i],m_pCombo[i]);
				break;
			}
		case 3://체크버튼(BS_NOTIFY를 사용해야 부모가 자식의 메시지를 받는다)
			{
				m_pCheck[i]=new CButton;
				m_pCheck[i]->Create(NULL,WS_CHILD | BS_PUSHBUTTON |BS_AUTOCHECKBOX |BS_NOTIFY,
					rect,this,IDC_CHECK_CHILD);
				
				m_pCheck[i]->SetFont(GetFont());
				
				m_pColumnsCtrl->SetAt(nColumns[i],m_pCheck[i]);
				break;
			}
		}//switch 닫음
	}//for닫음
}
//사용자가 지정한 각 컬럼별 정보로 실제 컨트럴 그리는 함수를 호출한다.
void CListCtrlPAIX::SetColumnsCtrl(int nCountCtrl[3])
{
	//읽어들인 정보에서 사용자가 지정한 컨트럴을 각컬럼에 배치한다.
	int i,j,k;
	int sum=0;
	int *pColumns=NULL;
	for(j=0;j<3;j++) //컨트럴 배열의 종류 갯수만큼 루프를 돌린다.
	{
		if(!nCountCtrl[j]) continue;//컨트럴수가 0이면 통과한다.
		sum=nCountCtrl[j];
		i=0;
		if(pColumns) {delete pColumns;pColumns=NULL;}//컬럼 포인터가 메모리에 할당되어 있으면 지운다.
		pColumns=new int[sum];//컨트럴갯수 만큼 포인터에 메모리 할당
		for(k=0;k<GetColumnsCnt();k++)//컬럼의 수만큼 루프를 돌린다.
		{
				if(m_pCCInfo[k]==j+1)//배열중 인덱스 추출(1=edit2=combo3=check)
				{
						pColumns[i]=k;//콤보상자 배열에 인덱스를 값으로 준다.
						if(i<sum-1)
							i++;
						else
						{//만들어진 컬럼정보 배열을 인자로 컨트럴 생성함수 호출
							AddColumnsCtrl(pColumns,sizeof(int)*sum,j+1);
							break;
						}//else 닫음
						
				}//if(m_pCCInfo[k]==j+1) 닫음
		}//for(k) 닫음
	}//for(j) 닫음
}
//지정된 컬럼에 설정되어 있는 컨트럴의 종류를 알아낸다
int CListCtrlPAIX::FindColumnCtrl(int nCol)
{
	for(int i=0;i<GetColumnsCnt();i++)
	{
		if(i==nCol)
			return m_pCCInfo[i];
	}
	return -1;
}

//사용자그리기 아이템텍스트
void CListCtrlPAIX::DrawItemText(int nRow, int nCol, CDC *pDC,CRect &rect,
									LVITEMDATA *pItemData)
{

	if (rect.IsRectEmpty())
	{
		return;
	}

	if(!IsBkImage())pDC->FillSolidRect(&rect,pItemData[nCol].clrBkgnd);

	CString str;
	str = GetItemText(nRow, nCol);

	if (!str.IsEmpty())
	{
		// get text justification
		HDITEM hditem;
		hditem.mask = HDI_FORMAT;
		GetHeaderCtrl()->GetItem(nCol, &hditem);
		int nFmt = hditem.fmt & HDF_JUSTIFYMASK;
		UINT nFormat = DT_VCENTER | DT_SINGLELINE;
		
		if (nFmt == HDF_CENTER)
			nFormat |= DT_CENTER;
		else if (nFmt == HDF_LEFT)
			nFormat |= DT_LEFT;
		else
			nFormat |= DT_RIGHT;
		//사용자 지정 글자체가 지정되었는가 검사한다.
		if(pItemData[nCol].nFontEffect || pItemData[nCol].nFontSize)
		{
			CFont newFont;
			GetItemFont(newFont,pItemData[nCol].nFontEffect,pItemData[nCol].nFontSize);
			CFont *prevFont=pDC->SelectObject(&newFont);
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(pItemData[nCol].clrText);
			//pDC->SetTextColor(RGB(255,0,0));
			pDC->DrawText(str, &rect, nFormat);
			pDC->SelectObject(prevFont);
			newFont.DeleteObject();
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(pItemData[nCol].clrText);
			//pDC->SetTextColor(RGB(255,0,0));
			pDC->DrawText(str, &rect, nFormat);
		}

	}
}

//아이템에 체크상자를 그린다.
void CListCtrlPAIX::DrawItemCheckBox(int nRow, int nCol, CDC *pDC,CRect &rect,
									LVITEMDATA *pItemData)
{
	if (rect.IsRectEmpty())
	{
		return;
	}

	if(!IsBkImage()) pDC->FillSolidRect(&rect,pItemData[nCol].clrBkgnd);

	CRect chkboxrect;
	chkboxrect = rect;
	chkboxrect.top+=2;
	chkboxrect.bottom -= 2;
	chkboxrect.left += 1;		// line up checkbox with header checkbox
	chkboxrect.right = chkboxrect.left + chkboxrect.Height();	// width = height
	CString str=GetItemText(nRow,nCol);
	if (str.IsEmpty())
	{
		// center the checkbox

		chkboxrect.left = rect.left + rect.Width()/2 - chkboxrect.Height()/2 - 1;
		chkboxrect.right = chkboxrect.left + chkboxrect.Height();
	}

	// fill rect around checkbox with white
	//pDC->FillSolidRect(&chkboxrect,clrBkgnd);

	// draw border
	CBrush brush(RGB(51,102,153));
	pDC->FrameRect(&chkboxrect, &brush);
	
	if(pItemData[nCol].nChkState)
	{
		CPen *prevPen = NULL;
		CPen blackPen(PS_SOLID, 1, RGB(51,153,51));

		prevPen = pDC->SelectObject(&blackPen);
		
		// draw the checkmark
		int x = chkboxrect.left + 9;
		ASSERT(x < chkboxrect.right);
		int y = chkboxrect.top + 3;
		int i;
		for (i = 0; i < 4; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y+3);
			x--;
			y++;
		}
		for (i = 0; i < 3; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y+3);
			x--;
			y--;
		}

		if (prevPen)
		{
			pDC->SelectObject(prevPen);
			blackPen.DeleteObject();
		}
	}

	if (!str.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(pItemData[nCol].clrText);
		CRect textrect;
		textrect = rect;
		textrect.left = chkboxrect.right + 4;

		UINT nFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE;	//+++
		
		pDC->DrawText(str, &textrect, nFormat);
	}
}

//아이템에 아이콘 이미지를 그린다

int CListCtrlPAIX::DrawItemImage(int nRow, int nCol, CDC *pDC, CRect &rect,
									 LVITEMDATA *pItemData)
{
	if (rect.IsRectEmpty())
	{
		return 0;
	}
	//배경이미지가 지정되어 있으면 아이템 배경을 칠하지 않는다.
	if(!IsBkImage()) pDC->FillSolidRect(&rect, pItemData[nCol].clrBkgnd);

	int nWidth = 0;
	rect.left+=1;

	CImageList* pImageList = GetImageList(LVSIL_SMALL);
	if (pImageList)
	{
		SIZE sizeImage;
		sizeImage.cx = sizeImage.cy = 0;
		IMAGEINFO info;

		int nImage = -1;
		if (pItemData)
			nImage = pItemData[nCol].nImage;

		if (nImage == -1)
			return 0;

		if (pImageList->GetImageInfo(nImage, &info))
		{
			sizeImage.cx = info.rcImage.right - info.rcImage.left;
			sizeImage.cy = info.rcImage.bottom - info.rcImage.top;
		}

		if (nImage >= 0)
		{
			if (rect.Width() > 0)
			{
				POINT point;

				point.y = rect.CenterPoint().y - (sizeImage.cy >> 1);
				point.x = rect.left;

				SIZE size;
				size.cx = rect.Width() < sizeImage.cx ? rect.Width() : sizeImage.cx;
				size.cy = rect.Height() < sizeImage.cy ? rect.Height() : sizeImage.cy;

				// save image list background color
				COLORREF rgb = pImageList->GetBkColor();

				// set image list background color
				pImageList->SetBkColor(pItemData[nCol].clrBkgnd);
				pImageList->DrawIndirect(pDC, nImage, point, size, CPoint(0, 0));
				pImageList->SetBkColor(rgb);

				//nWidth = sizeImage.cx + m_HeaderCtrl.GetSpacing();
				nWidth=sizeImage.cx;
			}
		}
	}

	return nWidth;
}
//진행막대를 아이템에 그린다.
void CListCtrlPAIX::DrawItemProgress(int nRow, int nCol, CDC *pDC, CRect &rect,
								  LVITEMDATA *pItemData)
{
	UNUSED_ALWAYS(nRow);

	if (rect.IsRectEmpty())
	{
		return;
	}
	//배경색상 적용
	if(!IsBkImage()) pDC->FillSolidRect(rect,pItemData[nCol].clrBkgnd);
	rect.top+=2;
	rect.bottom -= 2;
	rect.left += 1;		// leave margin in case row is highlighted
	int nPercent=pItemData[nCol].nProgressPercent;

	// 진행막대의 색상 다크그레이
	pDC->FillSolidRect(rect, RGB(192,192,192));

	// 입체 사각테두리
	pDC->Draw3dRect(&rect, RGB(0,0,0),GetSysColor(COLOR_BTNSHADOW));

	if (nPercent >= 0)
	{
		// draw progress bar and text

		CRect LeftRect, RightRect;
		LeftRect = rect;
		LeftRect.left += 1;
		LeftRect.top += 1;
		LeftRect.bottom -= 1;
		RightRect = LeftRect;
		if(nPercent>100) nPercent=100;
		int w = (LeftRect.Width() * nPercent) / 100;
		LeftRect.right = LeftRect.left + w - 1;
		RightRect.left = LeftRect.right;
		pDC->FillSolidRect(LeftRect,RGB(0,128,255)/*RGB(255,22,255)*/);

			
		CString str;
		CFont *PrevFont=NULL;
		if(!nPercent)
			str=_T("대기");
		else if(nPercent<100)
			str.Format(_T("%d%%"), nPercent);
		else if(nPercent>=100)
			str=_T("완료");
			
			
		pDC->SetBkMode(TRANSPARENT);
		//콤보폰트를 진행막대 폰트로 지정한다. 3포인트 작다 기본폰트 크기보다
		PrevFont=pDC->SelectObject(m_cb_font);

			CRect TextRect;
			TextRect = rect;
			TextRect.DeflateRect(1, 1);

			CRgn rgn;
			rgn.CreateRectRgn(LeftRect.left, LeftRect.top, LeftRect.right, 
					LeftRect.bottom);
			pDC->SelectClipRgn(&rgn);
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));//crBkgnd);
			pDC->DrawText(str, &TextRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			rgn.DeleteObject();
			
			rgn.CreateRectRgn(RightRect.left, RightRect.top, RightRect.right, 
					RightRect.bottom);
			pDC->SelectClipRgn(&rgn);
			pDC->SetTextColor(RGB(217,217,217));
			pDC->DrawText(str, &TextRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			rgn.DeleteObject();
			pDC->SelectClipRgn(NULL);
			pDC->SelectObject(PrevFont);
			//콤보 폰트는 제거하지 않는다.
			

		
	}
	
}
//진행막대의 위치를 지정한다.
void CListCtrlPAIX::SetProgressPos(int nRow, int nCol, int nPos)
{
	LVITEMDATA *pItemData=(LVITEMDATA*)GetItemData(nRow);
	pItemData[nCol].nProgressPercent=nPos;
}
//진행막대의 위치를 얻는다.
int CListCtrlPAIX::GetProgressPos(int nRow, int nCol)
{
	LVITEMDATA *pItemData=(LVITEMDATA*)GetItemData(nRow);
	return pItemData[nCol].nProgressPercent;

}
//각 아이템의 배경색상과 글자색상을 지정한다.
void CListCtrlPAIX::SetItemColor(int nRow1, int nRow2, int nCol1, int nCol2,
							  COLORREF clrBkgnd,COLORREF clrText)
{
	for(int i=nRow1;i<=nRow2;i++)
	{
		LVITEMDATA *pItemData=(LVITEMDATA*)GetItemData(i);
		for(int j=nCol1;j<=nCol2;j++)
		{
			pItemData[j].clrBkgnd=clrBkgnd;
			pItemData[j].clrText=clrText;
		}
	}


}
//아이템의 배경색과 글자색을 얻는다.
void CListCtrlPAIX::GetItemColor(int nRow,int nCol,COLORREF &clrBkgnd,COLORREF &clrText)
{
	LVITEMDATA *pItemData=(LVITEMDATA*)GetItemData(nRow);

	clrBkgnd=pItemData[nCol].clrBkgnd;
	clrText=pItemData[nCol].clrText;

}
//아이템의 체크상태를 입력한다.
void CListCtrlPAIX::SetItemCheck(int nRow,int nCol,int nChkState)
{
	LVITEMDATA *pItemData=(LVITEMDATA*)GetItemData(nRow);
	if(nChkState)
		pItemData[nCol].nChkState=1;
	else
		pItemData[nCol].nChkState=0;

	
}
//아이템의 체크상태를 얻는다.
int CListCtrlPAIX::GetItemCheck(int nRow, int nCol)
{
	LVITEMDATA *pItemData=(LVITEMDATA*)GetItemData(nRow);
	return pItemData[nCol].nChkState;
}
//배경이미지가 지정되어 있는지 판단한다.
bool CListCtrlPAIX::IsBkImage()
{
	LVBKIMAGE bki;
	if(GetBkImage(&bki) && (bki.ulFlags != LVBKIF_SOURCE_NONE))
		return true;

	return false;


}

void CListCtrlPAIX::GetItemFont(CFont &pFont,int nFontEffect,int nFontSize)
{
	
	LOGFONT *plf=(LOGFONT*)malloc(sizeof(LOGFONT));
	memset(plf,0,sizeof(LOGFONT));
	GetFont()->GetLogFont(plf);
	plf->lfWidth=nFontSize;
	switch(nFontEffect)
	{
	case 1://bold
		plf->lfWeight=FW_BOLD;
		break;
	case 2://italic
		plf->lfItalic=true;
		break;
	case 3://underline
		plf->lfUnderline=true;

	}
	pFont.CreateFontIndirect(plf);
	delete plf;
}
//사용자로 부터 글꼴 정보를 입력받아 아이템데이타에 저장한다.
void CListCtrlPAIX::SetItemFont(int nRow, int nCol, int nFontEffect, int nFontSize)
{
	LVITEMDATA *pItemData=(LVITEMDATA*)GetItemData(nRow);
	pItemData[nCol].nFontEffect=nFontEffect;
	pItemData[nCol].nFontSize=nFontSize;
}

//사용자에게 아아템데이타에 저장된 글꼴 정보를 보여준다.
void CListCtrlPAIX::GetItemFont(int nRow, int nCol, int &nFontEffect, int &nFontSize)
{
	LVITEMDATA *pItemData=(LVITEMDATA*)GetItemData(nRow);
	nFontEffect=pItemData[nCol].nFontEffect;
	nFontSize=pItemData[nCol].nFontSize;

}

void CListCtrlPAIX::ComboBoxClear()
{
	m_iStringStartPoint = 0;
}

void CListCtrlPAIX::AddRowComboBoxSet(int iRow, int iCBCount, LPSTR * pstrData)				//ComboBox에 Item 추가
{
	int iCount = 0;
	int CBCount = 0;
	int iItemCount = 0;

	m_pComboNo =new int[iCBCount];
	memset(m_pComboNo,0,sizeof(int)*iCBCount);//초기화
	m_pComboNo[CBCount] = iItemCount;
	do
	{
		memcpy(m_pString + iCount, pstrData + iCount, sizeof(pstrData));
		if (m_pString[iCount] == NULL)
		{
			CBCount++;
			m_pComboNo[CBCount] = iItemCount + 1;
		}
		iItemCount++;
		iCount++;
	}while(CBCount < iCBCount);
}

//사용자로 부터 Row별 컨트럴 종류 정보를 입력받는다.
void CListCtrlPAIX::SetRowCtrlInfo(int nRCInfo[][2],int nRCInfoSize, int nRCCount, int nRCDesc[])
{
	int nCount;
	int i,j,k;
	if(!GetColumnsCnt()) return;//컬럼의 수가 0이면 끝낸다.

	nCount=(nRCInfoSize/sizeof(int)) / 2;//입력된 배열의 크기를 구한다.
	m_pRCInfo=new int[nRCCount];     //포인터에 배열의 크기만큼 메모리를 할당
	m_pComboDesc = new int[nRCCount];
	memset(m_pCCInfo,0,sizeof(int)*nRCCount);//초기화
	memset(m_pComboDesc,0,sizeof(int)*nRCCount);//초기화

//2차원 배열을 읽어서 1차원 배열에 넣는다.(컬럼 컨트럴 정보)
	for(k=0;k<nRCCount;k++)//컬럼수 만큼 반복
	{
		for(i=0;i<nRCCount;i++)//컨트럴 수만큼 반복
		{
			for(j=0;j<2;j++)//배열의 인수갯수 반복
			{
				if(j==0 && k==nRCInfo[i][j])
				{
					m_pRCInfo[k]=nRCInfo[i][j+1];//2차원배열을 전역변수에 복사한다.
					m_pComboDesc[k] = nRCDesc[i];
					if(m_pRCInfo[k]==1)
					{
						m_nCountCtrl[0]++;//1이면 에디터 수증가
					}
					else if(m_pRCInfo[k]==2) 
					{
						m_nCountCtrl[1]++;//2이면 콤보증가
					}
					else if(m_pRCInfo[k]==3) 
					{
						m_nCountCtrl[2]++;//3이면 체크증가
					}
				}//if(j==0) 닫음

			}//for(j=0)닫음
		}//for(i=0)닫음

	}//for(k=0)닫음
}

//사용자로 부터 컬럼별 컨트럴 종류 정보를 입력받는다.
void CListCtrlPAIX::SetColumnsCtrlInfo(int nCCInfo[][2], int nCCInfoSize)
{
	int nCount;
	int i,j,k;

	if(!GetColumnsCnt()) return;//컬럼의 수가 0이면 끝낸다.

	nCount=(nCCInfoSize/sizeof(int))/2;//입력된 배열의 크기를 구한다.
	m_pCCInfo=new int[GetColumnsCnt()];//포인터에 배열의 크기만큼 메모리를 할당
	m_pComboDesc = new int[GetColumnsCnt()];
	memset(m_pCCInfo,0,sizeof(int)*GetColumnsCnt());//초기화
	memset(m_pComboDesc,0,sizeof(int)*GetColumnsCnt());//초기화
//2차원 배열을 읽어서 1차원 배열에 넣는다.(컬럼 컨트럴 정보)
	for(k=0;k<GetColumnsCnt();k++)//컬럼수 만큼 반복
	{
		for(i=0;i<nCount;i++)//컨트럴 수만큼 반복
		{
			for(j=0;j<2;j++)//배열의 인수갯수 반복
			{
				if(j==0 && k==nCCInfo[i][j])
				{
					m_pCCInfo[k]=nCCInfo[i][j+1];//2차원배열을 전역변수에 복사한다.
					if(m_pCCInfo[k]==1) m_nCountCtrl[0]++;//1이면 에디터 수증가
					else if(m_pCCInfo[k]==2) {
						m_pComboDesc[k] = m_nCountCtrl[1];
						m_nCountCtrl[1]++;//2이면 콤보증가
					}
					else if(m_pCCInfo[k]==3) m_nCountCtrl[2]++;//3이면 체크증가
				}//if(j==0) 닫음

			}//for(j=0)닫음
		}//for(i=0)닫음

	}//for(k=0)닫음
}


LRESULT CListCtrlPAIX::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//자식윈도우의 메시지는 WM_COMMAND
	//(LOWORD)WPARAM->자식윈도우 ID,(HIWORD)WPARAM->메시지 값
	//LPARM->자식윈도우 핸들
	switch(message)
	{
	case WM_COMMAND:
		{
			//체크버튼을 클릭했을때
			if(LOWORD(wParam)==IDC_CHECK_CHILD && HIWORD(wParam)==BN_CLICKED)
				OnCheckChild_Click((HWND)lParam);
			//체크버튼 포커스 잃을때
			else if(LOWORD(wParam)==IDC_CHECK_CHILD && HIWORD(wParam)==BN_KILLFOCUS)
				OnCheckChild_LostFocus((HWND)lParam);
		}
		break;
	case WM_NOTIFY:
		{
			if((int)wParam==GetHeaderCtrl()->GetDlgCtrlID())
			{
				NMHEADER *pnmhdr=(NMHEADER*)lParam;
				//사용자가 컬럼의 넓이를 조정할때
				if(pnmhdr->hdr.code==HDN_BEGINTRACK ||  pnmhdr->hdr.code==HDN_ENDTRACK)
				{
					OnHdnTracking(pnmhdr);
					return (LRESULT)0;//0이면 컬럼 넓이조정가능
				}
				
			
			}
			if((int)wParam==IDC_CHECK_CHILD)//체크버튼의 메시지
			{
				NMCUSTOMDRAW *pnmcd=(NMCUSTOMDRAW*)lParam;//사용자그리기 신호
				if(pnmcd->hdr.code==NM_CUSTOMDRAW)
				{
					if(pnmcd->dwDrawStage==CDDS_PREPAINT )//컨트럴을 그리기 전 처리
					{
						CDC *pDC=pDC->FromHandle(pnmcd->hdc);//HDC->CDC
						CButton *pCheck=(CButton*)pCheck->FromHandle(pnmcd->hdr.hwndFrom);//HWND->CWnd
						LVITEMDATA *pItemData=(LVITEMDATA*)GetItemData(m_nRow);
						CString str;					
						CRect rect;
						rect=pnmcd->rc;//RECT->CRect
						rect.left+=rect.Height()+2;//체크 그림을 제외한 부분의 영역
						pCheck->GetWindowText(str);
						pDC->SetTextColor(pItemData[m_nCol].clrText);//글자의 색상을 사용자색으로 변경
						pDC->DrawText(str,rect,DT_LEFT|DT_VCENTER);//글자를 다시 그린다.
						
						return CDRF_SKIPDEFAULT ;//이 신호는 사용자가 직접 처리한다.*/
					}
					return CDRF_DODEFAULT ;//이 신호는 시스템이 자동으로 처리한다.
				}
			}
		}
	}
	

	return CListCtrl::DefWindowProc(message, wParam, lParam);//나머지 메시지가 들어오면 시스템이 처리
}


//자식윈도우의 컨트럴 색상변경
HBRUSH CListCtrlPAIX::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CListCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	LVITEMDATA *pItemData=(LVITEMDATA*)GetItemData(m_nRow);
	COLORREF clrBkgnd,clrText;
	clrBkgnd=pItemData[m_nCol].clrBkgnd;
	clrText=pItemData[m_nCol].clrText;
	if(nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		
		if(IsBkImage()) return CreateSolidBrush(GetBkColor());
		return CreateSolidBrush(clrBkgnd); //배경색을 그리드와 같은 배경색으로 변경
	}
	
	else if(nCtlColor==CTLCOLOR_EDIT )
		
	{		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(clrText);
		if(!IsBkImage()) return CreateSolidBrush(clrBkgnd); //배경색을 그리드와 같은 배경색으로 변경
	}

	return hbr;
}


void CListCtrlPAIX::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	LVITEMDATA *pItemData=(LVITEMDATA*)GetItemData(pNMLV->iItem);
	if(pItemData) 
	{
		delete [] pItemData;pItemData=NULL;
	}
}

void CListCtrlPAIX::PopupMenuSet(BOOL bPopMenu, int UserMenu)
{
	m_pUmenu=NULL;
	m_bPopMenuEnable = TRUE;
	//메뉴를 사용하지 않을 경우
	if(bPopMenu == FALSE)
	{
		m_bPopMenuEnable = FALSE;
		if(m_menu.GetSafeHmenu())
		{
			m_menu.DestroyMenu();
		}
		return;
	}

	if(m_menu.GetSafeHmenu())
	{
		m_menu.DestroyMenu();
	}
	m_menu.CreatePopupMenu( ); 

	//clipboard Option
	m_menu.AppendMenu(MF_STRING, CCMSG_INSERT_CLIPBOARD, _T( "삽입	Ctrl+I")); 
	m_menu.AppendMenu(MF_STRING, CCMSG_COPY_CLIPBOARD, _T( "복사	Ctrl+C")); 
	m_menu.AppendMenu(MF_STRING, CCMSG_PASTE_CLIPBOARD, _T("붙여넣기	Ctrl+V")); 
	m_menu.AppendMenu(MF_STRING, CCMSG_CUT_CLIPBOARD, _T("잘라내기	Ctrl+X")); 
	m_menu.AppendMenu(MF_STRING, CCMSG_SELITEM_DELETE, _T("삭제	Ctrl+Del"));
	if (UserMenu)
	{
		m_menu.AppendMenu(MF_SEPARATOR); 
		if (UserMenu == 1) m_menu.AppendMenu(MF_STRING, WM_LISTCTRL_USER1, _T("Teaching")); 
	}
}


int CListCtrlPAIX::GetSubItemCount()
{
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	int iColCnt=0;
	while(GetColumn( ++iColCnt, &lvcol) ){};
	return iColCnt;
}

void CListCtrlPAIX::SelectItemInsert()
{
	int nSubItemCount=GetSubItemCount();
	CString str;
	
	POSITION pos=GetFirstSelectedItemPosition();

	int nItem=GetNextSelectedItem(pos);
	str.Format(_T("%d"), nItem + m_iItemNoStartText);
	InsertItem(nItem,str);
	
	/*SetItemText(nItem, 1, _T("Mode 0"));
	SetItemText(nItem, 2, _T("INC"));
	SetItemText(nItem, 3, _T("0.00"));
	SetItemText(nItem, 4, _T("200"));
	SetItemText(nItem, 5, _T("200"));
	SetItemText(nItem, 6, _T("50.00"));
	str.Format(_T("%d"), nItem + 1);
	SetItemText(nItem, 7, str);
	SetItemText(nItem, 8, _T("0"));
	SetItemText(nItem, 9, _T("0"));
	SetItemText(nItem, 10, _T("0.00"));
	SetItemText(nItem, 11, _T("0.00"));
	SetItemText(nItem, 12, _T("0.00"));
	SetItemText(nItem, 13, _T("Full Servo"));*/

	
	DeleteItem(m_iMaxItemCount);
	nItem++;
	for (int i = nItem; i < m_iMaxItemCount; i++)
	{
		str.Format(_T("%d"), i + m_iItemNoStartText);
		SetItemText(i, 0, str);
	}
	HWND hWnd1 = ::GetParent(m_hWnd);
	::PostMessage(hWnd1, WM_LISTCTRL_PASTE_END, GetDlgCtrlID(), 0);
}

void CListCtrlPAIX::CopyClipboard()
{
	/*if(!(m_nClipOptin & clipCOPY))
		return;*/
	if(!::OpenClipboard(m_hWnd))
		return;

	if (!::EmptyClipboard())
		return;

	int nSubItemCount=GetSubItemCount();
	
	LPTSTR pBuff;
	CString szDivide= _T("	");
	CString strCopyData= _T("");
	HGLOBAL hGlobal;
	
	POSITION pos=GetFirstSelectedItemPosition();
	while(1)
	{
		if(!pos) break;
		int nItem=GetNextSelectedItem(pos);

		CString strText= _T("");
		for( int i=1; i<nSubItemCount; i++)
			strText+=(GetItemText(nItem,i)+szDivide);

		strText.TrimRight(szDivide);
		strText += _T("\r\n");
		strCopyData += strText;
	}

	int size = strCopyData.GetLength() * sizeof(LPTSTR) +1;
	hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, size);
	pBuff = (LPTSTR)::GlobalLock(hGlobal);

	memcpy(pBuff, strCopyData, size  ) ;

	
	::GlobalUnlock(hGlobal);
	
	::SetClipboardData(CF_UNICODETEXT, hGlobal);
	::CloseClipboard();
	m_ucClipType = COPY_CLIP;
}

void CListCtrlPAIX::PasteClipboard()
{
	/*if(!(m_nClipOptin & clipPASTE))
		return;*/

	if ( !::OpenClipboard(m_hWnd) ) 
		return;

	int i=0,j=0;
	int nRow=GetItemCount();
	int nCol=GetSubItemCount();
	CString szTab=_T("	");	//TAB KEY

	CString strText= _T("");
	CString strPasteData= _T("");
	CString str;

	HANDLE hData = ::GetClipboardData( CF_TEXT );
	strPasteData = (LPCSTR)GlobalLock( hData );

	POSITION pos=GetFirstSelectedItemPosition();
	if(!pos) return;
	int nItem=GetNextSelectedItem(pos);
	
	while(1)
	{
		int nStart=0, nEnd=0;
		int nFind = strPasteData.Find(_T("\r\n"));
		if(nFind<0)	break;

		strText = strPasteData.Left(nFind);
		strPasteData.Delete(0, nFind+2);
		
		if (nItem < m_iMaxItemCount)
		{
			InsertItem(nItem,_T(""));
		}
		str.Format(_T("%d"), nItem + m_iItemNoStartText);
		SetItemText(nItem, 0, str);

		nCol= 1;
		
		while((nEnd=strText.Find(szTab, nStart))>=0)
		{
			SetItemText(nItem, nCol++, strText.Mid(nStart, nEnd-nStart));
			nStart=nEnd+1;
		}

		SetItemText(nItem, nCol, strText.Mid(nStart, nStart+256));
		nItem++;
	}
	
	while(nItem < m_iMaxItemCount)
	{
		str.Format(_T("%d"), nItem + m_iItemNoStartText);
		SetItemText(nItem, 0, str);
		nItem++;
	}
	nItem=GetItemCount()-1;
	if (nItem >= m_iMaxItemCount)
	{
		while(nItem >= m_iMaxItemCount)
		{
			DeleteItem(nItem);
			nItem--;
		}
	}

	m_ucClipType = COPY_CLIP;
	GlobalUnlock(hData);
	CloseClipboard();
	HWND hWnd1 = ::GetParent(m_hWnd);
	::PostMessage(hWnd1, WM_LISTCTRL_PASTE_END, GetDlgCtrlID(), 0);
}

void CListCtrlPAIX::CutClipboard()
{
	CopyClipboard();
	SelectItemDelete();
	m_ucClipType = CUT_CLIP;
}

void CListCtrlPAIX::SelectItemDelete()
{
	/*if(!(m_nClipOptin & clipDELETE))
		return;*/

	int i = 0;
	CString str;
	int Aboveitem = 0;

	m_iTotalRow = GetItemCount();

	ShowWindow(FALSE);
	int nItem=GetItemCount() - 1;
	while(1)
	{
		if(nItem==-1) break;
		Aboveitem = nItem;
		nItem = GetNextItem(nItem, LVNI_ABOVE|LVNI_SELECTED);
		if (nItem == -1)
		{
			DeleteItem(m_iMaxItemCount - 1);
		}
		else
		{
			DeleteItem(nItem);
		}
	}
	for (i = Aboveitem; i< m_iMaxItemCount;i++)
	{
		str.Format(_T("%d"), i + m_iItemNoStartText);
		SetItemText(i, 0, str);
	}
	nItem=GetItemCount();
	for (i = nItem; i< m_iMaxItemCount;i++)
	{
		str.Format(_T("%d"), i + m_iItemNoStartText);
		InsertItem(i, str);
	}

	ShowWindow(TRUE);
	HWND hWnd1 = ::GetParent(m_hWnd);
	::PostMessage(hWnd1, WM_LISTCTRL_PASTE_END, GetDlgCtrlID(), 0);
}

void CListCtrlPAIX::LIstUserCMD1()
{
}

void CListCtrlPAIX::LIstUserCMD2()
{
}

void CListCtrlPAIX::LIstUserCMD3()
{
}


BOOL CListCtrlPAIX::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam == VK_SPACE)   //ESC 키면 무시..
       return TRUE;

	if(pMsg->message == WM_KEYDOWN)
	{	
		if(m_bEdit == true)//&& nSubItem != 0 && nSubItem != 1 && nSubItem != 3 && nSubItem != 4 && nSubItem != 5)//&& nItem != 0)
		{
			if(((0x2F < pMsg->wParam) && (pMsg->wParam < 0x3A)) || ((0x5F < pMsg->wParam) && (pMsg->wParam < 0x6A)) || ((0x24 < pMsg->wParam) && (pMsg->wParam < 0x29)) 
				|| (pMsg->wParam == VK_BACK) || (pMsg->wParam == VK_DELETE) || (pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_SUBTRACT)
				|| (pMsg->wParam == 0xBD) || (pMsg->wParam == VK_DECIMAL) || (pMsg->wParam == VK_OEM_PERIOD) || (pMsg->wParam == VK_TAB) || (pMsg->wParam == VK_LEFT)
				|| (pMsg->wParam == VK_RIGHT) || (pMsg->wParam == VK_UP) || (pMsg->wParam == VK_DOWN))
			{
				//if(pMsg->wParam == VK_ESCAPE)   //ESC 키면 무시..
                //OnListAdd_CANCLE();

				if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_DOWN)   //ESC 키면 무시..
				{
					//OnBnClickedButtonDataWritePos();
					Enter_Refresh(1); 
				}
				
				if (pMsg->wParam == VK_UP)   //ESC 키면 무시..
				{
					//OnBnClickedButtonDataWritePos();
					Enter_Refresh(-1); 
				}

				/*if((pMsg->wParam == VK_TAB) && (GetKeyState(VK_SHIFT) < 0) || (pMsg->wParam == VK_LEFT))
				{
					Tab_Refresh(-1); 
					return TRUE;
				}
				
				if(pMsg->wParam == VK_TAB || (pMsg->wParam == VK_RIGHT))   //ESC 키면 무시..
				{
					Tab_Refresh(1); 
					return TRUE;
				}*/
			}
			else
			{
				return TRUE;
			}
		}
		else
		{
			if(pMsg->wParam == VK_RETURN)   //ESC 키면 무시.
			{
				if((m_nCol != 0) && (m_nRow != -1))
				{
					//ListEditEanble(::GetDlgItem (m_hWnd,IDC_LIST_POSITION));
					//Enter_Refresh(1); 
				}
				else
				{
					//m_bListDataDblclk = FALSE; //리스트 박스 더블 클릭 상태
					//Invalidate(FALSE);
					//return;
					//Enter_Refresh(1); 
				}						
			}
		}
	}


	if(pMsg->wParam == VK_ESCAPE)   //ESC 키면 무시..
       return TRUE;

	if(pMsg->wParam == VK_RETURN)   //ESC 키면 무시..	
       return TRUE;

	if(pMsg->wParam == VK_SPACE)   //ESC 키면 무시..
       return TRUE;

	return CListCtrl::PreTranslateMessage(pMsg);
}



void CListCtrlPAIX::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_bPopMenuEnable == FALSE) 
	{
		return;
	}
	if(point.x==-1 && point.y==-1)
	{
		point=CPoint(0,0);
		ClientToScreen(&point);
	}
	if(m_pUmenu)
	{
		CWnd* pParWnd = GetParent();
 		m_pUmenu->TrackPopupMenu(
 			TPM_CENTERALIGN | TPM_RIGHTBUTTON,
 			point.x, point.y, pWnd, 0);
	}
	else if(m_menu.GetSafeHmenu())
	{
 		m_menu.TrackPopupMenu(
 			TPM_CENTERALIGN | TPM_RIGHTBUTTON,
 			point.x, point.y, pWnd, 0);
	}
}