
// OpenGL3DTransformView.cpp : COpenGL3DTransformView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OpenGL3DTransform.h"
#endif

#include "OpenGL3DTransformDoc.h"
#include "OpenGL3DTransformView.h"
#include "CArcBall.h"
#include <Windows.h> //Windows的头文件
#include <sstream>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class ArcBall;
// COpenGL3DTransformView

IMPLEMENT_DYNCREATE(COpenGL3DTransformView, CView)

BEGIN_MESSAGE_MAP(COpenGL3DTransformView, CView)
	// 标准打印命令
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

// COpenGL3DTransformView 构造/析构


COpenGL3DTransformView::COpenGL3DTransformView()
{
	// TODO:  在此处添加构造代码
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
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

// COpenGL3DTransformView 绘制

void COpenGL3DTransformView::OnDraw(CDC* /*pDC*/)
{
	COpenGL3DTransformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderScene();
	// Tell OpenGL to flush its pipeline  
	::glFinish();
	// Now Swap the buffers  
	::SwapBuffers(m_pDC->GetSafeHdc());
}


// COpenGL3DTransformView 打印


void COpenGL3DTransformView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COpenGL3DTransformView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void COpenGL3DTransformView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void COpenGL3DTransformView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}



//void COpenGL3DTransformView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
//{
//#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
//}


// COpenGL3DTransformView 诊断

#ifdef _DEBUG
void COpenGL3DTransformView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGL3DTransformView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGL3DTransformDoc* COpenGL3DTransformView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGL3DTransformDoc)));
	return (COpenGL3DTransformDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenGL3DTransformView 消息处理程序


int COpenGL3DTransformView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
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
	glEnable(GL_TEXTURE_2D);//启用纹理映射
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

	// TODO:  在此处添加消息处理程序代码


	if (0 >= cx || 0 >= cy)
	{
		return;
	}
	//视区变换
	// select the full client area  
	::glViewport(0, 0, cx, cy);
	// 计算aspect ratio  
	// this will keep all dimension scales equal  
	GLdouble aspect_ratio = (GLdouble)cx / (GLdouble)cy;

	//投影视图变换
	// select the projection matrix and clear it  
	::glMatrixMode(GL_PROJECTION);

	//glLoadIdentity()实际上是对当前矩阵进行初始化
	//无论以前进行了多少次矩阵变换，执行该命令后当前矩阵都会恢复成一个单位矩阵
	//即相当于没有进行任何矩阵变换状态
	//实际上将当前点移到了屏幕中心，类似于复位操作
	//其等同于用单位矩阵调用glLoadMatrix()，但是glLoadIdentity更有效率
	::glLoadIdentity();
	// select the viewing volume  
	//透视投影 
	//用gluPerspective(fovy, ratio, near, far)函数指定视景体
	::gluPerspective(60.0f, aspect_ratio, .1f, 200.0f);
	//正交投影
	//glFrustum(-1, 1, -1, 1, 1.5, 20);


	//模型视图变换，以视图变换开始
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
	glTranslatef(translateVector.s.X, translateVector.s.Y, -5.0f);  //1.平移
	glScalef(m_Scale, m_Scale, m_Scale);  //2.缩放

	//glMultMatrixf(M)的作用是将矩阵M乘到矩阵堆栈顶端的矩阵T，并且是左乘，即MT
	//这里是把旋转得到的变换矩阵Transform.M左乘到栈顶的矩阵T
	glMultMatrixf(ArcBall->Transform.M);                        //3. 旋转

	//设置光照下的材质
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
	//画一个茶壶
	//glutSolidTeapot(0.5);
	show3DModel();
	glFlush();


}




