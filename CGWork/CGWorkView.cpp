// CGWorkView.cpp : implementation of the CCGWorkView class
//
#include "stdafx.h"
#include "CGWork.h"

extern CCGWorkApp theApp;

#include "CGWorkDoc.h"
#include "CGWorkView.h"

#include <iostream>
using std::cout;
using std::endl;
#include "MaterialDlg.h"
#include "LightDialog.h"
#include "PerspectiveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PngWrapper.h"
#include "iritSkel.h"
#include "Globals.h"

// For Status Bar access
#include "MainFrm.h"

#include <set>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include "DialMouseSens.h"
#include "ColorsSelect.h"

// Use this macro to display text messages in the status bar.
#define STATUS_BAR_TEXT(str) (((CMainFrame*)GetParentFrame())->getStatusBar().SetWindowText(str))


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView

IMPLEMENT_DYNCREATE(CCGWorkView, CView)

BEGIN_MESSAGE_MAP(CCGWorkView, CView)
	//{{AFX_MSG_MAP(CCGWorkView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_VIEW_ORTHOGRAPHIC, OnViewOrthographic)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORTHOGRAPHIC, OnUpdateViewOrthographic)
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, OnUpdateViewPerspective)
	ON_COMMAND(ID_ACTION_ROTATE, OnActionRotate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_ROTATE, OnUpdateActionRotate)
	ON_COMMAND(ID_ACTION_SCALE, OnActionScale)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SCALE, OnUpdateActionScale)
	ON_COMMAND(ID_ACTION_TRANSLATE, OnActionTranslate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSLATE, OnUpdateActionTranslate)
	ON_COMMAND(ID_AXIS_X, OnAxisX)
	ON_UPDATE_COMMAND_UI(ID_AXIS_X, OnUpdateAxisX)
	ON_COMMAND(ID_AXIS_Y, OnAxisY)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Y, OnUpdateAxisY)
	ON_COMMAND(ID_AXIS_Z, OnAxisZ)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Z, OnUpdateAxisZ)
	ON_COMMAND(ID_LIGHT_SHADING_FLAT, OnLightShadingFlat)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_FLAT, OnUpdateLightShadingFlat)
	ON_COMMAND(ID_LIGHT_SHADING_GOURAUD, OnLightShadingGouraud)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_GOURAUD, OnUpdateLightShadingGouraud)
	ON_COMMAND(ID_LIGHT_CONSTANTS, OnLightConstants)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_COMMAND(ID_POLYGON_INCLUDED, &CCGWorkView::OnPolygonIncluded)
	ON_COMMAND(ID_VERTEX_INCLUDED, &CCGWorkView::OnVertexIncluded)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_POLYGON_CALCULATED, &CCGWorkView::OnPolygonCalculated)
	ON_COMMAND(ID_VERTEX_CALCULATED, &CCGWorkView::OnVertexCalculated)
	ON_UPDATE_COMMAND_UI(ID_POLYGON_INCLUDED, &CCGWorkView::OnUpdatePolygonIncluded)
	ON_UPDATE_COMMAND_UI(ID_POLYGON_CALCULATED, &CCGWorkView::OnUpdatePolygonCalculated)
	ON_UPDATE_COMMAND_UI(ID_VERTEX_INCLUDED, &CCGWorkView::OnUpdateVertexIncluded)
	ON_UPDATE_COMMAND_UI(ID_VERTEX_CALCULATED, &CCGWorkView::OnUpdateVertexCalculated)
	ON_COMMAND(ID_ACTION_TRANSFORMOBJECT, &CCGWorkView::OnActionTransformobject)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSFORMOBJECT, &CCGWorkView::OnUpdateActionTransformobject)
	ON_COMMAND(ID_ACTION_TRANSFORMVIEW, &CCGWorkView::OnActionTransformview)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSFORMVIEW, &CCGWorkView::OnUpdateActionTransformview)
	ON_COMMAND(ID_NORMAL_PLANE_TOGGLE, &CCGWorkView::OnNormalPlaneToggle)
	ON_UPDATE_COMMAND_UI(ID_NORMAL_PLANE_TOGGLE, &CCGWorkView::OnUpdateNormalPlaneToggle)
	ON_COMMAND(ID_NORMAL_VERTICE_TOGGLE, &CCGWorkView::OnNormalVerticeToggle)
	ON_UPDATE_COMMAND_UI(ID_NORMAL_VERTICE_TOGGLE, &CCGWorkView::OnUpdateNormalVerticeToggle)
	ON_COMMAND(ID_SHOW_BBOX, &CCGWorkView::OnShowBbox)
	ON_UPDATE_COMMAND_UI(ID_SHOW_BBOX, &CCGWorkView::OnUpdateShowBbox)
	ON_COMMAND(ID_OPTIONS_PERSPECTIVECONTROL, &CCGWorkView::OnOptionsPerspectivecontrol)
	ON_COMMAND(ID_OPTIONS_MOUSESENSITIVITY, &CCGWorkView::OnOptionsMousesensitivity)
	ON_COMMAND(ID_OPTIONS_COLORS, &CCGWorkView::OnOptionsColors)
