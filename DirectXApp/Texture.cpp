#include "stdafx.h"
#include "Texture.h"

#include <DDSTextureLoader.h>
#include <d3d11.h>

ITexture::ITexture()
{
	SetScale(1, 1);
	SetPosition(0, 0);
	SetRotation(0, 0, 0);

	RecalculateWorldMatrix();
}

ITexture::~ITexture()
{

}


void ITexture::SetPosition(float x, float y)
{
	m_Translation = DirectX::XMVectorSet(x, y, 0, 1);

	RecalculateWorldMatrix();
}

void ITexture::SetRotation(float roll, float pitch, float yaw)
{
	m_Rotation = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, yaw);

	RecalculateWorldMatrix();
}

void ITexture::SetScale(float x, float y)
{
	m_Scale = DirectX::XMVectorSet(x, y, 1, 0);

	RecalculateWorldMatrix();
}

void ITexture::RecalculateWorldMatrix()
{
	m_Transform = DirectX::XMMatrixTranslationFromVector(m_Translation) * DirectX::XMMatrixRotationQuaternion(m_Rotation) * DirectX::XMMatrixScalingFromVector(m_Scale);
}

Texture::Texture(const std::wstring& name)
 :m_Name(name)
{

}


Texture::~Texture()
{
}

bool Texture::LoadContent(ID3D11Device* device)
{

	HRESULT hr = DirectX::CreateDDSTextureFromFile(device, m_Name.c_str(), nullptr, &m_Resource);

	return true;
}

bool Texture::UnloadContent(ID3D11Device* device)
{
	if (m_Resource)
		m_Resource->Release();
	m_Resource = nullptr;

	return true;
}

void Texture::Update(float delta)
{

}

AnimateTexture::AnimateTexture(const std::wstring& name, int frames)
: m_CurrentFrame(0)
, m_CurrentTime(0)
, m_MaxFrame(frames)
, m_AnimTime(0.f)
, m_Ratio(15.0f)
, m_Name(name)
{
	m_AnimTime = m_MaxFrame / m_Ratio;
}

AnimateTexture::~AnimateTexture()
{

}

bool AnimateTexture::LoadContent(ID3D11Device* device)
{
	for (int i = 1; i < m_MaxFrame + 1; ++i)
	{
		ID3D11ShaderResourceView* fire;

		std::wstring name = m_Name;

		if (i < 100)
			name += std::to_wstring(0);

		if (i < 10)
		name += std::to_wstring(0);
		
		name += std::to_wstring(i);

		name += L".dds";

		
		HRESULT hr =  DirectX::CreateDDSTextureFromFile(device, name.c_str(), nullptr, &fire);

		m_Resources.push_back(fire);

	}

	return true;
}

bool AnimateTexture::UnloadContent(ID3D11Device* device)
{
	for each (auto resource in m_Resources)
	{
		resource->Release();
	}
	return true;
}

void AnimateTexture::Update(float delta)
{
	m_CurrentTime += delta;

	while(m_CurrentTime > m_AnimTime)
		m_CurrentTime -= m_AnimTime;

	m_CurrentFrame = static_cast<int>((m_CurrentTime / m_AnimTime) * ((float)m_MaxFrame));
}