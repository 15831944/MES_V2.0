#pragma once
//#pragma warning (push)
//#pragma warning(disable:C4018)
#include "DataBelong.h"
#include <vector>
using namespace std;
//#include "DlgSet.h"

#if !defined CATCH_ERROR
#define CATCH_ERROR														\
{																\
	CString strComError;										\
	strComError.Format("������: %08lx\n������Ϣ: %s			\
	\n����Դ: %s\n��������: %s",			\
	e.Error(),								\
	e.ErrorMessage(),						\
	(LPCSTR) e.Source(),					\
	(LPCSTR) e.Description());				\
	::MessageBox(NULL,strComError,"����",MB_ICONEXCLAMATION);	\
}
#endif
class CAdoInterface
{	
public:
	//��ȡ_ConnectionPtr��_RecordsetPtr
	_ConnectionPtr& GetConnPtr()    {return m_pCon;}
	_RecordsetPtr& GetRecoPtr()     {return m_pRec;}

	//����ĳ�����ݿ�
	CString ConnecDataLibrary(LPCTSTR ConnStr,LPCTSTR UserID,
		LPCTSTR PassWord,long Options=adModeUnknown);
	//�ؿ�
	void CloseDataLibrary();
	//��ĳ�ַ�ʽ�򿪱� 
	void OpenSheet(LPCTSTR Sql,
		enum CursorTypeEnum CursorType=adOpenDynamic,
		enum LockTypeEnum LockType=adLockOptimistic,
		long Options=adCmdText);
	//�ر�
	void CloseSheet();	

	//ͷ
	BOOL BOF()          {return m_pRec->BOF;}
	//β
	BOOL adoEOF()       {return m_pRec->adoEOF;}
	//��һ����¼
	void MoveFirst()    {m_pRec->MoveFirst();}
	//���һ����¼
	void MoveLast()     {m_pRec->MoveLast();}
	//�����ƶ�һ����¼
	void MoveNext()     {m_pRec->MoveNext();}
	//�����ƶ�һ����¼
	void MovePrevious() {m_pRec->MovePrevious();}
	//�ƶ���ָ��λ��
	void Move(long Num) {m_pRec->Move(Num);}

	//ִ��SQL���
	_RecordsetPtr Execute(LPCTSTR strSQL, VARIANT * RecordsAffected=NULL, long lOptions = adCmdText);	

	//��ȡ�ֶ�����
	BOOL GetCollect(LPCTSTR Name,_variant_t& OutCol);
	
	//---------------------��ֵ����--------------------------------
	//����һ���µļ�¼
	void AddNewRecode(AddNewCode* code,long ColCount);
	//ɾ��һ����¼
	void Delet(enum AffectEnum AffectRecords=adAffectCurrent);
	//��ȡһ����¼����
	void GetOneRecord(_variant_t* ColName,long ColCount,
		                                 _variant_t* OutValue);
	//����
	BOOL Find(LPCTSTR lpszFind, 
		SearchDirectionEnum SearchDirection = adSearchForward);
	//������һ��
	BOOL FindNext();
	//����
	BOOL Sort(LPCTSTR lpszSort);
	//����
	BOOL Filter(LPCTSTR lpszFilter);	

	//----------------------��չ��װ����------------------------
	//����һ���µļ�¼
	void AddNewRecodeEx(CString ColName, ... );
	//��ȡһ���µļ�¼
	void GetOneRecordEx(CString ColName, ... );

public:
	CString DB_FAIL;
	BOOL UP_FAIL;
	_ConnectionPtr       m_pCon;
	_RecordsetPtr        m_pRec;

private:
	//_ConnectionPtr       m_pCon;
	//_RecordsetPtr        m_pRec;
	CString              m_strFind;
	SearchDirectionEnum  m_SearchDirection;
    //-----------------------�㷨-----------------------
	void Ufo(CString InStr,vector<CString>& OutStr);	
	void GetNameandType(CString ColName,vector<FieldInfor>& OutVnt);	
};
