// CGWorkView.cpp : implementation of the CCGWorkView class
//
#include "stdafx.h"
#include "CGWork.h"

#include "CGWorkDoc.h"
#include "CGWorkView.h"

#include <iostream>
using std::cout;
using std::endl;
#include "MaterialDlg.h"
#include "LightDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PngWrapper.h"
#include "iritSkel.h"

// For Status Bar access
#include "MainFrm.h"

#include <set>
#include <unordered_map>
#include <unordered_set>
#include <bitset>

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
	ON_COMMAND(ID_NORMAL_PLANE_TOGGLE, IdNormalPlanToggle)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_COMMAND(ID_POLYGON_INCLUDED, &CCGWorkView::OnPolygonIncluded)
	ON_COMMAND(ID_VERTEX_INCLUDED, &CCGWorkView::OnVertexIncluded)
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

	m_nLightShading = ID_LIGHT_SHADING_FLAT;

	m_lMaterialAmbient = 0.2;
	m_lMaterialDiffuse = 0.8;
	m_lMaterialSpecular = 1.0;
	m_nMaterialCosineFactor = 32;

	//init the first light to be enabled
	m_lights[LIGHT_ID_1].enabled = true;
	m_pDbBitMap = NULL;
	m_pDbDC = NULL;
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


	scene.draw(pDCToUse);



	//pDCToUse->FillSolidRect(&r, RGB(0, 0, 0));

	//mat3 screen_scale(SetScreenScale());	////
	//vec3* boundingBox[2] = { NULL };			////
	//for (CModel& model : models) {
	//	mat3 rendering_mat = screen_scale * camera.projection * model.position * model.transform;
	//	for (CPolygon& polygon : model.polygons) {
	//		vector<vec3> points;
	//		for (vec3& point : polygon.vertices) {
	//			points.push_back(rendering_mat * point);
	//			setBoundingBox(boundingBox, &point);			////
	//		}
	//		int size = points.size() - 1;
	//		for (int i = 0; i < size; i++) {
	//			draw_line(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
	//		}
	//		draw_line(points[size].x, points[size].y, points[0].x, points[0].y);

	//	}
	//	DrawBoundingBox(boundingBox, rendering_mat);		////
	//	delete boundingBox[0]; delete boundingBox[1];
	//	boundingBox[0] = boundingBox[1] = NULL;
	//}
	/*auto t1 = std::chrono::high_resolution_clock::now();
	pDCToUse->SetPixel(0, 0, RGB(255, 255, 255));
	auto t2 = std::chrono::high_resolution_clock::now();
	std::ostringstream ss;
	ss << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " us" << endl;
	OutputDebugStringA(ss.str().c_str());*/

	if (pDCToUse != m_pDC)
	{
		m_pDC->BitBlt(r.left, r.top, r.Width(), r.Height(), pDCToUse, r.left, r.top, SRCCOPY);
	}
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
		model.set_bounding_box();
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





// OPTIONS HANDLERS ///////////////////////////////////////////




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
	parent(parent), context(nullptr),
	background_color(RGB(0, 0, 0)),
	bounding_box_color(RGB(255, 255, 0)),
	highlight_polygon(RGB(0, 255, 0)) {}

void CCGWorkView::IdNormalPlanToggle() {
	scene.renderer.toggle_poly_normals();
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

	//context->SetPixel(p, color);
	if (p.x >= 0 && p.y >= 0 && p.x < 3840 && p.y < 2160)
		if (!bitemap.test(p.x + p.y * 3840) || forcePrint) {
			bitemap.set(p.x + p.y * 3840);
			context->SetPixelV(p, color);
			//BitBlt(context->m_hDC, p.x, p.y, 1, 1, context->m_hDC, p.x, p.y, color);
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
						context->SetPixelV(p, color);
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
						context->SetPixelV(p, color);
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
						context->SetPixelV(p, color);
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
						context->SetPixelV(p, color);
					}
			}
		}
	}
}

vec2 CCGWorkView::CRenderer::cast(const vec2& v)
{
	int width = screen.Width(), height = screen.Height();
	int scale = min(height, width);
	vec2 v2(v.x * scale + width / 2, -v.y * scale + height / 2);
	return v2;
}

void CCGWorkView::CRenderer::toggle_bounding_box()
{
	bounding_box_toggled = !bounding_box_toggled;
}

void CCGWorkView::CRenderer::toggle_poly_normals()
{
	poly_normals_toggled = !poly_normals_toggled;
}

void CCGWorkView::CRenderer::toggle_poly_included()
{
	poly_included_normals = !poly_included_normals;
}

void CCGWorkView::CRenderer::toggle_vertices_normals()
{
	vertices_normals_toggled = !vertices_normals_toggled;
}

void CCGWorkView::CRenderer::toggle_vertices_included()
{
	vertices_included_normals = !vertices_included_normals;
}

