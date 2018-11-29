// DiaFineNess.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "DiaFineNess.h"
#include "afxdialogex.h"

#include "iritSkel.h"
// DiaFineNess dialog

IMPLEMENT_DYNAMIC(DiaFineNess, CDialogEx)

DiaFineNess::DiaFineNess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIAFINENESS, pParent)
{
	fineNess = getFineNess();
}

DiaFineNess::~DiaFineNess()
{
}

void DiaFineNess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, edit_fineNess);
}


BEGIN_MESSAGE_MAP(DiaFineNess, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &DiaFineNess::OnEnChangeEdit1)
END_MESSAGE_MAP()


// DiaFineNess message handlers


void DiaFineNess::OnEnChangeEdit1()
{
	CString str;
	edit_fineNess.GetWindowTextW(str);
	if (!str.IsEmpty()) {
		try {
			fineNess = std::stof(str.GetString());
		}
		catch (...) {
			fineNess = getFineNess();
		}
	}
}


BOOL DiaFineNess::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;
	int d = fineNess;
	str.Format(L"%d", d);
	edit_fineNess.SetWindowText(str);
	return TRUE;
}