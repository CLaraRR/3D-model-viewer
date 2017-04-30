
// OpenGL3DTransformView.cpp : COpenGL3DTransformView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "OpenGL3DTransform.h"
#endif

#include "OpenGL3DTransformDoc.h"
#include "OpenGL3DTransformView.h"
#include "CArcBall.h"
#include <Windows.h> //Windows��ͷ�ļ�
#include <sstream>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class ArcBall;
// COpenGL3DTransformView

IMPLEMENT_DYNCREATE(COpenGL3DTransformView, CView)

BEGIN_MESSAGE_MAP(COpenGL3DTransformView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COpenGL3DTransformView::OnFilePrintPreview)
//	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
//	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// COpenGL3DTransformView ����/����


COpenGL3DTransformView::COpenGL3DTransformView()
{
	// TODO:  �ڴ˴���ӹ������
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	/*m_xAngle = 0.0f;
	m_yAngle = 0.0f;*/
	m_Scale = 1.0f;
	translateVector.s.X = 0.0f;
	translateVector.s.Y = 0.0f;
	translateVector.s.Z = 0.0f;
	leftButtonDown = false;
	rightButtonDown = false;
	ArcBall = new ArcBallT();


}

COpenGL3DTransformView::~COpenGL3DTransformView()
{
}

BOOL COpenGL3DTransformView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

// COpenGL3DTransformView ����

void COpenGL3DTransformView::OnDraw(CDC* /*pDC*/)
{
	COpenGL3DTransformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderScene();
	// Tell OpenGL to flush its pipeline  
	::glFinish();
	// Now Swap the buffers  
	::SwapBuffers(m_pDC->GetSafeHdc());
}


// COpenGL3DTransformView ��ӡ


void COpenGL3DTransformView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COpenGL3DTransformView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void COpenGL3DTransformView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void COpenGL3DTransformView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}



//void COpenGL3DTransformView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
//{
//#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
//}


// COpenGL3DTransformView ���

#ifdef _DEBUG
void COpenGL3DTransformView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGL3DTransformView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGL3DTransformDoc* COpenGL3DTransformView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGL3DTransformDoc)));
	return (COpenGL3DTransformDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenGL3DTransformView ��Ϣ�������


int COpenGL3DTransformView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	//Initialize OpenGL Here  

	InitializeOpenGL();
	
	ReadOBJ();
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);//��������ӳ��
}

BOOL COpenGL3DTransformView::InitializeOpenGL()
{
	//Get a DC for the Client Area  

	m_pDC = new CClientDC(this);

	//Failure to Get DC  

	if (m_pDC == NULL)

	{

		//MessageBox("Error Obtaining DC");

		return FALSE;

	}

	//Failure to set the pixel format  

	if (!SetupPixelFormat())

	{

		return FALSE;

	}

	//Create Rendering Context  

	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());

	//Failure to Create Rendering Context  

	if (m_hRC == 0)

	{

		//MessageBox("Error Creating RC");

		return FALSE;

	}

	//Make the RC Current  

	if (::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC) == FALSE)

	{

		//MessageBox("Error making RC Current");

		return FALSE;

	}

	//Specify Black as the clear color  

	::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Specify the back of the buffer as clear depth  

	::glClearDepth(1.0f);

	//Enable Depth Testing  

	::glEnable(GL_DEPTH_TEST);

	return TRUE;

}

BOOL COpenGL3DTransformView::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =

	{

		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd  
		1,                              // version number  

		PFD_DRAW_TO_WINDOW |            // support window  

		PFD_SUPPORT_OPENGL |            // support OpenGL  

		PFD_DOUBLEBUFFER,                // double buffered  

		PFD_TYPE_RGBA,                  // RGBA type  
		24,                             // 24-bit color depth  
		0, 0, 0, 0, 0, 0,               // color bits ignored  
		0,                              // no alpha buffer  
		0,                              // shift bit ignored  
		0,                              // no accumulation buffer  
		0, 0, 0, 0,                     // accum bits ignored  
		16,                             // 16-bit z-buffer  
		0,                              // no stencil buffer  
		0,                              // no auxiliary buffer  

		PFD_MAIN_PLANE,                 // main layer  
		0,                              // reserved  
		0, 0, 0                         // layer masks ignored  

	};

	int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	if (m_nPixelFormat == 0)

	{

		return FALSE;

	}

	if (::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)

	{

		return FALSE;

	}

	return TRUE;
}

