#pragma once
#include <memory>

#include "Effects/EffectsManager.h"
#include "MaterialManager.h"

class ContentManager
{
public:
	ContentManager();
	~ContentManager();

	static ContentManager&				GetInstance();

	void	Initialize(ID3D11Device* device);
	void	Release();
public:

	std::shared_ptr<EffectsManager>		m_EffectManager;
	std::shared_ptr<MaterialManager>	m_MaterialManager;
	

};

