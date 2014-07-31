#include "stdafx.h"
#include "Application.h"

#include <d3dx11effect.h>
#include <d3dcompiler.h>

#include <math.h>

#include <sstream>

#include <ModelObject.h>
#include <MeshData.h>
#include <GeometryGenerator.h>

Application::Application(HINSTANCE instance)
	: DirectXApp(instance)
	, m_Effect(nullptr)
	, m_Tech(nullptr)
	, m_WorldViewProj(nullptr)
	, m_Color(nullptr)
	, m_WireframeRS(nullptr)
	, m_VerticesBuffer(nullptr)
	, m_IndicesBuffer(nullptr)
	, m_InputLayout(nullptr)
	, m_MouseDown(false)
	, m_Theta(0)
	, m_Phi(0)
	, m_Radius(250)
	, m_level(0)
	, m_Triangles(0)
{
}

Application::~Application(void)
{
	if (m_WireframeRS)
	{
		m_WireframeRS->Release();
		m_WireframeRS = nullptr;
	}

	if (m_WorldViewProj)
	{
		m_WorldViewProj->Release();
		m_WorldViewProj = nullptr;
	}

	if (m_Color)
	{
		m_Color->Release();
		m_Color = nullptr;
	}

	if (m_Effect)
	{
		m_Effect->Release();
		m_Effect = nullptr;
	}

	if (m_InputLayout)
	{
		m_InputLayout->Release();
		m_InputLayout = nullptr;
	}

	DestroyObjects();
	
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

	
	if (!BuildFX())
		return false;

	BuildVertexLayout();

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_SOLID;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	if(FAILED(GetDevicePtr()->CreateRasterizerState(&wireframeDesc, &m_WireframeRS)))
		return false;
	
	return true;
}

