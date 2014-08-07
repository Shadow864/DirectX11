#include "stdafx.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "Highmap.h"


GeometryGenerator::GeometryGenerator(void)
{
}


GeometryGenerator::~GeometryGenerator(void)
{
}



void GeometryGenerator::Subdivide(MeshData& meshData) const
{
	// Save a copy of the input geometry.
	MeshData inputCopy = meshData;


	meshData.m_Positions.resize(0);
	meshData.m_Indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	//UINT numTris = inputCopy.m_Indices.size()/3;
	//for(UINT i = 0; i < numTris; ++i)
	//{
	//	MeshData::Vertex v0 = inputCopy.m_Vertices[inputCopy.m_Indices[i * 3 + 0]];
	//	MeshData::Vertex v1 = inputCopy.m_Vertices[inputCopy.m_Indices[i * 3 + 1]];
	//	MeshData::Vertex v2 = inputCopy.m_Vertices[inputCopy.m_Indices[i * 3 + 2]];

	//	//
	//	// Generate the midpoints.
	//	//

	//	MeshData::Vertex m0, m1, m2;

	//	// For subdivision, we just care about the position component.  We derive the other
	//	// vertex components in CreateGeosphere.

	//	m0.Position = DirectX::XMFLOAT3(
	//		0.5f*(v0.Position.x + v1.Position.x),
	//		0.5f*(v0.Position.y + v1.Position.y),
	//		0.5f*(v0.Position.z + v1.Position.z));

	//	m1.Position = DirectX::XMFLOAT3(
	//		0.5f*(v1.Position.x + v2.Position.x),
	//		0.5f*(v1.Position.y + v2.Position.y),
	//		0.5f*(v1.Position.z + v2.Position.z));

	//	m2.Position = DirectX::XMFLOAT3(
	//		0.5f*(v0.Position.x + v2.Position.x),
	//		0.5f*(v0.Position.y + v2.Position.y),
	//		0.5f*(v0.Position.z + v2.Position.z));

	//	//
	//	// Add new geometry.
	//	//

	//	meshData.m_Vertices.push_back(v0); // 0
	//	meshData.m_Vertices.push_back(v1); // 1
	//	meshData.m_Vertices.push_back(v2); // 2
	//	meshData.m_Vertices.push_back(m0); // 3
	//	meshData.m_Vertices.push_back(m1); // 4
	//	meshData.m_Vertices.push_back(m2); // 5
 //
	//	meshData.m_Indices.push_back(i*6+0);
	//	meshData.m_Indices.push_back(i*6+3);
	//	meshData.m_Indices.push_back(i*6+5);

	//	meshData.m_Indices.push_back(i*6+3);
	//	meshData.m_Indices.push_back(i*6+4);
	//	meshData.m_Indices.push_back(i*6+5);

	//	meshData.m_Indices.push_back(i*6+5);
	//	meshData.m_Indices.push_back(i*6+4);
	//	meshData.m_Indices.push_back(i*6+2);

	//	meshData.m_Indices.push_back(i*6+3);
	//	meshData.m_Indices.push_back(i*6+1);
	//	meshData.m_Indices.push_back(i*6+4);
	//}
}

void GeometryGenerator::ComputeNormals(MeshData& meshData) const
{
	for (UINT i = 0; i < meshData.m_Positions.size(); ++i)
	{

		std::vector<UINT> neightboors; 


		for (UINT j = 0; j < meshData.m_Indices.size();)
		{
			// Check if contains this point
			if (meshData.m_Indices[j + 0] == i ||
				meshData.m_Indices[j + 1] == i ||
				meshData.m_Indices[j + 2] == i
				)
			{
				neightboors.push_back(meshData.m_Indices[j + 0]);
				neightboors.push_back(meshData.m_Indices[j + 1]);
				neightboors.push_back(meshData.m_Indices[j + 2]);
			}

			j = j + 3;
		}

		
		//ComputeNormals(meshData.m_Vertices[i], neightboors, meshData.m_Vertices);
	}
}

