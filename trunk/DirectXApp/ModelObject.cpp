#include "stdafx.h"

#include <d3d11.h>

#include <sstream>
#include <iostream>
#include <fstream>


#include "ModelObject.h"
#include "GeometryGenerator.h"

#include "Material.h"

#include "Effects/EffectsManager.h"
#include "Effects/Effect.h"

#include "RenderState.h"

#include "MathHelper.h"

#include "Texture.h"

#include "ContentManager.h"
#include "GraphicsDevice.h"

ModelObject::ModelObject()
: m_Material(nullptr)
, m_CastShadow(false)
{
	SetScale(1, 1, 1);

	if (!m_Data)
		m_Data = std::make_shared<MeshData>();

	m_Material = ContentManager::GetInstance().m_MaterialManager->Default;
}

void ModelObject::SetPosition(float x, float y, float z)
{
	m_Translation = DirectX::XMVectorSet(x, y, z, 0);

	RecalculateWorldMatrix();
}

void ModelObject::SetRotation(float roll, float pitch, float yaw)
{
	m_Pith = pitch;
	m_Roll = roll;
	m_Yaw = yaw;

	m_Rotation = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(roll), DirectX::XMConvertToRadians(pitch), DirectX::XMConvertToRadians(yaw));

	RecalculateWorldMatrix();
}

void ModelObject::SetScale(float x, float y, float z)
{
	m_Scale = DirectX::XMVectorSet(x, y, z, 0);

	RecalculateWorldMatrix();
}

void ModelObject::RecalculateWorldMatrix()
{
	m_WorldMatrix = DirectX::XMMatrixRotationQuaternion(m_Rotation) * DirectX::XMMatrixScalingFromVector(m_Scale)*  DirectX::XMMatrixTranslationFromVector(m_Translation);
	m_WorldMatrix = m_WorldMatrix;
}

ModelObject::~ModelObject(void)
{

}

void ModelObject::Initialize(ID3D11Device* device)
{

	if (m_Material)
		m_Material->Initialize(device);


	m_Material->CreateVertexBuffer(device, m_Data.get(), &m_VerticesBuffer);


	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)* m_Data->GetIndices().size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &m_Data->GetIndices()[0];
	device->CreateBuffer(&ibd, &iinitData, &m_IndicesBuffer);



}

void ModelObject::Render(GraphicsDevice* device)
{

}

bool ModelObject::LoadContent(ID3D11Device* device)
{
	if (m_Material)
		m_Material->LoadContent(device);

	return true;
}

bool ModelObject::UnloadContent(ID3D11Device* device)
{

	if (m_Material)
		m_Material->UnloadContent(device);


	return true;
}

void ModelObject::Update(float delta)
{

	if (m_Material)
		m_Material->Update(delta);
}

void ModelObject::SetMaterial(std::shared_ptr<Material> material)
{
	m_Material = material;
}

Triangle::Triangle()
{
	GeometryGenerator gen;

	gen.CreateTriangle(*m_Data);
}

Pyramid::Pyramid()
{
	GeometryGenerator gen;

	gen.CreatePyramid(*m_Data);
}

SierpinskiTriangle::SierpinskiTriangle(UINT level)
{
	GeometryGenerator gen;

	gen.CreateSierpinskiTrinagle(*m_Data, level);
}

SierpinskiPyramid::SierpinskiPyramid(UINT level)
{
	GeometryGenerator gen;

	gen.CreateSierpinskiPyramid(*m_Data, level);
}

Plane::Plane()
{
	GeometryGenerator gen;

	gen.CreatePlane(*m_Data);
}

Grid::Grid(float width, float depth, UINT m, UINT n)
{
	GeometryGenerator gen;

	gen.CreateGrid(*m_Data, width * 2.f, depth * 2.f, m, n);
}

Cylinder::Cylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount)
{
	GeometryGenerator gen;

	gen.CreateCylinder(*m_Data, bottomRadius, topRadius, height, sliceCount, stackCount);
}

Geosphere::Geosphere(float radius, UINT numSubdivisions)
{
	GeometryGenerator gen;

	gen.CreateGeosphere(*m_Data, radius, numSubdivisions);
}

Box::Box()
{
	GeometryGenerator gen;

	gen.CreateBox(*m_Data);
}

Wave::Wave(float width, float depth, UINT m, UINT n)
: m_Time(0)
, m_Width(width)
, m_Depth(depth)
, m_M(m)
, m_N(n)
{
	GeometryGenerator gen;

	gen.CreateGrid(*m_Data, width, depth, m, n);
}

void Wave::Update(float delta)
{
	m_Time += delta;

	if (m_Material && m_Material->m_DiffuseMap)
		m_Material->m_DiffuseMap->SetPosition(m_Time * 0.005, m_Time * 0.005);

	UpdateMesh();
}

void Wave::UpdateMesh()
{
	for (UINT i = 0; i < m_M; ++i)
	{
		for (UINT j = 0; j < m_N; ++j)
		{
			m_Data->m_Positions[i * m_M + j].y = GetHeight(i, j, m_M, m_N, m_Time);
			m_Data->m_Normals[i * m_M + j] = GetNormal(i, j, m_M, m_N, m_Time);
		}
	}
}

float Wave::GetHeight(UINT x, UINT z, UINT m, UINT n, float time) const
{
	return GetHeight(((float)x) / ((float)m) - 0.5f, ((float)z) / ((float)n) - 0.5f, time);
}

float Wave::GetHeight(float x, float z, float time) const
{
	float result = 0.5f *  (z*sinf(x * 8.f + time) + x*cosf(z*8.f));

	return result;
}

DirectX::XMFLOAT3 Wave::GetNormal(UINT x, UINT z, UINT m, UINT n, float time) const
{
	return GetNormal(((float)x) / ((float)m) - 0.5f, ((float)z) / ((float)n) - 0.5f, time);
}

DirectX::XMFLOAT3 Wave::GetNormal(float x, float z, float time) const
{

	float nor_x = 0.05f * z * cosf(x * 8.f + time) + 0.5f * cosf(z*8.f);
	float nor_y = 1;
	float nor_z = 0.5f * sinf(x * 8.f + time) + 0.05f * x*sinf(z*8.f);

	DirectX::XMFLOAT3 normal(-nor_x, nor_y, nor_z);

	DirectX::XMVECTOR unitNormal = DirectX::XMVector2Normalize(DirectX::XMLoadFloat3(&normal));

	DirectX::XMStoreFloat3(&normal, unitNormal);

	return normal;
}


TestObject::TestObject(const std::string& file_name)
{
	GeometryGenerator gen;

	gen.CreateFromFile(*m_Data, file_name);
}

void TestObject::Update(float delta)
{
	SetRotation(m_Roll, m_Pith + 10 * delta, m_Yaw);
}


MirrorObject::MirrorObject(float width, float depth, UINT m, UINT n)
: Grid(width, depth, m, n)
{

}

void MirrorObject::Render(GraphicsDevice* device)
{

}

Billboard::Billboard()
{
	GeometryGenerator gen;

	gen.CreatePointWithSize(*m_Data);
}


Circle::Circle()
{
	GeometryGenerator gen;

	gen.CreateCircle(*m_Data, 0.5f, 12);
}



Point::Point()
{
	GeometryGenerator gen;

	gen.CreatePointWithSize(*m_Data);
}
