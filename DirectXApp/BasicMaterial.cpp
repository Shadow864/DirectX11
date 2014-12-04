#include "stdafx.h"
#include "BasicMaterial.h"
#include "MeshData.h"

BasicMaterial::BasicMaterial()
{
	m_VertexType	= VertexType::POS_NORMAL_TEXCOORD;
	m_TopologyType	= TopologyType::TRIANGLE_LIST;
}


BasicMaterial::~BasicMaterial()
{
}


void BasicMaterial::CreateVertexBuffer(ID3D11Device* device, const MeshData* mesh, ID3D11Buffer** vertexBuffer) const
{
	const std::vector<DirectX::XMFLOAT3>& sourceVertices = mesh->GetPositions();

	std::vector<Vertex::PosNormalTexCoord> vertices;
	vertices.resize(sourceVertices.size());
	

	for (UINT i = 0; i < mesh->GetPositions().size(); ++i)
	{
		vertices[i].Pos = mesh->GetPositions()[i];
	}

	for (UINT i = 0; i < mesh->GetPositions().size(); ++i)
	{
		if (i < mesh->GetNormals().size())
			vertices[i].Normal = mesh->GetNormals()[i];
	}

	for (UINT i = 0; i < mesh->GetPositions().size(); ++i)
	{
		if (i < mesh->GetTexCoords().size())
			vertices[i].TexCoord = mesh->GetTexCoords()[i];
	}
	
	
	CreateVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
}
void BasicMaterial::CreateVertexBuffer(ID3D11Device* device, Vertex::PosNormalTexCoord* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = VertexSize() * vertexCount;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData;
	ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
	vertexSubResourceData.pSysMem = vertices;
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer)))
	{
		
	}
}

UINT BasicMaterial::VertexSize() const
{
	return sizeof(Vertex::PosNormalTexCoord);
}