// OnDutyPage1.cpp : implementation file
//

#include "stdafx.h"
#include "domitory.h"
#include "OnDutyPage1.h"
#include "OnDutyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COnDutyPage1 property page
extern CDatabase Database;
//
IMPLEMENT_DYNCREATE(COnDutyPage1, CPropertyPage)

COnDutyPage1::COnDutyPage1() : CPropertyPage(COnDutyPage1::IDD)
{
	//{{AFX_DATA_INIT(COnDutyPage1)
	m_Date_Retrieve = FALSE;
	m_ID_Retrieve = FALSE;
	m_OnDutyTime = 0;
	m_OnDutyTime=CTime::GetCurrentTime(); //����ʱ��ؼ���ʼ��ʱ����ʾ
	m_State=FALSE;//ָʾ�б�ǰ��ʾ״̬Ϊ��ǰ��¼�������м�¼��Ĭ��Ϊ��ǰ��¼
	//}}AFX_DATA_INIT
}

COnDutyPage1::~COnDutyPage1()
{
}

void COnDutyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COnDutyPage1)
	DDX_Control(pDX, IDC_CURLIST, m_CurList);
	DDX_Check(pDX, IDC_CHECK_BY_DATE, m_Date_Retrieve);
	DDX_Check(pDX, IDC_CHECK_BY_ID, m_ID_Retrieve);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_OnDutyTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COnDutyPage1, CPropertyPage)
	//{{AFX_MSG_MAP(COnDutyPage1)
	ON_BN_CLICKED(IDC_RETRIEVE, OnRetrieve)
	ON_BN_CLICKED(IDC_ALL_RECORD, OnAllRecord)
	ON_EN_CHANGE(IDC_ON_DUTY_ID, OnChangeOnDutyId)
	ON_BN_CLICKED(IDC_ADD_RECORD, OnAddRecord)
	ON_BN_CLICKED(IDC_DELETE_RECORD, OnDeleteRecord)
	ON_BN_CLICKED(IDC_CHANGE_RECORD, OnChangeRecord)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COnDutyPage1 message handlers

BOOL COnDutyPage1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_CurList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_CurList.InsertColumn(0,"���",LVCFMT_LEFT,90);
	m_CurList.InsertColumn(1,"����",LVCFMT_LEFT,110);
	m_CurList.InsertColumn(2,"ֵ������",LVCFMT_LEFT,110);
	m_CurList.InsertColumn(3,"��ע",LVCFMT_LEFT,150);

	//��ʾ�����¼
	COnDutyRS OnDutyRS(&Database);
	//��ȡ��ǰ���ڼ�
	CString TempStr;
	CTime CurTime;
	CurTime=CTime::GetCurrentTime();
	int Day=CurTime.GetDayOfWeek();
	TempStr.Format("%d",Day);
	OnDutyRS.m_strFilter="DAY_OF_WEEK = "+ TempStr;
	ShowODList(OnDutyRS);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COnDutyPage1::ShowODList(COnDutyRS &OnDutyRS)
{
	int i=0;
	CString Str;
	OnDutyRS.Open();
	m_CurList.DeleteAllItems();
	while (!OnDutyRS.IsEOF())
	{
 		m_CurList.InsertItem(i," ");
		m_CurList.SetItemText(i,0,OnDutyRS.m_ON_DUTY_ID);
		m_CurList.SetItemText(i,1,OnDutyRS.m_NAME);
		//add on duty time
        Str=TransformTime(OnDutyRS.m_DAY_OF_WEEK,OnDutyRS.m_DAY_NIGHT); //��ȡֵ��ʱ��
		m_CurList.SetItemText(i,2,Str);
		m_CurList.SetItemText(i,3,OnDutyRS.m_REMARKS);
		OnDutyRS.MoveNext();
		i++;
	}
	OnDutyRS.Close();
}

CString COnDutyPage1::TransformTime(long a, CString b)
{
	CString Temp1,Temp2;
	switch (a)
	{
	case 1:
		Temp1="����";
		break;
	case 2:
		Temp1="��һ";
		break;
	case 3:
		Temp1="�ܶ�";
		break;
	case 4:
		Temp1="����";
		break;
	case 5:
		Temp1="����";
		break;
	case 6:
		Temp1="����";
		break;
	case 7:
		Temp1="����";
		break;
	default:
		break;
	}
	if (b.Compare("D")==0)
	{
		Temp2="����";
	}
	if (b.Compare("N")==0)
	{
		Temp2="ҹ��";
	}
	Temp1=Temp1 + " " + Temp2;
	return Temp1;
}

