// PerspectiveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "PerspectiveDlg.h"
#include "afxdialogex.h"
#include "Globals.h"
#include <string>


// PerspectiveDlg dialog

IMPLEMENT_DYNAMIC(PerspectiveDlg, CDialog)

PerspectiveDlg::PerspectiveDlg(CWnd* pParent /*=nullptr*/, float depth)
	: CDialog(IDD_PERSPECTIVE_DLG, pParent), depth(depth * DEPTH_FACTOR)
{

}

PerspectiveDlg::~PerspectiveDlg()
{
}

void PerspectiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEPTH_CONTROL, edit_depth);
}


BEGIN_MESSAGE_MAP(PerspectiveDlg, CDialog)
	ON_EN_CHANGE(IDC_DEPTH_CONTROL, &PerspectiveDlg::OnEnChangeDepthControl)
END_MESSAGE_MAP()


// PerspectiveDlg message handlers


void PerspectiveDlg::OnEnChangeDepthControl()
{
	CString str;
	edit_depth.GetWindowTextW(str);
	if (!str.IsEmpty()) {
		depth = std::stof(str.GetString());
	}
}

BOOL PerspectiveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;
	str.Format(L"%d", depth);
	edit_depth.SetWindowText(str);
	return TRUE;
}