void CCGWorkView::CRenderer::set_context(CDC* context)
{
	this->context = context;
	parent->GetClientRect(&screen);
}

void CCGWorkView::CRenderer::draw_background()
{
	context->FillSolidRect(&screen, background_color);
}

void CCGWorkView::CRenderer::draw_normal(const vec3& startPoint, const vec3& givenNormal, mat4 transform, COLORREF color) {
	vec3 sourceNormal = startPoint + normalized(givenNormal)*0.2f;

	vec2 newSource = cast(vec2(transform *  vec4(sourceNormal.x, sourceNormal.y, sourceNormal.z, 1.0f)));
	vec2 newStart = cast(vec2(transform *  vec4(startPoint.x, startPoint.y, startPoint.z, 1.0f)));

	//	if(!check_if_drawn)
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

void CCGWorkView::CRenderer::draw_model(const CCamera & camera, const CModel & model)
{
	mat4 transform = model.model_transform * model.view_transform * camera.transform * camera.projection;

	std::unordered_map<vec3, std::unordered_set<vec3>> verticesMap;
	std::unordered_set<edge> edgesDone;

	POINT g;
	GetCursorPos(&g);
	g.x -= screen.TopLeft().x;
	g.y -= screen.TopLeft().y;
	context->SetPixelV(g, model.color);
	bool flagMousePoly = false;

	for (const CPolygon& polygon : model.polygons) {
		vector<vec2> points;
		vector<vec3> source;

		for (const CVertice& vertice : polygon.vertices) {
			vec3 point = vertice.point;
			points.push_back(cast(vec2(transform * vec4(point.x, point.y, point.z, 1.0f))));
			source.push_back(point);
		}
		int size = points.size() - 1;


		COLORREF ccc = model.color;
		bool forcePrint = false;
		if (!flagMousePoly)
			if (pointIsInside(points, vec2(g.x, g.y))) {
				ccc = highlight_polygon;
				forcePrint = true;
				flagMousePoly++;
			}

		for (int i = 0; i < size; i++) {
			draw_line(points[i], points[i + 1], ccc, forcePrint);
		}

		draw_line(points[size], points[0], ccc, forcePrint);


		vec3 sourceNormal;
		if (poly_included_normals) {
			sourceNormal = polygon.included_normal;
		}
		else if (source.size() >= 3) {
			sourceNormal = normalized(cross(source[2] - source[1], source[0] - source[1]))*0.2f;
		}
		if (poly_normals_toggled) {
			vec3 normalStart;
			for (const vec3& point : source) {
				normalStart = point + normalStart;
			}
			normalStart = normalStart / (float)source.size();

			draw_normal(normalStart, sourceNormal, transform, model.normalsColor);

		}

		if (vertices_normals_toggled && !vertices_included_normals)
			for (const vec3& point : source) {
				verticesMap[point].insert(sourceNormal);
			}

		if (vertices_normals_toggled && vertices_included_normals) {
			for (const CVertice& point : polygon.vertices) {
				draw_normal(point.point, point.imported_normal, transform, model.normalsColor);
			}
		}
	}
	if (vertices_normals_toggled && !vertices_included_normals) {
		for (auto i = verticesMap.begin(); i != verticesMap.end(); i++) {
			vec3 normalStart;
			for (const vec3& point : i->second) {
				normalStart = point + normalStart;
			}
			normalStart = normalStart / (float)i->second.size();

			draw_normal(i->first, normalStart, transform, model.normalsColor);
		}
	}


	if (bounding_box_toggled) {
		for (const vec3& p1 : model.bounding_box) {
			for (const vec3& p2 : model.bounding_box) {
				vec3 res = p1 - p2;
				if ((!res.x && !res.y) || (!res.y && !res.z) || (!res.x && !res.z)) {
					vec2 a = cast(vec2(transform * vec4(p1.x, p1.y, p1.z, 1.0f)));
					vec2 b = cast(vec2(transform * vec4(p2.x, p2.y, p2.z, 1.0f)));
					draw_line(a, b, bounding_box_color);
				}
			}
		}
	}
}

CCGWorkView::CScene::CScene(CCGWorkView* parent) :
	current_camera(0), renderer(parent)
{
	cameras.push_back(CCamera());
}

void CCGWorkView::CScene::add_model(const CModel & model)
{
	models.push_back(model);
}

void CCGWorkView::CScene::add_camera(const CCamera & camera)
{
}

void CCGWorkView::CScene::draw(CDC* context)
{
	renderer.set_context(context);
	renderer.draw_background();
	renderer.bitemap.reset();
	for (const CModel& model : models) {
		renderer.draw_model(cameras[current_camera], model);
	}
	//for (CModel& model : models)
	//	model.model_transform = rotation_Y(2.0f) * model.model_transform;
}


void CCGWorkView::OnPolygonIncluded()
{
	scene.renderer.toggle_poly_included();
}


void CCGWorkView::OnVertexIncluded()
{
	scene.renderer.toggle_vertices_included();
}
