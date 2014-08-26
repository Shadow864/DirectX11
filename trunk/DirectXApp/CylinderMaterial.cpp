#include "stdafx.h"
#include "CylinderMaterial.h"
#include "MeshData.h"

CylinderMaterial::CylinderMaterial()
{
}


CylinderMaterial::~CylinderMaterial()
{
}

void CylinderMaterial::CreateVertexBuffer(ID3D11Device* device, const MeshData* mesh, ID3D11Buffer** vertexBuffer) const
{
	const std::vector<DirectX::XMFLOAT3>& sourceVertices = mesh->GetPositions();

	std::vector<Vertex::Pos> vertices;
	vertices.resize(sourceVertices.size());


	for (UINT i = 0; i < mesh->GetPositions().size(); ++i)
	{
		vertices[i].Position = mesh->GetPositions()[i];
	}


	CreateVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
}
void CylinderMaterial::CreateVertexBuffer(ID3D11Device* device, Vertex::Pos* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
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

UINT CylinderMaterial::VertexSize() const
{
	return sizeof(Vertex::Pos);
}