END_MESSAGE_MAP()


// A patch to fix GLaux disappearance from VS2005 to VS2008
void auxSolidCone(GLdouble radius, GLdouble height) {
	GLUquadric *quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	gluCylinder(quad, radius, 0.0, height, 20, 20);
	gluDeleteQuadric(quad);
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView construction/destruction

CCGWorkView::CCGWorkView() :
	scene(this)
{
	// Set default values
	m_nAxis = ID_AXIS_X;
	m_nAction = ID_ACTION_ROTATE;
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	transform_context = TRANSFORM_MODEL;
	clicking = false;

	m_nLightShading = ID_LIGHT_SHADING_FLAT;

	m_lMaterialAmbient = 0.2;
	m_lMaterialDiffuse = 0.8;
	m_lMaterialSpecular = 1.0;
	m_nMaterialCosineFactor = 32;

	//init the first light to be enabled
	m_lights[LIGHT_ID_1].enabled = true;
	m_pDbBitMap = NULL;
	m_pDbDC = NULL;
	mouse_sensitivity = 0.5f;
}

CCGWorkView::~CCGWorkView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView diagnostics

#ifdef _DEBUG
void CCGWorkView::AssertValid() const
{
	CView::AssertValid();
}

void CCGWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGWorkDoc* CCGWorkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGWorkDoc)));
	return (CCGWorkDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView Window Creation - Linkage of windows to CGWork

BOOL CCGWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// An CGWork window must be created with the following
	// flags and must NOT include CS_PARENTDC for the
	// class style.

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}



int CCGWorkView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitializeCGWork();

	return 0;
}


// This method initialized the CGWork system.
BOOL CCGWorkView::InitializeCGWork()
{
	m_pDC = new CClientDC(this);

	if (NULL == m_pDC) { // failure to get DC
		::AfxMessageBox(CString("Couldn't get a valid DC."));
		return FALSE;
	}

	CRect r;
	GetClientRect(&r);
	m_pDbDC = new CDC();
	m_pDbDC->CreateCompatibleDC(m_pDC);
	SetTimer(1, 1, NULL);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);
	m_pDbDC->SelectObject(m_pDbBitMap);

	// Code
	update_draw_bitmap();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView message handlers


void CCGWorkView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy) {
		return;
	}

	// save the width and height of the current window
	m_WindowWidth = cx;
	m_WindowHeight = cy;

	// compute the aspect ratio
	// this will keep all dimension scales equal
	m_AspectRatio = (GLdouble)m_WindowWidth / (GLdouble)m_WindowHeight;

	CRect r;
	GetClientRect(&r);
	DeleteObject(m_pDbBitMap);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);
	m_pDbDC->SelectObject(m_pDbBitMap);
	update_draw_bitmap();
}


BOOL CCGWorkView::SetupViewingFrustum(void)
{
	return TRUE;
}


