#pragma once
#include <string>
#include <memory>
#include <vector>

#include <DirectXMath.h>

struct ID3D11ShaderResourceView;
struct ID3D11Device;

class ITexture
{
public:
	ITexture();
	virtual ~ITexture();

	DirectX::XMMATRIX			GetTransform() { return m_Transform; }

	virtual ID3D11ShaderResourceView* GetResource() = 0;
	virtual bool LoadContent(ID3D11Device* device) = 0;
	virtual bool UnloadContent(ID3D11Device* device) = 0;
	virtual void Update(float delta) = 0;

	void	SetPosition(float x, float y);
	void	SetRotation(float roll, float pitch, float yaw);
	void	SetScale(float x, float y);

protected:
	void	RecalculateWorldMatrix();

protected:
	DirectX::XMMATRIX			m_Transform;

	DirectX::XMVECTOR			m_Translation;
	DirectX::XMVECTOR			m_Scale;
	DirectX::XMVECTOR			m_Rotation;
};

class Texture : public ITexture
{
	std::wstring m_Name;
	ID3D11ShaderResourceView* m_Resource;

public:
	Texture(const std::wstring& name);
	~Texture();

	virtual ID3D11ShaderResourceView* GetResource() { return m_Resource; }
	virtual bool LoadContent(ID3D11Device* device) override;
	virtual bool UnloadContent(ID3D11Device* device) override;
	virtual void Update(float delta) override;
};


class AnimateTexture : public ITexture
{
	std::wstring m_Name;
	std::vector<ID3D11ShaderResourceView*> m_Resources;

	int			m_CurrentFrame;
	int			m_MaxFrame;
	float		m_CurrentTime;
	float		m_AnimTime;
	float		m_Ratio;

public:
	AnimateTexture(const std::wstring& name, int frames);
	~AnimateTexture();

	virtual ID3D11ShaderResourceView* GetResource() { return m_Resources[m_CurrentFrame]; }
	virtual bool LoadContent(ID3D11Device* device) override;
	virtual bool UnloadContent(ID3D11Device* device) override;
	virtual void Update(float delta) override;
};

