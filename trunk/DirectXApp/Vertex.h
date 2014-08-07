#pragma once
#include <DirectXMath.h>
#include <d3d11.h>


struct ID3DX11EffectTechnique;
class EffectsManager;
class MeshData;

namespace Vertex
{

	struct PosSize
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 Size;
	};

	struct PosNormalTexCoord
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 TexCoord;
	};
};

class InputLayoutDesc
{
public:
	static const D3D11_INPUT_ELEMENT_DESC PosNormal[2];
	static const D3D11_INPUT_ELEMENT_DESC PosSize[2];
	static const D3D11_INPUT_ELEMENT_DESC PosNormalTexCoord[3];
};

class InputLayoutsManager
{
public:
	InputLayoutsManager();
	~InputLayoutsManager();

	void Init(ID3D11Device* device, EffectsManager* effects);
	void Release();

	ID3D11InputLayout* PosNormal;
	ID3D11InputLayout* PosSize;
	ID3D11InputLayout* PosNormalTexCoord;
};