// This viewing projection gives us a constant aspect ration. This is done by
// increasing the corresponding size of the ortho cube.
BOOL CCGWorkView::SetupViewingOrthoConstAspect(void)
{
	return TRUE;
}

void CCGWorkView::update_draw_bitmap()
{
	BMInfo = { 0 };
	BMInfo.bmiHeader.biSize = sizeof(BMInfo.bmiHeader);
	GetDIBits(*m_pDbDC, m_pDbBitMap, 0, 0, nullptr, &BMInfo, DIB_RGB_COLORS);
	BMInfo.bmiHeader.biBitCount = 32;
	BMInfo.bmiHeader.biCompression = BI_RGB;
	BMInfo.bmiHeader.biHeight = abs(BMInfo.bmiHeader.biHeight);
	scene.renderer.set_bitmap_dimensions(BMInfo);
}

BOOL CCGWorkView::OnEraseBkgnd(CDC* pDC)
{
	// Windows will clear the window with the background color every time your window 
	// is redrawn, and then CGWork will clear the viewport with its own background color.


	return true;
}



/////////////////////////////////////////////////////////////////////////////
// CCGWorkView drawing
/////////////////////////////////////////////////////////////////////////////

void CCGWorkView::OnDraw(CDC* pDC)
{
	static float theta = 0.0f;
	CCGWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect r;
	GetClientRect(&r);
	CDC *pDCToUse = /*m_pDC*/m_pDbDC;

	POINT mouse;
	if (GetCursorPos(&mouse) && clicking) {
		scene.update(this, mouse.x - mouse_pos_x);
	}
	mouse_pos_x = mouse.x;

	//pDC->GetWindow()->EnableWindow();
	//ON_UPDATE_COMMAND_UI(IDD_MOUS_SENS, OnOptionsMousesensitivity);
	scene.draw(pDCToUse);
	m_pDC->BitBlt(r.left, r.top, r.Width(), r.Height(), pDCToUse, r.left, r.top, SRCCOPY);

	theApp.OnIdle(-1);
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView CGWork Finishing and clearing...

void CCGWorkView::OnDestroy()
{
	CView::OnDestroy();

	// delete the DC
	if (m_pDC) {
		delete m_pDC;
	}

	if (m_pDbDC) {
		delete m_pDbDC;
	}
}



/////////////////////////////////////////////////////////////////////////////
// User Defined Functions

void CCGWorkView::RenderScene() {
	// do nothing. This is supposed to be overriden...

	return;
}


void CCGWorkView::OnFileLoad()
{
	TCHAR szFilters[] = _T("IRIT Data Files (*.itd)|*.itd|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("itd"), _T("*.itd"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK) {
		reset_current_model();
		m_strItdFileName = dlg.GetPathName();		// Full path and filename
		PngWrapper p;
		CGSkelProcessIritDataFiles(m_strItdFileName, 1);
		CModel model = get_current_model();
		model.setup_model();
		scene.add_model(model);
		// Open the file and read it.
		// Your code here...

		Invalidate();	// force a WM_PAINT for drawing.
	}

}





// VIEW HANDLERS ///////////////////////////////////////////

// Note: that all the following Message Handlers act in a similar way.
// Each control or command has two functions associated with it.

void CCGWorkView::OnViewOrthographic()
{
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	scene.update_projection(m_nView);
	Invalidate();		// redraw using the new view.
}

void CCGWorkView::OnUpdateViewOrthographic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_ORTHOGRAPHIC);
}

void CCGWorkView::OnViewPerspective()
{
	m_nView = ID_VIEW_PERSPECTIVE;
	m_bIsPerspective = true;
	scene.update_projection(m_nView);
	Invalidate();
}

void CCGWorkView::OnUpdateViewPerspective(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_PERSPECTIVE);
}




// ACTION HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnActionRotate()
{
	m_nAction = ID_ACTION_ROTATE;
}

void CCGWorkView::OnUpdateActionRotate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_ROTATE);
}