void COpenGL3DTransformView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������


	if (0 >= cx || 0 >= cy)
	{
		return;
	}
	//�����任
	// select the full client area  
	::glViewport(0, 0, cx, cy);
	// ����aspect ratio  
	// this will keep all dimension scales equal  
	GLdouble aspect_ratio = (GLdouble)cx / (GLdouble)cy;

	//ͶӰ��ͼ�任
	// select the projection matrix and clear it  
	::glMatrixMode(GL_PROJECTION);

	//glLoadIdentity()ʵ�����ǶԵ�ǰ������г�ʼ��
	//������ǰ�����˶��ٴξ���任��ִ�и������ǰ���󶼻�ָ���һ����λ����
	//���൱��û�н����κξ���任״̬
	//ʵ���Ͻ���ǰ���Ƶ�����Ļ���ģ������ڸ�λ����
	//���ͬ���õ�λ�������glLoadMatrix()������glLoadIdentity����Ч��
	::glLoadIdentity();
	// select the viewing volume  
	//͸��ͶӰ 
	//��gluPerspective(fovy, ratio, near, far)����ָ���Ӿ���
	::gluPerspective(60.0f, aspect_ratio, .1f, 200.0f);
	//����ͶӰ
	//glFrustum(-1, 1, -1, 1, 1.5, 20);


	//ģ����ͼ�任������ͼ�任��ʼ
	// switch back to the modelview matrix and clear it  
	::glMatrixMode(GL_MODELVIEW);
	//::glLoadIdentity();
	gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);


	ArcBall->setWindow(cx, cy);
}


void COpenGL3DTransformView::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(translateVector.s.X, translateVector.s.Y, -5.0f);  //1.ƽ��
	glScalef(m_Scale, m_Scale, m_Scale);  //2.����

	//glMultMatrixf(M)�������ǽ�����M�˵������ջ���˵ľ���T����������ˣ���MT
	//�����ǰ���ת�õ��ı任����Transform.M��˵�ջ���ľ���T
	glMultMatrixf(ArcBall->Transform.M);                        //3. ��ת

	//���ù����µĲ���
	GLfloat mat_ambient[] = { 0.2, 0.5, 0.7, 1.0 };
	GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
	GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 0.5 };
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat shininess = 50.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	loadTexture();
	//��һ�����
	//glutSolidTeapot(0.5);
	show3DModel();
	glFlush();


}




void COpenGL3DTransformView::show3DModel()
{
	//������������Ƭ����ѭ������
	for (int i = 0; i<m_pic.F.size(); i++)
	{
		glBegin(GL_TRIANGLES); // ����������GL_TRIANGLES;GL_LINE_LOOP;GL_LINES;GL_POINTS
		//���� 
		if (m_pic.VT.size() != 0)
			glTexCoord2f(m_pic.VT[m_pic.F[i].T[0]].TU, m_pic.VT[m_pic.F[i].T[0]].TV);  
		//������
		if (m_pic.VN.size() != 0)
			glNormal3f(m_pic.VN[m_pic.F[i].N[0]].NX, m_pic.VN[m_pic.F[i].N[0]].NY, m_pic.VN[m_pic.F[i].N[0]].NZ);
		//�϶���
		glVertex3f(m_pic.V[m_pic.F[i].V[0]].s.X, m_pic.V[m_pic.F[i].V[0]].s.Y, m_pic.V[m_pic.F[i].V[0]].s.Z);

		//����
		if (m_pic.VT.size() != 0)
			glTexCoord2f(m_pic.VT[m_pic.F[i].T[1]].TU, m_pic.VT[m_pic.F[i].T[1]].TV);  
		//������
		if (m_pic.VN.size() != 0)
			glNormal3f(m_pic.VN[m_pic.F[i].N[1]].NX, m_pic.VN[m_pic.F[i].N[1]].NY, m_pic.VN[m_pic.F[i].N[1]].NZ);
		//���¶���
		glVertex3f(m_pic.V[m_pic.F[i].V[1]].s.X, m_pic.V[m_pic.F[i].V[1]].s.Y, m_pic.V[m_pic.F[i].V[1]].s.Z);

		//����
		if (m_pic.VT.size() != 0)
			glTexCoord2f(m_pic.VT[m_pic.F[i].T[2]].TU, m_pic.VT[m_pic.F[i].T[2]].TV); 
		//������
		if (m_pic.VN.size() != 0)
			glNormal3f(m_pic.VN[m_pic.F[i].N[2]].NX, m_pic.VN[m_pic.F[i].N[2]].NY, m_pic.VN[m_pic.F[i].N[2]].NZ);
		//���¶���
		glVertex3f(m_pic.V[m_pic.F[i].V[2]].s.X, m_pic.V[m_pic.F[i].V[2]].s.Y, m_pic.V[m_pic.F[i].V[2]].s.Z);

		glEnd();// �����λ��ƽ���   
		//glDisable(GL_TEXTURE_2D); /* disable texture mapping */

	}

}


