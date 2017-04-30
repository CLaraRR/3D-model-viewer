#pragma once
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////ArcBall.h///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _ArcBall_h
#define _ArcBall_h

// ����Debugģʽ�£����ö���
#ifdef _DEBUG
#include "stdafx.h"
# include "assert.h"
#include "GL/glut.h"
#include "math.h"
#include <vector>
using namespace std;

#else
# define assert(x) { }
#endif


//2ά��
typedef union Tuple2f_t
{
	struct
	{
		GLfloat X, Y;
	} s;

	GLfloat T[2];
} Tuple2fT;

//3ά��
typedef union Tuple3f_t
{
	struct
	{
		GLfloat X, Y, Z;
	} s;

	GLfloat T[3];
} Tuple3fT;

//4ά��
typedef union Tuple4f_t
{
	struct
	{
		GLfloat X, Y, Z, W;
	} s;

	GLfloat T[4];
} Tuple4fT;


//3x3����
typedef union Matrix3f_t
{
	struct
	{
		//column major
		union { GLfloat M00; GLfloat XX; GLfloat SX; };
		union { GLfloat M10; GLfloat XY; };
		union { GLfloat M20; GLfloat XZ; };
		union { GLfloat M01; GLfloat YX; };
		union { GLfloat M11; GLfloat YY; GLfloat SY; };
		union { GLfloat M21; GLfloat YZ; };
		union { GLfloat M02; GLfloat ZX; };
		union { GLfloat M12; GLfloat ZY; };
		union { GLfloat M22; GLfloat ZZ; GLfloat SZ; };
	} s;
	GLfloat M[9];
} Matrix3fT;

//4x4����
typedef union Matrix4f_t
{
	struct
	{
		//column major
		union { GLfloat M00; GLfloat XX; GLfloat SX; };
		union { GLfloat M10; GLfloat XY; };
		union { GLfloat M20; GLfloat XZ; };
		union { GLfloat M30; GLfloat XW; };
		union { GLfloat M01; GLfloat YX; };
		union { GLfloat M11; GLfloat YY; GLfloat SY; };
		union { GLfloat M21; GLfloat YZ; };
		union { GLfloat M31; GLfloat YW; };
		union { GLfloat M02; GLfloat ZX; };
		union { GLfloat M12; GLfloat ZY; };
		union { GLfloat M22; GLfloat ZZ; GLfloat SZ; };
		union { GLfloat M32; GLfloat ZW; };
		union { GLfloat M03; GLfloat TX; };
		union { GLfloat M13; GLfloat TY; };
		union { GLfloat M23; GLfloat TZ; };
		union { GLfloat M33; GLfloat TW; GLfloat SW; };
	} s;
	GLfloat M[16];
} Matrix4fT;


//�������͵ı���
#define Point2fT    Tuple2fT   
#define Point3fT    Tuple3fT
#define Quat4fT     Tuple4fT   
#define Vector2fT   Tuple2fT   
#define Vector3fT   Tuple3fT   
#define FuncSqrt    sqrtf
#define Epsilon     1.0e-5


//2ά�����
inline
static void Point2fAdd(Point2fT* NewObj, const Tuple2fT* t1)
{
	assert(NewObj && t1);

	NewObj->s.X += t1->s.X;
	NewObj->s.Y += t1->s.Y;
}

//2ά�����
inline
static void Point2fSub(Point2fT* NewObj, const Tuple2fT* t1)
{
	assert(NewObj && t1);

	NewObj->s.X -= t1->s.X;
	NewObj->s.Y -= t1->s.Y;
}

//3ά�����������
inline
static void Vector3fCross(Vector3fT* NewObj, const Vector3fT* v1, const Vector3fT* v2)
{
	Vector3fT Result;

	assert(NewObj && v1 && v2);
	//��ά����������㹫ʽ��
	//a��b=(x1,y1,z1)����x2,y2,z2��=(y1*z2-z1*y2,z1*x2-x1*z2,x1*y2-y1*x2)
	Result.s.X = (v1->s.Y * v2->s.Z) - (v1->s.Z * v2->s.Y);
	Result.s.Y = (v1->s.Z * v2->s.X) - (v1->s.X * v2->s.Z);
	Result.s.Z = (v1->s.X * v2->s.Y) - (v1->s.Y * v2->s.X);

	*NewObj = Result;
}

