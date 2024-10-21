// StuInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "domitory.h"
#include "StuInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStuInfoDlg dialog
extern CDatabase Database;

CStuInfoDlg::CStuInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStuInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStuInfoDlg)
	m_StuName = FALSE;
	m_StuDomiID = FALSE;
	//}}AFX_DATA_INIT
}


void CStuInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStuInfoDlg)
	DDX_Control(pDX, IDC_STU_LIST, m_StuList);
	DDX_Check(pDX, IDC_STUDENT_NAME, m_StuName);
	DDX_Check(pDX, IDC_STU_DOMI_ID, m_StuDomiID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStuInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CStuInfoDlg)
	ON_BN_CLICKED(IDC_STU_RETRIEVE, OnStuRetrieve)
	ON_BN_CLICKED(IDC_ADD_STU_RECORD, OnAddStuRecord)
	ON_BN_CLICKED(IDC_DEL_STU_RECORD, OnDelStuRecord)
	ON_BN_CLICKED(IDC_CHANGE_STU_RECORD, OnChangeStuRecord)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStuInfoDlg message handlers

BOOL CStuInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_StuList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_StuList.InsertColumn(0,"ѧ�����",LVCFMT_LEFT,70);
	m_StuList.InsertColumn(1,"ѧ������",LVCFMT_LEFT,80);
	m_StuList.InsertColumn(2,"�꼶",LVCFMT_LEFT,60);
	m_StuList.InsertColumn(3,"�༶",LVCFMT_LEFT,60);
	m_StuList.InsertColumn(4,"�����",LVCFMT_LEFT,70);
	m_StuList.InsertColumn(5,"��ע",LVCFMT_LEFT,80);
    
	CStudentRS TempRS(&Database);
	ShowStuList(TempRS);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStuInfoDlg::ShowStuList(CStudentRS &StudentRS)
{
	int i=0;
	CString Str;
	StudentRS.Open();
	//
	m_StuList.DeleteAllItems();
	while (!StudentRS.IsEOF())
	{
		m_StuList.InsertItem(i," ");
		// 		Str.Format("%d",OnDutyRS.m_ON_DUTY_ID);
		m_StuList.SetItemText(i,0,StudentRS.m_STUDENT_ID);
		m_StuList.SetItemText(i,1,StudentRS.m_NAME);
		m_StuList.SetItemText(i,2,StudentRS.m_GRADE);
		m_StuList.SetItemText(i,3,StudentRS.m_CLASS);
		m_StuList.SetItemText(i,4,StudentRS.m_DOMITORY_ID);
		m_StuList.SetItemText(i,5,StudentRS.m_REMARKS);
		StudentRS.MoveNext();
		i++;
	}
	StudentRS.Close();
}

void CStuInfoDlg::OnStuRetrieve() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	//����ź�ѧ����������������Ϊ��
	if (m_StuName==FALSE && m_StuDomiID==FALSE)
	{
		MessageBox("��ѡ���������","��ʾ",MB_OK | MB_ICONINFORMATION);
		return;
	}
    //
	CStudentRS StudentRS(&Database);
	//��ȡ��������ź�ѧ������
	CString StrName,StrID;
	GetDlgItemText(IDC_STU_NAME,StrName);
	GetDlgItemText(IDC_STU_DOMITORY_ID,StrID);
	//����ź�ѧ����������������Ϊ��
	if (m_StuDomiID==TRUE && m_StuName==TRUE)
	{
		StudentRS.m_strFilter="NAME = '" + StrName + "' and DOMITORY_ID = '" + StrID + "'";  
	}
	//����ż�������Ϊ�棬ѧ��������������Ϊ��
	else if (m_StuDomiID==TRUE && m_StuName==FALSE)
	{
		StudentRS.m_strFilter="DOMITORY_ID = '" + StrID + "'";;
	}
	//ʱ���������Ϊ�٣�Ա����������Ϊ��
	else
	{
		StudentRS.m_strFilter="NAME = '" + StrName + "'";
	}
	//��ʾ
	ShowStuList(StudentRS);
}

void CStuInfoDlg::OnAddStuRecord() 
{
	// TODO: Add your control notification handler code here
	CAddStuReDlg AddDlg;
	if (AddDlg.DoModal()==IDOK)
	{
		CStudentRS StudentRS(&Database);
		ShowStuList(StudentRS);
	}
}

void CStuInfoDlg::OnChangeStuRecord() 
{
	// TODO: Add your control notification handler code here
	CAddStuReDlg ChangeDlg;
	ChangeDlg.m_Change=TRUE;
	//���öԻ����ʼ���ؼ��ı���ʾ
	int Index;
	CString Sql;
	POSITION pos=m_StuList.GetFirstSelectedItemPosition();
	if (pos==NULL)
	{
		return;
	}
	Index=m_StuList.GetNextSelectedItem(pos);
	ChangeDlg.m_StuID=m_StuList.GetItemText(Index,0);
	ChangeDlg.m_StuName=m_StuList.GetItemText(Index,1);
	ChangeDlg.m_StuGrade=m_StuList.GetItemText(Index,2);
	ChangeDlg.m_StuClass=m_StuList.GetItemText(Index,3);
	ChangeDlg.m_StuDomiID=m_StuList.GetItemText(Index,4);
	ChangeDlg.m_StuRemarks=m_StuList.GetItemText(Index,5);
	//
	if (ChangeDlg.DoModal()==IDOK)
	{
		CStudentRS StudentRS(&Database);
		ShowStuList(StudentRS);
	}
}

void CStuInfoDlg::OnDelStuRecord() 
{
	// TODO: Add your control notification handler code here
	CString StuDomiID; //����Ҫɾ��ѧ���������
	//ɾ��ѡ�е�ѧ����¼
	int Index;
	CString Sql;
	POSITION pos=m_StuList.GetFirstSelectedItemPosition();
	if (pos==NULL)
	{
		return;
	}
	Index=m_StuList.GetNextSelectedItem(pos);
	StuDomiID=m_StuList.GetItemText(Index,4);//����Ҫɾ��ѧ���������
	Sql="delete * from STUDENT where STUDENT_ID = '"+m_StuList.GetItemText(Index,0) + "'";
	Database.ExecuteSQL(Sql);
	CStudentRS StudentRS(&Database);
	ShowStuList(StudentRS);
	//���ݿ���DOMITORY����ѧ����Ŀ��һ
	CDomitoryRS DomitoryRS(&Database);
	DomitoryRS.m_strFilter="DOMITORY_ID = '" + StuDomiID +"'";
	DomitoryRS.Open();
	DomitoryRS.Edit();
	DomitoryRS.m_NUMBER--;
	DomitoryRS.Update();
	DomitoryRS.Close();
}

