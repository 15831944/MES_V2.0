
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// JIMI.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

void CStringSplit(CString &strSource, CString *strDes, CString strSplitChar)
{
	int d_len = strSplitChar.GetLength();
	int j = 0;
	int n = 0;
	int m_pos;
	int iIndex;
	while (1)
	{
		m_pos = strSource.Find(strSplitChar, j);
		if (m_pos == -1 && j == 0)
		{
			iIndex = -1;
			break;
		}
		if (m_pos == -1 && j != 0)
		{
			strDes[n] = strSource.Mid(j, strSource.GetLength() - j);
			iIndex = n;
			break;
		}
		if (j == 0)
		{
			strDes[n] = strSource.Mid(0, m_pos);
			j = m_pos + d_len;
		}
		else
		{
			strDes[n] = strSource.Mid(j, m_pos - j);
			j = m_pos + d_len;
		}
		n++;
	}
}

