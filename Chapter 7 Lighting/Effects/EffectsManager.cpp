#include "stdafx.h"
#include "EffectsManager.h"
#include <d3d11.h>

#include "Effect.h"

EffectsManager::EffectsManager()
: m_BaseEffect(nullptr)
{

}
EffectsManager::~EffectsManager()
{
	Release();
}

bool EffectsManager::Init(ID3D11Device* device)
{
	m_BaseEffect = new BaseEffect(device, L"FX/Lighting.fxo");

	return true;
}
void EffectsManager::Release()
{
	if (m_BaseEffect)
		delete m_BaseEffect;
}