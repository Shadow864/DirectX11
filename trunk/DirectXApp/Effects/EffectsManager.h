#pragma once
#include <memory>

struct ID3D11Device;

class BaseEffect;
class EffectsManager
{
public:
	EffectsManager();
	~EffectsManager();

	bool Init(ID3D11Device* device);
	void Release();


	std::shared_ptr<BaseEffect>		GetEffect(EffectType type) { return m_Effects[(int)type]; }

private:

	std::shared_ptr<BaseEffect>		m_Effects[(int)EffectType::COUNT];
};

