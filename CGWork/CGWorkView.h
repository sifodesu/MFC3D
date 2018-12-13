// CGWorkView.h : interface of the CCGWorkView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
#define AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "gl\gl.h"    // Include the standard CGWork  headers
#include "gl\glu.h"   // Add the utility library


#include "Light.h"
#include "Camera.h"
#include "Model.h"
#include "Mat.h"
#include <bitset>
#include <unordered_set>
#include <unordered_map>
#include "CGWorkDoc.h"

class CCGWorkView : public CView
{
public:
	class CRenderer
	{
	public:
		CCGWorkView* parent;
		BYTE* bitmap;
		CCamera camera;


		
		void set_pixel(POINT P, const vec3& v1, const vec3& v2, COLORREF color, bool forcePrint);
		void flood_fill(vector<vec3>& poly, vec2& p, COLORREF color);
		void draw_pixel(POINT p, COLORREF c);
		void draw_line(const vec3& v1, const vec3& v2, COLORREF color, bool forcePrint = false);
		void apply_perspective(vec4& v);
		vec3 cast(const vec3& v);

		bool draw_bbox;
		bool draw_polygon_normals;
		bool draw_vertice_normals;
		bool draw_polygon_included_normals;
		bool draw_vertice_included_normals;

		CRenderer(CCGWorkView* parent);
		~CRenderer();
		void set_bitmap_dimensions(const BITMAPINFO& info);
		void get_bitmap(CDC* context);
		void draw_bitmap(CDC* context);
		void set_camera(const CCamera& camera);
		void draw_model(const CModel& model);
		void draw_edges(const CModel& model);
		void draw_normals(const CModel& model);
		void draw_normal(const vec3& origin, const vec3& direction, COLORREF color);
		void draw_bounding_box(const CModel& model);

		CRect screen;
		std::bitset<3840 * 2160> bitFlag;
		std::vector<std::vector<float>> z_buffer;
		int mouse_x, mouse_y;
		bool select_highlighted_pol;
		COLORREF background_color;
		COLORREF highlight_polygon;
		COLORREF normals_color;
		COLORREF wireframe_color;
	};

	class CScene
	{
		vector<CCamera> cameras;
		int current_camera;

	public:
		CScene(CCGWorkView* parent);
		void add_model(const CModel& model);
		void add_camera(const CCamera& camera);
		void update_projection(int projection_type);
		CCamera& get_current_camera();

		void update(CCGWorkView* app, int mouse_dx);
		void draw(CDC* context);
		void drawZBuffer();
		
		CRenderer renderer;
		vector<CModel> models;
	};

protected: // create from serialization only
	CCGWorkView();
	DECLARE_DYNCREATE(CCGWorkView)

// Attributes
public:
	CCGWorkDoc* GetDocument();

// Operations
public:
	int mouse_pos_x;
	bool clicking;

	bool X;
	bool Y;
	bool Z;
	int m_nAction;				// Rotate, Translate, Scale
	int m_nView;				// Orthographic, perspective
	bool m_bIsPerspective;			// is the view perspective

	enum { TRANSFORM_MODEL, TRANSFORM_VIEW };
	int transform_context;

private:
	CString m_strItdFileName;		// file name of IRIT data

	int m_nLightShading;			// shading: Flat, Gouraud.

	double m_lMaterialAmbient;		// The Ambient in the scene
	double m_lMaterialDiffuse;		// The Diffuse in the scene
	double m_lMaterialSpecular;		// The Specular in the scene
	int m_nMaterialCosineFactor;		// The cosine factor for the specular

	LightParams m_lights[MAX_LIGHT];	//configurable lights array
	LightParams m_ambientLight;		//ambient light (only RGB is used)


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCGWorkView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCGWorkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL InitializeCGWork();
	BOOL SetupViewingFrustum(void);
	BOOL SetupViewingOrthoConstAspect(void);

	virtual void RenderScene();


	HGLRC    m_hRC;			// holds the Rendering Context
	CDC*     m_pDC;			// holds the Device Context
	int m_WindowWidth;		// hold the windows width
	int m_WindowHeight;		// hold the windows height
	double m_AspectRatio;		// hold the fixed Aspect Ration

	HBITMAP m_pDbBitMap;
	CDC* m_pDbDC;
	BITMAPINFO BMInfo;
	void update_draw_bitmap();

public:
	CScene scene;



// Generated message map functions
protected:
	//{{AFX_MSG(CCGWorkView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileLoad();
	afx_msg void OnViewOrthographic();
	afx_msg void OnUpdateViewOrthographic(CCmdUI* pCmdUI);
	afx_msg void OnViewPerspective();
	afx_msg void OnUpdateViewPerspective(CCmdUI* pCmdUI);
	afx_msg void OnActionRotate();
	afx_msg void OnUpdateActionRotate(CCmdUI* pCmdUI);
	afx_msg void OnActionScale();
	afx_msg void OnUpdateActionScale(CCmdUI* pCmdUI);
	afx_msg void OnActionTranslate();
	afx_msg void OnUpdateActionTranslate(CCmdUI* pCmdUI);
	afx_msg void OnAxisX();
	afx_msg void OnUpdateAxisX(CCmdUI* pCmdUI);
	afx_msg void OnAxisY();
	afx_msg void OnUpdateAxisY(CCmdUI* pCmdUI);
	afx_msg void OnAxisZ();
	afx_msg void OnUpdateAxisZ(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingFlat();
	afx_msg void OnUpdateLightShadingFlat(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingGouraud();
	afx_msg void OnUpdateLightShadingGouraud(CCmdUI* pCmdUI);
	afx_msg void OnLightConstants();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPolygonIncluded();
	afx_msg void OnVertexIncluded();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPolygonCalculated();
	afx_msg void OnVertexCalculated();
	afx_msg void OnUpdatePolygonIncluded(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePolygonCalculated(CCmdUI *pCmdUI);
	afx_msg void OnUpdateVertexIncluded(CCmdUI *pCmdUI);
	afx_msg void OnUpdateVertexCalculated(CCmdUI *pCmdUI);
	afx_msg void OnActionTransformobject();
	afx_msg void OnUpdateActionTransformobject(CCmdUI *pCmdUI);
	afx_msg void OnActionTransformview();
	afx_msg void OnUpdateActionTransformview(CCmdUI *pCmdUI);
	afx_msg void OnNormalPlaneToggle();
	afx_msg void OnUpdateNormalPlaneToggle(CCmdUI *pCmdUI);
	afx_msg void OnNormalVerticeToggle();
	afx_msg void OnUpdateNormalVerticeToggle(CCmdUI *pCmdUI);
	afx_msg void OnShowBbox();
	afx_msg void OnUpdateShowBbox(CCmdUI *pCmdUI);
	afx_msg void OnOptionsPerspectivecontrol();
//	afx_msg void OnEnChangeEditMouseSens();
	float mouse_sensitivity;
	afx_msg void OnOptionsMousesensitivity();
	afx_msg void OnOptionsColors();
	afx_msg void OnMfineness();
};

#ifndef _DEBUG  // debug version in CGWorkView.cpp
inline CCGWorkDoc* CCGWorkView::GetDocument()
   { return (CCGWorkDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
