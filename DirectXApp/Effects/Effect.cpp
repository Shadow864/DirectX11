#include "stdafx.h"
#include "Effect.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <vector>
#include <fstream>

#include "../Material.h"
#include "../Texture.h"
Effect::Effect(ID3D11Device* device, const std::wstring& filename)
 : m_Effect(nullptr)
{
	CreateEffectFromMemory(device, filename);

	//m_Effect->GetConstantBufferByIndex

	ID3D11ShaderReflection* pReflector = nullptr;
	
	//HRESULT hr = D3DReflect(m_Effect->, m_Effect->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
}

Effect::~Effect()
{
	if (m_Effect)
		m_Effect->Release();

	m_Effect = nullptr;
}

HRESULT Effect::CreateEffectFromMemory(ID3D11Device* device, const std::wstring& filename)
{
	std::ifstream fin(filename.c_str(), std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);

	if (size == -1)
		return STG_E_FILENOTFOUND;

	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	return D3DX11CreateEffectFromMemory(&compiledShader[0], size, dwShaderFlags, device, &(m_Effect));
}

BaseEffect::BaseEffect(ID3D11Device* device, const std::wstring& filename)
 : Effect(device, filename)
 , m_TechLight(nullptr)
 , m_TechLightTexture(nullptr)
 , m_World(nullptr)
 , m_WorldInvTranspose(nullptr)
 , m_WorldViewProj(nullptr)
 , m_TexTransform(nullptr)
 , m_EyePos(nullptr)
 , m_DirLight(nullptr)
 , m_DiffuseColor(nullptr)
 , m_SpecularColor(nullptr)
 , m_AmbientColor(nullptr)
 , m_FogColor(nullptr)
 , m_FogStart(nullptr)
 , m_FogRange(nullptr)
 , m_Time(nullptr)
 , m_DiffuseTexture(nullptr)
{
	m_TechLight = m_Effect->GetTechniqueByName("Light");
	m_TechLightTexture = m_Effect->GetTechniqueByName("LightTexture");
	m_World = m_Effect->GetVariableByName("gWorld")->AsMatrix();
	m_ViewProj = m_Effect->GetVariableByName("gViewProj")->AsMatrix();
	m_WorldInvTranspose = m_Effect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	m_WorldViewProj = m_Effect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_TexTransform = m_Effect->GetVariableByName("gTexTransform")->AsMatrix();
	m_DirLight = m_Effect->GetVariableByName("gDirLight");
	m_EyePos = m_Effect->GetVariableByName("gEyePosW")->AsVector();

	m_AmbientColor = m_Effect->GetVariableByName("gAmbient")->AsVector();
	m_DiffuseColor = m_Effect->GetVariableByName("gDiffuse")->AsVector();
	m_SpecularColor = m_Effect->GetVariableByName("gSpecular")->AsVector();

	m_FogColor = m_Effect->GetVariableByName("gFogColor")->AsVector();
	m_FogStart = m_Effect->GetVariableByName("gFogStart")->AsScalar();
	m_FogRange = m_Effect->GetVariableByName("gFogRange")->AsScalar();

	m_Time	= m_Effect->GetVariableByName("gTime")->AsScalar();

	m_DiffuseTexture = m_Effect->GetVariableByName("gDiffuseMap")->AsShaderResource();

}

BaseEffect::~BaseEffect()
{
	if (m_World)
	{
		m_World->Release();
		m_World = nullptr;
	}

	if (m_WorldInvTranspose)
	{
		m_WorldInvTranspose->Release();
		m_WorldInvTranspose = nullptr;
	}

	if (m_WorldViewProj)
	{
		m_WorldViewProj->Release();
		m_WorldViewProj = nullptr;
	}

	if (m_DirLight)
	{
		m_DirLight->Release();
		m_DirLight = nullptr;
	}

	
	if (m_EyePos)
	{
		m_EyePos->Release();
		m_EyePos = nullptr;
	}

	if (m_DiffuseTexture)
	{
		m_DiffuseTexture->Release();
		m_DiffuseTexture = nullptr;
	}

	if (m_DiffuseColor)
	{
		m_DiffuseColor->Release();
		m_DiffuseColor = nullptr;
	}
	if (m_SpecularColor)
	{
		m_SpecularColor->Release();
		m_SpecularColor = nullptr;
	}

	if (m_AmbientColor)
	{
		m_AmbientColor->Release();
		m_AmbientColor = nullptr;
	}

	if (m_FogColor)
	{
		m_FogColor->Release();
		m_FogColor = nullptr;
	}
	
	if (m_FogRange)
	{
		m_FogRange->Release();
		m_FogRange = nullptr;
	}
	
	if (m_FogStart)
	{
		m_FogStart->Release();
		m_FogStart = nullptr;
	}

	if (m_Time)
	{
		m_Time->Release();
		m_Time = nullptr;
	}

	if (m_TechLight)
	{
		m_TechLight->Release();
		m_TechLight = nullptr;
	}

	if (m_TechLightTexture)
	{
		m_TechLightTexture->Release();
		m_TechLightTexture = nullptr;
	}
}

void BaseEffect::Apply(std::shared_ptr<Material> material)
{
	SetAmbient(material->m_Ambient);
	SetDiffuse(material->m_Diffuse);
	SetSpecular(material->m_Specular);

	if (material->m_DiffuseMap)
	{
		SetTexTranform(material->m_DiffuseMap->GetTransform());
		SetDiffuseMap(material->m_DiffuseMap->GetResource());
	}
}

ID3DX11EffectTechnique* BaseEffect::GetTechnique(const std::string& name)
{
	return m_TechLightTexture;
}

ID3DX11EffectTechnique* BaseEffect::GetTechnique(int index)
{
	if (index == 0)
		return m_TechLight;
	else 
		return m_TechLightTexture;
}