/*
void COpenGL3DTransformView::DrawCylinder()
{
	//������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double delta = 1.0;//���ӵĶ���
	int radius = 50;//�뾶
	int height = 180;//��
	int degree = 360;//����
	double theta = delta;



	glColor3f(0.5f, 0.5f, 1.0f);
	glBegin(GL_QUAD_STRIP);
	glVertex3d(radius, 0, height / 2);
	glVertex3d(radius, 0, -height / 2);
	for (int i = 1; i< degree; ++i)
	{
		double x = radius * cos(theta);
		double y = radius * sin(theta);
		glVertex3d(x, y, height / 2);
		glVertex3d(x, y, -height / 2);
		theta += delta;
	}
	glVertex3d(radius, 0, height / 2);
	glVertex3d(radius, 0, -height / 2);
	glEnd();



	//���µ���
	glColor3f(0.8f, 0.0f, 0.0f);
	theta = 0.0;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, -height / 2);
	glVertex3d(radius, 0, -height / 2);
	for (int i = 1; i < degree; ++i)
	{
		double x = radius * cos(theta);
		double y = radius * sin(theta);
		glVertex3d(x, y, -height / 2);
		theta += delta;
	}
	glVertex3d(radius, 0, -height / 2);
	glEnd();



	//���ϵ���
	glColor3f(1.0f, 1.0f, 1.0f);
	theta = 0.0;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, height / 2);
	glVertex3d(radius, 0, height / 2);
	for (int i = 0; i < degree; ++i)
	{
		double x = radius * cos(theta);
		double y = radius * sin(theta);
		glVertex3d(x, y, height / 2);
		theta += delta;
	}
	glVertex3d(radius, 0, height / 2);
	glEnd();


	//glCallList(ID_COORDINATY);
}

*/
BOOL COpenGL3DTransformView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
}


void COpenGL3DTransformView::OnDestroy()
{
	CView::OnDestroy();

	// TODO:  �ڴ˴������Ϣ����������

	//Make the RC non-current  
	if (::wglMakeCurrent(0, 0) == FALSE)
	{
		//MessageBox("Could not make RC non-current");
	}

	//Delete the rendering context  
	if (::wglDeleteContext(m_hRC) == FALSE)
	{
		//MessageBox("Could not delete RC");
	}
	//Delete the DC  
	if (m_pDC)
	{
		delete m_pDC;
	}
	//Set it to NULL  
	m_pDC = NULL;
}





void COpenGL3DTransformView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	leftButtonDown = true;  //����������Ϊ�Ѱ���
	ArcBall->isClicked = true;  //��ArcBall������������
	ArcBall->MousePt.s.X = point.x;  //��ArcBall�������õ�ǰ��갴�µ�����
	ArcBall->MousePt.s.Y = point.y;
	ArcBall->upstate();  //���ø��º���
	SetCapture();  //������겶��
	CView::OnLButtonDown(nFlags, point);
}