//3ά�������
inline
static GLfloat Vector3fDot(const Vector3fT* NewObj, const Vector3fT* v1)
{
	assert(NewObj && v1);

	return  (NewObj->s.X * v1->s.X) +
		(NewObj->s.Y * v1->s.Y) +
		(NewObj->s.Z * v1->s.Z);
}

//3ά�����ĳ��ȵ�ƽ��
inline
static GLfloat Vector3fLengthSquared(const Vector3fT* NewObj)
{
	assert(NewObj);

	return  (NewObj->s.X * NewObj->s.X) +
		(NewObj->s.Y * NewObj->s.Y) +
		(NewObj->s.Z * NewObj->s.Z);
}

//3ά�����ĳ���
inline
static GLfloat Vector3fLength(const Vector3fT* NewObj)
{ 
	assert(NewObj);

	return sqrt(Vector3fLengthSquared(NewObj));
}

//����3x3����Ϊ0����
inline
static void Matrix3fSetZero(Matrix3fT* NewObj)
{
	NewObj->s.M00 = NewObj->s.M01 = NewObj->s.M02 =
		NewObj->s.M10 = NewObj->s.M11 = NewObj->s.M12 =
		NewObj->s.M20 = NewObj->s.M21 = NewObj->s.M22 = 0.0f;

}

//����4x4����Ϊ0����
inline
static void Matrix4fSetZero(Matrix4fT* NewObj)
{
	NewObj->s.M00 = NewObj->s.M01 = NewObj->s.M02 = NewObj->s.M03=
		NewObj->s.M10 = NewObj->s.M11 = NewObj->s.M12 = NewObj->s.M13=
		NewObj->s.M20 = NewObj->s.M21 = NewObj->s.M22 = NewObj->s.M23=
		NewObj->s.M30 = NewObj->s.M31 = NewObj->s.M32 = 0.0f;

}

//����3x3����Ϊ��λ����
inline
static void Matrix3fSetIdentity(Matrix3fT* NewObj)
{
	Matrix3fSetZero(NewObj);
	NewObj->s.M00 =
		NewObj->s.M11 =
		NewObj->s.M22 = 1.0f;
}

//����4x4����Ϊ��λ����
inline
static void Matrix4fSetIdentity(Matrix4fT* NewObj)
{
	Matrix4fSetZero(NewObj);

	NewObj->s.M00 = 1.0f;
	NewObj->s.M11 = 1.0f;
	NewObj->s.M22 = 1.0f;
	NewObj->s.M33 = 1.0f;
}

