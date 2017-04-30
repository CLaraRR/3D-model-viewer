#include <vector>
#include "CArcBall.h"
using namespace std;

//��������
typedef struct Texture{
	double TU;
	double TV;
}CTexture;

//����������
typedef struct NormalVector{
	double NX;
	double NY;
	double NZ;
}CNormalVector;

//��Ľṹ��ʾ
typedef struct Face{
	int V[3];
	int T[3];
	int N[3];
}CFace;


class CMesh{
public:
	vector<Tuple3fT> V;  //V�������㡣��ʽΪV X Y Z��V�����X Y Z��ʾ�����������ꡣ������
	vector<Texture> VT;  //��ʾ�������ꡣ��ʽΪVT TU TV��������
	vector<NormalVector> VN;  //VN����������ÿ�������ε��������㶼Ҫָ��һ������������ʽΪVN NX NY NZ��������
	vector<Face> F;  //F���档�������ŵ�����ֵ�ֱ������������Ķ��㡢�������ꡢ��������������
	//��ĸ�ʽΪ��f Vertex1/Texture1/Normal1 Vertex2/Texture2/Normal2 Vertex3/Texture3/Normal3
};