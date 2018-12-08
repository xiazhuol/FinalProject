
// FinalProj.h : main header file for the FinalProj application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CFinalProjApp:
// See FinalProj.cpp for the implementation of this class
//

class CFinalProjApp : public CWinApp
{
public:
	CFinalProjApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFinalProjApp theApp;
