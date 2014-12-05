#include "stdafx.h"
#include "Application.h"

#include <d3dx11effect.h>
#include <d3dcompiler.h>

#include <math.h>

#include <sstream>
#include <algorithm>

#include <Terrain.h>
#include <ModelObject.h>
#include <MeshData.h>
#include <GeometryGenerator.h>
#include "..\DirectXApp\MathHelper.h"

#include "ContentManager.h"

#include "Effects/Effect.h"
#include "Effects/EffectsManager.h"

#include "../DirectXApp/Material.h"
#include "Vertex.h"
#include "..\DirectXApp\Texture.h"

#include <GraphicsDevice.h>

Application::Application(HINSTANCE instance)
	: DirectXApp(instance)
	, m_InputLayoutsManager(nullptr)
	, m_MouseDown(false)
	, m_Theta(0)
	, m_Phi(0)
	, m_Radius(10)
	, m_level(0)
	, m_Triangles(0)
{
}

Application::~Application(void)
{

	DestroyObjects();

	if (m_InputLayoutsManager)
		delete m_InputLayoutsManager;
	m_InputLayoutsManager = nullptr;

}

void Application::DestroyObjects()
{
	while (!m_ModelObjects.empty())
	{
		if (m_ModelObjects[m_ModelObjects.size() - 1])
			delete m_ModelObjects[m_ModelObjects.size() - 1];
		m_ModelObjects.pop_back();
	}
}

bool Application::Init()
{
	if(!DirectXApp::Init())
		return false;

	BuildVertexLayout();

	CreateLights();

	return true;
}

void Application::OnResize()
{
	DirectXApp::OnResize();

	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH( 0.25f * DirectX::XM_PI,  GetAspectRatio(), 1.0f, 10000.0f);

	XMStoreFloat4x4(&m_Proj, P);
}

void Application::Update()
{
	__super::Update();

	for (auto obj : m_ModelObjects)
		obj->Update(GetDeltaTime());

	if (m_MouseDown)
	{
		m_Theta		+= DirectX::XMConvertToRadians((float)(m_LastPos.y - m_MousePos.y));// * GetDeltaTime();
		m_Phi	    += DirectX::XMConvertToRadians((float)(m_LastPos.x - m_MousePos.x));// * GetDeltaTime();
	}

	if (m_Phi > 2.0f * DirectX::XM_PI)
		m_Phi -= 2.0f * DirectX::XM_PI;

	if (m_Phi < -2.0f * DirectX::XM_PI)
		m_Phi += 2.0f * DirectX::XM_PI;


	if (m_Theta >= DirectX::XM_PI/2.f - 0.001f)
		m_Theta = DirectX::XM_PI/2.f - 0.001f;

	if (m_Theta <= -DirectX::XM_PI/2.f + 0.001f)
		m_Theta = -DirectX::XM_PI/2.f + 0.001f;

	m_LastPos.x = m_MousePos.x;
	m_LastPos.y = m_MousePos.y;

	std::wostringstream outs;   
	outs.precision(6);
	outs << L"m_Theta: " << m_Theta << " ";
	outs << L"m_Phi: " << m_Phi << " ";
	outs << L"Frame Time: " << GetDeltaTime() * 1000.0f << L" (ms)";
	outs << L"Triangles: "	<< m_Triangles; 

	m_WindowTitle = outs.str();

	// Convert Spherical to Cartesian coordinates.
	float x = m_Radius * cosf(m_Theta) * sinf(m_Phi);
	float z = -m_Radius * cosf(m_Theta) * cosf(m_Phi);
	float y = m_Radius * sinf(m_Theta);

	// Build the view matrix.
	DirectX::XMVECTOR pos    = DirectX::XMVectorSet(x, y, z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up     = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(pos, target, up);
	DirectX::XMStoreFloat4x4(&m_View, V);

	m_EyePos = DirectX::XMFLOAT3(x, y, z);

}

void Application::Draw()
{
	__super::Draw();

	m_Triangles = 0;

	const float color[4] = { 0, 0 ,0, 0};

	m_DeviceContextPtr->ClearRenderTargetView(m_RenderTargetView, color);
	m_DeviceContextPtr->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&m_View);
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&m_Proj);

	m_GraphicsDevice->SetViewProj(view * proj);

	for (auto object : m_ModelObjects)
	{
		Render(object, DirectX::XMMatrixIdentity());
	}

	for (auto object : m_ModelObjects)
	{
		if (object->CanCastShadow())
			RenderShadow(object, DirectX::XMMatrixIdentity());
	}

	m_SwapChainPtr->Present(0, 0);
}

