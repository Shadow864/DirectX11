#include "stdafx.h"
#include "Vertex.h"

#include "Effects/Effect.h"
#include "Effects/EffectsManager.h"
#include <d3dx11effect.h>
//#include "../../Effects11/inc/d3dx11effect.h"


#pragma region InputLayoutDesc

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Pos[1] =
{

	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNormal[2] =
{
	
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};


const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosSize[2] =
{

	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNormalTexCoord[3] =
{

	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};


#pragma endregion

#pragma region InputLayoutsManager

InputLayoutsManager::InputLayoutsManager()
{
	memset(m_InputLayouts, 0, sizeof(m_InputLayouts));
}

InputLayoutsManager::~InputLayoutsManager()
{
	Release();
}

void InputLayoutsManager::Init(ID3D11Device* device, EffectsManager* effects)
{
	D3DX11_PASS_DESC passDesc;
	//effects->m_BaseEffect->m_TechLight->GetPassByIndex(0)->GetDesc(&passDesc);
	//device->CreateInputLayout(InputLayoutDesc::PosNormal, 2, passDesc.pIAInputSignature,
	//	passDesc.IAInputSignatureSize, &PosNormal);

	effects->GetEffect(EffectType::BASIC)->m_TechLight->GetPassByIndex(0)->GetDesc(&passDesc);
	device->CreateInputLayout(InputLayoutDesc::PosNormalTexCoord, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_InputLayouts[(int)VertexType::POS_NORMAL_TEXCOORD]);

	effects->GetEffect(EffectType::SUBDIVISION)->m_TechLight->GetPassByIndex(0)->GetDesc(&passDesc);
	device->CreateInputLayout(InputLayoutDesc::PosNormalTexCoord, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_InputLayouts[(int)VertexType::POS_NORMAL_TEXCOORD]);


	effects->GetEffect(EffectType::BILLBOARD)->m_TechLight->GetPassByIndex(0)->GetDesc(&passDesc);
	device->CreateInputLayout(InputLayoutDesc::PosSize, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_InputLayouts[(int)VertexType::POS_SIZE]);

	effects->GetEffect(EffectType::CYLINDER)->m_TechLight->GetPassByIndex(0)->GetDesc(&passDesc);
	device->CreateInputLayout(InputLayoutDesc::PosNormal, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_InputLayouts[(int)VertexType::POS_NORMAL]);

}

void InputLayoutsManager::Release()
{

	for (auto layout : m_InputLayouts)
	{
		layout->Release();
		layout = nullptr;
	}
}

ID3D11InputLayout* InputLayoutsManager::GetInputLayout(VertexType type) const
{
	return m_InputLayouts[(int)type];
}

D3D_PRIMITIVE_TOPOLOGY InputLayoutsManager::GetTopology(TopologyType type) const
{
	switch (type)
	{
	case TopologyType::LINE_STRIP:
		return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case TopologyType::POINT_LIST:
		return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	case TopologyType::TRIANGLE_LIST:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
}
#pragma endregion