void CCGWorkView::OnActionTranslate()
{
	m_nAction = ID_ACTION_TRANSLATE;
}

void CCGWorkView::OnUpdateActionTranslate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_TRANSLATE);
}

void CCGWorkView::OnActionScale()
{
	m_nAction = ID_ACTION_SCALE;
}

void CCGWorkView::OnUpdateActionScale(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_SCALE);
}




// AXIS HANDLERS ///////////////////////////////////////////


// Gets calles when the X button is pressed or when the Axis->X menu is selected.
// The only thing we do here is set the ChildView member variable m_nAxis to the 
// selected axis.
void CCGWorkView::OnAxisX()
{
	m_nAxis = ID_AXIS_X;
}

// Gets called when windows has to repaint either the X button or the Axis pop up menu.
// The control is responsible for its redrawing.
// It sets itself disabled when the action is a Scale action.
// It sets itself Checked if the current axis is the X axis.
void CCGWorkView::OnUpdateAxisX(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_X);
}

void CCGWorkView::OnAxisY()
{
	m_nAxis = ID_AXIS_Y;
}

void CCGWorkView::OnUpdateAxisY(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Y);
}


void CCGWorkView::OnAxisZ()
{
	m_nAxis = ID_AXIS_Z;
}

void CCGWorkView::OnUpdateAxisZ(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Z);
}

// LIGHT SHADING HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnLightShadingFlat()
{
	m_nLightShading = ID_LIGHT_SHADING_FLAT;
}

void CCGWorkView::OnUpdateLightShadingFlat(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_FLAT);
}


void CCGWorkView::OnLightShadingGouraud()
{
	m_nLightShading = ID_LIGHT_SHADING_GOURAUD;
}

void CCGWorkView::OnUpdateLightShadingGouraud(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_GOURAUD);
}

// LIGHT SETUP HANDLER ///////////////////////////////////////////

void CCGWorkView::OnLightConstants()
{
	CLightDialog dlg;

	for (int id = LIGHT_ID_1; id < MAX_LIGHT; id++)
	{
		dlg.SetDialogData((LightID)id, m_lights[id]);
	}
	dlg.SetDialogData(LIGHT_ID_AMBIENT, m_ambientLight);

	if (dlg.DoModal() == IDOK)
	{
		for (int id = LIGHT_ID_1; id < MAX_LIGHT; id++)
		{
			m_lights[id] = dlg.GetDialogData((LightID)id);
		}
		m_ambientLight = dlg.GetDialogData(LIGHT_ID_AMBIENT);
	}
	Invalidate();
}

void CCGWorkView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnTimer(nIDEvent);
	if (nIDEvent == 1)
		Invalidate();
}

CCGWorkView::CRenderer::CRenderer(CCGWorkView* parent) :
	parent(parent), bitmap(nullptr),
	background_color(BLACK),
	highlight_polygon(GREEN),
	normals_color(PINK),
	wireframe_color(WHITE)
{
	draw_bounding_box = false;
	draw_polygon_normals = false;
	draw_vertice_normals = false;
	draw_polygon_included_normals = true;
	draw_vertice_included_normals = true;
	select_highlighted_pol = false;
}

CCGWorkView::CRenderer::~CRenderer()
{
	if (bitmap != nullptr) {
		delete[] bitmap;
	}
}

void CCGWorkView::CRenderer::draw_pixel(POINT p, COLORREF c)
{
	int offset = 4 * ((screen.Height() - p.y) * screen.Width() + p.x);
	if (offset > parent->BMInfo.bmiHeader.biSizeImage || offset < 0 ||
		p.x >= screen.Width() || p.x < 0) {
		return;
	}
	bitmap[offset] = GetBValue(c);
	bitmap[offset + 1] = GetGValue(c);
	bitmap[offset + 2] = GetRValue(c);
}

