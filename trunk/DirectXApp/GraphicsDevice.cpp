#include "stdafx.h"
#include "GraphicsDevice.h"
#include "RenderState.h"

GraphicsDevice::GraphicsDevice(ID3D11Device* device, ID3D11DeviceContext* context)
: m_Device(device)
, m_Context(context)
{
	RenderState = std::make_unique<RenderStatesManager>();
	RenderState->InitAll(this);
}


GraphicsDevice::~GraphicsDevice()
{
}



void GraphicsDevice::Init()
{

}