//void GeometryGenerator::ComputeNormals(MeshData::Vertex& vertex, const std::vector<UINT>& indices, const std::vector<MeshData::Vertex>& vertices) const
//{
//	DirectX::XMVECTOR normal = DirectX::XMVectorZero();
//	for (UINT i = 0; i < indices.size();)
//	{
//		DirectX::XMVECTOR p0 = DirectX::XMLoadFloat3(&vertices[indices[i + 0]].Position);
//		DirectX::XMVECTOR p1 = DirectX::XMLoadFloat3(&vertices[indices[i + 1]].Position);
//		DirectX::XMVECTOR p2 = DirectX::XMLoadFloat3(&vertices[indices[i + 2]].Position);
//
//		DirectX::XMVECTOR v0 = DirectX::XMVectorSubtract(p0, p1);
//		DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(p1, p2);
//
//		DirectX::XMVECTOR face_normal = DirectX::XMVector3Cross(v0, v1);
//
//		normal = DirectX::XMVectorAdd(normal, face_normal);
//
//		i = i + 3;
//	}
//
//	normal = DirectX::XMVector3Normalize(normal);
//
//	DirectX::XMStoreFloat3(&vertex.Normal, normal);
//}
void GeometryGenerator::ComputeNormals(MeshData& meshData, const std::vector<face>& faces, UINT index) const
{
	DirectX::XMVECTOR normal = DirectX::XMVectorZero();
	for each (auto face in faces)
	{
		normal = DirectX::XMVectorAdd(normal, face.normal);
	}

	normal = DirectX::XMVector3Normalize(normal);

	DirectX::XMStoreFloat3(&meshData.m_Normals[index], normal);
}

void GeometryGenerator::CreateTriangle(MeshData& meshData) const
{

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, 0.f, 1.f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0, 0));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, 0.f, 1.f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0.5, 1));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, 0.f, 1.f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(1, 0));

	meshData.m_Indices.push_back(0);
	meshData.m_Indices.push_back(1);
	meshData.m_Indices.push_back(2);

	ComputeNormals(meshData);
}

void GeometryGenerator::CreatePyramid(MeshData& meshData) const
{
	meshData.m_Indices.clear();
	meshData.m_Positions.clear();
	meshData.m_Normals.clear();


	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f));
		
	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, 1.f, 0.f));

	meshData.m_Indices.push_back(0);
	meshData.m_Indices.push_back(1);
	meshData.m_Indices.push_back(4);

	meshData.m_Indices.push_back(1);
	meshData.m_Indices.push_back(2);
	meshData.m_Indices.push_back(4);

	meshData.m_Indices.push_back(2);
	meshData.m_Indices.push_back(3);
	meshData.m_Indices.push_back(4);

	meshData.m_Indices.push_back(3);
	meshData.m_Indices.push_back(0);
	meshData.m_Indices.push_back(4);

	meshData.m_Indices.push_back(2);
	meshData.m_Indices.push_back(1);
	meshData.m_Indices.push_back(0);

	meshData.m_Indices.push_back(0);
	meshData.m_Indices.push_back(3);
	meshData.m_Indices.push_back(2);

	ComputeNormals(meshData);
}

void GeometryGenerator::CreateSierpinskiTrinagle(MeshData& meshData, UINT level) const
{
	meshData.m_Indices.clear();
	meshData.m_Positions.clear();

	CreateSierpinskiTrinagle(meshData, 0, level, DirectX::XMFLOAT3(0,0,0), 0.5f);

	ComputeNormals(meshData);
}

void GeometryGenerator::CreateSierpinskiTrinagle(MeshData& meshData, UINT level, UINT max_level, DirectX::XMFLOAT3 pos, float scale) const
{

	float current_scale = scale;

	float y = pos.y;
	float x = pos.x;
	float z = pos.z;

	if ( level == max_level)
	{
		meshData.m_Positions.push_back(DirectX::XMFLOAT3(pos.x - scale, pos.y - scale, pos.z));
		meshData.m_Normals.push_back(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

		meshData.m_Positions.push_back(DirectX::XMFLOAT3(pos.x, pos.y + scale, pos.z));
		meshData.m_Normals.push_back(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

		meshData.m_Positions.push_back(DirectX::XMFLOAT3(pos.x + scale, pos.y - scale, pos.z));
		meshData.m_Normals.push_back(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 3);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 2);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 1);

	}
	else  
	{
		CreateSierpinskiTrinagle(meshData, level + 1, max_level, DirectX::XMFLOAT3(x - current_scale * 0.5f	, y - current_scale* 0.5f   , z ) , current_scale * 0.5f);
		CreateSierpinskiTrinagle(meshData, level + 1, max_level, DirectX::XMFLOAT3(x + current_scale * 0.5f	, y - current_scale* 0.5f   , z ) , current_scale * 0.5f);
		CreateSierpinskiTrinagle(meshData, level + 1, max_level, DirectX::XMFLOAT3(x							, y + current_scale * 0.5f	, z	) , current_scale * 0.5f);
	}
}

