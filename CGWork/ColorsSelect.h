#pragma once

#include "CGWorkView.h"
// ColorsSelect dialog

class ColorsSelect : public CDialogEx
{
	DECLARE_DYNAMIC(ColorsSelect)

public:
	ColorsSelect(CCGWorkView* parent, CWnd* pParent = nullptr);   // standard constructor
	virtual ~ColorsSelect();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLORS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	COLORREF wrfrmClr;
	COLORREF nrmlsClr;
	COLORREF bckgrndClr;
	CCGWorkView* pview;
	afx_msg void OnBnClickedBwwrfrmclr();
	afx_msg void OnBnClickedBnrmlsclr();
	afx_msg void OnBnClickedBbckgrndclr();
};
