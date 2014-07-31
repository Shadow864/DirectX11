#include "stdafx.h"
#include "Effect.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <vector>
#include <fstream>


Effect::Effect(ID3D11Device* device, const std::wstring& filename)
 : m_Effect(nullptr)
{
	CreateEffectFromMemory(device, filename);
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
 , m_Tech(nullptr)
 , m_World(nullptr)
 , m_WorldInvTranspose(nullptr)
 , m_WorldViewProj(nullptr)
 , m_EyePos(nullptr)
 , m_DirLight(nullptr)
 , m_Material(nullptr)

{
	m_Tech = m_Effect->GetTechniqueByName("LightingTech");
	m_World = m_Effect->GetVariableByName("gWorld")->AsMatrix();
	m_WorldInvTranspose = m_Effect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	m_WorldViewProj = m_Effect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_DirLight = m_Effect->GetVariableByName("gDirLight");
	m_Material = m_Effect->GetVariableByName("gMaterial");
	m_EyePos = m_Effect->GetVariableByName("gEyePosW")->AsVector();
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

	if (m_Material)
	{
		m_Material->Release();
		m_Material = nullptr;
	}

	if (m_EyePos)
	{
		m_EyePos->Release();
		m_EyePos = nullptr;
	}

	if (m_Tech)
	{
		m_Tech->Release();
		m_Tech = nullptr;
	}
}