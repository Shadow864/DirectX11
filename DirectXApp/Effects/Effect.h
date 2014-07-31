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

	void SetViewProj(DirectX::CXMMATRIX M)                       { m_ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(DirectX::CXMMATRIX M)                  { m_WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(DirectX::CXMMATRIX M)                          { m_World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(DirectX::CXMMATRIX M)              { m_WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTranform(DirectX::CXMMATRIX M)					 { m_TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const DirectX::XMFLOAT3& v)                  { m_EyePos->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3)); }
	void SetDirLights(const Lights::DirectionalLight* lights)	 { m_DirLight->SetRawValue(lights, 0, sizeof(Lights::DirectionalLight)); }
	
	
	void SetDiffuse(const DirectX::XMFLOAT4& v)					 { m_DiffuseColor->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT4)); }
	void SetSpecular(const DirectX::XMFLOAT4& v)				 { m_SpecularColor->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT4)); }
	void SetAmbient(const DirectX::XMFLOAT4& v)					 { m_AmbientColor->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT4)); }
	
	void SetDiffuseMap(ID3D11ShaderResourceView* tex)			 { m_DiffuseTexture->SetResource(tex); }

	void SetFogColor(const DirectX::FXMVECTOR v)				 { m_FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float start)								 { m_FogStart->SetFloat(start); }
	void SetFogRange(float range)								 { m_FogRange->SetFloat(range); }


	void Apply(std::shared_ptr<Material> material);

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