void GeometryGenerator:: CreateSierpinskiPyramid(MeshData& meshData, UINT level) const
{
	meshData.m_Indices.clear();
	meshData.m_Positions.clear();

	CreateSierpinskiPyramid(meshData, 0, level, DirectX::XMFLOAT3(0,0,0), 0.5f);

	ComputeNormals(meshData);
}


void GeometryGenerator::CreateSierpinskiPyramid(MeshData& meshData, UINT level, UINT max_level, DirectX::XMFLOAT3 pos, float scale) const
{
	float current_scale = scale;

	float y = pos.y;
	float x = pos.x;
	float z = pos.z;

	if ( level == max_level)
	{

		meshData.m_Positions.push_back(DirectX::XMFLOAT3(pos.x - scale, pos.y - scale, pos.z - scale));
		meshData.m_Positions.push_back(DirectX::XMFLOAT3(pos.x - scale, pos.y - scale, pos.z + scale));
		meshData.m_Positions.push_back(DirectX::XMFLOAT3(pos.x + scale, pos.y - scale, pos.z + scale));
		meshData.m_Positions.push_back(DirectX::XMFLOAT3(pos.x + scale, pos.y - scale, pos.z - scale));
		meshData.m_Positions.push_back(DirectX::XMFLOAT3(pos.x, pos.y + scale, pos.z));

		meshData.m_Normals.push_back(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f));
		meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
		meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));
		meshData.m_Normals.push_back(DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f));
		meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f));


		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 5);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 4);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 1);

		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 4);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 3);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 1);

		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 3);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 2);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 1);

		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 2);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 5);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 1);

		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 3);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 4);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 5);

		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 5);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 2);
		meshData.m_Indices.push_back(static_cast<UINT>(meshData.m_Positions.size()) - 3);

	}
	else  
	{
		CreateSierpinskiPyramid(meshData, level + 1, max_level, DirectX::XMFLOAT3(x - current_scale * 0.5f	, y - current_scale* 0.5f   , z + current_scale * 0.5f ) , current_scale * 0.5f);
		CreateSierpinskiPyramid(meshData, level + 1, max_level, DirectX::XMFLOAT3(x - current_scale * 0.5f	, y - current_scale* 0.5f   , z - current_scale * 0.5f ) , current_scale * 0.5f);
		CreateSierpinskiPyramid(meshData, level + 1, max_level, DirectX::XMFLOAT3(x + current_scale * 0.5f	, y - current_scale* 0.5f   , z + current_scale * 0.5f ) , current_scale * 0.5f);
		CreateSierpinskiPyramid(meshData, level + 1, max_level, DirectX::XMFLOAT3(x + current_scale * 0.5f	, y - current_scale* 0.5f   , z - current_scale * 0.5f ) , current_scale * 0.5f);
		CreateSierpinskiPyramid(meshData, level + 1, max_level, DirectX::XMFLOAT3(x							, y + current_scale * 0.5f	, z	)						 , current_scale * 0.5f);
	}
}


void GeometryGenerator::CreatePlane(MeshData& meshData) const
{
	meshData.m_Indices.clear();
	meshData.m_Positions.clear();

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f));
	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f));
	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f));
	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f));

	meshData.m_Normals.push_back(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	meshData.m_Indices.push_back(2);
	meshData.m_Indices.push_back(1);
	meshData.m_Indices.push_back(0);

	meshData.m_Indices.push_back(0);
	meshData.m_Indices.push_back(3);
	meshData.m_Indices.push_back(2);

	ComputeNormals(meshData);
}

