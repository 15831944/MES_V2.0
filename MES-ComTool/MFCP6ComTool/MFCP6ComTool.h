
// MFCP6ComTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCP6ComToolApp: 
// �йش����ʵ�֣������ MFCP6ComTool.cpp
//

class CMFCP6ComToolApp : public CWinApp
{
public:
	CMFCP6ComToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCP6ComToolApp theApp;