void CCGWorkView::CRenderer::draw_line(const vec2& v1, const vec2& v2, COLORREF color, bool forcePrint)
{
	int x1 = (int)v1.x, x2 = (int)v2.x, y1 = (int)v1.y, y2 = (int)v2.y;
	if (x2 - x1 < 0) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	int dx = x2 - x1, dy = y2 - y1, d, delta_e, delta_ne;

	POINT p;
	p.x = x1;
	p.y = y1;

	if (p.x >= 0 && p.y >= 0 && p.x < 3840 && p.y < 2160)
		if (!bitemap.test(p.x + p.y * 3840) || forcePrint) {
			bitemap.set(p.x + p.y * 3840);
			draw_pixel(p, color);
		}

	if (dy >= 0) {
		if (dx >= dy) {
			d = 2 * dy - dx;
			delta_e = 2 * dy;
			delta_ne = 2 * (dy - dx);
			while (p.x < x2) {
				if (d < 0) {
					d += delta_e;
					p.x++;
				}
				else {
					d += delta_ne;
					p.x++;
					p.y++;
				}
				if (p.x >= 0 && p.y >= 0 && p.x < 3840 && p.y < 2160)
					if (!bitemap.test(p.x + p.y * 3840) || forcePrint) {
						bitemap.set(p.x + p.y * 3840);
						draw_pixel(p, color);
					}

			}
		}
		else {
			d = 2 * dx - dy;
			delta_e = 2 * dx;
			delta_ne = 2 * (dx - dy);
			while (p.y < y2) {
				if (d < 0) {
					d += delta_e;
					p.y++;
				}
				else {
					d += delta_ne;
					p.x++;
					p.y++;
				}
				if (p.x >= 0 && p.y >= 0 && p.x < 3840 && p.y < 2160)
					if (!bitemap.test(p.x + p.y * 3840) || forcePrint) {
						bitemap.set(p.x + p.y * 3840);
						draw_pixel(p, color);
					}

			}
		}
	}
	else {
		if (dx >= -dy) {
			d = 2 * dy + dx;
			delta_e = 2 * dy;
			delta_ne = 2 * (dy + dx);
			while (p.x < x2) {
				if (d > 0) {
					d += delta_e;
					p.x++;
				}
				else {
					d += delta_ne;
					p.x++;
					p.y--;
				}
				if (p.x >= 0 && p.y >= 0 && p.x < 3840 && p.y < 2160)
					if (!bitemap.test(p.x + p.y * 3840) || forcePrint) {
						bitemap.set(p.x + p.y * 3840);
						draw_pixel(p, color);
					}
			}
		}
		else {
			d = 2 * dx + dy;
			delta_e = 2 * dx;
			delta_ne = 2 * (dx + dy);
			while (p.y > y2) {
				if (d < 0) {
					d += delta_e;
					p.y--;
				}
				else {
					d += delta_ne;
					p.x++;
					p.y--;
				}
				if (p.x >= 0 && p.y >= 0 && p.x < 3840 && p.y < 2160)
					if (!bitemap.test(p.x + p.y * 3840) || forcePrint) {
						bitemap.set(p.x + p.y * 3840);
						draw_pixel(p, color);
					}
			}
		}
	}
}

void CCGWorkView::CRenderer::apply_perspective(vec4 & v)
{
	float f = 1.0f / v.w;
	v = scaling(f, f, f) * v;
}

vec2 CCGWorkView::CRenderer::cast(const vec2& v)
{
	int width = screen.Width(), height = screen.Height();
	int scale = min(height, width);
	vec2 v2(v.x * scale + width / 2, -v.y * scale + height / 2);
	return v2;
}

void CCGWorkView::CRenderer::set_bitmap_dimensions(const BITMAPINFO& info)
{
	if (bitmap != nullptr) {
		delete[] bitmap;
	}
	bitmap = new BYTE[info.bmiHeader.biSizeImage];
	parent->GetClientRect(&screen);
}

void CCGWorkView::CRenderer::get_bitmap(CDC * context)
{
	context->FillSolidRect(&screen, background_color);
	int i = GetDIBits(*context, parent->m_pDbBitMap, 0, parent->BMInfo.bmiHeader.biHeight,
		bitmap, &(parent->BMInfo), DIB_RGB_COLORS);
}

