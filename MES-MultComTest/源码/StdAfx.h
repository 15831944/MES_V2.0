// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__5A72C70D_404D_4C83_A51A_B10919679674__INCLUDED_)
#define AFX_STDAFX_H__5A72C70D_404D_4C83_A51A_B10919679674__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
extern "C" CString DeviceCountM;
extern "C" int ReIniCount;
#include <afxsock.h>		// MFC socket extensions

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

//����ADO���
//#import "D:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
//#import "C:\ado\msado15.dll" no_namespace rename("EOF","adoEOF")

extern CString UserNameDB;

//����һ��������
extern int g_ADCTFlag;//��ΪҪʵ������һ��Ե�ʣ�����������һ����־λ��ʵ���Զ���½���Զ���ȡ����
extern int g_IsHideFlag;//�Ƿ����ش���
extern int g_ToolFlag;//�ֹ�λ������
extern int g_ExitFlag;//�˳���־λ
extern int g_WaitTimeoutFlag;//�ȴ����ӳ�ʱ��־λ
extern CString g_TesterIdStr;//������ID

extern CString g_BackDoorIP;//������һ������
extern CString g_BackDoorDatabase;
extern CString g_BackDoorUser;
extern CString g_BackDoorPassword ;
extern CString g_BackDoorSocket;

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
//#import "E:\��������\�������\winxp\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__5A72C70D_404D_4C83_A51A_B10919679674__INCLUDED_)