void COnDutyPage1::OnRetrieve() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	//ʱ���Ա������������Ϊ��
	if (m_Date_Retrieve==FALSE && m_ID_Retrieve==FALSE)
	{
		MessageBox("��ѡ���������","��ʾ",MB_OK | MB_ICONINFORMATION);
		return;
	}
    //
	COnDutyRS OnDutyRS(&Database);
	int CurDay;
	CString Temp1,Temp2;
	CurDay=m_OnDutyTime.GetDayOfWeek();
	Temp1.Format("%d",CurDay);
	GetDlgItemText(IDC_ON_DUTY_ID,Temp2);
	//ʱ���Ա������������Ϊ��
	if (m_Date_Retrieve==TRUE && m_ID_Retrieve==TRUE)
	{
		OnDutyRS.m_strFilter="DAY_OF_WEEK = " + Temp1 + " and ON_DUTY_ID = '" + Temp2 + "'";  
	}
	//ʱ���������Ϊ�棬Ա����������Ϊ��
	else if (m_Date_Retrieve==TRUE && m_ID_Retrieve==FALSE)
	{
		OnDutyRS.m_strFilter="DAY_OF_WEEK = " + Temp1;
	}
	//ʱ���������Ϊ�٣�Ա����������Ϊ��
	else
	{
		OnDutyRS.m_strFilter="ON_DUTY_ID = '" + Temp2 + "'";
	}
	//��ʾ
	ShowODList(OnDutyRS);
}

void COnDutyPage1::OnAllRecord() 
{
	// TODO: Add your control notification handler code here
	m_State=TRUE;
	COnDutyRS TempRS(&Database);
	ShowODList(TempRS);
}

void COnDutyPage1::OnChangeOnDutyId() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	CString PersonID;
	CPersonODRS PersonODRS(&Database);
	GetDlgItemText(IDC_ON_DUTY_ID,PersonID);
	PersonODRS.m_strFilter="PERSON_ID = '"+ PersonID + "'";
	PersonODRS.Open();
	if (PersonODRS.GetRecordCount()==1)
	{
		SetDlgItemText(IDC_ON_DUTY_NAME,PersonODRS.m_NAME);
	}
	else
	{
		SetDlgItemText(IDC_ON_DUTY_NAME,"");
	}
	PersonODRS.Close();
	// TODO: Add your control notification handler code here
	
}

void COnDutyPage1::OnAddRecord() 
{
	// TODO: Add your control notification handler code here
	CAddOnDutyRDlg AddDlg;
	if (AddDlg.DoModal()==IDOK)
	{
		
		COnDutyRS OnDutyRS(&Database);
		if (m_State==FALSE)   //�ж��Ƿ���ʾ��ǰ��¼
		{
			//��ʾ�����¼
			//��ȡ��ǰ���ڼ�
			CString TempStr;
			CTime CurTime;
			CurTime=CTime::GetCurrentTime();
			int Day=CurTime.GetDayOfWeek();
			TempStr.Format("%d",Day);
			OnDutyRS.m_strFilter="DAY_OF_WEEK = "+ TempStr;
		}
		ShowODList(OnDutyRS);
	}
}

void COnDutyPage1::OnDeleteRecord() 
{
	// TODO: Add your control notification handler code here
	int Index;
	CString Sql;
	POSITION pos=m_CurList.GetFirstSelectedItemPosition();
	if (pos==NULL)
	{
		return;
	}
	Index=m_CurList.GetNextSelectedItem(pos);
	Sql="delete * from ON_DUTY where ON_DUTY_ID = '"+m_CurList.GetItemText(Index,0) + "'";
	Database.ExecuteSQL(Sql);
	COnDutyRS TempRS(&Database);
	//
	if (m_State==FALSE)   //�ж��Ƿ���ʾ��ǰ��¼
	{
		//��ʾ�����¼
		//��ȡ��ǰ���ڼ�
		CString TempStr;
		CTime CurTime;
		CurTime=CTime::GetCurrentTime();
		int Day=CurTime.GetDayOfWeek();
		TempStr.Format("%d",Day);
		TempRS.m_strFilter="DAY_OF_WEEK = "+ TempStr;
		}
	//
	ShowODList(TempRS);
}

void COnDutyPage1::OnChangeRecord() //�޸ļ�¼
{
	// TODO: Add your control notification handler code here
	int Index;
	CString Sql;
	POSITION pos=m_CurList.GetFirstSelectedItemPosition();
	if (pos==NULL)
	{
		return;
	}
	Index=m_CurList.GetNextSelectedItem(pos);
	//
	CAddOnDutyRDlg AddDlg;
	AddDlg.m_Change=TRUE;//���/�޸�ָʾ����
	//
	AddDlg.m_AddOnDutyID=m_CurList.GetItemText(Index,0);
	//
	AddDlg.m_AddODName=m_CurList.GetItemText(Index,1);
	//
	AddDlg.m_AddODRemarks=m_CurList.GetItemText(Index,3);
	
	//��ʾ�Ի���
	if (AddDlg.DoModal()==IDOK)
	{
		COnDutyRS TempRS(&Database);
		if (m_State==FALSE)   //�ж��Ƿ���ʾ��ǰ��¼
		{
			//��ʾ�����¼
			//��ȡ��ǰ���ڼ�
			CString TempStr;
			CTime CurTime;
			CurTime=CTime::GetCurrentTime();
			int Day=CurTime.GetDayOfWeek();
			TempStr.Format("%d",Day);
			TempRS.m_strFilter="DAY_OF_WEEK = "+ TempStr;
		}
		ShowODList(TempRS);
	}
}
