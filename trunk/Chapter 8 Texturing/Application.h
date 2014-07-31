#pragma once

#include "../DirectXApp/DirectXApp.h"

#include <LightHelper.h>

#include <DirectXMath.h>

class ModelObject;
class MirrorObject;
class EffectsManager;
class InputLayoutsManager;


class Material;

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
	void CreateLights();
	void BuildGeometryBuffers();
	void BuildVertexLayout();


	void Render(UINT stride, UINT offset, ID3D11Buffer* vertex_buffer, ID3D11Buffer* indices_buffer, std::shared_ptr<Material> material, UINT index_count, UINT start_index_location, INT base_vertex_location, DirectX::CXMMATRIX matrix);
	void Render(const ModelObject* object, DirectX::CXMMATRIX, std::shared_ptr<Material> material = nullptr);
	void RenderModelObject(const ModelObject* object, DirectX::CXMMATRIX, std::shared_ptr<Material>);
	void RenderMirror(const MirrorObject* mirror, DirectX::CXMMATRIX, std::shared_ptr<Material>);
	void RenderShadow(const ModelObject* object,DirectX::CXMMATRIX matrix, std::shared_ptr<Material> material = nullptr);
private:
	DirectX::XMFLOAT3				m_EyePos;
	DirectX::XMFLOAT4X4				m_View;
	DirectX::XMFLOAT4X4				m_Proj;

	//Rasterizer::TYPE				m_RSState;

	InputLayoutsManager*			m_InputLayoutsManager;

	std::vector<ModelObject*>		m_ModelObjects;

	POINT							m_MousePos;
	POINT							m_LastPos;

	Lights::DirectionalLight		m_Light;

	bool							m_MouseDown;

	float							m_Theta;
	float							m_Phi;
	float							m_Radius;

	int								m_level;
	int								m_Triangles;
};

