
// MFCP1LD.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCP1LDApp: 
// �йش����ʵ�֣������ MFCP1LD.cpp
//

class CMFCP1LDApp : public CWinApp
{
public:
	CMFCP1LDApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCP1LDApp theApp;