//����Ԫ��������ת����
inline
static void Matrix3fSetRotationFromQuat4f(Matrix3fT* NewObj, const Quat4fT* q1)
{


	double angle = 2 * acos(q1->s.W);
	double s = sin(angle);
	double c = cos(angle);
    #define  ZERO_TOL    1.0e-7
	if (fabs(q1->s.Z) < ZERO_TOL)
	{
		if (fabs(q1->s.Y) < ZERO_TOL)
		{
			//if the axis is (0, 0, 0), assume to be identity matrix
			if (fabs(q1->s.X) < ZERO_TOL)
				return ;

			//rotation axis is (1, 0, 0)

			NewObj->s.M00 = 1.0;
			NewObj->s.M11 = c;
			NewObj->s.M22 = c;

			if (q1->s.X > 0)
			{
				NewObj->s.M12 = -s;
				NewObj->s.M21 = s;
			}
			else
			{
				NewObj->s.M12 = s;
				NewObj->s.M21 = -s;
			}

			return ;

		}
		else if (fabs(q1->s.X) < ZERO_TOL)
		{
			//rotation axis is (0, 1, 0)
			NewObj->s.M11 = 1.0;
			NewObj->s.M00 = c;
			NewObj->s.M22 = c;

			if (q1->s.Y > 0)
			{
				NewObj->s.M02 = s;
				NewObj->s.M20 = -s;
			}
			else
			{
				NewObj->s.M02 = -s;
				NewObj->s.M20 = s;
			}

			return;
		}
	}
	else if (fabs(q1->s.Y) < ZERO_TOL)
	{
		if (fabs(q1->s.X) < ZERO_TOL)
		{
			//rotation axis is (0, 0, 1)
			NewObj->s.M22 = 1.0;
			NewObj->s.M00 = c;
			NewObj->s.M11 = c;

			if (q1->s.Z > 0)
			{
				NewObj->s.M01 = -s;
				NewObj->s.M10 = s;
			}
			else
			{
				NewObj->s.M01 = s;
				NewObj->s.M10 = -s;
			}

			return;
		}
	}




	//common case

	//normalize the rotation axis
	double mag = sqrt(q1->s.X * q1->s.X + q1->s.Y * q1->s.Y + q1->s.Z * q1->s.Z);
	mag = 1.0 / mag;
	double x = q1->s.X*mag;
	double y = q1->s.Y* mag;
	double z = q1->s.Z* mag;

	double t = 1.0 - c;

	double tx = t * x;
	double ty = t * y;
	double tz = t * z;
	double sx = s * x;
	double sy = s * y;
	double sz = s * z;

	//-----------------------------------------------------------
	//		| t*x*x + c		t*x*y - s*z		t*x*z + s*y |
	//		|											|
	//	R = | t*x*y + s*z	t*y*y + c		t*y*z - s*x |
	//		|											|
	//		| t*x*z - s*y	t*y*z + s*x		t*z*z + c	|
	//
	// where c = cos(theta), s = sin(theta), t = 1 - c and(x, y, z) is a unit
	// vector on the axis of rotation.
	//-----------------------------------------------------------

	// row one
	NewObj->s.M00 = tx * x + c;
	NewObj->s.M01 = tx * y - sz;
	NewObj->s.M02 = tx * z + sy;

	// row two
	NewObj->s.M10 = NewObj->s.M01 + sz + sz;
	NewObj->s.M11 = ty * y + c;
	NewObj->s.M12 = ty * z - sx;

	// row three
	NewObj->s.M20 = NewObj->s.M02 - sy - sy;
	NewObj->s.M21 = NewObj->s.M12 + sx + sx;
	NewObj->s.M22 = tz * z + c;

	return ;




}

//3x3�������
inline
static void Matrix3fMulMatrix3f(Matrix3fT* NewObj, const Matrix3fT* m1)
{
	Matrix3fT Result;

	assert(NewObj && m1);

	Result.s.M00 = (NewObj->s.M00 * m1->s.M00) + (NewObj->s.M01 * m1->s.M10) + (NewObj->s.M02 * m1->s.M20);
	Result.s.M01 = (NewObj->s.M00 * m1->s.M01) + (NewObj->s.M01 * m1->s.M11) + (NewObj->s.M02 * m1->s.M21);
	Result.s.M02 = (NewObj->s.M00 * m1->s.M02) + (NewObj->s.M01 * m1->s.M12) + (NewObj->s.M02 * m1->s.M22);

	Result.s.M10 = (NewObj->s.M10 * m1->s.M00) + (NewObj->s.M11 * m1->s.M10) + (NewObj->s.M12 * m1->s.M20);
	Result.s.M11 = (NewObj->s.M10 * m1->s.M01) + (NewObj->s.M11 * m1->s.M11) + (NewObj->s.M12 * m1->s.M21);
	Result.s.M12 = (NewObj->s.M10 * m1->s.M02) + (NewObj->s.M11 * m1->s.M12) + (NewObj->s.M12 * m1->s.M22);

	Result.s.M20 = (NewObj->s.M20 * m1->s.M00) + (NewObj->s.M21 * m1->s.M10) + (NewObj->s.M22 * m1->s.M20);
	Result.s.M21 = (NewObj->s.M20 * m1->s.M01) + (NewObj->s.M21 * m1->s.M11) + (NewObj->s.M22 * m1->s.M21);
	Result.s.M22 = (NewObj->s.M20 * m1->s.M02) + (NewObj->s.M21 * m1->s.M12) + (NewObj->s.M22 * m1->s.M22);

	*NewObj = Result;
}

