#pragma once

#include <DirectXMath.h>
#include <memory>

#include "GraphicsCommon.h"
#include "RenderState.h"
class BaseEffect;
class ITexture;

struct ID3D11Device;
struct ID3DX11EffectTechnique;
class IMaterial
{
	virtual bool LoadContent(ID3D11Device* device) = 0;
	virtual bool UnloadContent(ID3D11Device* device) = 0;
	virtual void Update(float delta) = 0;
	virtual ID3DX11EffectTechnique* GetTechnique() = 0;

};

class Material : public IMaterial
{

public:
	Material();
	~Material();

	virtual bool LoadContent(ID3D11Device* device) override;
	virtual bool UnloadContent(ID3D11Device* device) override;
	virtual void Update(float delta)  override;

	ID3DX11EffectTechnique* GetTechnique();

	DirectX::XMFLOAT4 m_Ambient;
	DirectX::XMFLOAT4 m_Diffuse;
	DirectX::XMFLOAT4 m_Specular;

	RenderState					m_RenderState;
	EffectType					m_EffectType;

	std::shared_ptr<ITexture>   m_DiffuseMap;
	std::shared_ptr<BaseEffect> m_Effect;

};

