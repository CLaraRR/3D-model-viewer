
// OpenGL3DTransform.h : OpenGL3DTransform Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// COpenGL3DTransformApp:
// �йش����ʵ�֣������ OpenGL3DTransform.cpp
//

class COpenGL3DTransformApp : public CWinAppEx
{
public:
	COpenGL3DTransformApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenGL3DTransformApp theApp;
