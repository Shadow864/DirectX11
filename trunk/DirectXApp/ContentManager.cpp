#include "stdafx.h"
#include "ContentManager.h"


ContentManager::ContentManager()
: m_EffectManager(nullptr)
, m_MaterialManager(nullptr)
{
}


ContentManager::~ContentManager()
{
}

ContentManager&	 ContentManager::GetInstance()
{ 
	static ContentManager INSTANCE;

	return INSTANCE;
}


void ContentManager::Initialize(ID3D11Device* device)
{
	m_EffectManager = std::make_shared<EffectsManager>();
	m_EffectManager->Init(device);

	m_MaterialManager = std::make_shared<MaterialManager>();
}
void ContentManager::Release()
{

}