void COpenGL3DTransformView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_MouseDownPoint = CPoint(0, 0); //�����������Ϊ0��0
	leftButtonDown = false; //����������Ϊ����
	ArcBall->isClicked = false; //��ArcBall����������������
	ArcBall->MousePt.s.X = 0; //��ArcBall�����������Ϊ0��0
	ArcBall->MousePt.s.Y = 0;
	ArcBall->upstate(); //����ArcBall״̬
	ReleaseCapture();  //�ͷ���겶��
	CView::OnLButtonUp(nFlags, point);
}


void COpenGL3DTransformView::OnRButtonUp(UINT nFlags , CPoint point)
{
	m_MouseDownPoint = CPoint(0, 0);  //�����������Ϊ0��0
	rightButtonDown = false;  //��������Ҽ�Ϊ����
	ReleaseCapture(); //�ͷ���겶��
	CView::OnRButtonUp(nFlags, point);
}

void COpenGL3DTransformView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_MouseDownPoint = point; //��¼��ǰ����Ҽ����µ�����
	rightButtonDown = true;  //��������Ҽ�Ϊ����
	SetCapture();  //������겶��
	m_OldMousePoint = m_MouseDownPoint;  //�������������Ϊ��ǰ��ֵ
	CView::OnRButtonDown(nFlags, point);
}

void COpenGL3DTransformView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (GetCapture() == this)
	{
		if (leftButtonDown){
			//�Ѷ�άƽ���������긳��MousePt
			ArcBall->MousePt.s.X = point.x; 
			ArcBall->MousePt.s.Y = point.y;
			ArcBall->upstate(); //����ArcBall��״̬

			m_OldMousePoint = point; //�����������Ϊ�ɵ�
			//Redraw the view  
			InvalidateRect(NULL, FALSE); //ˢ����Ļ
		}
		else if (rightButtonDown){
			m_MouseDownPoint = point; //��¼��ǰ�������
			
			Vector3fT vec; //����һ����������¼��ǰ���λ�ú��ϴ�����λ���ƶ�������

			vec.s.X = m_MouseDownPoint.x - m_OldMousePoint.x; //�ƶ�������x����
			vec.s.Y = m_OldMousePoint.y - m_MouseDownPoint.y; //�ƶ�������y����
			vec.s.Z = 0; //�ƶ�������z����

			//���ƶ������ӵ�ԭ����ƽ��������
			translateVector.s.X += vec.s.X*0.01; //����һ�����ӵ����ƶ��Ĳ���
			translateVector.s.Y += vec.s.Y*0.01;
			translateVector.s.Z =0;

			m_OldMousePoint = m_MouseDownPoint; //�����������Ϊ�ɵĵ�
			InvalidateRect(NULL, FALSE);
		}

	};
	CView::OnMouseMove(nFlags, point);
}


BOOL COpenGL3DTransformView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	double scaleFactor;

	if (zDelta >= 0)//���������ϻ���
	{
		scaleFactor = 0.8; //���÷Ŵ�����Ϊ����0.1
		if (m_Scale <= 0){  //����Ŵ���С��������ƷŴ���Ϊ0.1
			m_Scale = 0.1;
		}
		else{   //�������0�򲻶����ӷŴ���
			m_Scale += scaleFactor;
		}
	}
	else  //���������»���
	{
		scaleFactor = -0.8;
		if (m_Scale <= 0){
			m_Scale = 0.1;
		}
		else{
			m_Scale += scaleFactor;
		}
	}
	Invalidate(FALSE);



	//InvalidateRect(NULL, FALSE);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}