void Application::Render(const ModelObject* object, DirectX::CXMMATRIX matrix, std::shared_ptr<Material> material)
{
	if (dynamic_cast<const MirrorObject*>(object))
	{
		RenderMirror(static_cast<const MirrorObject*>(object), matrix, material);
	}
	else
	{
		RenderModelObject(object, matrix, material);
	}
}



void Application::Render(
	UINT stride,
	UINT offset, 
	ID3D11Buffer* vertex_buffer, 
	ID3D11Buffer* indices_buffer, 
	std::shared_ptr<Material> material, 
	UINT index_count, 
	UINT start_index_location, 
	INT base_vertex_location,
	DirectX::CXMMATRIX world
	)
{
	const float color[4] = { 0, 0, 0, 0 };

	std::shared_ptr<Effect> effect = material->m_Effect;

	if (!effect)
		return;
	
	m_DeviceContextPtr->IASetInputLayout(m_InputLayoutsManager->GetInputLayout(material->m_VertexType));
	m_DeviceContextPtr->IASetPrimitiveTopology(m_InputLayoutsManager->GetTopology(material->m_TopologyType));

	DirectX::FXMVECTOR fog_color = DirectX::XMVectorSet(0.4f, 0.4f, 0.4f, 1.0f);

	effect->SetFogColor(fog_color);
	effect->SetFogStart(40.f);
	effect->SetFogRange(60.f);


	effect->SetDirLights(&m_Light);
	effect->SetEyePosW(m_EyePos);


	m_GraphicsDevice->m_Context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
	m_GraphicsDevice->m_Context->IASetIndexBuffer(indices_buffer, DXGI_FORMAT_R32_UINT, 0);

	// Set constants
	ID3DX11EffectTechnique* technique = material->GetTechnique();


	m_GraphicsDevice->RenderState->Apply(material->m_RenderState, Scope::MATERIAL);
	D3DX11_TECHNIQUE_DESC techDesc;
	technique->GetDesc(&techDesc);


	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		DirectX::XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);


		effect->SetWorld(world);
		effect->SetViewProj(m_GraphicsDevice->m_ViewProj);
		effect->SetWorldInvTranspose(worldInvTranspose);
		effect->SetWorldViewProj(world* m_GraphicsDevice->m_ViewProj);


		effect->Apply(material);

		technique->GetPassByIndex(p)->Apply(0, m_GraphicsDevice->m_Context);

		m_GraphicsDevice->m_Context->DrawIndexed(index_count, start_index_location, base_vertex_location);
	}

	m_GraphicsDevice->RenderState->Reset(Scope::MATERIAL);
}
void Application::RenderModelObject(const ModelObject* object, DirectX::CXMMATRIX matrix, std::shared_ptr<Material> material)
{
	UINT stride = object->GetMaterial()->VertexSize();
	UINT offset = 0;
	ID3D11Buffer* vertex_buffer =	object->GetVerticesBuffer();
	ID3D11Buffer* index_buffer = object->GetIndicesBuffer();
	UINT index_count = object->GetMeshData()->GetIndices().size();
	UINT start_index_location = 0;// object->m_IndicesOffset;
	INT base_vertex_location = 0;// object->m_VerticesOffset;
	DirectX::CXMMATRIX world = object->GetWorldMatrix() * matrix;
	

	if (!material)
		material = object->GetMaterial();


	Render(stride, offset, vertex_buffer, index_buffer, material, index_count, start_index_location, base_vertex_location, world);
}

