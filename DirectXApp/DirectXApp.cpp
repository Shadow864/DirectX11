#include "stdafx.h"

#include "DirectXApp.h"

#include <sstream>
#include <iostream>
#include <fstream>

#include <d3dcompiler.h>
#include "ContentManager.h"
#include "GraphicsDevice.h"
DirectXApp* app = NULL;

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
#define GET_X_LPARAM(x) ((int)(short)LOWORD(x))
#define GET_Y_LPARAM(x) ((int)(short)HIWORD(x))

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
		}                                                      \
	}
	#endif

#else
	#ifndef HR
#define HR(x) (x);
	#endif
#endif 


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return app->MsgProc(hwnd, msg, wParam, lParam);
}


DirectXApp::DirectXApp(HINSTANCE instance)
	: m_WindowTitle(L"Direct3D 11")
	, m_ScreenWidth(800)
	, m_ScreenHeight(600)
	, m_DevicePtr(nullptr)
	, m_DeviceContextPtr(nullptr)
	, m_SwapChainPtr(nullptr)
	, m_DepthStencilBuffer(nullptr)
	, m_RenderTargetView(nullptr)
	, m_DepthStencilView(nullptr)
	, m_4xMsaaQuality(0)
	, m_AppPaused(false)
{
	ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));
	m_AppInst = instance;
	app = this;
}

DirectXApp::~DirectXApp()
{
	ReleaseCOM(m_SwapChainPtr);
	ReleaseCOM(m_DeviceContextPtr);
	ReleaseCOM(m_DevicePtr);
	ReleaseCOM(m_RenderTargetView);
	ReleaseCOM(m_DepthStencilBuffer);
	ReleaseCOM(m_DepthStencilView);
}

bool DirectXApp::Init()
{
	if (!InitWindow())
		return false;
	if (!InitDirect3D())
		return false;
	if (!InitManagers())
		return false;

	OnResize();


	return true;
}

int DirectXApp::Run()
{ 

	 // Main message loop:
    MSG msg = {0};

	m_Timer.Reset();
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_Timer.Tick();
			//if(!mAppPaused)
			{
				//CalculateFrameStats();
				Update();
				Draw();
			}
			//else
			{
				//Sleep(100);
			}
		}
	}


	return (int) msg.wParam;
}

bool DirectXApp::InitWindow()
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_AppInst;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"D3DWndClassName";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	DWORD       dwExStyle	=	WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // Window Extended Style
	DWORD		dwStyle		=	WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;              // Window Extended Style

	RECT R = { 0, 0, m_ScreenWidth, m_ScreenHeight };
	AdjustWindowRectEx(&R, dwStyle, false, dwExStyle);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	m_MainWnd = CreateWindowEx(
		dwExStyle, 
		L"D3DWndClassName", 
		m_WindowTitle.c_str(), 
		dwStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
        m_ScreenWidth,
		m_ScreenHeight,
		NULL, 
		NULL, 
		m_AppInst,
		NULL);

	
	if( !m_MainWnd )
	{

		DWORD dw = GetLastError(); 
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	

	ShowWindow(m_MainWnd, SW_SHOW);
	UpdateWindow(m_MainWnd);

	return true;
}

bool DirectXApp::InitDirect3D()
{

	if (!Direct3DCreateDeviceAndContext())
		return false;

	if (!Direct3DCheckMSAA())
		return false;

	if (!Direct3DCreateSwapChain())
		return false;
	
	if (!Direct3DCreateRenderTarget())
		return false;

	if (!Direct3DCreateDepthStencilBuffer())
		return false;

	if (!Direct3DBindViewsToOutputMerger())
		return false;

	if (!Direct3dCreateViewport())
		return false;
	
	return true;
}

bool DirectXApp::InitManagers()
{

	m_GraphicsDevice = std::make_shared<GraphicsDevice>(m_DevicePtr, m_DeviceContextPtr);

	
	ContentManager::GetInstance().Initialize(m_DevicePtr);

	return true;
}

bool DirectXApp::Direct3DCreateDeviceAndContext()
{
	D3D_FEATURE_LEVEL		featureLevel;

	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG|D3D11_CREATE_DEVICE_SINGLETHREADED, NULL, 0, D3D11_SDK_VERSION, &m_DevicePtr, &featureLevel, &m_DeviceContextPtr);

	if (FAILED(hr))
		return false;

	if ( featureLevel != D3D_FEATURE_LEVEL_11_0)
		return false;

	return true;
}

bool DirectXApp::Direct3DCheckMSAA()
{
	if (!m_DevicePtr)
		return false;


	HRESULT hr = m_DevicePtr->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);

	if (FAILED(hr))
		return false;

	if (m_4xMsaaQuality <= 0)
		return false;

	return true;
}

