#pragma once
#include "Material.h"
#include "Vertex.h"

class CylinderMaterial :
	public Material
{
public:
	CylinderMaterial();
	~CylinderMaterial();

	void CreateVertexBuffer(ID3D11Device* device, const MeshData* mesh, ID3D11Buffer** vertexBuffer) const override;
	UINT VertexSize() const override;

private:
	void CreateVertexBuffer(ID3D11Device* device, Vertex::Pos* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const;

};