//4x4�������
inline
static void Matrix4fSetRotationScaleFromMatrix4f(Matrix4fT* NewObj, const Matrix4fT* m1)
{
	assert(NewObj && m1);

	NewObj->s.XX = m1->s.XX; NewObj->s.YX = m1->s.YX; NewObj->s.ZX = m1->s.ZX;
	NewObj->s.XY = m1->s.XY; NewObj->s.YY = m1->s.YY; NewObj->s.ZY = m1->s.ZY;
	NewObj->s.XZ = m1->s.XZ; NewObj->s.YZ = m1->s.YZ; NewObj->s.ZZ = m1->s.ZZ;

}

//���о��������ֵ�ֽ⣬��ת���󱻱��浽rot3��rot4�У����ؾ������������
inline
static GLfloat Matrix4fSVD(const Matrix4fT* NewObj, Matrix3fT* rot3, Matrix4fT* rot4)
{
	GLfloat s, n;

	assert(NewObj);

	s = FuncSqrt(
		((NewObj->s.XX * NewObj->s.XX) + (NewObj->s.XY * NewObj->s.XY) + (NewObj->s.XZ * NewObj->s.XZ) +
		(NewObj->s.YX * NewObj->s.YX) + (NewObj->s.YY * NewObj->s.YY) + (NewObj->s.YZ * NewObj->s.YZ) +
		(NewObj->s.ZX * NewObj->s.ZX) + (NewObj->s.ZY * NewObj->s.ZY) + (NewObj->s.ZZ * NewObj->s.ZZ)) / 3.0f);

	if (rot3)
	{
		rot3->s.XX = NewObj->s.XX; rot3->s.XY = NewObj->s.XY; rot3->s.XZ = NewObj->s.XZ;
		rot3->s.YX = NewObj->s.YX; rot3->s.YY = NewObj->s.YY; rot3->s.YZ = NewObj->s.YZ;
		rot3->s.ZX = NewObj->s.ZX; rot3->s.ZY = NewObj->s.ZY; rot3->s.ZZ = NewObj->s.ZZ;

		n = 1.0f / FuncSqrt((NewObj->s.XX * NewObj->s.XX) +
			(NewObj->s.XY * NewObj->s.XY) +
			(NewObj->s.XZ * NewObj->s.XZ));
		rot3->s.XX *= n;
		rot3->s.XY *= n;
		rot3->s.XZ *= n;

		n = 1.0f / FuncSqrt((NewObj->s.YX * NewObj->s.YX) +
			(NewObj->s.YY * NewObj->s.YY) +
			(NewObj->s.YZ * NewObj->s.YZ));
		rot3->s.YX *= n;
		rot3->s.YY *= n;
		rot3->s.YZ *= n;

		n = 1.0f / FuncSqrt((NewObj->s.ZX * NewObj->s.ZX) +
			(NewObj->s.ZY * NewObj->s.ZY) +
			(NewObj->s.ZZ * NewObj->s.ZZ));
		rot3->s.ZX *= n;
		rot3->s.ZY *= n;
		rot3->s.ZZ *= n;
	}

	if (rot4)
	{
		if (rot4 != NewObj)
		{
			Matrix4fSetRotationScaleFromMatrix4f(rot4, NewObj);
		}


		n = 1.0f / FuncSqrt((NewObj->s.XX * NewObj->s.XX) +
			(NewObj->s.XY * NewObj->s.XY) +
			(NewObj->s.XZ * NewObj->s.XZ));
		rot4->s.XX *= n;
		rot4->s.XY *= n;
		rot4->s.XZ *= n;

		n = 1.0f / FuncSqrt((NewObj->s.YX * NewObj->s.YX) +
			(NewObj->s.YY * NewObj->s.YY) +
			(NewObj->s.YZ * NewObj->s.YZ));
		rot4->s.YX *= n;
		rot4->s.YY *= n;
		rot4->s.YZ *= n;

		n = 1.0f / FuncSqrt((NewObj->s.ZX * NewObj->s.ZX) +
			(NewObj->s.ZY * NewObj->s.ZY) +
			(NewObj->s.ZZ * NewObj->s.ZZ));
		rot4->s.ZX *= n;
		rot4->s.ZY *= n;
		rot4->s.ZZ *= n;
	}

	return s;
}

