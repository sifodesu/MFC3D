// DialMouseSens.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "DialMouseSens.h"
#include "afxdialogex.h"
#include <string>

// DialMouseSens dialog

IMPLEMENT_DYNAMIC(DialMouseSens, CDialogEx)

DialMouseSens::DialMouseSens(CCGWorkView* parent, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MOUS_SENS, pParent), parent(parent)
{

	mouse_sensitivity = parent->mouse_sensitivity;
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
	ON_BN_CLICKED(IDOK, &DialMouseSens::OnBnClickedOk)
END_MESSAGE_MAP()


// DialMouseSens message handlers


void DialMouseSens::OnEnChangeEditMouseSens()
{
	CString str;
	edit_mouse_sensitivity.GetWindowTextW(str);
	if (!str.IsEmpty()) {
		try {
			mouse_sensitivity = std::stof(str.GetString());
		}
		catch (...) {
			mouse_sensitivity = parent->mouse_sensitivity;
		}
	}
}


BOOL DialMouseSens::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;
	float d = mouse_sensitivity;
	str.Format(L"%f", d);
	edit_mouse_sensitivity.SetWindowText(str);
	return TRUE;
}



void DialMouseSens::OnBnClickedOk()
{

	CDialogEx::OnOK();
}
