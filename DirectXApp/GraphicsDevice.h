#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>

class RenderStatesManager;
class GraphicsDevice
{
public:
	GraphicsDevice(ID3D11Device* device, ID3D11DeviceContext* context);
	~GraphicsDevice();

	void Init();

	void SetViewProj(DirectX::XMMATRIX viewproj) { m_ViewProj = viewproj; }

	
	DirectX::XMMATRIX m_ViewProj;
	


	ID3D11Device*					m_Device;
	ID3D11DeviceContext*			m_Context;
	

public:
	std::unique_ptr<RenderStatesManager>	RenderState;
	//= params.view*params.proj;



};

