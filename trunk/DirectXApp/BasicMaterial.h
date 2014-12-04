#pragma once
#include "Material.h"
#include "Vertex.h"

class BasicMaterial :
	public Material
{
public:
	BasicMaterial();
	~BasicMaterial();

	void CreateVertexBuffer(ID3D11Device* device, const MeshData* mesh, ID3D11Buffer** vertexBuffer) const override;
	UINT VertexSize() const override;

private:
	void CreateVertexBuffer(ID3D11Device* device, Vertex::PosNormalTexCoord* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const;
	
};

