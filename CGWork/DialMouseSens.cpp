// DialMouseSens.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "DialMouseSens.h"
#include "afxdialogex.h"
#include <string>

// DialMouseSens dialog

IMPLEMENT_DYNAMIC(DialMouseSens, CDialogEx)

DialMouseSens::DialMouseSens(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOUS_SENS, pParent)
{

	mouse_sensitivity = 0.0f;
}

DialMouseSens::~DialMouseSens()
{
}

void DialMouseSens::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MOUSE_SENS, edit_mouse_sensitivity);
}


BEGIN_MESSAGE_MAP(DialMouseSens, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_MOUSE_SENS, &DialMouseSens::OnEnChangeEditMouseSens)
END_MESSAGE_MAP()


// DialMouseSens message handlers


void DialMouseSens::OnEnChangeEditMouseSens()
{
	CString str;
	edit_mouse_sensitivity.GetWindowTextW(str);
	if (!str.IsEmpty()) {
		mouse_sensitivity = std::stof(str.GetString());
	}
}
