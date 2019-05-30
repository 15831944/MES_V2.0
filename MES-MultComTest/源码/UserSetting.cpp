// UserSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WriteIMEI.h"
#include "UserSetting.h"


// CUserSetting �Ի���

IMPLEMENT_DYNAMIC(CUserSetting, CDialog)

CUserSetting::CUserSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CUserSetting::IDD, pParent)
	, UserName(_T(""))
	, Password(_T(""))
	, NewPassword(_T(""))
	, Limits(_T(""))
{

}

CUserSetting::~CUserSetting()
{
}

void CUserSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, UserName);
	DDX_Text(pDX, IDC_EDIT2, Password);
	DDX_Text(pDX, IDC_EDIT3, NewPassword);
	DDX_Text(pDX, IDC_EDIT10, Limits);
	DDX_Control(pDX, IDC_COMBO3, LimitsControl);
	DDX_Control(pDX, IDC_COMBO2, AllUsersBox);
}


BEGIN_MESSAGE_MAP(CUserSetting, CDialog)

	ON_WM_TIMER()//��ʱ��

	ON_BN_CLICKED(IDC_BUTTON1, &CUserSetting::OnBnClickedButtonstart1)
	ON_BN_CLICKED(IDC_BUTTON7, &CUserSetting::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CUserSetting::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON10, &CUserSetting::OnBnClickedButtonstart10)
	ON_BN_CLICKED(IDC_BUTTON11, &CUserSetting::OnBnClickedButtonstart11)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CUserSetting::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CUserSetting ��Ϣ�������

BOOL CUserSetting::OnInitDialog()
{
	CDialog::OnInitDialog();	
	if (ConnecDB(g_BackDoorIP, g_BackDoorDatabase, g_BackDoorUser, g_BackDoorPassword) == TRUE)
	//if (ConnecDB("192.168.0.240", "GPSTest", "sa", "sa123abc") != TRUE)
	{
		return FALSE;
	}
	GetUsersFromDB();
	for (int i = 0; i < UserArray.GetCount(); i++)
	{
		CString user = GetData((LPSTR)(LPCTSTR)UserArray.GetAt(i), "NULL", "@@", 1);	//����
		AllUsersBox.AddString(user);
	}
	//AllUsersBox.SetCurSel(0);
	UpdateData(FALSE);
	return TRUE;
}
CString CUserSetting::GetData(char* Serial_Order_Return, CString Start, CString End, int Count, int HandleNum)
{
	CString DataGet;
	//CString DataGetTemp;
	int pos = -1;
	CString Serial_Order_Return_CS;
	Serial_Order_Return_CS.Format("%s", Serial_Order_Return);
	for (int i = 0; i<Count; i++)
	{
		if (i == Count - 1)
		{
			if ((Start == "NULL") || (Start == ""))
			{
				pos = Serial_Order_Return_CS.Find(End);
				if (pos >= 0)
				{
					DataGet = Serial_Order_Return_CS.Left(pos);
				}
				else
				{
					return Serial_Order_Return_CS;
				}
			}
			else
			{
				pos = Serial_Order_Return_CS.Find(Start);
				if (pos >= 0)
				{
					Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + Start.GetLength());
					pos = Serial_Order_Return_CS.Find(End);
					if (pos >= 0)
					{
						DataGet = Serial_Order_Return_CS.Left(pos);
					}
					else
					{
						return Serial_Order_Return_CS;//return "NULL";
					}
				}
				else
				{
					return "NULL";
				}
			}
		}
		else
		{
			if ((Start == "NULL") || (Start == ""))
			{
				continue;
			}
			else
			{
				pos = Serial_Order_Return_CS.Find(Start);
				if (pos >= 0)
				{
					Serial_Order_Return_CS = Serial_Order_Return_CS.Mid(pos + Start.GetLength());
					continue;
				}
				else
				{
					return "NULL";
				}
			}
		}

		return DataGet;
	}
	return "NULL";
}

