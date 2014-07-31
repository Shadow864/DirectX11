#ifndef  MESHDATA_H
#define MESHDATA_H


#include <vector>
#include <DirectXMath.h>

class IMeshData
{
public:

	virtual const void*		GetVerticesPtr		() const = 0;
	virtual const void*		GetIndicesPtr		() const = 0;
	virtual unsigned int	GetVertexSize		() const = 0;
	virtual unsigned int	GetIndexSize		() const = 0;
	virtual unsigned int	GetVerticesNumber	() const = 0;
	virtual unsigned int	GetIndicesNumber	() const = 0;

	virtual					~IMeshData			(){};
};

class MeshData : public IMeshData
{
public:
	struct Vertex
	{
		Vertex();
		Vertex(float x, float y, float z, float r, float g, float b);

		Vertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2&  size);
		Vertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3&  normal);
		Vertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3&  normal, const DirectX::XMFLOAT2& tex_coord);
		Vertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3&  normal, const DirectX::XMFLOAT4& color);


		friend std::istream & operator>>(std::istream &in, Vertex& ver);


		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 TexCoord;
		DirectX::XMFLOAT2 Size;
		DirectX::XMFLOAT4 Color;
		DirectX::XMFLOAT3 Binormal;
		DirectX::XMFLOAT3 Tangent;




	};

public:

	MeshData();

	///
	/// IMeshData implementation
	///
	virtual const void*		GetVerticesPtr		() const	{ return &m_Vertices[0];		}
	virtual const void*		GetIndicesPtr		() const	{ return &m_Indices[0];			}
	virtual unsigned int	GetVertexSize		() const	{ return sizeof(Vertex);		}
	virtual unsigned int	GetIndexSize		() const	{ return sizeof(unsigned int);	}
	virtual unsigned int	GetVerticesNumber	() const	{ return static_cast<unsigned int>(m_Vertices.size());		}
	virtual unsigned int	GetIndicesNumber	() const	{ return static_cast<unsigned int>(m_Indices.size()); }

public:
	std::vector<Vertex>			m_Vertices;
	std::vector<unsigned int>	m_Indices;

};





#endif // ! MESHDATA_H