void COpenGL3DTransformView::ReadOBJ()
{
	ifstream ifs("MeshData/fish/result_Fish_Final.obj");//����һ���ļ�������
	string s;
	CFace* f; //��
	Tuple3fT *v; //����
	NormalVector *vn;  //������
	Texture	*vt; //��������
	while (getline(ifs, s))//�Ѵ��������������ַ����浽s����
	{
		//.obj�ļ��������м�����ʽ��
		//1�� v -0.211499 0.000134 0.082503  v�����㣬�����Ƕ�������
		//2�� vt 0.601082 0.685057   vt����������������������
		//3�� vn -0.292611 -0.000354 -0.956231  vn���������������Ƿ���������
		//4�� f 2180/2180/12526 2181/2181/12527 2198/2198/12528  f�����棬������3��ֵ
		//ÿ��ֵ��3�����ֱ����������/��������/����������
		if (s.length()<2)continue;
		if (s[0] == 'v'){
			if (s[1] == 't'){//vt 0.601082 0.685057 ����
				istringstream in(s);
				vt = new Texture();//�������
				string head;
				in >> head >> vt->TU >> vt->TV; //�ָ��ַ���
				m_pic.VT.push_back(*vt); //��ӵ��������������������
			}
			else if (s[1] == 'n'){//vn -0.292611 -0.000354 -0.956231 ������
				istringstream in(s);
				vn = new NormalVector();//����������
				string head;
				in >> head >> vn->NX >> vn->NY >> vn->NZ; //�ָ��ַ���
				m_pic.VN.push_back(*vn);//��ӵ���������ķ�����������
			}
			else{//v-0.211499 0.000134 0.082503 ��
				istringstream in(s);
				v = new Tuple3fT();//�������
				string head;
				in >> head >> v->s.X >> v->s.Y >> v->s.Z; //�ָ��ַ���
				m_pic.V.push_back(*v);//��ӵ���������Ķ���������
			}
		}
		else if (s[0] == 'f'){//f 2443//2656 2442//2656 2444//2656 ��
			for (int k = s.size() - 1; k >= 0; k--){
				if (s[k] == '/')s[k] = ' ';
			}
			istringstream in(s);
			f = new CFace();//�����
			string head;
			in >> head;
			int i = 0;
			while (i<3)
			{
				//   ��������/��������/����������
				if (m_pic.V.size() != 0)
				{
					in >> f->V[i];
					f->V[i] -= 1; //���鿪ʼ��0Ϊ�±꣬������1������Ҫ��ȥ1���Ƕ�Ӧ������
				}
				if (m_pic.VT.size() != 0)
				{
					in >> f->T[i];
					f->T[i] -= 1;
				}
				if (m_pic.VN.size() != 0)
				{
					in >> f->N[i];
					f->N[i] -= 1;
				}
				i++;
			}
			m_pic.F.push_back(*f);//��ӵ������������������
		}
	}
}



void COpenGL3DTransformView::loadTexture()
{
	//AUX_RGBImageRec������һ��RGBͼ��ṹ���͡��ýṹ������������Ա��
	//sizeX :ͼ��Ŀ��
	//sizeY :ͼ��ĸ߶�
	//data  :ͼ�������������ݣ���ʵҲ���Ǹ�ͼ�����ڴ��е��������ݵ�һ��ָ��
	AUX_RGBImageRec *TextureImage[1];//��������Ĵ洢�ռ�
	memset(TextureImage, 0, sizeof(void *) * 1);//��ָ����ΪNULL
	//����λͼ��������޴������λͼû�ҵ����˳�
	if (TextureImage[0] = LoadBMP("MeshData/fish/carp.bmp"))
	{
		glGenTextures(1, &texture[0]);//��������
		//������λͼ�������ɵ�����
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		//��������
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, TextureImage[0]->sizeX, TextureImage[0]->sizeY,
			GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//�����˲�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//�����˲�
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//GL_REPLACEʹ��������ɫ���滻ԭ�е�������ɫ
		                                                         //GL_BLEND����GL_MODULATE����ʾ�����ս����������ɫ���е���
	}
	if (TextureImage[0])//�����Ƿ����
	{
		if (TextureImage[0]->data)//����ͼ���Ƿ����
		{
			free(TextureImage[0]->data);//�Ƿ�����ͼ��ռ�õ��ڴ�
		}
		free(TextureImage[0]);//�Ƿ�ͼ��ṹ
	}
}
AUX_RGBImageRec* COpenGL3DTransformView::LoadBMP(char *Filename) //����λͼͼ��
{
	FILE *File = NULL; //�ļ����
	if (!Filename)//ȷ���ļ������ṩ
	{
		return NULL;
	}
	File = fopen(Filename, "r");//���Դ��ļ�


	if (File)
	{
		fclose(File);
		return auxDIBImageLoad(Filename);//����λͼ������ָ��
	}

	return NULL;
}
