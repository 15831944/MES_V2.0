
// MFCP4ECurrentTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCP4ECurrentToolApp: 
// �йش����ʵ�֣������ MFCP4ECurrentTool.cpp
//

class CMFCP4ECurrentToolApp : public CWinApp
{
public:
	CMFCP4ECurrentToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCP4ECurrentToolApp theApp;