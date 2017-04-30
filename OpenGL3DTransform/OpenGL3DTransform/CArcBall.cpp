////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////ArcBall.cpp////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                         
#include "stdafx.h"
#include <GL/glut.h>        

#include <math.h>                                    
#include "CArcBall.h"     


//轨迹球参数:
//直径                    2.0f
//半径                    1.0f
//半径平方                1.0f


void ArcBallT::Coordinate2Sphere(const Point2fT* NewPt, Vector3fT* NewVec)
{

	double xScale = 2.0 / NewWidth;
	double yScale = 2.0 / NewHeight;

	double xPos = NewPt->s.X * xScale - 1;
	double yPos = 1 - NewPt->s.Y * yScale;
	double zPos = 0.0;
	NewVec->s.X = xPos;
	NewVec->s.Y = yPos;
	NewVec->s.Z = zPos;


	double len = xPos * xPos + yPos * yPos;
	if (len < 1.0)
	{
		zPos = sqrt(1.0 - len);
		NewVec->s.Z = zPos;
	}

}





void ArcBallT::upstate()
{
	if (!this->isDragging && this->isClicked) {                                                // 如果没有拖动
		this->isDragging = true;                                        // 设置拖动为变量为true
		this->LastRot = this->ThisRot;
		this->click(&this->MousePt);
	}
	else if (this->isDragging) {
		if (this->isClicked) {                                            //如果按住拖动
			Quat4fT     ThisQuat;//旋转四元数

			//获得旋转四元数ThisQuat（旋转轴的x，旋转轴的y，旋转轴的z，两个球面坐标向量的点积）
			this->drag(&this->MousePt, &ThisQuat);                        // 更新轨迹球的变量
			//设置旋转矩阵ThisRot
			Matrix3fSetRotationFromQuat4f(&this->ThisRot, &ThisQuat);        // 计算旋转量
			//3×3矩阵相乘，新的旋转矩阵右乘旧的旋转矩阵赋给ThisRot
			Matrix3fMulMatrix3f(&this->ThisRot, &this->LastRot);
			//得到四维变换矩阵Transform
			Matrix4fSetRotationFromMatrix3f(&this->Transform, &this->ThisRot);
		}
		else                                                        // 如果放开鼠标，设置拖动为false
			this->isDragging = false;
	}
}


void ArcBallT::setWindow(GLfloat Width, GLfloat Height){
	this->NewWidth = Width;
	this->NewHeight = Height;
}

//按下鼠标,记录当前对应的轨迹球的位置
void  ArcBallT::click(const Point2fT* NewPt)
{
	this->Coordinate2Sphere(NewPt, &this->StVec);
}

//鼠标拖动,计算旋转四元数
//参数NewPt是当前鼠标的位置（二维平面）
//NewRot是旋转四元数
void  ArcBallT::drag(const Point2fT* NewPt, Quat4fT* NewRot)
{
	//新的位置
	//把鼠标现在的位置（二维平面坐标）转换到球面坐标EnVec
	this->Coordinate2Sphere(NewPt, &this->EnVec);

	//计算旋转
	if (NewRot)
	{
		Vector3fT  Perp;//旋转轴

		//计算旋转轴，调用计算三维向量叉乘函数
		Vector3fCross(&Perp, &this->StVec, &this->EnVec);

		//如果不为0
		if (Vector3fLength(&Perp) > Epsilon)
		{
			//记录旋转轴
			NewRot->s.X = Perp.s.X;
			NewRot->s.Y = Perp.s.Y;
			NewRot->s.Z = Perp.s.Z;
			
			//θ=2*arccos(m1·m2)
			//设W=m1·m2
			//所以在四元数中,W=cos(θ/2)，θ为旋转的角度
			//调用计算三维向量点积函数
			NewRot->s.W = Vector3fDot(&this->StVec, &this->EnVec);

		}
		//如果是0，说明没有旋转
		else
		{
			NewRot->s.X = 0.0f;
			NewRot->s.Y = 0.0f;
			NewRot->s.Z = 0.0f;
			NewRot->s.W = 0.0f;
		}
	}
}



