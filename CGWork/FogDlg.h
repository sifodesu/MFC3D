#pragma once
#include "CGWorkView.h"

// FogDlg dialog

class FogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FogDlg)

public:
	FogDlg(CCGWorkView* parent, CWnd* pParent = nullptr);   // standard constructor
	virtual ~FogDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CCGWorkView* pview;
	CButton enable;
	CMFCColorButton color;
	CEdit intensity;
	CEdit near_bound;
	CEdit far_bound;

	BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CButton show_on_fog;
};