void CCGWorkView::CRenderer::draw_bitmap(CDC * context)
{
	int i = SetDIBits(*context, parent->m_pDbBitMap, 0, parent->BMInfo.bmiHeader.biHeight,
		bitmap, &(parent->BMInfo), DIB_RGB_COLORS);
}

void CCGWorkView::CRenderer::set_camera(const CCamera & camera)
{
	this->camera = camera;
}

void CCGWorkView::CRenderer::draw_normal(const vec3& startPoint, const vec3& givenNormal, mat4 transform, COLORREF color) {
	vec3 sourceNormal = startPoint + normalized(givenNormal)*0.02f;

	vec4 a = transform * vec4(sourceNormal.x, sourceNormal.y, sourceNormal.z, 1.0f);
	vec4 b = transform * vec4(startPoint.x, startPoint.y, startPoint.z, 1.0f);

	if (!camera.is_orthographic()) {
		apply_perspective(a);
		apply_perspective(b);
	}

	vec2 newSource = cast(vec2(a));
	vec2 newStart = cast(vec2(b));

	draw_line(newStart, newSource, color);

}

bool CCGWorkView::CRenderer::check_if_drawn(const vec2& startPoint, const vec2& endPoint, std::unordered_set<edge>& current_set) {
	return false;
	edge tempEdge(startPoint, endPoint);
	if (current_set.find(tempEdge) == current_set.end()) {
		current_set.insert(tempEdge);
		return false;
	}
	return true;
}


bool pointIsInside(vector<vec2> points, vec2 dot) {

	int i, j = points.size() - 1;
	bool res = false;

	for (i = 0; i < points.size(); j = i, i++) {
		if (points[i].y < dot.y && points[j].y >= dot.y || points[j].y < dot.y && points[i].y >= dot.y) {
			if (points[i].x + (dot.y - points[i].y) / (points[j].y - points[i].y)*(points[j].x - points[i].x) < dot.x) {
				res = !res;
			}
		}
	}

	return res;
}

void CCGWorkView::CRenderer::draw_bounding_box_if_needed(CModel& model, mat4& transform) {
	if (draw_bounding_box) {
		for (const vec3& p1 : model.bounding_box) {
			for (const vec3& p2 : model.bounding_box) {
				vec3 res = p1 - p2;
				if ((!res.x && !res.y) || (!res.y && !res.z) || (!res.x && !res.z)) {
					vec4 a = transform * vec4(p1.x, p1.y, p1.z, 1.0f);
					vec4 b = transform * vec4(p2.x, p2.y, p2.z, 1.0f);
					if (!camera.is_orthographic()) {
						apply_perspective(a);
						apply_perspective(b);
					}
					vec2 p1 = cast(vec2(a));
					vec2 p2 = cast(vec2(b));
					draw_line(p1, p2, model.bbox_color);
				}
			}
		}
	}
}

void CCGWorkView::CRenderer::draw_normals(CModel& model, CPolygon& polygon,
	mat4& transform, vector<vec3> source, vector<vec2> points, std::unordered_map<vec3, std::unordered_set<vec3>>& verticesMap) {
	vec3 sourceNormal;

	if (draw_polygon_included_normals) {
		sourceNormal = polygon.included_normal;
	}
	else if (source.size() >= 3) {
		sourceNormal = normalized(cross(source[2] - source[1], source[0] - source[1]))*0.02f;
	}
	if (draw_polygon_normals) {
		vec3 normalStart;
		for (const vec3& point : source) {
			normalStart = point + normalStart;
		}
		normalStart = normalStart / (float)source.size();

		draw_normal(normalStart, sourceNormal, transform, normals_color);
	}

	if (draw_vertice_normals && !draw_vertice_included_normals) {
		for (const vec3& point : source) {
			verticesMap[point].insert(sourceNormal);
		}
	}

	if (draw_vertice_normals && draw_vertice_included_normals) {
		for (const CVertice& point : polygon.vertices) {
			draw_normal(point.point, point.imported_normal, transform, normals_color);
		}
	}
}

