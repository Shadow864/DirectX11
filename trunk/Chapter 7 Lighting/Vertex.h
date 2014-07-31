#pragma once
#include <DirectXMath.h>
#include <d3d11.h>


struct ID3DX11EffectTechnique;
class EffectsManager;

namespace Vertex
{
	struct PosNormal
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
	};
};

class InputLayoutDesc
{
public:
	static const D3D11_INPUT_ELEMENT_DESC PosNormal[2];
};

class InputLayoutsManager
{
public:

	~InputLayoutsManager();
	void Init(ID3D11Device* device, EffectsManager* technique);
	void Release();

	ID3D11InputLayout* PosNormal;
};

