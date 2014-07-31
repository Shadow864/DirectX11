#include "stdafx.h"
#include "EffectsManager.h"
#include <d3d11.h>

#include "Effect.h"

EffectsManager::EffectsManager()
{

}
EffectsManager::~EffectsManager()
{
	Release();
}

bool EffectsManager::Init(ID3D11Device* device)
{
	m_Effects[(int)EffectType::BASIC]		= std::make_shared<BaseEffect>(device, L"FX/Basic.fxo");
	m_Effects[(int)EffectType::BILLBOARD]	= std::make_shared<BaseEffect>(device, L"FX/Billboard.fxo");
	return true;
}
void EffectsManager::Release()
{

}