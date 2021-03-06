#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

#include "GraphicsCommon.h"
struct ID3DX11EffectTechnique;
class EffectsManager;
class MeshData;

namespace Vertex
{
	struct Pos
	{
		DirectX::XMFLOAT3 Position;
	};

	struct PosSize
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 Size;
	};

	struct PosNormal
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
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
	static const D3D11_INPUT_ELEMENT_DESC Pos[1];
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



	ID3D11InputLayout* GetInputLayout(VertexType type) const;

	D3D_PRIMITIVE_TOPOLOGY GetTopology(TopologyType type) const;
private:
	ID3D11InputLayout* m_InputLayouts[(int)VertexType::COUNT];

};

