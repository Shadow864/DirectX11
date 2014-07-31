#pragma once

class BaseEffect;
struct ID3D11Device;

class EffectsManager
{
public:
	EffectsManager();
	~EffectsManager();

	bool Init(ID3D11Device* device);
	void Release();

public:
	BaseEffect*		m_BaseEffect;
};

