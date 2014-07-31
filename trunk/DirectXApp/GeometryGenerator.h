#pragma once
#include "MeshData.h"
#include <string>

class Highmap;
class GeometryGenerator
{

	struct face
	{


		
		UINT i1;
		UINT i2;
		UINT i3;

		DirectX::XMVECTOR normal;

		std::vector<UINT> neighbours;
	};
public:
	GeometryGenerator(void);
	~GeometryGenerator(void);

	void Subdivide					(MeshData& meshData) const;
	void ComputeNormals				(MeshData& meshData) const;
	void ComputeNormals				(MeshData::Vertex& vertex, const std::vector<UINT>& indices, const std::vector<MeshData::Vertex>& vertices) const;
	void ComputeNormals(MeshData::Vertex& vertex, const std::vector<face>& faces) const;

	void CreateTriangle				(MeshData& meshData) const;
	void CreateSierpinskiTrinagle	(MeshData& meshData, UINT level)const;
	void CreatePyramid				(MeshData& meshData) const;
	void CreateSierpinskiPyramid	(MeshData& meshData, UINT level) const;
	void CreatePlane				(MeshData& meshData) const;
	void CreateGrid					(MeshData& meshData, float width, float depth, UINT m, UINT n) const;
	void CreateCylinder				(MeshData& meshData, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount) const;
	void CreateGeosphere			(MeshData& meshData, float radius, UINT numSubdivisions) const;
	void CreateBox					(MeshData& meshData) const;
	void CreateFromFile				(MeshData& meshData, const std::string& file_name) const;
	void CreateFromHightMap			(MeshData& meshData, const Highmap* highmap) const;
	void CreateBillboard			(MeshData& meshDate) const;

private:
	void CreateSierpinskiTrinagle	(MeshData& meshData, UINT level, UINT max_level, DirectX::XMFLOAT3 pos, float scale) const;
	void CreateSierpinskiPyramid	(MeshData& meshData, UINT level, UINT max_level, DirectX::XMFLOAT3 pos, float scale) const;
	void BuildTopCap				(MeshData& meshData, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount) const;
	void BuildBottomCap				(MeshData& meshData, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount) const;
	void BuildStacks				(MeshData& meshData, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount) const;
};

