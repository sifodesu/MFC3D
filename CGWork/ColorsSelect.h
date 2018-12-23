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
	CCGWorkView* pview;
	virtual BOOL OnInitDialog();
	CMFCColorButton bg_picker;
	CMFCColorButton wireframe_picker;
	CMFCColorButton bbox_picker;
	CMFCColorButton normal_picker;
};
