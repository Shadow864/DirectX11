
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <d3d11.h>
#include <string>
#include <vector>
#include <memory>
#include "../Effects11/inc/d3dx11effect.h"
#include "RenderState.h"


#include "GameTimer.h"

class ContentManager;
class GraphicsDevice;

class DirectXApp 
{

public:
	DirectXApp(HINSTANCE instance);
	virtual ~DirectXApp();
	virtual bool	Init();
	virtual int		Run();
	
	virtual void	OnResize();


	virtual void    Update();
	virtual void	Draw();
	
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool InitWindow();
	bool InitDirect3D();
	bool InitManagers();


	bool Direct3DCreateDeviceAndContext();
	bool Direct3DCheckMSAA();
	bool Direct3DCreateSwapChain(); 
	bool Direct3DCreateRenderTarget();
	bool Direct3DCreateDepthStencilBuffer();
	bool Direct3DBindViewsToOutputMerger();
	bool Direct3dCreateViewport();

	virtual void OnMouseDown	(WPARAM btnState, int x, int y)	{ }
	virtual void OnMouseUp		(WPARAM btnState, int x, int y)	{ }
	virtual void OnMouseMove	(WPARAM btnState, int x, int y)	{ }
	virtual void OnMouseWheel	(WPARAM btnState, int x, int y) { }

	virtual void OnKeyUp		(WPARAM btnState) { } 
	virtual void OnKeyDown		(WPARAM btnState) { }

	float GetAspectRatio	() const { return static_cast<float>(m_ScreenWidth)/m_ScreenHeight; }
	float GetDeltaTime		() const { return m_Timer.DeltaTime();	} 
	float GetTime			() const { return m_Timer.GameTime();	}


	std::vector <IDXGIAdapter*> EnumerateAdapters(IDXGIFactory* factory);

	ID3D11Device*			GetDevicePtr()	{ return m_DevicePtr;	}

private:
	
	//Window handlers
	HINSTANCE						m_AppInst;
	HWND							m_MainWnd;
	
	GameTimer						m_Timer;

protected:
	std::wstring					m_WindowTitle;

	//Direct3D 
	ID3D11Device*					m_DevicePtr;
	ID3D11DeviceContext*			m_DeviceContextPtr;
	IDXGISwapChain*					m_SwapChainPtr;

	ID3D11Texture2D*				m_DepthStencilBuffer;
	ID3D11RenderTargetView*			m_RenderTargetView;
	ID3D11DepthStencilView*			m_DepthStencilView;
	D3D11_VIEWPORT					m_ScreenViewport;

	UINT							m_4xMsaaQuality;

	std::shared_ptr<GraphicsDevice>	m_GraphicsDevice;
	
	//
	int								m_ScreenWidth;
	int								m_ScreenHeight; 
	bool							m_AppPaused;




};