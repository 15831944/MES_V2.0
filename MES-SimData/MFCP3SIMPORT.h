
// MFCP3SIMPORT.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCP3SIMPORTApp: 
// �йش����ʵ�֣������ MFCP3SIMPORT.cpp
//

class CMFCP3SIMPORTApp : public CWinApp
{
public:
	CMFCP3SIMPORTApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCP3SIMPORTApp theApp;