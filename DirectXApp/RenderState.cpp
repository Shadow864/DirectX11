#include "stdafx.h"
#include "RenderState.h"
#include <d3d11.h>

#include "GraphicsDevice.h"

ID3D11RasterizerState* RenderStatesManager::SolidRS = 0;
ID3D11RasterizerState* RenderStatesManager::WireframeRS = 0;
ID3D11RasterizerState* RenderStatesManager::Solid_NoCullRS = 0;
ID3D11RasterizerState* RenderStatesManager::SolidCounterRS = 0;
ID3D11RasterizerState* RenderStatesManager::Wireframe_NoCullRS = 0;

ID3D11BlendState*      RenderStatesManager::AlphaToCoverageBS = 0;
ID3D11BlendState*      RenderStatesManager::TransparentBS = 0;
ID3D11BlendState*      RenderStatesManager::AdditiveBS = 0;
ID3D11BlendState*	   RenderStatesManager::NoRenderTargetWritesBS = 0;

ID3D11DepthStencilState* RenderStatesManager::NormalDSS = 0;
ID3D11DepthStencilState* RenderStatesManager::MirrorDSS = 0;
ID3D11DepthStencilState* RenderStatesManager::ReflectionDSS = 0;
ID3D11DepthStencilState* RenderStatesManager::ShadowDSS = 0;
ID3D11DepthStencilState* RenderStatesManager::ReflectionShadowDSS = 0;

RenderStatesManager::RenderStatesManager()
{
}


RenderStatesManager::~RenderStatesManager()
{
	DestroyAll();
}