//��3x3�����Ϊ4x4����ת����
inline
static void Matrix4fSetRotationScaleFromMatrix3f(Matrix4fT* NewObj, const Matrix3fT* m1)
{
	assert(NewObj && m1);

	NewObj->s.XX = m1->s.XX; NewObj->s.YX = m1->s.YX; NewObj->s.ZX = m1->s.ZX;
	NewObj->s.XY = m1->s.XY; NewObj->s.YY = m1->s.YY; NewObj->s.ZY = m1->s.ZY;
	NewObj->s.XZ = m1->s.XZ; NewObj->s.YZ = m1->s.YZ; NewObj->s.ZZ = m1->s.ZZ;

}

//4x4�����������ĳ˻�
inline
static void Matrix4fMulRotationScale(Matrix4fT* NewObj, GLfloat scale)
{
	assert(NewObj);

	NewObj->s.XX *= scale; NewObj->s.YX *= scale; NewObj->s.ZX *= scale;
	NewObj->s.XY *= scale; NewObj->s.YY *= scale; NewObj->s.ZY *= scale;
	NewObj->s.XZ *= scale; NewObj->s.YZ *= scale; NewObj->s.ZZ *= scale;
}

//������ת����
inline
static void Matrix4fSetRotationFromMatrix3f(Matrix4fT* NewObj, const Matrix3fT* m1)
{
	assert(NewObj && m1);

	//��3x3�����Ϊ4x4����ת����
	Matrix4fSetRotationScaleFromMatrix3f(NewObj, m1);
}


class ArcBallT
{
protected:
	//�Ѷ�ά��ӳ�䵽��ά��
	inline
		void Coordinate2Sphere(const Point2fT* NewPt, Vector3fT* NewVec);

public:
	//����/��������
	//ArcBallT(GLfloat NewWidth, GLfloat NewHeight);
	ArcBallT(){
		this->MousePt.s.X = 0.0f;
		this->MousePt.s.Y = 0.0f;
		this->StVec.s.X = 0.0f;
		this->StVec.s.Y = 0.0f;
		this->StVec.s.Z = 0.0f;

		this->EnVec.s.X = 0.0f;
		this->EnVec.s.Y = 0.0f;
		this->EnVec.s.Z = 0.0f;


		Matrix4fSetIdentity(&Transform);
		Matrix3fSetIdentity(&LastRot);
		Matrix3fSetIdentity(&ThisRot);

		this->isDragging = false;
		this->isClicked = false;

	}
	~ArcBallT() { };

	void setWindow(GLfloat Width, GLfloat Height);
	//�����
	void    click(const Point2fT* NewPt);

	//����϶�������ת
	void    drag(const Point2fT* NewPt, Quat4fT* NewRot);

	//�������״̬
	void    upstate();

protected:
	Vector3fT   StVec;          //���������������
	Vector3fT   EnVec;          //��������϶�������


public:
	Matrix4fT   Transform;      //����任            
	Matrix3fT   LastRot;        //��һ�ε���ת 
	Matrix3fT   ThisRot;        //��ε���ת

	bool        isDragging;     // �Ƿ��϶�
	bool        isClicked;      // �Ƿ������
	Point2fT    MousePt;        // ��ǰ�����λ��
	GLfloat NewWidth;
	GLfloat NewHeight;
};

#endif