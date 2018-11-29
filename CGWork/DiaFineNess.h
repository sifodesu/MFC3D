#pragma once

#include "CGWorkView.h"
// DiaFineNess dialog

class DiaFineNess : public CDialogEx
{
	DECLARE_DYNAMIC(DiaFineNess)

public:
	DiaFineNess(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DiaFineNess();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAFINENESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	int fineNess;
	CEdit edit_fineNess;


	BOOL OnInitDialog();
};