void RenderStatesManager::InitAll(GraphicsDevice* device)
{
	m_Device = device;

	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;
	desc.DepthClipEnable = true;

	if (FAILED(m_Device->m_Device->CreateRasterizerState(&desc, &WireframeRS)))
		return;

	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;
	desc.DepthClipEnable = true;

	if (FAILED(m_Device->m_Device->CreateRasterizerState(&desc, &SolidRS)))
		return;


	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	desc.FrontCounterClockwise = true;
	desc.DepthClipEnable = true;

	if (FAILED(m_Device->m_Device->CreateRasterizerState(&desc, &SolidCounterRS)))
		return;

	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;
	desc.FrontCounterClockwise = false;
	desc.DepthClipEnable = true;

	if (FAILED(m_Device->m_Device->CreateRasterizerState(&desc, &Wireframe_NoCullRS)))
		return;

	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	desc.FrontCounterClockwise = false;
	desc.DepthClipEnable = true;

	if (FAILED(m_Device->m_Device->CreateRasterizerState(&desc, &Solid_NoCullRS)))
		return;


	D3D11_BLEND_DESC blend_desc;
	ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
	blend_desc.AlphaToCoverageEnable = true;
	blend_desc.IndependentBlendEnable = false;
	blend_desc.RenderTarget->BlendEnable = true;
	blend_desc.RenderTarget->SrcBlend = D3D11_BLEND_ONE;
	blend_desc.RenderTarget->DestBlend = D3D11_BLEND_ONE;
	blend_desc.RenderTarget->BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget->SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget->DestBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget->BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget->RenderTargetWriteMask = 0x0f;

	if (FAILED(m_Device->m_Device->CreateBlendState(&blend_desc, &AdditiveBS)))
		return;

	ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
	blend_desc.AlphaToCoverageEnable = true;
	blend_desc.IndependentBlendEnable = false;
	blend_desc.RenderTarget->BlendEnable = true;
	blend_desc.RenderTarget->SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget->DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget->BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget->SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget->DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget->BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget->RenderTargetWriteMask = 0x0f;

	if (FAILED(m_Device->m_Device->CreateBlendState(&blend_desc, &TransparentBS)))
		return;

	ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
	blend_desc.AlphaToCoverageEnable = true;
	blend_desc.IndependentBlendEnable = false;
	blend_desc.RenderTarget->BlendEnable = false;
	blend_desc.RenderTarget->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_Device->m_Device->CreateBlendState(&blend_desc, &AlphaToCoverageBS)))
		return;

	ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
	blend_desc.AlphaToCoverageEnable = false;
	blend_desc.IndependentBlendEnable = false;

	blend_desc.RenderTarget[0].BlendEnable = false;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = 0;

	if (FAILED(m_Device->m_Device->CreateBlendState(&blend_desc, &NoRenderTargetWritesBS)))
		return;

	
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	ZeroMemory(&depth_stencil_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depth_stencil_desc.DepthEnable		= true;
	depth_stencil_desc.DepthFunc		= D3D11_COMPARISON_LESS;
	depth_stencil_desc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;

	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0xff;
	depth_stencil_desc.StencilWriteMask = 0xff;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;


	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	if (FAILED(m_Device->m_Device->CreateDepthStencilState(&depth_stencil_desc, &MirrorDSS)))
		return;

	ZeroMemory(&depth_stencil_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0xff;
	depth_stencil_desc.StencilWriteMask = 0xff;


	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	if (FAILED(m_Device->m_Device->CreateDepthStencilState(&depth_stencil_desc, &ReflectionDSS)))
		return;

	ZeroMemory(&depth_stencil_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depth_stencil_desc.DepthEnable		= true;
	depth_stencil_desc.DepthFunc		= D3D11_COMPARISON_LESS;
	depth_stencil_desc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;

	depth_stencil_desc.StencilEnable		= true;
	depth_stencil_desc.StencilReadMask		= 0xff;
	depth_stencil_desc.StencilWriteMask		= 0xff;

	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;


	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;

	if (FAILED(m_Device->m_Device->CreateDepthStencilState(&depth_stencil_desc, &ShadowDSS)))
		return;


	ZeroMemory(&depth_stencil_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0xff;
	depth_stencil_desc.StencilWriteMask = 0xff;

	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;


	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	if (FAILED(m_Device->m_Device->CreateDepthStencilState(&depth_stencil_desc, &ReflectionShadowDSS)))
		return;

	NormalDSS = nullptr;

	m_RasterizerStates[std::pair<FillMode, CullFaceMode>(FillMode::SOLID,		CullFaceMode::FRONT)] = SolidRS;
	m_RasterizerStates[std::pair<FillMode, CullFaceMode>(FillMode::SOLID,		CullFaceMode::NONE)] = Solid_NoCullRS;
	m_RasterizerStates[std::pair<FillMode, CullFaceMode>(FillMode::SOLID,		CullFaceMode::COUNTER)] = Solid_NoCullRS;
	
	m_RasterizerStates[std::pair<FillMode, CullFaceMode>(FillMode::WIREFRAME,	CullFaceMode::FRONT)] = WireframeRS;
	m_RasterizerStates[std::pair<FillMode, CullFaceMode>(FillMode::WIREFRAME,	CullFaceMode::NONE)] = Wireframe_NoCullRS;

	m_BlendStates[BlendMode::NONE]		= nullptr;
	m_BlendStates[BlendMode::ADDITIVE] = AdditiveBS;
	m_BlendStates[BlendMode::MODULATE] = nullptr;
	m_BlendStates[BlendMode::TRANSPARENT] = TransparentBS;
	m_BlendStates[BlendMode::ALPHA_TO_COVERAGE] = AlphaToCoverageBS;
	m_BlendStates[BlendMode::NO_RENDER_TARGET] = NoRenderTargetWritesBS;

	m_DepthStencilStates[DepthStencilMode::NONE] = NormalDSS;
	m_DepthStencilStates[DepthStencilMode::MIRROR] = MirrorDSS;
	m_DepthStencilStates[DepthStencilMode::REFLECTION] = ReflectionDSS;
	m_DepthStencilStates[DepthStencilMode::SHADOW] = ShadowDSS;
	m_DepthStencilStates[DepthStencilMode::REFLECTION_SHADOW] = ReflectionShadowDSS;

}

void RenderStatesManager::Apply(const RenderState& state, Scope scope)
{
	SetFillMode(state.FillMode.Get(), scope);
	SetCulling(state.CullMode.Get(), scope);
	SetBlendType(state.BlendMode.Get(), scope);
	SetDepthStencilMode(state.DepthStencilMode.Get(), state.DepthStencilRef.Get(), scope);
}
void RenderStatesManager::Reset(Scope scope)
{
	m_FillMode.Reset(scope);
	m_CullMode.Reset(scope);
	m_BlendMode.Reset(scope);
	m_DepthStencilMode.Reset(scope);
	m_DepthStencilRef.Reset(scope);
}

void RenderStatesManager::SetFillMode(FillMode fill_mode, Scope scope)
{
	m_FillMode.Set(fill_mode, scope);

	SetRSState();
}

void RenderStatesManager::SetCulling(CullFaceMode cull_mode, Scope scope)
{
	m_CullMode.Set(cull_mode, scope);

	SetRSState();
}

void RenderStatesManager::SetRSState()
{
	auto key = std::pair<FillMode, CullFaceMode>(m_FillMode.Get(), m_CullMode.Get());

	if (m_RasterizerStates.find(key) != m_RasterizerStates.end())
		m_Device->m_Context->RSSetState(m_RasterizerStates[key]);
}

void RenderStatesManager::SetBlendType(BlendMode type, Scope scope)
{
	m_BlendMode.Set(type, scope);

	SetBSState();
}

void RenderStatesManager::SetBSState()
{
	static const float color[4] = { 0, 0, 0, 0 };

	auto key = m_BlendMode.Get();

	if (m_BlendStates.find(key) != m_BlendStates.end())
		m_Device->m_Context->OMSetBlendState(m_BlendStates[key], color, 0xff);
}

void RenderStatesManager::SetDepthStencilMode(DepthStencilMode depth_stencil_mode, UINT depth_stencil_ref, Scope scope)
{

	m_DepthStencilMode.Set(depth_stencil_mode, scope);
	m_DepthStencilRef.Set(depth_stencil_ref, scope);

	SetDepthStencilMode();
}

void RenderStatesManager::SetDepthStencilMode()
{
	auto key = m_DepthStencilMode.Get();

	if (m_DepthStencilStates.find(key) != m_DepthStencilStates.end())
		m_Device->m_Context->OMSetDepthStencilState(m_DepthStencilStates[key], m_DepthStencilRef.Get());
}

void RenderStatesManager::DestroyAll()
{
	if (WireframeRS)
		WireframeRS->Release();
	WireframeRS = nullptr;

	if (Wireframe_NoCullRS)
		Wireframe_NoCullRS->Release();
	Wireframe_NoCullRS = nullptr;

	if (SolidRS)
		SolidRS->Release();
	SolidRS = nullptr;

	if (Solid_NoCullRS)
		Solid_NoCullRS->Release();
	Solid_NoCullRS = nullptr;

	if (TransparentBS)
		TransparentBS->Release();
	TransparentBS = nullptr;

	if (AlphaToCoverageBS)
		AlphaToCoverageBS->Release();
	AlphaToCoverageBS = nullptr;

	if (NoRenderTargetWritesBS)
		NoRenderTargetWritesBS->Release();
	NoRenderTargetWritesBS = nullptr;

	if (ShadowDSS)
		ShadowDSS->Release();
	ShadowDSS = nullptr;

}