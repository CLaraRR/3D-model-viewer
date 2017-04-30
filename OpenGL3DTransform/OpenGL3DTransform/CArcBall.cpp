////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////ArcBall.cpp////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                         
#include "stdafx.h"
#include <GL/glut.h>        

#include <math.h>                                    
#include "CArcBall.h"     


//�켣�����:
//ֱ��                    2.0f
//�뾶                    1.0f
//�뾶ƽ��                1.0f


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
	if (!this->isDragging && this->isClicked) {                                                // ���û���϶�
		this->isDragging = true;                                        // �����϶�Ϊ����Ϊtrue
		this->LastRot = this->ThisRot;
		this->click(&this->MousePt);
	}
	else if (this->isDragging) {
		if (this->isClicked) {                                            //�����ס�϶�
			Quat4fT     ThisQuat;//��ת��Ԫ��

			//�����ת��Ԫ��ThisQuat����ת���x����ת���y����ת���z�������������������ĵ����
			this->drag(&this->MousePt, &ThisQuat);                        // ���¹켣��ı���
			//������ת����ThisRot
			Matrix3fSetRotationFromQuat4f(&this->ThisRot, &ThisQuat);        // ������ת��
			//3��3������ˣ��µ���ת�����ҳ˾ɵ���ת���󸳸�ThisRot
			Matrix3fMulMatrix3f(&this->ThisRot, &this->LastRot);
			//�õ���ά�任����Transform
			Matrix4fSetRotationFromMatrix3f(&this->Transform, &this->ThisRot);
		}
		else                                                        // ����ſ���꣬�����϶�Ϊfalse
			this->isDragging = false;
	}
}


void ArcBallT::setWindow(GLfloat Width, GLfloat Height){
	this->NewWidth = Width;
	this->NewHeight = Height;
}

//�������,��¼��ǰ��Ӧ�Ĺ켣���λ��
void  ArcBallT::click(const Point2fT* NewPt)
{
	this->Coordinate2Sphere(NewPt, &this->StVec);
}

//����϶�,������ת��Ԫ��
//����NewPt�ǵ�ǰ����λ�ã���άƽ�棩
//NewRot����ת��Ԫ��
void  ArcBallT::drag(const Point2fT* NewPt, Quat4fT* NewRot)
{
	//�µ�λ��
	//��������ڵ�λ�ã���άƽ�����꣩ת������������EnVec
	this->Coordinate2Sphere(NewPt, &this->EnVec);

	//������ת
	if (NewRot)
	{
		Vector3fT  Perp;//��ת��

		//������ת�ᣬ���ü�����ά������˺���
		Vector3fCross(&Perp, &this->StVec, &this->EnVec);

		//�����Ϊ0
		if (Vector3fLength(&Perp) > Epsilon)
		{
			//��¼��ת��
			NewRot->s.X = Perp.s.X;
			NewRot->s.Y = Perp.s.Y;
			NewRot->s.Z = Perp.s.Z;
			
			//��=2*arccos(m1��m2)
			//��W=m1��m2
			//��������Ԫ����,W=cos(��/2)����Ϊ��ת�ĽǶ�
			//���ü�����ά�����������
			NewRot->s.W = Vector3fDot(&this->StVec, &this->EnVec);

		}
		//�����0��˵��û����ת
		else
		{
			NewRot->s.X = 0.0f;
			NewRot->s.Y = 0.0f;
			NewRot->s.Z = 0.0f;
			NewRot->s.W = 0.0f;
		}
	}
}