void COpenGL3DTransformView::show3DModel()
{
	//对所有三角面片进行循环绘制
	for (int i = 0; i<m_pic.F.size(); i++)
	{
		glBegin(GL_TRIANGLES); // 绘制三角形GL_TRIANGLES;GL_LINE_LOOP;GL_LINES;GL_POINTS
		//纹理 
		if (m_pic.VT.size() != 0)
			glTexCoord2f(m_pic.VT[m_pic.F[i].T[0]].TU, m_pic.VT[m_pic.F[i].T[0]].TV);  
		//法向量
		if (m_pic.VN.size() != 0)
			glNormal3f(m_pic.VN[m_pic.F[i].N[0]].NX, m_pic.VN[m_pic.F[i].N[0]].NY, m_pic.VN[m_pic.F[i].N[0]].NZ);
		//上顶点
		glVertex3f(m_pic.V[m_pic.F[i].V[0]].s.X, m_pic.V[m_pic.F[i].V[0]].s.Y, m_pic.V[m_pic.F[i].V[0]].s.Z);

		//纹理
		if (m_pic.VT.size() != 0)
			glTexCoord2f(m_pic.VT[m_pic.F[i].T[1]].TU, m_pic.VT[m_pic.F[i].T[1]].TV);  
		//法向量
		if (m_pic.VN.size() != 0)
			glNormal3f(m_pic.VN[m_pic.F[i].N[1]].NX, m_pic.VN[m_pic.F[i].N[1]].NY, m_pic.VN[m_pic.F[i].N[1]].NZ);
		//左下顶点
		glVertex3f(m_pic.V[m_pic.F[i].V[1]].s.X, m_pic.V[m_pic.F[i].V[1]].s.Y, m_pic.V[m_pic.F[i].V[1]].s.Z);

		//纹理
		if (m_pic.VT.size() != 0)
			glTexCoord2f(m_pic.VT[m_pic.F[i].T[2]].TU, m_pic.VT[m_pic.F[i].T[2]].TV); 
		//法向量
		if (m_pic.VN.size() != 0)
			glNormal3f(m_pic.VN[m_pic.F[i].N[2]].NX, m_pic.VN[m_pic.F[i].N[2]].NY, m_pic.VN[m_pic.F[i].N[2]].NZ);
		//右下顶点
		glVertex3f(m_pic.V[m_pic.F[i].V[2]].s.X, m_pic.V[m_pic.F[i].V[2]].s.Y, m_pic.V[m_pic.F[i].V[2]].s.Z);

		glEnd();// 三角形绘制结束   
		//glDisable(GL_TEXTURE_2D); /* disable texture mapping */

	}

}


