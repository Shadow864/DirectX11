#include "stdafx.h"
#include "Material.h"
#include "Texture.h"
#include "ContentManager.h"
#include "Effects/Effect.h"
#include "d3dx11effect.h"

Material::Material()
{
	m_Ambient		= DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_Diffuse		= DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Specular		= DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0);
	m_EffectType	= EffectType::BASIC;
	m_TopologyType	= TopologyType::TRIANGLE_LIST;
	m_VertexType	= VertexType::POS_NORMAL_TEXCOORD;
}


Material::~Material()
{
}


void Material::Initialize(ID3D11Device* device)
{
	m_Effect = ContentManager::GetInstance().m_EffectManager->GetEffect(m_EffectType);
}

bool Material::LoadContent(ID3D11Device* device)
{
	if (m_DiffuseMap)
		m_DiffuseMap->LoadContent(device);

	return true;
}

bool Material::UnloadContent(ID3D11Device* device)
{
	if (m_DiffuseMap)
		m_DiffuseMap->UnloadContent(device);

	return true;
}

void Material::Update(float delta)
{
	if (m_DiffuseMap)
		m_DiffuseMap->Update(delta);
}

ID3DX11EffectTechnique* Material::GetTechnique()
{
	if (m_DiffuseMap)
	{
		return m_Effect->GetTechnique(1);
	}
	else
	{
		return  m_Effect->GetTechnique(0);
	}
}