void CUserSetting::OnBnClickedButtonstart1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (ConnecDB(g_BackDoorIP, g_BackDoorDatabase, g_BackDoorUser, g_BackDoorPassword) == TRUE)
		//if (ConnecDB("192.168.0.240", "GPSTest", "sa", "sa123abc") != TRUE)
	{
		return;
	}
	UpdateUserToDb(UserName, Password, Limits);
}
BOOL CUserSetting::ConnecDB(CString m_server, CString m_db, CString m_user, CString m_pwd)
{
	if (1)//���ݿ��ַ��Ϊ�գ����ϴ�����
	{
		CString Conn = "";
		CString DB_FAIL = "";
		Conn.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", m_server, m_db, m_user, m_pwd);
		if (myado.m_pCon == NULL)
		{
			CoInitialize(NULL);
			DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);
		}
		if (myado.m_pCon->State == 0)  //1��ʾ�Ѿ���,0��ʾ�رգ����ݿ�����Ͽ�������
		{
			if (MessageBox("���ݿ⴦�ڶϿ�״̬��������", "��ܰȷ��", MB_OK | MB_OKCANCEL) == IDOK)
				DB_FAIL = myado.ConnecDataLibrary(Conn, "", "", adModeUnknown);//���ݿ�����Ͽ�������
			else
				return FALSE;
		}

		if (DB_FAIL == "FAIL")
		{
			AfxMessageBox("�������ݿ�ʧ�ܣ���������.......");
			return FALSE;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//MD5 mb;
		//CString CpuOrderTemp = "AT01a#^GT_CM=TEST";
		//mb.update(CpuOrderTemp, strlen(CpuOrderTemp));
		//CString CpuOrderMD5 = (mb.toString()).c_str();

	}
	return TRUE;
}
BOOL CUserSetting::GetUserType(CAdoInterface& myado, CString Name, CString Password, CString *Limits)
{
	_variant_t var;
	CString FieldName, FieldValue;
	try
	{
		myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'");
		//var= myado.m_pRec->GetCollect(_variant_t((long)5));
		FieldName.Format("UserType");
		var = myado.m_pRec->GetCollect((LPCTSTR)FieldName);
		if (var.vt != VT_NULL)
		{
			FieldValue.Format("%s", (LPCSTR)(_bstr_t)var);
			*Limits = FieldValue;
		}
		else
		{
			*Limits = "";
		}

	}
	catch (_com_error e)
	{
		*Limits = "";
		myado.CloseSheet();
		return FALSE;
	}
	myado.CloseSheet();
	return TRUE;
}

BOOL CUserSetting::UpdateUserToDb(CString Name, CString Password, CString Limits, BOOL ChangePw, CString NewPassword)
{
	if (1)//���ݿ��ַ��Ϊ�գ����ϴ�����
	{
		
		BOOL UP_Barcode = TRUE;
		BOOL Barcode_Check_UP = TRUE;

		if(LimitsSys.Find("&10") != -1)
			myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "'");
		else
			myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'");
		BOOL Barcode_Check = myado.Find("Name='" + Name + "'");
		myado.CloseSheet();
		if ((ChangePw==TRUE) && (NewPassword != "") && (Barcode_Check != TRUE))//�޸����뵫ԭʼ�������
		{
			AfxMessageBox("����������ȷԭʼ�˻����룡");
			return FALSE;
		}
		if (Barcode_Check == TRUE)
		{
			if (Limits == "")
			{
				AfxMessageBox("Ȩ������Ϊ�գ�");
				return FALSE;
			}
			CString Field="UserType";//�ֶ�

			CString strSQL_Write_Barcode;
			_variant_t var;
			if(LimitsSys.Find("&10") != -1)//��������Ȩ��,����������Ը���Ȩ������
			{
				myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "'");

				if ((ChangePw == TRUE) && (NewPassword != ""))
				{
					strSQL_Write_Barcode = "UPDATE LUserAccount SET " + Field + " = '" + Limits + "',Password = '" + NewPassword + "' WHERE Name='" + Name + "'";
				}
				else
				{
					strSQL_Write_Barcode = "UPDATE LUserAccount SET " + Field + " = '" + Limits + "' WHERE Name='" + Name + "'";
				}
			}
			else//�ǳ���������Ա,��������ɸ�������
			{
				myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'");

				if ((ChangePw == TRUE) && (NewPassword != ""))
				{
					strSQL_Write_Barcode = "UPDATE LUserAccount SET Password = '" + NewPassword + "' WHERE Name='" + Name + "' AND Password='" + Password + "'";
				}
			}
			//����
			
			UP_Barcode &= myado.Execute(strSQL_Write_Barcode, &var);

			myado.CloseSheet();
			if ((UP_Barcode != TRUE) || (var.intVal != 1))
			{
				AfxMessageBox("����ʧ�ܣ�");
				return FALSE;
			}
			//����
		}
		else
		{
			if(LimitsSys.Find("&10") == -1)//��ǰ�˺Ų�����,�賬�������˺�
			{
				AfxMessageBox("���˺�û�г�������Ȩ��");
				return FALSE;
			}
			//ֱ���ϴ�
			CString strSQL1 = "Insert into dbo.LUserAccount(Name,Password,UserType)\
							  							  				values('" + Name + "','" + Password + "','" + Limits + "')";


			myado.OpenSheet("select * from LUserAccount");

			_variant_t var;
			BOOL SqlResult = myado.Execute(strSQL1, &var);
			myado.CloseSheet();
			if ((SqlResult != TRUE) || (var.intVal != 1))
			{
				AfxMessageBox("����ʧ�ܣ�");
				return FALSE;
			}
			//ֱ���ϴ�

		}

		if ((UP_Barcode == TRUE&&Barcode_Check_UP == TRUE))
		{
			AfxMessageBox("���óɹ�������");
		}
		else
		{
			AfxMessageBox("����ʧ�ܣ�����");
		}

	}
	return TRUE;
}