/*
void COpenGL3DTransformView::DrawCylinder()
{
	//画柱面
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double delta = 1.0;//增加的度数
	int radius = 50;//半径
	int height = 180;//高
	int degree = 360;//度数
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



	//画下底面
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



	//画上底面
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
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}


void COpenGL3DTransformView::OnDestroy()
{
	CView::OnDestroy();

	// TODO:  在此处添加消息处理程序代码

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
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	leftButtonDown = true;  //设置鼠标左键为已按下
	ArcBall->isClicked = true;  //在ArcBall设置鼠标键按下
	ArcBall->MousePt.s.X = point.x;  //在ArcBall里面设置当前鼠标按下的坐标
	ArcBall->MousePt.s.Y = point.y;
	ArcBall->upstate();  //调用更新函数
	SetCapture();  //设置鼠标捕获
	CView::OnLButtonDown(nFlags, point);
}


void COpenGL3DTransformView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_MouseDownPoint = CPoint(0, 0); //将鼠标坐标置为0，0
	leftButtonDown = false; //设置鼠标左键为弹起
	ArcBall->isClicked = false; //在ArcBall里面设置鼠标键弹起
	ArcBall->MousePt.s.X = 0; //在ArcBall设置鼠标坐标为0，0
	ArcBall->MousePt.s.Y = 0;
	ArcBall->upstate(); //更新ArcBall状态
	ReleaseCapture();  //释放鼠标捕获
	CView::OnLButtonUp(nFlags, point);
}


void COpenGL3DTransformView::OnRButtonUp(UINT nFlags , CPoint point)
{
	m_MouseDownPoint = CPoint(0, 0);  //将鼠标坐标置为0，0
	rightButtonDown = false;  //设置鼠标右键为弹起
	ReleaseCapture(); //释放鼠标捕获
	CView::OnRButtonUp(nFlags, point);
}

void COpenGL3DTransformView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_MouseDownPoint = point; //记录当前鼠标右键按下的坐标
	rightButtonDown = true;  //设置鼠标右键为按下
	SetCapture();  //设置鼠标捕获
	m_OldMousePoint = m_MouseDownPoint;  //把这个坐标设置为以前的值
	CView::OnRButtonDown(nFlags, point);
}

void COpenGL3DTransformView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (GetCapture() == this)
	{
		if (leftButtonDown){
			//把二维平面的鼠标坐标赋给MousePt
			ArcBall->MousePt.s.X = point.x; 
			ArcBall->MousePt.s.Y = point.y;
			ArcBall->upstate(); //更新ArcBall的状态

			m_OldMousePoint = point; //把这个点设置为旧的
			//Redraw the view  
			InvalidateRect(NULL, FALSE); //刷新屏幕
		}
		else if (rightButtonDown){
			m_MouseDownPoint = point; //记录当前鼠标坐标
			
			Vector3fT vec; //定义一个向量来记录当前鼠标位置和上次鼠标的位置移动的向量

			vec.s.X = m_MouseDownPoint.x - m_OldMousePoint.x; //移动向量的x分量
			vec.s.Y = m_OldMousePoint.y - m_MouseDownPoint.y; //移动向量的y分量
			vec.s.Z = 0; //移动向量的z分量

			//把移动向量加到原来的平移向量中
			translateVector.s.X += vec.s.X*0.01; //乘上一个因子调整移动的步长
			translateVector.s.Y += vec.s.Y*0.01;
			translateVector.s.Z =0;

			m_OldMousePoint = m_MouseDownPoint; //把这个点设置为旧的点
			InvalidateRect(NULL, FALSE);
		}

	};
	CView::OnMouseMove(nFlags, point);
}


BOOL COpenGL3DTransformView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	double scaleFactor;

	if (zDelta >= 0)//如果鼠标往上滑动
	{
		scaleFactor = 0.8; //设置放大因子为正的0.1
		if (m_Scale <= 0){  //如果放大倍数小于零则控制放大倍数为0.1
			m_Scale = 0.1;
		}
		else{   //如果大于0则不断增加放大倍数
			m_Scale += scaleFactor;
		}
	}
	else  //如果鼠标往下滑动
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
	ifstream ifs("MeshData/fish/result_Fish_Final.obj");//定义一个文件输入流
	string s;
	CFace* f; //面
	Tuple3fT *v; //顶点
	NormalVector *vn;  //法向量
	Texture	*vt; //纹理坐标
	while (getline(ifs, s))//把从输入流读到的字符串存到s里面
	{
		//.obj文件的数据有几种形式：
		//1、 v -0.211499 0.000134 0.082503  v代表顶点，后面是顶点坐标
		//2、 vt 0.601082 0.685057   vt代表纹理，后面是纹理坐标
		//3、 vn -0.292611 -0.000354 -0.956231  vn代表法向量，后面是法向量坐标
		//4、 f 2180/2180/12526 2181/2181/12527 2198/2198/12528  f代表面，后面有3组值
		//每组值的3个数分别代表顶点索引/纹理索引/法向量索引
		if (s.length()<2)continue;
		if (s[0] == 'v'){
			if (s[1] == 't'){//vt 0.601082 0.685057 纹理
				istringstream in(s);
				vt = new Texture();//纹理对象
				string head;
				in >> head >> vt->TU >> vt->TV; //分割字符串
				m_pic.VT.push_back(*vt); //添加到整个网格的纹理向量中
			}
			else if (s[1] == 'n'){//vn -0.292611 -0.000354 -0.956231 法向量
				istringstream in(s);
				vn = new NormalVector();//法向量对象
				string head;
				in >> head >> vn->NX >> vn->NY >> vn->NZ; //分割字符串
				m_pic.VN.push_back(*vn);//添加到整个网格的法向量向量中
			}
			else{//v-0.211499 0.000134 0.082503 点
				istringstream in(s);
				v = new Tuple3fT();//顶点对象
				string head;
				in >> head >> v->s.X >> v->s.Y >> v->s.Z; //分割字符串
				m_pic.V.push_back(*v);//添加到整个网格的顶点向量中
			}
		}
		else if (s[0] == 'f'){//f 2443//2656 2442//2656 2444//2656 面
			for (int k = s.size() - 1; k >= 0; k--){
				if (s[k] == '/')s[k] = ' ';
			}
			istringstream in(s);
			f = new CFace();//面对象
			string head;
			in >> head;
			int i = 0;
			while (i<3)
			{
				//   顶点索引/纹理索引/法向量索引
				if (m_pic.V.size() != 0)
				{
					in >> f->V[i];
					f->V[i] -= 1; //数组开始以0为下标，而不是1，所以要减去1才是对应的索引
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
			m_pic.F.push_back(*f);//添加到整个网格的面向量中
		}
	}
}



void COpenGL3DTransformView::loadTexture()
{
	//AUX_RGBImageRec类型是一个RGB图像结构类型。该结构定义了三个成员：
	//sizeX :图像的宽度
	//sizeY :图像的高度
	//data  :图形所包含的数据，其实也就是该图形在内存中的像素数据的一个指针
	AUX_RGBImageRec *TextureImage[1];//创建纹理的存储空间
	memset(TextureImage, 0, sizeof(void *) * 1);//将指针设为NULL
	//载入位图，检查有无错误，如果位图没找到则退出
	if (TextureImage[0] = LoadBMP("MeshData/fish/carp.bmp"))
	{
		glGenTextures(1, &texture[0]);//创建纹理
		//绑定来自位图数据生成的纹理
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		//生成纹理
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, TextureImage[0]->sizeX, TextureImage[0]->sizeY,
			GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//线形滤波
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//线形滤波
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//GL_REPLACE使用纹理颜色来替换原有的像素颜色
		                                                         //GL_BLEND或者GL_MODULATE，表示将光照结果与纹理颜色进行叠加
	}
	if (TextureImage[0])//纹理是否存在
	{
		if (TextureImage[0]->data)//纹理图像是否存在
		{
			free(TextureImage[0]->data);//是否纹理图像占用的内存
		}
		free(TextureImage[0]);//是否图像结构
	}
}
AUX_RGBImageRec* COpenGL3DTransformView::LoadBMP(char *Filename) //载入位图图像
{
	FILE *File = NULL; //文件句柄
	if (!Filename)//确保文件名已提供
	{
		return NULL;
	}
	File = fopen(Filename, "r");//尝试打开文件


	if (File)
	{
		fclose(File);
		return auxDIBImageLoad(Filename);//载入位图并返回指针
	}

	return NULL;
}
