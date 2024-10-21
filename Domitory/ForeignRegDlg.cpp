// ForeignRegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "domitory.h"
#include "ForeignRegDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CForeignRegDlg dialog
extern CDatabase Database;

CForeignRegDlg::CForeignRegDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CForeignRegDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CForeignRegDlg)
	/*m_TimeRetrieve = FALSE;*/
	m_NameRetrieve = FALSE;
	/*m_FTime = 0;*/
	m_FName = _T("");
	m_VDomitory = _T("");
	m_V_D_Retrieve = FALSE;
	//}}AFX_DATA_INIT
}


void CForeignRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CForeignRegDlg)
	DDX_Control(pDX, IDC_FOREIGN_LIST, m_ForeignList);
	/*DDX_Check(pDX, IDC_F_TIME_RETRIEVE, m_TimeRetrieve);*/
	DDX_Check(pDX, IDC_F_NAME_RETRIEVE, m_NameRetrieve);
	/*DDX_DateTimeCtrl(pDX, IDC_F_TIME, m_FTime);*/
	DDX_Text(pDX, IDC_F_NAME, m_FName);
	DDX_Text(pDX, IDC_F_DOMITORY, m_VDomitory);
	DDX_Check(pDX, IDC_V_DOMITORY, m_V_D_Retrieve);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CForeignRegDlg, CDialog)
	//{{AFX_MSG_MAP(CForeignRegDlg)
	ON_BN_CLICKED(IDC_ADD_FORE_RECORD, OnAddForeRecord)
	ON_BN_CLICKED(IDC_DEL_FORE_RECORD, OnDelForeRecord)
	ON_BN_CLICKED(IDC_ADD_LEAVE_TIME, OnAddLeaveTime)
	ON_BN_CLICKED(IDC_F_B_RETRIEVE, OnFBRetrieve)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CForeignRegDlg message handlers

BOOL CForeignRegDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ForeignList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ForeignList.InsertColumn(0,"����",LVCFMT_LEFT,70);
	m_ForeignList.InsertColumn(1,"�ݷ�����",LVCFMT_LEFT,70);
	m_ForeignList.InsertColumn(2,"�ݷö���",LVCFMT_LEFT,70);
	m_ForeignList.InsertColumn(3,"����ʱ��",LVCFMT_LEFT,150);
	m_ForeignList.InsertColumn(4,"�뿪ʱ��",LVCFMT_LEFT,150);
	m_ForeignList.InsertColumn(5,"��ע",LVCFMT_LEFT,80);
    //
	CForeignRegRS TempRS(&Database);
	ShowForeList(TempRS);
	//
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CForeignRegDlg::ShowForeList(CForeignRegRS &ForeignRegRS)
{

	int i=0;
	CString Str;
	CString ArriveTime,LeaveTime;
	ForeignRegRS.Open();
	//
	m_ForeignList.DeleteAllItems();
	while (!ForeignRegRS.IsEOF())
	{
		m_ForeignList.InsertItem(i," ");
		// 		Str.Format("%d",OnDutyRS.m_ON_DUTY_ID);
		m_ForeignList.SetItemText(i,0,ForeignRegRS.m_NAME);
		m_ForeignList.SetItemText(i,1,ForeignRegRS.m_DOMITORY_ID);
		m_ForeignList.SetItemText(i,2,ForeignRegRS.m_STUDENT_NAME);
		//
		m_ForeignList.SetItemText(i,3,ForeignRegRS.m_ARRIVAL_TIME);
		//
		m_ForeignList.SetItemText(i,4,ForeignRegRS.m_LEAVE_TIME);
		m_ForeignList.SetItemText(i,5,ForeignRegRS.m_REMARKS);
		ForeignRegRS.MoveNext();
		i++;
	}
	ForeignRegRS.Close();
}

void CForeignRegDlg::OnAddForeRecord() 
{
	// TODO: Add your control notification handler code here
	CAddForeRecordDlg AddDlg;
	if (AddDlg.DoModal()==IDOK)
	{
		CForeignRegRS TempRS(&Database);
		ShowForeList(TempRS);
	}
}

void CForeignRegDlg::OnDelForeRecord() 
{
	// TODO: Add your control notification handler code here
	int Index;
	CString Sql;
	POSITION pos=m_ForeignList.GetFirstSelectedItemPosition();
	if (pos==NULL)
	{
		return;
	}
	Index=m_ForeignList.GetNextSelectedItem(pos);
	Sql="delete * from F_REGISTER where NAME = '"+m_ForeignList.GetItemText(Index,0) + "' and STUDENT_NAME = '"+ m_ForeignList.GetItemText(Index,2) + "'";
	Database.ExecuteSQL(Sql);
	CForeignRegRS ForeignRegRS(&Database);
	ShowForeList(ForeignRegRS);	
}

void CForeignRegDlg::OnAddLeaveTime() 
{
	// TODO: Add your control notification handler code here
	//��ȡѡ��������
	int Index;
	POSITION pos=m_ForeignList.GetFirstSelectedItemPosition();
	if (pos==NULL)
	{
		return;
	}
	Index=m_ForeignList.GetNextSelectedItem(pos);
	//
	CForeignRegRS ForeRS(&Database);
	ForeRS.m_strFilter="NAME = '"+m_ForeignList.GetItemText(Index,0) + "' and STUDENT_NAME = '"+ m_ForeignList.GetItemText(Index,2) + "'";
	ForeRS.Open();
	//�ж��뿪ʱ���ַ����Ƿ����
	if (!ForeRS.m_LEAVE_TIME.IsEmpty())
	{
		return;
	}
	ForeRS.Edit();
	//�����뿪ʱ��
    CString LeaveTime;
	CTime Temp=CTime::GetCurrentTime();
	LeaveTime=Temp.Format("%Y-%m-%d %H:%M:%S");
	ForeRS.m_LEAVE_TIME=LeaveTime;
	//
	ForeRS.Update();
	ForeRS.Close();
	//������ʾ
	CForeignRegRS ForeignRegRS(&Database);
	ShowForeList(ForeignRegRS);	
}

void CForeignRegDlg::OnFBRetrieve() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	//�����ͷ����������������Ϊ��
	if (m_V_D_Retrieve==FALSE && m_NameRetrieve==FALSE)
	{
		MessageBox("��ѡ���������","��ʾ",MB_OK | MB_ICONINFORMATION);
		return;
	}
    //
	CForeignRegRS ForeRS(&Database);

	//�����ͷ����������������Ϊ��
	if (m_V_D_Retrieve==TRUE && m_NameRetrieve==TRUE)
	{
		ForeRS.m_strFilter="NAME = '" + m_FName + "' and DOMITORY_ID = '" + m_VDomitory + "'";  
	}
	//������������Ϊ�棬���������������Ϊ��
	else if (m_NameRetrieve==TRUE && m_V_D_Retrieve==FALSE)
	{
		ForeRS.m_strFilter="NAME = '" + m_FName + "'";;
	}
	//���������������Ϊ�٣�������������Ϊ��
	else
	{
		ForeRS.m_strFilter="DOMITORY_ID = '" + m_VDomitory + "'";
	}
	//��ʾ
	ShowForeList(ForeRS);
}
