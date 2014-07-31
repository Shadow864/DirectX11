#include "stdafx.h"
#include "MeshData.h"

MeshData::Vertex::Vertex()
	: Position(0,0,0)
	, Color(1.f,1.f,1.f,1.f)
	, Normal(0,0,0)
	, Binormal(0,0,0)
	, Tangent(0,0,0)
	, TexCoord(0,0)

{
}

MeshData::Vertex::Vertex(float x, float y, float z, float r, float g, float b)
	: Position(x, y, z)
	, Normal(0,0,0)
	, Color(r, g, b, 1.0f)
{
}


MeshData::Vertex::Vertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2&  size)
: Position(position)
, Size(size.x,size.y)
, Color(1.f, 1.f, 1.f, 1.f)
{

}

MeshData::Vertex::Vertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3&  normal)
	: Position(position)
	, Normal(normal)
	, Color(1.f, 1.f, 1.f, 1.f)
{

}

MeshData::Vertex::Vertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& normal, const DirectX::XMFLOAT2& tex_coord)
: Position(position)
, Normal(normal)
, TexCoord(tex_coord)
, Color(1.f, 1.f, 1.f, 1.f)
{

}

MeshData::Vertex::Vertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3&  normal, const DirectX::XMFLOAT4& color)
	: Position(position)
	, Normal(normal)
	, Color(color)
{

}

std::istream & operator>>(std::istream &in, MeshData::Vertex& ver)
{

	in >> ver.Position.x;
	in >> ver.Position.y;
	in >> ver.Position.z;

	in >> ver.Normal.x;
	in >> ver.Normal.y;
	in >> ver.Normal.z;

	//ver.Color.x = ver.Normal.x;
	//ver.Color.y = ver.Normal.y;
	//ver.Color.z = ver.Normal.z;
	return in;
}

MeshData::MeshData()
{

	/*m_Vertices.push_back(Vertex(10.0f, -5.0f, -5.0f, 1.0f, 0.0f, 0.0f));
	m_Vertices.push_back(Vertex(10.0f,  5.0f,  5.0f, 0.0f, 1.0f, 0.0f));
	m_Vertices.push_back(Vertex(10.0f, -5.0f,  5.0f, 0.0f, 0.0f, 1.0f));
	m_Vertices.push_back(Vertex(10.0f,  5.0f, -5.0f, 1.0f, 1.0f, 0.0f));
	
	m_Indices.push_back(2);
	m_Indices.push_back(1);
	m_Indices.push_back(0);
	
	m_Indices.push_back(0);
	m_Indices.push_back(1);
	m_Indices.push_back(3);*/
}