void Application::RenderMirror(const MirrorObject* mirror, DirectX::CXMMATRIX matrix, std::shared_ptr<Material> material)
{
	m_GraphicsDevice->RenderState->SetCulling(CullFaceMode::FRONT, Scope::OBJECT);
	m_GraphicsDevice->RenderState->SetDepthStencilMode(DepthStencilMode::MIRROR, 1, Scope::OBJECT);
	m_GraphicsDevice->RenderState->SetBlendType(BlendMode::NO_RENDER_TARGET, Scope::OBJECT);

	RenderModelObject(mirror, matrix, material);

	m_GraphicsDevice->RenderState->Reset(Scope::OBJECT);
	m_GraphicsDevice->RenderState->SetDepthStencilMode(DepthStencilMode::REFLECTION, 1, Scope::OBJECT);
	m_GraphicsDevice->RenderState->SetCulling(CullFaceMode::COUNTER, Scope::OBJECT);


	for (auto object : mirror->m_Objects)
	{
		DirectX::XMVECTOR mirrorPlane = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, -2.0f); // xy plane
		DirectX::XMMATRIX R = DirectX::XMMatrixReflect(mirrorPlane);
		Render(object, matrix * R, material);
	}

	for (auto object : mirror->m_Objects)
	{
		if (!object->CanCastShadow())
			continue;
		m_GraphicsDevice->RenderState->SetDepthStencilMode(DepthStencilMode::REFLECTION_SHADOW, 1, Scope::OBJECT);
		DirectX::XMVECTOR mirrorPlane = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, -2.0f); // xy plane
		DirectX::XMMATRIX R = DirectX::XMMatrixReflect(mirrorPlane);
		RenderShadow(object, R * matrix, material);
	}
	m_GraphicsDevice->RenderState->SetDepthStencilMode(DepthStencilMode::NONE, 0, Scope::OBJECT);
	m_GraphicsDevice->RenderState->Reset(Scope::OBJECT);
	
	RenderModelObject(mirror, matrix, material);
	
}

void Application::RenderShadow(const ModelObject* object, DirectX::CXMMATRIX matrix, std::shared_ptr<Material> material)
{

	DirectX::XMMATRIX shadow= DirectX::XMMatrixShadow(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0), DirectX::XMVectorSet(0, 1, 0, 0));// m_Light.Direction);

	Render(object, matrix *  shadow* DirectX::XMMatrixTranslation(0, -0.999, 0), ContentManager::GetInstance().m_MaterialManager->Shadow);
}

void Application::OnMouseDown(WPARAM btnState, int x, int y)
{
	m_MouseDown = true;
}

void Application::OnMouseUp(WPARAM btnState, int x, int y)
{
	m_MouseDown = false;	
}

void Application::OnMouseMove(WPARAM btnState, int x, int y)
{
	m_MousePos.x = x;
	m_MousePos.y = y;
}

void Application::OnMouseWheel(WPARAM btnState, int x, int y) 
{
	m_Radius -= (((GET_WHEEL_DELTA_WPARAM(btnState)))) * 0.01f;
}

void Application::OnKeyUp(WPARAM btnState)
{
	int level = m_level;

	switch(btnState)
	{
	case VK_UP:
		++level;
		break;
	case VK_DOWN:
		--level;
		break;
	}

	if (level < 0)
		level = 0;

	if (level > 6)
		level = 6;

	if (level != m_level)
	{

	}
}

void Application::OnKeyDown(WPARAM btnState)
{
	switch(btnState)
	{

	//case 'A':

	case VK_NUMPAD0:
	case VK_NUMPAD1:
	case VK_NUMPAD2:
	case VK_NUMPAD3:
	case VK_NUMPAD4:
	case VK_NUMPAD5:
	case VK_NUMPAD6:
	case VK_NUMPAD7:
	case VK_NUMPAD8:
	case VK_NUMPAD9:
		CreateObject(btnState - VK_NUMPAD0);
		break;
	case VK_F1:
		m_GraphicsDevice->RenderState->SetFillMode(FillMode::SOLID, Scope::GLOBAL);
		break;
	case VK_F2:
		m_GraphicsDevice->RenderState->SetFillMode(FillMode::WIREFRAME, Scope::GLOBAL);
		break;
	}
}