void GeometryGenerator::CreateGrid(MeshData& meshData, float width, float depth, UINT m, UINT n) const
{
	meshData.m_Indices.clear();
	meshData.m_Positions.clear();

	UINT vertexCount = m*n;
	UINT faceCount = (m-1)*(n-1)*2;
	//
	// Create the vertices.
	//
	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;
	float dx = width / (n-1);
	float dz = depth / (m-1);
	float du = 1.0f / (n-1);
	float dv = 1.0f / (m-1);

	meshData.m_Positions.resize(vertexCount);
	meshData.m_Normals.resize(vertexCount);
	meshData.m_Tangents.resize(vertexCount);
	meshData.m_TexCoords.resize(vertexCount);
	for(UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dz;
		for(UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;
			meshData.m_Positions[i*n + j] = DirectX::XMFLOAT3(x, 0, z);
			// Ignore for now, used for lighting.
			meshData.m_Normals[i*n + j] = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			meshData.m_Tangents[i*n + j] = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			// Ignore for now, used for texturing.
			meshData.m_TexCoords[i*n + j].x = j*du;
			meshData.m_TexCoords[i*n + j].y = i*dv;
		}	
	}

	meshData.m_Indices.resize(faceCount*3); // 3 indices per face
	// Iterate over each quad and compute indices.
	UINT k = 0;
	for(UINT i = 0; i < m-1; ++i)
	{
		for(UINT j = 0; j < n-1; ++j)
		{
			meshData.m_Indices[k] = i*n+j;
			meshData.m_Indices[k+1] = i*n+j+1;
			meshData.m_Indices[k+2] = (i+1)*n+j;

			meshData.m_Indices[k+3] = (i+1)*n+j;
			meshData.m_Indices[k+4] = i*n+j+1;
			meshData.m_Indices[k+5] = (i+1)*n+j+1;
			k += 6; // next quad
		}
	}

	ComputeNormals(meshData);
}

void GeometryGenerator::CreateCylinder(MeshData& meshData, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount) const
{
	meshData.m_Indices.clear();
	meshData.m_Positions.clear();

	BuildTopCap(meshData, bottomRadius, topRadius, height, sliceCount, stackCount);
	
	BuildStacks(meshData, bottomRadius, topRadius, height, sliceCount, stackCount);

	BuildBottomCap(meshData, bottomRadius, topRadius, height, sliceCount, stackCount);

	ComputeNormals(meshData);
}

void GeometryGenerator::BuildTopCap(MeshData& meshData, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount) const
{
	float theta			= 2.f * DirectX::XM_PI / sliceCount;
	float y = height/2;

	UINT indicex_index = meshData.m_Positions.size();

	for (UINT slice = 0; slice < sliceCount; ++slice)
	{

		float radius = topRadius;

		float x = radius * cosf(theta * slice);
		
		float z = radius * sinf(theta * slice);

		meshData.m_Positions.push_back(DirectX::XMFLOAT3(x, y, z));
		meshData.m_Normals.push_back(DirectX::XMFLOAT3(1, 1, 1));
	}

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0, y, 0));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(1, 1, 1));

	for (UINT slice = 0; slice < sliceCount; ++slice)
	{

		meshData.m_Indices.push_back(indicex_index + sliceCount);
		meshData.m_Indices.push_back(indicex_index + (slice + 1 == sliceCount ? 0 : slice + 1));
		meshData.m_Indices.push_back(indicex_index + slice);
	}
}
	
void GeometryGenerator::BuildBottomCap(MeshData& meshData, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount) const
{
	float theta			= 2.f * DirectX::XM_PI / sliceCount;
	float y				= -height/2;

	UINT indicex_index = meshData.m_Positions.size();

	for (UINT slice = 0; slice < sliceCount; ++slice)
	{

		float radius = bottomRadius;

		float x = radius * cosf(theta * slice);
		
		float z = radius * sinf(theta * slice);

		meshData.m_Positions.push_back(DirectX::XMFLOAT3(x, y, z));
		meshData.m_Normals.push_back(DirectX::XMFLOAT3(1, 1, 1));
	}

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0, y, 0));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(1, 1, 1));

	for (UINT slice = 0; slice < sliceCount; ++slice)
	{
		meshData.m_Indices.push_back(indicex_index + sliceCount);
		meshData.m_Indices.push_back(indicex_index + slice);
		meshData.m_Indices.push_back(indicex_index + (slice + 1 == sliceCount ? 0 : slice + 1));
	}
}

