// OnePlayer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// COnePlayerApp:
// �йش����ʵ�֣������ OnePlayer.cpp
//

class COnePlayerApp : public CWinApp
{
public:
	COnePlayerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern COnePlayerApp theApp;