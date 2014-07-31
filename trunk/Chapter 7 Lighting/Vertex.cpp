#include "stdafx.h"
#include "Vertex.h"

#include "Effects/Effect.h"
#include "Effects/EffectsManager.h"
#include <d3dx11effect.h>
//#include "../../Effects11/inc/d3dx11effect.h"


#pragma region InputLayoutDesc

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNormal[2] =
{
	
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

#pragma endregion

#pragma region InputLayoutsManager

InputLayoutsManager::~InputLayoutsManager()
{
	Release();
}

void InputLayoutsManager::Init(ID3D11Device* device, EffectsManager* effects)
{
	D3DX11_PASS_DESC passDesc;
	effects->m_BaseEffect->m_Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	device->CreateInputLayout(InputLayoutDesc::PosNormal, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &PosNormal);
}

void InputLayoutsManager::Release()
{
	if (PosNormal)
		PosNormal->Release();

	PosNormal = nullptr;

}

#pragma endregion
