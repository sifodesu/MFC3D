// PerspectiveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "PerspectiveDlg.h"
#include "afxdialogex.h"
#include <string>


// PerspectiveDlg dialog

IMPLEMENT_DYNAMIC(PerspectiveDlg, CDialog)

PerspectiveDlg::PerspectiveDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PERSPECTIVE_DLG, pParent)
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
