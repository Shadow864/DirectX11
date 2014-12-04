#pragma once

#include <DirectXMath.h>
#include <memory>

#include "GraphicsCommon.h"
#include "RenderState.h"
class Effect;
class ITexture;

struct ID3D11Device;
struct ID3DX11EffectTechnique;
struct ID3D11Buffer;

class MeshData;

class Material 
{

public:
	Material();
	~Material();

	virtual void Initialize(ID3D11Device* device);
	virtual bool LoadContent(ID3D11Device* device);
	virtual bool UnloadContent(ID3D11Device* device);
	virtual void Update(float delta);


	virtual void CreateVertexBuffer(ID3D11Device* device, const MeshData* mesh, ID3D11Buffer** vertexBuffer) const = 0;
	virtual UINT VertexSize() const = 0;

	ID3DX11EffectTechnique* GetTechnique();

	DirectX::XMFLOAT4 m_Ambient;
	DirectX::XMFLOAT4 m_Diffuse;
	DirectX::XMFLOAT4 m_Specular;

	RenderState					m_RenderState;
	EffectType					m_EffectType;

	TopologyType				m_TopologyType;
	VertexType					m_VertexType;

	std::shared_ptr<ITexture>   m_DiffuseMap;
	std::shared_ptr<Effect>		m_Effect;

};

