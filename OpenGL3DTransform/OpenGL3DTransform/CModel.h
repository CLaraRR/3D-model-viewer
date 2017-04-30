#include <vector>
#include "CArcBall.h"
using namespace std;

//纹理坐标
typedef struct Texture{
	double TU;
	double TV;
}CTexture;

//法向量坐标
typedef struct NormalVector{
	double NX;
	double NY;
	double NZ;
}CNormalVector;

//面的结构表示
typedef struct Face{
	int V[3];
	int T[3];
	int N[3];
}CFace;


class CMesh{
public:
	vector<Tuple3fT> V;  //V：代表顶点。格式为V X Y Z，V后面的X Y Z表示三个顶点坐标。浮点型
	vector<Texture> VT;  //表示纹理坐标。格式为VT TU TV。浮点型
	vector<NormalVector> VN;  //VN：法向量。每个三角形的三个顶点都要指定一个法向量。格式为VN NX NY NZ。浮点型
	vector<Face> F;  //F：面。面后面跟着的整型值分别是属于这个面的顶点、纹理坐标、法向量的索引。
	//面的格式为：f Vertex1/Texture1/Normal1 Vertex2/Texture2/Normal2 Vertex3/Texture3/Normal3
};