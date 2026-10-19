
// Bai1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CBai1App:
// See Bai1.cpp for the implementation of this class
//

class CBai1App : public CWinApp
{
public:
	CBai1App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CBai1App theApp;
