#pragma once
#include <string>
#include "../../DirectXApp/LightHelper.h"

#include "../../Effects11/inc/d3dx11effect.h"

struct ID3D11Device;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;


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


	void SetWorldViewProj(DirectX::CXMMATRIX M)                  { m_WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(DirectX::CXMMATRIX M)                          { m_World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(DirectX::CXMMATRIX M)              { m_WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const DirectX::XMFLOAT3& v)                  { m_EyePos->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3)); }
	void SetDirLights(const Lights::DirectionalLight* lights)	 { m_DirLight->SetRawValue(lights, 0, sizeof(Lights::DirectionalLight)); }
	void SetMaterial(const Material& mat)						 { m_Material->SetRawValue(&mat, 0, sizeof(Material)); }

public:
	ID3DX11EffectTechnique*			m_Tech;


protected:
	ID3DX11EffectMatrixVariable*	m_World;
	ID3DX11EffectMatrixVariable*	m_WorldInvTranspose;
	ID3DX11EffectMatrixVariable*	m_WorldViewProj;

	ID3DX11EffectVectorVariable*	m_EyePos;

	ID3DX11EffectVariable*			m_DirLight;
	ID3DX11EffectVariable*			m_Material;
};