void Application::OnResize()
{
	DirectXApp::OnResize();

	//DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(m_ScreenWidth, m_ScreenHeight, 1.0f, 1000.0f);
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
	float z = m_Radius * cosf(m_Theta) * cosf(m_Phi);
	float y = m_Radius * sinf(m_Theta);


	// Build the view matrix.
	DirectX::XMVECTOR pos    = DirectX::XMVectorSet(x, y, -z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up     = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(pos, target, up);
	DirectX::XMStoreFloat4x4(&m_View, V);

	if (m_ModelObjects.empty() || !m_ModelObjects[0]->IsDynamic())
		return;

	D3D11_MAPPED_SUBRESOURCE mappedData;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem	= m_ModelObjects[0]->GetMeshData()->GetVerticesPtr();

	HRESULT result = m_DeviceContextPtr->Map(m_VerticesBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	// = (md3dImmediateContext->Map(mWavesVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	MeshData::Vertex* v = reinterpret_cast<Vertex*>(mappedData.pData);
	for (auto object = m_ModelObjects.begin(); object != m_ModelObjects.end(); ++object)
	{
		for (UINT i = 0; i < (*object)->GetMeshData()->GetVerticesNumber(); ++i)
		{
			v[i].Position = (*object)->GetMeshData()->m_Vertices[i].Position;
			v[i].Color = (*object)->GetMeshData()->m_Vertices[i].Color;
		}
	}
	m_DeviceContextPtr->Unmap(m_VerticesBuffer, 0);
}

void Application::Draw()
{

	__super::Draw();


	m_Triangles = 0;

	const float color[4] = { 0, 0 ,0, 0};

	m_DeviceContextPtr->ClearRenderTargetView(m_RenderTargetView, color);
	m_DeviceContextPtr->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_DeviceContextPtr->IASetInputLayout(m_InputLayout);
	m_DeviceContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_DeviceContextPtr->RSSetState(m_WireframeRS);

	for (auto object = m_ModelObjects.begin(); object != m_ModelObjects.end(); ++object)
	{

		UINT stride = (*object)->GetMeshData()->GetVertexSize();
		UINT offset = 0;
		m_DeviceContextPtr->IASetVertexBuffers(0, 1, &m_VerticesBuffer, &stride, &offset);
		m_DeviceContextPtr->IASetIndexBuffer(m_IndicesBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set constants
		DirectX::XMMATRIX view  = DirectX::XMLoadFloat4x4(&m_View);
		DirectX::XMMATRIX proj  = DirectX::XMLoadFloat4x4(&m_Proj);
		DirectX::XMMATRIX viewProj = view*proj;
 
		D3DX11_TECHNIQUE_DESC techDesc;
		m_Tech->GetDesc( &techDesc );

		for(UINT p = 0; p < techDesc.Passes; ++p)
		{
			DirectX::XMMATRIX world;// = DirectX::DirectX::XMLoadFloat4x4((*object)->GetWorldMatrix());
			DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0, 0, 0, 1.0);
			//	
			m_WorldViewProj->SetMatrix(reinterpret_cast<float*>(&(world*viewProj)));
			m_Color->SetFloatVector(reinterpret_cast<float*>(&color));
			m_Tech->GetPassByIndex(p)->Apply(0, m_DeviceContextPtr);

			m_DeviceContextPtr->DrawIndexed((*object)->GetIndicesNumber(), (*object)->GetIndicesOffset(), (*object)->GetVerticesOffset()); 

			m_Triangles += (*object)->GetIndicesNumber() / 3; 
		}
	}

	m_SwapChainPtr->Present(0, 0);


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
	}
}

void Application::CreateObject(UINT id)
{
	DestroyObjects();

	if ( id == 1)
		m_ModelObjects.push_back(new Triangle());
	else if ( id == 2)
		m_ModelObjects.push_back(new Pyramid());
	else if ( id == 3)
		m_ModelObjects.push_back(new SierpinskiTriangle(2));
	else if ( id == 4)
		m_ModelObjects.push_back(new SierpinskiPyramid(1));
	else if ( id == 5)
		m_ModelObjects.push_back(new Cylinder(1, 0.5, 1, 20, 4));
	else if ( id == 6)
		m_ModelObjects.push_back(new Geosphere(1,0));
	else if ( id == 7)
		m_ModelObjects.push_back(new Grid(1,1, 50,50));
	else if ( id == 8)
		m_ModelObjects.push_back(new Wave(1,1, 50,50));
	else if ( id == 9)
		m_ModelObjects.push_back(new TestObject("Models/skull.txt"));
		

	BuildGeometryBuffers();
}
void Application::BuildGeometryBuffers()
{	
	if (m_ModelObjects.empty())
		return;


	D3D11_BUFFER_DESC vbd;

	vbd.ByteWidth		= m_ModelObjects[0]->GetMeshData()->GetVertexSize() * m_ModelObjects[0]->GetMeshData()->GetVerticesNumber();
	vbd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	vbd.MiscFlags		= 0;

	if (m_ModelObjects[0]->IsDynamic())
	{
		vbd.Usage			= D3D11_USAGE_DYNAMIC;
		vbd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		vbd.Usage			= D3D11_USAGE_IMMUTABLE;
		vbd.CPUAccessFlags	= 0;
	}
	
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem	= m_ModelObjects[0]->GetMeshData()->GetVerticesPtr();
	GetDevicePtr()->CreateBuffer(&vbd, &vinitData, &m_VerticesBuffer);


	//////////////////////////

	D3D11_BUFFER_DESC ibd;
    ibd.Usage			= D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth		= m_ModelObjects[0]->GetMeshData()->GetIndexSize() * m_ModelObjects[0]->GetMeshData()->GetIndicesNumber();
    ibd.BindFlags		= D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags	= 0;
    ibd.MiscFlags		= 0;

    D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem	= m_ModelObjects[0]->GetMeshData()->GetIndicesPtr();
	GetDevicePtr()->CreateBuffer(&ibd, &iinitData, &m_IndicesBuffer);
}

bool Application::BuildFX()
{
	if(FAILED(CreateEffectFromMemory("FX/color.cso", &m_Effect)))
		return false;

	m_Tech				= m_Effect->GetTechniqueByName("ColorTech");
	m_WorldViewProj		= m_Effect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_Color				= m_Effect->GetVariableByName("gColor")->AsVector();

	return true;
	
}
void Application::BuildVertexLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
    D3DX11_PASS_DESC passDesc;

	m_Tech->GetPassByIndex(0)->GetDesc(&passDesc);

	HRESULT hr = GetDevicePtr()->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, &m_InputLayout);
}