void GeometryGenerator::BuildStacks(MeshData& meshData, float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount) const
{
	float theta			= 2.f * DirectX::XM_PI / sliceCount;
	float delta_radius	= (topRadius - bottomRadius) / stackCount;
	float delta_height	= height/stackCount;
	float height_half	= height/2.f;

	UINT indicex_index = meshData.m_Positions.size();
	for (UINT stack = 0; stack < stackCount + 1; ++stack)
	{
		for (UINT slice = 0; slice < sliceCount; ++slice)
		{

			float radius = bottomRadius + stack * delta_radius;

			float x = radius * cosf(theta * slice);
			float y = -height_half + stack * delta_height;
			float z = radius * sinf(theta * slice);

			meshData.m_Positions.push_back(DirectX::XMFLOAT3(x, y, z));
			meshData.m_Normals.push_back(DirectX::XMFLOAT3(1, 1, 1));
		}
	}

	UINT ringVertexCount = sliceCount;

	for (UINT stack = 0; stack < stackCount; ++stack)
	{
		for (UINT slice = 0; slice < sliceCount; ++slice)
		{
			meshData.m_Indices.push_back(indicex_index + stack*ringVertexCount + slice);
			meshData.m_Indices.push_back(indicex_index + (stack+1)*ringVertexCount + slice);
			meshData.m_Indices.push_back(indicex_index + (stack + 1)*ringVertexCount + (slice + 1 == sliceCount ? 0 : slice + 1));

			meshData.m_Indices.push_back(indicex_index + stack*ringVertexCount + slice);
			meshData.m_Indices.push_back(indicex_index + (stack+1)*ringVertexCount + (slice + 1 == sliceCount? 0: slice + 1));
			meshData.m_Indices.push_back(indicex_index + stack*ringVertexCount + (slice + 1 == sliceCount ? 0 : slice + 1));
		}
	}
}



void GeometryGenerator::CreateGeosphere(MeshData& meshData, float radius, UINT numSubdivisions) const
{
	// Put a cap on the number of subdivisions.
	//numSubdivisions = MathHelper::Min(numSubdivisions, 5u);

	// Approximate a sphere by tessellating an icosahedron.

	meshData.m_Indices.clear();
	meshData.m_Positions.clear();

	const float X = 0.525731f; 
	const float Z = 0.850651f;

	DirectX::XMFLOAT3 pos[12] = 
	{
		DirectX::XMFLOAT3(-X, 0.0f, Z),  DirectX::XMFLOAT3(X, 0.0f, Z),  
		DirectX::XMFLOAT3(-X, 0.0f, -Z), DirectX::XMFLOAT3(X, 0.0f, -Z),    
		DirectX::XMFLOAT3(0.0f, Z, X),   DirectX::XMFLOAT3(0.0f, Z, -X), 
		DirectX::XMFLOAT3(0.0f, -Z, X),  DirectX::XMFLOAT3(0.0f, -Z, -X),    
		DirectX::XMFLOAT3(Z, X, 0.0f),   DirectX::XMFLOAT3(-Z, X, 0.0f), 
		DirectX::XMFLOAT3(Z, -X, 0.0f),  DirectX::XMFLOAT3(-Z, -X, 0.0f)
	};

	DWORD k[60] = 
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};

	meshData.m_Positions.resize(12);
	meshData.m_Normals.resize(12);
	meshData.m_TexCoords.resize(12);
	meshData.m_Tangents.resize(12);
	meshData.m_Indices.resize(60);

	for (UINT i = 0; i < 12; ++i)
		meshData.m_Positions[i] = pos[i];

	for(UINT i = 0; i < 60; ++i)
		meshData.m_Indices[i] = k[i];

	for(UINT i = 0; i < numSubdivisions; ++i)
		Subdivide(meshData);

	// Project vertices onto sphere and scale.
	for (UINT i = 0; i < meshData.m_Positions.size(); ++i)
	{
		// Project onto unit sphere.
		DirectX::XMVECTOR n = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&meshData.m_Positions[i]));
	
		// Project onto sphere.
		DirectX::XMVECTOR p = DirectX::operator*(n,radius);
		DirectX::XMStoreFloat3(&meshData.m_Positions[i], p);
		DirectX::XMStoreFloat3(&meshData.m_Normals[i], n);

		// Derive texture coordinates from spherical coordinates.
		float theta = MathHelper::AngleFromXY(
			meshData.m_Positions[i].x,
			meshData.m_Positions[i].z);

		float phi = acosf(meshData.m_Positions[i].y / radius);

		meshData.m_TexCoords[i].x = theta / DirectX::XM_2PI;
		meshData.m_TexCoords[i].y = phi / DirectX::XM_PI;

		// Partial derivative of P with respect to theta
		meshData.m_Tangents[i].x = -radius*sinf(phi)*sinf(theta);
		meshData.m_Tangents[i].y = 0.0f;
		meshData.m_Tangents[i].z = +radius*sinf(phi)*cosf(theta);

		DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&meshData.m_Tangents[i]);
		DirectX::XMStoreFloat3(&meshData.m_Tangents[i], DirectX::XMVector3Normalize(T));
	}
}

