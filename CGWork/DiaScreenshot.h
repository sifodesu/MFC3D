#pragma once

#include "CGWorkView.h"
// DiaScreenshot dialog

class DiaScreenshot : public CDialogEx
{
	DECLARE_DYNAMIC(DiaScreenshot)

public:
	DiaScreenshot(CCGWorkView* parent, CWnd* pParent = NULL);   // standard constructor
	CCGWorkView* parent;
	int height;
	int width;
	CEdit edit_height;
	CEdit edit_width;
	virtual ~DiaScreenshot();

// Dialog Data
	enum { IDD = IDD_SCREENSHOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditwscrn();
	afx_msg void OnEnChangeEdithscrn();
	BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