void CUserSetting::OnBnClickedButton7()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (ConnecDB(g_BackDoorIP, g_BackDoorDatabase, g_BackDoorUser, g_BackDoorPassword) == TRUE)
	//if (ConnecDB("192.168.0.240", "GPSTest", "sa", "sa123abc") != TRUE)
	{
		return;
	}
	UpdateUserToDb(UserName, Password, Limits, TRUE, NewPassword);
}


void CUserSetting::OnBnClickedButton6()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (ConnecDB(g_BackDoorIP, g_BackDoorDatabase, g_BackDoorUser, g_BackDoorPassword) == TRUE)
		//if (ConnecDB("192.168.0.240", "GPSTest", "sa", "sa123abc") != TRUE)
	{
		return;
	}
	if(LimitsSys.Find("&10") == -1)
	{
		AfxMessageBox("���˺�û�г�������Ȩ��");
		return;
	}
	DeleteUserFromDb(UserName, Password,FALSE);
}


BOOL CUserSetting::DeleteUserFromDb(CString Name, CString Password,BOOL UsePW)
{
	if (UsePW==TRUE)//ʹ������
	{
		myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'");
		BOOL Barcode_Check = myado.Find("Name='" + Name + "'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)
		{
			_variant_t var;
			CString strSQL1 = "DELETE FROM LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'";
			BOOL SqlResult = myado.Execute(strSQL1, &var);
			if (SqlResult != TRUE)
			{
				AfxMessageBox("ɾ���û�ʧ�ܣ�");
				return FALSE;
			}
			else
			{
				AfxMessageBox("ɾ���û��ɹ���");
			}
		}
		else
		{
			AfxMessageBox("�û������ڣ�");
			return FALSE;
		}
	}
	else
	{
		myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "'");
		BOOL Barcode_Check = myado.Find("Name='" + Name + "'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)
		{
			_variant_t var;
			CString strSQL1 = "DELETE FROM LUserAccount WHERE Name='" + Name + "'";
			BOOL SqlResult = myado.Execute(strSQL1, &var);
			if (SqlResult != TRUE)
			{
				AfxMessageBox("ɾ���û�ʧ�ܣ�");
				return FALSE;
			}
			else
			{
				AfxMessageBox("ɾ���û��ɹ���");
			}
		}
		else
		{
			AfxMessageBox("�û������ڣ�");
			return FALSE;
		}
	}
	return TRUE;
}

void CUserSetting::OnBnClickedButtonstart10()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString LimitsCS;
	LimitsControl.GetWindowTextA(LimitsCS);
	if (LimitsCS != "")
	{
		if (Limits.Find(LimitsCS.Left(2)) == -1)
		{
			Limits += ("&" + LimitsCS.Left(2));
		}
	}
	UpdateData(FALSE);
}


