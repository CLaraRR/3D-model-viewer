
// OpenGL3DTransformView.h : COpenGL3DTransformView ��Ľӿ�
//

#pragma once
class ArcBall;
#include "CArcBall.h"
#include "CModel.h"
class COpenGL3DTransformView : public CView
{
protected: // �������л�����
	COpenGL3DTransformView();
	DECLARE_DYNCREATE(COpenGL3DTransformView)

// ����
public:
	COpenGL3DTransformDoc* GetDocument() const;

// ����
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
	GLuint texture[1];//�洢һ������

	BOOL InitializeOpenGL();    //Initialize OpenGL  
	BOOL SetupPixelFormat();    //Set up the Pixel Format  
	void RenderScene();            //Render the Scene  
	void DrawCylinder();
	void show3DModel();
	void loadTexture();
	AUX_RGBImageRec* LoadBMP(char *Filename);

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~COpenGL3DTransformView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // OpenGL3DTransformView.cpp �еĵ��԰汾
inline COpenGL3DTransformDoc* COpenGL3DTransformView::GetDocument() const
   { return reinterpret_cast<COpenGL3DTransformDoc*>(m_pDocument); }
#endif

