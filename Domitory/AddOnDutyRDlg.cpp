// AddOnDutyRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "domitory.h"
#include "AddOnDutyRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddOnDutyRDlg dialog
extern CDatabase Database;

CAddOnDutyRDlg::CAddOnDutyRDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddOnDutyRDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddOnDutyRDlg)
	m_AddOnDutyID = _T("");
	m_AddODName = _T("");
	m_AddODRemarks = _T("");
	m_Change=FALSE;
// 	m_CurSel1=0; //ָʾCComboBox�ؼ���ǰĬ��ѡ�еڼ���
// 	m_CurSel2=0;//ָʾCComboBox�ؼ���ǰĬ��ѡ�еڼ���
	//}}AFX_DATA_INIT
}


void CAddOnDutyRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddOnDutyRDlg)
	DDX_Control(pDX, IDC_COMBO2, m_ComBox2);
	DDX_Control(pDX, IDC_COMBO1, m_ComBox1);
	DDX_Text(pDX, IDC_ADD_O_D_ID, m_AddOnDutyID);
	DDX_Text(pDX, IDC_ADD_O_D_NAME, m_AddODName);
	DDX_Text(pDX, IDC_ADD_O_D_REMARKS, m_AddODRemarks);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddOnDutyRDlg, CDialog)
	//{{AFX_MSG_MAP(CAddOnDutyRDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddOnDutyRDlg message handlers

void CAddOnDutyRDlg::InitComBox()
{
	m_ComBox1.AddString("����");
	m_ComBox1.AddString("��һ");
	m_ComBox1.AddString("�ܶ�");
	m_ComBox1.AddString("����");
	m_ComBox1.AddString("����");
	m_ComBox1.AddString("����");
	m_ComBox1.AddString("����");
	m_ComBox1.SetCurSel(0);
	//
	m_ComBox2.AddString("����");
	m_ComBox2.AddString("ҹ��");
	m_ComBox2.SetCurSel(0);
}

BOOL CAddOnDutyRDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_Change==TRUE)
	{
		SetWindowText("�޸�ֵ���¼");
		SetDlgItemText(IDOK,"�޸�");
		GetDlgItem(IDC_ADD_O_D_ID)->EnableWindow(FALSE);
	}
	InitComBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddOnDutyRDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if (m_Change==FALSE)  //��Ӽ�¼
	{
		COnDutyRS OnDutyRS(&Database);
		CString Temp;
		//
		OnDutyRS.Open();
		OnDutyRS.AddNew();
		//
		OnDutyRS.m_ON_DUTY_ID=m_AddOnDutyID;
		//
		OnDutyRS.m_NAME=m_AddODName;
		//������Ӽ�¼�ܼ�ֵ��
		int CurSel=m_ComBox1.GetCurSel();
		CurSel++;
		OnDutyRS.m_DAY_OF_WEEK=CurSel;
		//������Ӽ�¼�����ҹ��ֵ��
		CString Str;
		CurSel=m_ComBox2.GetCurSel();
		if (CurSel==0)
		{
			Str="D";
		}
		if(CurSel==1)
		{
			Str="N";
		}
		OnDutyRS.m_DAY_NIGHT=Str;
		//
		OnDutyRS.m_REMARKS=m_AddODRemarks;
		//
		OnDutyRS.Update();
		OnDutyRS.Close();
	} 
	else                       //�޸ļ�¼
	{
		/*UpdateData(TRUE);*/
		COnDutyRS OnDutyRS(&Database);
		OnDutyRS.m_strFilter="ON_DUTY_ID = '" + m_AddOnDutyID + "'";
		OnDutyRS.Open();
		OnDutyRS.Edit();
		//�޸�ֵ
		OnDutyRS.m_NAME=m_AddODName;
		//������Ӽ�¼�ܼ�ֵ��
		int CurSel=m_ComBox1.GetCurSel();
		CurSel++;
		OnDutyRS.m_DAY_OF_WEEK=CurSel;
		//������Ӽ�¼�����ҹ��ֵ��
		CString Str;
		CurSel=m_ComBox2.GetCurSel();
		if (CurSel==0)
		{
			Str="D";
		}
		if(CurSel==1)
		{
			Str="N";
		}
		OnDutyRS.m_DAY_NIGHT=Str;
		OnDutyRS.m_REMARKS=m_AddODRemarks;
		//
		OnDutyRS.Update();
		OnDutyRS.Close();
	}
	CDialog::OnOK();
}
