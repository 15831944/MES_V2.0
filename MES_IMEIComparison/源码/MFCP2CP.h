
// MFCP2CP.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCP2CPApp: 
// �йش����ʵ�֣������ MFCP2CP.cpp
//

class CMFCP2CPApp : public CWinApp
{
public:
	CMFCP2CPApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCP2CPApp theApp;