#ifndef  MESHDATA_H
#define MESHDATA_H


#include <vector>
#include <DirectXMath.h>
class GeometryGenerator;
class Model;

struct ID3D11Buffer;

#define DECLARE_MESH_FIELD(TYPE, NAME)				\
	public:											\
	const std::vector<##TYPE>& Get ##NAME() const	\
		{											\
			return 	m_##NAME;						\
		}											\
	public:										\
	std::vector<##TYPE>	m_##NAME;					\


class MeshData 
{
	DECLARE_MESH_FIELD(DirectX::XMFLOAT3, Positions)
	DECLARE_MESH_FIELD(DirectX::XMFLOAT3, Normals)
	DECLARE_MESH_FIELD(DirectX::XMFLOAT3, Binormals)
	DECLARE_MESH_FIELD(DirectX::XMFLOAT3, Tangents)
	DECLARE_MESH_FIELD(DirectX::XMFLOAT2, TexCoords)
	DECLARE_MESH_FIELD(DirectX::XMFLOAT2, Sizes)
	DECLARE_MESH_FIELD(DirectX::XMFLOAT4, Colors)
	DECLARE_MESH_FIELD(UINT, Indices)


	friend GeometryGenerator;
	friend Model;
};
//
//std::istream & operator>>(std::istream &in, DirectX::XMFLOAT3& ver)
//{
//
//	in >> ver.x;
//	in >> ver.y;
//	in >> ver.z;
//
//	return in;
//}
//
//
//std::istream & operator>>(std::istream &in, DirectX::XMFLOAT2& ver)
//{
//
//	in >> ver.x;
//	in >> ver.y;
//
//	return in;
//}
//


#endif // ! MESHDATA_H