void Application::CreateObject(UINT id)
{
	DestroyObjects();

	if (id == 1)
	{
		ModelObject* ground = new Terrain("Textures//Highmaps//not_wo10.bmp");
		ground->SetPosition(0, 0 , 0);
		ground->SetMaterial(ContentManager::GetInstance().m_MaterialManager->Terrain);

		ModelObject* crate = new Box();
		crate->SetPosition(2, 12, 15);
		crate->SetMaterial(ContentManager::GetInstance().m_MaterialManager->WireFence);

		ModelObject* water = new Wave(256, 256, 10, 10);
		water->SetPosition(0, 12, 0);
		water->SetMaterial(ContentManager::GetInstance().m_MaterialManager->Water);
	
		m_ModelObjects.push_back(ground);
		m_ModelObjects.push_back(crate);
		m_ModelObjects.push_back(water);
		
	}

	else if (id == 2)
	{
		/*Circle* circle = new Circle();
		circle->SetMaterial(ContentManager::GetInstance().m_MaterialManager->Cylinder);
		m_ModelObjects.push_back(circle);*/

		Triangle* triangle = new Triangle();
		triangle->SetMaterial(ContentManager::GetInstance().m_MaterialManager->Subdivision);
		m_ModelObjects.push_back(triangle);

		/*ModelObject* traingle = new Triangle();
		traingle->SetMaterial(ContentManager::GetInstance().m_MaterialManager->WoodenBox);
		m_ModelObjects.push_back(traingle);*/
	}
	else if ( id == 3)
		m_ModelObjects.push_back(new SierpinskiTriangle(2));
	else if ( id == 4)
		m_ModelObjects.push_back(new SierpinskiPyramid(1));
	else if ( id == 5)
		m_ModelObjects.push_back(new Cylinder(1, 0.5, 1, 4, 2));
	else if (id == 6)
	{
		ModelObject* geosphere = new Geosphere(1,2);
		geosphere->SetMaterial(ContentManager::GetInstance().m_MaterialManager->Fire);
		m_ModelObjects.push_back(geosphere);
	}
	else if ( id == 7)
		m_ModelObjects.push_back(new Grid(1,1, 50,50));
	else if ( id == 8)
		m_ModelObjects.push_back(new Wave(1,1, 50,50));
	else if ( id == 9)
		m_ModelObjects.push_back(new TestObject("Models/skull.txt"));
	else if (id == 0)
	{

		ModelObject* Wall1 = new Billboard();// Cylinder(1, 1, 2, 6, 6);
		Wall1->SetMaterial(ContentManager::GetInstance().m_MaterialManager->Tree);
		Wall1->SetPosition(1, -0.5, 1);
		m_ModelObjects.push_back(Wall1);
		
		
		ModelObject* ground = new Grid(1, 1, 10, 10);
		ground->SetMaterial(ContentManager::GetInstance().m_MaterialManager->BrickFloor);
		ground->SetPosition(0, -1, -9);
		ground->SetScale(11, 1, 11);
		m_ModelObjects.push_back(ground);


		ModelObject* skull = new TestObject("Models/skull.txt");
		skull->SetCastShadow(true);
		skull->SetMaterial(ContentManager::GetInstance().m_MaterialManager->Skull);
		m_ModelObjects.push_back(skull);



		MirrorObject* mirror = new MirrorObject(1, 2, 4, 4);
		mirror->SetMaterial(ContentManager::GetInstance().m_MaterialManager->Mirror);
		mirror->SetPosition(0, 1, 1.999);
		mirror->SetRotation(-90, 0, 0);
		mirror->SetScale(1, 1, 1);
		mirror->AddObject(ground);
		mirror->AddObject(skull); 
		mirror->AddObject(Wall1);
		m_ModelObjects.push_back(mirror);

		Circle* circle = new Circle();
		circle->SetMaterial(ContentManager::GetInstance().m_MaterialManager->Cylinder);
		m_ModelObjects.push_back(circle);

		for (INT i = 0; i <= 10; ++i)
		{

			if (i == 5)
				continue;

			ModelObject* wall = new Grid(1, 2, 2, 2);
			wall->SetMaterial(ContentManager::GetInstance().m_MaterialManager->Wall);
			wall->SetPosition(-10 + 2 * i, 1, 2);
			wall->SetRotation(-90, 0, 0);
			wall->SetScale(1, 1, 1);

			m_ModelObjects.push_back(wall);
		}

	}

	for (auto model : m_ModelObjects)
	{
		model->Initialize(GetDevicePtr());
		model->LoadContent(GetDevicePtr());
	}

	BuildGeometryBuffers();
}


void Application::CreateLights()
{
	m_Light.Ambient = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_Light.Diffuse = DirectX::XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	m_Light.Specular = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light.Direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	ContentManager::GetInstance().m_MaterialManager->Shadow->Initialize(m_DevicePtr);
	ContentManager::GetInstance().m_MaterialManager->Shadow->LoadContent(m_DevicePtr);
}

void Application::BuildGeometryBuffers()
{	

}

void Application::BuildVertexLayout()
{
	m_InputLayoutsManager = new InputLayoutsManager();
	m_InputLayoutsManager->Init(GetDevicePtr(), ContentManager::GetInstance().m_EffectManager.get());
}