bool DirectXApp::Direct3DCreateSwapChain()
{
	if (!m_DevicePtr)
		return false;

	IDXGIDevice*	dxgiDevice	= 0;
	IDXGIAdapter*	dxgiAdapter = 0;
	IDXGIFactory*	dxgiFactory = 0;


	HR(m_DevicePtr->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter),(void**)&dxgiAdapter));
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory),(void**)&dxgiFactory));

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width						= m_ScreenWidth; // use window's client area dims
	sd.BufferDesc.Height					= m_ScreenHeight;
	sd.BufferDesc.RefreshRate.Numerator		= 60;
	sd.BufferDesc.RefreshRate.Denominator	= 1;
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	// Use 4X MSAA?
	//if( mEnable4xMsaa )
	{
		sd.SampleDesc.Count = 4;
		// m4xMsaaQuality is returned via CheckMultisampleQualityLevels().
		sd.SampleDesc.Quality = m_4xMsaaQuality-1;
	}
	/*else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}*/
	
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_MainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HR(dxgiFactory->CreateSwapChain(m_DevicePtr, &sd,&m_SwapChainPtr))
		
	HR(dxgiFactory->MakeWindowAssociation(m_MainWnd, DXGI_MWA_NO_ALT_ENTER))

	auto adapters = EnumerateAdapters(dxgiFactory);
	
	std::wostringstream text;

	text << "*** NUM ADAPTERS = " << adapters.size() << std::endl;

	for (UINT i = 0; i < adapters.size(); ++i)
	{
		if ((adapters[i])->CheckInterfaceSupport(__uuidof(ID3D10Device), NULL) == S_OK)
		{
			text << "*** D3D11 SUPPORTED FOR ADAPTER " << i + 1 << std::endl;
		}
	}

	for (auto cit = adapters.cbegin(); cit != adapters.cend(); ++cit)
	{
		IDXGIOutput* output = NULL;
		
		UINT num_of_outputs = 0;

		for (UINT i = 0; (*cit)->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND; i++)
		{
			++num_of_outputs;
		}
		
		if ( cit == adapters.cbegin())
			text << "*** NUM OUTPUTS FOR DEFAULT ADAPTER = " << num_of_outputs << std::endl;
			
		for (UINT i = 0; (*cit)->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND; i++)
		{

			UINT num_of_display_modes = 0;

			output->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, 0, &num_of_display_modes, NULL);

			DXGI_MODE_DESC* desc = new DXGI_MODE_DESC[num_of_display_modes];

			output->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, 0, &num_of_display_modes, desc);

			

			for (UINT j = 0; j < num_of_display_modes; ++j)
				text << "***WIDTH =" << desc[j].Width << " HEIGHT = " << desc[j].Height << " REFRESH = " << desc[j].RefreshRate.Numerator << "/" << desc[j].RefreshRate.Denominator << std::endl;

			delete desc;

		}
		

		/*if(HR((*cit)->CheckInterfaceSupport(__uuidof(ID3D10Device), &umdVersion)))
		{
			std::cout << "failed";
		}*/
	}

	OutputDebugString(text.str().c_str());


	// Release our acquired COM interfaces (because we are done with them).
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	return true;
}

bool DirectXApp::Direct3DCreateRenderTarget()
{

	if (!m_SwapChainPtr || ! m_DevicePtr)
		return false;

	ID3D11Texture2D* backBuffer;
	m_SwapChainPtr->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	m_DevicePtr->CreateRenderTargetView(backBuffer, 0, &m_RenderTargetView);
	ReleaseCOM(backBuffer);

	return true;
}

bool DirectXApp::Direct3DCreateDepthStencilBuffer()
{


	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width			= m_ScreenWidth;
	depthStencilDesc.Height			= m_ScreenHeight;
	depthStencilDesc.MipLevels		= 1;
	depthStencilDesc.ArraySize		= 1;
	depthStencilDesc.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage			= D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	//depthStencilDesc.
	// Use 4X MSAA? --must match swap chain MSAA values.
	//if( mEnable4xMsaa )
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality-1;
	}
	/*else
	{
		depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	}*/

	if (FAILED(m_DevicePtr->CreateTexture2D(&depthStencilDesc, 0, &m_DepthStencilBuffer)))
		return false;

	if (FAILED(m_DevicePtr->CreateDepthStencilView(m_DepthStencilBuffer, 0, &m_DepthStencilView)))
		return false;


	return true;
}

bool DirectXApp::Direct3DBindViewsToOutputMerger()
{
	m_DeviceContextPtr->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	return true;
}

bool DirectXApp::Direct3dCreateViewport()
{
	if (!m_DeviceContextPtr)
		return false;

	D3D11_VIEWPORT vp;

	vp.TopLeftX			= 0;
	vp.TopLeftY			= 0;
	vp.Width			= (float)m_ScreenWidth;
	vp.Height			= (float)m_ScreenHeight;
	vp.MinDepth			= 0.0f;
	vp.MaxDepth			= 1.0f;

	m_DeviceContextPtr->RSSetViewports(1, &vp);
	
	return true;
}

LRESULT DirectXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
		{
			m_AppPaused = true;
			m_Timer.Stop();
		}
		else
		{
			m_AppPaused = false;
			m_Timer.Start();
		}
		return 0;
	// The WM_MENUCHAR message is sent when a menu is active and the user presses 
	// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
        // Don't beep when we alt-enter.
        return MAKELRESULT(0, MNC_CLOSE);

	// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200; 
		return 0;
	case WM_MOUSEWHEEL:
		OnMouseWheel(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_KEYDOWN:
		OnKeyDown(wParam);
		return 0;

	case WM_KEYUP:
		OnKeyUp(wParam);
		return 0;


	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void DirectXApp::Update()
{

	std::wostringstream outs;   
		outs.precision(6);
		outs << L"Frame Time: " << GetDeltaTime() * 1000.0f << L" (ms)";

	m_WindowTitle = outs.str();
}

void DirectXApp::Draw()
{
	
	SetWindowText(m_MainWnd, m_WindowTitle.c_str());
}

void DirectXApp::OnResize()
{
	
}

std::vector <IDXGIAdapter*> DirectXApp::EnumerateAdapters(IDXGIFactory* factory)
{
    IDXGIAdapter * pAdapter; 
    std::vector <IDXGIAdapter*> vAdapters; 
    

    // Create a DXGIFactory object.
    if(!factory)
    {
        return vAdapters;
    }


    for ( UINT i = 0;
          factory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND;
          ++i )
    {
        vAdapters.push_back(pAdapter); 
    } 

    return vAdapters;

}

