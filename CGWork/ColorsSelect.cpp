// ColorsSelect.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "ColorsSelect.h"
#include "afxdialogex.h"


// ColorsSelect dialog

IMPLEMENT_DYNAMIC(ColorsSelect, CDialogEx)

ColorsSelect::ColorsSelect(CCGWorkView* parent, CWnd* pParent)
	: CDialogEx(IDD_COLORS, pParent), pview(parent)
{
	bckgrndClr = pview->scene.renderer.background_color;
	wrfrmClr = pview->scene.renderer.wireframe_color;
	nrmlsClr = pview->scene.renderer.normals_color;
}

ColorsSelect::~ColorsSelect()
{
}

void ColorsSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ColorsSelect, CDialogEx)
	ON_BN_CLICKED(IDOK, &ColorsSelect::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BWWRFRMCLR, &ColorsSelect::OnBnClickedBwwrfrmclr)
	ON_BN_CLICKED(IDC_BNRMLSCLR, &ColorsSelect::OnBnClickedBnrmlsclr)
	ON_BN_CLICKED(IDC_BBCKGRNDCLR, &ColorsSelect::OnBnClickedBbckgrndclr)
END_MESSAGE_MAP()


// ColorsSelect message handlers


void ColorsSelect::OnBnClickedOk()
{
	pview->scene.renderer.background_color = bckgrndClr;
	pview->scene.renderer.wireframe_color = wrfrmClr;
	pview->scene.renderer.normals_color = nrmlsClr;
	CDialogEx::OnOK();
}


void ColorsSelect::OnBnClickedBwwrfrmclr()
{
	CColorDialog dlg(wrfrmClr);
	dlg.DoModal();
	wrfrmClr = dlg.GetColor();
}


void ColorsSelect::OnBnClickedBnrmlsclr()
{
	CColorDialog dlg(nrmlsClr);
	dlg.DoModal();
	nrmlsClr = dlg.GetColor();
}


void ColorsSelect::OnBnClickedBbckgrndclr()
{
	CColorDialog dlg(bckgrndClr);
	dlg.DoModal();
	bckgrndClr = dlg.GetColor();
}
