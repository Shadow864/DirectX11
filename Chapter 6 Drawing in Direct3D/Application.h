#pragma once

#include "../DirectXApp/DirectXApp.h"

#include <DirectXMath.h>

class ModelObject;

class Application :
	public DirectXApp
{
public:
	Application(HINSTANCE instance);
	~Application(void);

	virtual bool Init();

	virtual void OnResize();
	virtual void Update();
	virtual void Draw();

protected:

	virtual void OnMouseDown	(WPARAM btnState, int x, int y) override;
	virtual void OnMouseUp		(WPARAM btnState, int x, int y)	override;
	virtual void OnMouseMove	(WPARAM btnState, int x, int y) override;
	virtual void OnMouseWheel	(WPARAM btnState, int x, int y) override;

	virtual void OnKeyUp		(WPARAM btnState) override; 
	virtual void OnKeyDown		(WPARAM btnState) override;

private:
	void DestroyObjects();
	void CreateObject(UINT id);
	void BuildGeometryBuffers();
	bool BuildFX();
	void BuildVertexLayout();


private:
	ID3DX11Effect*					m_Effect;
	ID3DX11EffectTechnique*			m_Tech;
	ID3DX11EffectMatrixVariable*	m_WorldViewProj;
	ID3DX11EffectVectorVariable*	m_Color;
	ID3D11RasterizerState*			m_WireframeRS;

	ID3D11Buffer*					m_VerticesBuffer;
	ID3D11Buffer*					m_IndicesBuffer;
	ID3D11InputLayout*				m_InputLayout;

	DirectX::XMFLOAT4X4				m_View;
	DirectX::XMFLOAT4X4				m_Proj;

	std::vector<ModelObject*>		m_ModelObjects;

	POINT							m_MousePos;
	POINT							m_LastPos;

	bool							m_MouseDown;

	float							m_Theta;
	float							m_Phi;
	float							m_Radius;

	int								m_level;
	int								m_Triangles;
};

