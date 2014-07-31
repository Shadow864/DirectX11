#pragma once

#include <vector>
#include <map>
#include "GraphicsCommon.h"

struct ID3D11RasterizerState;
struct ID3D11BlendState;
struct ID3D11DepthStencilState;

class GraphicsDevice;


template <typename TYPE>
class RenderValue
{
	TYPE	value;
	Scope	priority;
public:
	RenderValue()
		: value((TYPE)0)
		, priority(Scope::COUNT)
	{

	}

	void Set(TYPE value, Scope priority)
	{
		if (priority <= this->priority)
		{
			this->value = value;
			this->priority = priority;
		}
	}

	void Reset(Scope priority)
	{
		if (priority <= this->priority)
		{
			this->value = (TYPE)0;
			this->priority = Scope::COUNT;
		}
	}

	TYPE Get() const { return value;  }
};

struct RenderState
{

	RenderValue <FillMode>			FillMode;
	RenderValue <CullFaceMode>		CullMode;
	RenderValue <BlendMode>			BlendMode;
	RenderValue <DepthStencilMode>	DepthStencilMode;
	RenderValue <UINT>				DepthStencilRef;
};

class RenderStatesManager
{
	typedef std::map<std::pair<FillMode, CullFaceMode>, ID3D11RasterizerState*>	RasterizerStates;
	typedef std::map<BlendMode, ID3D11BlendState*>								BlendStates;
	typedef std::map<DepthStencilMode, ID3D11DepthStencilState*>				DepthStencilStates;
public:
	RenderStatesManager();
	~RenderStatesManager();

	void InitAll(GraphicsDevice* device);
	void DestroyAll();

	void Apply(const RenderState& state, Scope scope);
	void Reset(Scope scope);

	void SetFillMode(FillMode fill_mode, Scope scope);
	void SetCulling(CullFaceMode cull_mode, Scope scope);
	void SetRSState();
	
	void SetBlendType(BlendMode type, Scope scope);
	void SetBSState();

	BlendMode GetBSState() const { return m_BlendMode.Get(); }


	void SetDepthStencilMode(DepthStencilMode depth_stencil_mode, UINT depth_stencil_ref, Scope scope);
	void SetDepthStencilMode();
private:
	static ID3D11RasterizerState* SolidRS;
	static ID3D11RasterizerState* WireframeRS;
	static ID3D11RasterizerState* SolidCounterRS;
	static ID3D11RasterizerState* Solid_NoCullRS;
	static ID3D11RasterizerState* Wireframe_NoCullRS;

	static ID3D11BlendState*      AlphaToCoverageBS;
	static ID3D11BlendState*      TransparentBS;
	static ID3D11BlendState*	  AdditiveBS;
	static ID3D11BlendState*	  NoRenderTargetWritesBS;

	static ID3D11DepthStencilState*	NormalDSS;
	static ID3D11DepthStencilState* MirrorDSS;
	static ID3D11DepthStencilState* ReflectionDSS;
	static ID3D11DepthStencilState* ShadowDSS;
	static ID3D11DepthStencilState* ReflectionShadowDSS;

	RenderValue<FillMode>				m_FillMode;
	RenderValue<CullFaceMode>			m_CullMode;
	RenderValue<BlendMode>				m_BlendMode;
	RenderValue<DepthStencilMode>		m_DepthStencilMode;
	RenderValue<UINT>					m_DepthStencilRef;

	RasterizerStates		m_RasterizerStates;
	BlendStates				m_BlendStates;
	DepthStencilStates		m_DepthStencilStates;

	GraphicsDevice*			m_Device;

};


