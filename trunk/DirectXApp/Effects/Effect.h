#pragma once
#include <string>
#include <memory>

#include "../LightHelper.h"
#include "../../Effects11/inc/d3dx11effect.h"

struct ID3D11Device;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;

class Material;

class Effect
{
public:
	Effect(ID3D11Device* device, const std::wstring& filename);

	virtual ~Effect() = 0;


	virtual void SetViewProj(DirectX::CXMMATRIX /*M*/) {}
	virtual void SetWorldViewProj(DirectX::CXMMATRIX /*M*/) {}
	virtual void SetWorld(DirectX::CXMMATRIX /*M*/) {}
	virtual void SetWorldInvTranspose(DirectX::CXMMATRIX /*M*/) {}
	virtual void SetTexTranform(DirectX::CXMMATRIX /*M*/) {}
	virtual void SetEyePosW(const DirectX::XMFLOAT3& /*v*/) {}
	virtual void SetDirLights(const Lights::DirectionalLight* /*lights*/) {}


	virtual void SetDiffuse(const DirectX::XMFLOAT4& /*v*/)	{}
	virtual void SetSpecular(const DirectX::XMFLOAT4& /*v*/) {}
	virtual void SetAmbient(const DirectX::XMFLOAT4& /*v*/)	{}

	virtual void SetDiffuseMap(ID3D11ShaderResourceView* tex) {}

	virtual void SetFogColor(const DirectX::FXMVECTOR /*v*/) {}
	virtual void SetFogStart(float /*start*/) {}
	virtual void SetFogRange(float /*range*/) {}

	virtual void Apply(std::shared_ptr<Material> material) = 0;

	virtual ID3DX11EffectTechnique* GetTechnique(const std::string& name) = 0;
	virtual ID3DX11EffectTechnique* GetTechnique(int index) = 0;

private:
	HRESULT CreateEffectFromMemory(ID3D11Device* device, const std::wstring& filename);

protected:
	ID3DX11Effect*					m_Effect;
};


class BaseEffect : public Effect
{
public:
	BaseEffect(ID3D11Device* device, const std::wstring& filename);
	virtual ~BaseEffect();

	void SetViewProj(DirectX::CXMMATRIX M)						override { m_ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(DirectX::CXMMATRIX M)					override { m_WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(DirectX::CXMMATRIX M)							override { m_World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(DirectX::CXMMATRIX M)				override { m_WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTranform(DirectX::CXMMATRIX M)					override { m_TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const DirectX::XMFLOAT3& v)					override { m_EyePos->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3)); }
	void SetDirLights(const Lights::DirectionalLight* lights)	override { m_DirLight->SetRawValue(lights, 0, sizeof(Lights::DirectionalLight)); }
	
	
	void SetDiffuse(const DirectX::XMFLOAT4& v)					override { m_DiffuseColor->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT4)); }
	void SetSpecular(const DirectX::XMFLOAT4& v)				override { m_SpecularColor->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT4)); }
	void SetAmbient(const DirectX::XMFLOAT4& v)					override { m_AmbientColor->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT4)); }
	
	void SetDiffuseMap(ID3D11ShaderResourceView* tex)			override { m_DiffuseTexture->SetResource(tex); }

	void SetFogColor(const DirectX::FXMVECTOR v)				override { m_FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float start)								override { m_FogStart->SetFloat(start); }
	void SetFogRange(float range)								override { m_FogRange->SetFloat(range); }


	void Apply(std::shared_ptr<Material> material)				override;

	virtual ID3DX11EffectTechnique* GetTechnique(const std::string& name) override;
	virtual ID3DX11EffectTechnique* GetTechnique(int index)  override;

public:
	ID3DX11EffectTechnique*					m_TechLight;
	ID3DX11EffectTechnique*					m_TechLightTexture;

protected:
	ID3DX11EffectMatrixVariable*			m_World;
	ID3DX11EffectMatrixVariable*			m_ViewProj;
	ID3DX11EffectMatrixVariable*			m_WorldInvTranspose;
	ID3DX11EffectMatrixVariable*			m_WorldViewProj;
	ID3DX11EffectMatrixVariable*			m_TexTransform;


	ID3DX11EffectVectorVariable*			m_EyePos;

	ID3DX11EffectVectorVariable*			m_DiffuseColor;
	ID3DX11EffectVectorVariable*			m_SpecularColor;
	ID3DX11EffectVectorVariable*			m_AmbientColor;

	ID3DX11EffectVectorVariable*			m_FogColor;
	ID3DX11EffectScalarVariable*			m_FogStart;
	ID3DX11EffectScalarVariable*			m_FogRange;

	ID3DX11EffectVariable*					m_DirLight;
	ID3DX11EffectShaderResourceVariable*	m_DiffuseTexture;
};

