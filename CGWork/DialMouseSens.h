#pragma once


// DialMouseSens dialog
#include "CGWorkView.h"
class DialMouseSens : public CDialogEx
{
	DECLARE_DYNAMIC(DialMouseSens)

public:
	DialMouseSens(CCGWorkView* parent, CWnd* pParent = nullptr);   // standard constructor
	virtual ~DialMouseSens();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOUS_SENS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit edit_mouse_sensitivity;
	afx_msg void OnEnChangeEditMouseSens();
	float mouse_sensitivity;
	CCGWorkView* parent;

	BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