void GeometryGenerator::CreateBox(MeshData& meshData) const
{
	meshData.m_Positions.clear();
	meshData.m_Indices.clear();
	meshData.m_TexCoords.clear();


	//Front
	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, -0.0f, -1.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0.f, 1.0f));
	
	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, -0.0f, -1.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0.f, 0.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, -0.0f, -1.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(1.f, 0.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, -0.0f, -1.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(1.f, 1.0f));

	//Back
	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, -0.0f, 1.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(1.f, 1.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, -0.0f, 1.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(1.f, 0.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, -0.0f, 1.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0.f, 0.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, -0.0f, 1.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0.f, 1.0f));

	// Left
	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(-1.f, -0.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(1.f, 1.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(-1.f, -0.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(1.f, 0.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(-1.f, -0.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0.f, 0.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(-1.f, -0.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0.f, 1.0f));

	// Right
	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(1.f, -0.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0.f, 1.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(1.f, -0.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0.f, 0.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(1.f, -0.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(1.f, 0.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(1.f, -0.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(1.f, 1.0f));

	// Top
	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, 1.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0.f, 1.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, 1.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0.f, 0.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, 1.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(1.f, 0.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, 1.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(1.f, 1.0f));

	// Bottom
	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, -1.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0.f, 0.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, -1.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(0.f, 1.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, -1.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(1.f, 1.0f));

	meshData.m_Positions.push_back(DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f));
	meshData.m_Normals.push_back(DirectX::XMFLOAT3(0.f, -1.0f, 0.0f));
	meshData.m_TexCoords.push_back(DirectX::XMFLOAT2(1.f, 0.0f));

	meshData.m_Indices.push_back(2);
	meshData.m_Indices.push_back(3);
	meshData.m_Indices.push_back(0);


	meshData.m_Indices.push_back(6);
	meshData.m_Indices.push_back(5);
	meshData.m_Indices.push_back(4);

	meshData.m_Indices.push_back(4);
	meshData.m_Indices.push_back(7);
	meshData.m_Indices.push_back(6);

	meshData.m_Indices.push_back(10);
	meshData.m_Indices.push_back(9);
	meshData.m_Indices.push_back(8);

	meshData.m_Indices.push_back(8);
	meshData.m_Indices.push_back(11);
	meshData.m_Indices.push_back(10);

	meshData.m_Indices.push_back(12);
	meshData.m_Indices.push_back(13);
	meshData.m_Indices.push_back(14);

	meshData.m_Indices.push_back(14);
	meshData.m_Indices.push_back(15);
	meshData.m_Indices.push_back(12);

	meshData.m_Indices.push_back(16);
	meshData.m_Indices.push_back(17);
	meshData.m_Indices.push_back(18);

	meshData.m_Indices.push_back(18);
	meshData.m_Indices.push_back(19);
	meshData.m_Indices.push_back(16);

	meshData.m_Indices.push_back(22);
	meshData.m_Indices.push_back(21);
	meshData.m_Indices.push_back(20);

	meshData.m_Indices.push_back(20);
	meshData.m_Indices.push_back(23);
	meshData.m_Indices.push_back(22);

	meshData.m_Indices.push_back(0);
	meshData.m_Indices.push_back(1);
	meshData.m_Indices.push_back(2);

}

void GeometryGenerator::CreateFromFile(MeshData& meshData, const std::string& file_name) const
{
	meshData.m_Indices.clear();
	meshData.m_Positions.clear();
	meshData.m_Normals.clear();
	meshData.m_TexCoords.clear();

	std::ifstream file(file_name.c_str());

	if(!file)
	{
		MessageBox(0, L"File missing", 0, 0);
		return;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	file >> ignore >> vcount;
	file >> ignore >> tcount;
	file >> ignore >> ignore >> ignore >> ignore;

	meshData.m_Positions.resize(vcount);
	meshData.m_Normals.resize(vcount);
	//meshData.m_TexCoords.resize(vcount);

	float m = 0;
	for (UINT i = 0; i < vcount; ++i)
	{
		
		file >> meshData.m_Positions[i].x;
		file >> meshData.m_Positions[i].y;
		file >> meshData.m_Positions[i].z;

		file >> meshData.m_Normals[i].x;
		file >> meshData.m_Normals[i].y;
		file >> meshData.m_Normals[i].z;

		//file >> meshData.m_TexCoords[i].x;
		//file >> meshData.m_TexCoords[i].y;

		m = max(m, abs(meshData.m_Positions[i].x));
		m = max(m, abs(meshData.m_Positions[i].y));
		m = max(m, abs(meshData.m_Positions[i].z));
	}

	if ( m > 1.f)
	{
		for (UINT i = 0; i < meshData.m_Positions.size(); ++i)
		{
			meshData.m_Positions[i].x /= m;
			meshData.m_Positions[i].y /= m;
			meshData.m_Positions[i].z /= m;
		}
	}

	file >> ignore >> ignore >> ignore;
	
	for (UINT i = 0; i < tcount * 3; ++i)
	{
		UINT ind;

		file >> ind;

		meshData.m_Indices.push_back(ind);
	}
}

void GeometryGenerator::CreateFromHightMap(MeshData& meshData, const Highmap* highmap) const
{
	meshData.m_Indices.clear();
	meshData.m_Positions.clear();

	UINT m = highmap->GetWidth();
	UINT n = highmap->GetHeight();

	float width = highmap->GetWidth();
	float depth = highmap->GetHeight();

	UINT vertexCount = m*n;
	UINT faceCount = (m - 1)*(n - 1) * 2;
	//
	// Create the vertices.
	//
	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;
	float dx = width / (n - 1);
	float dz = depth / (m - 1);
	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);
	meshData.m_Positions.resize(vertexCount);
	meshData.m_Normals.resize(vertexCount);
	meshData.m_TexCoords.resize(vertexCount);
	meshData.m_Tangents.resize(vertexCount);

	std::vector<face> faces;

	faces.resize(faceCount);

	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dz;
		for (UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;
			meshData.m_Positions[i*n + j] = DirectX::XMFLOAT3(x, highmap->GetValue(i, j) * 40, z);
			// Ignore for now, used for lighting.

			//Highmap::HeightMapType no = highmap->GetNormal(i, j);
			//DirectX::XMFLOAT3 normal;

			//DirectX::XMVECTOR normal_vec= DirectX::XMLoadFloat3(&normal);

			//normal.x = no.x;
			//normal.y = no.y;
			//normal.z = no.z;

			//normal_vec = DirectX::XMVector3Normalize(normal_vec);
			//DirectX::XMStoreFloat3(&meshData.m_Vertices[i*n + j].Normal, normal_vec);

			//DirectX::XMVector3Normalize


			//meshData.m_Vertices[i*n + j].Normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			meshData.m_Tangents[i*n + j] = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			// Ignore for now, used for texturing.
			meshData.m_TexCoords[i*n + j].x = j*du;
			meshData.m_TexCoords[i*n + j].y = i*dv;
		}
	}

	meshData.m_Indices.resize(faceCount * 3); // 3 indices per face
	// Iterate over each quad and compute indices.
	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			meshData.m_Indices[k] = i*n + j;
			meshData.m_Indices[k + 1] = i*n + j + 1;
			meshData.m_Indices[k + 2] = (i + 1)*n + j;


			faces[k / 3].i1 = i*n + j;
			faces[k / 3].i2 = i*n + j + 1;
			faces[k / 3].i3 = (i + 1)*n + j;



			DirectX::XMVECTOR p0 = DirectX::XMLoadFloat3(&meshData.m_Positions[faces[k / 3].i1]);
			DirectX::XMVECTOR p1 = DirectX::XMLoadFloat3(&meshData.m_Positions[faces[k / 3].i2]);
			DirectX::XMVECTOR p2 = DirectX::XMLoadFloat3(&meshData.m_Positions[faces[k / 3].i3]);

			DirectX::XMVECTOR v0 = DirectX::XMVectorSubtract(p0, p1);
			DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(p1, p2);

			DirectX::XMVECTOR face_normal = DirectX::XMVector3Cross(v0, v1);

			faces[k / 3].normal = face_normal;

			meshData.m_Indices[k + 3] = (i + 1)*n + j;
			meshData.m_Indices[k + 4] = i*n + j + 1;
			meshData.m_Indices[k + 5] = (i + 1)*n + j + 1;

			faces[(k + 3) / 3].i1 = (i + 1)*n + j;
			faces[(k + 3) / 3].i2 = i*n + j + 1;
			faces[(k + 3) / 3].i3 = (i + 1)*n + j + 1;


			p0 = DirectX::XMLoadFloat3(&meshData.m_Positions[faces[(k + 3) / 3].i1]);
			p1 = DirectX::XMLoadFloat3(&meshData.m_Positions[faces[(k + 3) / 3].i2]);
			p2 = DirectX::XMLoadFloat3(&meshData.m_Positions[faces[(k + 3) / 3].i3]);
			
			v0 = DirectX::XMVectorSubtract(p0, p1);
			v1 = DirectX::XMVectorSubtract(p1, p2);

			face_normal = DirectX::XMVector3Cross(v0, v1);

			faces[(k + 3) / 3].normal = face_normal;

			k += 6; // next quad
		}
	}


	for (UINT i = 0; i < m; ++i)
	{
		for (UINT j = 0; j < n; ++j)
		{

			std::vector<face> neightboors;


			int vertex_index = i*n + j;
		
			int faceUpR1 = -1;
			int faceBottomR1 = -1;
			int faceBottomR2 = -1;


			int faceUpL1 = - 1;
			int faceUpL2 = -1;
			int faceBottomL1 = -1;

		
			faceUpR1 = (vertex_index - i) * 2;
			faceBottomL1 = faceUpR1 - ((vertex_index - i) * 2 - 1);
			faceBottomR1 = faceBottomL1 + 1;
			faceBottomR2 = faceBottomL1 + 2;

			faceUpL1 = faceUpR1 - 1;
			faceUpL2 = faceUpR1 - 2;

			if (i < n - 2 && j < m - 2)
			{
				neightboors.push_back(faces[faceUpR1]);
			}
			

			if (i > 1 && j > 1)
			{
				neightboors.push_back(faces[faceBottomL1]);
			}
		

			if (i > 1 && j < m - 2)
			{
				neightboors.push_back(faces[faceBottomR1]);
				neightboors.push_back(faces[faceBottomR2]);
			}

			if (i < n - 1 && j > 1)
			{
				neightboors.push_back(faces[faceUpL1]);
				neightboors.push_back(faces[faceUpL2]);
			}


			ComputeNormals(meshData, neightboors, vertex_index);
		}
	}
}

void GeometryGenerator::CreateBillboard(MeshData& meshData) const
{
	meshData.m_Positions.push_back(DirectX::XMFLOAT3(-0.0f, -0.0f, 0.0f));
	meshData.m_Sizes.push_back(DirectX::XMFLOAT2(1.f, 1.f));
	meshData.m_Indices.push_back(0);
}