void CCGWorkView::CRenderer::draw_model(CModel & model)
{
	mat4 transform = model.model_transform;
	transform = transform * model.view_transform;
	transform = transform * camera.projection;

	std::unordered_map<vec3, std::unordered_set<vec3>> verticesMap;
	std::unordered_set<edge> edgesDone;


	for (CPolygon& polygon : model.polygons) {
		vector<vec2> points;
		vector<vec3> source;

		for (const CVertice& vertice : polygon.vertices) {
			vec3 point = vertice.point;
			vec4 res = transform * vec4(point.x, point.y, point.z, 1.0f);
			if (!camera.is_orthographic()) {
				apply_perspective(res);
			}
			vec2 v = cast(vec2(res));
			points.push_back(v);
			source.push_back(point);
		}
		int size = points.size() - 1;

		if (select_highlighted_pol) {
			if (pointIsInside(points, vec2(mouse_x, mouse_y))) {
				polygon.highlight = true;
			}
			else {
				polygon.highlight = false;
			}
		}

		for (int i = 0; i < size; i++) {
			draw_line(points[i], points[i + 1], polygon.highlight ? highlight_polygon : wireframe_color, polygon.highlight);
		}

		draw_line(points[size], points[0], polygon.highlight ? highlight_polygon : wireframe_color, polygon.highlight);

		draw_normals(model, polygon, transform, source, points, verticesMap);
	}
	if (draw_vertice_normals && !draw_vertice_included_normals) {
		for (auto i = verticesMap.begin(); i != verticesMap.end(); i++) {
			vec3 normalStart;
			for (const vec3& point : i->second) {
				normalStart = point + normalStart;
			}
			normalStart = normalStart / (float)i->second.size();

			draw_normal(i->first, normalStart, transform, normals_color);
		}
	}

	draw_bounding_box_if_needed(model, transform);

	select_highlighted_pol = false;
}

CCGWorkView::CScene::CScene(CCGWorkView* parent) :
	current_camera(0), renderer(parent)
{
	cameras.push_back(CCamera());
}

void CCGWorkView::CScene::add_model(const CModel & model)
{
	models.clear();
	models.push_back(model);
}

void CCGWorkView::CScene::add_camera(const CCamera & camera)
{
}

void CCGWorkView::CScene::update_projection(int projection_type)
{
	for (CCamera& camera : cameras) {
		if (projection_type == ID_VIEW_ORTHOGRAPHIC) {
			camera.set_orthographic();
		}
		else {
			camera.set_perspective();
		}
	}
}

CCamera & CCGWorkView::CScene::get_current_camera()
{
	return cameras[current_camera];
}

void CCGWorkView::CScene::update(CCGWorkView* app, int mouse_dx)
{
	if (mouse_dx == 0) {
		return;
	}

	mat4 transformation;
	float x = 0.0f, y = 0.0f, z = 0.0f;
	switch (app->m_nAxis) {
	case ID_AXIS_X:
		x = mouse_dx;
		break;
	case ID_AXIS_Y:
		y = mouse_dx;
		break;
	case ID_AXIS_Z:
		z = mouse_dx;
		break;
	}
	switch (app->m_nAction) {
	case ID_ACTION_TRANSLATE:
		transformation = translation(x * TRANSLATION_FACTOR * app->mouse_sensitivity,
			y * TRANSLATION_FACTOR * app->mouse_sensitivity, z * TRANSLATION_FACTOR * app->mouse_sensitivity);
		break;
	case ID_ACTION_ROTATE:
		transformation = rotation(x * ROTATION_FACTOR * app->mouse_sensitivity,
			y * ROTATION_FACTOR * app->mouse_sensitivity, z * ROTATION_FACTOR * app->mouse_sensitivity);
		break;
	case ID_ACTION_SCALE:
		transformation = scaling(1 + x * SCALING_FACTOR * app->mouse_sensitivity,
			1 + y * SCALING_FACTOR * app->mouse_sensitivity, 1 + z * SCALING_FACTOR * app->mouse_sensitivity);
		break;
	}

	for (CModel& model : models) {
		switch (app->transform_context) {
		case TRANSFORM_MODEL:
			model.transform_model(transformation);
			break;
		case TRANSFORM_VIEW:
			model.transform_view(transformation);
			break;
		}
	}
}

