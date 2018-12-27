// DiaScreenshot.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "DiaScreenshot.h"
#include "afxdialogex.h"


// DiaScreenshot dialog

IMPLEMENT_DYNAMIC(DiaScreenshot, CDialogEx)

DiaScreenshot::DiaScreenshot(CCGWorkView* parent, CWnd* pParent /*=NULL*/)
: CDialogEx(DiaScreenshot::IDD, pParent), parent(parent)
{
	width = parent->scene.renderer.screen.Width();
	height = parent->scene.renderer.screen.Height();
}

DiaScreenshot::~DiaScreenshot()
{
}

void DiaScreenshot::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITWSCRN, edit_width);
	DDX_Control(pDX, IDC_EDITHSCRN, edit_height);
}


BEGIN_MESSAGE_MAP(DiaScreenshot, CDialogEx)
	ON_EN_CHANGE(IDC_EDITWSCRN, &DiaScreenshot::OnEnChangeEditwscrn)
	ON_EN_CHANGE(IDC_EDITHSCRN, &DiaScreenshot::OnEnChangeEdithscrn)
	ON_BN_CLICKED(IDOK, &DiaScreenshot::OnBnClickedOk)
END_MESSAGE_MAP()


// DiaScreenshot message handlers


void DiaScreenshot::OnEnChangeEditwscrn()
{
	CString str;
	edit_width.GetWindowTextW(str);
	if (!str.IsEmpty()) {
		try {
			width = (int)std::stof(str.GetString());
		}
		catch (...) {
			width = parent->scene.renderer.screen.Width();
		}
	}
}


void DiaScreenshot::OnEnChangeEdithscrn()
{
	CString str;
	edit_height.GetWindowTextW(str);
	if (!str.IsEmpty()) {
		try {
			height = (int)std::stof(str.GetString());
		}
		catch (...) {
			height = parent->scene.renderer.screen.Height();
		}
	}
}


BOOL DiaScreenshot::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;

	int h = height;
	str.Format(L"%d", h);
	edit_height.SetWindowText(str);

	int w = width;
	str.Format(L"%d", w);
	edit_width.SetWindowText(str);

	return TRUE;
}

void DiaScreenshot::OnBnClickedOk()
{
	parent->scene.screenshot(parent, width, height);
	CDialogEx::OnOK();
}
