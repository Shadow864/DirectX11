#include "stdafx.h"
#include "BillboardMaterial.h"
#include "MeshData.h"

BillboardMaterial::BillboardMaterial()
{
	m_VertexType	 = VertexType::POS_SIZE;
	m_TopologyType	= TopologyType::POINT_LIST;
}


BillboardMaterial::~BillboardMaterial()
{
}

void BillboardMaterial::CreateVertexBuffer(ID3D11Device* device, const MeshData* mesh, ID3D11Buffer** vertexBuffer) const
{
	const std::vector<DirectX::XMFLOAT3>& sourceVertices = mesh->GetPositions();

	std::vector<Vertex::PosSize> vertices;
	vertices.resize(sourceVertices.size());


	for (UINT i = 0; i < mesh->GetPositions().size(); ++i)
	{
		vertices[i].Pos = mesh->GetPositions()[i];
	}

	for (UINT i = 0; i < mesh->GetPositions().size(); ++i)
	{
		if (i < mesh->GetSizes().size())
			vertices[i].Size = mesh->GetSizes()[i];
	}

	CreateVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
}
void BillboardMaterial::CreateVertexBuffer(ID3D11Device* device, Vertex::PosSize* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
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

UINT BillboardMaterial::VertexSize() const
{
	return sizeof(Vertex::PosSize);
}