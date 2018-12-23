// ColorsSelect.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "ColorsSelect.h"
#include "afxdialogex.h"


// ColorsSelect dialog

IMPLEMENT_DYNAMIC(ColorsSelect, CDialogEx)

ColorsSelect::ColorsSelect(CCGWorkView* parent, CWnd* pParent)
	: CDialogEx(IDD_COLORS, pParent), pview(parent) {}

ColorsSelect::~ColorsSelect()
{
}

void ColorsSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BG_COLOR, bg_picker);
	DDX_Control(pDX, IDC_WIREFRAME_COLOR, wireframe_picker);
	DDX_Control(pDX, IDC_BBOX_COLOR, bbox_picker);
	DDX_Control(pDX, IDC_NORMAL_COLOR, normal_picker);
}


BEGIN_MESSAGE_MAP(ColorsSelect, CDialogEx)
	ON_BN_CLICKED(IDOK, &ColorsSelect::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL ColorsSelect::OnInitDialog()
{
	CDialog::OnInitDialog();
	bg_picker.SetColor(pview->scene.renderer.background_color);
	wireframe_picker.SetColor(pview->scene.renderer.wireframe_color);
	bbox_picker.SetColor(pview->scene.renderer.bbox_color);
	normal_picker.SetColor(pview->scene.renderer.normals_color);
	return TRUE;
}

void ColorsSelect::OnBnClickedOk()
{
	pview->scene.renderer.background_color = bg_picker.GetColor();
	pview->scene.renderer.wireframe_color = wireframe_picker.GetColor();
	pview->scene.renderer.normals_color = normal_picker.GetColor();
	pview->scene.renderer.bbox_color = bbox_picker.GetColor();
	CDialogEx::OnOK();
}
