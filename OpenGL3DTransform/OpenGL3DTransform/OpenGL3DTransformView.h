
// OpenGL3DTransformView.h : COpenGL3DTransformView 类的接口
//

#pragma once
class ArcBall;
#include "CArcBall.h"
#include "CModel.h"
class COpenGL3DTransformView : public CView
{
protected: // 仅从序列化创建
	COpenGL3DTransformView();
	DECLARE_DYNCREATE(COpenGL3DTransformView)

// 特性
public:
	COpenGL3DTransformDoc* GetDocument() const;

// 操作
public:
	HGLRC m_hRC;    //Rendering Context  
	CDC* m_pDC;        //Device Context  

	GLfloat m_xAngle;
	GLfloat m_yAngle;
	GLfloat m_xPos;
	GLfloat m_yPos;
	CPoint m_MouseDownPoint;
	CPoint m_OldMousePoint;
	Vector3fT translateVector;

	float m_Scale;
    ArcBallT* ArcBall;
	bool leftButtonDown;
	bool rightButtonDown;
	CMesh m_pic;
	GLuint texture[1];//存储一个纹理

	BOOL InitializeOpenGL();    //Initialize OpenGL  
	BOOL SetupPixelFormat();    //Set up the Pixel Format  
	void RenderScene();            //Render the Scene  
	void DrawCylinder();
	void show3DModel();
	void loadTexture();
	AUX_RGBImageRec* LoadBMP(char *Filename);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~COpenGL3DTransformView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	void ReadOBJ();
};

#ifndef _DEBUG  // OpenGL3DTransformView.cpp 中的调试版本
inline COpenGL3DTransformDoc* COpenGL3DTransformView::GetDocument() const
   { return reinterpret_cast<COpenGL3DTransformDoc*>(m_pDocument); }
#endif