BOOL CUserSetting::GetUsersFromDB()
{
	if (1)//���ݿ��ַ��Ϊ�գ����ϴ�����
	{
		myado.OpenSheet("select * from LUserAccount order by Name");

		CString temp;
		/////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////
		try
		{
			myado.m_pRec->MoveFirst(); //myado.m_pRec->MoveLast();
			do
			{
				try
				{
					temp = "";
					for (int aa = 0; aa < 3; aa++)
					{
						CString str1, str2;
						_variant_t varTemp;
						//�ж����ݿ��е�NULLֵ
						varTemp = myado.m_pRec->GetCollect(_variant_t((long)aa));
						if (varTemp.vt == VT_NULL)
							str1 = "";
						else
							str1.Format("%s", (LPCSTR)(_bstr_t)varTemp);

						temp += (str1 + "@@");

					}
					UserArray.Add(temp);
					if (myado.adoEOF() == 0)
					{
						myado.MoveNext();
						if (myado.adoEOF() == -1)
							break;
					}
					else
					{
						break;
					}
					/*if (myado.BOF() == 0)
					{
					myado.m_pRec->MovePrevious();
					if (myado.BOF() == -1)
					break;
					}
					else
					{
					break;
					}*/
				}
				catch (_com_error e)
				{
					CATCH_ERROR;
					myado.CloseSheet();
					return FALSE;
				}

			} while (myado.adoEOF() == 0);
		}
		catch (_com_error e)
		{
			//CATCH_ERROR;
			myado.CloseSheet();
			return FALSE;
		}
		myado.CloseSheet();
	}
	return TRUE;
}

void CUserSetting::OnBnClickedButtonstart11()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString LimitsCS;
	LimitsControl.GetWindowTextA(LimitsCS);
	if (LimitsCS != "")
	{
		if (Limits.Find(LimitsCS.Left(2)) != -1)
		{
			Limits.Replace("&" + LimitsCS.Left(2), "");
		}
	}
	UpdateData(FALSE);
}


void CUserSetting::OnCbnSelchangeCombo2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SetTimer(0, 100, NULL);
}
void CUserSetting::ChangeCombo()
{
	CString AllUsersBoxCS;
	AllUsersBox.GetWindowTextA(AllUsersBoxCS);
	for (int i = 0; i < UserArray.GetCount(); i++)
	{
		CString strValue = UserArray.GetAt(i);
		CString user = GetData((LPSTR)(LPCTSTR)strValue, "NULL", "@@", 1);	//�û�
		CString password = GetData((LPSTR)(LPCTSTR)strValue, "@@", "@@", 1);
		CString limits = GetData((LPSTR)(LPCTSTR)strValue, "@@", "@@", 2);
		if (user == AllUsersBoxCS)
		{
			UserName = user;
			Limits = limits;
			break;
		}
		
	}
	Password="";
	NewPassword="";
	UpdateData(FALSE);
}

BOOL CUserSetting::CheckUser(CString Name, CString Password, CString* Limits)
{
	if (1)//���ݿ��ַ��Ϊ�գ����ϴ�����
	{

		BOOL UP_Barcode = TRUE;
		BOOL Barcode_Check_UP = TRUE;

		myado.OpenSheet("select * from LUserAccount WHERE Name='" + Name + "' AND Password='" + Password + "'");
		BOOL Barcode_Check = myado.Find("Name='" + Name + "'");
		myado.CloseSheet();
		if (Barcode_Check == TRUE)//�����˺�
		{
			if (GetUserType(myado, Name, Password, Limits) == TRUE)
			{

			}
			else
			{
				return FALSE;
			}
				
		}
	}
	return TRUE;
}
void CUserSetting::OnTimer(UINT nIDEvent)//��ʱ��
{
	CString LineZhiDanCS;
	switch (nIDEvent)//��ʱ��ͬʱ�����󣬻���ִ����һ��������һ�����ظ�����,��ð취�Ǻϲ���ʱ��
	{
	case 0:
		KillTimer(0);
		ChangeCombo();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}