void CCGWorkView::CScene::draw(CDC* context)
{
	renderer.get_bitmap(context);
	renderer.set_camera(cameras[current_camera]);
	renderer.bitemap.reset();
	for (CModel& model : models) {
		renderer.draw_model(model);
	}
	renderer.draw_bitmap(context);
}


void CCGWorkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	clicking = true;
	CView::OnLButtonDown(nFlags, point);
}

void CCGWorkView::OnLButtonUp(UINT nFlags, CPoint point)
{
	clicking = false;
	CView::OnLButtonUp(nFlags, point);
	scene.renderer.mouse_x = point.x;
	scene.renderer.mouse_y = point.y;
	scene.renderer.select_highlighted_pol = true;
}

void CCGWorkView::OnPolygonIncluded()
{
	scene.renderer.draw_polygon_included_normals = true;
}

void CCGWorkView::OnPolygonCalculated()
{
	scene.renderer.draw_polygon_included_normals = false;
}

void CCGWorkView::OnVertexIncluded()
{
	scene.renderer.draw_vertice_included_normals = true;
}

void CCGWorkView::OnVertexCalculated()
{
	scene.renderer.draw_vertice_included_normals = false;
}

void CCGWorkView::OnUpdatePolygonIncluded(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.draw_polygon_included_normals);
}

void CCGWorkView::OnUpdatePolygonCalculated(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!scene.renderer.draw_polygon_included_normals);
}

void CCGWorkView::OnUpdateVertexIncluded(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.draw_vertice_included_normals);
}

void CCGWorkView::OnUpdateVertexCalculated(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!scene.renderer.draw_vertice_included_normals);
}

void CCGWorkView::OnActionTransformobject()
{
	transform_context = TRANSFORM_MODEL;
}

void CCGWorkView::OnUpdateActionTransformobject(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(transform_context == TRANSFORM_MODEL);
}


void CCGWorkView::OnActionTransformview()
{
	transform_context = TRANSFORM_VIEW;
}


void CCGWorkView::OnUpdateActionTransformview(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(transform_context == TRANSFORM_VIEW);
}


void CCGWorkView::OnNormalPlaneToggle()
{
	scene.renderer.draw_polygon_normals = !scene.renderer.draw_polygon_normals;
}


void CCGWorkView::OnUpdateNormalPlaneToggle(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.draw_polygon_normals);
}


void CCGWorkView::OnNormalVerticeToggle()
{
	scene.renderer.draw_vertice_normals = !scene.renderer.draw_vertice_normals;
}


void CCGWorkView::OnUpdateNormalVerticeToggle(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.draw_vertice_normals);
}


void CCGWorkView::OnShowBbox()
{
	scene.renderer.draw_bounding_box = !scene.renderer.draw_bounding_box;
}


void CCGWorkView::OnUpdateShowBbox(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.draw_bounding_box);
}


void CCGWorkView::OnOptionsPerspectivecontrol()
{
	PerspectiveDlg dlg(nullptr, scene.get_current_camera().d);
	if (dlg.DoModal() == IDOK) {
		scene.get_current_camera().set_depth(dlg.depth);
	}
}



void CCGWorkView::OnOptionsMousesensitivity()
{
	DialMouseSens dlg(this);
	if (dlg.DoModal() == IDOK) {
		mouse_sensitivity = dlg.mouse_sensitivity;
	}
}


void CCGWorkView::OnOptionsColors()
{
	ColorsSelect dlg(this);
	dlg.DoModal();
}
